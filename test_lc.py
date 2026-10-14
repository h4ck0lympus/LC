#!/usr/bin/env python3
"""Tests for `lc`. Run: python3 test_lc.py

These never touch the network: the pull/test/push flows run against stubbed
lookup, fetch_question, http_json and poll, and Sandbox makes any unstubbed
http_json call fail the test. The live smoke test (`lc test 27`) remains the
only check against LeetCode itself.
"""

import json
import os
import pty
import re
import select
import shutil
import subprocess
import sys
import time
import tempfile
import unittest
from importlib.machinery import SourceFileLoader
from importlib.util import module_from_spec, spec_from_loader
from pathlib import Path

_path = Path(__file__).resolve().parent / "lc"
_spec = spec_from_loader("lc", SourceFileLoader("lc", str(_path)))
lc = module_from_spec(_spec)
_spec.loader.exec_module(lc)
lc._COLOR = False  # deterministic, uncoloured assertions


class ProblemNumber(unittest.TestCase):
    def test_plain_number(self):
        self.assertEqual(lc.problem_number("547"), "547")

    def test_todo_suffix(self):
        self.assertEqual(lc.problem_number("1021_TODO"), "1021")

    def test_review_suffix(self):
        self.assertEqual(lc.problem_number("28_REVIEW"), "28")

    def test_non_problem_dirs(self):
        for name in ("MISC", "leetcode.h", "", "speedup", "v2_547"):
            self.assertIsNone(lc.problem_number(name), name)


class BuildIndex(unittest.TestCase):
    # Trimmed from a real /api/problems/all/ response.
    PAYLOAD = {"stat_status_pairs": [
        {"stat": {"question_id": 547, "frontend_question_id": 547,
                  "question__title_slug": "number-of-provinces"}},
        # Problems where the internal id differs from the displayed number.
        {"stat": {"question_id": 1176, "frontend_question_id": 1021,
                  "question__title_slug": "remove-outermost-parentheses"}},
        {"stat": {"question_id": 0, "frontend_question_id": None,
                  "question__title_slug": "broken"}},
    ]}

    def test_maps_frontend_number_to_slug_and_internal_id(self):
        index = lc.build_index(self.PAYLOAD)
        self.assertEqual(index["547"], {"slug": "number-of-provinces", "qid": "547"})

    def test_internal_id_is_kept_when_it_differs_from_the_number(self):
        index = lc.build_index(self.PAYLOAD)
        self.assertEqual(index["1021"]["qid"], "1176")

    def test_skips_malformed_entries(self):
        self.assertEqual(len(lc.build_index(self.PAYLOAD)), 2)

    def test_empty_payload(self):
        self.assertEqual(lc.build_index({}), {})


class SplitCases(unittest.TestCase):
    def test_single_argument_per_case(self):
        self.assertEqual(lc.split_cases("[1,2]\n[3,4]", 2), ["[1,2]", "[3,4]"])

    def test_two_arguments_per_case(self):
        self.assertEqual(
            lc.split_cases("[2,7,11,15]\n9\n[3,2,4]\n6", 2),
            ["[2,7,11,15]\n9", "[3,2,4]\n6"])

    def test_uneven_split_falls_back_to_one_block(self):
        self.assertEqual(lc.split_cases("a\nb\nc", 2), ["a\nb\nc"])

    def test_no_answers(self):
        self.assertEqual(lc.split_cases("a\nb", 0), ["a\nb"])


class FormatRun(unittest.TestCase):
    ACCEPTED = {"state": "SUCCESS", "status_code": 10, "status_msg": "Accepted",
                "run_success": True, "correct_answer": True,
                "code_answer": ["2", "3"], "expected_code_answer": ["2", "3"],
                "std_output_list": ["", ""], "total_correct": 2,
                "total_testcases": 2, "status_runtime": "7 ms"}

    WRONG = {"state": "SUCCESS", "status_code": 11, "status_msg": "Wrong Answer",
             "run_success": True, "correct_answer": False,
             "code_answer": ["2", "1"], "expected_code_answer": ["2", "3"],
             "std_output_list": ["", ""], "total_correct": 1,
             "total_testcases": 2, "status_runtime": "7 ms"}

    COMPILE_ERROR = {"state": "SUCCESS", "status_code": 20,
                     "status_msg": "Compile Error", "run_success": False,
                     "compile_error": "line 4: expected ';'",
                     "full_compile_error": "Line 4: Char 9: error: expected ';'"}

    RUNTIME_ERROR = {"state": "SUCCESS", "status_code": 15, "run_success": False,
                     "status_msg": "Runtime Error",
                     "full_runtime_error": "AddressSanitizer: heap-buffer-overflow"}

    def test_accepted_marks_every_case_passing(self):
        out = lc.fmt_run(self.ACCEPTED, "[[1,1]]\n[[1,0]]")
        self.assertEqual(out.count("PASS"), 2)
        self.assertNotIn("FAIL", out)
        self.assertIn("Accepted", out)
        self.assertIn("2/2 cases", out)

    def test_accepted_shows_inputs_and_runtime(self):
        out = lc.fmt_run(self.ACCEPTED, "[[1,1]]\n[[1,0]]")
        self.assertIn("[[1,1]]", out)
        self.assertIn("7 ms", out)

    def test_wrong_answer_flags_only_the_failing_case(self):
        out = lc.fmt_run(self.WRONG, "[[1,1]]\n[[1,0]]")
        self.assertEqual(out.count("PASS"), 1)
        self.assertEqual(out.count("FAIL"), 1)

    def test_wrong_answer_shows_expected_value(self):
        out = lc.fmt_run(self.WRONG, "[[1,1]]\n[[1,0]]")
        self.assertIn("expected  3", out)

    def test_compile_error_reports_compiler_text_only(self):
        out = lc.fmt_run(self.COMPILE_ERROR, "")
        self.assertIn("Compile Error", out)
        self.assertIn("expected ';'", out)
        self.assertNotIn("PASS", out)

    def test_runtime_error_reports_the_trace(self):
        out = lc.fmt_run(self.RUNTIME_ERROR, "[[1,1]]")
        self.assertIn("Runtime Error", out)
        self.assertIn("heap-buffer-overflow", out)

    # Shape of a real response: the judge stops at the crashing case, so
    # code_answer holds only the cases before it, and expected_code_answer
    # carries a trailing empty entry.
    CRASH = {"state": "SUCCESS", "status_code": 15, "run_success": False,
             "status_msg": "Runtime Error", "correct_answer": False,
             "runtime_error": "Line 1263: Char 9: runtime error: addition of "
                              "unsigned offset overflowed (basic_string.h)",
             "full_runtime_error": "Line 1263: Char 9: runtime error: addition of "
                                   "unsigned offset overflowed (basic_string.h)\n"
                                   "SUMMARY: UndefinedBehaviorSanitizer: "
                                   "undefined-behavior basic_string.h:1272:9",
             "code_answer": ['"baabc"'], "std_output_list": ["", "about to crash\n"],
             "expected_code_answer": ['"baabc"', '"az"', '"abaab"', '"kddsbncd"', ""],
             "total_correct": 1, "total_testcases": 4, "status_runtime": "N/A"}
    CRASH_INPUT = '"cbabc"\n"aa"\n"acbbc"\n"leetcode"'

    def test_crash_names_the_case_that_crashed(self):
        out = lc.fmt_run(self.CRASH, self.CRASH_INPUT)
        self.assertIn("PASS  case 1", out)
        self.assertIn("FAIL  case 2  Runtime Error", out)

    def test_crash_shows_input_and_expected_of_crashing_case(self):
        out = lc.fmt_run(self.CRASH, self.CRASH_INPUT)
        crash = out[out.index("case 2"):]
        self.assertIn('input     "aa"', crash)
        self.assertIn('expected  "az"', crash)
        self.assertNotIn("output    ", crash)

    def test_crash_shows_stdout_printed_before_the_crash(self):
        out = lc.fmt_run(self.CRASH, self.CRASH_INPUT)
        self.assertIn("stdout    about to crash", out[out.index("case 2"):])

    def test_crash_lists_the_cases_that_never_ran(self):
        self.assertIn("cases 3-4 not run", lc.fmt_run(self.CRASH, self.CRASH_INPUT))

    def test_crash_on_second_last_case_names_the_single_skipped_case(self):
        r = dict(self.CRASH, code_answer=['"baabc"', '"az"'])
        self.assertIn("case 4 not run", lc.fmt_run(r, self.CRASH_INPUT))

    def test_crash_on_last_case_mentions_nothing_skipped(self):
        r = dict(self.CRASH, code_answer=['"baabc"', '"az"', '"abaab"'])
        out = lc.fmt_run(r, self.CRASH_INPUT)
        self.assertIn("FAIL  case 4  Runtime Error", out)
        self.assertNotIn("not run", out)

    def test_crash_on_first_case(self):
        r = dict(self.CRASH, code_answer=[], std_output_list=[""])
        out = lc.fmt_run(r, self.CRASH_INPUT)
        self.assertNotIn("PASS", out)
        self.assertIn("FAIL  case 1  Runtime Error", out)
        self.assertIn('input     "cbabc"', out)
        self.assertIn("cases 2-4 not run", out)

    def test_crash_keeps_full_trace_and_summary(self):
        out = lc.fmt_run(self.CRASH, self.CRASH_INPUT)
        self.assertIn("SUMMARY: UndefinedBehaviorSanitizer", out)
        self.assertIn("Runtime Error  1/4 cases", out)

    def test_crash_in_library_header_explains_the_line_number(self):
        out = lc.fmt_run(self.CRASH, self.CRASH_INPUT)
        self.assertIn("hint: that line is inside basic_string.h", out)

    def test_crash_in_solution_has_no_library_hint(self):
        r = dict(self.CRASH, full_runtime_error="Line 9: Char 5: runtime error: "
                                                "index -1 out of bounds (Solution.cpp)")
        self.assertNotIn("hint", lc.fmt_run(r, self.CRASH_INPUT))

    def test_lower_case_solution_frame_has_no_library_hint(self):
        # The sanitizer names the user's file in lower case.
        r = dict(self.CRASH, full_runtime_error=(
            "Line 9: Char 23: runtime error: signed integer overflow: "
            "2147483647 + 1 cannot be represented in type 'int' (solution.cpp)\n"
            "SUMMARY: UndefinedBehaviorSanitizer: undefined-behavior "
            "prog_joined.cpp:18:23"))
        self.assertNotIn("hint", lc.fmt_run(r, self.CRASH_INPUT))
        self.assertNotIn("hint", lc.fmt_push(r))

    def test_crash_without_location_has_no_hint(self):
        r = dict(self.CRASH, full_runtime_error="AddressSanitizer: heap-buffer-overflow")
        self.assertNotIn("hint", lc.fmt_run(r, self.CRASH_INPUT))

    def test_short_runtime_error_used_when_full_is_missing(self):
        r = dict(self.CRASH)
        del r["full_runtime_error"]
        self.assertIn("Line 1263", lc.fmt_run(r, self.CRASH_INPUT))

    def test_time_limit_names_the_stalled_case(self):
        r = {"state": "SUCCESS", "status_code": 14, "run_success": False,
             "status_msg": "Time Limit Exceeded", "code_answer": ["1"],
             "expected_code_answer": ["1", "2"], "total_testcases": 2}
        out = lc.fmt_run(r, "[1]\n[2]")
        self.assertIn("FAIL  case 2  Time Limit Exceeded", out)
        self.assertIn("input     [2]", out)
        self.assertNotIn("not run", out)

    def test_crash_with_unsplittable_input_still_reports(self):
        out = lc.fmt_run(self.CRASH, '"cbabc"\n"aa"\n"acbbc"')
        self.assertIn("FAIL  case 2  Runtime Error", out)
        self.assertIn("Line 1263", out)

    def test_crash_without_total_testcases_does_not_crash(self):
        r = dict(self.CRASH)
        del r["total_testcases"]
        out = lc.fmt_run(r, self.CRASH_INPUT)
        self.assertIn("Line 1263", out)
        self.assertNotIn("not run", out)

    # Real clean-run shape: one phantom empty answer past total_testcases,
    # and status_msg "Accepted" although every answer is wrong.
    PHANTOM = {"state": "SUCCESS", "status_code": 10, "run_success": True,
               "status_msg": "Accepted", "correct_answer": False,
               "code_answer": ['"cbabc"', '"aa"', ""],
               "expected_code_answer": ['"baabc"', '"az"', ""],
               "std_output_list": ["", "", ""],
               "total_correct": 0, "total_testcases": 2}

    def test_phantom_trailing_answer_is_not_a_case(self):
        out = lc.fmt_run(self.PHANTOM, '"cbabc"\n"aa"')
        self.assertNotIn("case 3", out)
        self.assertNotIn("PASS", out)

    def test_inputs_split_per_case_despite_phantom_answer(self):
        out = lc.fmt_run(self.PHANTOM, '"cbabc"\n"aa"')
        case2 = out[out.index("case 2"):]
        self.assertIn('input     "aa"', case2)
        self.assertNotIn('"cbabc"', case2.split("output")[0])

    def test_finished_run_with_wrong_answers_says_wrong_answer(self):
        out = lc.fmt_run(self.PHANTOM, '"cbabc"\n"aa"')
        self.assertIn("Wrong Answer  0/2 cases", out)
        self.assertNotIn("Accepted", out)

    def test_correct_run_still_says_accepted(self):
        self.assertIn("Accepted  2/2", lc.fmt_run(self.ACCEPTED, "[[1,1]]\n[[1,0]]"))

    def test_stdout_from_debug_prints_is_shown(self):
        r = dict(self.ACCEPTED, std_output_list=["debug line\n", ""])
        self.assertIn("debug line", lc.fmt_run(r, "[[1,1]]\n[[1,0]]"))

    def test_missing_expected_answers_do_not_crash(self):
        r = dict(self.ACCEPTED, expected_code_answer=[])
        self.assertIn("FAIL", lc.fmt_run(r, "[[1,1]]\n[[1,0]]"))


class FormatSubmit(unittest.TestCase):
    ACCEPTED = {"state": "SUCCESS", "status_code": 10, "status_msg": "Accepted",
                "total_correct": 113, "total_testcases": 113,
                "status_runtime": "12 ms", "runtime_percentile": 85.23,
                "status_memory": "14.2 MB", "memory_percentile": 40.5}

    WRONG = {"state": "SUCCESS", "status_code": 11, "status_msg": "Wrong Answer",
             "total_correct": 3, "total_testcases": 113,
             "last_testcase": "[[1,1,0],[1,1,0],[0,0,1]]",
             "code_answer": "1", "code_output": "1", "expected_output": "2"}

    TLE = {"state": "SUCCESS", "status_code": 14,
           "status_msg": "Time Limit Exceeded",
           "total_correct": 60, "total_testcases": 113,
           "last_testcase": "[[1,0],[0,1]]"}

    def test_accepted_shows_runtime_and_memory_percentiles(self):
        out = lc.fmt_push(self.ACCEPTED)
        self.assertIn("Accepted", out)
        self.assertIn("113/113", out)
        self.assertIn("12 ms", out)
        self.assertIn("beats 85.2%", out)
        self.assertIn("14.2 MB", out)

    def test_accepted_omits_the_failing_case_section(self):
        self.assertNotIn("expected", lc.fmt_push(self.ACCEPTED))

    def test_wrong_answer_shows_the_failing_case(self):
        out = lc.fmt_push(self.WRONG)
        self.assertIn("Wrong Answer", out)
        self.assertIn("3/113", out)
        self.assertIn("[[1,1,0],[1,1,0],[0,0,1]]", out)
        self.assertIn("expected  2", out)

    def test_tle_reports_progress_without_an_output_diff(self):
        out = lc.fmt_push(self.TLE)
        self.assertIn("Time Limit Exceeded", out)
        self.assertIn("60/113", out)
        self.assertNotIn("expected  ", out)

    def test_missing_percentiles_do_not_crash(self):
        r = dict(self.ACCEPTED)
        del r["runtime_percentile"], r["memory_percentile"]
        self.assertIn("12 ms", lc.fmt_push(r))

    def test_compile_error_on_submit(self):
        out = lc.fmt_push({"status_code": 20, "status_msg": "Compile Error",
                             "full_compile_error": "no matching function"})
        self.assertIn("Compile Error", out)
        self.assertIn("no matching function", out)

    def test_runtime_error_on_submit_shows_the_failing_case_and_trace(self):
        out = lc.fmt_push({"status_code": 15, "status_msg": "Runtime Error",
                           "total_correct": 7, "total_testcases": 113,
                           "last_testcase": '"zz"', "expected_output": '"yz"',
                           "std_output": "dbg\n",
                           "full_runtime_error": "Line 3: index out of range "
                                                 "(stl_vector.h)"})
        self.assertIn("7/113", out)
        self.assertIn('input     "zz"', out)
        self.assertIn('expected  "yz"', out)
        self.assertIn("stdout    dbg", out)
        self.assertIn("index out of range", out)
        self.assertIn("inside stl_vector.h", out)


class CleanCredential(unittest.TestCase):
    def test_plain_value_passes_through(self):
        self.assertEqual(lc.clean_credential("eyJhbGciOi"), "eyJhbGciOi")

    def test_strips_surrounding_whitespace(self):
        self.assertEqual(lc.clean_credential("  eyJhbGciOi \n"), "eyJhbGciOi")

    def test_strips_a_pasted_key_prefix(self):
        self.assertEqual(lc.clean_credential("LEETCODE_SESSION=eyJhbGciOi"), "eyJhbGciOi")
        self.assertEqual(lc.clean_credential("csrftoken=8Kd9fXm2"), "8Kd9fXm2")

    def test_strips_quotes_devtools_adds(self):
        self.assertEqual(lc.clean_credential('"eyJhbGciOi"'), "eyJhbGciOi")
        self.assertEqual(lc.clean_credential("'8Kd9fXm2'"), "8Kd9fXm2")

    def test_strips_trailing_cookie_separator(self):
        self.assertEqual(lc.clean_credential("8Kd9fXm2;"), "8Kd9fXm2")

    def test_handles_none_and_empty(self):
        self.assertEqual(lc.clean_credential(None), "")
        self.assertEqual(lc.clean_credential("   "), "")

    def test_does_not_mangle_a_value_containing_equals(self):
        self.assertEqual(lc.clean_credential("abc=def"), "abc=def")


class ParseCredentials(unittest.TestCase):
    EXPECTED = {"LEETCODE_SESSION": "eyJhbGciOi", "csrftoken": "8Kd9fXm2"}

    def test_json_object(self):
        self.assertEqual(
            lc.parse_credentials('{"LEETCODE_SESSION":"eyJhbGciOi","csrftoken":"8Kd9fXm2"}'),
            self.EXPECTED)

    def test_json_with_extra_keys_is_narrowed(self):
        got = lc.parse_credentials(
            '{"LEETCODE_SESSION":"eyJhbGciOi","csrftoken":"8Kd9fXm2","other":"x"}')
        self.assertEqual(got, self.EXPECTED)

    def test_key_equals_value_lines(self):
        self.assertEqual(
            lc.parse_credentials("LEETCODE_SESSION=eyJhbGciOi\ncsrftoken=8Kd9fXm2"),
            self.EXPECTED)

    def test_key_colon_value_lines(self):
        self.assertEqual(
            lc.parse_credentials("LEETCODE_SESSION: eyJhbGciOi\ncsrftoken: 8Kd9fXm2"),
            self.EXPECTED)

    def test_two_bare_lines_in_order(self):
        self.assertEqual(lc.parse_credentials("eyJhbGciOi\n8Kd9fXm2"), self.EXPECTED)

    def test_blank_lines_are_ignored(self):
        self.assertEqual(
            lc.parse_credentials("\n\nLEETCODE_SESSION=eyJhbGciOi\n\ncsrftoken=8Kd9fXm2\n"),
            self.EXPECTED)

    def test_empty_stdin_is_rejected(self):
        with self.assertRaises(lc.Fail) as cm:
            lc.parse_credentials("   \n")
        self.assertIn("Nothing on stdin", str(cm.exception))

    def test_malformed_json_is_rejected(self):
        with self.assertRaises(lc.Fail) as cm:
            lc.parse_credentials('{"LEETCODE_SESSION": ')
        self.assertIn("not valid JSON", str(cm.exception))

    def test_unrecognisable_input_is_rejected(self):
        with self.assertRaises(lc.Fail) as cm:
            lc.parse_credentials("just one line of nonsense")
        self.assertIn("Could not read credentials", str(cm.exception))

    def test_a_session_value_containing_a_colon_survives(self):
        got = lc.parse_credentials("LEETCODE_SESSION=eyJhbGci:OiJ\ncsrftoken=8Kd9fXm2")
        self.assertEqual(got["LEETCODE_SESSION"], "eyJhbGci:OiJ")


class SaveCookies(unittest.TestCase):
    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory()
        self._saved = (lc.CONFIG, lc.COOKIE_FILE)
        lc.CONFIG = Path(self._tmp.name) / "lc"
        lc.COOKIE_FILE = lc.CONFIG / "cookies.json"

    def tearDown(self):
        lc.CONFIG, lc.COOKIE_FILE = self._saved
        self._tmp.cleanup()

    def test_creates_the_directory_and_round_trips(self):
        lc.save_cookies({"LEETCODE_SESSION": "abc", "csrftoken": "xyz"})
        self.assertEqual(lc.load_cookies()["LEETCODE_SESSION"], "abc")

    def test_file_is_not_readable_by_others(self):
        lc.save_cookies({"LEETCODE_SESSION": "abc", "csrftoken": "xyz"})
        self.assertEqual(lc.COOKIE_FILE.stat().st_mode & 0o077, 0)

    def test_overwrites_an_existing_file(self):
        lc.save_cookies({"LEETCODE_SESSION": "old", "csrftoken": "xyz"})
        lc.save_cookies({"LEETCODE_SESSION": "new", "csrftoken": "xyz"})
        self.assertEqual(lc.load_cookies()["LEETCODE_SESSION"], "new")


class LoadCookies(unittest.TestCase):
    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory()
        self._saved = lc.COOKIE_FILE
        lc.COOKIE_FILE = Path(self._tmp.name) / "cookies.json"

    def tearDown(self):
        lc.COOKIE_FILE = self._saved
        self._tmp.cleanup()

    def write(self, text):
        lc.COOKIE_FILE.write_text(text)

    def test_valid_cookies_are_returned(self):
        self.write('{"LEETCODE_SESSION": "abc", "csrftoken": "xyz"}')
        self.assertEqual(lc.load_cookies()["csrftoken"], "xyz")

    def test_missing_file_explains_how_to_create_it(self):
        with self.assertRaises(lc.Fail) as cm:
            lc.load_cookies()
        self.assertIn("No session cookies", str(cm.exception))
        self.assertIn("DevTools", str(cm.exception))

    def test_empty_file_is_named_as_empty(self):
        self.write("")
        with self.assertRaises(lc.Fail) as cm:
            lc.load_cookies()
        self.assertIn("is empty", str(cm.exception))
        self.assertIn("LEETCODE_SESSION", str(cm.exception))

    def test_whitespace_only_file_counts_as_empty(self):
        self.write("\n  \n")
        with self.assertRaises(lc.Fail) as cm:
            lc.load_cookies()
        self.assertIn("is empty", str(cm.exception))

    def test_malformed_json_explains_how_to_fix_it(self):
        self.write("LEETCODE_SESSION=abc")
        with self.assertRaises(lc.Fail) as cm:
            lc.load_cookies()
        self.assertIn("not valid JSON", str(cm.exception))
        self.assertIn("DevTools", str(cm.exception))

    def test_missing_key_is_named(self):
        self.write('{"LEETCODE_SESSION": "abc"}')
        with self.assertRaises(lc.Fail) as cm:
            lc.load_cookies()
        self.assertIn("csrftoken", str(cm.exception))

    def test_blank_value_counts_as_missing(self):
        self.write('{"LEETCODE_SESSION": "abc", "csrftoken": ""}')
        with self.assertRaises(lc.Fail) as cm:
            lc.load_cookies()
        self.assertIn("csrftoken", str(cm.exception))


class PromptMasked(unittest.TestCase):
    """Drive prompt_masked through a real pty, the only way it behaves."""

    def drive(self, keystrokes):
        """Send keystrokes to prompt_masked; return (value, terminal output).

        Waits for the prompt before typing. Without that the writes race the
        child into raw mode, and the tty echoes the input in canonical mode -
        which is a property of the test, not of prompt_masked.
        """
        child = (
            "import sys;"
            "from importlib.machinery import SourceFileLoader;"
            "from importlib.util import module_from_spec, spec_from_loader;"
            "sp = spec_from_loader('lc', SourceFileLoader('lc', %r));"
            "m = module_from_spec(sp); sp.loader.exec_module(m);"
            "sys.stderr.write('VALUE:' + m.prompt_masked('pw: '))" % str(_path))
        master, slave = pty.openpty()
        proc = subprocess.Popen([sys.executable, "-c", child],
                                stdin=slave, stdout=slave,
                                stderr=subprocess.PIPE, close_fds=True)
        os.close(slave)
        self.addCleanup(self._reap, proc, master)

        shown = self._read_until(master, b"pw: ", deadline=time.time() + 10)
        self.assertIn(b"pw: ", shown, "prompt never appeared")
        os.write(master, keystrokes)

        proc.wait(timeout=10)
        stderr = proc.stderr.read().decode()
        shown += self._read_until(master, None, deadline=time.time() + 2)
        value = stderr.split("VALUE:", 1)[1] if "VALUE:" in stderr else None
        return value, shown.decode(errors="replace")

    @staticmethod
    def _read_until(master, needle, deadline):
        """Drain the pty until `needle` appears, or until it goes quiet."""
        buf = b""
        while time.time() < deadline:
            if needle is not None and needle in buf:
                break
            if not select.select([master], [], [], 0.2)[0]:
                if needle is None:
                    break
                continue
            try:
                chunk = os.read(master, 4096)
            except OSError:
                break
            if not chunk:
                break
            buf += chunk
        return buf

    @staticmethod
    def _reap(proc, master):
        if proc.poll() is None:
            proc.kill()
            proc.wait()
        proc.stderr.close()
        os.close(master)

    def test_returns_what_was_typed(self):
        value, _ = self.drive(b"eyJhbGciOi\n")
        self.assertEqual(value, "eyJhbGciOi")

    def test_shows_a_star_per_character_and_never_the_secret(self):
        _, shown = self.drive(b"eyJhbGciOi\n")
        self.assertIn("*" * 10, shown)
        self.assertNotIn("eyJhbGciOi", shown)

    def test_prompt_label_is_visible(self):
        _, shown = self.drive(b"abc\n")
        self.assertIn("pw: ", shown)

    def test_backspace_removes_a_character(self):
        value, _ = self.drive(b"abcX\x7f\n")
        self.assertEqual(value, "abc")

    def test_backspace_on_empty_input_is_harmless(self):
        value, _ = self.drive(b"\x7f\x7fab\n")
        self.assertEqual(value, "ab")

    def test_ctrl_u_clears_the_line(self):
        value, _ = self.drive(b"garbage\x15abc\n")
        self.assertEqual(value, "abc")

    def test_carriage_return_also_submits(self):
        value, _ = self.drive(b"abc\r")
        self.assertEqual(value, "abc")

    def test_empty_input_returns_empty(self):
        value, _ = self.drive(b"\n")
        self.assertEqual(value, "")

    def test_control_characters_are_ignored(self):
        value, _ = self.drive(b"ab\x01\x1bcd\n")
        self.assertEqual(value, "abcd")

    def test_a_long_paste_arrives_intact(self):
        token = "e" * 300
        value, _ = self.drive(token.encode() + b"\n")
        self.assertEqual(value, token)


class Resolve(unittest.TestCase):
    def test_explicit_number(self):
        self.assertEqual(lc.resolve("547"), "547")

    def test_suffixed_directory_name_as_argument(self):
        self.assertEqual(lc.resolve("1021_TODO"), "1021")

    def test_rejects_non_numeric_argument(self):
        with self.assertRaises(lc.Fail):
            lc.resolve("two-sum")


class Todo(unittest.TestCase):
    """Tagging renames problem directories inside a throwaway ROOT."""

    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory()
        self._saved = (lc.ROOT, lc.INDEX_FILE)
        lc.ROOT = Path(self._tmp.name)
        lc.INDEX_FILE = lc.ROOT / "problems.json"
        self._cwd = os.getcwd()

    def tearDown(self):
        os.chdir(self._cwd)
        lc.ROOT, lc.INDEX_FILE = self._saved
        self._tmp.cleanup()

    def mk(self, name, solution="// x\n"):
        d = lc.ROOT / name
        d.mkdir()
        (d / "Solution.cpp").write_text(solution)
        return d

    def names(self):
        return sorted(d.name for d in lc.ROOT.iterdir() if d.is_dir())

    def run_todo(self, *args):
        from io import StringIO
        from contextlib import redirect_stdout
        buf = StringIO()
        with redirect_stdout(buf):
            code = lc.main(["todo", *args])
        return code, buf.getvalue()

    # -- tag_of / problem_dirs

    def test_tag_of(self):
        self.assertEqual(lc.tag_of("1021_TODO"), "TODO")
        self.assertEqual(lc.tag_of("28_REVIEW"), "REVIEW")
        self.assertIsNone(lc.tag_of("547"))

    def test_problem_dirs_lists_plain_first(self):
        self.mk("1021_TODO")
        self.mk("1021")
        self.mk("10210")
        self.assertEqual([d.name for d in lc.problem_dirs("1021")],
                         ["1021", "1021_TODO"])

    def test_problem_dir_still_finds_a_suffixed_dir(self):
        self.mk("24_TODO")
        self.assertEqual(lc.problem_dir("24").name, "24_TODO")

    def test_problem_dir_falls_back_to_the_plain_path(self):
        self.assertEqual(lc.problem_dir("9"), lc.ROOT / "9")

    # -- add / review / done

    def test_add_renames_and_keeps_the_solution(self):
        self.mk("547", "int main;\n")
        self.run_todo("add", "547")
        self.assertEqual(self.names(), ["547_TODO"])
        self.assertEqual((lc.ROOT / "547_TODO" / "Solution.cpp").read_text(),
                         "int main;\n")

    def test_review_retags_a_todo(self):
        self.mk("547_TODO")
        self.run_todo("review", "547")
        self.assertEqual(self.names(), ["547_REVIEW"])

    def test_done_strips_the_tag(self):
        self.mk("28_REVIEW")
        _, out = self.run_todo("done", "28")
        self.assertEqual(self.names(), ["28"])
        self.assertIn("28_REVIEW -> 28", out)

    def test_add_for_an_unpulled_problem_creates_the_dir(self):
        _, out = self.run_todo("add", "42")
        self.assertEqual(self.names(), ["42_TODO"])
        self.assertIn("created", out)

    def test_adding_twice_is_a_no_op(self):
        self.mk("547_TODO")
        _, out = self.run_todo("add", "547")
        self.assertEqual(self.names(), ["547_TODO"])
        self.assertIn("already", out)

    def test_done_on_an_untagged_problem_fails(self):
        self.mk("547")
        with self.assertRaises(lc.Fail) as cm:
            self.run_todo("done", "547")
        self.assertIn("not tagged", str(cm.exception))

    def test_done_on_a_missing_problem_fails(self):
        with self.assertRaises(lc.Fail):
            self.run_todo("done", "547")

    def test_done_refuses_to_overwrite_a_plain_dir(self):
        self.mk("1021")
        self.mk("1021_TODO")
        with self.assertRaises(lc.Fail) as cm:
            self.run_todo("done", "1021")
        self.assertIn("Merge them by hand", str(cm.exception))
        self.assertEqual(self.names(), ["1021", "1021_TODO"])

    def test_add_refuses_when_several_dirs_exist(self):
        self.mk("1021")
        self.mk("1021_REVIEW")
        with self.assertRaises(lc.Fail):
            self.run_todo("add", "1021")
        self.assertEqual(self.names(), ["1021", "1021_REVIEW"])

    def test_number_defaults_to_the_current_directory(self):
        d = self.mk("547")
        os.chdir(d)
        _, out = self.run_todo("add")
        self.assertEqual(self.names(), ["547_TODO"])
        self.assertIn("cd ../547_TODO", out)

    def test_suffixed_argument_is_accepted(self):
        self.mk("547_TODO")
        self.run_todo("done", "547_TODO")
        self.assertEqual(self.names(), ["547"])

    def test_unknown_action_is_rejected(self):
        with self.assertRaises(lc.Fail) as cm:
            self.run_todo("remove", "547")
        self.assertIn("add, review or done", str(cm.exception))

    def test_bare_number_is_rejected_as_an_action(self):
        with self.assertRaises(lc.Fail):
            self.run_todo("547")

    def test_extra_arguments_are_rejected(self):
        with self.assertRaises(lc.Fail):
            self.run_todo("add", "1", "2")

    def test_non_numeric_problem_is_rejected(self):
        with self.assertRaises(lc.Fail):
            self.run_todo("add", "two-sum")

    # -- listing

    def test_empty_list(self):
        self.mk("547")
        _, out = self.run_todo()
        self.assertIn("nothing tagged", out)

    def test_list_groups_by_tag_sorted_numerically(self):
        for name in ("942_TODO", "24_TODO", "28_REVIEW", "547", "5_LATER"):
            self.mk(name)
        (lc.ROOT / "notes_TODO").mkdir()   # not a problem dir
        (lc.ROOT / "7_TODO").write_text("")  # a file, not a dir
        _, out = self.run_todo()
        lines = [l.strip() for l in out.splitlines() if l.strip()]
        self.assertEqual(lines, ["TODO (2)", "24", "942",
                                 "REVIEW (1)", "28",
                                 "LATER (1)", "5"])

    def test_list_shows_slugs_from_the_cache(self):
        self.mk("24_TODO")
        lc.INDEX_FILE.write_text('{"24": {"slug": "swap-nodes-in-pairs", "qid": "24"}}')
        _, out = self.run_todo()
        self.assertIn("swap-nodes-in-pairs", out)

    def test_list_survives_a_corrupt_cache(self):
        self.mk("24_TODO")
        lc.INDEX_FILE.write_text("{not json")
        code, out = self.run_todo()
        self.assertEqual(code, 0)
        self.assertIn("24", out)


# ---------------------------------------------------------------- languages

class Sandbox(unittest.TestCase):
    """Throwaway ROOT and CONFIG, no LC_LANG, and no network.

    Every module attribute a test swaps out is restored afterwards, so a
    stub can never leak into another test.
    """

    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self._tmp.cleanup)
        tmp = Path(self._tmp.name)
        self.patch("ROOT", tmp / "repo")
        lc.ROOT.mkdir()
        self.patch("CONFIG", tmp / "config")
        self.patch("CONFIG_FILE", lc.CONFIG / "config.json")
        self.patch("INDEX_FILE", lc.CONFIG / "problems.json")
        self.patch("COOKIE_FILE", lc.CONFIG / "cookies.json")
        self.set_env(None)
        self.patch("http_json", lambda *a, **k: self.fail("network call in test: %r" % (a,)))
        cwd = os.getcwd()
        self.addCleanup(os.chdir, cwd)
        self.stderr = ""

    def patch(self, name, value):
        saved = getattr(lc, name)
        setattr(lc, name, value)
        self.addCleanup(setattr, lc, name, saved)

    def set_env(self, value):
        saved = os.environ.get("LC_LANG")
        if value is None:
            os.environ.pop("LC_LANG", None)
        else:
            os.environ["LC_LANG"] = value
        self.addCleanup(self._restore_env, saved)

    @staticmethod
    def _restore_env(saved):
        if saved is None:
            os.environ.pop("LC_LANG", None)
        else:
            os.environ["LC_LANG"] = saved

    def mk(self, name, *files, content="// x\n"):
        d = lc.ROOT / name
        d.mkdir(exist_ok=True)
        for f in files:
            (d / f).write_text(content)
        return d

    def run_main(self, *args):
        """Run lc.main; return (exit code, stdout). stderr lands in self.stderr."""
        from io import StringIO
        from contextlib import redirect_stderr, redirect_stdout
        buf, err = StringIO(), StringIO()
        try:
            with redirect_stdout(buf), redirect_stderr(err):
                code = lc.main(list(args))
        finally:
            self.stderr = err.getvalue()
        return code, buf.getvalue()

    def fail_text(self, *args):
        with self.assertRaises(lc.Fail) as cm:
            self.run_main(*args)
        return str(cm.exception)


class LangTable(unittest.TestCase):
    REQUIRED = {"cpp": ".cpp", "c": ".c", "java": ".java", "python3": ".py",
                "rust": ".rs", "go": ".go", "javascript": ".js",
                "typescript": ".ts", "kotlin": ".kt", "swift": ".swift",
                "csharp": ".cs", "ruby": ".rb", "scala": ".scala",
                "php": ".php", "dart": ".dart"}

    def test_covers_every_required_language_with_its_extension(self):
        for lang, ext in self.REQUIRED.items():
            self.assertEqual(lc.LANGS[lang]["ext"], ext, lang)

    def test_extensions_are_unique(self):
        exts = [info["ext"] for info in lc.LANGS.values()]
        self.assertEqual(len(exts), len(set(exts)))

    def test_slugs_are_unique(self):
        slugs = [info["slug"] for info in lc.LANGS.values()]
        self.assertEqual(len(slugs), len(set(slugs)))

    def test_leetcode_slugs_that_differ_from_our_names(self):
        self.assertEqual(lc.LANGS["go"]["slug"], "golang")
        self.assertEqual(lc.LANGS["python3"]["slug"], "python3")
        self.assertEqual(lc.LANGS["csharp"]["slug"], "csharp")
        self.assertEqual(lc.LANGS["cpp"]["slug"], "cpp")

    def test_every_entry_has_the_same_keys(self):
        for lang, info in lc.LANGS.items():
            self.assertEqual(set(info), {"slug", "ext", "label", "check"}, lang)

    def test_checkers_are_known(self):
        for lang, info in lc.LANGS.items():
            self.assertIn(info["check"], (None, "g++", "gcc", "python"), lang)

    def test_aliases_point_at_real_languages(self):
        for alias, target in lc.LANG_ALIASES.items():
            self.assertIn(target, lc.LANGS, alias)

    def test_aliases_never_shadow_a_canonical_name(self):
        for alias, target in lc.LANG_ALIASES.items():
            self.assertTrue(alias not in lc.LANGS or alias == target, alias)

    def test_aliases_are_lower_case(self):
        for alias in lc.LANG_ALIASES:
            self.assertEqual(alias, alias.lower())

    def test_default_is_cpp(self):
        self.assertEqual(lc.DEFAULT_LANG, "cpp")

    def test_solution_name(self):
        self.assertEqual(lc.solution_name("cpp"), "Solution.cpp")
        self.assertEqual(lc.solution_name("python3"), "Solution.py")
        self.assertEqual(lc.solution_name("go"), "Solution.go")

    def test_lang_aliases_lists_spellings(self):
        self.assertEqual(lc.lang_aliases("csharp"), ["cs", "c#"])
        self.assertEqual(lc.lang_aliases("java"), [])


class CanonicalLang(unittest.TestCase):
    CASES = {
        "cpp": "cpp", "c++": "cpp", "cc": "cpp", "CPP": "cpp", "C++": "cpp",
        "c": "c", "C": "c",
        "py": "python3", "python": "python3", "Python3": "python3", "PY": "python3",
        "rs": "rust", "rust": "rust",
        "go": "go", "golang": "go", "GoLang": "go",
        "js": "javascript", "JavaScript": "javascript",
        "ts": "typescript", "kt": "kotlin", "kotlin": "kotlin",
        "cs": "csharp", "c#": "csharp", "C#": "csharp", "csharp": "csharp",
        "rb": "ruby", "swift": "swift", "scala": "scala", "php": "php",
        "dart": "dart", "java": "java",
    }

    def test_names_and_aliases(self):
        for name, want in self.CASES.items():
            with self.subTest(name=name):
                self.assertEqual(lc.canonical_lang(name), want)

    def test_every_canonical_name_resolves_to_itself(self):
        for lang in lc.LANGS:
            self.assertEqual(lc.canonical_lang(lang), lang)

    def test_surrounding_whitespace_is_ignored(self):
        self.assertEqual(lc.canonical_lang("  py \n"), "python3")

    def test_unknown_language_lists_valid_names(self):
        with self.assertRaises(lc.Fail) as cm:
            lc.canonical_lang("cobol")
        msg = str(cm.exception)
        self.assertIn("Unknown language 'cobol'", msg)
        for lang in lc.LANGS:
            self.assertIn(lang, msg)

    def test_the_langslug_golang_is_accepted_but_python2_is_not(self):
        self.assertEqual(lc.canonical_lang("golang"), "go")
        with self.assertRaises(lc.Fail):
            lc.canonical_lang("python2")

    def test_empty_and_none_are_rejected(self):
        for bad in ("", "   ", None):
            with self.assertRaises(lc.Fail):
                lc.canonical_lang(bad)

    def test_extension_with_dot_is_not_a_name(self):
        with self.assertRaises(lc.Fail):
            lc.canonical_lang(".py")


class LangPrecedence(Sandbox):
    def save(self, text):
        lc.CONFIG.mkdir(parents=True, exist_ok=True)
        lc.CONFIG_FILE.write_text(text)

    def test_builtin_default_when_nothing_is_set(self):
        self.assertEqual(lc.default_lang(), ("cpp", "built-in default"))

    def test_config_beats_builtin(self):
        self.save('{"lang": "rust"}')
        self.assertEqual(lc.default_lang(), ("rust", str(lc.CONFIG_FILE)))

    def test_config_value_may_be_an_alias(self):
        self.save('{"lang": "PY"}')
        self.assertEqual(lc.default_lang()[0], "python3")

    def test_env_beats_config(self):
        self.save('{"lang": "rust"}')
        self.set_env("go")
        self.assertEqual(lc.default_lang(), ("go", "LC_LANG"))

    def test_env_alias_is_resolved(self):
        self.set_env("c#")
        self.assertEqual(lc.default_lang()[0], "csharp")

    def test_flag_beats_env_and_config(self):
        self.save('{"lang": "rust"}')
        self.set_env("go")
        self.assertEqual(lc.resolve_lang("py"), ("python3", "--lang"))

    def test_no_flag_falls_through_to_default(self):
        self.set_env("java")
        self.assertEqual(lc.resolve_lang(None), ("java", "LC_LANG"))

    def test_empty_env_is_treated_as_unset(self):
        self.save('{"lang": "rust"}')
        self.set_env("")
        self.assertEqual(lc.default_lang()[0], "rust")

    def test_whitespace_env_is_treated_as_unset(self):
        self.set_env("   ")
        self.assertEqual(lc.default_lang()[0], "cpp")

    def test_invalid_env_is_an_error(self):
        self.set_env("cobol")
        with self.assertRaises(lc.Fail) as cm:
            lc.default_lang()
        self.assertIn("cobol", str(cm.exception))

    def test_invalid_env_error_names_lc_lang_and_the_choices(self):
        self.set_env("pyhton")
        with self.assertRaises(lc.Fail) as cm:
            lc.default_lang()
        msg = str(cm.exception)
        self.assertIn("$LC_LANG='pyhton'", msg)
        self.assertIn("unset it", msg)
        for lang in lc.LANGS:
            self.assertIn(lang, msg)

    def test_invalid_env_is_ignored_when_flag_given(self):
        self.set_env("cobol")
        self.assertEqual(lc.resolve_lang("rs"), ("rust", "--lang"))

    def test_env_lang_unset_is_none(self):
        self.assertIsNone(lc.env_lang())

    def test_invalid_flag_is_an_error_even_with_valid_env(self):
        self.set_env("go")
        with self.assertRaises(lc.Fail):
            lc.resolve_lang("cobol")

    def test_corrupt_config_counts_as_no_default(self):
        self.save("{not json")
        self.assertEqual(lc.default_lang(), ("cpp", "built-in default"))

    def test_empty_config_counts_as_no_default(self):
        self.save("")
        self.assertEqual(lc.default_lang()[0], "cpp")

    def test_non_object_config_counts_as_no_default(self):
        self.save('["rust"]')
        self.assertEqual(lc.default_lang()[0], "cpp")

    def test_unknown_lang_in_config_counts_as_no_default(self):
        self.save('{"lang": "cobol"}')
        self.assertEqual(lc.default_lang()[0], "cpp")

    def test_non_string_lang_in_config_counts_as_no_default(self):
        self.save('{"lang": 3}')
        self.assertEqual(lc.default_lang()[0], "cpp")

    def test_binary_garbage_config_counts_as_no_default(self):
        lc.CONFIG.mkdir(parents=True, exist_ok=True)
        lc.CONFIG_FILE.write_bytes(b"\xff\xfe\x00garbage")
        self.assertEqual(lc.default_lang()[0], "cpp")

    def test_config_that_is_a_directory_counts_as_no_default(self):
        lc.CONFIG_FILE.mkdir(parents=True)
        self.assertEqual(lc.load_config(), {})


class SaveConfig(Sandbox):
    def test_creates_the_directory(self):
        lc.save_config({"lang": "go"})
        self.assertEqual(lc.load_config(), {"lang": "go"})

    def test_keeps_other_keys(self):
        lc.CONFIG.mkdir(parents=True)
        lc.CONFIG_FILE.write_text('{"other": 1, "lang": "cpp"}')
        lc.save_config({"lang": "go"})
        self.assertEqual(lc.load_config(), {"other": 1, "lang": "go"})

    def test_replaces_a_corrupt_file(self):
        lc.CONFIG.mkdir(parents=True)
        lc.CONFIG_FILE.write_text("{nope")
        lc.save_config({"lang": "go"})
        self.assertEqual(lc.load_config(), {"lang": "go"})

    def test_leaves_no_temp_file(self):
        lc.save_config({"lang": "go"})
        self.assertEqual([p.name for p in lc.CONFIG.iterdir()], ["config.json"])

    def test_replaces_a_non_object_file(self):
        lc.CONFIG.mkdir(parents=True)
        lc.CONFIG_FILE.write_text('["x"]')
        lc.save_config({"lang": "go"})
        self.assertEqual(json.loads(lc.CONFIG_FILE.read_text()), {"lang": "go"})

    def test_unwritable_target_is_a_fail_and_leaves_no_temp_file(self):
        lc.CONFIG_FILE.mkdir(parents=True)  # config.json is a directory
        with self.assertRaises(lc.Fail) as cm:
            lc.save_config({"lang": "go"})
        self.assertIn("Could not save", str(cm.exception))
        self.assertIn(str(lc.CONFIG_FILE), str(cm.exception))
        self.assertEqual([p.name for p in lc.CONFIG.iterdir()], ["config.json"])

    def test_unwritable_directory_is_a_fail(self):
        # CONFIG's parent is a file, so mkdir cannot create it.
        blocker = lc.ROOT / "blocker"
        blocker.write_text("")
        self.patch("CONFIG_FILE", blocker / "lc" / "config.json")
        with self.assertRaises(lc.Fail) as cm:
            lc.save_config({"lang": "go"})
        self.assertIn("Could not save", str(cm.exception))

    def test_lang_command_reports_a_failed_save(self):
        lc.CONFIG_FILE.mkdir(parents=True)
        self.assertIn("Could not save", self.fail_text("lang", "py"))


class LangCommand(Sandbox):
    def test_show_builtin_default(self):
        code, out = self.run_main("lang")
        self.assertEqual(code, 0)
        self.assertIn("default language cpp", out)
        self.assertIn("from built-in default", out)

    def test_show_lists_every_language_and_file(self):
        _, out = self.run_main("lang")
        rows = [line.split() for line in out.splitlines()]
        for lang in lc.LANGS:
            with self.subTest(lang=lang):
                self.assertTrue(any(lang in r and lc.solution_name(lang) in r
                                    for r in rows), out)

    def test_show_has_exactly_one_row_per_language(self):
        _, out = self.run_main("lang")
        names = [m.group(1) for m in
                 re.finditer(r"^\s*\*?\s+(\S+)\s+Solution\.\S+", out, re.M)]
        self.assertEqual(names, list(lc.LANGS))

    def test_show_marks_an_alias_saved_in_config(self):
        lc.CONFIG.mkdir(parents=True)
        lc.CONFIG_FILE.write_text('{"lang": "PY"}')
        _, out = self.run_main("lang")
        marked = [l for l in out.splitlines() if l.strip().startswith("*")]
        self.assertEqual(len(marked), 1)
        self.assertEqual(marked[0].split()[1], "python3")

    def test_show_lists_aliases(self):
        _, out = self.run_main("lang")
        self.assertIn("py, python", out)
        self.assertIn("cs, c#", out)

    def test_show_marks_the_current_language(self):
        self.set_env("rust")
        _, out = self.run_main("lang")
        marked = [l for l in out.splitlines() if l.strip().startswith("*")]
        self.assertEqual(len(marked), 1)
        self.assertIn("rust", marked[0])

    def test_show_names_env_as_the_source(self):
        self.set_env("go")
        _, out = self.run_main("lang")
        self.assertIn("default language go", out)
        self.assertIn("from LC_LANG", out)

    def test_set_persists_and_show_reports_it(self):
        code, out = self.run_main("lang", "py")
        self.assertEqual(code, 0)
        self.assertIn("python3", out)
        self.assertEqual(lc.load_config(), {"lang": "python3"})
        _, out = self.run_main("lang")
        self.assertIn("default language python3", out)
        self.assertIn(f"from {lc.CONFIG_FILE}", out)

    def test_set_stores_the_canonical_name(self):
        self.run_main("lang", "GOLANG")
        self.assertEqual(lc.load_config()["lang"], "go")

    def test_set_overwrites_previous_default(self):
        self.run_main("lang", "rust")
        self.run_main("lang", "java")
        self.assertEqual(lc.default_lang()[0], "java")

    def test_set_rejects_unknown_language_and_saves_nothing(self):
        msg = self.fail_text("lang", "cobol")
        self.assertIn("Unknown language", msg)
        self.assertFalse(lc.CONFIG_FILE.exists())

    def test_set_warns_when_env_overrides(self):
        self.set_env("go")
        _, out = self.run_main("lang", "rust")
        self.assertIn("LC_LANG=go overrides", out)

    def test_set_does_not_warn_without_env(self):
        _, out = self.run_main("lang", "rust")
        self.assertNotIn("overrides", out)

    def test_extra_arguments_are_rejected(self):
        self.assertIn("at most one", self.fail_text("lang", "py", "go"))

    def test_options_are_rejected(self):
        self.assertIn("Unknown option", self.fail_text("lang", "--reset"))

    def test_show_survives_corrupt_config(self):
        lc.CONFIG.mkdir(parents=True)
        lc.CONFIG_FILE.write_text("{nope")
        code, out = self.run_main("lang")
        self.assertEqual(code, 0)
        self.assertIn("default language cpp", out)

    def test_show_with_bad_env_reports_it_and_still_lists(self):
        self.set_env("cobol")
        code, out = self.run_main("lang")
        self.assertEqual(code, 1)
        self.assertIn("$LC_LANG='cobol'", out)
        self.assertIn("Solution.py", out)
        self.assertFalse([l for l in out.splitlines() if l.strip().startswith("*")])

    def test_set_works_with_bad_env_and_warns(self):
        self.set_env("cobol")
        code, out = self.run_main("lang", "rust")
        self.assertEqual(code, 0)
        self.assertEqual(lc.load_config(), {"lang": "rust"})
        self.assertIn("LC_LANG=cobol overrides", out)

    def test_help_flag_prints_usage(self):
        code, out = self.run_main("lang", "--help")
        self.assertEqual(code, 0)
        self.assertIn("lc lang <name>", out)


class ParseLangArgs(unittest.TestCase):
    def parse(self, *args, cmd="test"):
        return lc.parse_lang_args(cmd, list(args))

    def fails(self, *args, cmd="test"):
        with self.assertRaises(lc.Fail) as cm:
            self.parse(*args, cmd=cmd)
        return str(cm.exception)

    def test_nothing(self):
        self.assertEqual(self.parse(), (None, None))

    def test_number_only(self):
        self.assertEqual(self.parse("547"), ("547", None))

    def test_flag_after_number(self):
        self.assertEqual(self.parse("547", "--lang", "py"), ("547", "python3"))

    def test_flag_before_number(self):
        self.assertEqual(self.parse("--lang", "py", "547"), ("547", "python3"))

    def test_short_flag(self):
        self.assertEqual(self.parse("547", "-l", "rs"), ("547", "rust"))

    def test_equals_form(self):
        self.assertEqual(self.parse("--lang=go", "547"), ("547", "go"))

    def test_flag_without_number(self):
        self.assertEqual(self.parse("--lang", "java"), (None, "java"))

    def test_symbolic_names_survive(self):
        self.assertEqual(self.parse("547", "--lang", "c++"), ("547", "cpp"))
        self.assertEqual(self.parse("547", "--lang=c#"), ("547", "csharp"))

    def test_missing_value_at_end(self):
        self.assertIn("--lang needs a language", self.fails("547", "--lang"))

    def test_missing_value_for_short_flag(self):
        self.assertIn("-l needs a language", self.fails("547", "-l"))

    def test_value_that_looks_like_a_flag(self):
        self.assertIn("needs a language", self.fails("--lang", "--lang", "py"))

    def test_empty_equals_value(self):
        self.assertIn("--lang= needs a language", self.fails("547", "--lang="))

    def test_unknown_language(self):
        self.assertIn("Unknown language 'cobol'", self.fails("547", "--lang", "cobol"))

    def test_duplicate_flag(self):
        self.assertIn("more than once", self.fails("--lang", "py", "547", "-l", "go"))

    def test_duplicate_flag_even_when_equal(self):
        self.assertIn("more than once", self.fails("--lang=py", "--lang", "py"))

    def test_unknown_long_flag(self):
        msg = self.fails("547", "--verbose")
        self.assertIn("Unknown option '--verbose'", msg)
        self.assertIn("lc test", msg)

    def test_unknown_short_flag(self):
        self.assertIn("Unknown option '-x'", self.fails("-x", "547", cmd="push"))

    def test_lookalike_flag_is_unknown(self):
        self.assertIn("Unknown option '--language'", self.fails("--language", "py"))

    def test_two_numbers(self):
        self.assertIn("one problem number", self.fails("1", "2"))

    def test_error_names_the_command(self):
        self.assertIn("lc pull", self.fails("--lang", cmd="pull"))

    def test_unknown_option_lists_every_accepted_form(self):
        msg = self.fails("--verbose")
        for form in ("--lang L", "-l L", "--lang=L"):
            self.assertIn(form, msg)

    def test_short_equals_form_is_not_supported(self):
        self.assertIn("Unknown option '-l=py'", self.fails("547", "-l=py"))

    def test_forgotten_language_swallows_the_number(self):
        # `lc test -l 547`: the number is taken as the language.
        self.assertIn("Unknown language '547'", self.fails("-l", "547"))

    def test_suffixed_number_passes_through(self):
        self.assertEqual(self.parse("547_TODO", "--lang", "py"), ("547_TODO", "python3"))


class Detection(Sandbox):
    def test_no_files_says_pull(self):
        self.mk("547", "tests.txt")
        with self.assertRaises(lc.Fail) as cm:
            lc.read_solution("547")
        self.assertIn("Run `lc pull 547` first", str(cm.exception))

    def test_missing_dir_says_pull(self):
        with self.assertRaises(lc.Fail) as cm:
            lc.read_solution("547")
        self.assertIn("lc pull 547", str(cm.exception))

    def test_single_file_is_used(self):
        self.mk("547", "Solution.py", content="x = 1\n")
        lang, path, code = lc.read_solution("547")
        self.assertEqual((lang, path.name, code), ("python3", "Solution.py", "x = 1\n"))

    def test_single_file_wins_over_a_different_default(self):
        self.set_env("rust")
        self.mk("547", "Solution.go")
        self.assertEqual(lc.read_solution("547")[0], "go")

    def test_single_file_in_a_suffixed_dir(self):
        self.mk("547_TODO", "Solution.rs")
        lang, path, _ = lc.read_solution("547")
        self.assertEqual(lang, "rust")
        self.assertEqual(path.parent.name, "547_TODO")

    def read(self, *args):
        """read_solution with stderr captured into self.stderr."""
        from io import StringIO
        from contextlib import redirect_stderr
        err = StringIO()
        with redirect_stderr(err):
            result = lc.read_solution(*args)
        self.stderr = err.getvalue()
        return result

    def test_several_files_prefer_the_default(self):
        self.mk("547", "Solution.cpp", "Solution.py")
        self.assertEqual(lc.read_solution("547")[0], "cpp")

    def test_tie_broken_by_default_names_the_chosen_file(self):
        self.mk("547", "Solution.cpp", "Solution.py", "Solution.go")
        self.read("547")
        self.assertIn("using Solution.cpp", self.stderr)
        self.assertIn("also Solution.py, Solution.go", self.stderr)
        self.assertIn("--lang", self.stderr)

    def test_single_file_prints_no_notice(self):
        self.mk("547", "Solution.py")
        self.read("547")
        self.assertEqual(self.stderr, "")

    def test_explicit_flag_prints_no_notice(self):
        self.mk("547", "Solution.cpp", "Solution.py")
        self.read("547", "python3")
        self.assertEqual(self.stderr, "")

    def test_bad_env_does_not_matter_with_a_single_file(self):
        self.set_env("cobol")
        self.mk("547", "Solution.py")
        self.assertEqual(self.read("547")[0], "python3")

    def test_bad_env_with_several_files_fails_naming_it(self):
        self.set_env("cobol")
        self.mk("547", "Solution.cpp", "Solution.py")
        with self.assertRaises(lc.Fail) as cm:
            self.read("547")
        self.assertIn("LC_LANG='cobol'", str(cm.exception))

    def test_bad_env_does_not_matter_with_a_flag(self):
        self.set_env("cobol")
        self.mk("547", "Solution.cpp", "Solution.py")
        self.assertEqual(self.read("547", "python3")[0], "python3")

    def test_several_files_prefer_the_env_default(self):
        self.set_env("py")
        self.mk("547", "Solution.cpp", "Solution.py")
        self.assertEqual(lc.read_solution("547")[0], "python3")

    def test_several_files_prefer_the_config_default(self):
        lc.save_config({"lang": "java"})
        self.mk("547", "Solution.cpp", "Solution.java")
        self.assertEqual(lc.read_solution("547")[0], "java")

    def test_several_files_without_the_default_fail_listing_them(self):
        self.mk("547", "Solution.py", "Solution.go")
        with self.assertRaises(lc.Fail) as cm:
            lc.read_solution("547")
        msg = str(cm.exception)
        self.assertIn("Several solutions", msg)
        self.assertIn("Solution.py", msg)
        self.assertIn("Solution.go", msg)
        self.assertIn("--lang", msg)

    def test_flag_picks_among_several(self):
        self.mk("547", "Solution.cpp", "Solution.py")
        self.assertEqual(lc.read_solution("547", "python3")[0], "python3")

    def test_flag_overrides_a_lone_other_file(self):
        self.mk("547", "Solution.cpp")
        with self.assertRaises(lc.Fail) as cm:
            lc.read_solution("547", "python3")
        msg = str(cm.exception)
        self.assertIn("Solution.py does not exist", msg)
        self.assertIn("lc pull 547 --lang python3", msg)

    def test_flag_with_no_files_suggests_pull_with_lang(self):
        self.mk("547")
        with self.assertRaises(lc.Fail) as cm:
            lc.read_solution("547", "rust")
        self.assertIn("lc pull 547 --lang rust", str(cm.exception))

    def test_unrelated_files_are_ignored(self):
        self.mk("547", "Solution.cpp", "Solution.h", "notes.py", "solution.py",
                "tests.txt")
        self.assertEqual(lc.read_solution("547")[0], "cpp")

    def test_a_directory_named_like_a_solution_is_ignored(self):
        d = self.mk("547", "Solution.py")
        (d / "Solution.go").mkdir()
        self.assertEqual(lc.read_solution("547")[0], "python3")

    def test_empty_file_is_rejected(self):
        self.mk("547", "Solution.py", content="  \n")
        with self.assertRaises(lc.Fail) as cm:
            lc.read_solution("547")
        self.assertIn("is empty", str(cm.exception))

    def test_solution_files_is_in_table_order(self):
        d = self.mk("547", "Solution.dart", "Solution.c", "Solution.cpp")
        self.assertEqual(list(lc.solution_files(d)), ["cpp", "c", "dart"])


class SyntaxCheck(Sandbox):
    def write(self, name, code):
        p = lc.ROOT / name
        p.write_text(code)
        return p

    def test_valid_python_passes(self):
        lc.syntax_check(self.write("Solution.py",
                                   "class Solution:\n    def f(self) -> int:\n"
                                   "        return 1\n"), "python3")

    def test_python_error_reports_line_and_message(self):
        p = self.write("Solution.py", "class Solution:\n    def f(self)\n        pass\n")
        with self.assertRaises(lc.Fail) as cm:
            lc.syntax_check(p, "python3")
        msg = str(cm.exception)
        self.assertIn("Syntax error", msg)
        self.assertIn("(local python)", msg)
        self.assertIn("Solution.py, line 2", msg)
        self.assertIn("def f(self)", msg)

    def test_python_indentation_error(self):
        p = self.write("Solution.py", "def f():\nreturn 1\n")
        with self.assertRaises(lc.Fail) as cm:
            lc.syntax_check(p, "python3")
        self.assertIn("line 2", str(cm.exception))

    def test_python_null_byte_is_reported_not_raised(self):
        p = self.write("Solution.py", "x = 1\x00\n")
        with self.assertRaises(lc.Fail):
            lc.syntax_check(p, "python3")

    def test_python_value_error_is_reported(self):
        # Older Pythons raise ValueError (not SyntaxError) for null bytes.
        def compile(*a, **k):
            raise ValueError("source code string cannot contain null bytes")
        lc.compile = compile  # shadows the builtin inside lc only
        self.addCleanup(delattr, lc, "compile")
        with self.assertRaises(lc.Fail) as cm:
            lc.syntax_check(self.write("Solution.py", "x = 1\n"), "python3")
        msg = str(cm.exception)
        self.assertIn("Syntax error", msg)
        self.assertIn("Solution.py: source code string cannot contain null bytes", msg)

    def test_python_check_runs_no_subprocess(self):
        def boom(*a, **k):
            raise AssertionError("subprocess used")
        self.patch("subprocess", type("S", (), {"run": staticmethod(boom)}))
        lc.syntax_check(self.write("Solution.py", "x = 1\n"), "python3")

    def test_python_name_errors_are_left_to_the_judge(self):
        # List is undefined without an import, but that is a runtime matter.
        lc.syntax_check(self.write("Solution.py",
                                   "def f(x: List[int]) -> int:\n    return 0\n"),
                        "python3")

    def test_languages_without_a_checker_are_skipped(self):
        def boom(*a, **k):
            raise AssertionError("checker ran")
        self.patch("subprocess", type("S", (), {"run": staticmethod(boom)}))
        unchecked = [l for l, info in lc.LANGS.items() if info["check"] is None]
        self.assertGreaterEqual(len(unchecked), 12)
        for lang in unchecked:
            with self.subTest(lang=lang):
                lc.syntax_check(self.write(lc.solution_name(lang),
                                           "this is { not code"), lang)

    def capture_cmd(self, returncode=0, stderr=""):
        calls = []

        def run(cmd, **kw):
            calls.append(cmd)
            return subprocess.CompletedProcess(cmd, returncode, "", stderr)
        self.patch("subprocess", type("S", (), {"run": staticmethod(run)}))
        return calls

    def test_cpp_uses_gxx(self):
        calls = self.capture_cmd()
        p = self.write("Solution.cpp", "int x;\n")
        lc.syntax_check(p, "cpp")
        self.assertEqual(calls, [["g++", "-std=c++23", "-fsyntax-only", str(p)]])

    def test_cpp_is_the_default(self):
        calls = self.capture_cmd()
        lc.syntax_check(self.write("Solution.cpp", "int x;\n"))
        self.assertEqual(calls[0][0], "g++")

    def test_cpp_includes_leetcode_h_when_present(self):
        (lc.ROOT / "leetcode.h").write_text("")
        calls = self.capture_cmd()
        p = self.write("Solution.cpp", "int x;\n")
        lc.syntax_check(p, "cpp")
        self.assertEqual(calls, [["g++", "-std=c++23", "-fsyntax-only",
                                  "-include", str(lc.ROOT / "leetcode.h"), str(p)]])

    def test_c_uses_gcc_with_standard_headers(self):
        calls = self.capture_cmd()
        (lc.ROOT / "leetcode.h").write_text("")  # C must not pick it up
        p = self.write("Solution.c", "int x;\n")
        lc.syntax_check(p, "c")
        cmd = calls[0]
        self.assertEqual(cmd[:3], ["gcc", "-std=gnu17", "-fsyntax-only"])
        self.assertEqual(cmd[-1], str(p))
        self.assertEqual(cmd[3:-1], [x for h in lc.C_PRELUDE for x in ("-include", h)])
        for h in ("stdlib.h", "stdbool.h", "string.h"):
            self.assertIn(h, lc.C_PRELUDE)

    def test_compiler_error_is_reported_with_the_tool(self):
        self.capture_cmd(returncode=1, stderr="Solution.c:1: error: boom\n")
        with self.assertRaises(lc.Fail) as cm:
            lc.syntax_check(self.write("Solution.c", "int x\n"), "c")
        self.assertIn("(local gcc)", str(cm.exception))
        self.assertIn("error: boom", str(cm.exception))

    def test_missing_toolchain_is_skipped(self):
        def missing(*a, **k):
            raise FileNotFoundError("gcc")
        self.patch("subprocess", type("S", (), {"run": staticmethod(missing)}))
        lc.syntax_check(self.write("Solution.c", "int x\n"), "c")
        lc.syntax_check(self.write("Solution.cpp", "int x\n"), "cpp")


class RuntimeHintFilename(unittest.TestCase):
    CRASH = FormatRun.CRASH

    def trace(self, where):
        return dict(self.CRASH, full_runtime_error=f"Line 9: boom ({where})")

    def test_default_filename_is_solution_cpp(self):
        self.assertNotIn("hint", lc.fmt_run(self.trace("Solution.cpp"), ""))
        self.assertNotIn("hint", lc.fmt_push(self.trace("Solution.cpp")))

    def test_own_file_in_another_language_has_no_hint(self):
        self.assertNotIn("hint", lc.fmt_run(self.trace("Solution.java"), "",
                                            "Solution.java"))
        self.assertNotIn("hint", lc.fmt_push(self.trace("Solution.py"), "Solution.py"))

    def test_no_hint_outside_c_and_cpp(self):
        for name in ("Solution.rs", "Solution.py", "Solution.java", "Solution.go"):
            with self.subTest(name=name):
                self.assertNotIn("hint", lc.fmt_run(self.trace("Solution.cpp"), "", name))
                self.assertNotIn("hint", lc.fmt_push(self.trace("basic_string.h"), name))

    def test_library_frame_still_hinted_for_c(self):
        self.assertIn("inside basic_string.h",
                      lc.fmt_push(self.trace("basic_string.h"), "Solution.c"))
        self.assertIn("inside stdlib.h",
                      lc.fmt_run(self.trace("stdlib.h"), "", "Solution.c"))

    def test_own_file_compared_case_insensitively(self):
        self.assertNotIn("hint", lc.fmt_run(self.trace("solution.cpp"), ""))
        self.assertNotIn("hint", lc.fmt_push(self.trace("SOLUTION.C"), "Solution.c"))

    def test_c_solution_frame_is_foreign_to_cpp(self):
        self.assertIn("inside solution.c",
                      lc.fmt_run(self.trace("solution.c"), "", "Solution.cpp"))


class Pull(Sandbox):
    QUESTION = {
        "title": "Number of Provinces", "titleSlug": "number-of-provinces",
        "exampleTestcaseList": ["[[1,1,0],[1,1,0],[0,0,1]]", "[[1,0,0],[0,1,0],[0,0,1]]"],
        "codeSnippets": [
            {"langSlug": "cpp", "code": "class Solution {};\n\n"},
            {"langSlug": "python3", "code": "class Solution:\n    pass"},
            {"langSlug": "golang", "code": "func findCircleNum() int {}"},
            {"langSlug": "csharp", "code": "public class Solution {}"},
            {"langSlug": "erlang", "code": "-spec x() -> ok."},
        ],
    }

    def setUp(self):
        super().setUp()
        self.question = dict(self.QUESTION)
        self.fetched, self.looked = [], []
        self.patch("lookup", self._lookup)
        self.patch("fetch_question", self._fetch)

    def _lookup(self, num):
        self.looked.append(num)
        return {"slug": "number-of-provinces", "qid": "547"}

    def assert_no_network(self):
        self.assertEqual(self.looked, [])
        self.assertEqual(self.fetched, [])

    def _fetch(self, slug):
        self.fetched.append(slug)
        return self.question

    def d(self):
        return lc.ROOT / "547"

    def test_default_writes_solution_cpp(self):
        code, out = self.run_main("pull", "547")
        self.assertEqual(code, 0)
        self.assertEqual((self.d() / "Solution.cpp").read_text(), "class Solution {};\n")
        self.assertIn("wrote Solution.cpp", out)

    def test_writes_tests(self):
        self.run_main("pull", "547")
        self.assertEqual((self.d() / "tests.txt").read_text(),
                         "[[1,1,0],[1,1,0],[0,0,1]]\n[[1,0,0],[0,1,0],[0,0,1]]\n")

    def test_flag_writes_the_matching_file(self):
        _, out = self.run_main("pull", "547", "--lang", "py")
        self.assertEqual((self.d() / "Solution.py").read_text(),
                         "class Solution:\n    pass\n")
        self.assertFalse((self.d() / "Solution.cpp").exists())
        self.assertIn("wrote Solution.py", out)

    def test_flag_before_number(self):
        self.run_main("pull", "-l", "go", "547")
        self.assertEqual((self.d() / "Solution.go").read_text(),
                         "func findCircleNum() int {}\n")

    def test_go_uses_the_golang_slug(self):
        self.run_main("pull", "547", "--lang=golang")
        self.assertTrue((self.d() / "Solution.go").exists())

    def test_csharp_alias(self):
        self.run_main("pull", "547", "--lang", "C#")
        self.assertTrue((self.d() / "Solution.cs").exists())

    def test_env_default_is_used(self):
        self.set_env("python")
        self.run_main("pull", "547")
        self.assertTrue((self.d() / "Solution.py").exists())

    def test_config_default_is_used(self):
        lc.save_config({"lang": "go"})
        self.run_main("pull", "547")
        self.assertTrue((self.d() / "Solution.go").exists())

    def test_flag_beats_env(self):
        self.set_env("go")
        self.run_main("pull", "547", "--lang", "cpp")
        self.assertEqual([p.name for p in self.d().glob("Solution.*")], ["Solution.cpp"])

    def test_keeps_existing_file(self):
        self.mk("547", "Solution.py", content="mine\n")
        _, out = self.run_main("pull", "547", "--lang", "py")
        self.assertEqual((self.d() / "Solution.py").read_text(), "mine\n")
        self.assertIn("kept existing Solution.py", out)

    def test_keeping_existing_solution_writes_missing_tests(self):
        self.mk("547", "Solution.py", content="mine\n")
        _, out = self.run_main("pull", "547", "--lang", "py")
        self.assertTrue((self.d() / "tests.txt").read_text().startswith("[[1,1,0]"))
        self.assertIn("2 example cases ->", out)

    def test_existing_tests_are_kept(self):
        self.mk("547", "Solution.py", content="mine\n")
        (self.d() / "tests.txt").write_text("my case\n")
        _, out = self.run_main("pull", "547", "--lang", "py")
        self.assertEqual((self.d() / "tests.txt").read_text(), "my case\n")
        self.assertIn("kept existing tests.txt", out)
        self.assertNotIn("example cases", out)

    def test_second_language_keeps_edited_tests(self):
        self.run_main("pull", "547")
        tests = self.d() / "tests.txt"
        tests.write_text(tests.read_text() + "[[1]]\n")
        _, out = self.run_main("pull", "547", "--lang", "py")
        self.assertTrue(tests.read_text().endswith("[[1]]\n"))
        self.assertEqual(len(tests.read_text().splitlines()), 3)
        self.assertIn("wrote Solution.py", out)
        self.assertIn("kept existing tests.txt", out)

    def test_empty_tests_file_is_still_kept(self):
        self.mk("547", "tests.txt", content="")
        self.run_main("pull", "547")
        self.assertEqual((self.d() / "tests.txt").read_text(), "")

    def test_no_examples_writes_an_empty_tests_file(self):
        self.question = dict(self.QUESTION, exampleTestcaseList=[])
        _, out = self.run_main("pull", "547")
        self.assertEqual((self.d() / "tests.txt").read_text(), "")
        self.assertIn("0 example cases", out)

    def test_keeps_existing_even_if_no_snippet_now(self):
        self.mk("547", "Solution.rb", content="mine\n")
        _, out = self.run_main("pull", "547", "--lang", "ruby")
        self.assertEqual((self.d() / "Solution.rb").read_text(), "mine\n")
        self.assertIn("kept existing Solution.rb", out)
        self.assertTrue((self.d() / "tests.txt").read_text().startswith("[[1,1,0]"))

    def test_directory_in_place_of_the_solution_fails_before_network(self):
        (self.mk("547") / "Solution.py").mkdir()
        msg = self.fail_text("pull", "547", "--lang", "py")
        self.assertIn("Solution.py exists but is not a file", msg)
        self.assert_no_network()

    def test_directory_in_place_of_tests_fails_before_network(self):
        (self.mk("547") / "tests.txt").mkdir()
        self.assertIn("tests.txt exists but is not a file", self.fail_text("pull", "547"))
        self.assert_no_network()
        self.assertFalse((self.d() / "Solution.cpp").exists())

    def test_suffixed_argument_resolves_to_the_number(self):
        self.mk("547_TODO")
        self.run_main("pull", "547_TODO", "--lang", "py")
        self.assertTrue((lc.ROOT / "547_TODO" / "Solution.py").exists())
        self.assertEqual(self.looked, ["547"])

    def test_languages_coexist(self):
        self.run_main("pull", "547")
        (self.d() / "Solution.cpp").write_text("edited\n")
        self.run_main("pull", "547", "--lang", "py")
        self.assertEqual((self.d() / "Solution.cpp").read_text(), "edited\n")
        self.assertTrue((self.d() / "Solution.py").exists())

    def test_pulls_into_a_suffixed_dir(self):
        self.mk("547_TODO", "Solution.cpp")
        self.run_main("pull", "547", "--lang", "py")
        self.assertTrue((lc.ROOT / "547_TODO" / "Solution.py").exists())
        self.assertFalse(self.d().exists())

    def test_missing_snippet_names_the_language_and_alternatives(self):
        msg = self.fail_text("pull", "547", "--lang", "rust")
        self.assertIn("No Rust snippet", msg)
        self.assertIn("number-of-provinces", msg)
        self.assertIn("Available: cpp, python3, go, csharp", msg)
        self.assertNotIn("erlang", msg)

    def test_missing_snippet_leaves_nothing_behind(self):
        self.fail_text("pull", "547", "--lang", "rust")
        self.assertFalse(self.d().exists())

    def test_missing_snippet_in_existing_dir_leaves_it_untouched(self):
        self.mk("547", "Solution.cpp")
        self.fail_text("pull", "547", "--lang", "rust")
        self.assertEqual(sorted(p.name for p in self.d().iterdir()), ["Solution.cpp"])

    def test_sql_only_problem(self):
        self.question = dict(self.QUESTION, codeSnippets=[
            {"langSlug": "mysql", "code": "# Write your MySQL query"}])
        msg = self.fail_text("pull", "547")
        self.assertIn("No C++ snippet", msg)
        self.assertIn("SQL or shell", msg)

    def test_no_snippets_at_all(self):
        self.question = dict(self.QUESTION, codeSnippets=None)
        self.assertIn("No C++ snippet", self.fail_text("pull", "547"))

    def test_blank_snippet_counts_as_missing(self):
        self.question = dict(self.QUESTION, codeSnippets=[
            {"langSlug": "cpp", "code": "   \n"}])
        self.assertIn("No C++ snippet", self.fail_text("pull", "547"))

    def test_snippet_without_code_key_counts_as_missing(self):
        self.question = dict(self.QUESTION, codeSnippets=[{"langSlug": "cpp"}])
        self.assertIn("No C++ snippet", self.fail_text("pull", "547"))

    def test_bad_lang_fails_before_any_network(self):
        self.fail_text("pull", "547", "--lang", "cobol")
        self.assert_no_network()

    def test_bad_env_fails_before_any_network(self):
        self.set_env("cobol")
        self.assertIn("LC_LANG", self.fail_text("pull", "547"))
        self.assert_no_network()

    def test_bad_env_does_not_matter_with_a_flag(self):
        self.set_env("cobol")
        self.run_main("pull", "547", "--lang", "py")
        self.assertTrue((self.d() / "Solution.py").exists())

    def test_help_flag_prints_usage_without_network(self):
        code, out = self.run_main("pull", "547", "-h")
        self.assertEqual(code, 0)
        self.assertIn("lc pull <number>", out)
        self.assert_no_network()

    def test_needs_a_number_even_with_lang(self):
        self.assertIn("needs a problem number", self.fail_text("pull", "--lang", "py"))

    def test_unknown_flag(self):
        self.assertIn("Unknown option", self.fail_text("pull", "547", "--force"))

    def test_offered_langs(self):
        self.assertEqual(lc.offered_langs(self.QUESTION), ["cpp", "python3", "go", "csharp"])
        self.assertEqual(lc.offered_langs({}), [])


class Judge(Sandbox):
    """test/push send the right langSlug; the network is stubbed out."""

    RUN_OK = {"state": "SUCCESS", "status_code": 10, "status_msg": "Accepted",
              "run_success": True, "correct_answer": True,
              "code_answer": ["2"], "expected_code_answer": ["2"],
              "total_correct": 1, "total_testcases": 1}
    PUSH_OK = {"state": "SUCCESS", "status_code": 10, "status_msg": "Accepted",
               "total_correct": 1, "total_testcases": 1}

    def setUp(self):
        super().setUp()
        self.posts = []
        self.result = None
        self.patch("lookup", lambda num: {"slug": "number-of-provinces", "qid": "547"})
        self.patch("auth_headers", lambda slug: {})
        self.patch("http_json", self._http)
        self.patch("poll", lambda url, headers, what: self.result)

    def _http(self, url, data=None, headers=None, timeout=30):
        self.posts.append((url, data))
        return {"interpret_id": "i1", "submission_id": 9}

    def problem(self, *files, content="x = 1\n"):
        d = self.mk("547", *files, content=content)
        (d / "tests.txt").write_text("[[1]]\n")
        return d

    def sent(self):
        self.assertEqual(len(self.posts), 1)
        return self.posts[0][1]

    def test_python_test_sends_python3(self):
        self.problem("Solution.py")
        self.result = self.RUN_OK
        code, _ = self.run_main("test", "547")
        self.assertEqual(code, 0)
        self.assertEqual(self.sent()["lang"], "python3")
        self.assertEqual(self.sent()["typed_code"], "x = 1\n")
        self.assertIn("interpret_solution", self.posts[0][0])

    def test_go_push_sends_golang(self):
        self.problem("Solution.go")
        self.result = self.PUSH_OK
        code, _ = self.run_main("push", "547")
        self.assertEqual(code, 0)
        self.assertEqual(self.sent()["lang"], "golang")
        self.assertIn("/submit/", self.posts[0][0])

    def test_go_test_sends_golang(self):
        self.problem("Solution.go")
        self.result = self.RUN_OK
        self.run_main("test", "547")
        self.assertEqual(self.sent()["lang"], "golang")
        self.assertIn("interpret_solution", self.posts[0][0])

    def test_every_language_sends_its_slug(self):
        self.patch("syntax_check", lambda path, lang: None)
        endpoints = {"test": ("interpret_solution", self.RUN_OK),
                     "push": ("/submit/", self.PUSH_OK)}
        for cmd, (endpoint, result) in endpoints.items():
            for lang, info in lc.LANGS.items():
                with self.subTest(cmd=cmd, lang=lang):
                    self.posts, self.result = [], result
                    d = self.problem(lc.solution_name(lang))
                    try:
                        code, _ = self.run_main(cmd, "547")
                        self.assertEqual(code, 0)
                        self.assertEqual(self.sent()["lang"], info["slug"])
                        self.assertIn(endpoint, self.posts[0][0])
                    finally:
                        for f in d.glob("Solution.*"):
                            f.unlink()

    def test_payload_fields(self):
        self.problem("Solution.py")
        self.result = self.RUN_OK
        self.run_main("test", "547")
        self.assertEqual(self.sent(), {"lang": "python3", "question_id": "547",
                                       "typed_code": "x = 1\n", "data_input": "[[1]]"})
        self.posts, self.result = [], self.PUSH_OK
        self.run_main("push", "547")
        self.assertEqual(self.sent(), {"lang": "python3", "question_id": "547",
                                       "typed_code": "x = 1\n"})

    def test_polls_the_returned_id(self):
        polled = []
        self.patch("poll", lambda url, headers, what: polled.append(url) or self.result)
        self.problem("Solution.py")
        self.result = self.RUN_OK
        self.run_main("test", "547")
        self.result = self.PUSH_OK
        _, out = self.run_main("push", "547")
        self.assertEqual([u.rsplit("/detail/", 1)[1] for u in polled],
                         ["i1/check/", "9/check/"])
        self.assertIn("/submissions/detail/9/", out)

    def test_judge_refusal_is_a_fail(self):
        self.patch("http_json", lambda *a, **k: {"error": "nope"})
        self.problem("Solution.py")
        self.assertIn("did not start a run", self.fail_text("test", "547"))
        self.assertIn("did not accept the submission", self.fail_text("push", "547"))

    def test_several_files_name_the_one_sent(self):
        self.problem("Solution.cpp", "Solution.py")
        self.patch("syntax_check", lambda path, lang: None)
        self.result = self.PUSH_OK
        self.run_main("push", "547")
        self.assertEqual(self.sent()["lang"], "cpp")
        self.assertIn("using Solution.cpp", self.stderr)
        self.assertIn("also Solution.py", self.stderr)

    def test_missing_tests_file_fails_before_any_lookup(self):
        looked = []
        self.patch("lookup", lambda num: looked.append(num))
        self.mk("547", "Solution.py", content="x = 1\n")
        self.assertIn("tests.txt does not exist", self.fail_text("test", "547"))
        self.assertEqual(looked, [])

    def test_empty_tests_file_fails(self):
        d = self.problem("Solution.py")
        (d / "tests.txt").write_text("\n\n")
        self.assertIn("is empty", self.fail_text("test", "547"))
        self.assertEqual(self.posts, [])

    def test_tests_are_read_from_the_suffixed_dir(self):
        d = self.mk("547_TODO", "Solution.py", content="x = 1\n")
        (d / "tests.txt").write_text("[[7]]\n")
        self.result = self.RUN_OK
        self.run_main("test", "547")
        self.assertEqual(self.sent()["data_input"], "[[7]]")

    def test_outside_a_problem_dir_with_flag(self):
        os.chdir(lc.ROOT)
        msg = self.fail_text("test", "--lang", "py")
        self.assertIn("Not inside a problem directory", msg)
        self.assertEqual(self.posts, [])

    def test_help_flag_prints_usage(self):
        for cmd in ("test", "push"):
            with self.subTest(cmd=cmd):
                code, out = self.run_main(cmd, "--help")
                self.assertEqual(code, 0)
                self.assertIn("lc push [number]", out)
        self.assertEqual(self.posts, [])

    def test_cpp_still_sends_cpp(self):
        self.patch("syntax_check", lambda path, lang: None)
        self.problem("Solution.cpp")
        self.result = self.RUN_OK
        self.run_main("test", "547")
        self.assertEqual(self.sent()["lang"], "cpp")

    def test_flag_chooses_among_several(self):
        self.problem("Solution.cpp", "Solution.py")
        self.result = self.RUN_OK
        self.run_main("test", "--lang", "py", "547")
        self.assertEqual(self.sent()["lang"], "python3")

    def test_several_without_default_file_sends_nothing(self):
        self.problem("Solution.py", "Solution.java")
        self.assertIn("Several solutions", self.fail_text("test", "547"))
        self.assertEqual(self.posts, [])

    def test_flag_for_missing_file_suggests_pull(self):
        self.problem("Solution.py")
        self.assertIn("lc pull 547 --lang rust", self.fail_text("push", "547", "-l", "rs"))
        self.assertEqual(self.posts, [])

    def test_python_syntax_error_stops_before_the_network(self):
        self.problem("Solution.py", content="def f(:\n")
        self.assertIn("Syntax error", self.fail_text("push", "547"))
        self.assertEqual(self.posts, [])

    def test_number_from_cwd_with_flag(self):
        d = self.problem("Solution.cpp", "Solution.py")
        os.chdir(d)
        self.result = self.RUN_OK
        self.run_main("test", "--lang", "python3")
        self.assertEqual(self.sent()["lang"], "python3")

    def test_runtime_hint_uses_the_real_filename(self):
        self.problem("Solution.py")
        self.result = dict(self.RUN_OK, run_success=False, correct_answer=False,
                           status_msg="Runtime Error", code_answer=[],
                           full_runtime_error="Line 3 in f (Solution.py)")
        code, out = self.run_main("test", "547")
        self.assertEqual(code, 1)
        self.assertIn("Line 3 in f (Solution.py)", out)
        self.assertNotIn("hint", out)

    def test_push_runtime_hint_uses_the_real_filename(self):
        self.problem("Solution.py")
        self.result = {"status_code": 15, "status_msg": "Runtime Error",
                       "full_runtime_error": "boom (Solution.py)"}
        _, out = self.run_main("push", "547")
        self.assertNotIn("hint", out)

    def test_missing_solution_fails_before_any_lookup(self):
        looked = []
        self.patch("lookup", lambda num: looked.append(num))
        self.mk("547")
        for cmd in ("test", "push"):
            self.fail_text(cmd, "547", "--lang", "rust")
            self.fail_text(cmd, "547")
        self.assertEqual(looked, [])

    def test_syntax_error_fails_before_any_lookup(self):
        looked = []
        self.patch("lookup", lambda num: looked.append(num))
        self.problem("Solution.py", content="def f(:\n")
        self.fail_text("test", "547")
        self.assertEqual(looked, [])

    def test_unknown_flag_is_rejected(self):
        self.problem("Solution.py")
        self.assertIn("Unknown option", self.fail_text("test", "547", "--dry-run"))

    def test_extra_number_is_rejected(self):
        self.problem("Solution.py")
        self.assertIn("one problem number", self.fail_text("push", "547", "548"))


class UsageText(unittest.TestCase):
    def test_usage_mentions_lang(self):
        self.assertIn("--lang", lc.USAGE)
        self.assertIn("lc lang", lc.USAGE)
        self.assertIn("LC_LANG", lc.USAGE)

    def test_usage_mentions_every_flag_form(self):
        self.assertIn("-l L", lc.USAGE)
        self.assertIn("--lang=L", lc.USAGE)

    def test_docstring_no_longer_assumes_cpp_is_the_default(self):
        self.assertNotIn("instead of Solution.cpp", lc.__doc__)

    def test_docstring_mentions_lang(self):
        self.assertIn("--lang", lc.__doc__)
        self.assertIn("lc lang", lc.__doc__)


# Real LeetCode Rust stubs, trimmed.
RUST_PLAIN = """impl Solution {
    pub fn find_circle_num(is_connected: Vec<Vec<i32>>) -> i32 {

    }
}"""

RUST_LIST = """// Definition for singly-linked list.
// #[derive(PartialEq, Eq, Clone, Debug)]
// pub struct ListNode {
//   pub val: i32,
//   pub next: Option<Box<ListNode>>
// }
//
// impl ListNode {
//   #[inline]
//   fn new(val: i32) -> Self {
//     ListNode {
//       next: None,
//       val
//     }
//   }
// }
impl Solution {
    pub fn reverse_list(head: Option<Box<ListNode>>) -> Option<Box<ListNode>> {

    }
}"""

RUST_TREE = """// Definition for a binary tree node.
// #[derive(Debug, PartialEq, Eq)]
// pub struct TreeNode {
//   pub val: i32,
//   pub left: Option<Rc<RefCell<TreeNode>>>,
//   pub right: Option<Rc<RefCell<TreeNode>>>,
// }
//
// impl TreeNode {
//   #[inline]
//   pub fn new(val: i32) -> Self {
//     TreeNode {
//       val,
//       left: None,
//       right: None
//     }
//   }
// }
use std::rc::Rc;
use std::cell::RefCell;
impl Solution {
    pub fn max_depth(root: Option<Rc<RefCell<TreeNode>>>) -> i32 {

    }
}"""

RUST_DESIGN = """struct LRUCache {

}

impl LRUCache {
    fn new(capacity: i32) -> Self {

    }
}"""


class StripLocal(unittest.TestCase):
    B, E = lc.LOCAL_BEGIN, lc.LOCAL_END

    def test_no_markers_is_unchanged(self):
        self.assertEqual(lc.strip_local("a\nb\n"), "a\nb\n")

    def test_block_and_markers_are_removed(self):
        code = f"{self.B}\npub struct Solution;\n{self.E}\n\nimpl Solution {{}}\n"
        self.assertEqual(lc.strip_local(code), "\nimpl Solution {}\n")

    def test_imports_and_blank_lines_outside_block_are_preserved(self):
        code = (f"use std::collections::HashMap;\n\n{self.B}\n"
                f"pub struct Solution;\n{self.E}\n\n"
                "use std::cmp::max;\nimpl Solution {}\n")
        self.assertEqual(lc.strip_local(code),
                         "use std::collections::HashMap;\n\n\n"
                         "use std::cmp::max;\nimpl Solution {}\n")

    def test_block_lines_are_deleted(self):
        code = f"a\n{self.B}\nx\ny\n{self.E}\nb"
        self.assertEqual(lc.strip_local(code), "a\nb")

    def test_several_blocks(self):
        code = f"a\n{self.B}\nx\n{self.E}\nb\n{self.B}\ny\n{self.E}\nc"
        self.assertEqual(lc.strip_local(code), "a\nb\nc")

    def test_marker_text_after_the_tag_may_change(self):
        code = "//lc:local-begin whatever\nx\n  //  lc:local-end   \ny"
        self.assertEqual(lc.strip_local(code), "y")

    def test_ordinary_comments_are_kept(self):
        code = "// a comment\n// lc: not a marker\n// lc:local-beginning\nx"
        self.assertEqual(lc.strip_local(code), code)

    def test_marker_inside_a_string_is_not_a_marker(self):
        code = 'let s = "// lc:local-begin";\n'
        self.assertEqual(lc.strip_local(code), code)

    def test_unclosed_block_fails_with_its_line(self):
        with self.assertRaises(lc.Fail) as cm:
            lc.strip_local(f"x\n{self.B}\ny")
        self.assertIn("line 2", str(cm.exception))
        self.assertIn("never closed", str(cm.exception))

    def test_stray_end_fails(self):
        with self.assertRaises(lc.Fail) as cm:
            lc.strip_local(f"x\n{self.E}\n")
        self.assertIn("no matching", str(cm.exception))

    def test_nested_begin_fails(self):
        with self.assertRaises(lc.Fail) as cm:
            lc.strip_local(f"{self.B}\n{self.B}\n{self.E}\n{self.E}")
        self.assertIn("nested", str(cm.exception))

    def test_crlf_lines(self):
        code = f"{self.B}\r\nx\r\n{self.E}\r\ny\r\n"
        self.assertEqual(lc.strip_local(code), "y\r\n")


class LocalPrelude(unittest.TestCase):
    def test_rust_gets_struct_solution(self):
        pre = lc.local_prelude("rust", RUST_PLAIN)
        self.assertEqual(pre, f"{lc.LOCAL_BEGIN}\npub struct Solution;\n{lc.LOCAL_END}\n")

    def test_prelude_strips_back_to_the_stub(self):
        for stub in (RUST_PLAIN, RUST_LIST, RUST_TREE):
            with self.subTest(stub=stub[:30]):
                full = lc.local_prelude("rust", stub) + stub
                sent = lc.strip_local(full)
                self.assertEqual(sent, stub)

    def test_list_node_definition_is_uncommented(self):
        pre = lc.local_prelude("rust", RUST_LIST)
        self.assertIn("\n#[derive(PartialEq, Eq, Clone, Debug)]\npub struct ListNode {\n"
                      "  pub val: i32,\n", pre)
        self.assertIn("impl ListNode {\n  #[inline]\n", pre)
        self.assertNotIn("// pub struct", pre)
        self.assertNotIn("Definition for", pre)

    def test_tree_node_definition_is_uncommented(self):
        pre = lc.local_prelude("rust", RUST_TREE)
        self.assertIn("pub struct TreeNode {", pre)
        self.assertIn("pub fn new(val: i32) -> Self {", pre)
        # the stub's own `use` lines stay where they are, outside the block
        self.assertNotIn("use std::rc::Rc;", pre)

    def test_definitions_stop_at_the_first_code_line(self):
        defs = lc.commented_definitions(RUST_TREE)
        self.assertNotIn("use std", defs)
        self.assertNotIn("impl Solution", defs)
        self.assertTrue(defs.endswith("}"))

    def test_bare_comment_slashes_become_blank_lines(self):
        self.assertEqual(lc.commented_definitions("// Definition for x.\n// a\n//\n// b\nfn"),
                         "a\n\nb")

    def test_comments_not_under_a_definition_header_are_left(self):
        stub = "// Some note\n// pub struct Nope;\n" + RUST_PLAIN
        self.assertEqual(lc.commented_definitions(stub), "")

    def test_design_problem_gets_no_prelude(self):
        self.assertEqual(lc.local_prelude("rust", RUST_DESIGN), "")

    def test_stub_that_already_has_struct_solution_gets_none(self):
        self.assertEqual(lc.local_prelude("rust", "pub struct Solution;\n" + RUST_PLAIN), "")

    def test_other_languages_get_no_prelude(self):
        for lang in lc.LANGS:
            if lang != "rust":
                with self.subTest(lang=lang):
                    self.assertEqual(lc.local_prelude(lang, RUST_PLAIN), "")

    @unittest.skipUnless(shutil.which("rustc"), "rustc not installed")
    def test_pulled_rust_files_build_locally(self):
        """The whole point: with the prelude, rustc accepts the stub."""
        filled = {
            RUST_PLAIN: "pub fn find_circle_num(is_connected: Vec<Vec<i32>>) -> i32 {\n        is_connected.len() as i32\n",
            RUST_LIST: "pub fn reverse_list(head: Option<Box<ListNode>>) -> Option<Box<ListNode>> {\n        head\n",
            RUST_TREE: "pub fn max_depth(root: Option<Rc<RefCell<TreeNode>>>) -> i32 {\n        root.map_or(0, |_| 1)\n",
        }
        for stub, body in filled.items():
            with self.subTest(stub=stub[:30]), tempfile.TemporaryDirectory() as tmp:
                head, _, _ = stub.rpartition("pub fn ")
                code = head + body + "    }\n}\n"
                src = Path(tmp) / "Solution.rs"
                src.write_text(lc.local_prelude("rust", stub) + code)
                proc = subprocess.run(
                    ["rustc", "--edition", "2021", "--crate-type", "lib",
                     "--emit=metadata", "-A", "dead_code", "-o", str(Path(tmp) / "out"),
                     str(src)], capture_output=True, text=True)
                self.assertEqual(proc.returncode, 0, proc.stderr)


class RustLocalBlock(Sandbox):
    """pull writes the local block; test and push never send it."""

    def setUp(self):
        super().setUp()
        self.question = {"title": "Reverse Linked List", "exampleTestcaseList": ["[1,2]"],
                         "codeSnippets": [{"langSlug": "rust", "code": RUST_LIST},
                                          {"langSlug": "cpp", "code": "class Solution {};"}]}
        self.patch("lookup", lambda num: {"slug": "reverse-linked-list", "qid": "206"})
        self.patch("fetch_question", lambda slug: self.question)
        self.patch("auth_headers", lambda slug: {})
        self.posts = []
        self.patch("http_json", lambda url, data=None, headers=None, timeout=30:
                   self.posts.append((url, data)) or {"interpret_id": "i", "submission_id": 1})
        self.patch("poll", lambda url, headers, what: {
            "state": "SUCCESS", "status_code": 10, "status_msg": "Accepted",
            "run_success": True, "correct_answer": True, "code_answer": ["1"],
            "expected_code_answer": ["1"], "total_correct": 1, "total_testcases": 1})

    def sol(self):
        return lc.ROOT / "206" / "Solution.rs"

    def test_pull_writes_struct_and_list_node_above_the_stub(self):
        self.run_main("pull", "206", "--lang", "rust")
        text = self.sol().read_text()
        self.assertTrue(text.startswith(lc.LOCAL_BEGIN + "\npub struct Solution;\n"))
        self.assertIn("\npub struct ListNode {", text)
        self.assertTrue(text.endswith(RUST_LIST + "\n"))  # stub itself untouched

    def test_pull_rust_writes_a_cargo_manifest(self):
        _, out = self.run_main("pull", "206", "--lang", "rust")
        manifest = (lc.ROOT / "206" / "Cargo.toml").read_text()
        self.assertEqual(manifest, lc.cargo_manifest("206"))
        self.assertIn('name = "lc-206"', manifest)
        self.assertIn('path = "Solution.rs"', manifest)
        self.assertIn("[workspace]", manifest)
        self.assertIn("wrote Cargo.toml", out)

    def test_pull_keeps_an_existing_manifest(self):
        (lc.ROOT / "206").mkdir()
        (lc.ROOT / "206" / "Cargo.toml").write_text("# mine\n")
        _, out = self.run_main("pull", "206", "--lang", "rust")
        self.assertEqual((lc.ROOT / "206" / "Cargo.toml").read_text(), "# mine\n")
        self.assertIn("kept existing Cargo.toml", out)

    def test_repull_adds_a_missing_manifest_and_keeps_the_solution(self):
        self.sol().parent.mkdir()
        self.sol().write_text("mine\n")
        _, out = self.run_main("pull", "206", "--lang", "rust")
        self.assertEqual(self.sol().read_text(), "mine\n")
        self.assertTrue((lc.ROOT / "206" / "Cargo.toml").is_file())
        self.assertIn("kept existing Solution.rs, wrote Cargo.toml", out)

    def test_manifest_path_blocked_by_a_dir_fails_before_any_request(self):
        (lc.ROOT / "206" / "Cargo.toml").mkdir(parents=True)
        fetched = []
        self.patch("lookup", lambda num: fetched.append(num))
        with self.assertRaises(lc.Fail) as cm:
            self.run_main("pull", "206", "--lang", "rust")
        self.assertIn("not a file", str(cm.exception))
        self.assertEqual(fetched, [])
        self.assertFalse(self.sol().exists())

    def test_other_languages_get_no_manifest(self):
        self.run_main("pull", "206")
        self.assertFalse((lc.ROOT / "206" / "Cargo.toml").exists())

    def test_suffixed_dir_gets_the_manifest(self):
        (lc.ROOT / "206_TODO").mkdir()
        self.run_main("pull", "206", "--lang", "rust")
        self.assertTrue((lc.ROOT / "206_TODO" / "Cargo.toml").is_file())

    @unittest.skipUnless(shutil.which("cargo"), "cargo not installed")
    def test_pulled_crate_passes_cargo_check(self):
        """End to end: a pulled Rust problem is a crate cargo accepts."""
        self.question["codeSnippets"][0]["code"] = RUST_LIST.replace(
            "-> Option<Box<ListNode>> {\n\n", "-> Option<Box<ListNode>> {\n        head\n")
        self.run_main("pull", "206", "--lang", "rust")
        d = lc.ROOT / "206"
        env = dict(os.environ, CARGO_TARGET_DIR=str(d.parent / "target"))
        proc = subprocess.run(["cargo", "check", "--offline", "--quiet"], cwd=d,
                              capture_output=True, text=True, env=env)
        self.assertEqual(proc.returncode, 0, proc.stderr)

    def test_pull_cpp_gets_no_block(self):
        self.run_main("pull", "206")
        self.assertNotIn("lc:local", (lc.ROOT / "206" / "Solution.cpp").read_text())

    def test_pull_keeps_an_existing_rust_file(self):
        self.sol().parent.mkdir()
        self.sol().write_text("mine\n")
        self.run_main("pull", "206", "--lang", "rust")
        self.assertEqual(self.sol().read_text(), "mine\n")

    def test_test_and_push_send_the_stub_without_the_block(self):
        self.run_main("pull", "206", "--lang", "rust")
        for cmd in ("test", "push"):
            with self.subTest(cmd=cmd):
                self.posts.clear()
                code, _ = self.run_main(cmd, "206")
                self.assertEqual(code, 0)
                sent = self.posts[0][1]["typed_code"]
                self.assertEqual(self.posts[0][1]["lang"], "rust")
                self.assertNotIn("lc:local", sent)
                self.assertNotIn("pub struct Solution", sent)
                self.assertEqual(sent, RUST_LIST + "\n")

    def test_unclosed_block_fails_before_any_request(self):
        self.mk("206", "Solution.rs", content=f"{lc.LOCAL_BEGIN}\nx\n")
        (lc.ROOT / "206" / "tests.txt").write_text("[1]\n")
        with self.assertRaises(lc.Fail) as cm:
            self.run_main("push", "206")
        self.assertIn("never closed", str(cm.exception))
        self.assertEqual(self.posts, [])

    def test_file_with_only_a_local_block_counts_as_empty(self):
        self.mk("206", "Solution.rs",
                content=f"{lc.LOCAL_BEGIN}\npub struct Solution;\n{lc.LOCAL_END}\n")
        (lc.ROOT / "206" / "tests.txt").write_text("[1]\n")
        with self.assertRaises(lc.Fail) as cm:
            self.run_main("test", "206")
        self.assertIn("is empty", str(cm.exception))
        self.assertEqual(self.posts, [])

    def test_blocks_work_in_any_slash_comment_language(self):
        self.mk("206", "Solution.cpp",
                content=f"{lc.LOCAL_BEGIN}\n#include <bits/stdc++.h>\n{lc.LOCAL_END}\nclass Solution {{}};\n")
        (lc.ROOT / "206" / "tests.txt").write_text("[1]\n")
        self.patch("syntax_check", lambda path, lang: None)
        self.run_main("test", "206")
        self.assertEqual(self.posts[0][1]["typed_code"], "class Solution {};\n")


if __name__ == "__main__":
    unittest.main(verbosity=2)

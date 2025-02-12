# Concepts

- Knuth-Morris-Pratt (KMP) Algorithm: string search algorithm to search a substring in a string in O(n + m) time complexity.
- Dutch national flag algorithm: to sort an array of 0, 1 and 2 (basically 3 numbers) in ascending manner.
- Moore's voting algorithm and Extended Moore's algorithm. Used to find cycles in linked list. Used to find middle of linked list (slow and fast pointer).
- Sieve of erathoneses: to find all primes in a range
- Floyd's Tortoise and Hare Algorithm 
- Binary exponentiation to find pow(x, n)
- Sliding window algorithm: generally used to find maximum or minimum subarray, substring of fixed size.

## Things to keep in mind
1. `substr` is heavy, use for loop and index based comparison if possible.
2. Use array as hashmap if you know how much elements are needed ex. to count character frequency in string.
3. things to keep in mind about palindrome: 
    - generally string a is palindrome, iff `a == a[::-1]`.
    - palindrome word will have only one odd character and rest will be even number letters distributed equally example "paap" or "pabap".

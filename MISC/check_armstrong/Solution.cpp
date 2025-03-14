int length_of_num(int n)
{
    int len = 0;
    while (n) {
        n /= 10;
        len++;
    }
    return len;
}

int pow(int number, int p)
{
    if (p == 0)
        return 1;
    return number * pow(number, p-1);
}

bool checkArmstrong(int number) {
    int n = length_of_num(number);
    int original = number, num = 0;
    
    while (number) {
        int last = number % 10;
        num += pow(last, n);
        number /= 10;
    }

    return (num == original);
}


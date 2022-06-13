#include <cs50.h>
#include <stdio.h>

int length(long);
long power(long,long);
long breakdown(long);

int main(void)
{
    long number = get_long("Number: ");

    int n = length(number);

    long a;
    long b;

    long sum = 0;

    for (int i = 1; i < n+1; ++i)
    {
        long x = number % power(10.0,(long) i);

        x = x / power(10.0, (long) i-1);

        if (i == n) a = x;
        if (i == n-1) b = x;

        if (i % 2 == 0) x = 2*x;

        x = breakdown(x);

        sum += x;


    }

    if (sum % 10 == 0)
    {
        if (a == 4 && (n==13 || n==16))
        {
            printf("VISA\n");
        } else if (a == 3 && (b == 4 || b == 7) && (n==15))
        {
            printf("AMEX\n");
        } else if (a == 5 && (b == 1 || b == 2 || b == 3 || b == 4 || b == 5) && (n==16))
        {
            printf("MASTERCARD\n");
        } else
        {
            printf("INVALID\n");
        }
    } else
    {
        printf("INVALID\n");
    }

}

int length(long number)
{
    int n = 0;
    while (number > 0)
    {
        number = number / 10;
        ++n;
    }

    return n;
}

long power(long base, long exp)
{
    long ans = 1;
    for (int i = 0; i < exp; ++i) {
        ans = ans * base;
    }

    return ans;
}

long breakdown(long x)
{
    if (x / 10.0 >= 1)
    {
        int a = (int) x / 10;
        int b = (int)x % 10;

        return (long) a + (long) b;
    } else
    {
        return x;
    }
}

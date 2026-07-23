#include "mutils.h"

int gcd(int a, int b) 
{
    if (a == 0) return b;
    if (b == 0) return a;
    if (a == b) return a;

    if (a % 2 == 0) {
        if (b % 2 == 0)
            return 2 * gcd(a / 2, b / 2);
        else
            return gcd(a / 2, b);
    } else {
        if (b % 2 == 0)
            return gcd(a, b / 2);
        else 
            return gcd(abs(a - b), min(a, b));
    }
}



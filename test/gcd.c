int gcd(int a, int b) {
    if (b == 0)
        return a;
    else 
        return gcd(b, a % b);
}

int main() {
    int n;
    n = getint();
    while (n > 0) {
        n = n - 1;
        int a;
        int b;
        a = getint();
        b = getint();
        int r;
        r = gcd(a, b);
        putint(r);
        putchar(10);
    }
    return 0;
}

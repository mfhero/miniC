int a;
int b;

int getint();
int putint();
int putchar();

int main() {
    int n;
    n = getint();
    while (n > 0) {
        a = getint();
        b = getint();
        if (a > 5 && b > 5)
            putint(1);
        if (a < 5 || b < 5)
            putint(-1);
        putchar(10);
        n = n - 1;
    }
    return 0;
}

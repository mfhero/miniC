int i;
int j[2];

int foo(int x) {
    int u;
    u = (x + 1) * 3;
    return u + 1;
}

int getint();
int putint(int x);
int n;
int a[10];

int main() {
    i = 10;
    j[1] = foo(i);
//    return 0;
n = getint();
if (n > 10)
return 1;
int s;
int i;
i = 0;
s = i;
while (i < n) {
a[i] = getint();
s = s + a[i];
i = i + 1;
}
putint(s);
return 0;
}

int i;
int j[2];

int foo(int x) {
    int u;
    u = (x + 1) * 3;
    return u + 1;
}

int main() {
    i = 10;
    j[0] = foo(i);
    return 0;
}

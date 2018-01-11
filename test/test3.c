int f(int x)
{
    if (x<2)
        return 1;
    else
        return f(x-1)+f(x-2);
}
int g(int x)
{
    int a[40];
    a[0]=1; a[1]=1;
    int i;
    i=2;
    while (i<x+1)
    {
        a[i]=a[i-1]+a[i-2];
        i=i+1;
    }
    return a[x];
}
int n;
int main()
{
    n=getint();
    if (n<0||n>30)
        return 1;
    int a;
    a=putint(f(n));
    a=putchar(10);
    a=putint(g(n));
    a=putchar(10);
    return 0;
} 

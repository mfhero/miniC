int n;
int ans;
int a[110];
int f[110];
int max(int a,int b)
{
  if (a>b)
    return a;
  else
    return b;
}
int main()
{
  int i;
  int j;
  n=getint();
  ans=0;
  i=1;
  while (i<n+1)
  {
    a[i]=getint();
    i=i+1;
  }
  i=1;
  while (i<n+1)
  {
    f[i]=1;
    j=1;
    while (j<i)
    {
      if (a[j]>a[i]-1)
        f[i]=max(f[i],f[j]+1);
      j=j+1;
    }
    ans=max(ans,f[i]);
    i=i+1;
  }
  putint(ans);
  return 0;
}

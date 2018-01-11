
int tot;
int d[200];
int pre[200];
int St[200];
int C[200];
int Cost[200];
int head[100];
int dis[100];
int path[100];
int h;
int t;
int queue[200];
int inqueue[100];
int s;
int T;
int j;
int MinCost;
int x;
int y;
int z;
int MinFlow;
int n;
int m;
int i;
int a[100];
int now;
int prework()
{
	tot=0;
 h=0;
 t=0;
 s=0;
 T=0;
 j=0;
 MinCost=0;
 x=0;
 y=0;
 z=0;
 MinFlow=0;
 n=0;
 m=0;
 i=0;
 now=0;
 int ii;
 ii=0;
 while (ii<100)
 {
 	 head[ii]=0;
 dis[ii]=0;
 path[ii]=0;
a[ii]=0;
 inqueue[ii]=0;
 	ii=ii+1;
 }
 ii=0;
 while (ii<200)
 {

d[ii]=0;
 pre[ii]=0;
 St[ii]=0;
 C[ii]=0;
 Cost[ii]=0;
 	ii=ii+1;
 }
 ii=0;
 while (ii<200)
 {
queue[ii]=0;
 	ii=ii+1;
 }
    return 0;
}
int Add(int a,int b,int c,int cost)
{
  tot=tot+1; d[tot]=b; St[tot]=a; pre[tot]=head[a]; C[tot]=c; Cost[tot]=cost; head[a]=tot;
  tot=tot+1; d[tot]=a; St[tot]=b; pre[tot]=head[b]; C[tot]=0; Cost[tot]=-cost; head[b]=tot;
    return 0;
}
int spfa()
{
  int i;
  i=0;
  while (i<100)
  {
  	dis[i]=999;
  	i=i+1;
  }
  dis[s]=0; h=0; t=1;
  inqueue[s]=1; queue[1]=s;
  while (h<t)
  {
    h=h+1;
    j=head[queue[h]];
    while (j)
    {
      if (C[j])
      if (dis[queue[h]]+Cost[j]<dis[d[j]])
      {
        dis[d[j]]=dis[queue[h]]+Cost[j];
        path[d[j]]=j;
        if (inqueue[d[j]] == 0)
        {
          inqueue[d[j]]=1;
          t=t+1;
          queue[t]=d[j];
        }
      }
      j=pre[j];
    }
    inqueue[queue[h]]=0;
  }
  return (dis[T]<999);
}
int CostFlow()
{
  while (spfa())
  {
    MinFlow=999;
    x=T;
    while (x!=s)
    {
      if (C[path[x]]<MinFlow) MinFlow=C[path[x]];
      x=St[path[x]];
    }
    x=T;
    while (x!=s)
    {
      C[path[x]]=C[path[x]]-MinFlow;
      if (path[x]%2==0) C[path[x]+1]=C[path[x]+1]+MinFlow;
      else C[path[x]-1]=C[path[x]-1]+MinFlow;
      x=St[path[x]];
    }
    MinCost=MinCost+dis[T]*MinFlow;
  }
    return 0;
}
int main()
{
  prework();
  tot=1;
  n=getint();
  m=getint();
  s=0; T=n+2; i=1;
  while (i<n+1)
  {
  	a[i]=getint();
    Add(i,i+1,999,0);
    i=i+1;
  }
  i=1;
  while (i<n+2)
  {
    now=a[i]-a[i-1];
    if (now<0) Add(s,i,-now,0);
    else if (now>0) Add(i,T,now,0);
  	i=i+1;
  }
  i=1;
  while (i<m+1)
  {
  	x=getint();
  	y=getint();
  	z=getint();
    Add(y+1,x,999,z);
    i=i+1;
  }
  //printf("costflow\n");
  CostFlow();
  putint(MinCost);
  return 0;
}

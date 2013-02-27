#include <cstdio>
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
const int MAXN=1001;
char str[MAXN],s[MAXN*2];
int n,p[MAXN*2],ans,cases=1;

#ifndef min
#define min(a, b)	(((a) <= (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a, b)	(((a) >= (b)) ? (a) : (b))
#endif

//longest palindrome substring
//http://bbs.dlut.edu.cn/bbstcon.php?board=Competition&gid=23474
void rebuild()
{
    s[0]='$',s[1]='#';
    n=strlen(str);
    for(int i=0;i<n;++i)
		s[2*i+2]=str[i],s[2*i+3]='#';
    s[n=n*2+2]=0;
}
void solve()
{
    int mx=0,id,ans=1;
    for(int i=1;i<n;++i)
    {
        if(mx>i)p[i]=min(p[2*id-i],mx-i);
        else p[i]=1;
        for(;s[i-p[i]]==s[i+p[i]];p[i]++);
        if(p[i]+i>mx)mx=p[i]+i,id=i;
        ans=max(ans,p[i]);
    }
    printf("Case %d: %d\n",cases++,ans-1);
}

#define MAXCHAR 5000 //????5000???
char c[MAXCHAR], *a[MAXCHAR];

int main()
{
    while(scanf("%s",str),str[0]-'E')
    {
        rebuild();
        solve();
    }

	char buf[32] = "banana";
	char* a[32] = {0};
	int n = strlen(buf);

	for(int i = 0; i < n; i++)
	{
		a[i] = &buf[i];
	}

	for(i=0; i < n; i++)
	{
		printf("%s\n", a[i]);
	}
    return 0;
}
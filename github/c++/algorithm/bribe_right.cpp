#include <cstdio>
#include <cstring>
#define max(a,b) ((a)>(b)?(a):(b))
#define oo 2147483647
#define maxn 211
#define maxe maxn
#define maxm maxn
#define maxl 111
 
using namespace std;
 
struct edge_type
{
    int x,y,next;
};
 
int n,m;
char name[maxn + 1][maxl + 1];
int cost[maxn + 1];
char dominate[maxn + 1][maxn + 1][maxl + 1];
int tree[maxn + 1][maxn + 1];
int sub[maxn + 1];
 
int f[maxn + 1][maxm + 1];
bool mark[maxn + 1];
 
int who(char *st)
{
    for (int i=1;i<=n;i++)
        if (!strcmp(st,name[i])) return i;
    return 0;
}
 
int calc_sub(int x)
{
    sub[x] = 1;
    for (int i=1;i<=tree[x][0];i++){
        sub[x] += calc_sub(tree[x][i]);
    }
    return sub[x];
}
 
void work(int x)
{
    if (mark[x]) return ;
    mark[x] = 1;
    for (int i=1;i<=tree[x][0];i++)
        work(tree[x][i]);
    for (int i=1;i<=n;i++)
        f[x][i] = oo;
    f[x][0] = 0;
    f[x][sub[x]] = cost[x];
    int temp;
 
    for (int i=1;i<=tree[x][0];i++)
        for (int j=sub[x];j>=1;j--){
            int y = tree[x][i];
            for (int k=0;k<=j;k++){
                if (f[y][k]>=oo) continue;
                if (f[x][j - k]>=oo) continue;
                if ((temp = f[x][j - k] + f[y][k])<f[x][j])
                    f[x][j] = temp;
            }
        }
}
 
int main()
{
    while (scanf("%d%d",&n,&m)!=EOF){
        if ((!n)&&(!m)) break;
        for (int i=1;i<=n;i++){
            scanf("%s%d",name[i],&cost[i]);
            tree[i][0] = 0;
            char ch;
            while ((ch = getchar())!='\n'){
                scanf("%s",dominate[i][++tree[i][0]]);
            }
        }
        memset(mark,0,sizeof(mark));
        for (int i=1;i<=n;i++){
            for (int j=1;j<=tree[i][0];j++){
                int y = who(dominate[i][j]);
                mark[y] = 1;
                tree[i][j] = y;
            }
        }
        tree[0][0] = 0;
        for (int i=1;i<=n;i++)
            if (!mark[i])
            tree[0][++tree[0][0]] = i;
        cost[0] = oo;
        calc_sub(0);
        sub[0] = n;
        memset(mark,0,sizeof(mark));
        work(0);
        int ans = oo;
        for (int i=m;i<=n;i++)
            if ((f[0][i])<ans)
                ans = f[0][i];
        printf("%d\n",ans);
        n = m = 0;
    }
    return 0;
}

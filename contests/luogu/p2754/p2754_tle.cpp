#include<bits/stdc++.h>
using namespace std;
#define int long long
const int N=50050,inf=1e9;
int n,m,k,sc[N],fa[N],dep[N];
vector<int> sh[N];
struct dat{
	int cap,net,to;
}e[N];
int tot=1,head[N],cur[N];
void add(int u,int v,int w){
	e[++tot].to=v;
	e[tot].net=head[u];
	e[tot].cap=w;
	head[u]=tot;
	
	
	e[++tot].to=u;
	e[tot].net=head[v];
	e[tot].cap=0;
	head[v]=tot;
}
int s=0,t=50001;
int maxnode=t;  // 动态跟踪最大节点
bool bfs(){
	for(int i=0;i<=maxnode;i++){
		dep[i]=inf;
	}
	queue<int> q;
	q.push(s);
	dep[s]=0;
	while(!q.empty()){
		int u=q.front();
		q.pop();
		for(int i=head[u];i;i=e[i].net){
			int v=e[i].to;
			if(dep[v]==inf&&e[i].cap>0){
				q.push(v);
				dep[v]=dep[u]+1;
				if(v==t) return 1;
			}
		}
	}
	return 0;
}
int dfs(int u,int sum){
	if(u==t) return sum;
	int res=0;
	for(int i=cur[u];i;i=e[i].net){
		int v=e[i].to;
		if(dep[v]==dep[u]+1&&e[i].cap>0){
			int k=dfs(v,min(sum-res,e[i].cap));
			res+=k;
			e[i].cap-=k;
			e[i^1].cap+=k;
			if(res==sum) break;
		}
	}
	return res;
}
int dinic(){
	int ans=0;
	while(bfs()){
		for(int i=0;i<=maxnode;i++) cur[i]=head[i];
		ans+=dfs(s,inf);
	}
//	cout<<ans<<endl;
	return ans;
}
int gid(int x,int y){
	return x*(n+2)+y;
}
int find(int x){
	if(fa[x]==x) return x;
	return fa[x]=find(fa[x]);
}
void ccb(int x,int y){
	int fx=find(x),fy=find(y);
	fa[fx]=fy;
}
void init(){
	cin>>n>>m>>k;
	for(int i=1;i<=n+2;i++) fa[i]=i;
}
void input(){
	for(int i=1,x;i<=m;i++){
		cin>>sc[i]>>x;
		for(int j=0,y;j<x;j++){
			cin>>y;
			if(y==0) y=n+1;
			else if(y==-1) y=n+2;
			sh[i].push_back(y);
			if(j>0) ccb(y,sh[i][j-1]);
		}
		if(x>1){
			ccb(sh[i][0],sh[i][x-1]);
		}
	}
}
void check_ans(){
	if(find(n+1)!=find(n+2)){
		cout<<0<<endl;
		exit(0);
	}
}
void build_up(){
	int ans=0;
//	add(s,gid(0,n+1),k);
	for(int tt=1;tt<=300;tt++){  // 减少时间上限
		// 更新最大节点范围
		maxnode=max(maxnode,gid(tt,n+2));
		
        add(s,gid(tt-1,n+1),inf);
        for(int i=1;i<=m;i++){
			int rl=sh[i].size();
			if(rl==0) continue;
			int now=(tt-1)%rl,nxt=tt%rl;
			int snow=sh[i][now],snxt=sh[i][nxt];
			if(snow==n+2) continue;
			else if(snxt==n+2) add(gid(tt-1,snow),t,sc[i]);
			else add(gid(tt-1,snow),gid(tt,snxt),sc[i]);
		}
		
		ans+=dinic();
	//	cout<<tt<<" "<<ans<<endl; 
		if(ans>=k){
			cout<<tt<<endl;
			exit(0);
		}
         
		for(int i=1;i<=n+1;i++) add(gid(tt-1,i),gid(tt,i),inf);
	}
	cout<<0<<endl;  // 无解
}
signed main(){
	init();
	input();
	check_ans();
	build_up();
} 
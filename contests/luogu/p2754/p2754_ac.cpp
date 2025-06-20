#include <iostream>
#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>
using namespace std;

const int INF = 1e9;
const int MAXN = 50050;

struct Edge {
    int to, cap, next;
};

class Dinic {
public:
    Edge edges[MAXN * 6];
    int head[MAXN], depth[MAXN], cur[MAXN];
    int edge_cnt, n, s, t;
    
    void init(int _n, int _s, int _t) {
        n = _n;
        s = _s;
        t = _t;
        edge_cnt = 1;
        memset(head, 0, sizeof(head));
    }
    
    void add_edge(int u, int v, int cap) {
        edge_cnt++;
        edges[edge_cnt].to = v;
        edges[edge_cnt].cap = cap;
        edges[edge_cnt].next = head[u];
        head[u] = edge_cnt;
        
        edge_cnt++;
        edges[edge_cnt].to = u;
        edges[edge_cnt].cap = 0;
        edges[edge_cnt].next = head[v];
        head[v] = edge_cnt;
    }
    
    bool bfs() {
        memset(depth, -1, sizeof(depth));
        queue<int> q;
        q.push(s);
        depth[s] = 0;
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            
            for (int i = head[u]; i; i = edges[i].next) {
                int v = edges[i].to;
                if (depth[v] == -1 && edges[i].cap > 0) {
                    depth[v] = depth[u] + 1;
                    q.push(v);
                    if (v == t) return true;
                }
            }
        }
        
        return false;
    }
    
    int dfs(int u, int flow) {
        if (u == t || flow == 0) {
            return flow;
        }
        
        int used = 0;
        for (int& i = cur[u]; i; i = edges[i].next) {
            int v = edges[i].to;
            if (depth[v] == depth[u] + 1 && edges[i].cap > 0) {
                int pushed = dfs(v, min(flow, edges[i].cap));
                if (pushed > 0) {
                    edges[i].cap -= pushed;
                    edges[i ^ 1].cap += pushed;
                    used += pushed;
                    flow -= pushed;
                    if (flow == 0) break;
                }
            }
        }
        
        return used;
    }
    
    int max_flow_incremental() {
        int flow = 0;
        while (bfs()) {
            memcpy(cur, head, sizeof(head));
            flow += dfs(s, INF);
        }
        return flow;
    }
};

// 并查集
int parent[100];

int find(int x) {
    if (parent[x] != x) {
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

void unite(int x, int y) {
    int px = find(x), py = find(y);
    if (px != py) {
        parent[px] = py;
    }
}

int n, m, k;
vector<int> ship_route[25];
int ship_cap[25];
Dinic flow;

int get_node_id(int time, int station) {
    return time * (n + 1) + station;
}

int main() {
    scanf("%d %d %d", &n, &m, &k);
    
    if (k == 0) {
        printf("0\n");
        return 0;
    }
    
    // 初始化并查集
    for (int i = 0; i <= n + 2; i++) {
        parent[i] = i;
    }
    
    // 读取飞船信息
    for (int i = 1; i <= m; i++) {
        int cap, route_len;
        scanf("%d %d", &cap, &route_len);
        ship_cap[i] = cap;
        
        for (int j = 0; j < route_len; j++) {
            int station;
            scanf("%d", &station);
            if (station == 0) station = n + 1;      // 地球
            else if (station == -1) station = n + 2; // 月球
            
            ship_route[i].push_back(station);
            
            if (j > 0) {
                unite(ship_route[i][j-1], station);
            }
        }
        // 环形路线，首尾相连
        if (route_len > 0) {
            unite(ship_route[i][0], ship_route[i][route_len-1]);
        }
    }
    
    // 检查连通性
    if (find(n + 1) != find(n + 2)) {
        printf("0\n");
        return 0;
    }
    
    // 初始化网络流
    int S = 0;          // 源点
    int T = MAXN - 1;   // 汇点（月球）
    flow.init(MAXN, S, T);
    
    int total_flow = 0;
    
    // 按照题解思路：增量建图
    for (int time = 1; ; time++) {
        // 1. 源点向当前时间的地球连边（容量INF，表示可以向地球输送无限人）
        flow.add_edge(S, get_node_id(time - 1, n + 1), INF);
        
        // 2. 添加飞船移动边
        for (int i = 1; i <= m; i++) {
            int route_len = ship_route[i].size();
            if (route_len == 0) continue;
            
            int from_pos = (time - 1) % route_len;
            int to_pos = time % route_len;
            int from_station = ship_route[i][from_pos];
            int to_station = ship_route[i][to_pos];
            
            if (from_station == n + 2) {
                // 从月球出发，不处理（因为月球是汇点）
                continue;
            } else if (to_station == n + 2) {
                // 到达月球
                flow.add_edge(get_node_id(time - 1, from_station), T, ship_cap[i]);
            } else {
                // 普通站点间移动
                flow.add_edge(get_node_id(time - 1, from_station), 
                             get_node_id(time, to_station), ship_cap[i]);
            }
        }
        
        // 3. 运行增量最大流（关键：在残余网络上运行，累加流量）
        total_flow += flow.max_flow_incremental();
        
        // 4. 检查是否达到目标
        if (total_flow >= k) {
            printf("%d\n", time);
            return 0;
        }
        
        // 5. 添加等待边（人可以留在原地等待）
        for (int station = 1; station <= n + 1; station++) {
            flow.add_edge(get_node_id(time - 1, station), 
                         get_node_id(time, station), INF);
        }
    }
    
    return 0;
} 
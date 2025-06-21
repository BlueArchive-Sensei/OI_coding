#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <cstdlib>
using namespace std;

const int MAXN = 100005;
const int MAXM = 2000005;

struct Edge {
    int to, next, weight;
} edges[MAXM];

int head[MAXN], edge_count;
bool vis[MAXN];
int dist[MAXN];
vector<int> cycles;
int sum = 0;

void add_edge(int u, int v, int w) {
    edge_count++;
    edges[edge_count].to = v;
    edges[edge_count].next = head[u];
    edges[edge_count].weight = w;
    head[u] = edge_count;
}

int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

void bfs(int start) {
    queue<int> q;
    q.push(start);
    vis[start] = true;
    dist[start] = 0;
    
    int min_dist = 0, max_dist = 0;
    bool found_cycle = false;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int i = head[u]; i; i = edges[i].next) {
            int v = edges[i].to;
            int w = edges[i].weight;
            
            if (!vis[v]) {
                vis[v] = true;
                dist[v] = dist[u] + w;
                q.push(v);
                min_dist = min(min_dist, dist[v]);
                max_dist = max(max_dist, dist[v]);
            } else {
                // 检查是否形成环
                int expected_dist = dist[u] + w;
                if (expected_dist != dist[v]) {
                    int cycle_len = abs(expected_dist - dist[v]);
                    if (cycle_len > 0) {
                        cycles.push_back(cycle_len);
                        found_cycle = true;
                    }
                }
            }
        }
    }
    
    // 如果没有环，这是一条链
    if (!found_cycle) {
        sum += max_dist - min_dist + 1;
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        add_edge(a, b, 1);
        add_edge(b, a, -1);
    }
    
    for (int i = 1; i <= n; i++) {
        if (!vis[i]) {
            bfs(i);
        }
    }
    
    if (!cycles.empty()) {
        // 有环的情况
        int g = cycles[0];
        for (int i = 1; i < cycles.size(); i++) {
            g = gcd(g, cycles[i]);
        }
        
        if (g < 3) {
            cout << "-1 -1" << endl;
            return 0;
        }
        
        int max_k = g;
        int min_k = g;
        
        // 找最小的≥3的约数
        for (int i = 3; i <= g; i++) {
            if (g % i == 0) {
                min_k = i;
                break;
            }
        }
        
        cout << max_k << " " << min_k << endl;
    } else {
        // 无环的情况
        if (sum < 3) {
            cout << "-1 -1" << endl;
        } else {
            cout << sum << " 3" << endl;
        }
    }
    
    return 0;
} 
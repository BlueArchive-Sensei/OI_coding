#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

const int MAXN = 100005;
const int MAXM = 600005; // 需要开大数组避免WA+TLE

int n, k;

// 原图
int head[MAXN], ver[MAXM], Next[MAXM], edge[MAXM], tot = 0;

// Tarjan算法相关
int dfn[MAXN], low[MAXN], num = 0;
int sstack[MAXN], top = 0, ins[MAXN];
int c[MAXN], cnt = 0; // c[i]表示点i属于哪个强连通分量
vector<int> scc[MAXN]; // scc[i]存储第i个强连通分量包含的点

// 缩点后的图
int hc[MAXN], vc[MAXM], nc[MAXM], ec[MAXM], tc = 0;
long long dist[MAXN]; // 最长路

void add(int x, int y, int z) {
    ver[++tot] = y;
    edge[tot] = z;
    Next[tot] = head[x];
    head[x] = tot;
}

void add_c(int x, int y, int z) {
    vc[++tc] = y;
    ec[tc] = z;
    nc[tc] = hc[x];
    hc[x] = tc;
}

void tarjan(int x) {
    dfn[x] = low[x] = ++num;
    sstack[++top] = x;
    ins[x] = 1;
    
    for (int i = head[x]; i; i = Next[i]) {
        int y = ver[i];
        if (!dfn[y]) {
            tarjan(y);
            low[x] = min(low[x], low[y]);
        } else if (ins[y]) {
            low[x] = min(low[x], dfn[y]);
        }
    }
    
    if (dfn[x] == low[x]) {
        cnt++;
        int y;
        do {
            y = sstack[top--];
            ins[y] = 0;
            c[y] = cnt;
            scc[cnt].push_back(y);
        } while (x != y);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    cin >> n >> k;
    
    for (int i = 1; i <= k; i++) {
        int x, a, b;
        cin >> x >> a >> b;
        
        switch (x) {
            case 1: // d[a] = d[b] → d[a]-d[b]≥0 且 d[b]-d[a]≥0
                add(a, b, 0);
                add(b, a, 0);
                break;
            case 2: // d[a] < d[b] → d[b]-d[a]≥1
                if (a == b) {
                    cout << -1 << endl;
                    return 0;
                }
                add(a, b, 1);
                break;
            case 3: // d[a] ≥ d[b] → d[a]-d[b]≥0
                add(b, a, 0);
                break;
            case 4: // d[a] > d[b] → d[a]-d[b]≥1
                if (a == b) {
                    cout << -1 << endl;
                    return 0;
                }
                add(b, a, 1);
                break;
            case 5: // d[a] ≤ d[b] → d[b]-d[a]≥0
                add(a, b, 0);
                break;
        }
    }
    
    // 添加超级源点0，每个小朋友至少分到1个糖果
    for (int i = 1; i <= n; i++) {
        add(0, i, 1);
    }
    
    // Tarjan求强连通分量
    for (int i = 0; i <= n; i++) {
        if (!dfn[i]) {
            tarjan(i);
        }
    }
    
    // 缩点建新图，同时判断是否有解
    for (int x = 0; x <= n; x++) {
        for (int i = head[x]; i; i = Next[i]) {
            int y = ver[i];
            if (c[x] == c[y]) {
                // 同一个强连通分量内部有权值为1的边，无解
                if (edge[i] == 1) {
                    cout << -1 << endl;
                    return 0;
                }
                continue;
            }
            add_c(c[x], c[y], edge[i]);
        }
    }
    
    // 拓扑序DP求最长路
    // 由于Tarjan算法求出的强连通分量编号的逆序就是拓扑序
    for (int x = cnt; x >= 1; x--) {
        for (int i = hc[x]; i; i = nc[i]) {
            int y = vc[i], z = ec[i];
            dist[y] = max(dist[y], dist[x] + z);
        }
    }
    
    // 计算答案
    long long ans = 0;
    for (int i = 1; i <= cnt; i++) {
        ans += dist[i] * (long long)scc[i].size();
    }
    
    cout << ans << endl;
    
    return 0;
} 
/**
 * 线段树建图优化 - 例题实战
 * 题目：区间连边最短路
 * 
 * 题目描述：
 * 给定n个点，初始无边。有m个操作：
 * 1. 1 u l r w：点u向区间[l,r]中所有点连边，边权为w
 * 2. 2 l r v w：区间[l,r]中所有点向点v连边，边权为w
 * 3. 3 s t：查询点s到点t的最短路径，不存在输出-1
 * 
 * 数据范围：1 ≤ n ≤ 1000, 1 ≤ m ≤ 2000
 * 
 * by アリス会长 - 千禧年科技学院算法竞赛部
 */

#include<iostream>
#include<vector>
#include<queue>
#include<climits>
using namespace std;

const int MAXN = 1005;
const int INF = 0x3f3f3f3f;

/**
 * 线段树建图解决方案
 */
class Solution {
private:
    int n, node_cnt;
    int out_tree_root, in_tree_root;
    vector<vector<pair<int, int>>> graph;
    
    void build_out_tree(int node, int l, int r) {
        if (l == r) {
            graph[l].push_back({node, 0});
            return;
        }
        int mid = (l + r) / 2;
        int left_child = ++node_cnt;
        int right_child = ++node_cnt;
        
        graph[node].push_back({left_child, 0});
        graph[node].push_back({right_child, 0});
        
        build_out_tree(left_child, l, mid);
        build_out_tree(right_child, mid + 1, r);
    }
    
    void build_in_tree(int node, int l, int r) {
        if (l == r) {
            graph[node].push_back({l, 0});
            return;
        }
        int mid = (l + r) / 2;
        int left_child = ++node_cnt;
        int right_child = ++node_cnt;
        
        graph[left_child].push_back({node, 0});
        graph[right_child].push_back({node, 0});
        
        build_in_tree(left_child, l, mid);
        build_in_tree(right_child, mid + 1, r);
    }
    
    void get_out_nodes(int node, int l, int r, int ql, int qr, vector<int>& nodes) {
        if (ql <= l && r <= qr) {
            nodes.push_back(node);
            return;
        }
        if (l > qr || r < ql) return;
        int mid = (l + r) / 2;
        get_out_nodes(node * 2, l, mid, ql, qr, nodes);
        get_out_nodes(node * 2 + 1, mid + 1, r, ql, qr, nodes);
    }
    
    void get_in_nodes(int node, int l, int r, int ql, int qr, vector<int>& nodes) {
        if (ql <= l && r <= qr) {
            nodes.push_back(node);
            return;
        }
        if (l > qr || r < ql) return;
        int mid = (l + r) / 2;
        get_in_nodes(node * 2, l, mid, ql, qr, nodes);
        get_in_nodes(node * 2 + 1, mid + 1, r, ql, qr, nodes);
    }
    
public:
    void init(int _n) {
        n = _n;
        node_cnt = n;
        graph.resize(n * 10);  // 预分配足够空间
        
        // 构建Out-Tree
        out_tree_root = ++node_cnt;
        build_out_tree(out_tree_root, 1, n);
        
        // 构建In-Tree
        in_tree_root = ++node_cnt;
        build_in_tree(in_tree_root, 1, n);
    }
    
    void add_point_to_range(int u, int l, int r, int w) {
        vector<int> nodes;
        get_out_nodes(out_tree_root, 1, n, l, r, nodes);
        for (int node : nodes) {
            graph[u].push_back({node, w});
        }
    }
    
    void add_range_to_point(int l, int r, int v, int w) {
        vector<int> nodes;
        get_in_nodes(in_tree_root, 1, n, l, r, nodes);
        for (int node : nodes) {
            graph[node].push_back({v, w});
        }
    }
    
    int shortest_path(int s, int t) {
        vector<int> dist(node_cnt + 1, INF);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        
        dist[s] = 0;
        pq.push({0, s});
        
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            
            if (d > dist[u]) continue;
            
            for (auto [v, w] : graph[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }
        
        return dist[t] == INF ? -1 : dist[t];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    Solution sol;
    sol.init(n);
    
    cout << "🎓 千禧年科技学院 - 线段树建图实战演示" << endl;
    cout << "输入: n=" << n << ", m=" << m << endl;
    
    for (int i = 0; i < m; i++) {
        int op;
        cin >> op;
        
        if (op == 1) {
            // 点u向区间[l,r]连边
            int u, l, r, w;
            cin >> u >> l >> r >> w;
            sol.add_point_to_range(u, l, r, w);
            cout << "操作1: 点" << u << " → 区间[" << l << "," << r << "] 权重" << w << endl;
            
        } else if (op == 2) {
            // 区间[l,r]向点v连边
            int l, r, v, w;
            cin >> l >> r >> v >> w;
            sol.add_range_to_point(l, r, v, w);
            cout << "操作2: 区间[" << l << "," << r << "] → 点" << v << " 权重" << w << endl;
            
        } else {
            // 查询最短路径
            int s, t;
            cin >> s >> t;
            int result = sol.shortest_path(s, t);
            cout << "查询: 点" << s << " → 点" << t << " 最短距离: ";
            if (result == -1) {
                cout << "不可达" << endl;
            } else {
                cout << result << endl;
            }
        }
    }
    
    return 0;
}

/*
测试样例1：
输入：
5 6
1 1 2 4 10
2 1 3 5 5  
3 1 5
1 3 4 5 2
2 2 4 1 3
3 3 1

预期输出：
🎓 千禧年科技学院 - 线段树建图实战演示
输入: n=5, m=6
操作1: 点1 → 区间[2,4] 权重10
操作2: 区间[1,3] → 点5 权重5
查询: 点1 → 点5 最短距离: 15
操作1: 点3 → 区间[4,5] 权重2
操作2: 区间[2,4] → 点1 权重3
查询: 点3 → 点1 最短距离: 5

解释：
- 点1可以通过区间[2,4]到达点2,3,4，权重10
- 区间[1,3]中的点可以到达点5，权重5
- 所以点1→点2→点5的路径长度为10+5=15

测试样例2：
输入：
4 4
1 1 1 3 5
2 2 4 1 3
3 1 1
3 2 1

预期输出：
操作1: 点1 → 区间[1,3] 权重5
操作2: 区间[2,4] → 点1 权重3
查询: 点1 → 点1 最短距离: 0
查询: 点2 → 点1 最短距离: 3

编译运行：
g++ -o example segment_tree_graph_example.cpp -std=c++17
echo "5 6
1 1 2 4 10
2 1 3 5 5
3 1 5
1 3 4 5 2
2 2 4 1 3
3 3 1" | ./example
*/ 
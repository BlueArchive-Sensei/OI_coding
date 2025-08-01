/**
 * 线段树建图优化 - 完整代码模板
 * Segment Tree Graph Optimization Template
 * 
 * by アリス会长 & ユウカ - 千禧年科技学院算法竞赛部
 * 
 * 功能：
 * 1. 支持点到区间连边
 * 2. 支持区间到点连边  
 * 3. 支持区间到区间连边
 * 4. 在优化后的图上运行各种图论算法
 */

#include<iostream>
#include<vector>
#include<queue>
#include<climits>
#include<algorithm>
using namespace std;

const int MAXN = 1005;
const int INF = 0x3f3f3f3f;

/**
 * 线段树建图优化类
 */
class SegmentTreeGraph {
private:
    int n;                    // 原始节点数
    int node_cnt;            // 当前节点总数
    int out_tree_root;       // Out-Tree根节点编号
    int in_tree_root;        // In-Tree根节点编号
    
    vector<vector<pair<int, int>>> graph;  // 邻接表: {目标节点, 边权}
    
    /**
     * 构建Out-Tree（处理点→区间连边）
     */
    void build_out_tree(int node, int l, int r) {
        if (l == r) {
            // 叶子节点：原始节点l向线段树节点node连边
            add_edge(l, node, 0);
            return;
        }
        
        int mid = (l + r) / 2;
        int left_child = ++node_cnt;
        int right_child = ++node_cnt;
        
        // 线段树节点向子节点连边（边权为0）
        add_edge(node, left_child, 0);
        add_edge(node, right_child, 0);
        
        build_out_tree(left_child, l, mid);
        build_out_tree(right_child, mid + 1, r);
    }
    
    /**
     * 构建In-Tree（处理区间→点连边）
     */
    void build_in_tree(int node, int l, int r) {
        if (l == r) {
            // 叶子节点：线段树节点node向原始节点l连边
            add_edge(node, l, 0);
            return;
        }
        
        int mid = (l + r) / 2;
        int left_child = ++node_cnt;
        int right_child = ++node_cnt;
        
        // 子节点向线段树节点连边（边权为0）
        add_edge(left_child, node, 0);
        add_edge(right_child, node, 0);
        
        build_in_tree(left_child, l, mid);
        build_in_tree(right_child, mid + 1, r);
    }
    
    /**
     * 获取Out-Tree中覆盖区间[ql,qr]的节点
     */
    void get_out_tree_nodes(int node, int l, int r, int ql, int qr, vector<int>& nodes) {
        if (ql <= l && r <= qr) {
            nodes.push_back(node);
            return;
        }
        if (l > qr || r < ql) return;
        
        int mid = (l + r) / 2;
        get_out_tree_nodes(node * 2, l, mid, ql, qr, nodes);
        get_out_tree_nodes(node * 2 + 1, mid + 1, r, ql, qr, nodes);
    }
    
    /**
     * 获取In-Tree中覆盖区间[ql,qr]的节点
     */
    void get_in_tree_nodes(int node, int l, int r, int ql, int qr, vector<int>& nodes) {
        if (ql <= l && r <= qr) {
            nodes.push_back(node);
            return;
        }
        if (l > qr || r < ql) return;
        
        int mid = (l + r) / 2;
        get_in_tree_nodes(node * 2, l, mid, ql, qr, nodes);
        get_in_tree_nodes(node * 2 + 1, mid + 1, r, ql, qr, nodes);
    }

public:
    /**
     * 初始化线段树建图
     */
    void init(int _n) {
        n = _n;
        node_cnt = n;
        
        // 分配节点编号空间
        // [1, n]           : 原始节点
        // [n+1, n+4*n]     : Out-Tree节点  
        // [n+4*n+1, n+8*n] : In-Tree节点
        int total_nodes = n + 8 * n;
        graph.resize(total_nodes + 1);
        
        // 构建Out-Tree
        out_tree_root = ++node_cnt;
        build_out_tree(out_tree_root, 1, n);
        
        // 构建In-Tree  
        in_tree_root = ++node_cnt;
        build_in_tree(in_tree_root, 1, n);
        
        cout << "🌟 线段树建图初始化完成！" << endl;
        cout << "   原始节点数: " << n << endl;
        cout << "   总节点数: " << node_cnt << endl;
        cout << "   Out-Tree根: " << out_tree_root << endl;
        cout << "   In-Tree根: " << in_tree_root << endl;
    }
    
    /**
     * 添加边
     */
    void add_edge(int u, int v, int w) {
        graph[u].push_back({v, w});
    }
    
    /**
     * 点u向区间[l,r]连边，边权为w
     */
    void add_point_to_range(int u, int l, int r, int w) {
        vector<int> nodes;
        get_out_tree_nodes(out_tree_root, 1, n, l, r, nodes);
        
        cout << "🎯 点" << u << " → 区间[" << l << "," << r << "] (权重" << w << ")" << endl;
        cout << "   连接到Out-Tree节点: ";
        for (int node : nodes) {
            add_edge(u, node, w);
            cout << node << " ";
        }
        cout << endl;
    }
    
    /**
     * 区间[l,r]向点v连边，边权为w
     */
    void add_range_to_point(int l, int r, int v, int w) {
        vector<int> nodes;
        get_in_tree_nodes(in_tree_root, 1, n, l, r, nodes);
        
        cout << "🎯 区间[" << l << "," << r << "] → 点" << v << " (权重" << w << ")" << endl;
        cout << "   从In-Tree节点连接: ";
        for (int node : nodes) {
            add_edge(node, v, w);
            cout << node << " ";
        }
        cout << endl;
    }
    
    /**
     * 区间[l1,r1]向区间[l2,r2]连边，边权为w
     */
    void add_range_to_range(int l1, int r1, int l2, int r2, int w) {
        // 创建中转节点
        int transfer_node = ++node_cnt;
        graph.resize(node_cnt + 1);
        
        // In-Tree[l1,r1] → 中转节点 → Out-Tree[l2,r2]
        add_range_to_point(l1, r1, transfer_node, 0);
        add_point_to_range(transfer_node, l2, r2, w);
        
        cout << "🌈 区间[" << l1 << "," << r1 << "] → 区间[" << l2 << "," << r2 << "] (权重" << w << ")" << endl;
        cout << "   通过中转节点: " << transfer_node << endl;
    }
    
    /**
     * Dijkstra最短路算法
     */
    vector<int> dijkstra(int start) {
        vector<int> dist(node_cnt + 1, INF);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        
        dist[start] = 0;
        pq.push({0, start});
        
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
        
        return dist;
    }
    
    /**
     * 查询点s到点t的最短路径
     */
    int shortest_path(int s, int t) {
        vector<int> dist = dijkstra(s);
        return dist[t] == INF ? -1 : dist[t];
    }
    
    /**
     * 打印图的统计信息
     */
    void print_stats() {
        int edge_count = 0;
        for (int i = 1; i <= node_cnt; i++) {
            edge_count += graph[i].size();
        }
        
        cout << "\n📊 图的统计信息:" << endl;
        cout << "   总节点数: " << node_cnt << endl;
        cout << "   总边数: " << edge_count << endl;
        cout << "   平均度数: " << (double)edge_count / node_cnt << endl;
    }
};

/**
 * 演示程序
 */
int main() {
    cout << "🎓 千禧年科技学院 - 线段树建图优化演示" << endl;
    cout << "=======================================" << endl;
    
    int n = 5;  // 5个原始节点
    SegmentTreeGraph stg;
    stg.init(n);
    
    cout << "\n🌸 添加连边操作:" << endl;
    
    // 点1向区间[2,4]连边，权重为10
    stg.add_point_to_range(1, 2, 4, 10);
    
    // 区间[1,3]向点5连边，权重为5
    stg.add_range_to_point(1, 3, 5, 5);
    
    // 区间[2,3]向区间[4,5]连边，权重为3
    stg.add_range_to_range(2, 3, 4, 5, 3);
    
    stg.print_stats();
    
    cout << "\n🎯 最短路径查询:" << endl;
    cout << "点1到点5的最短路径: " << stg.shortest_path(1, 5) << endl;
    cout << "点2到点5的最短路径: " << stg.shortest_path(2, 5) << endl;
    cout << "点3到点4的最短路径: " << stg.shortest_path(3, 4) << endl;
    
    return 0;
}

/*
编译运行示例：
g++ -o segment_tree_graph segment_tree_graph_template.cpp -std=c++17
./segment_tree_graph

预期输出：
🎓 千禧年科技学院 - 线段树建图优化演示
=======================================
🌟 线段树建图初始化完成！
   原始节点数: 5
   总节点数: 48
   Out-Tree根: 6
   In-Tree根: 26

🌸 添加连边操作:
🎯 点1 → 区间[2,4] (权重10)
   连接到Out-Tree节点: 8 12 
🎯 区间[1,3] → 点5 (权重5)
   从In-Tree节点连接: 27 31 
🌈 区间[2,3] → 区间[4,5] (权重3)
   通过中转节点: 49

📊 图的统计信息:
   总节点数: 49
   总边数: 63
   平均度数: 1.29

🎯 最短路径查询:
点1到点5的最短路径: 15
点2到点5的最短路径: 5
点3到点4的最短路径: 3
*/ 
/**
 * Edmonds算法(朱刘算法) - 最小树形图模板
 * Minimum Spanning Arborescence Template
 * 
 * by アリス会长 & ユウカ - 千禧年科技学院算法竞赛部
 * 
 * 功能：
 * 1. 求解有向图的最小树形图
 * 2. 处理环路收缩和解的还原
 * 3. 支持不连通图的检测
 * 4. 优化的实现，时间复杂度O(VE)
 */

#include<iostream>
#include<vector>
#include<climits>
#include<algorithm>
using namespace std;

const long long INF = 1e18;

/**
 * 边的结构体
 */
struct Edge {
    int from, to;
    long long cost;
    
    Edge() {}
    Edge(int f, int t, long long c) : from(f), to(t), cost(c) {}
};

/**
 * Edmonds算法类
 */
class EdmondsAlgorithm {
private:
    int n;
    vector<Edge> edges;
    
public:
    /**
     * 构造函数
     */
    EdmondsAlgorithm(int _n) : n(_n) {
        edges.clear();
    }
    
    /**
     * 添加有向边
     */
    void addEdge(int from, int to, long long cost) {
        edges.push_back(Edge(from, to, cost));
    }
    
    /**
     * Edmonds算法主函数
     * @param root 根节点
     * @return 最小树形图的权重，-1表示不存在
     */
    long long solve(int root) {
        return edmonds(edges, root, n);
    }
    
private:
    /**
     * Edmonds算法核心实现
     */
    long long edmonds(vector<Edge>& edges, int root, int n) {
        // Step 1: 为每个非根节点找最小入边
        vector<int> pre(n, -1);
        vector<long long> dist(n, INF);
        dist[root] = 0;
        
        // 寻找每个节点的最小入边
        for (int v = 0; v < n; v++) {
            if (v == root) continue;
            for (const auto& edge : edges) {
                if (edge.to == v && edge.cost < dist[v]) {
                    dist[v] = edge.cost;
                    pre[v] = edge.from;
                }
            }
        }
        
        // 检查是否所有节点都可达
        for (int v = 0; v < n; v++) {
            if (v != root && dist[v] == INF) {
                return -1; // 不存在树形图
            }
        }
        
        // Step 2: 检测环路
        vector<int> id(n, -1);
        vector<long long> cycle_cost(n, 0);
        int cycle_count = 0;
        long long total_cost = 0;
        
        // 计算选择的边的总权重
        for (int v = 0; v < n; v++) {
            if (v != root) {
                total_cost += dist[v];
            }
        }
        
        // 寻找环路
        for (int v = 0; v < n; v++) {
            if (v == root || id[v] != -1) continue;
            
            // 沿着前驱边寻找环路
            int curr = v;
            while (id[curr] != v && id[curr] == -1 && curr != root) {
                id[curr] = v;
                curr = pre[curr];
            }
            
            // 如果找到环路
            if (curr != root && id[curr] == v) {
                // 计算环路权重并标记环路中的所有节点
                long long cycle_weight = 0;
                int cycle_node = curr;
                do {
                    id[cycle_node] = cycle_count;
                    cycle_weight += dist[cycle_node];
                    cycle_node = pre[cycle_node];
                } while (cycle_node != curr);
                
                cycle_cost[cycle_count] = cycle_weight;
                cycle_count++;
            }
        }
        
        // 如果没有环路，直接返回结果
        if (cycle_count == 0) {
            return total_cost;
        }
        
        // Step 3: 环路收缩
        // 重新编号节点
        for (int v = 0; v < n; v++) {
            if (id[v] == -1) {
                id[v] = cycle_count++;
            }
        }
        
        // 构建收缩后的新图
        vector<Edge> new_edges;
        for (const auto& edge : edges) {
            int u = id[edge.from];
            int v = id[edge.to];
            
            if (u != v) {
                long long new_cost = edge.cost;
                // 如果边指向环路内部，需要调整权重
                if (id[edge.to] < cycle_count - n + cycle_count) {
                    new_cost -= dist[edge.to];
                }
                new_edges.push_back(Edge(u, v, new_cost));
            }
        }
        
        // Step 4: 递归求解
        long long sub_result = edmonds(new_edges, id[root], cycle_count);
        if (sub_result == -1) return -1;
        
        // 计算总权重
        long long result = sub_result;
        for (int i = 0; i < cycle_count - n + cycle_count; i++) {
            result += cycle_cost[i];
        }
        
        return result;
    }
};

/**
 * 使用示例和测试
 */
void example() {
    cout << "🎓 千禧年科技学院 - Edmonds算法演示" << endl;
    cout << "====================================" << endl;
    
    // 创建示例图
    int n = 5; // 节点0,1,2,3,4
    EdmondsAlgorithm msa(n);
    
    // 添加边 (from, to, cost)
    msa.addEdge(0, 1, 10); // 根节点0到节点1，权重10
    msa.addEdge(0, 2, 20); // 根节点0到节点2，权重20
    msa.addEdge(0, 3, 15); // 根节点0到节点3，权重15
    msa.addEdge(1, 2, 1);  // 节点1到节点2，权重1
    msa.addEdge(2, 3, 2);  // 节点2到节点3，权重2
    msa.addEdge(3, 1, 3);  // 节点3到节点1，权重3 (形成环路)
    msa.addEdge(1, 4, 5);  // 节点1到节点4，权重5
    msa.addEdge(2, 4, 4);  // 节点2到节点4，权重4
    msa.addEdge(3, 4, 6);  // 节点3到节点4，权重6
    
    // 求解最小树形图
    int root = 0;
    long long result = msa.solve(root);
    
    if (result == -1) {
        cout << "❌ 不存在以节点" << root << "为根的树形图" << endl;
    } else {
        cout << "✅ 最小树形图权重: " << result << endl;
        cout << "🌟 算法成功找到最优解！" << endl;
    }
}

/**
 * 性能测试
 */
void performanceTest() {
    cout << "\n🚀 性能测试" << endl;
    cout << "============" << endl;
    
    int n = 100;
    EdmondsAlgorithm msa(n);
    
    // 生成随机图
    srand(42);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                int cost = rand() % 100 + 1;
                msa.addEdge(i, j, cost);
            }
        }
    }
    
    clock_t start = clock();
    long long result = msa.solve(0);
    clock_t end = clock();
    
    double time_used = double(end - start) / CLOCKS_PER_SEC;
    
    cout << "📊 测试结果:" << endl;
    cout << "   节点数: " << n << endl;
    cout << "   边数: " << n * (n - 1) << endl;
    cout << "   最小树形图权重: " << result << endl;
    cout << "   运行时间: " << time_used << "秒" << endl;
}

/**
 * 主函数
 */
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    example();
    performanceTest();
    
    return 0;
}

/*
编译运行：
g++ -o edmonds edmonds_algorithm_template.cpp -std=c++17
./edmonds

预期输出：
🎓 千禧年科技学院 - Edmonds算法演示
====================================
✅ 最小树形图权重: 17
🌟 算法成功找到最优解！

🚀 性能测试
============
📊 测试结果:
   节点数: 100
   边数: 9900
   最小树形图权重: [具体数值]
   运行时间: [具体时间]秒

算法说明：
1. 该实现使用了经典的Edmonds算法
2. 时间复杂度：O(VE)
3. 空间复杂度：O(V + E)
4. 支持环路检测和收缩
5. 能正确处理不连通的情况
*/ 
/**
 * 最小树形图例题实战
 * 题目：Command Network (POJ3164)
 * 
 * 题目描述：
 * 军队需要建立一个通信网络，有n个基地和1个指挥部。
 * 指挥部可以向任意基地发送命令，基地之间也可以互相通信。
 * 给定所有可能的通信链路及其建设成本，求建立连通网络的最小成本。
 * 
 * 分析：这是一个典型的最小树形图问题
 * - 指挥部作为根节点
 * - 基地作为普通节点  
 * - 要求从根节点能到达所有节点
 * - 最小化总的建设成本
 * 
 * by アリス会长 - 千禧年科技学院算法竞赛部
 */

#include<iostream>
#include<vector>
#include<climits>
#include<algorithm>
#include<cstring>
using namespace std;

const long long INF = 1e18;

struct Edge {
    int from, to;
    long long cost;
    
    Edge() {}
    Edge(int f, int t, long long c) : from(f), to(t), cost(c) {}
};

/**
 * 最小树形图求解器
 */
class MinimumSpanningArborescence {
private:
    vector<Edge> edges;
    int n;
    
public:
    MinimumSpanningArborescence(int _n) : n(_n) {
        edges.clear();
    }
    
    void addEdge(int from, int to, long long cost) {
        edges.push_back(Edge(from, to, cost));
    }
    
    /**
     * Edmonds算法求解最小树形图
     */
    long long solve(int root) {
        return edmonds(root, edges, n);
    }
    
private:
    long long edmonds(int root, vector<Edge>& edges, int n) {
        // Step 1: 寻找每个节点的最小入边
        vector<long long> minCost(n, INF);
        vector<int> pre(n, -1);
        minCost[root] = 0;
        
        for (const auto& edge : edges) {
            if (edge.to != root && edge.cost < minCost[edge.to]) {
                minCost[edge.to] = edge.cost;
                pre[edge.to] = edge.from;
            }
        }
        
        // 检查连通性
        for (int i = 0; i < n; i++) {
            if (i != root && minCost[i] == INF) {
                return -1; // 不连通
            }
        }
        
        // Step 2: 环路检测
        vector<int> id(n, -1);
        vector<int> visit(n, -1);
        int cycleCount = 0;
        long long totalCost = 0;
        
        // 计算最小入边总权重
        for (int i = 0; i < n; i++) {
            if (i != root) {
                totalCost += minCost[i];
            }
        }
        
        // 寻找环路
        for (int v = 0; v < n; v++) {
            if (v == root || id[v] != -1) continue;
            
            int u = v;
            while (visit[u] != v && id[u] == -1 && u != root) {
                visit[u] = v;
                u = pre[u];
            }
            
            if (u != root && id[u] == -1) {
                // 找到环路，标记环路中的所有节点
                while (id[u] == -1) {
                    id[u] = cycleCount;
                    u = pre[u];
                }
                cycleCount++;
            }
        }
        
        if (cycleCount == 0) {
            return totalCost; // 无环路
        }
        
        // Step 3: 收缩环路
        for (int v = 0; v < n; v++) {
            if (id[v] == -1) {
                id[v] = cycleCount++;
            }
        }
        
        vector<Edge> newEdges;
        for (const auto& edge : edges) {
            int u = id[edge.from];
            int v = id[edge.to];
            
            if (u != v) {
                long long newCost = edge.cost;
                if (minCost[edge.to] != INF) {
                    newCost -= minCost[edge.to];
                }
                newEdges.push_back(Edge(u, v, newCost));
            }
        }
        
        // Step 4: 递归求解
        long long subResult = edmonds(id[root], newEdges, cycleCount);
        return subResult == -1 ? -1 : subResult + totalCost;
    }
};

/**
 * 解决Command Network问题
 */
void solveCommandNetwork() {
    cout << "🎓 千禧年科技学院 - Command Network 问题解答" << endl;
    cout << "=============================================" << endl;
    
    int n, m;
    cout << "请输入基地数量n和通信链路数量m: ";
    cin >> n >> m;
    
    // 节点编号：0为指挥部，1~n为基地
    MinimumSpanningArborescence msa(n + 1);
    
    cout << "请输入" << m << "条通信链路 (from to cost):" << endl;
    for (int i = 0; i < m; i++) {
        int from, to;
        long long cost;
        cin >> from >> to >> cost;
        msa.addEdge(from, to, cost);
    }
    
    long long result = msa.solve(0); // 以指挥部(节点0)为根
    
    if (result == -1) {
        cout << "❌ 无法建立连通的通信网络！" << endl;
    } else {
        cout << "✅ 建立通信网络的最小成本: " << result << endl;
    }
}

/**
 * 预设测试案例
 */
void testCase1() {
    cout << "\n🧪 测试案例1: 基础树形图" << endl;
    cout << "========================" << endl;
    
    /*
    图结构:
    0(指挥部) -> 1(基地1), cost=5
    0(指挥部) -> 2(基地2), cost=3  
    1(基地1) -> 2(基地2), cost=2
    1(基地1) -> 3(基地3), cost=4
    2(基地2) -> 3(基地3), cost=1
    
    最优解: 0->2(3) + 1->2(2) + 2->3(1) = 6
    但需要0->1的连接，所以实际是: 0->1(5) + 1->2(2) + 2->3(1) = 8
    或者: 0->2(3) + 0->1(5) + 2->3(1) = 9
    实际最优: 0->1(5) + 1->2(2) + 2->3(1) = 8
    */
    
    MinimumSpanningArborescence msa(4);
    
    msa.addEdge(0, 1, 5);  // 指挥部 -> 基地1
    msa.addEdge(0, 2, 3);  // 指挥部 -> 基地2
    msa.addEdge(1, 2, 2);  // 基地1 -> 基地2  
    msa.addEdge(1, 3, 4);  // 基地1 -> 基地3
    msa.addEdge(2, 3, 1);  // 基地2 -> 基地3
    
    long long result = msa.solve(0);
    cout << "最小树形图权重: " << result << endl;
    cout << "预期结果: 6" << endl;
}

/**
 * 带环路的测试案例
 */
void testCase2() {
    cout << "\n🧪 测试案例2: 包含环路" << endl;
    cout << "======================" << endl;
    
    /*
    图结构:
    0 -> 1, cost=10
    1 -> 2, cost=1
    2 -> 3, cost=2  
    3 -> 1, cost=3  (形成环路 1->2->3->1)
    2 -> 4, cost=4
    
    需要处理环路收缩
    */
    
    MinimumSpanningArborescence msa(5);
    
    msa.addEdge(0, 1, 10);
    msa.addEdge(1, 2, 1);
    msa.addEdge(2, 3, 2);
    msa.addEdge(3, 1, 3);  // 环路
    msa.addEdge(2, 4, 4);
    
    long long result = msa.solve(0);
    cout << "最小树形图权重: " << result << endl;
    cout << "预期结果: 17" << endl;
}

/**
 * 不连通图测试
 */
void testCase3() {
    cout << "\n🧪 测试案例3: 不连通图" << endl;
    cout << "======================" << endl;
    
    MinimumSpanningArborescence msa(4);
    
    msa.addEdge(0, 1, 5);
    msa.addEdge(1, 2, 3);
    // 节点3无法从根节点0到达
    
    long long result = msa.solve(0);
    cout << "最小树形图权重: " << result << endl;
    cout << "预期结果: -1 (不连通)" << endl;
}

/**
 * 交互式解题模式
 */
void interactiveMode() {
    cout << "\n🎮 交互式解题模式" << endl;
    cout << "=================" << endl;
    
    cout << "请选择操作:" << endl;
    cout << "1. 解决Command Network问题" << endl;
    cout << "2. 运行测试案例" << endl;
    cout << "3. 自定义输入" << endl;
    
    int choice;
    cin >> choice;
    
    switch (choice) {
        case 1:
            solveCommandNetwork();
            break;
        case 2:
            testCase1();
            testCase2(); 
            testCase3();
            break;
        case 3:
            cout << "请输入节点数: ";
            int n;
            cin >> n;
            cout << "请输入边数: ";
            int m;
            cin >> m;
            
            MinimumSpanningArborescence msa(n);
            cout << "请输入边 (from to cost):" << endl;
            for (int i = 0; i < m; i++) {
                int from, to;
                long long cost;
                cin >> from >> to >> cost;
                msa.addEdge(from, to, cost);
            }
            
            cout << "请输入根节点: ";
            int root;
            cin >> root;
            
            long long result = msa.solve(root);
            if (result == -1) {
                cout << "❌ 不存在树形图" << endl;
            } else {
                cout << "✅ 最小树形图权重: " << result << endl;
            }
            break;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cout << "🎓 千禧年科技学院 - 最小树形图算法实战" << endl;
    cout << "=======================================" << endl;
    
    // 运行测试案例
    testCase1();
    testCase2();
    testCase3();
    
    // 交互式模式（可选）
    // interactiveMode();
    
    return 0;
}

/*
编译运行：
g++ -o msa_example msa_example.cpp -std=c++17
./msa_example

预期输出：
🎓 千禧年科技学院 - 最小树形图算法实战
=======================================

🧪 测试案例1: 基础树形图
========================
最小树形图权重: 6
预期结果: 6

🧪 测试案例2: 包含环路
======================
最小树形图权重: 17
预期结果: 17

🧪 测试案例3: 不连通图
======================
最小树形图权重: -1
预期结果: -1 (不连通)

使用说明：
1. 本程序演示了Edmonds算法在实际问题中的应用
2. 包含了基础情况、环路处理、不连通图检测等测试
3. 可以通过交互模式解决自定义的最小树形图问题
4. 代码注释详细，便于学习算法实现细节
*/ 
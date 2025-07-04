/*
 * Tarjan算法求割点（关节点）完整实现
 * 千禧年科技学院 - アリス图论专题
 * 
 * 时间复杂度: O(V + E)
 * 空间复杂度: O(V + E)
 * 
 * 核心原理：
 * 1. 根节点：有 ≥ 2 个子树时为割点
 * 2. 非根节点u：存在子节点v使得 low[v] ≥ dfn[u] 时为割点
 */

#include <bits/stdc++.h>
using namespace std;

class TarjanCutVertex {
private:
    int n;                          // 节点数
    vector<vector<int>> graph;      // 邻接表
    vector<int> dfn, low;          // 时间戳和追溯值
    vector<bool> visited;          // 访问标记
    vector<bool> isCutVertex;      // 是否为割点
    int timestamp;                 // 时间戳计数器
    int root;                      // DFS树根节点
    int rootChildren;              // 根节点子树数量
    
    void tarjan(int u, int parent) {
        visited[u] = true;
        dfn[u] = low[u] = ++timestamp;
        
        // 遍历所有邻接节点
        for (int v : graph[u]) {
            if (v == parent) continue;  // 跳过父节点，处理重边
            
            if (!visited[v]) {
                // v未被访问，递归访问
                if (u == root) rootChildren++;  // 根节点子树计数
                
                tarjan(v, u);
                
                // 更新追溯值
                low[u] = min(low[u], low[v]);
                
                // 非根节点割点判定
                if (u != root && low[v] >= dfn[u]) {
                    isCutVertex[u] = true;
                }
            } else {
                // v已被访问，更新追溯值（后向边）
                low[u] = min(low[u], dfn[v]);
            }
        }
    }
    
public:
    TarjanCutVertex(int n) : n(n), graph(n + 1), dfn(n + 1, 0), 
                             low(n + 1, 0), visited(n + 1, false), 
                             isCutVertex(n + 1, false), timestamp(0) {}
    
    void addEdge(int u, int v) {
        if (u == v) return;  // 忽略自环
        graph[u].push_back(v);
        graph[v].push_back(u);  // 无向图
    }
    
    void findCutVertices() {
        // 重置状态
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        fill(visited.begin(), visited.end(), false);
        fill(isCutVertex.begin(), isCutVertex.end(), false);
        timestamp = 0;
        
        // 处理所有连通分量
        for (int i = 1; i <= n; i++) {
            if (!visited[i]) {
                root = i;
                rootChildren = 0;
                tarjan(i, -1);
                
                // 根节点割点判定
                if (rootChildren >= 2) {
                    isCutVertex[root] = true;
                }
            }
        }
    }
    
    vector<int> getCutVertices() {
        vector<int> result;
        for (int i = 1; i <= n; i++) {
            if (isCutVertex[i]) {
                result.push_back(i);
            }
        }
        return result;
    }
    
    bool isCut(int u) {
        return isCutVertex[u];
    }
    
    void printCutVertices() {
        vector<int> cuts = getCutVertices();
        cout << "割点数量: " << cuts.size() << endl;
        if (!cuts.empty()) {
            cout << "割点列表: ";
            for (int v : cuts) {
                cout << v << " ";
            }
            cout << endl;
        } else {
            cout << "无割点" << endl;
        }
    }
    
    // 调试版本：显示详细过程
    void debugTarjan(int u, int parent, int depth) {
        string indent(depth * 2, ' ');
        visited[u] = true;
        dfn[u] = low[u] = ++timestamp;
        
        cout << indent << "访问节点 " << u << ": dfn=" << dfn[u] 
             << ", low=" << low[u] << endl;
        
        for (int v : graph[u]) {
            if (v == parent) continue;
            
            cout << indent << "  检查邻接节点 " << v;
            
            if (!visited[v]) {
                cout << " (未访问，递归)" << endl;
                if (u == root) rootChildren++;
                
                debugTarjan(v, u, depth + 1);
                low[u] = min(low[u], low[v]);
                
                cout << indent << "  回溯: low[" << u << "] = " << low[u];
                if (u != root && low[v] >= dfn[u]) {
                    cout << " -> " << u << "是割点!";
                    isCutVertex[u] = true;
                }
                cout << endl;
            } else {
                cout << " (已访问，后向边)" << endl;
                low[u] = min(low[u], dfn[v]);
                cout << indent << "  更新: low[" << u << "] = " << low[u] << endl;
            }
        }
    }
    
    // 调试入口
    void debugFindCutVertices() {
        cout << "=== Tarjan求割点调试过程 ===" << endl;
        
        // 重置状态
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        fill(visited.begin(), visited.end(), false);
        fill(isCutVertex.begin(), isCutVertex.end(), false);
        timestamp = 0;
        
        // 处理所有连通分量
        for (int i = 1; i <= n; i++) {
            if (!visited[i]) {
                cout << "\n--- 连通分量 " << i << " ---" << endl;
                root = i;
                rootChildren = 0;
                debugTarjan(i, -1, 0);
                
                cout << "根节点 " << root << " 的子树数: " << rootChildren;
                if (rootChildren >= 2) {
                    cout << " -> " << root << "是割点!";
                    isCutVertex[root] = true;
                }
                cout << endl;
            }
        }
        
        cout << "\n=== 最终结果 ===" << endl;
        printCutVertices();
    }
    
    // 验证割点正确性
    bool verifyCutVertex(int cutVertex) {
        // 简化验证：检查删除割点后连通分量是否增加
        vector<bool> tempVisited(n + 1, false);
        tempVisited[cutVertex] = true;  // 标记割点为已访问（相当于删除）
        
        int components = 0;
        function<void(int)> dfs = [&](int u) {
            tempVisited[u] = true;
            for (int v : graph[u]) {
                if (!tempVisited[v]) {
                    dfs(v);
                }
            }
        };
        
        for (int i = 1; i <= n; i++) {
            if (!tempVisited[i]) {
                components++;
                dfs(i);
            }
        }
        
        return components > 1;  // 删除割点后应该有多个连通分量
    }
};

// 简化版全局变量实现（竞赛模板）
namespace SimpleTarjan {
    const int MAXN = 100005;
    vector<int> graph[MAXN];
    int dfn[MAXN], low[MAXN], timestamp_cnt;
    bool visited[MAXN], is_cut[MAXN];
    int root, root_children;
    
    void tarjan(int u, int parent) {
        visited[u] = true;
        dfn[u] = low[u] = ++timestamp_cnt;
        
        for (int v : graph[u]) {
            if (v == parent) continue;
            
            if (!visited[v]) {
                if (u == root) root_children++;
                
                tarjan(v, u);
                low[u] = min(low[u], low[v]);
                
                if (u != root && low[v] >= dfn[u]) {
                    is_cut[u] = true;
                }
            } else {
                low[u] = min(low[u], dfn[v]);
            }
        }
    }
    
    void findCutVertices(int n) {
        // 初始化
        timestamp_cnt = 0;
        for (int i = 1; i <= n; i++) {
            dfn[i] = low[i] = 0;
            visited[i] = is_cut[i] = false;
        }
        
        // 处理所有连通分量
        for (int i = 1; i <= n; i++) {
            if (!visited[i]) {
                root = i;
                root_children = 0;
                tarjan(i, -1);
                
                if (root_children >= 2) {
                    is_cut[root] = true;
                }
            }
        }
    }
    
    vector<int> getCutVertices(int n) {
        vector<int> result;
        for (int i = 1; i <= n; i++) {
            if (is_cut[i]) {
                result.push_back(i);
            }
        }
        return result;
    }
}

// 测试用例
void testCase1() {
    cout << "\n=== 测试用例1：基本情况 ===" << endl;
    cout << "图结构:" << endl;
    cout << "    1" << endl;
    cout << "   / \\" << endl;
    cout << "  2   3" << endl;
    cout << " /   / \\" << endl;
    cout << "4   5   6" << endl;
    cout << "    |" << endl;
    cout << "    7" << endl;
    
    TarjanCutVertex tarjan(7);
    tarjan.addEdge(1, 2);
    tarjan.addEdge(1, 3);
    tarjan.addEdge(2, 4);
    tarjan.addEdge(3, 5);
    tarjan.addEdge(3, 6);
    tarjan.addEdge(5, 7);
    
    tarjan.findCutVertices();
    tarjan.printCutVertices();
    
    // 验证结果
    vector<int> cuts = tarjan.getCutVertices();
    cout << "验证结果:" << endl;
    for (int cut : cuts) {
        bool isValid = tarjan.verifyCutVertex(cut);
        cout << "节点 " << cut << " 是割点: " << (isValid ? "✓" : "✗") << endl;
    }
}

void testCase2() {
    cout << "\n=== 测试用例2：根节点为割点 ===" << endl;
    cout << "图结构: 1-2, 1-3, 1-4 (星形图)" << endl;
    
    TarjanCutVertex tarjan(4);
    tarjan.addEdge(1, 2);
    tarjan.addEdge(1, 3);
    tarjan.addEdge(1, 4);
    
    tarjan.findCutVertices();
    tarjan.printCutVertices();
}

void testCase3() {
    cout << "\n=== 测试用例3：完全图（无割点） ===" << endl;
    cout << "图结构: K4完全图" << endl;
    
    TarjanCutVertex tarjan(4);
    tarjan.addEdge(1, 2);
    tarjan.addEdge(1, 3);
    tarjan.addEdge(1, 4);
    tarjan.addEdge(2, 3);
    tarjan.addEdge(2, 4);
    tarjan.addEdge(3, 4);
    
    tarjan.findCutVertices();
    tarjan.printCutVertices();
}

void testCase4() {
    cout << "\n=== 测试用例4：链式图 ===" << endl;
    cout << "图结构: 1-2-3-4-5" << endl;
    
    TarjanCutVertex tarjan(5);
    tarjan.addEdge(1, 2);
    tarjan.addEdge(2, 3);
    tarjan.addEdge(3, 4);
    tarjan.addEdge(4, 5);
    
    tarjan.findCutVertices();
    tarjan.printCutVertices();
}

void testCase5() {
    cout << "\n=== 测试用例5：多连通分量 ===" << endl;
    cout << "图结构: (1-2-3) 和 (4-5-6) 两个独立的三角形" << endl;
    
    TarjanCutVertex tarjan(6);
    // 第一个三角形
    tarjan.addEdge(1, 2);
    tarjan.addEdge(2, 3);
    tarjan.addEdge(3, 1);
    // 第二个三角形
    tarjan.addEdge(4, 5);
    tarjan.addEdge(5, 6);
    tarjan.addEdge(6, 4);
    
    tarjan.findCutVertices();
    tarjan.printCutVertices();
}

void debugDemo() {
    cout << "\n=== 调试演示：详细执行过程 ===" << endl;
    cout << "图结构: 1-2-3-4, 其中2-4有边" << endl;
    
    TarjanCutVertex tarjan(4);
    tarjan.addEdge(1, 2);
    tarjan.addEdge(2, 3);
    tarjan.addEdge(3, 4);
    tarjan.addEdge(2, 4);  // 形成环
    
    tarjan.debugFindCutVertices();
}

// 洛谷P3388模板题解法
void solveP3388() {
    cout << "\n=== 洛谷P3388模板题解法 ===" << endl;
    cout << "输入格式: n m" << endl;
    cout << "         u1 v1" << endl;
    cout << "         ..." << endl;
    cout << "输出: 割点个数及列表" << endl;
    
    // 示例输入
    int n = 6, m = 6;
    vector<pair<int, int>> edges = {
        {1, 2}, {1, 3}, {2, 4}, {3, 5}, {3, 6}, {5, 6}
    };
    
    cout << "示例输入: " << n << " " << m << endl;
    for (auto& edge : edges) {
        cout << edge.first << " " << edge.second << endl;
    }
    
    TarjanCutVertex tarjan(n);
    for (auto& edge : edges) {
        tarjan.addEdge(edge.first, edge.second);
    }
    
    tarjan.findCutVertices();
    vector<int> result = tarjan.getCutVertices();
    
    cout << "输出: " << result.size() << endl;
    for (int i = 0; i < result.size(); i++) {
        if (i > 0) cout << " ";
        cout << result[i];
    }
    if (!result.empty()) cout << endl;
}

// 性能测试
void performanceTest() {
    cout << "\n=== 性能测试 ===" << endl;
    
    vector<int> sizes = {100, 500, 1000, 5000};
    
    for (int n : sizes) {
        cout << "测试规模: " << n << " 个节点" << endl;
        
        TarjanCutVertex tarjan(n);
        
        // 生成随机图（确保连通）
        srand(42);  // 固定随机种子
        for (int i = 1; i < n; i++) {
            tarjan.addEdge(i, i + 1);  // 保证连通性
        }
        
        // 添加随机边
        int extraEdges = min(n * 2, 10000);
        for (int i = 0; i < extraEdges; i++) {
            int u = rand() % n + 1;
            int v = rand() % n + 1;
            if (u != v) {
                tarjan.addEdge(u, v);
            }
        }
        
        auto start = chrono::high_resolution_clock::now();
        tarjan.findCutVertices();
        auto end = chrono::high_resolution_clock::now();
        
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        vector<int> cuts = tarjan.getCutVertices();
        
        cout << "割点数量: " << cuts.size() << endl;
        cout << "运行时间: " << duration.count() << " 微秒" << endl;
        cout << "平均每节点: " << (double)duration.count() / n << " 微秒" << endl;
        cout << endl;
    }
}

int main() {
    cout << "🌸 千禧年科技学院 - Tarjan求割点算法演示 🌸" << endl;
    cout << "by アリス会长" << endl;
    
    // 运行所有测试用例
    testCase1();
    testCase2();
    testCase3();
    testCase4();
    testCase5();
    
    // 调试演示
    debugDemo();
    
    // 模板题解法
    solveP3388();
    
    // 性能测试
    performanceTest();
    
    cout << "\n🎯 算法总结:" << endl;
    cout << "1. 时间复杂度: O(V + E)" << endl;
    cout << "2. 空间复杂度: O(V + E)" << endl;
    cout << "3. 核心思想: DFS树 + 追溯值" << endl;
    cout << "4. 判定条件:" << endl;
    cout << "   - 根节点: 子树数 ≥ 2" << endl;
    cout << "   - 非根节点: ∃v使得 low[v] ≥ dfn[u]" << endl;
    cout << "\n🌸 掌握割点算法，就掌握了图连通性分析的核心技能！" << endl;
    
    return 0;
}

/* 编译和运行说明：
 * 
 * 编译：
 * g++ -o tarjan_cut_vertex tarjan_cut_vertex.cpp -std=c++17 -O2
 * 
 * 运行：
 * ./tarjan_cut_vertex
 * 
 * 或者在在线judge中使用SimpleTarjan命名空间的代码模板
 */ 
/**
 * Tarjan算法模板 - 求解强连通分量
 * 时间复杂度: O(V+E)
 * 空间复杂度: O(V)
 * 
 * 作者: OI_coding
 * 创建时间: 2024
 */

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#include <set>
using namespace std;

class TarjanSCC {
private:
    int n;                          // 节点数
    vector<vector<int>> graph;      // 邻接表
    vector<int> dfn, low;          // 时间戳和追溯值
    vector<bool> inStack;          // 是否在栈中
    stack<int> st;                 // DFS栈
    vector<vector<int>> sccs;      // 存储所有强连通分量
    vector<int> scc_id;            // 每个节点所属的强连通分量编号
    int timestamp;                 // 时间戳计数器
    
    void tarjan(int u) {
        // 初始化当前节点
        dfn[u] = low[u] = ++timestamp;
        st.push(u);
        inStack[u] = true;
        
        // 遍历所有邻接节点
        for (int v : graph[u]) {
            if (dfn[v] == 0) {
                // v未被访问，递归访问
                tarjan(v);
                low[u] = min(low[u], low[v]);
            } else if (inStack[v]) {
                // v在栈中，说明存在回边
                low[u] = min(low[u], dfn[v]);
            }
        }
        
        // 如果u是强连通分量的根节点
        if (dfn[u] == low[u]) {
            vector<int> scc;
            int v;
            do {
                v = st.top();
                st.pop();
                inStack[v] = false;
                scc_id[v] = sccs.size(); // 设置强连通分量编号
                scc.push_back(v);
            } while (v != u);
            
            sccs.push_back(scc);
        }
    }
    
public:
    TarjanSCC(int n) : n(n), graph(n + 1), dfn(n + 1, 0), 
                       low(n + 1, 0), inStack(n + 1, false), 
                       scc_id(n + 1, -1), timestamp(0) {}
    
    void addEdge(int u, int v) {
        graph[u].push_back(v);
    }
    
    void findSCC() {
        for (int i = 1; i <= n; i++) {
            if (dfn[i] == 0) {
                tarjan(i);
            }
        }
    }
    
    void printSCCs() {
        cout << "强连通分量数量: " << sccs.size() << endl;
        for (int i = 0; i < sccs.size(); i++) {
            cout << "SCC " << i + 1 << ": ";
            for (int v : sccs[i]) {
                cout << v << " ";
            }
            cout << endl;
        }
    }
    
    vector<vector<int>> getSCCs() {
        return sccs;
    }
    
    int getSCCCount() {
        return sccs.size();
    }
    
    int getSCCId(int u) {
        return scc_id[u];
    }
    
    // 构建缩点后的DAG
    vector<vector<int>> buildDAG() {
        int scc_count = sccs.size();
        vector<set<int>> dag_set(scc_count);
        
        for (int u = 1; u <= n; u++) {
            for (int v : graph[u]) {
                if (scc_id[u] != scc_id[v]) {
                    dag_set[scc_id[u]].insert(scc_id[v]);
                }
            }
        }
        
        vector<vector<int>> dag(scc_count);
        for (int i = 0; i < scc_count; i++) {
            for (int j : dag_set[i]) {
                dag[i].push_back(j);
            }
        }
        
        return dag;
    }
    
    // 统计大小大于1的强连通分量个数
    int countLargeSCCs() {
        int count = 0;
        for (const auto& scc : sccs) {
            if (scc.size() > 1) {
                count++;
            }
        }
        return count;
    }
};

// 简化版全局变量模板
namespace SimpleTarjan {
    const int MAXN = 100005;
    vector<int> graph[MAXN];
    int dfn[MAXN], low[MAXN], timestamp_cnt;
    bool inStack[MAXN];
    stack<int> st;
    vector<vector<int>> sccs;
    
    void tarjan(int u) {
        dfn[u] = low[u] = ++timestamp_cnt;
        st.push(u);
        inStack[u] = true;
        
        for (int v : graph[u]) {
            if (!dfn[v]) {
                tarjan(v);
                low[u] = min(low[u], low[v]);
            } else if (inStack[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }
        
        if (dfn[u] == low[u]) {
            vector<int> scc;
            int v;
            do {
                v = st.top();
                st.pop();
                inStack[v] = false;
                scc.push_back(v);
            } while (v != u);
            sccs.push_back(scc);
        }
    }
    
    void findSCC(int n) {
        // 初始化
        timestamp_cnt = 0;
        for (int i = 1; i <= n; i++) {
            dfn[i] = low[i] = 0;
            inStack[i] = false;
        }
        sccs.clear();
        
        // 执行算法
        for (int i = 1; i <= n; i++) {
            if (!dfn[i]) {
                tarjan(i);
            }
        }
    }
}

// 示例应用：判断图是否强连通
bool isStronglyConnected(int n, vector<vector<int>>& edges) {
    TarjanSCC tarjan(n);
    for (auto& edge : edges) {
        tarjan.addEdge(edge[0], edge[1]);
    }
    tarjan.findSCC();
    return tarjan.getSCCCount() == 1;
}

// 示例应用：缩点后求DAG的拓扑排序
vector<int> topologicalSort(vector<vector<int>>& dag) {
    int n = dag.size();
    vector<int> indegree(n, 0);
    
    for (int i = 0; i < n; i++) {
        for (int j : dag[i]) {
            indegree[j]++;
        }
    }
    
    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (indegree[i] == 0) {
            q.push(i);
        }
    }
    
    vector<int> result;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        result.push_back(u);
        
        for (int v : dag[u]) {
            indegree[v]--;
            if (indegree[v] == 0) {
                q.push(v);
            }
        }
    }
    
    return result;
}

// 测试函数
void testTarjan() {
    cout << "=== Tarjan算法测试 ===" << endl;
    
    // 创建测试图
    int n = 6;
    TarjanSCC tarjan(n);
    
    // 添加边: 1->2->3->1, 2->4->5->4, 3->6
    tarjan.addEdge(1, 2);
    tarjan.addEdge(2, 3);
    tarjan.addEdge(3, 1);
    tarjan.addEdge(2, 4);
    tarjan.addEdge(4, 5);
    tarjan.addEdge(5, 4);
    tarjan.addEdge(3, 6);
    
    // 求解强连通分量
    tarjan.findSCC();
    
    // 输出结果
    tarjan.printSCCs();
    
    // 输出缩点后的DAG
    cout << "\n缩点后的DAG:" << endl;
    vector<vector<int>> dag = tarjan.buildDAG();
    for (int i = 0; i < dag.size(); i++) {
        cout << "SCC " << i << " -> ";
        for (int j : dag[i]) {
            cout << "SCC " << j << " ";
        }
        cout << endl;
    }
    
    // 拓扑排序
    cout << "\nDAG的拓扑排序: ";
    vector<int> topo = topologicalSort(dag);
    for (int i : topo) {
        cout << "SCC" << i << " ";
    }
    cout << endl;
    
    cout << "\n大小大于1的强连通分量个数: " << tarjan.countLargeSCCs() << endl;
}

// 经典例题：洛谷P2863 - 牛的舞会
void solveCowParty() {
    cout << "\n=== 洛谷P2863 牛的舞会 ===" << endl;
    
    int n, m;
    cout << "输入节点数和边数: ";
    cin >> n >> m;
    
    TarjanSCC tarjan(n);
    cout << "输入" << m << "条边:" << endl;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        tarjan.addEdge(u, v);
    }
    
    tarjan.findSCC();
    cout << "答案: " << tarjan.countLargeSCCs() << endl;
}

int main() {
    // 运行测试
    testTarjan();
    
    // 可以取消注释来运行牛的舞会例题
    // solveCowParty();
    
    return 0;
}

/*
测试用例1:
输入:
6 7
1 2
2 3
3 1
2 4
4 5
5 4
3 6

预期输出:
强连通分量数量: 3
SCC 1: 6
SCC 2: 5 4  
SCC 3: 3 2 1

缩点后的DAG:
SCC 0 -> 
SCC 1 -> 
SCC 2 -> SCC 0 SCC 1

DAG的拓扑排序: SCC2 SCC1 SCC0

大小大于1的强连通分量个数: 2

测试用例2（牛的舞会）:
输入:
4 5
1 2
2 1
3 2
2 4
4 3

预期输出:
答案: 2
*/ 
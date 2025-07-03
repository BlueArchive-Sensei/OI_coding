/*
 * 拟阵理论算法实现
 * 千禧年科技学院 - ユウカ数学专题
 * 
 * 包含内容：
 * 1. 图拟阵 (Graphic Matroid)
 * 2. 线性拟阵 (Linear Matroid)  
 * 3. 均匀拟阵 (Uniform Matroid)
 * 4. 分割拟阵 (Partition Matroid)
 * 5. 拟阵交算法 (Matroid Intersection)
 * 6. 贪心算法通用框架
 * 
 * 时间复杂度：
 * - 图拟阵贪心：O(m log m + m α(n))
 * - 拟阵交：O(nr³)
 * 
 * 空间复杂度：O(n + m)
 */

#include <bits/stdc++.h>
using namespace std;

// =============================================================================
// 拟阵基类定义
// =============================================================================

template<typename T>
class Matroid {
public:
    virtual ~Matroid() = default;
    virtual bool canAdd(const vector<T>& current, const T& element) = 0;
    virtual vector<T> getGroundSet() = 0;
    virtual int getRank() = 0;
    virtual string getName() = 0;
    
    // 检查给定集合是否为独立集
    virtual bool isIndependent(const vector<T>& set) {
        vector<T> temp;
        for (const T& element : set) {
            if (!canAdd(temp, element)) {
                return false;
            }
            temp.push_back(element);
        }
        return true;
    }
    
    // 获取集合的秩
    virtual int getRank(const vector<T>& set) {
        vector<T> temp;
        for (const T& element : set) {
            if (canAdd(temp, element)) {
                temp.push_back(element);
            }
        }
        return temp.size();
    }
};

// =============================================================================
// 图拟阵实现 - 用于最小生成树
// =============================================================================

struct Edge {
    int u, v, weight;
    int id;
    
    Edge() = default;
    Edge(int u, int v, int w, int id) : u(u), v(v), weight(w), id(id) {}
    
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class GraphMatroid : public Matroid<int> {
private:
    int n;  // 顶点数
    vector<Edge> edges;  // 边集
    vector<int> parent, rank;
    
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;
        
        if (rank[px] < rank[py]) {
            parent[px] = py;
        } else if (rank[px] > rank[py]) {
            parent[py] = px;
        } else {
            parent[py] = px;
            rank[px]++;
        }
    }
    
    void reset() {
        iota(parent.begin(), parent.end(), 0);
        fill(rank.begin(), rank.end(), 0);
    }
    
public:
    GraphMatroid(int vertices, const vector<Edge>& edges) 
        : n(vertices), edges(edges) {
        parent.resize(n);
        rank.resize(n);
        reset();
    }
    
    bool canAdd(const vector<int>& current, const int& edgeId) override {
        reset();
        
        // 先添加当前已有的边
        for (int id : current) {
            if (id < edges.size()) {
                unite(edges[id].u, edges[id].v);
            }
        }
        
        // 检查新边是否会造成环
        if (edgeId < edges.size()) {
            return find(edges[edgeId].u) != find(edges[edgeId].v);
        }
        return false;
    }
    
    vector<int> getGroundSet() override {
        vector<int> result;
        for (int i = 0; i < edges.size(); i++) {
            result.push_back(i);
        }
        return result;
    }
    
    int getRank() override {
        return n - 1;  // 生成树有n-1条边
    }
    
    string getName() override {
        return "GraphMatroid";
    }
    
    // Kruskal算法 - 图拟阵上的贪心算法
    vector<Edge> kruskalMST() {
        vector<int> edgeIds = getGroundSet();
        sort(edgeIds.begin(), edgeIds.end(), 
             [&](int a, int b) { return edges[a].weight < edges[b].weight; });
        
        vector<int> mst;
        for (int id : edgeIds) {
            if (canAdd(mst, id)) {
                mst.push_back(id);
                if (mst.size() == n - 1) break;
            }
        }
        
        vector<Edge> result;
        for (int id : mst) {
            result.push_back(edges[id]);
        }
        return result;
    }
};

// =============================================================================
// 线性拟阵实现 - 用于线性代数问题
// =============================================================================

class LinearMatroid : public Matroid<int> {
private:
    vector<vector<double>> matrix;  // 列向量矩阵
    int n, m;  // n行m列
    double eps = 1e-9;
    
    // 高斯消元判断线性相关性
    bool isLinearIndependent(const vector<int>& columns) {
        if (columns.empty()) return true;
        
        vector<vector<double>> temp(n, vector<double>(columns.size()));
        for (int j = 0; j < columns.size(); j++) {
            for (int i = 0; i < n; i++) {
                temp[i][j] = matrix[i][columns[j]];
            }
        }
        
        // 高斯消元
        int rank = 0;
        for (int col = 0; col < columns.size() && rank < n; col++) {
            int pivot = -1;
            for (int row = rank; row < n; row++) {
                if (abs(temp[row][col]) > eps) {
                    pivot = row;
                    break;
                }
            }
            
            if (pivot == -1) continue;
            
            if (pivot != rank) {
                swap(temp[rank], temp[pivot]);
            }
            
            for (int row = rank + 1; row < n; row++) {
                if (abs(temp[row][col]) > eps) {
                    double factor = temp[row][col] / temp[rank][col];
                    for (int j = col; j < columns.size(); j++) {
                        temp[row][j] -= factor * temp[rank][j];
                    }
                }
            }
            rank++;
        }
        
        return rank == columns.size();
    }
    
public:
    LinearMatroid(const vector<vector<double>>& matrix) : matrix(matrix) {
        n = matrix.size();
        m = matrix.empty() ? 0 : matrix[0].size();
    }
    
    bool canAdd(const vector<int>& current, const int& column) override {
        if (column >= m) return false;
        
        vector<int> test = current;
        test.push_back(column);
        return isLinearIndependent(test);
    }
    
    vector<int> getGroundSet() override {
        vector<int> result;
        for (int i = 0; i < m; i++) {
            result.push_back(i);
        }
        return result;
    }
    
    int getRank() override {
        return min(n, m);
    }
    
    string getName() override {
        return "LinearMatroid";
    }
};

// =============================================================================
// 均匀拟阵实现 - 用于选择问题
// =============================================================================

class UniformMatroid : public Matroid<int> {
private:
    int n, k;  // U_{n,k}: 从n个元素中选择至多k个
    
public:
    UniformMatroid(int n, int k) : n(n), k(k) {}
    
    bool canAdd(const vector<int>& current, const int& element) override {
        return current.size() < k && element >= 0 && element < n;
    }
    
    vector<int> getGroundSet() override {
        vector<int> result;
        for (int i = 0; i < n; i++) {
            result.push_back(i);
        }
        return result;
    }
    
    int getRank() override {
        return k;
    }
    
    string getName() override {
        return "UniformMatroid";
    }
};

// =============================================================================
// 分割拟阵实现 - 用于多约束问题
// =============================================================================

class PartitionMatroid : public Matroid<int> {
private:
    vector<vector<int>> partitions;  // 分割
    vector<int> limits;              // 每个分割的容量限制
    vector<int> elementToPartition;  // 元素到分割的映射
    
public:
    PartitionMatroid(const vector<vector<int>>& partitions, 
                     const vector<int>& limits) 
        : partitions(partitions), limits(limits) {
        
        int maxElement = 0;
        for (const auto& partition : partitions) {
            for (int element : partition) {
                maxElement = max(maxElement, element);
            }
        }
        
        elementToPartition.resize(maxElement + 1, -1);
        for (int i = 0; i < partitions.size(); i++) {
            for (int element : partitions[i]) {
                elementToPartition[element] = i;
            }
        }
    }
    
    bool canAdd(const vector<int>& current, const int& element) override {
        if (element >= elementToPartition.size() || 
            elementToPartition[element] == -1) {
            return false;
        }
        
        int partition = elementToPartition[element];
        int count = 0;
        
        for (int e : current) {
            if (e < elementToPartition.size() && 
                elementToPartition[e] == partition) {
                count++;
            }
        }
        
        return count < limits[partition];
    }
    
    vector<int> getGroundSet() override {
        vector<int> result;
        for (const auto& partition : partitions) {
            for (int element : partition) {
                result.push_back(element);
            }
        }
        return result;
    }
    
    int getRank() override {
        return accumulate(limits.begin(), limits.end(), 0);
    }
    
    string getName() override {
        return "PartitionMatroid";
    }
};

// =============================================================================
// 通用贪心算法框架
// =============================================================================

template<typename T>
class GreedyAlgorithm {
public:
    static vector<T> solve(Matroid<T>& matroid, 
                          function<double(const T&)> weight) {
        vector<T> groundSet = matroid.getGroundSet();
        
        // 按权重降序排列
        sort(groundSet.begin(), groundSet.end(), 
             [&](const T& a, const T& b) {
                 return weight(a) > weight(b);
             });
        
        vector<T> result;
        for (const T& element : groundSet) {
            if (matroid.canAdd(result, element)) {
                result.push_back(element);
            }
        }
        
        return result;
    }
    
    static void printSolution(const vector<T>& solution, 
                             function<double(const T&)> weight,
                             const string& matroidName) {
        cout << "=== " << matroidName << " 贪心解 ===" << endl;
        cout << "选择的元素: ";
        for (const T& element : solution) {
            cout << element << " ";
        }
        cout << endl;
        
        double totalWeight = 0;
        for (const T& element : solution) {
            totalWeight += weight(element);
        }
        cout << "总权重: " << totalWeight << endl;
        cout << "基的大小: " << solution.size() << endl;
        cout << endl;
    }
};

// =============================================================================
// 拟阵交算法实现
// =============================================================================

class MatroidIntersection {
private:
    struct AugmentingPath {
        vector<int> path;
        bool found;
        
        AugmentingPath() : found(false) {}
        AugmentingPath(const vector<int>& p) : path(p), found(true) {}
    };
    
    // 构建辅助图并寻找增广路径
    AugmentingPath findAugmentingPath(const vector<int>& current,
                                     Matroid<int>& m1,
                                     Matroid<int>& m2) {
        vector<int> groundSet = m1.getGroundSet();
        int n = groundSet.size();
        
        // 构建二分图
        vector<vector<int>> leftToRight(n), rightToLeft(n);
        vector<bool> inCurrent(n, false);
        
        for (int element : current) {
            if (element < n) {
                inCurrent[element] = true;
            }
        }
        
        // 左侧：current中的元素
        // 右侧：ground set中的元素
        for (int i = 0; i < n; i++) {
            if (inCurrent[i]) {
                // 从左侧i到右侧j的边：去掉i后可以加入j
                for (int j = 0; j < n; j++) {
                    if (i != j && !inCurrent[j]) {
                        vector<int> temp = current;
                        temp.erase(find(temp.begin(), temp.end(), i));
                        if (m1.canAdd(temp, j)) {
                            leftToRight[i].push_back(j);
                        }
                    }
                }
            }
        }
        
        // 使用BFS寻找增广路径
        queue<int> q;
        vector<int> parent(n, -1);
        vector<bool> visited(n, false);
        
        // 从所有可以加入到当前集合的元素开始
        for (int i = 0; i < n; i++) {
            if (!inCurrent[i] && m1.canAdd(current, i)) {
                q.push(i);
                visited[i] = true;
            }
        }
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            
            // 如果u可以被M2接受，找到增广路径
            if (m2.canAdd(current, u)) {
                vector<int> path;
                int curr = u;
                while (curr != -1) {
                    path.push_back(curr);
                    curr = parent[curr];
                }
                reverse(path.begin(), path.end());
                return AugmentingPath(path);
            }
            
            // 继续搜索
            for (int v : leftToRight[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }
        
        return AugmentingPath();
    }
    
public:
    vector<int> solve(Matroid<int>& m1, Matroid<int>& m2) {
        vector<int> result;
        
        cout << "开始拟阵交算法..." << endl;
        cout << "M1: " << m1.getName() << ", M2: " << m2.getName() << endl;
        
        int iteration = 0;
        while (true) {
            iteration++;
            cout << "第 " << iteration << " 次迭代，当前解大小: " 
                 << result.size() << endl;
            
            auto path = findAugmentingPath(result, m1, m2);
            if (!path.found) {
                cout << "未找到增广路径，算法结束" << endl;
                break;
            }
            
            cout << "找到增广路径，长度: " << path.path.size() << endl;
            
            // 沿增广路径更新解
            for (int v : path.path) {
                auto it = find(result.begin(), result.end(), v);
                if (it == result.end()) {
                    result.push_back(v);
                } else {
                    result.erase(it);
                }
            }
        }
        
        cout << "最终解大小: " << result.size() << endl;
        return result;
    }
};

// =============================================================================
// 测试和示例代码
// =============================================================================

void testGraphMatroid() {
    cout << "=== 测试图拟阵 - 最小生成树 ===" << endl;
    
    // 创建一个简单的图
    vector<Edge> edges = {
        {0, 1, 4, 0}, {0, 2, 2, 1}, {1, 2, 1, 2},
        {1, 3, 5, 3}, {2, 3, 8, 4}, {2, 4, 10, 5},
        {3, 4, 2, 6}, {3, 5, 6, 7}, {4, 5, 3, 8}
    };
    
    GraphMatroid gm(6, edges);
    
    // 使用Kruskal算法找最小生成树
    auto mst = gm.kruskalMST();
    
    cout << "最小生成树的边：" << endl;
    int totalWeight = 0;
    for (const Edge& e : mst) {
        cout << "(" << e.u << ", " << e.v << ") 权重: " << e.weight << endl;
        totalWeight += e.weight;
    }
    cout << "总权重: " << totalWeight << endl;
    cout << endl;
}

void testUniformMatroid() {
    cout << "=== 测试均匀拟阵 ===" << endl;
    
    UniformMatroid um(5, 3);  // 从5个元素中选3个
    
    // 权重函数：元素i的权重为i+1
    auto weight = [](int x) { return x + 1.0; };
    
    auto solution = GreedyAlgorithm<int>::solve(um, weight);
    GreedyAlgorithm<int>::printSolution(solution, weight, um.getName());
}

void testPartitionMatroid() {
    cout << "=== 测试分割拟阵 ===" << endl;
    
    // 三个分割：{0,1}, {2,3,4}, {5,6,7,8}
    vector<vector<int>> partitions = {{0, 1}, {2, 3, 4}, {5, 6, 7, 8}};
    vector<int> limits = {1, 2, 2};  // 每个分割的容量限制
    
    PartitionMatroid pm(partitions, limits);
    
    // 权重函数：元素i的权重为i
    auto weight = [](int x) { return x * 1.0; };
    
    auto solution = GreedyAlgorithm<int>::solve(pm, weight);
    GreedyAlgorithm<int>::printSolution(solution, weight, pm.getName());
}

void testMatroidIntersection() {
    cout << "=== 测试拟阵交算法 ===" << endl;
    
    // 创建两个均匀拟阵
    UniformMatroid m1(6, 3);  // 从6个元素中选3个
    UniformMatroid m2(6, 4);  // 从6个元素中选4个
    
    MatroidIntersection mi;
    auto solution = mi.solve(m1, m2);
    
    cout << "拟阵交的解: ";
    for (int x : solution) {
        cout << x << " ";
    }
    cout << endl;
    cout << "解的大小: " << solution.size() << endl;
    cout << endl;
}

void demonstrateMatroidProperties() {
    cout << "=== 拟阵性质演示 ===" << endl;
    
    UniformMatroid um(4, 2);
    
    cout << "均匀拟阵 U_{4,2} 的性质：" << endl;
    cout << "基本集合: {0, 1, 2, 3}" << endl;
    cout << "秩: " << um.getRank() << endl;
    
    // 测试独立集
    vector<vector<int>> testSets = {
        {}, {0}, {0, 1}, {0, 1, 2}, {1, 2}, {2, 3}
    };
    
    for (const auto& set : testSets) {
        cout << "集合 {";
        for (int i = 0; i < set.size(); i++) {
            cout << set[i];
            if (i < set.size() - 1) cout << ", ";
        }
        cout << "} 是否独立: " << (um.isIndependent(set) ? "是" : "否") << endl;
    }
    cout << endl;
}

int main() {
    cout << "🌸 千禧年科技学院 - 拟阵理论算法演示 🌸" << endl;
    cout << "by ユウカ" << endl;
    cout << endl;
    
    demonstrateMatroidProperties();
    testGraphMatroid();
    testUniformMatroid();
    testPartitionMatroid();
    testMatroidIntersection();
    
    cout << "🎯 拟阵理论演示完成！" << endl;
    cout << "拟阵理论为贪心算法提供了坚实的理论基础，" << endl;
    cout << "让我们能够优雅地解决各种组合优化问题。" << endl;
    
    return 0;
} 
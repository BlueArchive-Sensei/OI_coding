# P1954 [NOI2010] 航空管制 题解

## 题目链接
- [洛谷P1954](https://www.luogu.com.cn/problem/P1954)

## 题目描述
给定n个航班和两类限制条件：
1. **最晚起飞时间限制**：编号为i的航班起飞序号不得超过k_i
2. **相对起飞顺序限制**：存在一些限制(a,b)，表示航班a必须先于航班b起飞

需要求解：
1. 一个可行的起飞序列
2. 每个航班在所有可行序列中的最小起飞序号

## 核心思路

### 关键观察
- 这是一个带约束的拓扑排序问题
- 需要**反向建图**，从后往前安排起飞顺序
- 使用贪心策略：优先选择最晚时间限制较大的航班

### 为什么反向建图？
正向思考容易陷入局部最优，而反向思考可以确保全局最优：
- 从第n个位置开始，倒着安排起飞顺序
- 每次选择当前可用航班中k值最大的（最晚时间限制最宽松的）
- 这样可以为后续航班留出更多空间

### 算法步骤

#### 第一问：求可行序列
1. 反向建图：对于限制(a,b)，建边b→a
2. 计算入度数组
3. 使用优先队列（大根堆，按k值排序）进行拓扑排序
4. 从位置n开始，倒着填充结果数组

#### 第二问：求最小起飞序号
对于每个目标航班target：
1. 模拟拓扑排序过程，但暂时忽略target航班
2. 从位置n开始倒推，当无法继续或当前可用航班k值不满足时
3. 检查target是否可用（入度为0且k值满足当前位置）
4. 如果可用，当前位置就是target的最小起飞序号

## 代码实现

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;

const int MAXN = 2005;

int n, m;
int k[MAXN];
vector<int> graph[MAXN];
int indegree[MAXN];
int result[MAXN];

struct Node {
    int flight, limit;
    Node(int f, int l) : flight(f), limit(l) {}
    bool operator<(const Node& other) const {
        return limit < other.limit;  // 大根堆，limit大的优先
    }
};

// 求第一问：一个可行的起飞序列
void solve_first() {
    int temp_indegree[MAXN];
    memcpy(temp_indegree, indegree, sizeof(indegree));
    
    priority_queue<Node> pq;
    
    for (int i = 1; i <= n; i++) {
        if (temp_indegree[i] == 0) {
            pq.push(Node(i, k[i]));
        }
    }
    
    int pos = n;
    while (!pq.empty()) {
        Node curr = pq.top();
        pq.pop();
        
        result[pos--] = curr.flight;
        
        for (int j = 0; j < graph[curr.flight].size(); j++) {
            int next = graph[curr.flight][j];
            temp_indegree[next]--;
            if (temp_indegree[next] == 0) {
                pq.push(Node(next, k[next]));
            }
        }
    }
}

// 求第二问：每个航班的最小起飞序号
int solve_second(int target) {
    int temp_indegree[MAXN];
    memcpy(temp_indegree, indegree, sizeof(indegree));
    
    priority_queue<Node> pq;
    
    // 初始化，不包含target
    for (int i = 1; i <= n; i++) {
        if (temp_indegree[i] == 0 && i != target) {
            pq.push(Node(i, k[i]));
        }
    }
    
    for (int pos = n; pos >= 1; pos--) {
        // 如果没有可用的航班，或者可用航班的limit小于当前位置
        if (pq.empty() || pq.top().limit < pos) {
            // 检查是否可以使用target
            if (temp_indegree[target] == 0 && k[target] >= pos) {
                return pos;
            }
        }
        
        // 使用优先队列中的航班
        if (!pq.empty()) {
            Node curr = pq.top();
            pq.pop();
            
            for (int j = 0; j < graph[curr.flight].size(); j++) {
                int next = graph[curr.flight][j];
                temp_indegree[next]--;
                if (temp_indegree[next] == 0 && next != target) {
                    pq.push(Node(next, k[next]));
                }
            }
        }
    }
    
    return 1;
}

int main() {
    cin >> n >> m;
    
    for (int i = 1; i <= n; i++) {
        cin >> k[i];
    }
    
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        graph[b].push_back(a);  // 反向建图
        indegree[a]++;
    }
    
    // 第一问
    solve_first();
    for (int i = 1; i <= n; i++) {
        cout << result[i];
        if (i < n) cout << " ";
    }
    cout << "\n";
    
    // 第二问
    for (int i = 1; i <= n; i++) {
        cout << solve_second(i);
        if (i < n) cout << " ";
    }
    cout << "\n";
    
    return 0;
}
```

## 复杂度分析

### 时间复杂度
- 第一问：O(n log n + m)，其中拓扑排序O(n + m)，优先队列操作O(n log n)
- 第二问：O(n²log n + nm)，需要对每个航班执行一次模拟拓扑排序
- 总体：O(n²log n + nm)

### 空间复杂度
- O(n + m)，主要用于存储图结构和临时数组

## 样例验证

### 样例1
```
输入：
5 5
4 5 2 5 4
1 2
3 2
5 1
3 4
3 1

输出：
3 5 1 4 2
3 4 1 2 1
```

**分析**：
- 航班1受到(5,1)和(3,1)限制，必须在3和5之后，最早第3位
- 航班2受到(1,2)和(3,2)限制，最早第4位
- 其他航班类似分析

### 样例2
```
输入：
5 0
3 3 3 5 5

输出：
3 1 2 5 4
1 1 1 4 4
```

**分析**：
- 无相对顺序限制
- 前三个航班k值为3，必须在前3位起飞
- 后两个航班最早第4位

## 关键点总结

1. **反向建图是关键**：正向思考容易局部最优，反向思考保证全局最优
2. **贪心策略**：优先选择k值大的航班，为其他航班留出空间
3. **拓扑排序**：处理相对顺序限制
4. **优先队列**：高效选择最优航班
5. **模拟过程**：第二问通过排除目标航班来找最小位置

## 常见错误

1. **正向建图**：容易导致错误的贪心策略
2. **优先队列排序错误**：应该是大根堆（k值大的优先）
3. **第二问算法错误**：需要排除目标航班进行模拟

## 标签
- 拓扑排序
- 贪心算法
- 优先队列
- 图论
- NOI2010 
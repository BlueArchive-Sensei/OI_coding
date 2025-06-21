/*
 * P2765 魔术球问题 - 增量式二分图匹配解法
 * 
 * 算法思路：
 * 逐步增加球的数量，每次只添加必要的新边，避免重建整个图
 * 这样可以显著提高效率，特别是对于大规模数据
 * 
 * 增量策略：
 * 1. 添加新球m时，只添加与球m相关的新边
 * 2. 为新的左侧节点(球m-1)添加边
 * 3. 为所有之前的左侧节点添加与新右侧节点(球m)的边
 * 4. 为所有未匹配的左侧节点寻找增广路径
 *
 * 优化点：
 * - 避免重建整个二分图
 * - 保持已有的匹配关系
 * - 只更新必要的部分
 *
 * 时间复杂度：O(M^2 * N)，比完全重建的O(M^4)有显著提升
 * 空间复杂度：O(M^2)，存储二分图和匹配关系
 */

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

/**
 * 判断一个数是否为完全平方数
 * @param num 待判断的数
 * @return 是否为完全平方数
 */
bool isPerfectSquare(int num) {
    if (num < 0) return false;
    int root = sqrt(num);
    return root * root == num;
}

/**
 * 增量式二分图匹配求解器
 * 支持动态添加节点和边，维护最大匹配
 */
class IncrementalBipartiteMatching {
private:
    vector<vector<int> > graph;    // 邻接表表示的二分图
    vector<int> match;             // match[v]表示右侧节点v匹配的左侧节点
    vector<bool> used;             // DFS过程中的访问标记
    int n, currentBalls;           // n为柱子数，currentBalls为当前球数
    
    /**
     * DFS寻找增广路径
     * @param u 当前左侧节点
     * @return 是否找到增广路径
     */
    bool dfs(int u) {
        for (int i = 0; i < graph[u].size(); i++) {
            int v = graph[u][i];
            if (used[v]) continue;    // 已访问过的右侧节点
            used[v] = true;
            
            // 如果右侧节点v未匹配，或者能为v的匹配找到新的增广路径
            if (match[v] == -1 || dfs(match[v])) {
                match[v] = u;         // 建立匹配
                return true;
            }
        }
        return false;
    }
    
public:
    /**
     * 构造函数：初始化增量式匹配求解器
     * @param pillars 柱子数量
     */
    IncrementalBipartiteMatching(int pillars) : n(pillars), currentBalls(0) {
        graph.resize(10000);          // 预分配足够空间，支持最多10000个球
        match.resize(10000, -1);      // 初始化所有右侧节点未匹配
        used.resize(10000);
    }
    
    /**
     * 增量添加新球，返回是否可以放置当前数量的球
     * 核心优化：只添加新的边，不重建整个图
     * @return 当前球数是否可以放置
     */
    bool addBall() {
        currentBalls++;
        
        if (currentBalls == 1) return true;  // 第一个球可以直接放置
        
        int newLeftNode = currentBalls - 2;   // 新左侧节点编号，对应球currentBalls-1
        int newRightNode = currentBalls - 2;  // 新右侧节点编号，对应球currentBalls
        
        // 增量步骤1：为新的左侧节点添加边
        // 球currentBalls-1可以与右侧球currentBalls, currentBalls+1, ...相邻
        int leftBall = newLeftNode + 1;  // 实际球编号currentBalls-1
        for (int rightBall = leftBall + 1; rightBall <= currentBalls; rightBall++) {
            if (isPerfectSquare(leftBall + rightBall)) {
                graph[newLeftNode].push_back(rightBall - 2);  // 右侧节点编号从0开始
            }
        }
        
        // 增量步骤2：为所有之前的左侧节点添加与新右侧节点的边
        int rightBall = currentBalls;  // 新右侧球编号
        for (int leftNode = 0; leftNode < newLeftNode; leftNode++) {
            int leftBallNum = leftNode + 1;  // 实际球编号
            if (isPerfectSquare(leftBallNum + rightBall)) {
                graph[leftNode].push_back(newRightNode);
            }
        }
        
        // 增量步骤3：为所有未匹配的左侧节点寻找增广路径
        // 这里不只是为新节点寻找，因为新边可能影响整个匹配结构
        for (int leftNode = 0; leftNode < currentBalls - 1; leftNode++) {
            // 检查左侧节点是否已匹配
            bool isMatched = false;
            for (int rightNode = 0; rightNode < currentBalls - 1; rightNode++) {
                if (match[rightNode] == leftNode) {
                    isMatched = true;
                    break;
                }
            }
            
            // 为未匹配的左侧节点寻找增广路径
            if (!isMatched) {
                used.assign(currentBalls - 1, false);
                dfs(leftNode);
            }
        }
        
        // 计算当前匹配数
        int currentMatching = 0;
        for (int i = 0; i < currentBalls - 1; i++) {
            if (match[i] != -1) currentMatching++;
        }
        
        // 判断是否可以放置：需要的匹配数 = currentBalls - n
        // 理由：m个球放在n根柱子上，需要m-n条相邻关系
        return currentMatching >= currentBalls - n;
    }
    
    /**
     * 获取当前球数
     * @return 当前处理的球数
     */
    int getCurrentBalls() {
        return currentBalls;
    }
};

/**
 * 使用贪心算法构造具体的放置方案
 * 虽然用二分图匹配求解了最大球数，但具体方案还是用贪心构造
 * @param n 柱子数量
 * @param m 球的数量
 * @param result 输出的放置方案
 */
void constructSolution(int n, int m, vector<vector<int> >& result) {
    vector<vector<int> > pillars(n);
    
    // 按顺序放置每个球，使用贪心策略
    for (int ball = 1; ball <= m; ball++) {
        int bestPillar = -1;
        
        // 贪心策略：选择顶部球编号最大的可放置柱子
        // 这样可以最大化每根柱子的利用率
        for (int i = 0; i < n; i++) {
            // 检查能否在第i根柱子上放置当前球
            if (pillars[i].empty() ||                                    // 空柱子可以放置
                isPerfectSquare(ball + pillars[i].back())) {            // 或者满足完全平方数条件
                
                if (bestPillar == -1 ||                                 // 第一个可放置的柱子
                    (pillars[i].empty() ? 0 : pillars[i].back()) >      // 或者这个柱子的顶部球编号
                    (pillars[bestPillar].empty() ? 0 : pillars[bestPillar].back())) {  // 比当前最优柱子更大
                    bestPillar = i;
                }
            }
        }
        
        // 将球放置在选定的柱子上
        pillars[bestPillar].push_back(ball);
    }
    
    result = pillars;
}

int main() {
    int n;
    cin >> n;
    
    IncrementalBipartiteMatching matcher(n);
    int maxBalls = 0;
    
    // 逐步增加球数，找到最大可放置的球数
    // 增量式处理：每次只处理新增的球，避免重建整个问题
    while (true) {
        if (matcher.addBall()) {
            maxBalls = matcher.getCurrentBalls();  // 可以放置，更新答案
        } else {
            break;                                  // 不能放置，结束搜索
        }
    }
    
    // 构造并输出最优放置方案
    vector<vector<int> > solution;
    constructSolution(n, maxBalls, solution);
    
    cout << maxBalls << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < solution[i].size(); j++) {
            cout << solution[i][j];
            if (j < solution[i].size() - 1) cout << " ";
        }
        cout << endl;
    }
    
    return 0;
} 
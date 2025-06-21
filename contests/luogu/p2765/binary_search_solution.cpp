/*
 * P2765 魔术球问题 - 二分答案+二分图匹配解法
 * 
 * 算法思路：
 * 1. 二分答案：二分搜索最多能放置的球数
 * 2. 验证函数：使用二分图匹配验证能否放置m个球
 * 
 * 二分图建模：
 * - 左侧节点：球1到球m-1  
 * - 右侧节点：球2到球m
 * - 连边条件：球i和球j可以相邻(i+j是完全平方数)
 * - 匹配含义：每条匹配边表示两球在同一柱子上相邻
 * - 判断条件：最大匹配数 >= m-n (需要m-n条相邻关系)
 *
 * 为什么需要m-n条匹配？
 * - 总共m个球，n根柱子
 * - 每根柱子至少1个球，剩余m-n个球需要"接在后面"
 * - 每个"接在后面"的球对应一条匹配边
 *
 * 时间复杂度：O(log(M) * M^3)，M为最大球数
 * 空间复杂度：O(M^2)，存储二分图
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
 * 二分图匹配求解器
 * 使用匈牙利算法求解最大匹配
 */
class BipartiteMatching {
private:
    vector<vector<int> > graph;    // 邻接表表示的二分图
    vector<int> match;             // match[v]表示右侧节点v匹配的左侧节点
    vector<bool> used;             // DFS过程中的访问标记
    
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
     * 判断是否能用n根柱子放置m个球
     * @param n 柱子数量
     * @param m 球的数量
     * @return 是否可以放置
     */
    bool canPlaceM(int n, int m) {
        if (m <= 1) return true;      // 1个或0个球总是可以放置
        
        // 构建二分图：左侧球1到球m-1，右侧球2到球m
        graph.assign(m - 1, vector<int>());
        
        // 添加边：如果球i和球j可以相邻，连边(i-1, j-2)
        for (int i = 1; i < m; i++) {
            for (int j = i + 1; j <= m; j++) {
                if (isPerfectSquare(i + j)) {
                    graph[i - 1].push_back(j - 2);  // 左侧节点i-1连接右侧节点j-2
                }
            }
        }
        
        // 使用匈牙利算法计算最大匹配
        match.assign(m - 1, -1);      // 初始化：所有右侧节点未匹配
        int matching = 0;
        
        // 为每个左侧节点寻找增广路径
        for (int u = 0; u < m - 1; u++) {
            used.assign(m - 1, false);
            if (dfs(u)) {
                matching++;           // 找到增广路径，匹配数+1
            }
        }
        
        // 判断条件：匹配数 >= m - n
        // 理由：需要m-n条相邻关系来连接m个球到n根柱子上
        return matching >= m - n;
    }
};

/**
 * 使用贪心算法构造具体的放置方案
 * @param n 柱子数量
 * @param m 球的数量
 * @param result 输出的放置方案
 */
void constructSolution(int n, int m, vector<vector<int> >& result) {
    vector<vector<int> > pillars(n);
    
    // 按顺序放置每个球
    for (int ball = 1; ball <= m; ball++) {
        int bestPillar = -1;
        
        // 贪心策略：选择顶部球编号最大的可放置柱子
        for (int i = 0; i < n; i++) {
            if (pillars[i].empty() || isPerfectSquare(ball + pillars[i].back())) {
                if (bestPillar == -1 || 
                    (pillars[i].empty() ? 0 : pillars[i].back()) > 
                    (pillars[bestPillar].empty() ? 0 : pillars[bestPillar].back())) {
                    bestPillar = i;
                }
            }
        }
        
        pillars[bestPillar].push_back(ball);
    }
    
    result = pillars;
}

int main() {
    int n;
    cin >> n;
    
    BipartiteMatching matcher;
    
    // 二分答案：在[1, 10000]范围内二分球的数量
    int left = 1, right = 10000;
    int maxBalls = 0;
    
    while (left <= right) {
        int mid = (left + right) / 2;
        
        // 验证：能否用n根柱子放置mid个球
        if (matcher.canPlaceM(n, mid)) {
            maxBalls = mid;           // 可以放置，尝试更多球
            left = mid + 1;
        } else {
            right = mid - 1;          // 不能放置，减少球数
        }
    }
    
    // 构造并输出最优方案
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
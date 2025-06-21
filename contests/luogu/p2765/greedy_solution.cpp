/*
 * P2765 魔术球问题 - 贪心算法解法
 * 
 * 题目要求：
 * - 有n根柱子，按1,2,3...顺序放球
 * - 每次只能在柱子最上面放球  
 * - 相邻球编号之和必须是完全平方数
 * - 求最多能放多少个球
 *
 * 算法思路：
 * 使用贪心策略：对每个球选择最优的柱子放置
 * 贪心策略：优先选择顶部球编号最大的可放置柱子
 * 这样可以最大化每根柱子的利用率
 *
 * 时间复杂度：O(M*N)，M为最终球数，N为柱子数
 * 空间复杂度：O(N*M)，存储每根柱子的球序列
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

int main() {
    int n;
    cin >> n;
    
    // pillars[i]存储第i根柱子上的球序列
    vector<vector<int> > pillars(n);
    int totalBalls = 0;
    
    // 按顺序尝试放置每个球
    for (int ball = 1; ; ball++) {
        int bestPillar = -1;
        
        // 贪心策略：找到最优的可放置柱子
        // 优先选择顶部球编号最大的柱子
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
        
        // 如果没有找到可放置的柱子，结束放置
        if (bestPillar == -1) break;
        
        // 将球放置在选定的柱子上
        pillars[bestPillar].push_back(ball);
        totalBalls++;
    }
    
    // 输出结果
    cout << totalBalls << endl;
    
    // 输出每根柱子的球序列
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < pillars[i].size(); j++) {
            cout << pillars[i][j];
            if (j < pillars[i].size() - 1) cout << " ";
        }
        cout << endl;
    }
    
    return 0;
} 
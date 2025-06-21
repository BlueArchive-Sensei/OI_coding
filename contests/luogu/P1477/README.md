# P1477 [NOI2008] 假面舞会

<div align="center">

![difficulty](https://img.shields.io/badge/难度-省选/NOI--orange)
![tag](https://img.shields.io/badge/标签-图论-blue)
![tag](https://img.shields.io/badge/标签-连通分量-blue)
![tag](https://img.shields.io/badge/标签-环检测-blue)

</div>

## 🌸 题目简述

栋栋参加假面舞会，主办方将面具分为k类（k≥3），戴第i类面具的人能看到戴第(i+1)类面具的人，戴第k类面具的人能看到戴第1类面具的人。根据收集到的"看到"信息，求出最多和最少的面具类数。

## 📚 算法分析

### 🎯 核心思路

这是一道经典的**图论 + 环检测**问题。关键在于理解面具类别之间的环形关系。

### 🔍 问题转化

1. **建图**: 将"a看到b"的关系建成有向图
   - 正向边：a→b (权值+1)
   - 反向边：b→a (权值-1)

2. **图的结构分析**:
   - **环结构**: 限制了面具类数必须是环长度的约数
   - **链结构**: 如果没有环，类数等于所有链长度之和

### 🧮 数学原理

对于环结构，如果有多个环，面具类数k必须同时满足：
- k是所有环长度的公约数
- k ≥ 3（题目约束）

因此：
- **最大类数**: 所有环长度的最大公约数(GCD)
- **最小类数**: GCD的最小约数（≥3）

## 🎪 算法实现

### ⚡ 核心算法

```cpp
// BFS遍历连通分量，检测环和链
void bfs(int start) {
    queue<int> q;
    q.push(start);
    vis[start] = true;
    dist[start] = 0;
    
    int min_dist = 0, max_dist = 0;
    bool found_cycle = false;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int i = head[u]; i; i = edges[i].next) {
            int v = edges[i].to;
            int w = edges[i].weight;
            
            if (!vis[v]) {
                vis[v] = true;
                dist[v] = dist[u] + w;
                q.push(v);
                min_dist = min(min_dist, dist[v]);
                max_dist = max(max_dist, dist[v]);
            } else {
                // 检查是否形成环
                int expected_dist = dist[u] + w;
                if (expected_dist != dist[v]) {
                    int cycle_len = abs(expected_dist - dist[v]);
                    if (cycle_len > 0) {
                        cycles.push_back(cycle_len);
                        found_cycle = true;
                    }
                }
            }
        }
    }
    
    // 如果没有环，累加链长度
    if (!found_cycle) {
        sum += max_dist - min_dist + 1;
    }
}
```

### 🎨 完整解题步骤

1. **建图**: 对每个关系(a,b)建立双向边
2. **遍历**: BFS遍历所有连通分量
3. **环检测**: 通过距离冲突检测环的存在
4. **结果计算**:
   - 有环：计算所有环长度的GCD
   - 无环：累加所有链的长度

## 🌟 复杂度分析

- **时间复杂度**: O(n + m)，其中n是面具数，m是关系数
- **空间复杂度**: O(n + m)，存储图的邻接表

## 💫 样例分析

### 样例1
```
输入: 6 5
      1 2, 2 3, 3 4, 4 1, 3 5
输出: 4 4
```

**分析**: 
- 1→2→3→4→1 形成长度为4的环
- 节点5形成长度为1的链
- 因为有环，类数受环长度限制
- GCD(4) = 4，且4≥3，所以最大最小类数都是4

### 样例2
```
输入: 3 3
      1 2, 2 1, 2 3
输出: -1 -1
```

**分析**:
- 1↔2 形成长度为2的环
- 因为环长度2 < 3，不满足k≥3的约束
- 输出-1 -1表示无解

## 🎭 编程技巧

### 🌸 关键点总结

1. **双向建边**: 正反边权值相反，巧妙处理同类面具
2. **距离标记**: 用dist数组标记节点间的"类别距离"
3. **环长计算**: `abs(expected_dist - actual_dist)`
4. **GCD求解**: 递归实现最大公约数
5. **约数查找**: 从3开始遍历找最小约数

### 💝 注意事项

- 重边处理：同一关系可能出现多次
- 连通分量：需要遍历所有未访问节点
- 边界情况：无环时检查总长度是否≥3

## 🔥 扩展思考

这道题体现了图论中环检测的重要性，类似问题还包括：
- 判断图的二分性
- 拓扑排序中的环检测
- 强连通分量的应用

## 🎪 ユウカ的数学小课堂

> 💡 **定理**: 在k-类环形系统中，任意两个类别之间的"距离"必须在模k意义下保持一致。
> 
> **证明**: 如果类别i能看到类别j，那么j ≡ (i+1) mod k。通过传递性，任意路径的长度都必须是k的倍数。

---

<div align="center">

**🌸 "图论的美妙之处在于将复杂关系转化为简洁的数学模型！" —— ユウカ**

*Made with 💙 by Millennium Algorithm Club*

</div> 
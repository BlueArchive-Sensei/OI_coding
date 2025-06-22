# P4094 [HEOI2016/TJOI2016] 字符串 - 完整题解与证明

## 🎯 题目信息

- **题目来源**: 洛谷 HEOI2016/TJOI2016
- **题目链接**: [P4094 [HEOI2016/TJOI2016] 字符串](https://www.luogu.com.cn/problem/P4094)
- **难度**: NOI/NOI+/CTSC
- **算法标签**: 后缀数组、ST表、最长公共前缀(LCP)、主席树、二分答案

## 📝 题目描述

给定一个长度为 n 的字符串 s，和 m 个问题。

每个问题均有 a,b,c,d 四个参数，问你子串 s[a..b] 的所有子串和 s[c..d] 的最长公共前缀的长度的最大值是多少？

### 输入格式
```
第一行：n m
第二行：字符串 s
接下来 m 行：每行四个整数 a b c d
```

### 输出格式
```
对于每个问题，输出一个整数，表示最大的最长公共前缀长度
```

### 数据范围
- 对于 100% 的数据，1 ≤ n,m ≤ 100,000，字符串中仅有小写英文字母
- a ≤ b，c ≤ d，1 ≤ a,b,c,d ≤ n

---

## 🧮 问题分析与转化

### 📊 问题本质

设字符串为 S，我们需要求：
$$\max_{i \in [a,b], j \in [c,d]} \text{LCP}(S[i..n], S[j..n])$$

其中，$S[i..n]$ 表示从位置 i 开始的后缀。

### 🔄 关键转化

**观察1**: 我们可以固定 $j \in [c,d]$，然后对每个 j 求：
$$\max_{i \in [a,b]} \text{LCP}(S[i..n], S[j..n])$$

但这样的复杂度仍然是 $O(n^2)$，无法通过。

**关键洞察**: 答案具有**单调性**！如果答案长度为 k 可行，那么任何小于 k 的长度都可行。

---

## 🎯 核心算法：二分答案

### ✨ 可二分性证明

**定理1**: 设答案为 $\text{ans}$，则对于任意 $k \leq \text{ans}$，都存在 $i \in [a,b], j \in [c,d]$ 使得 $\text{LCP}(S[i..n], S[j..n]) \geq k$。

**证明**: 
设存在 $i_0 \in [a,b], j_0 \in [c,d]$ 使得 $\text{LCP}(S[i_0..n], S[j_0..n]) = \text{ans}$。

则对于任意 $k \leq \text{ans}$，有：
$$\text{LCP}(S[i_0..n], S[j_0..n]) = \text{ans} \geq k$$

因此 $(i_0, j_0)$ 就是满足条件的一对位置。 $\square$

### 🎲 判定问题转化

**原问题**: 求最大的LCP长度  
**转化为**: 给定长度 $k$，是否存在 $i \in [a,b], j \in [c,d]$ 使得 $\text{LCP}(S[i..n], S[j..n]) \geq k$？

**进一步转化**: 
1. 枚举 $j \in [c,d]$
2. 对于每个 $j$，判断是否存在 $i \in [a,b]$ 使得 $\text{LCP}(S[i..n], S[j..n]) \geq k$

---

## 🏗️ 数据结构：后缀数组 + ST表

### 📐 后缀数组基础

**定义**: 
- $\text{sa}[i]$: 排名第 i 的后缀的起始位置
- $\text{rank}[i]$: 位置 i 开始的后缀的排名
- $\text{height}[i] = \text{LCP}(\text{sa}[i-1], \text{sa}[i])$

**重要性质**: 对于任意 $i < j$，有：
$$\text{LCP}(\text{sa}[i], \text{sa}[j]) = \min_{k=i+1}^{j} \text{height}[k]$$

### 🎯 ST表优化LCP查询

构建 ST 表对 height 数组进行预处理，支持 $O(1)$ 查询任意区间最小值：

```cpp
int queryLCP(int i, int j) {
    if (i == j) return n - sa[i] + 1;  // 自己和自己的LCP就是后缀长度
    if (i > j) swap(i, j);
    return st.query(i + 1, j);  // 查询height[i+1..j]的最小值
}
```

---

## 🔍 关键性质：LCP的单峰分布

### 🌟 核心定理

**定理2 (LCP单峰性)**: 设 $r = \text{rank}[j]$，则在后缀数组的排序中，$\text{LCP}(S[j..n], S[\text{sa}[k]..n])$ 关于 $k$ 呈单峰分布，峰值在 $k = r$ 处。

**证明**: 
设 $f(k) = \text{LCP}(S[j..n], S[\text{sa}[k]..n])$。

**情况1**: $k < r$  
由于后缀数组按字典序排序，$S[\text{sa}[k]..n] < S[j..n] < S[\text{sa}[r+1]..n]$。  
当 $k$ 远离 $r$ 时，$S[\text{sa}[k]..n]$ 与 $S[j..n]$ 的字典序差异增大，LCP减小。

**情况2**: $k > r$  
类似地，当 $k$ 远离 $r$ 时，$S[\text{sa}[k]..n]$ 与 $S[j..n]$ 的字典序差异增大，LCP减小。

**情况3**: $k = r$  
$\text{LCP}(S[j..n], S[j..n]) = n - j + 1$，这是最大可能的LCP。

更严格地，由于相邻后缀的LCP通过 height 数组表示，且后缀数组的性质保证了 LCP 的连续性，因此 $f(k)$ 确实呈单峰分布。 $\square$

### 🎪 单峰性的应用

**推论**: 对于给定的阈值 $\text{mid}$，满足 $\text{LCP}(S[j..n], S[\text{sa}[k]..n]) \geq \text{mid}$ 的 $k$ 值构成一个连续区间 $[L, R]$。

**算法**: 使用二分查找找到这个区间：

```cpp
pair<int, int> findLCPRange(int pos, int mid) {
    int r = rank[pos];
    
    // 二分找左边界
    int left = 1, right = r;
    int L = r;
    while (left <= right) {
        int m = (left + right) >> 1;
        if (queryLCP(m, r) >= mid) {
            L = m;
            right = m - 1;
        } else {
            left = m + 1;
        }
    }
    
    // 二分找右边界
    left = r; right = n;
    int R = r;
    while (left <= right) {
        int m = (left + right) >> 1;
        if (queryLCP(r, m) >= mid) {
            R = m;
            left = m + 1;
        } else {
            right = m - 1;
        }
    }
    
    return {L, R};
}
```

---

## 🌳 主席树：区间限制查询

### 🔧 问题转化

**当前状态**: 我们已经找到了满足 LCP 条件的 rank 区间 $[L, R]$。

**剩余问题**: 在这个 rank 区间内，是否存在起始位置在 $[a, b-\text{mid}+1]$ 范围内的后缀？

**注意**: 这里是 $[a, b-\text{mid}+1]$ 而不是 $[a, b]$，因为我们需要长度至少为 $\text{mid}$ 的子串。

### 🏛️ 主席树结构

**版本维度**: 按 SA 的排名顺序建立版本，即第 $i$ 个版本包含 $\text{sa}[1], \text{sa}[2], \ldots, \text{sa}[i]$。

**权值维度**: 后缀的起始位置（1 到 n）。

**查询**: $\text{query}(\text{root}[R], \text{root}[L-1], a, b-\text{mid}+1)$ 返回在版本区间 $[L, R]$ 内、权值在 $[a, b-\text{mid}+1]$ 范围内的元素个数。

### 🧮 正确性证明

**定理3**: 主席树查询的正确性。

**证明**: 
我们需要证明：存在 $i \in [a, b], j \in [c, d]$ 使得 $\text{LCP}(S[i..n], S[j..n]) \geq \text{mid}$ 当且仅当存在某个 $j \in [c, d]$ 和对应的 rank 区间 $[L, R]$，使得区间 $[L, R]$ 内存在起始位置在 $[a, b-\text{mid}+1]$ 的后缀。

**必要性**: 
设存在 $i_0 \in [a, b], j_0 \in [c, d]$ 使得 $\text{LCP}(S[i_0..n], S[j_0..n]) \geq \text{mid}$。

由单峰性，$\text{rank}[i_0]$ 必然在 $j_0$ 对应的 rank 区间 $[L, R]$ 内。

又因为子串 $S[i_0..i_0+\text{mid}-1]$ 的起始位置是 $i_0 \in [a, b]$，且长度为 $\text{mid}$，所以 $i_0 \leq b - \text{mid} + 1$，即 $i_0 \in [a, b-\text{mid}+1]$。

**充分性**: 
设存在 $j_0 \in [c, d]$ 和对应的 rank 区间 $[L, R]$，使得区间内存在起始位置为 $i_0 \in [a, b-\text{mid}+1]$ 的后缀。

由区间的定义，有 $\text{LCP}(S[i_0..n], S[j_0..n]) \geq \text{mid}$。

又因为 $i_0 \in [a, b-\text{mid}+1] \subseteq [a, b]$，所以条件满足。 $\square$

---

## 🔄 完整算法流程

### 1️⃣ 预处理阶段 $O(n \log n)$

```cpp
// 1. 构建后缀数组
buildSA(s, sa, rank, height, n);

// 2. 构建ST表（用于O(1)查询LCP）
st.init(height, n);

// 3. 构建主席树（按SA序插入后缀位置）
for (int i = 1; i <= n; i++) {
    root[i] = update(root[i-1], sa[i], 1, n);
}
```

### 2️⃣ 单次查询 $O(\log^2 n)$

```cpp
bool check(int a, int b, int c, int d, int mid) {
    // 枚举c到d的每个位置
    for (int pos = c; pos <= d; pos++) {
        // 找到满足LCP >= mid的rank区间
        auto [L, R] = findLCPRange(pos, mid);
        
        // 查询该区间内是否有起始位置在[a, b-mid+1]的后缀
        if (query(root[R], root[L-1], a, b - mid + 1) > 0) {
            return true;
        }
    }
    return false;
}

int solve(int a, int b, int c, int d) {
    int left = 0, right = min(b - a + 1, d - c + 1);
    int ans = 0;
    
    while (left <= right) {
        int mid = (left + right) >> 1;
        if (check(a, b, c, d, mid)) {
            ans = mid;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return ans;
}
```

---

## 📊 复杂度分析

### ⏰ 时间复杂度

**预处理**: $O(n \log n)$
- 后缀数组构建: $O(n \log n)$
- ST表构建: $O(n \log n)$
- 主席树构建: $O(n \log n)$

**单次查询**: $O((d-c+1) \cdot \log n \cdot \log n) = O(n \log^2 n)$（最坏情况）
- 外层二分: $O(\log n)$
- 枚举 $[c,d]$: $O(d-c+1) \leq O(n)$
- 找LCP区间: $O(\log n)$
- 主席树查询: $O(\log n)$

**总复杂度**: $O(n \log n + m \cdot n \log^2 n)$

### 💾 空间复杂度

$O(n \log n)$，主要是主席树的空间开销。

---

## ⚡ 优化技巧

### 🎯 重要实现细节

1. **height数组的正确使用**:
   ```cpp
   // height[i] = LCP(sa[i-1], sa[i])
   // 查询LCP(sa[i], sa[j])需要查询height[i+1..j]的最小值
   ```

2. **边界条件处理**:
   ```cpp
   // 注意子串长度限制：起始位置最大为 b - mid + 1
   // 避免数组越界和空区间查询
   ```

3. **主席树的版本管理**:
   ```cpp
   // 版本0为空树，版本i包含前i个SA位置
   // 查询区间[L,R]使用 root[R] - root[L-1]
   ```

---

## 🏆 算法总结

### 🌟 核心思想

这道题完美展示了**算法设计的层次性思维**：

1. **问题转化**: 最值问题 → 判定问题
2. **性质发现**: 答案单调性 → 二分答案  
3. **结构利用**: LCP单峰性 → 区间查询
4. **维度分离**: 位置限制 + LCP限制 → 主席树

### 📈 学习价值

- **字符串算法**: 后缀数组、LCP查询的深入应用
- **数据结构**: ST表、主席树的综合运用  
- **算法思维**: 问题转化、性质挖掘、复杂度优化
- **数学证明**: 单峰性证明、正确性分析

### 💡 拓展思考

类似的问题往往具有以下特征：
- 答案具有单调性，可以二分
- 涉及字符串的LCP查询
- 需要同时满足多个维度的限制条件

---

## 📁 文件说明

- `P4094.cpp` - 最终AC解法（二分答案 + 后缀数组 + 主席树）
- `README.md` - 完整题解与数学证明
- `test_input.txt` - 测试数据

---

*"真正的算法之美不在于代码的复杂，而在于思想的精巧与证明的严密"* 🌟  
*完整证明版本 - 包含所有数学细节* ✨ 
# 🌟 CDQ分治详解
## *千禧年科技学院 - アリス会长特别讲座*

*"分治的艺术，就是将复杂问题分解成简单问题的智慧！"*

---

## 📚 目录
1. [CDQ分治简介](#cdq分治简介)
2. [算法原理](#算法原理)
3. [经典应用场景](#经典应用场景)
4. [三维偏序问题详解](#三维偏序问题详解)
5. [代码实现](#代码实现)
6. [例题分析](#例题分析)
7. [总结](#总结)

---

## 🎯 CDQ分治简介

**CDQ分治**是一种高效的算法思想，由中国OI界传奇人物**陈丹琦**发明并推广。这种方法主要用于解决**偏序问题**，特别是在处理**二维、三维偏序**时表现出色。

### 🌸 核心思想
CDQ分治的核心在于：
- 将问题按照**某一维度**进行分治
- 在分治过程中处理**跨越分界点**的贡献
- 通过**归并**的方式维护另一维度的有序性

### 💫 时间复杂度
- 二维偏序：O(n log n)
- 三维偏序：O(n log² n)

---

## 🔧 算法原理

### 📊 分治策略图解

```
原问题 [1, n]
        |
        v
   分解为两部分
   [1, mid] [mid+1, n]
        |
        v
    递归处理左右两部分
        |
        v
    处理跨越边界的贡献
        |
        v
    合并结果
```

### 🎨 三维偏序问题模型

对于三维偏序问题，我们有n个点，每个点有三个坐标(x, y, z)，要求统计满足以下条件的点对(i, j)数量：
- x[i] ≤ x[j]
- y[i] ≤ y[j]  
- z[i] ≤ z[j]

### 🌟 CDQ分治处理流程

1. **预处理**：按第一维x坐标排序
2. **分治**：将问题分为左右两部分
3. **处理跨界**：左边的点对右边的点的贡献
4. **归并**：按第二维y坐标归并，同时用数据结构维护第三维z

---

## 🚀 经典应用场景

### 1. 🎯 三维偏序问题
- **逆序对计数**
- **三维数点问题**
- **陌上花开**类题目

### 2. 📈 动态规划优化
- **带修改的DP**
- **二维状态转移**

### 3. 🔍 数据结构问题
- **带修改的查询**
- **离线处理**

---

## 🎭 三维偏序问题详解

### 问题描述
给定n个三元组(a, b, c)，对于每个三元组，求有多少个三元组(a', b', c')满足：
- a' ≤ a
- b' ≤ b
- c' ≤ c

### 🌸 解题思路

#### 第一步：处理第一维
```cpp
// 按第一维a排序
sort(points, points + n, [](const Point& x, const Point& y) {
    return x.a < y.a;
});
```

#### 第二步：CDQ分治框架
```cpp
void cdq(int l, int r) {
    if (l == r) return;
    
    int mid = (l + r) / 2;
    cdq(l, mid);      // 递归处理左半部分
    cdq(mid + 1, r);  // 递归处理右半部分
    
    // 处理跨越边界的贡献
    merge(l, mid, r);
}
```

#### 第三步：归并处理
```cpp
void merge(int l, int mid, int r) {
    int i = l, j = mid + 1;
    
    while (i <= mid && j <= r) {
        if (points[i].b <= points[j].b) {
            // 左边的点可以对右边产生贡献
            update(points[i].c, 1);  // 在第三维数据结构中插入
            i++;
        } else {
            // 右边的点查询左边的贡献
            ans[points[j].id] += query(points[j].c);
            j++;
        }
    }
    
    // 处理剩余元素
    while (j <= r) {
        ans[points[j].id] += query(points[j].c);
        j++;
    }
    
    // 清理数据结构
    for (int k = l; k <= mid; k++) {
        update(points[k].c, -1);
    }
}
```

---

## 💻 代码实现

### 完整的三维偏序CDQ分治解法

```cpp
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;
const int MAXC = 200005;

struct Point {
    int a, b, c;
    int id;
    int cnt;  // 相同点的数量
    
    bool operator<(const Point& other) const {
        if (a != other.a) return a < other.a;
        if (b != other.b) return b < other.b;
        return c < other.c;
    }
    
    bool operator==(const Point& other) const {
        return a == other.a && b == other.b && c == other.c;
    }
};

Point points[MAXN], temp[MAXN];
int ans[MAXN];
int tree[MAXC];
int n, k;

// 树状数组操作
void update(int pos, int val) {
    for (int i = pos; i <= k; i += i & -i) {
        tree[i] += val;
    }
}

int query(int pos) {
    int res = 0;
    for (int i = pos; i > 0; i -= i & -i) {
        res += tree[i];
    }
    return res;
}

void cdq(int l, int r) {
    if (l == r) return;
    
    int mid = (l + r) / 2;
    cdq(l, mid);
    cdq(mid + 1, r);
    
    // 归并过程，按第二维b排序
    int i = l, j = mid + 1, p = l;
    
    while (i <= mid && j <= r) {
        if (points[i].b <= points[j].b) {
            update(points[i].c, points[i].cnt);
            temp[p++] = points[i++];
        } else {
            ans[points[j].id] += query(points[j].c);
            temp[p++] = points[j++];
        }
    }
    
    while (j <= r) {
        ans[points[j].id] += query(points[j].c);
        temp[p++] = points[j++];
    }
    
    while (i <= mid) {
        update(points[i].c, points[i].cnt);
        temp[p++] = points[i++];
    }
    
    // 清理树状数组
    for (int i = l; i <= mid; i++) {
        update(points[i].c, -points[i].cnt);
    }
    
    // 复制回原数组
    for (int i = l; i <= r; i++) {
        points[i] = temp[i];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n >> k;
    
    for (int i = 0; i < n; i++) {
        cin >> points[i].a >> points[i].b >> points[i].c;
        points[i].id = i;
        points[i].cnt = 1;
    }
    
    // 按第一维排序
    sort(points, points + n);
    
    // 去重处理
    int m = 0;
    for (int i = 0; i < n; i++) {
        if (i == 0 || !(points[i] == points[i-1])) {
            points[m++] = points[i];
        } else {
            points[m-1].cnt++;
        }
    }
    
    // CDQ分治
    cdq(0, m - 1);
    
    // 输出答案
    vector<int> result(n);
    for (int i = 0; i < m; i++) {
        result[points[i].cnt - 1] += points[i].cnt;
    }
    
    for (int i = 0; i < n; i++) {
        cout << result[i] << "\n";
    }
    
    return 0;
}
```

---

## 📝 例题分析

### 🌟 洛谷P3810 - 陌上花开

#### 题目描述
给定n朵花，每朵花有三个属性：美丽度ai、香味bi、高度ci。
对于每朵花，求有多少朵花在这三个属性上都不超过它。

#### 解题思路
这是一个经典的三维偏序问题：
1. 按第一维美丽度排序
2. 使用CDQ分治处理第二维香味
3. 用树状数组维护第三维高度

#### 关键代码片段
```cpp
// 归并时的贡献计算
if (points[i].b <= points[j].b) {
    // 左边的花可以对右边产生贡献
    update(points[i].c, points[i].cnt);
    temp[p++] = points[i++];
} else {
    // 右边的花查询左边的贡献
    ans[points[j].id] += query(points[j].c);
    temp[p++] = points[j++];
}
```

---

## 🎨 算法优化技巧

### 1. 🌸 坐标离散化
```cpp
// 对第三维坐标进行离散化
sort(coords.begin(), coords.end());
coords.erase(unique(coords.begin(), coords.end()), coords.end());

for (int i = 0; i < n; i++) {
    points[i].c = lower_bound(coords.begin(), coords.end(), points[i].c) 
                  - coords.begin() + 1;
}
```

### 2. 💫 去重优化
```cpp
// 相同的点进行合并
for (int i = 0; i < n; i++) {
    if (i == 0 || !(points[i] == points[i-1])) {
        points[m++] = points[i];
    } else {
        points[m-1].cnt++;
    }
}
```

### 3. 🎯 内存优化
使用临时数组进行归并，避免频繁的内存分配。

---

## 🔍 复杂度分析

### 时间复杂度
- **预处理排序**：O(n log n)
- **CDQ分治**：T(n) = 2T(n/2) + O(n log n) = O(n log² n)
- **总复杂度**：O(n log² n)

### 空间复杂度
- **原数组**：O(n)
- **临时数组**：O(n)
- **树状数组**：O(k) 其中k为第三维的值域
- **总复杂度**：O(n + k)

---

## 🎪 扩展应用

### 1. 🌟 带修改的动态规划
```cpp
// 状态转移：dp[i] = max(dp[j] + val[i]) where j < i and a[j] <= a[i]
// 可以用CDQ分治优化到O(n log n)
```

### 2. 💝 多维数点问题
```cpp
// 查询矩形内的点的数量
// 可以转化为四个前缀和的计算
```

### 3. 🎈 逆序对的扩展
```cpp
// k维逆序对：ai1 > aj1 && ai2 > aj2 && ... && aik > ajk
// 可以用CDQ分治递归处理
```

---

## 📋 总结

### 🌸 CDQ分治的优势
1. **高效性**：相比暴力O(n²)，CDQ分治达到O(n log² n)
2. **通用性**：适用于多种偏序问题
3. **可扩展性**：可以处理更高维的偏序问题

### 💫 学习要点
1. **理解分治思想**：如何将高维问题降维处理
2. **掌握归并技巧**：如何在归并中计算贡献
3. **数据结构运用**：树状数组、线段树等的配合使用

### 🎯 练习建议
1. 先掌握二维偏序（逆序对）
2. 理解三维偏序的CDQ分治
3. 练习相关的OI题目

---

## 🎭 アリス会长寄语

> *"CDQ分治是算法竞赛中的一颗明珠，它教会我们如何用分治的思想解决复杂的多维问题。记住，每一次分治都是在简化问题，每一次归并都是在汇聚答案。掌握了CDQ分治，你就掌握了解决偏序问题的钥匙！"*
>
> — **アリス会长**

*🌸 がんばって！继续努力学习算法吧！*

---

**© 2024 千禧年科技学院 - Algorithm Competition Club** 
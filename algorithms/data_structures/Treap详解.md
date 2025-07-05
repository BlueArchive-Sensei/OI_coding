# 🎯 Treap - 随机优先级的神奇平衡树
## *千禧年科技学院 - アリス老师的算法课*

*"想象一下，你既想要书架按编号排序，又想要重要的书在顶层，Treap就是这样的神奇书架！"*

---

## 🎪 开始之前 - 什么是Treap？

### 🌟 最简单的理解

**Treap = Tree + Heap**

想象你有一个**双重标准的书架**：
- 📚 **按编号排序**（像二叉搜索树）
- 🎲 **按重要程度排序**（像堆）
- ⚡ **每本书都有编号和重要程度**

```
每本书：(编号, 重要程度)
例如：(25, 90), (15, 60), (35, 80)
```

### 🎯 核心思想

```
编号满足二叉搜索树性质 + 重要程度满足堆性质 = 自然平衡
```

---

## 📖 第一章：从生活例子开始

### 🏆 图书馆的智能书架

假设你是图书馆管理员，有这些书：

| 书名 | 编号 | 重要程度 |
|------|------|----------|
| 数学 | 10 | 85 |
| 物理 | 20 | 92 |
| 化学 | 30 | 78 |
| 生物 | 40 | 88 |

**要求：**
1. 按编号排序（方便查找）
2. 重要的书在上层（方便拿取）

### 🎪 传统方法的问题

**只按编号排序：**
```
    20
   /  \
  10   30
        \
         40
```
问题：不考虑重要程度，重要的书可能在底层

**只按重要程度排序：**
```
    92(物理)
   /        \
  88(生物)   85(数学)
            /
           78(化学)
```
问题：编号没有规律，查找困难

### 🌟 Treap的解决方案

**同时满足两个条件：**
```
         20(92)
        /      \
    10(85)    30(78)
                 \
                 40(88)
```

- **编号：** 10 < 20 < 30 < 40 ✓
- **重要程度：** 92 > 85, 92 > 78, 78 < 88 ✓

---

## 🚀 第二章：Treap的两种实现

### 🎯 方法一：传统Treap（旋转版）

**核心思想：** 通过旋转维护堆性质

#### 🌸 基本操作

**插入过程：**
1. 按编号找到位置插入
2. 如果破坏堆性质，就旋转
3. 持续旋转直到满足堆性质

**删除过程：**
1. 找到要删除的节点
2. 通过旋转把它移到叶子
3. 直接删除

#### 📊 实战演示

**插入 (15, 95)**

```
步骤1：按编号插入
    20(92)
   /      \
10(85)    30(78)
   \         \
   15(95)    40(88)

步骤2：发现15的重要程度95 > 10的重要程度85，需要旋转
    20(92)
   /      \
15(95)    30(78)
/             \
10(85)        40(88)

步骤3：发现15的重要程度95 > 20的重要程度92，继续旋转
    15(95)
   /      \
10(85)    20(92)
             \
             30(78)
                \
                40(88)
```

### 🎯 方法二：FHQ-Treap（推荐）

**核心思想：** 通过分裂和合并操作

#### 🌟 为什么推荐FHQ-Treap？

1. **更简单** - 不需要复杂的旋转逻辑
2. **更好写** - 代码更短，更容易理解
3. **更强大** - 支持区间操作

#### 🎪 核心操作

**Split（分裂）：**
把一棵树分成两棵树

```
原树：包含 ≤ key 和 > key 的所有节点
分裂后：左树（≤ key）和右树（> key）
```

**Merge（合并）：**
把两棵树合并成一棵树

```
条件：左树的所有值 ≤ 右树的所有值
结果：合并后的树
```

---

## 💻 第三章：FHQ-Treap完整实现

### 🌸 基础版本

```cpp
#include <iostream>
#include <random>
using namespace std;

struct Node {
    int key, priority;
    Node* left;
    Node* right;
    
    Node(int k) : key(k), priority(rand()), left(nullptr), right(nullptr) {}
};

class FHQTreap {
private:
    Node* root;
    
    // 分裂：把树分成 ≤key 和 >key 两部分
    void split(Node* root, int key, Node*& left, Node*& right) {
        if (!root) {
            left = right = nullptr;
            return;
        }
        
        if (root->key <= key) {
            left = root;
            split(root->right, key, left->right, right);
        } else {
            right = root;
            split(root->left, key, left, right->left);
        }
    }
    
    // 合并：把两棵树合并（要求left的所有值 ≤ right的所有值）
    Node* merge(Node* left, Node* right) {
        if (!left) return right;
        if (!right) return left;
        
        if (left->priority > right->priority) {
            left->right = merge(left->right, right);
            return left;
        } else {
            right->left = merge(left, right->left);
            return right;
        }
    }
    
public:
    FHQTreap() : root(nullptr) {
        srand(time(0));  // 初始化随机种子
    }
    
    // 插入
    void insert(int key) {
        Node* left, *right;
        split(root, key, left, right);
        
        // 检查是否已存在
        Node* temp;
        split(left, key - 1, left, temp);
        
        if (!temp) {
            temp = new Node(key);
        }
        
        root = merge(merge(left, temp), right);
    }
    
    // 删除
    void remove(int key) {
        Node* left, *right, *temp;
        split(root, key, left, right);
        split(left, key - 1, left, temp);
        
        if (temp) {
            delete temp;
        }
        
        root = merge(left, right);
    }
    
    // 查找
    bool search(int key) {
        return searchHelper(root, key);
    }
    
    bool searchHelper(Node* node, int key) {
        if (!node) return false;
        if (node->key == key) return true;
        if (key < node->key) return searchHelper(node->left, key);
        return searchHelper(node->right, key);
    }
    
    // 中序遍历
    void inorder() {
        inorderHelper(root);
        cout << endl;
    }
    
    void inorderHelper(Node* node) {
        if (!node) return;
        inorderHelper(node->left);
        cout << "(" << node->key << "," << node->priority << ") ";
        inorderHelper(node->right);
    }
    
    // 显示树结构
    void printTree() {
        cout << "当前树结构：" << endl;
        printHelper(root, "", true);
        cout << endl;
    }
    
    void printHelper(Node* node, string indent, bool isLast) {
        if (!node) return;
        
        cout << indent;
        if (isLast) {
            cout << "└── ";
            indent += "    ";
        } else {
            cout << "├── ";
            indent += "│   ";
        }
        cout << "(" << node->key << "," << node->priority << ")" << endl;
        
        if (node->left || node->right) {
            if (node->right) {
                printHelper(node->right, indent, !node->left);
            }
            if (node->left) {
                printHelper(node->left, indent, true);
            }
        }
    }
};
```

### 🎮 测试代码

```cpp
int main() {
    FHQTreap treap;
    
    cout << "=== FHQ-Treap演示 ===" << endl;
    
    // 插入一些数据
    cout << "插入: 10, 20, 30, 15, 25" << endl;
    treap.insert(10);
    treap.insert(20);
    treap.insert(30);
    treap.insert(15);
    treap.insert(25);
    
    cout << "插入后的树：" << endl;
    treap.printTree();
    
    cout << "中序遍历：";
    treap.inorder();
    
    // 删除一个节点
    cout << "删除 20" << endl;
    treap.remove(20);
    
    cout << "删除后的树：" << endl;
    treap.printTree();
    
    // 查找测试
    cout << "查找 15: " << (treap.search(15) ? "找到" : "未找到") << endl;
    cout << "查找 20: " << (treap.search(20) ? "找到" : "未找到") << endl;
    
    return 0;
}
```

---

## 🎯 第四章：支持排名的完整版本

### 🌟 加入size信息

```cpp
struct RankNode {
    int key, priority, size;
    RankNode* left;
    RankNode* right;
    
    RankNode(int k) : key(k), priority(rand()), size(1), 
                      left(nullptr), right(nullptr) {}
};

class RankTreap {
private:
    RankNode* root;
    
    // 更新size
    void updateSize(RankNode* node) {
        if (!node) return;
        node->size = 1;
        if (node->left) node->size += node->left->size;
        if (node->right) node->size += node->right->size;
    }
    
    // 按值分裂
    void split(RankNode* root, int key, RankNode*& left, RankNode*& right) {
        if (!root) {
            left = right = nullptr;
            return;
        }
        
        if (root->key <= key) {
            left = root;
            split(root->right, key, left->right, right);
        } else {
            right = root;
            split(root->left, key, left, right->left);
        }
        updateSize(left);
        updateSize(right);
    }
    
    // 按排名分裂
    void splitByRank(RankNode* root, int rank, RankNode*& left, RankNode*& right) {
        if (!root) {
            left = right = nullptr;
            return;
        }
        
        int leftSize = root->left ? root->left->size : 0;
        
        if (leftSize < rank) {
            left = root;
            splitByRank(root->right, rank - leftSize - 1, left->right, right);
        } else {
            right = root;
            splitByRank(root->left, rank, left, right->left);
        }
        updateSize(left);
        updateSize(right);
    }
    
    // 合并
    RankNode* merge(RankNode* left, RankNode* right) {
        if (!left) return right;
        if (!right) return left;
        
        if (left->priority > right->priority) {
            left->right = merge(left->right, right);
            updateSize(left);
            return left;
        } else {
            right->left = merge(left, right->left);
            updateSize(right);
            return right;
        }
    }
    
public:
    RankTreap() : root(nullptr) {
        srand(time(0));
    }
    
    // 插入
    void insert(int key) {
        RankNode* left, *right;
        split(root, key, left, right);
        
        RankNode* temp;
        split(left, key - 1, left, temp);
        
        if (!temp) {
            temp = new RankNode(key);
        }
        
        root = merge(merge(left, temp), right);
    }
    
    // 删除
    void remove(int key) {
        RankNode* left, *right, *temp;
        split(root, key, left, right);
        split(left, key - 1, left, temp);
        
        if (temp) {
            delete temp;
        }
        
        root = merge(left, right);
    }
    
    // 查询第k小（1-indexed）
    int kth(int k) {
        return kthHelper(root, k);
    }
    
    int kthHelper(RankNode* node, int k) {
        if (!node) return -1;
        
        int leftSize = node->left ? node->left->size : 0;
        
        if (k <= leftSize) {
            return kthHelper(node->left, k);
        } else if (k == leftSize + 1) {
            return node->key;
        } else {
            return kthHelper(node->right, k - leftSize - 1);
        }
    }
    
    // 查询排名
    int getRank(int key) {
        RankNode* left, *right;
        split(root, key - 1, left, right);
        
        int rank = (left ? left->size : 0) + 1;
        
        root = merge(left, right);
        return rank;
    }
    
    // 查询前驱（小于key的最大值）
    int getPredecessor(int key) {
        RankNode* left, *right;
        split(root, key - 1, left, right);
        
        int result = -1;
        if (left) {
            result = kthHelper(left, left->size);
        }
        
        root = merge(left, right);
        return result;
    }
    
    // 查询后继（大于key的最小值）
    int getSuccessor(int key) {
        RankNode* left, *right;
        split(root, key, left, right);
        
        int result = -1;
        if (right) {
            result = kthHelper(right, 1);
        }
        
        root = merge(left, right);
        return result;
    }
    
    // 显示树结构
    void printTree() {
        cout << "当前树结构：" << endl;
        printHelper(root, "", true);
        cout << endl;
    }
    
    void printHelper(RankNode* node, string indent, bool isLast) {
        if (!node) return;
        
        cout << indent;
        if (isLast) {
            cout << "└── ";
            indent += "    ";
        } else {
            cout << "├── ";
            indent += "│   ";
        }
        cout << "(" << node->key << "," << node->priority << ",size:" << node->size << ")" << endl;
        
        if (node->left || node->right) {
            if (node->right) {
                printHelper(node->right, indent, !node->left);
            }
            if (node->left) {
                printHelper(node->left, indent, true);
            }
        }
    }
};
```

---

## 🎪 第五章：区间操作的Treap

### 🌟 支持区间翻转

```cpp
struct IntervalNode {
    int value, priority, size;
    bool reversed;
    IntervalNode* left;
    IntervalNode* right;
    
    IntervalNode(int v) : value(v), priority(rand()), size(1), 
                         reversed(false), left(nullptr), right(nullptr) {}
};

class IntervalTreap {
private:
    IntervalNode* root;
    
    // 下推标记
    void pushDown(IntervalNode* node) {
        if (!node || !node->reversed) return;
        
        // 交换左右子树
        swap(node->left, node->right);
        
        // 传递标记
        if (node->left) node->left->reversed ^= true;
        if (node->right) node->right->reversed ^= true;
        
        node->reversed = false;
    }
    
    // 更新size
    void updateSize(IntervalNode* node) {
        if (!node) return;
        node->size = 1;
        if (node->left) node->size += node->left->size;
        if (node->right) node->size += node->right->size;
    }
    
    // 按排名分裂
    void splitByRank(IntervalNode* root, int rank, IntervalNode*& left, IntervalNode*& right) {
        if (!root) {
            left = right = nullptr;
            return;
        }
        
        pushDown(root);
        
        int leftSize = root->left ? root->left->size : 0;
        
        if (leftSize < rank) {
            left = root;
            splitByRank(root->right, rank - leftSize - 1, left->right, right);
        } else {
            right = root;
            splitByRank(root->left, rank, left, right->left);
        }
        updateSize(left);
        updateSize(right);
    }
    
    // 合并
    IntervalNode* merge(IntervalNode* left, IntervalNode* right) {
        if (!left) return right;
        if (!right) return left;
        
        if (left->priority > right->priority) {
            pushDown(left);
            left->right = merge(left->right, right);
            updateSize(left);
            return left;
        } else {
            pushDown(right);
            right->left = merge(left, right->left);
            updateSize(right);
            return right;
        }
    }
    
public:
    IntervalTreap() : root(nullptr) {
        srand(time(0));
    }
    
    // 从数组构建
    void build(vector<int>& arr) {
        for (int i = 0; i < arr.size(); i++) {
            insert(i + 1, arr[i]);
        }
    }
    
    // 在位置pos插入值value
    void insert(int pos, int value) {
        IntervalNode* left, *right;
        splitByRank(root, pos - 1, left, right);
        
        IntervalNode* newNode = new IntervalNode(value);
        root = merge(merge(left, newNode), right);
    }
    
    // 删除位置pos的元素
    void remove(int pos) {
        IntervalNode* left, *right, *temp;
        splitByRank(root, pos - 1, left, right);
        splitByRank(right, 1, temp, right);
        
        if (temp) {
            delete temp;
        }
        
        root = merge(left, right);
    }
    
    // 翻转区间[l, r]
    void reverse(int l, int r) {
        IntervalNode* left, *right, *middle;
        
        // 分离出[l, r]区间
        splitByRank(root, l - 1, left, right);
        splitByRank(right, r - l + 1, middle, right);
        
        // 给中间部分打上翻转标记
        if (middle) {
            middle->reversed ^= true;
        }
        
        // 合并回去
        root = merge(merge(left, middle), right);
    }
    
    // 输出序列
    void printSequence() {
        cout << "当前序列：";
        printSequenceHelper(root);
        cout << endl;
    }
    
    void printSequenceHelper(IntervalNode* node) {
        if (!node) return;
        
        pushDown(node);
        printSequenceHelper(node->left);
        cout << node->value << " ";
        printSequenceHelper(node->right);
    }
};
```

### 🎮 区间操作测试

```cpp
int main() {
    IntervalTreap treap;
    
    cout << "=== 区间操作Treap演示 ===" << endl;
    
    // 构建初始序列 [1, 2, 3, 4, 5]
    vector<int> arr = {1, 2, 3, 4, 5};
    treap.build(arr);
    
    cout << "初始序列：";
    treap.printSequence();
    
    // 翻转区间[2, 4]
    cout << "翻转区间[2, 4]" << endl;
    treap.reverse(2, 4);
    treap.printSequence();
    
    // 翻转区间[1, 3]
    cout << "翻转区间[1, 3]" << endl;
    treap.reverse(1, 3);
    treap.printSequence();
    
    return 0;
}
```

---

## 🏆 第六章：Treap的优势

### ✨ 为什么选择Treap？

1. **实现简单** - 比红黑树、AVL树简单得多
2. **期望性能好** - 平均O(log n)的时间复杂度
3. **代码量少** - 特别是FHQ-Treap
4. **功能强大** - 支持各种复杂操作

### 🎯 适用场景

- **普通平衡树** - 插入、删除、查找、排名
- **区间操作** - 区间翻转、区间查询
- **动态序列** - 支持任意位置插入删除
- **可持久化** - 容易实现历史版本

### 📊 性能对比

| 操作 | 时间复杂度 | 空间复杂度 |
|------|------------|------------|
| 插入 | O(log n) | O(1) |
| 删除 | O(log n) | O(1) |
| 查找 | O(log n) | O(1) |
| 第k小 | O(log n) | O(1) |
| 区间翻转 | O(log n) | O(1) |

---

## 🎓 第七章：经典应用

### 🌟 洛谷P3369 - 普通平衡树

```cpp
int main() {
    RankTreap treap;
    int n;
    cin >> n;
    
    while (n--) {
        int op, x;
        cin >> op >> x;
        
        switch (op) {
            case 1:
                treap.insert(x);
                break;
            case 2:
                treap.remove(x);
                break;
            case 3:
                cout << treap.getRank(x) << "\n";
                break;
            case 4:
                cout << treap.kth(x) << "\n";
                break;
            case 5:
                cout << treap.getPredecessor(x) << "\n";
                break;
            case 6:
                cout << treap.getSuccessor(x) << "\n";
                break;
        }
    }
    
    return 0;
}
```

### 🎪 洛谷P3391 - 文艺平衡树

```cpp
int main() {
    IntervalTreap treap;
    int n, m;
    cin >> n >> m;
    
    // 构建初始序列
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
    treap.build(arr);
    
    // 处理翻转操作
    for (int i = 0; i < m; i++) {
        int l, r;
        cin >> l >> r;
        treap.reverse(l, r);
    }
    
    // 输出结果
    treap.printSequence();
    
    return 0;
}
```

---

## 🎯 第八章：调试技巧

### 🔍 常见错误

1. **忘记更新size** - 每次merge后要更新
2. **split逻辑错误** - 注意≤和<的区别
3. **随机数问题** - 记得初始化随机种子
4. **内存泄漏** - 删除节点时释放内存

### 🛠️ 调试方法

```cpp
// 检查树的性质
bool checkTreeProperties(Node* node) {
    if (!node) return true;
    
    // 检查BST性质
    if (node->left && node->left->key > node->key) return false;
    if (node->right && node->right->key < node->key) return false;
    
    // 检查堆性质
    if (node->left && node->left->priority > node->priority) return false;
    if (node->right && node->right->priority > node->priority) return false;
    
    return checkTreeProperties(node->left) && checkTreeProperties(node->right);
}

// 检查size是否正确
bool checkSize(Node* node) {
    if (!node) return true;
    
    int expectedSize = 1;
    if (node->left) expectedSize += node->left->size;
    if (node->right) expectedSize += node->right->size;
    
    return node->size == expectedSize && 
           checkSize(node->left) && checkSize(node->right);
}
```

---

## 🎪 总结

### 🌸 记住这些关键点

1. **Treap = BST + Heap** - 双重性质保证平衡
2. **FHQ-Treap更好写** - 只需要split和merge
3. **随机优先级是关键** - 保证期望平衡
4. **支持丰富操作** - 排名、区间、持久化

### 🌟 学习建议

1. **先掌握基本概念** - 理解BST和Heap的结合
2. **练习split和merge** - 这是FHQ-Treap的核心
3. **多做题目** - 从普通平衡树到区间操作
4. **注意实现细节** - size更新、内存管理等

### 🎯 下节课预告

下次アリス老师将为大家讲解**替罪羊树**：一个通过重构来维护平衡的特殊平衡树！

---

**🌸 "Treap用随机的力量创造平衡，体现了算法设计的巧妙！" - アリス老师**

*千禧年科技学院 - 让每个数据结构都充满魅力！* 
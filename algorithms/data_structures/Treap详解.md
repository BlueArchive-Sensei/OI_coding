# 🌸 Treap详解
## *千禧年科技学院 - ユウカ数据结构专题*

*"Treap完美结合了二叉搜索树和堆的优点，随机性保证了优秀的期望性能！"*

---

## 📚 目录

1. [Treap基本概念](#treap基本概念)
2. [核心操作详解](#核心操作详解)
3. [FHQ-Treap详解](#fhq-treap详解)
4. [算法实现](#算法实现)
5. [复杂度分析](#复杂度分析)
6. [经典应用](#经典应用)
7. [优化技巧](#优化技巧)

---

## 🎯 Treap基本概念

### 定义

**Treap（Tree + Heap）**是一种随机化的二叉搜索树，由Cecilia Aragon和Raimund Seidel在1989年提出。它同时满足：

1. **BST性质**：对于key值，满足二叉搜索树的性质
2. **Heap性质**：对于priority值，满足堆的性质

### 🌟 核心思想

#### 随机化平衡
```
通过随机优先级保证树的平衡性
期望深度为O(log n)
```

#### 双重性质
- **按key维护BST性质**：支持查找、插入、删除
- **按priority维护Heap性质**：保证树的平衡

### 🔍 结构示例

```
节点结构: (key, priority)

         (5, 9)
        /       \
    (3, 7)      (8, 6)
    /    \      /    \
(1, 4) (4, 5) (6, 3) (9, 2)

BST性质: 左 < 根 < 右 (按key)
Heap性质: 父 > 子 (按priority)
```

---

## 🚀 核心操作详解

### 🎭 基本旋转操作

#### 右旋 (rotateRight)
```
当左子节点的priority > 当前节点的priority时使用

    y               x
   / \             / \
  x   C    ->     A   y
 / \                 / \
A   B               B   C
```

#### 左旋 (rotateLeft)
```
当右子节点的priority > 当前节点的priority时使用

  x                 y
 / \               / \
A   y      ->     x   C
   / \           / \
  B   C         A   B
```

### 🌸 插入操作

```cpp
Node* insert(Node* root, int key, int priority) {
    if (!root) return new Node(key, priority);
    
    if (key < root->key) {
        root->left = insert(root->left, key, priority);
        if (root->left->priority > root->priority) {
            root = rotateRight(root);
        }
    } else {
        root->right = insert(root->right, key, priority);
        if (root->right->priority > root->priority) {
            root = rotateLeft(root);
        }
    }
    
    return root;
}
```

### 🎯 删除操作

```cpp
Node* remove(Node* root, int key) {
    if (!root) return nullptr;
    
    if (key < root->key) {
        root->left = remove(root->left, key);
    } else if (key > root->key) {
        root->right = remove(root->right, key);
    } else {
        // 找到要删除的节点
        if (!root->left && !root->right) {
            delete root;
            return nullptr;
        } else if (!root->left) {
            Node* temp = root->right;
            delete root;
            return temp;
        } else if (!root->right) {
            Node* temp = root->left;
            delete root;
            return temp;
        } else {
            // 两个子节点都存在，旋转priority大的上来
            if (root->left->priority > root->right->priority) {
                root = rotateRight(root);
                root->right = remove(root->right, key);
            } else {
                root = rotateLeft(root);
                root->left = remove(root->left, key);
            }
        }
    }
    
    return root;
}
```

---

## 🎪 FHQ-Treap详解

### 概念介绍

**FHQ-Treap（范浩强Treap）**是一种不需要旋转的Treap实现，通过**split**和**merge**操作实现所有功能。

### 🌟 核心操作

#### Split操作
```cpp
pair<Node*, Node*> split(Node* root, int key) {
    if (!root) return {nullptr, nullptr};
    
    if (root->key <= key) {
        auto [l, r] = split(root->right, key);
        root->right = l;
        return {root, r};
    } else {
        auto [l, r] = split(root->left, key);
        root->left = r;
        return {l, root};
    }
}
```

#### Merge操作
```cpp
Node* merge(Node* left, Node* right) {
    if (!left || !right) return left ? left : right;
    
    if (left->priority > right->priority) {
        left->right = merge(left->right, right);
        return left;
    } else {
        right->left = merge(left, right->left);
        return right;
    }
}
```

#### 基于Split/Merge的操作
```cpp
Node* insert(Node* root, int key) {
    auto [l, r] = split(root, key);
    Node* newNode = new Node(key, rand());
    return merge(merge(l, newNode), r);
}

Node* remove(Node* root, int key) {
    auto [l, temp] = split(root, key);
    auto [mid, r] = split(temp, key);
    return merge(l, r);
}
```

---

## 💻 完整算法实现

### 🌸 传统Treap实现

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreapNode {
    int key, priority, size;
    TreapNode *left, *right;
    
    TreapNode(int k) : key(k), priority(rand()), size(1), 
                       left(nullptr), right(nullptr) {}
};

class Treap {
private:
    TreapNode* root;
    
    void updateSize(TreapNode* node) {
        if (!node) return;
        node->size = 1;
        if (node->left) node->size += node->left->size;
        if (node->right) node->size += node->right->size;
    }
    
    TreapNode* rotateLeft(TreapNode* x) {
        TreapNode* y = x->right;
        x->right = y->left;
        y->left = x;
        updateSize(x);
        updateSize(y);
        return y;
    }
    
    TreapNode* rotateRight(TreapNode* y) {
        TreapNode* x = y->left;
        y->left = x->right;
        x->right = y;
        updateSize(y);
        updateSize(x);
        return x;
    }
    
    TreapNode* insertHelper(TreapNode* root, int key) {
        if (!root) return new TreapNode(key);
        
        if (key < root->key) {
            root->left = insertHelper(root->left, key);
            if (root->left->priority > root->priority) {
                root = rotateRight(root);
            }
        } else if (key > root->key) {
            root->right = insertHelper(root->right, key);
            if (root->right->priority > root->priority) {
                root = rotateLeft(root);
            }
        }
        
        updateSize(root);
        return root;
    }
    
    TreapNode* removeHelper(TreapNode* root, int key) {
        if (!root) return nullptr;
        
        if (key < root->key) {
            root->left = removeHelper(root->left, key);
        } else if (key > root->key) {
            root->right = removeHelper(root->right, key);
        } else {
            if (!root->left && !root->right) {
                delete root;
                return nullptr;
            } else if (!root->left) {
                TreapNode* temp = root->right;
                delete root;
                return temp;
            } else if (!root->right) {
                TreapNode* temp = root->left;
                delete root;
                return temp;
            } else {
                if (root->left->priority > root->right->priority) {
                    root = rotateRight(root);
                    root->right = removeHelper(root->right, key);
                } else {
                    root = rotateLeft(root);
                    root->left = removeHelper(root->left, key);
                }
            }
        }
        
        updateSize(root);
        return root;
    }
    
    bool searchHelper(TreapNode* root, int key) {
        if (!root) return false;
        
        if (key == root->key) return true;
        else if (key < root->key) return searchHelper(root->left, key);
        else return searchHelper(root->right, key);
    }
    
    int kthHelper(TreapNode* root, int k) {
        if (!root) return -1;
        
        int leftSize = root->left ? root->left->size : 0;
        
        if (k <= leftSize) {
            return kthHelper(root->left, k);
        } else if (k == leftSize + 1) {
            return root->key;
        } else {
            return kthHelper(root->right, k - leftSize - 1);
        }
    }
    
    int getRankHelper(TreapNode* root, int key) {
        if (!root) return 0;
        
        if (key <= root->key) {
            return getRankHelper(root->left, key);
        } else {
            int leftSize = root->left ? root->left->size : 0;
            return leftSize + 1 + getRankHelper(root->right, key);
        }
    }
    
public:
    Treap() : root(nullptr) {
        srand(time(nullptr));
    }
    
    void insert(int key) {
        root = insertHelper(root, key);
    }
    
    void remove(int key) {
        root = removeHelper(root, key);
    }
    
    bool search(int key) {
        return searchHelper(root, key);
    }
    
    int kth(int k) {
        return kthHelper(root, k);
    }
    
    int getRank(int key) {
        return getRankHelper(root, key) + 1;
    }
    
    int getPredecessor(int key) {
        TreapNode* current = root;
        int result = -1;
        
        while (current) {
            if (current->key < key) {
                result = current->key;
                current = current->right;
            } else {
                current = current->left;
            }
        }
        
        return result;
    }
    
    int getSuccessor(int key) {
        TreapNode* current = root;
        int result = -1;
        
        while (current) {
            if (current->key > key) {
                result = current->key;
                current = current->left;
            } else {
                current = current->right;
            }
        }
        
        return result;
    }
    
    void inorderTraversal() {
        inorderHelper(root);
        cout << endl;
    }
    
    void inorderHelper(TreapNode* root) {
        if (!root) return;
        inorderHelper(root->left);
        cout << root->key << " ";
        inorderHelper(root->right);
    }
    
    int getSize() {
        return root ? root->size : 0;
    }
};
```

### 🎯 FHQ-Treap实现

```cpp
#include <bits/stdc++.h>
using namespace std;

struct FHQNode {
    int key, priority, size;
    FHQNode *left, *right;
    
    FHQNode(int k) : key(k), priority(rand()), size(1), 
                     left(nullptr), right(nullptr) {}
};

class FHQTreap {
private:
    FHQNode* root;
    
    void updateSize(FHQNode* node) {
        if (!node) return;
        node->size = 1;
        if (node->left) node->size += node->left->size;
        if (node->right) node->size += node->right->size;
    }
    
    pair<FHQNode*, FHQNode*> split(FHQNode* root, int key) {
        if (!root) return {nullptr, nullptr};
        
        if (root->key <= key) {
            auto [l, r] = split(root->right, key);
            root->right = l;
            updateSize(root);
            return {root, r};
        } else {
            auto [l, r] = split(root->left, key);
            root->left = r;
            updateSize(root);
            return {l, root};
        }
    }
    
    pair<FHQNode*, FHQNode*> splitByRank(FHQNode* root, int k) {
        if (!root) return {nullptr, nullptr};
        
        int leftSize = root->left ? root->left->size : 0;
        
        if (leftSize >= k) {
            auto [l, r] = splitByRank(root->left, k);
            root->left = r;
            updateSize(root);
            return {l, root};
        } else {
            auto [l, r] = splitByRank(root->right, k - leftSize - 1);
            root->right = l;
            updateSize(root);
            return {root, r};
        }
    }
    
    FHQNode* merge(FHQNode* left, FHQNode* right) {
        if (!left || !right) return left ? left : right;
        
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
    FHQTreap() : root(nullptr) {
        srand(time(nullptr));
    }
    
    void insert(int key) {
        auto [l, r] = split(root, key);
        FHQNode* newNode = new FHQNode(key);
        root = merge(merge(l, newNode), r);
    }
    
    void remove(int key) {
        auto [l, temp] = split(root, key - 1);
        auto [mid, r] = split(temp, key);
        root = merge(l, r);
    }
    
    bool search(int key) {
        auto [l, temp] = split(root, key - 1);
        auto [mid, r] = split(temp, key);
        bool found = (mid != nullptr);
        root = merge(merge(l, mid), r);
        return found;
    }
    
    int kth(int k) {
        auto [l, r] = splitByRank(root, k - 1);
        auto [mid, r2] = splitByRank(r, 1);
        int result = mid ? mid->key : -1;
        root = merge(merge(l, mid), r2);
        return result;
    }
    
    int getRank(int key) {
        auto [l, r] = split(root, key - 1);
        int rank = (l ? l->size : 0) + 1;
        root = merge(l, r);
        return rank;
    }
    
    int getPredecessor(int key) {
        auto [l, r] = split(root, key - 1);
        int result = -1;
        
        if (l) {
            FHQNode* current = l;
            while (current->right) current = current->right;
            result = current->key;
        }
        
        root = merge(l, r);
        return result;
    }
    
    int getSuccessor(int key) {
        auto [l, r] = split(root, key);
        int result = -1;
        
        if (r) {
            FHQNode* current = r;
            while (current->left) current = current->left;
            result = current->key;
        }
        
        root = merge(l, r);
        return result;
    }
    
    // 区间操作支持
    void reverse(int l, int r) {
        auto [left, temp] = splitByRank(root, l - 1);
        auto [mid, right] = splitByRank(temp, r - l + 1);
        
        // 这里需要在节点中添加reverse标记
        // mid->reversed ^= true;
        
        root = merge(merge(left, mid), right);
    }
    
    void insertAt(int pos, int key) {
        auto [l, r] = splitByRank(root, pos - 1);
        FHQNode* newNode = new FHQNode(key);
        root = merge(merge(l, newNode), r);
    }
    
    void removeAt(int pos) {
        auto [l, temp] = splitByRank(root, pos - 1);
        auto [mid, r] = splitByRank(temp, 1);
        root = merge(l, r);
        delete mid;
    }
    
    void inorderTraversal() {
        inorderHelper(root);
        cout << endl;
    }
    
    void inorderHelper(FHQNode* root) {
        if (!root) return;
        inorderHelper(root->left);
        cout << root->key << " ";
        inorderHelper(root->right);
    }
    
    int getSize() {
        return root ? root->size : 0;
    }
};
```

### 🎪 带懒惰标记的FHQ-Treap

```cpp
struct LazyFHQNode {
    int key, priority, size;
    long long sum, lazy;
    bool reversed;
    LazyFHQNode *left, *right;
    
    LazyFHQNode(int k) : key(k), priority(rand()), size(1), 
                         sum(k), lazy(0), reversed(false),
                         left(nullptr), right(nullptr) {}
    
    void pushDown() {
        if (reversed) {
            swap(left, right);
            if (left) left->reversed ^= true;
            if (right) right->reversed ^= true;
            reversed = false;
        }
        
        if (lazy != 0) {
            key += lazy;
            sum += lazy * size;
            if (left) left->lazy += lazy;
            if (right) right->lazy += lazy;
            lazy = 0;
        }
    }
    
    void update() {
        size = 1;
        sum = key;
        if (left) {
            size += left->size;
            sum += left->sum;
        }
        if (right) {
            size += right->size;
            sum += right->sum;
        }
    }
};

class LazyFHQTreap {
private:
    LazyFHQNode* root;
    
    LazyFHQNode* merge(LazyFHQNode* left, LazyFHQNode* right) {
        if (!left || !right) return left ? left : right;
        
        if (left->priority > right->priority) {
            left->pushDown();
            left->right = merge(left->right, right);
            left->update();
            return left;
        } else {
            right->pushDown();
            right->left = merge(left, right->left);
            right->update();
            return right;
        }
    }
    
    pair<LazyFHQNode*, LazyFHQNode*> splitByRank(LazyFHQNode* root, int k) {
        if (!root) return {nullptr, nullptr};
        
        root->pushDown();
        int leftSize = root->left ? root->left->size : 0;
        
        if (leftSize >= k) {
            auto [l, r] = splitByRank(root->left, k);
            root->left = r;
            root->update();
            return {l, root};
        } else {
            auto [l, r] = splitByRank(root->right, k - leftSize - 1);
            root->right = l;
            root->update();
            return {root, r};
        }
    }
    
public:
    LazyFHQTreap() : root(nullptr) {
        srand(time(nullptr));
    }
    
    void insert(int pos, int key) {
        auto [l, r] = splitByRank(root, pos);
        LazyFHQNode* newNode = new LazyFHQNode(key);
        root = merge(merge(l, newNode), r);
    }
    
    void remove(int pos) {
        auto [l, temp] = splitByRank(root, pos - 1);
        auto [mid, r] = splitByRank(temp, 1);
        root = merge(l, r);
        delete mid;
    }
    
    void reverse(int l, int r) {
        auto [left, temp] = splitByRank(root, l - 1);
        auto [mid, right] = splitByRank(temp, r - l + 1);
        
        if (mid) mid->reversed ^= true;
        
        root = merge(merge(left, mid), right);
    }
    
    void add(int l, int r, int val) {
        auto [left, temp] = splitByRank(root, l - 1);
        auto [mid, right] = splitByRank(temp, r - l + 1);
        
        if (mid) mid->lazy += val;
        
        root = merge(merge(left, mid), right);
    }
    
    long long querySum(int l, int r) {
        auto [left, temp] = splitByRank(root, l - 1);
        auto [mid, right] = splitByRank(temp, r - l + 1);
        
        long long result = mid ? mid->sum : 0;
        
        root = merge(merge(left, mid), right);
        return result;
    }
    
    int queryAt(int pos) {
        auto [l, temp] = splitByRank(root, pos - 1);
        auto [mid, r] = splitByRank(temp, 1);
        
        int result = -1;
        if (mid) {
            mid->pushDown();
            result = mid->key;
        }
        
        root = merge(merge(l, mid), r);
        return result;
    }
};
```

---

## ⚡ 复杂度分析

### 时间复杂度

| 操作 | 期望复杂度 | 最坏情况 |
|------|------------|----------|
| **查找** | O(log n) | O(n) |
| **插入** | O(log n) | O(n) |
| **删除** | O(log n) | O(n) |
| **第k小** | O(log n) | O(n) |
| **分裂** | O(log n) | O(n) |
| **合并** | O(log n) | O(n) |

### 🎯 随机化分析

#### 期望深度证明
对于随机构建的Treap，期望深度为O(log n)。

**证明思路**：
- 每个节点的priority是随机的
- 对于任意路径，期望长度为O(log n)
- 类似于随机化快速排序的分析

#### 平衡性保证
随机priority保证了树的平衡性：
- 不需要复杂的旋转规则
- 结构完全由随机性决定
- 避免了特殊输入导致的退化

### 空间复杂度
- **基本版本**：O(n)
- **支持区间操作**：O(n)（需要额外的标记）

---

## 🏆 经典应用

### 应用场景

#### 1. 普通平衡树
```cpp
void solveBST() {
    FHQTreap treap;
    int q;
    cin >> q;
    
    while (q--) {
        int op;
        cin >> op;
        
        switch (op) {
            case 1: {  // 插入
                int x;
                cin >> x;
                treap.insert(x);
                break;
            }
            case 2: {  // 删除
                int x;
                cin >> x;
                treap.remove(x);
                break;
            }
            case 3: {  // 查询排名
                int x;
                cin >> x;
                cout << treap.getRank(x) << "\n";
                break;
            }
            case 4: {  // 查询第k小
                int k;
                cin >> k;
                cout << treap.kth(k) << "\n";
                break;
            }
            case 5: {  // 前驱
                int x;
                cin >> x;
                cout << treap.getPredecessor(x) << "\n";
                break;
            }
            case 6: {  // 后继
                int x;
                cin >> x;
                cout << treap.getSuccessor(x) << "\n";
                break;
            }
        }
    }
}
```

#### 2. 区间操作
```cpp
void solveInterval() {
    LazyFHQTreap treap;
    
    // 初始化序列
    vector<int> arr = {1, 2, 3, 4, 5};
    for (int i = 0; i < arr.size(); i++) {
        treap.insert(i + 1, arr[i]);
    }
    
    int q;
    cin >> q;
    
    while (q--) {
        int op;
        cin >> op;
        
        if (op == 1) {  // 区间翻转
            int l, r;
            cin >> l >> r;
            treap.reverse(l, r);
        } else if (op == 2) {  // 区间加
            int l, r, val;
            cin >> l >> r >> val;
            treap.add(l, r, val);
        } else if (op == 3) {  // 区间求和
            int l, r;
            cin >> l >> r;
            cout << treap.querySum(l, r) << "\n";
        } else if (op == 4) {  // 插入
            int pos, val;
            cin >> pos >> val;
            treap.insert(pos, val);
        } else if (op == 5) {  // 删除
            int pos;
            cin >> pos;
            treap.remove(pos);
        }
    }
}
```

#### 3. 权值线段树替代
```cpp
class TreapSegment {
private:
    FHQTreap treap;
    
public:
    void update(int pos, int val) {
        treap.remove(pos);
        treap.insert(val);
    }
    
    int queryKth(int k) {
        return treap.kth(k);
    }
    
    int queryRank(int val) {
        return treap.getRank(val);
    }
    
    int queryPrev(int val) {
        return treap.getPredecessor(val);
    }
    
    int queryNext(int val) {
        return treap.getSuccessor(val);
    }
};
```

### 经典例题

#### 例题1：洛谷P3369 - 普通平衡树
```cpp
int main() {
    FHQTreap treap;
    int n;
    cin >> n;
    
    while (n--) {
        int op, x;
        cin >> op >> x;
        
        switch (op) {
            case 1: treap.insert(x); break;
            case 2: treap.remove(x); break;
            case 3: cout << treap.getRank(x) << "\n"; break;
            case 4: cout << treap.kth(x) << "\n"; break;
            case 5: cout << treap.getPredecessor(x) << "\n"; break;
            case 6: cout << treap.getSuccessor(x) << "\n"; break;
        }
    }
    
    return 0;
}
```

#### 例题2：洛谷P3391 - 文艺平衡树
```cpp
int main() {
    LazyFHQTreap treap;
    int n, m;
    cin >> n >> m;
    
    // 初始化序列1,2,3,...,n
    for (int i = 1; i <= n; i++) {
        treap.insert(i, i);
    }
    
    for (int i = 0; i < m; i++) {
        int l, r;
        cin >> l >> r;
        treap.reverse(l, r);
    }
    
    // 输出最终序列
    for (int i = 1; i <= n; i++) {
        cout << treap.queryAt(i);
        if (i < n) cout << " ";
    }
    cout << "\n";
    
    return 0;
}
```

#### 例题3：动态逆序对
```cpp
class DynamicInversion {
private:
    FHQTreap treap;
    long long inversions;
    
public:
    DynamicInversion() : inversions(0) {}
    
    void insert(int val) {
        int rank = treap.getRank(val);
        int total = treap.getSize();
        
        // 新插入的数会与排名在它后面的数形成逆序对
        inversions += total - rank + 1;
        
        treap.insert(val);
    }
    
    void remove(int val) {
        int rank = treap.getRank(val);
        int total = treap.getSize();
        
        // 删除时减去相应的逆序对数
        inversions -= total - rank;
        
        treap.remove(val);
    }
    
    long long getInversions() {
        return inversions;
    }
};
```

---

## 💡 优化技巧

### 实现优化

#### 1. 内存池
```cpp
class MemoryPool {
private:
    FHQNode pool[MAXN];
    int poolTop;
    
public:
    MemoryPool() : poolTop(0) {}
    
    FHQNode* newNode(int key) {
        pool[poolTop] = FHQNode(key);
        return &pool[poolTop++];
    }
    
    void reset() {
        poolTop = 0;
    }
};
```

#### 2. 非递归实现
```cpp
FHQNode* mergeIterative(FHQNode* left, FHQNode* right) {
    if (!left || !right) return left ? left : right;
    
    FHQNode* root = nullptr;
    FHQNode** current = &root;
    
    while (left && right) {
        if (left->priority > right->priority) {
            *current = left;
            current = &(left->right);
            left = left->right;
        } else {
            *current = right;
            current = &(right->left);
            right = right->left;
        }
    }
    
    *current = left ? left : right;
    
    // 更新size（需要额外处理）
    return root;
}
```

#### 3. 批量操作优化
```cpp
void batchInsert(vector<int>& elements) {
    sort(elements.begin(), elements.end());
    
    // 构建平衡的初始树
    function<FHQNode*(int, int)> build = [&](int l, int r) -> FHQNode* {
        if (l > r) return nullptr;
        
        int mid = (l + r) / 2;
        FHQNode* node = new FHQNode(elements[mid]);
        node->left = build(l, mid - 1);
        node->right = build(mid + 1, r);
        node->update();
        return node;
    };
    
    FHQNode* batch = build(0, elements.size() - 1);
    root = merge(root, batch);
}
```

### 应用优化

#### 1. 持久化Treap
```cpp
struct PersistentNode {
    int key, priority, size;
    PersistentNode *left, *right;
    
    PersistentNode(int k) : key(k), priority(rand()), size(1),
                           left(nullptr), right(nullptr) {}
    
    PersistentNode(const PersistentNode& other) 
        : key(other.key), priority(other.priority), size(other.size),
          left(other.left), right(other.right) {}
};

class PersistentTreap {
private:
    vector<PersistentNode*> versions;
    
    PersistentNode* copyNode(PersistentNode* node) {
        return node ? new PersistentNode(*node) : nullptr;
    }
    
public:
    void insert(int version, int key) {
        PersistentNode* oldRoot = versions[version];
        PersistentNode* newRoot = insertHelper(oldRoot, key);
        versions.push_back(newRoot);
    }
    
    bool query(int version, int key) {
        return searchHelper(versions[version], key);
    }
};
```

#### 2. 可持久化操作
```cpp
PersistentNode* insertHelper(PersistentNode* root, int key) {
    if (!root) return new PersistentNode(key);
    
    PersistentNode* newRoot = copyNode(root);
    
    if (key < newRoot->key) {
        newRoot->left = insertHelper(newRoot->left, key);
        if (newRoot->left->priority > newRoot->priority) {
            newRoot = rotateRight(newRoot);
        }
    } else {
        newRoot->right = insertHelper(newRoot->right, key);
        if (newRoot->right->priority > newRoot->priority) {
            newRoot = rotateLeft(newRoot);
        }
    }
    
    updateSize(newRoot);
    return newRoot;
}
```

---

## 🎓 学习建议

### 🌟 掌握要点

1. **理解双重性质**：BST性质和Heap性质的结合
2. **掌握旋转操作**：维护堆性质的关键
3. **熟练Split/Merge**：FHQ-Treap的核心操作
4. **灵活应用**：根据问题选择合适的实现方式

### 📚 推荐练习题

#### 入门级
1. **洛谷P3369** - 普通平衡树
2. **洛谷P3391** - 文艺平衡树
3. **洛谷P1110** - 报表统计

#### 进阶级
1. **洛谷P2042** - 维护数列
2. **洛谷P4008** - 文艺平衡树加强版
3. **CF785E** - Anton and Permutation

#### 挑战级
1. **洛谷P5055** - 可持久化文艺平衡树
2. **CF765F** - Souvenirs
3. **BZOJ3223** - 文艺平衡树

### 🚀 进阶方向

1. **可持久化Treap**：支持历史版本查询
2. **动态开点Treap**：处理大范围稀疏数据
3. **多维Treap**：处理多维数据的平衡树
4. **并行Treap**：支持并发操作的实现

---

<div align="center">

### 🌸 特别鸣谢

**感谢千禧年科技学院数据结构研究小组！**

> *"Treap的美妙之处在于它将随机性与确定性完美结合。简单的随机priority就能保证优秀的期望性能，这正是算法设计中的智慧体现！"*
>
> — **ユウカ**

*🌸 在随机化的世界里，Treap展现了概率与数据结构的和谐统一！*

</div> 
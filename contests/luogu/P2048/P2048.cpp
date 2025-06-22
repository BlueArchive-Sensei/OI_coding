#include <iostream>
#include <cstdio>
#include <queue>
#include <algorithm>
#include <vector>
#include <cstring>
using namespace std;

const int MAXN = 500005;

// 主席树节点
struct Node {
    int ls, rs, cnt;
    Node() : ls(0), rs(0), cnt(0) {}
} tree[MAXN * 20];

// 堆中的元素
struct HeapNode {
    long long val;  // 区间值
    int left;       // 左端点
    int rank;       // 第几大
    
    bool operator<(const HeapNode& other) const {
        return val < other.val;  // 大根堆
    }
};

int n, k, L, R;
long long a[MAXN], sum[MAXN];  // 原数组和前缀和
int b[MAXN], size_b;           // 离散化数组
int root[MAXN];                // 主席树根节点
int node_cnt = 0;              // 主席树节点计数
priority_queue<HeapNode> pq;   // 优先队列

// 更新主席树
void update(int& rt, int pre, int pos, int l, int r) {
    rt = ++node_cnt;
    tree[rt] = tree[pre];
    tree[rt].cnt++;
    
    if (l == r) return;
    
    int mid = (l + r) >> 1;
    if (pos <= mid) {
        update(tree[rt].ls, tree[pre].ls, pos, l, mid);
    } else {
        update(tree[rt].rs, tree[pre].rs, pos, mid + 1, r);
    }
}

// 查询主席树中第k大值
long long query(int rt1, int rt2, int k, int l, int r) {
    if (l == r) {
        return b[l];
    }
    
    int mid = (l + r) >> 1;
    int cnt = tree[tree[rt1].rs].cnt - tree[tree[rt2].rs].cnt;
    
    if (k <= cnt) {
        return query(tree[rt1].rs, tree[rt2].rs, k, mid + 1, r);
    } else {
        return query(tree[rt1].ls, tree[rt2].ls, k - cnt, l, mid);
    }
}

int main() {
    scanf("%d%d%d%d", &n, &k, &L, &R);
    
    // 读入数据并计算前缀和
    for (int i = 1; i <= n; i++) {
        scanf("%lld", &a[i]);
        sum[i] = sum[i-1] + a[i];
    }
    
    // 离散化前缀和
    for (int i = 0; i <= n; i++) {
        b[i+1] = sum[i];
    }
    sort(b + 1, b + n + 2);
    size_b = unique(b + 1, b + n + 2) - b - 1;
    
    // 建立主席树
    for (int i = 0; i <= n; i++) {
        int pos = lower_bound(b + 1, b + size_b + 1, sum[i]) - b;
        update(root[i+1], root[i], pos, 1, size_b);
    }
    
    // 初始化堆：为每个左端点找到最大的区间
    for (int i = 1; i <= n; i++) {
        int left_bound = i + L - 1;   // 最小右端点
        int right_bound = min(n, i + R - 1);  // 最大右端点
        
        if (left_bound > n) continue;  // 左端点不合法
        
        // 查询区间[left_bound, right_bound]中前缀和的最大值
        long long max_val = query(root[right_bound+1], root[left_bound], 1, 1, size_b);
        long long interval_val = max_val - sum[i-1];
        
        pq.push({interval_val, i, 1});
    }
    
    long long answer = 0;
    
    // 选择k个最大的区间
    for (int cnt = 0; cnt < k && !pq.empty(); cnt++) {
        HeapNode top = pq.top();
        pq.pop();
        
        answer += top.val;
        
        // 为当前左端点添加下一个最大的区间
        int left_bound = top.left + L - 1;
        int right_bound = min(n, top.left + R - 1);
        int total_cnt = right_bound - left_bound + 1;
        
        if (top.rank < total_cnt) {
            long long next_val = query(root[right_bound+1], root[left_bound], top.rank + 1, 1, size_b);
            long long interval_val = next_val - sum[top.left-1];
            
            pq.push({interval_val, top.left, top.rank + 1});
        }
    }
    
    printf("%lld\n", answer);
    
    return 0;
} 
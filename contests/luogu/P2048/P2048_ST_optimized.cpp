#include <iostream>
#include <cstdio>
#include <queue>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cmath>
using namespace std;

const int MAXN = 500005;
const int LOGN = 20;

int n, k, L, R;
long long a[MAXN], sum[MAXN];  // 原数组和前缀和
long long st[MAXN][LOGN];      // ST表，存储前缀和
int lg[MAXN];                  // 预处理log值

// 堆中的元素
struct HeapNode {
    long long val;     // 区间值
    int left;          // 左端点
    int rank;          // 在该左端点中的排名
    
    bool operator<(const HeapNode& other) const {
        return val < other.val;  // 大根堆
    }
};

priority_queue<HeapNode> pq;   // 优先队列

// 初始化ST表
void build_st() {
    // 预处理log值
    lg[1] = 0;
    for (int i = 2; i <= n + 1; i++) {
        lg[i] = lg[i / 2] + 1;
    }
    
    // 初始化ST表第一层
    for (int i = 0; i <= n; i++) {
        st[i][0] = sum[i];
    }
    
    // 构建ST表
    for (int j = 1; j < LOGN; j++) {
        for (int i = 0; i + (1 << j) - 1 <= n; i++) {
            st[i][j] = max(st[i][j-1], st[i + (1 << (j-1))][j-1]);
        }
    }
}

// ST表查询区间[l, r]的最大值
long long query_max(int l, int r) {
    if (l > r) return -1e18;  // 非法区间
    int len = r - l + 1;
    int k = lg[len];
    return max(st[l][k], st[r - (1 << k) + 1][k]);
}

// 获取指定左端点的第k大区间值
long long get_kth_max_interval(int left, int k_th) {
    int min_right = left + L - 1;  // 最小右端点
    int max_right = min(n, left + R - 1);  // 最大右端点
    
    if (min_right > n || k_th <= 0) return -1e18;  // 无合法区间
    
    vector<long long> values;
    
    // 收集所有可能的区间值
    for (int right = min_right; right <= max_right; right++) {
        long long val = sum[right] - sum[left - 1];
        values.push_back(val);
    }
    
    // 排序获取第k大
    sort(values.rbegin(), values.rend());
    
    if (k_th > values.size()) return -1e18;
    
    return values[k_th - 1];
}

// 使用ST表优化的方法：快速找到某个左端点的最优区间
long long get_best_interval_with_st(int left) {
    int min_right = left + L - 1;  // 最小右端点
    int max_right = min(n, left + R - 1);  // 最大右端点
    
    if (min_right > n) return -1e18;  // 无合法区间
    
    // 使用ST表查询区间[min_right, max_right]中前缀和的最大值
    long long max_prefix = query_max(min_right, max_right);
    
    return max_prefix - sum[left - 1];
}

int main() {
    scanf("%d%d%d%d", &n, &k, &L, &R);
    
    // 读入数据并计算前缀和
    for (int i = 1; i <= n; i++) {
        scanf("%lld", &a[i]);
        sum[i] = sum[i-1] + a[i];
    }
    
    // 构建ST表
    build_st();
    
    // 初始化堆：为每个左端点找到最大的区间
    for (int i = 1; i <= n; i++) {
        long long best_val = get_best_interval_with_st(i);
        if (best_val > -1e18) {
            pq.push({best_val, i, 1});
        }
    }
    
    long long answer = 0;
    
    // 选择k个最大的区间
    for (int cnt = 0; cnt < k && !pq.empty(); cnt++) {
        HeapNode top = pq.top();
        pq.pop();
        
        answer += top.val;
        
        // 为当前左端点添加下一个最大的区间
        long long next_val = get_kth_max_interval(top.left, top.rank + 1);
        if (next_val > -1e18) {
            pq.push({next_val, top.left, top.rank + 1});
        }
    }
    
    printf("%lld\n", answer);
    
    return 0;
} 
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
long long st[MAXN][LOGN];      // ST表
int lg[MAXN];                  // 预处理log值

// 堆中的元素
struct HeapNode {
    long long val;    // 区间值
    int left, right;  // 左右端点
    
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

// 获取某个左端点对应的所有可能区间，按美妙度降序排列
vector<HeapNode> get_intervals(int left) {
    vector<HeapNode> intervals;
    
    int min_right = left + L - 1;  // 最小右端点
    int max_right = min(n, left + R - 1);  // 最大右端点
    
    if (min_right > n) return intervals;  // 无合法区间
    
    // 枚举所有可能的右端点
    for (int right = min_right; right <= max_right; right++) {
        long long val = sum[right] - sum[left - 1];
        intervals.push_back({val, left, right});
    }
    
    // 按美妙度降序排列
    sort(intervals.rbegin(), intervals.rend());
    
    return intervals;
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
    
    // 为每个左端点预处理所有可能的区间
    vector<vector<HeapNode>> all_intervals(n + 1);
    vector<int> current_index(n + 1, 0);  // 每个左端点当前使用到的区间索引
    
    for (int i = 1; i <= n; i++) {
        all_intervals[i] = get_intervals(i);
        if (!all_intervals[i].empty()) {
            pq.push(all_intervals[i][0]);  // 将每个左端点的最大区间加入堆
        }
    }
    
    long long answer = 0;
    
    // 选择k个最大的区间
    for (int cnt = 0; cnt < k && !pq.empty(); cnt++) {
        HeapNode top = pq.top();
        pq.pop();
        
        answer += top.val;
        
        // 找到这个区间对应的左端点，并加入该左端点的下一个最大区间
        int left = top.left;
        current_index[left]++;
        
        if (current_index[left] < all_intervals[left].size()) {
            pq.push(all_intervals[left][current_index[left]]);
        }
    }
    
    printf("%lld\n", answer);
    
    return 0;
} 
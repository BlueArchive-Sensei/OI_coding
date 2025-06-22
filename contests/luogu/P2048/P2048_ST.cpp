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
const long long NEG_INF = -1e18;

int n, k, L, R;
long long a[MAXN], sum[MAXN];
long long st[MAXN][LOGN];
int st_pos[MAXN][LOGN];
int lg[MAXN];

struct IntervalNode {
    long long val;
    int left_pos;
    int left_bound;
    int right_bound;
    
    bool operator<(const IntervalNode& other) const {
        return val < other.val;
    }
};

priority_queue<IntervalNode> pq;

void build_st() {
    lg[1] = 0;
    for (int i = 2; i <= n + 1; i++) {
        lg[i] = lg[i / 2] + 1;
    }
    
    for (int i = 0; i <= n; i++) {
        st[i][0] = sum[i];
        st_pos[i][0] = i;
    }
    
    for (int j = 1; j < LOGN; j++) {
        for (int i = 0; i + (1 << j) - 1 <= n; i++) {
            if (st[i][j-1] >= st[i + (1 << (j-1))][j-1]) {
                st[i][j] = st[i][j-1];
                st_pos[i][j] = st_pos[i][j-1];
            } else {
                st[i][j] = st[i + (1 << (j-1))][j-1];
                st_pos[i][j] = st_pos[i + (1 << (j-1))][j-1];
            }
        }
    }
}

pair<long long, int> query_max_with_pos(int l, int r) {
    if (l > r) return make_pair(NEG_INF, -1);
    
    int len = r - l + 1;
    int k = lg[len];
    
    if (st[l][k] >= st[r - (1 << k) + 1][k]) {
        return make_pair(st[l][k], st_pos[l][k]);
    } else {
        return make_pair(st[r - (1 << k) + 1][k], st_pos[r - (1 << k) + 1][k]);
    }
}

IntervalNode create_interval_node(int left_pos, int left_bound, int right_bound) {
    if (left_bound > right_bound) {
        return {NEG_INF, left_pos, left_bound, right_bound};
    }
    
    pair<long long, int> max_info = query_max_with_pos(left_bound, right_bound);
    long long max_prefix = max_info.first;
    long long interval_val = max_prefix - sum[left_pos - 1];
    
    return {interval_val, left_pos, left_bound, right_bound};
}

int main() {
    scanf("%d%d%d%d", &n, &k, &L, &R);
    
    for (int i = 1; i <= n; i++) {
        scanf("%lld", &a[i]);
        sum[i] = sum[i-1] + a[i];
    }
    
    build_st();
    
    // 初始化堆
    for (int i = 1; i <= n; i++) {
        int left_bound = i + L - 1;
        int right_bound = min(n, i + R - 1);
        
        if (left_bound > n) continue;
        
        IntervalNode initial_node = create_interval_node(i, left_bound, right_bound);
        if (initial_node.val > NEG_INF) {
            pq.push(initial_node);
        }
    }
    
    long long answer = 0;
    
    // 选择k个最大的区间
    for (int cnt = 0; cnt < k && !pq.empty(); cnt++) {
        IntervalNode top = pq.top();
        pq.pop();
        
        answer += top.val;
        
        // 区间分裂
        pair<long long, int> max_info = query_max_with_pos(top.left_bound, top.right_bound);
        int max_pos = max_info.second;
        
        // 左子区间
        if (max_pos > top.left_bound) {
            IntervalNode left_child = create_interval_node(
                top.left_pos, 
                top.left_bound, 
                max_pos - 1
            );
            
            if (left_child.val > NEG_INF) {
                pq.push(left_child);
            }
        }
        
        // 右子区间
        if (max_pos < top.right_bound) {
            IntervalNode right_child = create_interval_node(
                top.left_pos, 
                max_pos + 1, 
                top.right_bound
            );
            
            if (right_child.val > NEG_INF) {
                pq.push(right_child);
            }
        }
    }
    
    printf("%lld\n", answer);
    
    return 0;
} 
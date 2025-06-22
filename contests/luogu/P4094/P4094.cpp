#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

const int MAXN = 100005;

// 后缀数组
struct SuffixArray {
    char s[MAXN];
    int n, m, sa[MAXN], t[MAXN], t1[MAXN], c[MAXN], rnk[MAXN], height[MAXN];
    
    void init() { memset(this, 0, sizeof(SuffixArray)); }
    
    void buildSA() {
        int *x = t, *y = t1;
        for (int i = 1; i <= m; i++) c[i] = 0;
        for (int i = 1; i <= n; i++) c[x[i] = s[i]]++;
        for (int i = 1; i <= m; i++) c[i] += c[i - 1];
        for (int i = n; i >= 1; i--) sa[c[x[i]]--] = i;
        
        for (int k = 1; k <= n; k <<= 1) {
            int p = 0;
            for (int i = n - k + 1; i <= n; i++) y[++p] = i;
            for (int i = 1; i <= n; i++) if (sa[i] > k) y[++p] = sa[i] - k;
            
            for (int i = 1; i <= m; i++) c[i] = 0;
            for (int i = 1; i <= n; i++) c[x[y[i]]]++;
            for (int i = 1; i <= m; i++) c[i] += c[i - 1];
            for (int i = n; i >= 1; i--) sa[c[x[y[i]]]--] = y[i];
            
            swap(x, y);
            p = x[sa[1]] = 1;
            for (int i = 2; i <= n; i++) 
                x[sa[i]] = (y[sa[i - 1]] == y[sa[i]] && y[sa[i - 1] + k] == y[sa[i] + k]) ? p : ++p;
            
            if (p >= n) break;
            m = p;
        }
        
        for (int i = 1; i <= n; i++) rnk[sa[i]] = i;
        
        // 构建height数组
        int k = 0;
        for (int i = 1; i <= n; i++) {
            if (rnk[i] == 1) continue;
            int j = sa[rnk[i] - 1];
            if (k) k--;
            while (i + k <= n && j + k <= n && s[i + k] == s[j + k]) k++;
            height[rnk[i]] = k;
        }
    }
} sa;

// ST表，查询height数组区间最小值
struct ST {
    int st[MAXN][20], lg[MAXN];
    
    void init() {
        lg[1] = 0;
        for (int i = 2; i < MAXN; i++) lg[i] = lg[i >> 1] + 1;
        
        for (int i = 1; i <= sa.n; i++) st[i][0] = sa.height[i];
        
        for (int j = 1; (1 << j) <= sa.n; j++) {
            for (int i = 1; i + (1 << j) - 1 <= sa.n; i++) {
                st[i][j] = min(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);
            }
        }
    }
    
    // 查询(l, r]的最小值（左开右闭）
    int query(int l, int r) {
        if (l >= r) return sa.n;
        int k = lg[r - l];
        return min(st[l + 1][k], st[r - (1 << k) + 1][k]);
    }
} st;

// 主席树，权值是后缀的实际编号
struct ChairmanTree {
    struct Node { int ls, rs, cnt; };
    vector<Node> tree;
    vector<int> root;
    int nodeCount;
    
    void init() {
        tree.resize(sa.n * 40);
        root.resize(sa.n + 5);
        nodeCount = 0;
        root[0] = build(1, sa.n);
        for (int i = 1; i <= sa.n; i++) {
            root[i] = update(root[i - 1], 1, sa.n, sa.sa[i]);
        }
    }
    
    int build(int l, int r) {
        int cur = ++nodeCount;
        tree[cur].cnt = 0;
        if (l == r) return cur;
        int mid = (l + r) >> 1;
        tree[cur].ls = build(l, mid);
        tree[cur].rs = build(mid + 1, r);
        return cur;
    }
    
    int update(int pre, int l, int r, int pos) {
        int cur = ++nodeCount;
        tree[cur] = tree[pre];
        tree[cur].cnt++;
        if (l == r) return cur;
        int mid = (l + r) >> 1;
        if (pos <= mid) tree[cur].ls = update(tree[pre].ls, l, mid, pos);
        else tree[cur].rs = update(tree[pre].rs, mid + 1, r, pos);
        return cur;
    }
    
    int query(int rt1, int rt2, int l, int r, int ql, int qr) {
        if (ql > qr || ql > r || qr < l) return 0;
        if (ql <= l && r <= qr) return tree[rt2].cnt - tree[rt1].cnt;
        int mid = (l + r) >> 1;
        return query(tree[rt1].ls, tree[rt2].ls, l, mid, ql, qr) +
               query(tree[rt1].rs, tree[rt2].rs, mid + 1, r, ql, qr);
    }
    
    // 查询区间[rankL, rankR]内位置在[posL, posR]的后缀个数
    int countInRange(int rankL, int rankR, int posL, int posR) {
        if (rankL > rankR) return 0;
        return query(root[rankL - 1], root[rankR], 1, sa.n, posL, posR);
    }
} ct;

// 二分查找满足lcp(sa[rank], c) >= mid的rank区间
pair<int, int> findLCPRange(int c, int mid) {
    int rc = sa.rnk[c];
    
    // 二分左端点
    int left = 1, right = rc, leftBound = rc + 1;
    while (left <= right) {
        int m = (left + right) >> 1;
        if (st.query(m, rc) >= mid) {
            leftBound = m;
            right = m - 1;
        } else {
            left = m + 1;
        }
    }
    
    // 二分右端点
    left = rc;
    right = sa.n;
    int rightBound = rc - 1;
    while (left <= right) {
        int m = (left + right) >> 1;
        if (st.query(rc, m) >= mid) {
            rightBound = m;
            left = m + 1;
        } else {
            right = m - 1;
        }
    }
    
    return make_pair(leftBound, rightBound);
}

// 判断答案mid是否可行
bool check(int a, int b, int c, int d, int mid) {
    if (mid > d - c + 1) return false;  // mid不能超过目标串长度
    
    // 找到满足lcp条件的rank区间
    pair<int, int> range = findLCPRange(c, mid);
    int leftRank = range.first;
    int rightRank = range.second;
    
    // 查询这个区间内是否有开头在[a, b-mid+1]的后缀
    return ct.countInRange(leftRank, rightRank, a, b - mid + 1) > 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    string str;
    cin >> str;
    
    sa.init();
    sa.n = n;
    sa.m = 256;
    for (int i = 0; i < n; i++) sa.s[i + 1] = str[i];
    
    sa.buildSA();
    st.init();
    ct.init();
    
    for (int q = 0; q < m; q++) {
        int a, b, c, d;
        cin >> a >> b >> c >> d;
        
        // 二分答案
        int left = 0, right = min(b - a + 1, d - c + 1), ans = 0;
        while (left <= right) {
            int mid = (left + right) >> 1;
            if (check(a, b, c, d, mid)) {
                ans = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        cout << ans << "\n";
    }
    
    return 0;
} 
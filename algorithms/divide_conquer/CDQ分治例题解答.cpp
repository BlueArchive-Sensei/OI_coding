/*
 * 洛谷P3810 - 陌上花开（三维偏序）
 * CDQ分治经典例题
 * 
 * 题目描述：
 * 有n朵花，每朵花有3个属性：美丽度ai、香味bi、高度ci
 * 对于每朵花，求有多少朵花的3个属性都不超过它
 * 
 * 解题思路：
 * 1. 按第一维美丽度排序
 * 2. CDQ分治处理第二维香味
 * 3. 树状数组维护第三维高度
 * 
 * 时间复杂度：O(n log² n)
 * 空间复杂度：O(n + k)
 */

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;
const int MAXK = 200005;

struct Flower {
    int a, b, c;    // 美丽度、香味、高度
    int id;         // 原始编号
    int cnt;        // 相同花的数量
    
    bool operator<(const Flower& other) const {
        if (a != other.a) return a < other.a;
        if (b != other.b) return b < other.b;
        return c < other.c;
    }
    
    bool operator==(const Flower& other) const {
        return a == other.a && b == other.b && c == other.c;
    }
};

Flower flowers[MAXN], temp[MAXN];
int ans[MAXN];
int tree[MAXK];
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

// CDQ分治核心函数
void cdq(int l, int r) {
    if (l == r) return;
    
    int mid = (l + r) / 2;
    cdq(l, mid);
    cdq(mid + 1, r);
    
    // 按第二维香味归并
    int i = l, j = mid + 1, p = l;
    
    while (i <= mid && j <= r) {
        if (flowers[i].b <= flowers[j].b) {
            // 左边的花可以对右边产生贡献
            update(flowers[i].c, flowers[i].cnt);
            temp[p++] = flowers[i++];
        } else {
            // 右边的花查询左边的贡献
            ans[flowers[j].id] += query(flowers[j].c);
            temp[p++] = flowers[j++];
        }
    }
    
    // 处理剩余的右边元素
    while (j <= r) {
        ans[flowers[j].id] += query(flowers[j].c);
        temp[p++] = flowers[j++];
    }
    
    // 处理剩余的左边元素
    while (i <= mid) {
        update(flowers[i].c, flowers[i].cnt);
        temp[p++] = flowers[i++];
    }
    
    // 清理树状数组
    for (int i = l; i <= mid; i++) {
        update(flowers[i].c, -flowers[i].cnt);
    }
    
    // 复制回原数组
    for (int i = l; i <= r; i++) {
        flowers[i] = temp[i];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n >> k;
    
    // 读入花的信息
    for (int i = 0; i < n; i++) {
        cin >> flowers[i].a >> flowers[i].b >> flowers[i].c;
        flowers[i].id = i;
        flowers[i].cnt = 1;
    }
    
    // 按第一维美丽度排序
    sort(flowers, flowers + n);
    
    // 去重处理，相同的花合并
    int m = 0;
    for (int i = 0; i < n; i++) {
        if (i == 0 || !(flowers[i] == flowers[i-1])) {
            flowers[m++] = flowers[i];
        } else {
            flowers[m-1].cnt++;
        }
    }
    
    // CDQ分治求解
    cdq(0, m - 1);
    
    // 统计答案
    vector<int> result(n);
    for (int i = 0; i < m; i++) {
        result[flowers[i].cnt - 1] += flowers[i].cnt;
    }
    
    // 输出结果
    for (int i = 0; i < n; i++) {
        cout << result[i] << "\n";
    }
    
    return 0;
}

/*
 * 测试样例：
 * 输入：
 * 5 3
 * 3 3 3
 * 2 3 3
 * 2 3 1
 * 3 1 1
 * 1 1 1
 * 
 * 输出：
 * 1
 * 1
 * 1
 * 2
 * 0
 * 
 * 解释：
 * 花0(3,3,3)：被花1(2,3,3)和花4(1,1,1)支配，答案为2
 * 花1(2,3,3)：被花4(1,1,1)支配，答案为1
 * 花2(2,3,1)：被花4(1,1,1)支配，答案为1
 * 花3(3,1,1)：被花4(1,1,1)支配，答案为1
 * 花4(1,1,1)：没有被任何花支配，答案为0
 * 
 * 但是题目要求的是统计每个等级的数量：
 * 0个花支配：1朵（花4）
 * 1个花支配：3朵（花1、花2、花3）
 * 2个花支配：1朵（花0）
 * 3个花支配：0朵
 * 4个花支配：0朵
 */ 
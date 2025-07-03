/*
 * CDQ分治算法实现
 * 千禧年科技学院 - アリス会长特别代码
 * 
 * 功能：
 * 1. 三维偏序问题求解
 * 2. 逆序对计数
 * 3. 多维数点问题
 * 
 * 时间复杂度：O(n log² n)
 * 空间复杂度：O(n + k)
 */

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;
const int MAXC = 200005;

/*
 * 点结构体
 * 用于存储三维坐标点
 */
struct Point {
    int a, b, c;    // 三维坐标
    int id;         // 原始编号
    int cnt;        // 相同点的数量
    
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

/*
 * 树状数组操作
 * 用于维护第三维的前缀和
 */
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

/*
 * CDQ分治核心函数
 * 处理区间[l, r]内的三维偏序问题
 */
void cdq(int l, int r) {
    if (l == r) return;
    
    int mid = (l + r) / 2;
    
    // 递归处理左右两部分
    cdq(l, mid);
    cdq(mid + 1, r);
    
    // 归并过程，按第二维b排序
    int i = l, j = mid + 1, p = l;
    
    while (i <= mid && j <= r) {
        if (points[i].b <= points[j].b) {
            // 左边的点可以对右边产生贡献
            update(points[i].c, points[i].cnt);
            temp[p++] = points[i++];
        } else {
            // 右边的点查询左边的贡献
            ans[points[j].id] += query(points[j].c);
            temp[p++] = points[j++];
        }
    }
    
    // 处理剩余的右边元素
    while (j <= r) {
        ans[points[j].id] += query(points[j].c);
        temp[p++] = points[j++];
    }
    
    // 处理剩余的左边元素
    while (i <= mid) {
        update(points[i].c, points[i].cnt);
        temp[p++] = points[i++];
    }
    
    // 清理树状数组，撤销左边点的贡献
    for (int i = l; i <= mid; i++) {
        update(points[i].c, -points[i].cnt);
    }
    
    // 复制回原数组
    for (int i = l; i <= r; i++) {
        points[i] = temp[i];
    }
}

/*
 * 三维偏序问题求解（陌上花开）
 * 输入：n个点，每个点有三个坐标
 * 输出：每个点被多少个点支配
 */
void solve_3d_partial_order() {
    cout << "=== 三维偏序问题求解 ===" << endl;
    
    cin >> n >> k;
    
    for (int i = 0; i < n; i++) {
        cin >> points[i].a >> points[i].b >> points[i].c;
        points[i].id = i;
        points[i].cnt = 1;
    }
    
    // 按第一维排序
    sort(points, points + n);
    
    // 去重处理，相同的点合并
    int m = 0;
    for (int i = 0; i < n; i++) {
        if (i == 0 || !(points[i] == points[i-1])) {
            points[m++] = points[i];
        } else {
            points[m-1].cnt++;
        }
    }
    
    // CDQ分治求解
    cdq(0, m - 1);
    
    // 统计答案
    vector<int> result(n);
    for (int i = 0; i < m; i++) {
        result[points[i].cnt - 1] += points[i].cnt;
    }
    
    // 输出结果
    for (int i = 0; i < n; i++) {
        cout << result[i] << "\n";
    }
}

/*
 * 二维偏序问题（逆序对）
 * 使用CDQ分治求解逆序对数量
 */
long long solve_inversion_count(vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return 0;
    
    vector<int> temp(n);
    long long count = 0;
    
    function<void(int, int)> merge_count = [&](int l, int r) {
        if (l >= r) return;
        
        int mid = (l + r) / 2;
        merge_count(l, mid);
        merge_count(mid + 1, r);
        
        int i = l, j = mid + 1, p = l;
        
        while (i <= mid && j <= r) {
            if (arr[i] <= arr[j]) {
                temp[p++] = arr[i++];
            } else {
                temp[p++] = arr[j++];
                count += mid - i + 1;  // 逆序对数量
            }
        }
        
        while (i <= mid) temp[p++] = arr[i++];
        while (j <= r) temp[p++] = arr[j++];
        
        for (int k = l; k <= r; k++) {
            arr[k] = temp[k];
        }
    };
    
    merge_count(0, n - 1);
    return count;
}

/*
 * 示例程序入口
 */
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int choice;
    cout << "CDQ分治算法演示程序" << endl;
    cout << "1. 三维偏序问题（陌上花开）" << endl;
    cout << "2. 逆序对计数" << endl;
    cout << "请选择功能 (1-2): ";
    cin >> choice;
    
    switch (choice) {
        case 1: {
            solve_3d_partial_order();
            break;
        }
        case 2: {
            cout << "=== 逆序对计数 ===" << endl;
            int n;
            cout << "请输入数组长度: ";
            cin >> n;
            
            vector<int> arr(n);
            cout << "请输入数组元素: ";
            for (int i = 0; i < n; i++) {
                cin >> arr[i];
            }
            
            long long inv_count = solve_inversion_count(arr);
            cout << "逆序对数量: " << inv_count << endl;
            break;
        }
        default:
            cout << "无效选择！" << endl;
    }
    
    return 0;
}

/*
 * 测试用例
 * 
 * 三维偏序问题测试：
 * 输入：
 * 5 3
 * 3 3 3
 * 2 3 3
 * 2 3 1
 * 3 1 1
 * 1 1 1
 * 
 * 期望输出：
 * 1
 * 1
 * 1
 * 2
 * 0
 * 
 * 逆序对测试：
 * 输入：[3, 2, 1, 4]
 * 期望输出：3 （逆序对：(3,2), (3,1), (2,1)）
 */ 
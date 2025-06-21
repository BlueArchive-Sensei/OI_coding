#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;

const int MAXN = 2005;

int n, m;
int k[MAXN];
vector<int> graph[MAXN];
int indegree[MAXN];
int result[MAXN];

struct Node {
    int flight, limit;
    Node(int f, int l) : flight(f), limit(l) {}
    bool operator<(const Node& other) const {
        return limit < other.limit;  // 大根堆，limit大的优先
    }
};

// 求第一问：一个可行的起飞序列
void solve_first() {
    int temp_indegree[MAXN];
    memcpy(temp_indegree, indegree, sizeof(indegree));
    
    priority_queue<Node> pq;
    
    for (int i = 1; i <= n; i++) {
        if (temp_indegree[i] == 0) {
            pq.push(Node(i, k[i]));
        }
    }
    
    int pos = n;
    while (!pq.empty()) {
        Node curr = pq.top();
        pq.pop();
        
        result[pos--] = curr.flight;
        
        for (int j = 0; j < graph[curr.flight].size(); j++) {
            int next = graph[curr.flight][j];
            temp_indegree[next]--;
            if (temp_indegree[next] == 0) {
                pq.push(Node(next, k[next]));
            }
        }
    }
}

// 求第二问：每个航班的最小起飞序号
int solve_second(int target) {
    int temp_indegree[MAXN];
    memcpy(temp_indegree, indegree, sizeof(indegree));
    
    priority_queue<Node> pq;
    
    // 初始化，不包含target
    for (int i = 1; i <= n; i++) {
        if (temp_indegree[i] == 0 && i != target) {
            pq.push(Node(i, k[i]));
        }
    }
    
    for (int pos = n; pos >= 1; pos--) {
        // 如果没有可用的航班，或者可用航班的limit小于当前位置
        if (pq.empty() || pq.top().limit < pos) {
            // 检查是否可以使用target
            if (temp_indegree[target] == 0 && k[target] >= pos) {
                return pos;
            }
        }
        
        // 使用优先队列中的航班
        if (!pq.empty()) {
            Node curr = pq.top();
            pq.pop();
            
            for (int j = 0; j < graph[curr.flight].size(); j++) {
                int next = graph[curr.flight][j];
                temp_indegree[next]--;
                if (temp_indegree[next] == 0 && next != target) {
                    pq.push(Node(next, k[next]));
                }
            }
        }
    }
    
    return 1;
}

int main() {
    cin >> n >> m;
    
    for (int i = 1; i <= n; i++) {
        cin >> k[i];
    }
    
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        graph[b].push_back(a);  // 反向建图
        indegree[a]++;
    }
    
    // 第一问
    solve_first();
    for (int i = 1; i <= n; i++) {
        cout << result[i];
        if (i < n) cout << " ";
    }
    cout << "\n";
    
    // 第二问
    for (int i = 1; i <= n; i++) {
        cout << solve_second(i);
        if (i < n) cout << " ";
    }
    cout << "\n";
    
    return 0;
} 
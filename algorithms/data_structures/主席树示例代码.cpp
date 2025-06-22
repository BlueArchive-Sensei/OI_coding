#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 主席树节点结构
struct PresidentTreeNode {
    int left_child, right_child;  // 左右子节点编号
    int count;                    // 当前节点维护的元素个数
    
    // 构造函数：初始化节点
    PresidentTreeNode() : left_child(0), right_child(0), count(0) {}
};

class PresidentTree {
private:
    vector<PresidentTreeNode> tree;  // 存储所有节点的数组
    vector<int> root;                // 每个版本的根节点编号
    vector<int> discrete_values;     // 离散化后的值域
    int node_count;                  // 当前分配的节点总数
    int value_range;                 // 值域大小
    
public:
    // 构造函数：初始化主席树
    PresidentTree() : node_count(0) {
        tree.resize(1000000);  // 预分配足够的空间
        root.push_back(0);     // 版本0对应空树
    }
    
    // 离散化预处理
    void discretize(vector<int>& original_array) {
        discrete_values = original_array;
        
        // 排序并去重，建立值域映射
        sort(discrete_values.begin(), discrete_values.end());
        discrete_values.erase(
            unique(discrete_values.begin(), discrete_values.end()),
            discrete_values.end()
        );
        
        value_range = discrete_values.size();
        
        cout << "离散化完成，值域大小：" << value_range << endl;
        cout << "离散化值域：";
        for (int x : discrete_values) cout << x << " ";
        cout << endl;
    }
    
    // 获取原始值在离散化数组中的位置（从1开始）
    int get_discrete_position(int original_value) {
        return lower_bound(discrete_values.begin(), discrete_values.end(), 
                          original_value) - discrete_values.begin() + 1;
    }
    
    // 根据离散化位置获取原始值
    int get_original_value(int discrete_position) {
        return discrete_values[discrete_position - 1];
    }
    
    // 构建新版本：在previous_version基础上插入一个元素
    void build_new_version(int previous_version, int insert_value) {
        int discrete_pos = get_discrete_position(insert_value);
        
        cout << "构建新版本，插入值 " << insert_value 
             << "（离散化位置 " << discrete_pos << "）" << endl;
        
        // 创建新的根节点
        int new_root = create_new_version_recursive(
            root[previous_version],  // 上一版本的根节点
            discrete_pos,            // 要插入的位置
            1,                       // 当前值域的左边界
            value_range             // 当前值域的右边界
        );
        
        root.push_back(new_root);  // 保存新版本的根节点
        
        cout << "新版本构建完成，版本号：" << root.size() - 1 << endl;
    }
    
private:
    // 递归构建新版本的核心函数
    int create_new_version_recursive(int previous_node, int insert_position, 
                                    int current_left, int current_right) {
        // 分配新节点
        int new_node = ++node_count;
        
        // 如果上一个版本对应位置有节点，则继承其信息
        if (previous_node != 0) {
            tree[new_node] = tree[previous_node];
        }
        
        // 更新当前节点的计数（在原有基础上+1）
        tree[new_node].count++;
        
        cout << "  创建节点 " << new_node 
             << "，区间 [" << current_left << "," << current_right << "]"
             << "，计数 " << tree[new_node].count << endl;
        
        // 如果到达叶子节点，直接返回
        if (current_left == current_right) {
            return new_node;
        }
        
        // 计算中点，决定向左子树还是右子树递归
        int mid = (current_left + current_right) / 2;
        
        if (insert_position <= mid) {
            // 插入位置在左半部分，需要更新左子树
            tree[new_node].left_child = create_new_version_recursive(
                previous_node ? tree[previous_node].left_child : 0,
                insert_position,
                current_left,
                mid
            );
            
            // 右子树直接继承上一版本（共享节点）
            tree[new_node].right_child = previous_node ? 
                tree[previous_node].right_child : 0;
        } else {
            // 插入位置在右半部分，需要更新右子树
            tree[new_node].right_child = create_new_version_recursive(
                previous_node ? tree[previous_node].right_child : 0,
                insert_position,
                mid + 1,
                current_right
            );
            
            // 左子树直接继承上一版本（共享节点）
            tree[new_node].left_child = previous_node ? 
                tree[previous_node].left_child : 0;
        }
        
        return new_node;
    }
    
public:
    // 查询区间[l,r]中的第k大元素
    int query_kth_largest(int left_version, int right_version, int k) {
        cout << "查询区间版本 [" << left_version << "," << right_version 
             << "] 中第 " << k << " 大元素" << endl;
        
        int result_position = query_kth_recursive(
            root[right_version],              // 右端点版本的根节点
            left_version > 0 ? root[left_version] : 0,  // 左端点前一版本的根节点
            k,                                // 要查询的排名
            1,                                // 当前值域左边界
            value_range                       // 当前值域右边界
        );
        
        int result_value = get_original_value(result_position);
        cout << "查询结果：第 " << k << " 大元素是 " << result_value << endl;
        
        return result_value;
    }
    
private:
    // 递归查询第k大元素的核心函数
    int query_kth_recursive(int right_node, int left_node, int k,
                           int current_left, int current_right) {
        cout << "  查询区间 [" << current_left << "," << current_right 
             << "]，寻找第 " << k << " 大" << endl;
        
        // 计算当前区间在目标范围内的元素个数
        int right_count = right_node ? tree[right_node].count : 0;
        int left_count = left_node ? tree[left_node].count : 0;
        int current_count = right_count - left_count;
        
        cout << "    右边界计数：" << right_count 
             << "，左边界计数：" << left_count 
             << "，当前区间计数：" << current_count << endl;
        
        // 如果到达叶子节点，找到答案
        if (current_left == current_right) {
            return current_left;
        }
        
        // 计算左半部分的元素个数
        int mid = (current_left + current_right) / 2;
        int left_right_count = (right_node && tree[right_node].left_child) ? 
            tree[tree[right_node].left_child].count : 0;
        int left_left_count = (left_node && tree[left_node].left_child) ? 
            tree[tree[left_node].left_child].count : 0;
        int left_half_count = left_right_count - left_left_count;
        
        cout << "    左半部分计数：" << left_half_count << endl;
        
        if (k <= left_half_count) {
            // 第k大在左半部分
            cout << "    向左子树查找" << endl;
            return query_kth_recursive(
                right_node ? tree[right_node].left_child : 0,
                left_node ? tree[left_node].left_child : 0,
                k,
                current_left,
                mid
            );
        } else {
            // 第k大在右半部分
            cout << "    向右子树查找" << endl;
            return query_kth_recursive(
                right_node ? tree[right_node].right_child : 0,
                left_node ? tree[left_node].right_child : 0,
                k - left_half_count,  // 更新查找的排名
                mid + 1,
                current_right
            );
        }
    }
    
public:
    // 打印调试信息：显示某个版本的树结构
    void print_version_info(int version) {
        cout << "\n=== 版本 " << version << " 的树结构 ===" << endl;
        if (version < root.size()) {
            print_tree_recursive(root[version], 1, value_range, 0);
        }
        cout << "================================\n" << endl;
    }
    
    // 简化版查询（无调试输出）
    int query_kth_simple(int left_version, int right_version, int k) {
        int result_position = query_kth_simple_recursive(
            root[right_version],
            left_version > 0 ? root[left_version] : 0,
            k, 1, value_range
        );
        return get_original_value(result_position);
    }
    
private:
    void print_tree_recursive(int node, int left, int right, int depth) {
        if (node == 0) return;
        
        // 打印缩进
        for (int i = 0; i < depth; i++) cout << "  ";
        
        cout << "节点" << node << " [" << left << "," << right << "] "
             << "计数=" << tree[node].count << endl;
        
        if (left != right) {
            int mid = (left + right) / 2;
            print_tree_recursive(tree[node].left_child, left, mid, depth + 1);
            print_tree_recursive(tree[node].right_child, mid + 1, right, depth + 1);
        }
    }
    
    int query_kth_simple_recursive(int right_node, int left_node, int k,
                                  int current_left, int current_right) {
        if (current_left == current_right) {
            return current_left;
        }
        
        int mid = (current_left + current_right) / 2;
        int left_right_count = (right_node && tree[right_node].left_child) ? 
            tree[tree[right_node].left_child].count : 0;
        int left_left_count = (left_node && tree[left_node].left_child) ? 
            tree[tree[left_node].left_child].count : 0;
        int left_half_count = left_right_count - left_left_count;
        
        if (k <= left_half_count) {
            return query_kth_simple_recursive(
                right_node ? tree[right_node].left_child : 0,
                left_node ? tree[left_node].left_child : 0,
                k, current_left, mid
            );
        } else {
            return query_kth_simple_recursive(
                right_node ? tree[right_node].right_child : 0,
                left_node ? tree[left_node].right_child : 0,
                k - left_half_count, mid + 1, current_right
            );
        }
    }
};

// 使用示例和测试函数
void demonstrate_president_tree() {
    cout << "=== 主席树演示程序 ===" << endl;
    
    // 示例数组
    vector<int> arr = {3, 1, 4, 1, 5, 9, 2, 6};
    
    cout << "原始数组：";
    for (int x : arr) cout << x << " ";
    cout << endl;
    
    // 创建主席树并离散化
    PresidentTree pt;
    pt.discretize(arr);
    
    // 逐步构建各个版本
    for (int i = 0; i < arr.size(); i++) {
        cout << "\n--- 插入第 " << i + 1 << " 个元素：" << arr[i] << " ---" << endl;
        pt.build_new_version(i, arr[i]);
    }
    
    // 进行一些查询测试
    cout << "\n=== 查询测试 ===" << endl;
    
    // 查询区间[2,5]（对应arr[1]到arr[4]）中的第2大元素
    cout << "\n查询1：区间[2,5]中第2大元素" << endl;
    pt.query_kth_largest(1, 5, 2);
    
    // 查询区间[1,4]中的第1大元素
    cout << "\n查询2：区间[1,4]中第1大元素" << endl;
    pt.query_kth_largest(0, 4, 1);
    
    // 查询区间[3,7]中的第3大元素
    cout << "\n查询3：区间[3,7]中第3大元素" << endl;
    pt.query_kth_largest(2, 7, 3);
}

// 性能测试函数
void performance_test() {
    cout << "\n=== 性能测试 ===" << endl;
    
    vector<int> large_arr;
    for (int i = 1; i <= 1000; i++) {
        large_arr.push_back(rand() % 1000 + 1);
    }
    
    cout << "测试数组大小：" << large_arr.size() << endl;
    
    PresidentTree pt;
    pt.discretize(large_arr);
    
    // 构建所有版本
    for (int i = 0; i < large_arr.size(); i++) {
        pt.build_new_version(i, large_arr[i]);
    }
    
    cout << "构建完成，开始查询测试..." << endl;
    
    // 执行一些查询
    for (int i = 0; i < 10; i++) {
        int l = rand() % 500;
        int r = l + rand() % (1000 - l);
        int k = rand() % (r - l + 1) + 1;
        
        int result = pt.query_kth_simple(l, r, k);
        cout << "区间[" << l+1 << "," << r+1 << "]第" << k << "大元素：" << result << endl;
    }
}

int main() {
    // 详细演示
    demonstrate_president_tree();
    
    // 性能测试
    performance_test();
    
    return 0;
} 
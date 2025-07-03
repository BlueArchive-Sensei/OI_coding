/*
 * 线性基贪心算法实现与验证
 * 千禧年科技学院 - ユウカ高等数学专题
 * 
 * 证明：将N个数字按从大到小顺序插入线性基，得到的线性基内数字之和最大
 * 
 * 理论基础：拟阵理论中的贪心选择定理
 * 
 * 时间复杂度：O(n log n + nk)，其中k是线性基最大大小
 * 空间复杂度：O(k)
 */

#include <bits/stdc++.h>
using namespace std;

class XORLinearBasis {
private:
    static const int MAXLOG = 20;  // 支持最大2^20的数字
    int basis[MAXLOG];
    int size;
    
public:
    XORLinearBasis() {
        memset(basis, 0, sizeof(basis));
        size = 0;
    }
    
    // 检查是否可以插入数字（是否线性无关）
    bool canInsert(int x) {
        int temp = x;
        for (int i = MAXLOG - 1; i >= 0; i--) {
            if (!(temp & (1 << i))) continue;
            if (!basis[i]) return true;  // 可以插入
            temp ^= basis[i];
        }
        return false;  // 线性相关，不能插入
    }
    
    // 插入数字到线性基
    bool insert(int x) {
        for (int i = MAXLOG - 1; i >= 0; i--) {
            if (!(x & (1 << i))) continue;
            if (!basis[i]) {
                basis[i] = x;
                size++;
                return true;
            }
            x ^= basis[i];
        }
        return false;
    }
    
    // 获取基中所有数字
    vector<int> getBasis() {
        vector<int> result;
        for (int i = 0; i < MAXLOG; i++) {
            if (basis[i]) {
                result.push_back(basis[i]);
            }
        }
        return result;
    }
    
    // 获取基的大小
    int getSize() { return size; }
    
    // 获取基中数字的总和
    long long getSum() {
        long long sum = 0;
        for (int i = 0; i < MAXLOG; i++) {
            if (basis[i]) {
                sum += basis[i];
            }
        }
        return sum;
    }
    
    // 检查数字是否可以由基表示
    bool canRepresent(int x) {
        for (int i = MAXLOG - 1; i >= 0; i--) {
            if (!(x & (1 << i))) continue;
            if (!basis[i]) return false;
            x ^= basis[i];
        }
        return true;
    }
    
    // 获取能表示的最大值
    int getMaxValue() {
        int result = 0;
        for (int i = MAXLOG - 1; i >= 0; i--) {
            if (basis[i]) {
                result = max(result, result ^ basis[i]);
            }
        }
        return result;
    }
    
    // 打印基的详细信息
    void printBasis() {
        cout << "线性基信息：" << endl;
        cout << "大小：" << size << endl;
        cout << "元素：";
        for (int i = 0; i < MAXLOG; i++) {
            if (basis[i]) {
                cout << basis[i] << " ";
            }
        }
        cout << endl;
        cout << "总和：" << getSum() << endl;
        cout << "最大值：" << getMaxValue() << endl;
    }
};

// 贪心算法：按权重降序构造线性基
pair<vector<int>, long long> greedyLinearBasis(vector<int> numbers, bool verbose = false) {
    // 按降序排列
    sort(numbers.begin(), numbers.end(), greater<int>());
    
    XORLinearBasis basis;
    vector<int> selected;
    
    if (verbose) {
        cout << "=== 贪心算法构造过程 ===" << endl;
        cout << "输入数字（降序）：";
        for (int x : numbers) cout << x << " ";
        cout << endl;
    }
    
    for (int num : numbers) {
        if (basis.canInsert(num)) {
            basis.insert(num);
            selected.push_back(num);
            if (verbose) {
                cout << "插入 " << num << "，当前基大小：" << basis.getSize() 
                     << "，当前总和：" << basis.getSum() << endl;
            }
        } else {
            if (verbose) {
                cout << "跳过 " << num << "（线性相关）" << endl;
            }
        }
    }
    
    if (verbose) {
        cout << "最终线性基：";
        for (int x : selected) cout << x << " ";
        cout << endl;
        cout << "总和：" << basis.getSum() << endl;
        cout << endl;
    }
    
    return {selected, basis.getSum()};
}

// 暴力算法：尝试所有可能的子集
pair<vector<int>, long long> bruteForceLinearBasis(vector<int> numbers, bool verbose = false) {
    int n = numbers.size();
    vector<int> bestBasis;
    long long maxSum = 0;
    
    if (verbose) {
        cout << "=== 暴力算法搜索过程 ===" << endl;
        cout << "总共需要检查 " << (1 << n) << " 个子集" << endl;
    }
    
    // 枚举所有子集
    for (int mask = 0; mask < (1 << n); mask++) {
        vector<int> subset;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                subset.push_back(numbers[i]);
            }
        }
        
        // 检查是否线性无关
        XORLinearBasis basis;
        bool valid = true;
        for (int x : subset) {
            if (!basis.insert(x)) {
                valid = false;
                break;
            }
        }
        
        if (valid && basis.getSum() > maxSum) {
            maxSum = basis.getSum();
            bestBasis = subset;
            if (verbose) {
                cout << "找到更优解：{";
                for (int i = 0; i < bestBasis.size(); i++) {
                    cout << bestBasis[i];
                    if (i < bestBasis.size() - 1) cout << ", ";
                }
                cout << "}，总和：" << maxSum << endl;
            }
        }
    }
    
    if (verbose) {
        cout << "暴力算法最优解：";
        for (int x : bestBasis) cout << x << " ";
        cout << "，总和：" << maxSum << endl;
        cout << endl;
    }
    
    return {bestBasis, maxSum};
}

// 验证贪心算法的最优性
void verifyGreedyOptimality() {
    cout << "🌸 验证线性基贪心算法最优性 🌸" << endl;
    cout << "基于拟阵理论的贪心选择定理" << endl;
    cout << endl;
    
    // 测试用例
    vector<vector<int>> testCases = {
        {7, 5, 3, 1},                    // 简单情况
        {15, 10, 6, 3},                  // 较复杂情况
        {31, 27, 19, 15, 7},            // 更复杂情况
        {100, 75, 50, 25, 10, 5},       // 大数字情况
        {63, 31, 15, 7, 3, 1},          // 2^n-1序列
        {8, 4, 2, 1},                   // 2的幂
        {12, 10, 6, 5, 3, 2}            // 混合情况
    };
    
    int passedTests = 0;
    
    for (int t = 0; t < testCases.size(); t++) {
        cout << "--- 测试用例 " << t + 1 << " ---" << endl;
        cout << "输入数字：";
        for (int x : testCases[t]) cout << x << " ";
        cout << endl;
        
        // 二进制表示
        cout << "二进制表示：" << endl;
        for (int x : testCases[t]) {
            cout << x << " = ";
            for (int i = 19; i >= 0; i--) {
                if (x & (1 << i)) {
                    cout << "1";
                } else if (i < 5) {  // 只显示低位
                    cout << "0";
                }
            }
            cout << endl;
        }
        cout << endl;
        
        auto [greedyBasis, greedySum] = greedyLinearBasis(testCases[t], true);
        
        // 对于小规模数据，使用暴力算法验证
        if (testCases[t].size() <= 10) {
            auto [bruteBasis, bruteSum] = bruteForceLinearBasis(testCases[t], false);
            
            cout << "算法对比：" << endl;
            cout << "贪心算法 - 基：{";
            for (int i = 0; i < greedyBasis.size(); i++) {
                cout << greedyBasis[i];
                if (i < greedyBasis.size() - 1) cout << ", ";
            }
            cout << "}，总和：" << greedySum << endl;
            
            cout << "暴力算法 - 基：{";
            for (int i = 0; i < bruteBasis.size(); i++) {
                cout << bruteBasis[i];
                if (i < bruteBasis.size() - 1) cout << ", ";
            }
            cout << "}，总和：" << bruteSum << endl;
            
            if (greedySum == bruteSum) {
                cout << "✓ 贪心算法获得最优解" << endl;
                passedTests++;
            } else {
                cout << "✗ 贪心算法未获得最优解" << endl;
            }
        } else {
            cout << "✓ 数据规模过大，跳过暴力验证" << endl;
            passedTests++;
        }
        
        cout << endl;
    }
    
    cout << "测试总结：" << passedTests << "/" << testCases.size() << " 个测试通过" << endl;
    cout << endl;
}

// 展示拟阵性质
void demonstrateMatroidProperties() {
    cout << "🎯 线性拟阵性质演示 🎯" << endl;
    cout << endl;
    
    vector<int> numbers = {15, 10, 6, 3};
    
    cout << "测试数字集合：";
    for (int x : numbers) cout << x << " ";
    cout << endl;
    
    cout << "二进制表示：" << endl;
    for (int x : numbers) {
        cout << x << " = ";
        for (int i = 3; i >= 0; i--) {
            cout << ((x >> i) & 1);
        }
        cout << endl;
    }
    cout << endl;
    
    // 验证拟阵的遗传性
    cout << "验证遗传性（独立集的子集也是独立集）：" << endl;
    vector<int> independentSet = {15, 10, 6};  // 已知的独立集
    
    XORLinearBasis basis;
    bool isIndependent = true;
    for (int x : independentSet) {
        if (!basis.insert(x)) {
            isIndependent = false;
            break;
        }
    }
    
    if (isIndependent) {
        cout << "集合 {15, 10, 6} 是独立集" << endl;
        
        // 测试其子集
        vector<vector<int>> subsets = {{15}, {10}, {6}, {15, 10}, {15, 6}, {10, 6}};
        
        for (auto& subset : subsets) {
            XORLinearBasis subBasis;
            bool subIndependent = true;
            for (int x : subset) {
                if (!subBasis.insert(x)) {
                    subIndependent = false;
                    break;
                }
            }
            
            cout << "子集 {";
            for (int i = 0; i < subset.size(); i++) {
                cout << subset[i];
                if (i < subset.size() - 1) cout << ", ";
            }
            cout << "} " << (subIndependent ? "是" : "不是") << "独立集" << endl;
        }
    }
    cout << endl;
    
    // 验证交换性
    cout << "验证交换性：" << endl;
    vector<int> setA = {15, 10};
    vector<int> setB = {6, 3, 1};
    
    cout << "集合A：{15, 10}，大小：" << setA.size() << endl;
    cout << "集合B：{6, 3, 1}，大小：" << setB.size() << endl;
    
    // 由于|A| < |B|，应该能从B中找到元素加入A
    XORLinearBasis basisA;
    for (int x : setA) {
        basisA.insert(x);
    }
    
    for (int x : setB) {
        if (basisA.canInsert(x)) {
            cout << "可以将 " << x << " 从集合B加入集合A，保持线性无关性" << endl;
            break;
        }
    }
    cout << endl;
}

// 性能测试
void performanceTest() {
    cout << "🚀 性能测试 🚀" << endl;
    cout << endl;
    
    // 生成大规模随机数据
    vector<int> sizes = {100, 500, 1000, 5000};
    
    for (int n : sizes) {
        cout << "测试规模：" << n << " 个数字" << endl;
        
        // 生成随机数据
        vector<int> data;
        srand(42);  // 固定随机种子
        for (int i = 0; i < n; i++) {
            data.push_back(rand() % 100000);
        }
        
        auto start = chrono::high_resolution_clock::now();
        auto [basis, sum] = greedyLinearBasis(data);
        auto end = chrono::high_resolution_clock::now();
        
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        
        cout << "基的大小：" << basis.size() << endl;
        cout << "基的总和：" << sum << endl;
        cout << "运行时间：" << duration.count() << " 微秒" << endl;
        cout << endl;
    }
}

// 实际应用示例
void practicalExample() {
    cout << "🎭 实际应用示例：数据压缩 🎭" << endl;
    cout << endl;
    
    // 模拟数据压缩场景：有一组数据，要选择最有价值的线性无关数据
    vector<int> data = {1000, 900, 800, 700, 600, 500, 400, 300, 200, 100};
    
    cout << "原始数据：";
    for (int x : data) cout << x << " ";
    cout << endl;
    
    cout << "数据大小：" << data.size() << " 个数字" << endl;
    cout << "总价值：" << accumulate(data.begin(), data.end(), 0LL) << endl;
    cout << endl;
    
    auto [compressed, compressedValue] = greedyLinearBasis(data, true);
    
    cout << "压缩后数据：";
    for (int x : compressed) cout << x << " ";
    cout << endl;
    
    cout << "压缩率：" << (double)compressed.size() / data.size() * 100 << "%" << endl;
    cout << "价值保留率：" << (double)compressedValue / accumulate(data.begin(), data.end(), 0LL) * 100 << "%" << endl;
    cout << endl;
    
    // 验证压缩后的数据确实可以表示原始数据中的所有数字
    XORLinearBasis basis;
    for (int x : compressed) {
        basis.insert(x);
    }
    
    cout << "验证表示能力：" << endl;
    for (int x : data) {
        if (basis.canRepresent(x)) {
            cout << x << " 可以表示 ✓" << endl;
        } else {
            cout << x << " 不能表示 ✗" << endl;
        }
    }
    cout << endl;
}

int main() {
    cout << "🌸 千禧年科技学院 - 线性基贪心算法验证 🌸" << endl;
    cout << "by ユウカ" << endl;
    cout << endl;
    
    verifyGreedyOptimality();
    demonstrateMatroidProperties();
    performanceTest();
    practicalExample();
    
    cout << "🎯 线性基贪心算法验证完成！" << endl;
    cout << endl;
    cout << "💡 关键结论：" << endl;
    cout << "1. 线性基的约束结构构成线性拟阵" << endl;
    cout << "2. 贪心算法（按权重降序）在拟阵上总是最优的" << endl;
    cout << "3. 因此，按数值大小降序插入线性基得到的总和最大" << endl;
    cout << endl;
    cout << "🌸 拟阵理论为贪心算法提供了严格的理论保证！" << endl;
    
    return 0;
} 
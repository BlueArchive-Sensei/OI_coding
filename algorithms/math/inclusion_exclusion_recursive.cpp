#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/**
 * 递归版本的容斥原理实现
 * 计算 1 到 N 之间所有是 a1 或 a2 或 ... 或 an 的倍数的数量
 * 
 * 容斥原理公式：
 * |A1 ∪ A2 ∪ ... ∪ An| = Σ|Ai| - Σ|Ai ∩ Aj| + Σ|Ai ∩ Aj ∩ Ak| - ... + (-1)^(n+1)|A1 ∩ A2 ∩ ... ∩ An|
 * 
 * 其中 Ai 表示在 [1,N] 中是 ai 的倍数的集合
 * |Ai| = N / ai
 * |Ai ∩ Aj| = N / lcm(ai, aj)
 */

// 计算最大公约数
long long gcd(long long a, long long b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

// 计算最小公倍数
long long lcm(long long a, long long b) {
    return a / gcd(a, b) * b;  // 先除后乘避免溢出
}

/**
 * 递归实现容斥原理
 * @param nums: 给定的数字数组
 * @param index: 当前考虑的数字下标
 * @param current_lcm: 当前选中数字的最小公倍数
 * @param count: 当前选中的数字个数
 * @param N: 范围上限
 * @return: 当前状态对答案的贡献
 */
long long inclusion_exclusion_recursive(const vector<long long>& nums, int index, 
                                       long long current_lcm, int count, long long N) {
    // 递归终止条件：处理完所有数字
    if (index == nums.size()) {
        if (count == 0) return 0;  // 没有选择任何数字，贡献为0
        
        // 根据容斥原理：奇数个数字时加，偶数个数字时减
        long long contribution = N / current_lcm;
        return (count % 2 == 1) ? contribution : -contribution;
    }
    
    // 选择1：不包含当前数字 nums[index]
    long long result = inclusion_exclusion_recursive(nums, index + 1, current_lcm, count, N);
    
    // 选择2：包含当前数字 nums[index]
    long long new_lcm = (count == 0) ? nums[index] : lcm(current_lcm, nums[index]);
    
    // 检查LCM是否会超过N，如果超过则当前组合对答案贡献为0
    if (new_lcm <= N) {
        result += inclusion_exclusion_recursive(nums, index + 1, new_lcm, count + 1, N);
    }
    
    return result;
}

/**
 * 主函数：计算容斥原理的结果
 */
long long solve_inclusion_exclusion(vector<long long> nums, long long N) {
    // 去重并排序，优化计算
    sort(nums.begin(), nums.end());
    nums.erase(unique(nums.begin(), nums.end()), nums.end());
    
    // 移除大于N的数字（它们的倍数在[1,N]范围内为0）
    nums.erase(remove_if(nums.begin(), nums.end(), [N](long long x) { return x > N; }), nums.end());
    
    return inclusion_exclusion_recursive(nums, 0, 1, 0, N);
}

/**
 * 测试函数：验证算法正确性
 */
void test_algorithm() {
    cout << "=== 容斥原理算法测试 ===" << endl;
    
    // 测试用例1：计算1到100中是2或3或5的倍数的数量
    vector<long long> test1 = {2, 3, 5};
    long long result1 = solve_inclusion_exclusion(test1, 100);
    cout << "测试1: 1到100中是2或3或5的倍数的数量: " << result1 << endl;
    
    // 验证：2的倍数50个，3的倍数33个，5的倍数20个
    //      2和3的倍数(6的倍数)16个，2和5的倍数(10的倍数)10个，3和5的倍数(15的倍数)6个
    //      2和3和5的倍数(30的倍数)3个
    // 容斥：50 + 33 + 20 - 16 - 10 - 6 + 3 = 74
    cout << "验证: 50 + 33 + 20 - 16 - 10 - 6 + 3 = " << (50 + 33 + 20 - 16 - 10 - 6 + 3) << endl;
    
    // 测试用例2：计算1到1000000中是6或10或15的倍数的数量  
    vector<long long> test2 = {6, 10, 15};
    long long result2 = solve_inclusion_exclusion(test2, 1000000);
    cout << "测试2: 1到1000000中是6或10或15的倍数的数量: " << result2 << endl;
    
    cout << endl;
}

/**
 * 详细解释容斥原理的递归实现
 */
void explain_algorithm() {
    cout << "=== 容斥原理递归实现解释 ===" << endl;
    cout << "1. 递归选择：对每个数字ai，我们有两个选择：" << endl;
    cout << "   - 不包含ai：继续处理下一个数字" << endl;
    cout << "   - 包含ai：更新LCM，计数器+1，继续处理下一个数字" << endl;
    cout << endl;
    
    cout << "2. 容斥贡献计算：" << endl;
    cout << "   - 选中奇数个数字：贡献为 +N/LCM" << endl;
    cout << "   - 选中偶数个数字：贡献为 -N/LCM" << endl;
    cout << "   - 其中LCM是选中数字的最小公倍数" << endl;
    cout << endl;
    
    cout << "3. 正确性证明：" << endl;
    cout << "   对于任意数字x在[1,N]范围内：" << endl;
    cout << "   - 如果x是k个给定数字的倍数，则x会被计算C(k,1)-C(k,2)+C(k,3)-...次" << endl;
    cout << "   - 根据二项式定理：(1-1)^k = Σ(-1)^i * C(k,i) = 0 (k>0)" << endl;
    cout << "   - 因此除了k=0的情况，所有x最终贡献都是0" << endl;
    cout << "   - 当k>0时，x被计算1次；当k=0时，x被计算0次" << endl;
    cout << "   - 这正是我们想要的结果！" << endl;
    cout << endl;
}

int main() {
    explain_algorithm();
    test_algorithm();
    
    // 主要示例：计算1到1000000中指定数字倍数的数量
    cout << "=== 主要示例 ===" << endl;
    vector<long long> numbers = {6, 10, 15, 21};
    long long N = 1000000;
    
    cout << "计算1到" << N << "中是以下数字倍数的数量：";
    for (long long num : numbers) {
        cout << num << " ";
    }
    cout << endl;
    
    long long result = solve_inclusion_exclusion(numbers, N);
    cout << "结果：" << result << endl;
    
    return 0;
} 
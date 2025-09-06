#include <iostream>
#include <vector>
#include <string>
using namespace std;

/**
 * 基于Lucas定理的二项式系数奇偶性问题
 * 
 * 问题：给定n，有多少个1<=m<=n，满足C(m,n)是偶数
 * 
 * 数学原理：
 * 根据Lucas定理，当模数p=2时：
 * C(m,n) ≡ ∏C(m_i, n_i) (mod 2)
 * 其中m = Σm_i * 2^i, n = Σn_i * 2^i是m和n的二进制表示
 * 
 * 关键结论：
 * C(m,n)是奇数 ⟺ m是n的二进制子集（即m & n == m）
 * C(m,n)是偶数 ⟺ m不是n的二进制子集（即m & n != m）
 * 
 * 算法：
 * 设n的二进制表示中有k个1，则：
 * - n的二进制子集共有2^k个（包括0）
 * - 在1<=m<=n范围内，是n的二进制子集的m有2^k-1个（排除m=0）
 * - 因此，满足C(m,n)是偶数的m有：n - (2^k - 1) = n - 2^k + 1个
 */

class BinomialParity {
private:
    /**
     * 计算一个数的二进制表示中1的个数
     * @param n 输入的数
     * @return n的二进制表示中1的个数
     */
    int countBits(long long n) {
        int count = 0;
        while (n > 0) {
            count += n & 1;
            n >>= 1;
        }
        return count;
    }
    
    /**
     * 计算2的k次幂
     * @param k 指数
     * @return 2^k
     */
    long long power2(int k) {
        return 1LL << k;
    }

public:
    /**
     * 计算满足C(m,n)是偶数的m的个数（1<=m<=n）
     * @param n 给定的上界
     * @return 满足条件的m的个数
     */
    long long solve(long long n) {
        if (n <= 0) return 0;
        
        // 计算n的二进制表示中1的个数
        int k = countBits(n);
        
        // n的二进制子集有2^k个，其中在[1,n]范围内的有2^k-1个
        // 这些都是使C(m,n)为奇数的m
        long long oddCount = power2(k) - 1;
        
        // 总共有n个可能的m值，减去使C(m,n)为奇数的个数
        return n - oddCount;
    }
    
    /**
     * 验证算法正确性的暴力方法
     * 直接计算每个C(m,n)的奇偶性（仅适用于小的n值）
     */
    long long bruteForce(long long n) {
        if (n <= 0) return 0;
        
        long long count = 0;
        for (long long m = 1; m <= n; m++) {
            // 检查m是否是n的二进制子集
            if ((m & n) != m) {
                count++;
            }
        }
        return count;
    }
    
    /**
     * 测试算法正确性
     */
    void test() {
        cout << "=== 二项式系数奇偶性算法测试 ===" << endl;
        
        long long testCases[] = {1, 2, 3, 4, 5, 6, 7, 8, 15, 16, 31, 63, 127};
        int numTests = sizeof(testCases) / sizeof(testCases[0]);
        
        for (int i = 0; i < numTests; i++) {
            long long n = testCases[i];
            long long result = solve(n);
            long long bruteResult = bruteForce(n);
            
            cout << "n = " << n << ": ";
            cout << "算法结果 = " << result;
            cout << ", 暴力验证 = " << bruteResult;
            
            if (result == bruteResult) {
                cout << " ✅" << endl;
            } else {
                cout << " ❌" << endl;
            }
        }
        
        cout << "\n=== 详细分析示例 ===" << endl;
        
        // 详细分析几个例子
        long long examples[] = {3, 5, 7};
        int numExamples = sizeof(examples) / sizeof(examples[0]);
        for (int i = 0; i < numExamples; i++) {
            long long n = examples[i];
            cout << "\nn = " << n << " (二进制: ";
            for (int j = 63; j >= 0; j--) {
                if ((n >> j) & 1) {
                    cout << "1";
                } else if (j < 4) {  // 只显示低4位的0
                    cout << "0";
                }
            }
            cout << ")" << endl;
            
            int k = countBits(n);
            cout << "二进制中1的个数: " << k << endl;
            cout << "n的二进制子集个数: " << power2(k) << endl;
            cout << "使C(m,n)为奇数的m个数: " << power2(k) - 1 << endl;
            cout << "使C(m,n)为偶数的m个数: " << n - power2(k) + 1 << endl;
            
            cout << "验证：";
            for (long long m = 1; m <= n; m++) {
                bool isSubset = (m & n) == m;
                cout << "C(" << m << "," << n << ")是" << (isSubset ? "奇数" : "偶数") << " ";
            }
            cout << endl;
        }
    }
};

/**
 * 交互式求解函数
 */
void interactiveSolve() {
    BinomialParity solver;
    
    cout << "\n=== 交互式求解 ===" << endl;
    cout << "请输入n的值（输入0退出）：";
    
    long long n;
    while (cin >> n && n > 0) {
        long long result = solver.solve(n);
        
        cout << "给定 n = " << n << endl;
        cout << "满足 C(m,n) 是偶数的 m (1<=m<=n) 有 " << result << " 个" << endl;
        
        // 如果n较小，显示详细信息
        if (n <= 20) {
            int k = 0;
            long long temp = n;
            while (temp > 0) {
                k += temp & 1;
                temp >>= 1;
            }
            
            cout << "详细分析：" << endl;
            cout << "- n的二进制表示中有 " << k << " 个1" << endl;
            cout << "- 使C(m,n)为奇数的m有 " << ((1LL << k) - 1) << " 个" << endl;
            cout << "- 因此使C(m,n)为偶数的m有 " << result << " 个" << endl;
        }
        
        cout << "\n请输入n的值（输入0退出）：";
    }
}

int main() {
    BinomialParity solver;
    
    // 运行测试
    solver.test();
    
    // 交互式求解
    interactiveSolve();
    
    return 0;
}

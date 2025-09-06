#include <iostream>
#include <vector>
using namespace std;

/**
 * 分析 (1+x)^p ≡ 1 + x^p (mod m) 中模数的重要性
 * 
 * 这个程序验证为什么模数必须是p，而不能是其他数
 */

/**
 * 计算组合数C(n,k)
 */
long long combination(int n, int k) {
    if (k > n || k < 0) return 0;
    if (k == 0 || k == n) return 1;
    
    long long result = 1;
    for (int i = 0; i < k; i++) {
        result = result * (n - i) / (i + 1);
    }
    return result;
}

/**
 * 验证 (1+x)^p 在不同模数下的表现
 */
void test_different_modulus(int p) {
    cout << "=== 测试 (1+x)^" << p << " 在不同模数下的表现 ===" << endl;
    
    // 计算 (1+x)^p 的所有系数
    vector<long long> coeffs(p + 1);
    for (int k = 0; k <= p; k++) {
        coeffs[k] = combination(p, k);
    }
    
    cout << "(1+x)^" << p << " 的系数: ";
    for (int k = 0; k <= p; k++) {
        cout << "C(" << p << "," << k << ")=" << coeffs[k] << " ";
    }
    cout << endl;
    
    // 测试不同的模数
    int moduli[] = {2, 3, 5, 7, 11};
    int num_moduli = sizeof(moduli) / sizeof(moduli[0]);
    
    for (int i = 0; i < num_moduli; i++) {
        int m = moduli[i];
        cout << "\n模数 m = " << m << ":" << endl;
        
        cout << "系数模" << m << ": ";
        bool all_middle_zero = true;
        for (int k = 0; k <= p; k++) {
            int coeff_mod = coeffs[k] % m;
            cout << coeff_mod << " ";
            
            // 检查中间项是否为0
            if (k > 0 && k < p && coeff_mod != 0) {
                all_middle_zero = false;
            }
        }
        cout << endl;
        
        cout << "中间项(k=1到k=" << (p-1) << ")是否都模" << m << "为0: ";
        cout << (all_middle_zero ? "✅ 是" : "❌ 不是") << endl;
        
        // 检查是否等价于 1 + x^p
        bool equivalent = true;
        for (int k = 0; k <= p; k++) {
            int expected = (k == 0 || k == p) ? 1 : 0;
            if ((coeffs[k] % m) != expected) {
                equivalent = false;
                break;
            }
        }
        
        cout << "(1+x)^" << p << " ≡ 1 + x^" << p << " (mod " << m << "): ";
        cout << (equivalent ? "✅ 成立" : "❌ 不成立") << endl;
        
        if (m == p) {
            cout << "👑 这是素数p本身，根据费马小定理，必须成立！" << endl;
        } else if (equivalent) {
            cout << "🎯 巧合！但不能保证对所有素数p都成立" << endl;
        }
    }
}

/**
 * 解释费马小定理的作用
 */
void explain_fermat_little_theorem() {
    cout << "\n=== 费马小定理的关键作用 ===" << endl;
    
    cout << "费马小定理: 对于素数p和不被p整除的整数a，有 a^(p-1) ≡ 1 (mod p)" << endl;
    cout << "推论: a^p ≡ a (mod p)" << endl;
    
    cout << "\n对于二项式系数 C(p,k) = p!/(k!(p-k)!):" << endl;
    cout << "当 1 ≤ k ≤ p-1 时:" << endl;
    cout << "• 分子包含因子 p" << endl;
    cout << "• 分母 k!(p-k)! 中，k < p 且 (p-k) < p" << endl;
    cout << "• 由于p是素数，gcd(k!(p-k)!, p) = 1" << endl;
    cout << "• 因此 p | C(p,k)，即 C(p,k) ≡ 0 (mod p)" << endl;
    
    cout << "\n这就是为什么:" << endl;
    cout << "(1+x)^p = Σ C(p,k) x^k ≡ C(p,0) + C(p,p) x^p ≡ 1 + x^p (mod p)" << endl;
    
    cout << "\n⚠️  关键点: 这个结论只在模数是p时成立！" << endl;
    cout << "如果模数是其他数m≠p，C(p,k)不一定模m为0！" << endl;
}

/**
 * 反例演示
 */
void show_counterexample() {
    cout << "\n=== 反例演示 ===" << endl;
    
    int p = 5;  // 素数
    int m = 3;  // 不同的素数
    
    cout << "例子: p=" << p << ", m=" << m << endl;
    cout << "计算 C(5,2) = " << combination(5, 2) << endl;
    cout << "C(5,2) mod 5 = " << combination(5, 2) % 5 << " (应该为0)" << endl;
    cout << "C(5,2) mod 3 = " << combination(5, 2) % 3 << " (不为0!)" << endl;
    
    cout << "\n因此:" << endl;
    cout << "• (1+x)^5 ≡ 1 + x^5 (mod 5) ✅ 成立" << endl;
    cout << "• (1+x)^5 ≡ 1 + x^5 (mod 3) ❌ 不成立" << endl;
    
    cout << "\n具体验证模3的情况:" << endl;
    cout << "(1+x)^5 = 1 + 5x + 10x^2 + 10x^3 + 5x^4 + x^5" << endl;
    cout << "模3后: 1 + 2x + 1x^2 + 1x^3 + 2x^4 + x^5" << endl;
    cout << "这显然不等于 1 + x^5" << endl;
}

int main() {
    cout << "🔍 模数分析：为什么 (1+x)^p ≡ 1 + x^p (mod p) 中模数必须是p" << endl;
    cout << "================================================================" << endl;
    
    // 测试p=5的情况
    test_different_modulus(5);
    
    explain_fermat_little_theorem();
    
    show_counterexample();
    
    cout << "\n💡 结论:" << endl;
    cout << "1. (1+x)^p ≡ 1 + x^p (mod p) 只在模数是p时成立" << endl;
    cout << "2. 这基于费马小定理，是p的本质属性" << endl;
    cout << "3. 如果改变模数，这个关系通常不成立" << endl;
    cout << "4. Lucas定理正是利用了这个p的特殊性质" << endl;
    
    return 0;
}

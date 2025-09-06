#include <iostream>
#include <vector>
using namespace std;

/**
 * Lucas定理具体例子：演示为什么可以提取系数
 * 
 * 这个程序通过具体的数值计算来说明Lucas定理证明中
 * "系数提取"的合理性和必然性
 */

/**
 * 计算组合数C(n,k) mod p
 */
long long combination_mod(int n, int k, int p) {
    if (k > n || k < 0) return 0;
    if (k == 0 || k == n) return 1;
    
    long long result = 1;
    for (int i = 0; i < k; i++) {
        result = result * (n - i) / (i + 1);
    }
    return result % p;
}

/**
 * 计算多项式 (1+x)^n 在模p下的系数
 */
vector<int> polynomial_coeffs(int n, int p) {
    vector<int> coeffs(n + 1, 0);
    
    // 计算二项式系数
    for (int k = 0; k <= n; k++) {
        coeffs[k] = combination_mod(n, k, p);
    }
    
    return coeffs;
}

/**
 * 验证多项式同余关系的具体例子
 */
void demonstrate_polynomial_congruence() {
    cout << "=== 验证 (1+x)^p ≡ 1 + x^p (mod p) ===" << endl;
    
    int p = 5;
    
    // 计算 (1+x)^5 的系数
    vector<int> left_coeffs = polynomial_coeffs(5, p);
    
    cout << "(1+x)^5 的系数: ";
    for (int i = 0; i <= 5; i++) {
        cout << "C(5," << i << ")=" << left_coeffs[i] << " ";
    }
    cout << endl;
    
    cout << "模5后的系数: ";
    for (int i = 0; i <= 5; i++) {
        cout << left_coeffs[i] % p << " ";
    }
    cout << endl;
    
    cout << "右边 1 + x^5 的系数: 1 0 0 0 0 1" << endl;
    
    cout << "验证同余关系: ";
    bool congruent = true;
    for (int i = 0; i <= 5; i++) {
        int expected = (i == 0 || i == 5) ? 1 : 0;
        if (left_coeffs[i] % p != expected) {
            congruent = false;
            break;
        }
    }
    cout << (congruent ? "✅ 成立" : "❌ 不成立") << endl;
    
    cout << "\n关键观察: 中间项系数(mod 5)确实都是0！" << endl;
    cout << "但这个同余关系的意义不在于个别系数，而在于整体结构！" << endl;
}

/**
 * 演示Lucas定理的完整构造过程
 */
void demonstrate_lucas_construction() {
    cout << "\n=== Lucas定理构造过程演示 ===" << endl;
    
    // 以 p=2, n=5=101₂, m=3=011₂ 为例
    int p = 2;
    int n = 5; // 101₂: n₂=1, n₁=0, n₀=1  
    int m = 3; // 011₂: m₂=0, m₁=1, m₀=1
    
    cout << "例子: p=2, n=5=101₂, m=3=011₂" << endl;
    
    // 第一步：基础同余关系
    cout << "\n第一步：基础同余关系" << endl;
    cout << "(1+x)² ≡ 1 + x² (mod 2)" << endl;
    
    vector<int> coeffs_x2 = polynomial_coeffs(2, 2);
    cout << "(1+x)² = ";
    for (int i = 0; i <= 2; i++) {
        if (i > 0) cout << " + ";
        cout << coeffs_x2[i] << "x^" << i;
    }
    cout << endl;
    
    cout << "模2后: ";
    for (int i = 0; i <= 2; i++) {
        if (i > 0) cout << " + ";  
        cout << (coeffs_x2[i] % 2) << "x^" << i;
    }
    cout << " = 1 + x² ✅" << endl;
    
    // 第二步：递归构造
    cout << "\n第二步：递归构造" << endl;
    cout << "(1+x)⁴ = ((1+x)²)² ≡ (1 + x²)² ≡ 1 + x⁴ (mod 2)" << endl;
    
    // 第三步：分解n
    cout << "\n第三步：分解 n=5=101₂" << endl;
    cout << "(1+x)⁵ = (1+x)⁴⁺¹ = (1+x)⁴ · (1+x)¹" << endl;
    cout << "     ≡ (1 + x⁴) · (1 + x) (mod 2)" << endl;
    cout << "     = 1 + x + x⁴ + x⁵" << endl;
    
    // 第四步：验证左边
    cout << "\n第四步：验证左边 (1+x)⁵" << endl;
    vector<int> coeffs_left = polynomial_coeffs(5, 2);
    cout << "(1+x)⁵ 的系数(mod 2): ";
    for (int i = 0; i <= 5; i++) {
        cout << (coeffs_left[i] % 2) << " ";
    }
    cout << endl;
    cout << "即: 1 + x¹ + x⁴ + x⁵ ✅" << endl;
    
    // 第五步：Lucas公式验证
    cout << "\n第五步：Lucas定理验证" << endl;
    cout << "C(3,5) ≡ C(0,1) × C(1,0) × C(1,1) (mod 2)" << endl;
    
    int c01 = combination_mod(1, 0, 2);
    int c10 = combination_mod(0, 1, 2); 
    int c11 = combination_mod(1, 1, 2);
    
    cout << "     = " << c01 << " × " << c10 << " × " << c11 << endl;
    cout << "     = " << (c01 * c10 * c11) % 2 << endl;
    
    int direct = combination_mod(5, 3, 2);
    cout << "直接计算: C(3,5) mod 2 = " << direct << endl;
    
    cout << "验证: " << ((c01 * c10 * c11) % 2 == direct ? "✅ 相等" : "❌ 不相等") << endl;
}

/**
 * 解释为什么系数提取是合理的
 */
void explain_coefficient_extraction() {
    cout << "\n=== 为什么可以提取系数？ ===" << endl;
    
    cout << "关键理解:" << endl;
    cout << "1. 我们不是从单一的 (1+x)^p ≡ 1 + x^p 提取系数" << endl;
    cout << "2. 我们是构造了两个完整的多项式:" << endl;
    cout << "   左边: (1+x)^n" << endl;
    cout << "   右边: ∏(1+x^{p^i})^{n_i}" << endl;
    cout << "3. 这两个多项式在模p下相等" << endl;
    cout << "4. 相等的多项式，对应系数必须相等" << endl;
    cout << "5. 因此可以比较x^m的系数得出Lucas定理" << endl;
    
    cout << "\n多项式同余的含义:" << endl;
    cout << "• 不是说个别系数有特殊意义" << endl;
    cout << "• 而是说整个函数的结构发生了简化" << endl;
    cout << "• (1+x)^p 在模p下的\"行为\"等同于 1+x^p" << endl;
    cout << "• 这为递归构造提供了基础变换规则" << endl;
}

int main() {
    cout << "🔍 Lucas定理核心疑惑：系数提取的合理性演示" << endl;
    cout << "====================================================" << endl;
    
    demonstrate_polynomial_congruence();
    demonstrate_lucas_construction();  
    explain_coefficient_extraction();
    
    cout << "\n💡 总结:" << endl;
    cout << "Lucas定理的证明不是魔法，而是严谨的数学构造!" << endl;
    cout << "每一步都有清晰的逻辑依据，系数提取是必然的结果！" << endl;
    
    return 0;
}

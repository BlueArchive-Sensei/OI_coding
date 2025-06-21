#include <iostream>
#include <string>
using namespace std;

const long long MOD = 1000000007;

long long power_mod(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

long long string_mod(const string& s, long long mod) {
    long long result = 0;
    for (char c : s) {
        result = (result * 10 + (c - '0')) % mod;
    }
    return result;
}

// 计算(s-1) mod mod，特别处理超大数
long long string_minus_1_mod(const string& s, long long mod) {
    if (s == "1") return 0;
    
    // 对于不太大的数，直接计算
    if (s.length() <= 18) {
        long long val = 0;
        for (char c : s) {
            val = val * 10 + (c - '0');
        }
        return (val - 1) % mod;
    }
    
    // 对于超大数，使用模运算
    long long val = string_mod(s, mod);
    return (val - 1 + mod) % mod;
}

// 矩阵快速幂（只处理指数不太大的情况）
void matrix_power(long long mat[2][2], long long exp, long long result[2][2]) {
    result[0][0] = result[1][1] = 1;
    result[0][1] = result[1][0] = 0;
    
    if (exp == 0) return;
    
    long long base[2][2];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            base[i][j] = mat[i][j];
        }
    }
    
    while (exp > 0) {
        if (exp & 1) {
            long long temp[2][2];
            temp[0][0] = (result[0][0] * base[0][0] + result[0][1] * base[1][0]) % MOD;
            temp[0][1] = (result[0][0] * base[0][1] + result[0][1] * base[1][1]) % MOD;
            temp[1][0] = (result[1][0] * base[0][0] + result[1][1] * base[1][0]) % MOD;
            temp[1][1] = (result[1][0] * base[0][1] + result[1][1] * base[1][1]) % MOD;
            
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    result[i][j] = temp[i][j];
                }
            }
        }
        
        long long temp[2][2];
        temp[0][0] = (base[0][0] * base[0][0] + base[0][1] * base[1][0]) % MOD;
        temp[0][1] = (base[0][0] * base[0][1] + base[0][1] * base[1][1]) % MOD;
        temp[1][0] = (base[1][0] * base[0][0] + base[1][1] * base[1][0]) % MOD;
        temp[1][1] = (base[1][0] * base[0][1] + base[1][1] * base[1][1]) % MOD;
        
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                base[i][j] = temp[i][j];
            }
        }
        
        exp >>= 1;
    }
}

long long solve(const string& n_str, const string& m_str, long long a, long long b, long long c, long long d) {
    if (n_str == "1" && m_str == "1") return 1;
    
    // 判断数据规模
    bool n_large = (n_str.length() > 18);
    bool m_large = (m_str.length() > 18);
    
    // Case: m = 1
    if (m_str == "1") {
        if (n_str == "1") return 1;
        
        if (c == 1) {
            long long n_minus_1_mod = string_minus_1_mod(n_str, MOD);
            return (1 + n_minus_1_mod * d) % MOD;
        } else {
            // 对于超大的n，矩阵快速幂的指数处理需要特别小心
            if (n_large) {
                // 使用近似方法：假设矩阵的周期是MOD-1
                long long exp = string_minus_1_mod(n_str, MOD - 1);
                if (exp == 0 && n_str != "1") exp = MOD - 1;
                
                long long matrix[2][2] = {{c, d}, {0, 1}};
                long long result[2][2];
                matrix_power(matrix, exp, result);
                return (result[0][0] + result[0][1]) % MOD;
            } else {
                long long n = 0;
                for (char ch : n_str) {
                    n = n * 10 + (ch - '0');
                }
                long long matrix[2][2] = {{c, d}, {0, 1}};
                long long result[2][2];
                matrix_power(matrix, n - 1, result);
                return (result[0][0] + result[0][1]) % MOD;
            }
        }
    }
    
    // Case: n = 1
    if (n_str == "1") {
        if (a == 1) {
            long long m_minus_1_mod = string_minus_1_mod(m_str, MOD);
            return (1 + b * m_minus_1_mod) % MOD;
        } else {
            long long exp = string_minus_1_mod(m_str, MOD - 1);
            long long a_pow = power_mod(a, exp, MOD);
            
            if ((a - 1) % MOD == 0) {
                long long m_minus_1_mod = string_minus_1_mod(m_str, MOD);
                return (1 + b * m_minus_1_mod) % MOD;
            } else {
                long long inv = power_mod(a - 1, MOD - 2, MOD);
                return (a_pow + b * (a_pow - 1 + MOD) % MOD * inv) % MOD;
            }
        }
    }
    
    // Case: a = c = 1 (线性情况，可以直接计算)
    if (a == 1 && c == 1) {
        long long m_minus_1_mod = string_minus_1_mod(m_str, MOD);
        long long n_minus_1_mod = string_minus_1_mod(n_str, MOD);
        long long step = (b * m_minus_1_mod + d) % MOD;
        long long f_n_1 = (1 + n_minus_1_mod * step) % MOD;
        return (f_n_1 + b * m_minus_1_mod) % MOD;
    }
    
    // General case
    long long m_minus_1_exp = string_minus_1_mod(m_str, MOD - 1);
    long long m_minus_1_mod = string_minus_1_mod(m_str, MOD);
    
    long long coeff, const_term;
    if (a == 1) {
        coeff = 1;
        const_term = (b * m_minus_1_mod) % MOD;
    } else {
        long long a_pow = power_mod(a, m_minus_1_exp, MOD);
        coeff = a_pow;
        
        if ((a - 1) % MOD == 0) {
            const_term = (b * m_minus_1_mod) % MOD;
        } else {
            long long inv = power_mod(a - 1, MOD - 2, MOD);
            const_term = (b * (a_pow - 1 + MOD) % MOD * inv) % MOD;
        }
    }
    
    // 矩阵快速幂
    long long matrix[2][2] = {
        {(c * coeff) % MOD, (c * const_term + d) % MOD},
        {0, 1}
    };
    
    long long result[2][2];
    
    if (n_large) {
        long long exp = string_minus_1_mod(n_str, MOD - 1);
        if (exp == 0 && n_str != "1") exp = MOD - 1;
        matrix_power(matrix, exp, result);
    } else {
        long long n = 0;
        for (char ch : n_str) {
            n = n * 10 + (ch - '0');
        }
        matrix_power(matrix, n - 1, result);
    }
    
    long long f_n_1 = (result[0][0] + result[0][1]) % MOD;
    return (coeff * f_n_1 + const_term) % MOD;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string n_str, m_str;
    long long a, b, c, d;
    
    cin >> n_str >> m_str >> a >> b >> c >> d;
    
    a %= MOD; b %= MOD; c %= MOD; d %= MOD;
    
    cout << solve(n_str, m_str, a, b, c, d) << endl;
    
    return 0;
} 
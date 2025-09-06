## 子集反演与 FWT（Walsh–Hadamard）原理与证明

本文系统梳理集合子集偏序上的 Zeta/莫比乌斯变换及其反演（简称“子集反演”），以及三类经典卷积（OR/AND/XOR）与快速变换（FWT/FWHT）的等价关系与证明，并给出可直接使用的 C++ 模板与实现细节、易错点与复杂度分析。

---

### 1. 问题背景与基本记号

- **全集与掩码**：设全集为 $[n]=\{0,1,\dots,n-1\}$。用整数掩码 `S ∈ [0,2^n)` 表示子集 $S\subseteq [n]$。位 $i$ 处为 1 表示元素 $i\in S$。
- **函数与数组**：任意函数 $f:2^{[n]}\to R$ 可用长度 $N=2^n$ 的数组表示，记 $f[S]$ 为子集 $S$ 处的值。
- **三类卷积定义**：给定 $f,g$，定义
  - OR 卷积：$ (f\,\text{OR}\,g)[S] = \sum\limits_{A\cup B = S} f[A]g[B] $
  - AND 卷积：$ (f\,\text{AND}\,g)[S] = \sum\limits_{A\cap B = S} f[A]g[B] $
  - XOR 卷积：$ (f\,\text{XOR}\,g)[S] = \sum\limits_{A\oplus B = S} f[A]g[B] $，其中 $\oplus$ 为对称差（按位异或）。

我们的目标是用合适的变换将上述卷积转化为“逐点乘法”，从而在 $O(n2^n)$ 时间内完成计算。

---

### 2. 子集偏序、Zeta 与 Möbius 反演

#### 2.1 子集偏序与两种 Zeta 变换

在偏序集合 $(2^{[n]}, \subseteq)$ 上，我们常用两种 Zeta 变换：

- 子集 Zeta（顺着 $\subseteq$ 汇总）：
  $$ (\mathcal{Z}_{\subseteq} f)[S] = \sum_{T\subseteq S} f[T]. $$

- 超集 Zeta（顺着 $\supseteq$ 汇总）：
  $$ (\mathcal{Z}_{\supseteq} f)[S] = \sum_{T\supseteq S} f[T]. $$

二者分别对应从“下往上”和“上往下”的累加。

#### 2.2 Möbius 反演（子集格上的 μ）

在子集格（布尔格）上，Möbius 函数为：
$$
\mu_{\subseteq}(S,T) = \begin{cases}
(-1)^{|T|-|S|}, & S\subseteq T,\\
0, & \text{otherwise}.
\end{cases}
$$
于是，对 $F=\mathcal{Z}_{\subseteq}f$，有反演公式
$$
f[S] = \sum_{T\subseteq S} \mu_{\subseteq}(T,S)\,F[T] = \sum_{T\subseteq S} (-1)^{|S|-|T|} F[T].
$$

同理，针对 $\mathcal{Z}_{\supseteq}$ 也有对应的 Möbius 反演。

#### 2.3 反演证明一：二项式恒等式法

关键恒等式：对任意 $S\subseteq T$，有
$$\sum_{U:\,S\subseteq U\subseteq T} (-1)^{|T|-|U|} = [S=T],$$
其中 $[\cdot]$ 为指示函数。证明：令 $k=|T|-|S|$，上式变为 $\sum_{i=\,0}^{k} \binom{k}{i} (-1)^{k-i} = (1-1)^k = [k=0]$。

这恰是 $\mu$ 与 $\zeta$ 的互逆性：$\mu * \zeta = \delta$。

#### 2.4 反演证明二：坐标分离（张量积）

一维情况下（单个比特），“下向 Zeta”对应矩阵 $\begin{pmatrix}1 & 0\\ 1 & 1\end{pmatrix}$，其逆矩阵为 $\begin{pmatrix}1 & 0\\ -1 & 1\end{pmatrix}$。$n$ 维布尔格是这套 2×2 矩阵的 $n$ 次 Kronecker 积，因此逆仍成立。这给出高效的逐位 DP 实现（见第 6 节）。

---

### 3. OR 卷积与子集 Zeta 的等价性

令 $h = f\,\text{OR}\,g$。定义 $F=\mathcal{Z}_{\subseteq} f$, $G=\mathcal{Z}_{\subseteq} g$。记点乘为逐点乘法。则有
$$ \mathcal{Z}_{\subseteq} h = F \odot G. $$

证明：对任意 $S$，
$$
\begin{aligned}
(\mathcal{Z}_{\subseteq} h)[S]
&= \sum_{U\subseteq S} h[U]
 = \sum_{U\subseteq S} \sum_{A\cup B = U} f[A]g[B] \\
&= \sum_{A\subseteq S}\sum_{B\subseteq S} f[A]g[B] \cdot [A\cup B\subseteq S]
 = \sum_{A\subseteq S} f[A] \cdot \sum_{B\subseteq S} g[B] \\
&= F[S]\cdot G[S].
\end{aligned}
$$
直观理解：$F[S]$ 统计所有 $A\subseteq S$，$G[S]$ 统计所有 $B\subseteq S$。两者相乘即统计所有 $A,B\subseteq S$ 的贡献；它们的并一定 $\subseteq S$。最后对 $\mathcal{Z}_{\subseteq} h$ 反演即可得 $h$。

---

### 4. AND 卷积与超集 Zeta 的等价性

令 $h = f\,\text{AND}\,g$。定义 $F=\mathcal{Z}_{\supseteq} f$, $G=\mathcal{Z}_{\supseteq} g$。则有
$$ \mathcal{Z}_{\supseteq} h = F \odot G. $$

证明：对任意 $S$，
$$
\begin{aligned}
(\mathcal{Z}_{\supseteq} h)[S]
&= \sum_{U\supseteq S} h[U]
 = \sum_{U\supseteq S} \sum_{A\cap B = U} f[A]g[B] \\
&= \sum_{A\supseteq S}\sum_{B\supseteq S} f[A]g[B] \cdot [A\cap B\supseteq S]
 = \sum_{A\supseteq S} f[A] \cdot \sum_{B\supseteq S} g[B] \\
&= F[S]\cdot G[S].
\end{aligned}
$$
因此计算 AND 卷积可用 $\mathcal{Z}_{\supseteq}$ 做点乘后再做 Möbius 反演。

---

### 5. XOR 卷积与 Walsh–Hadamard 变换

设群 $G=(\mathbb{Z}/2\mathbb{Z})^n$（按位异或）。其特征（角色）为
$$ \chi_{\omega}(x) = (-1)^{\langle \omega, x\rangle}, \quad \omega,x\in G, $$
其中 $\langle\cdot,\cdot\rangle$ 为按位内积模 2。定义群傅里叶变换：
$$ \widehat{f}(\omega) = \sum_{x\in G} f(x)\,\chi_{\omega}(x). $$

群代数中，卷积定义为 $((f\ast g)(s) = \sum_{x+y=s} f(x)g(y))$（此处 + 即按位异或）。标准 Fourier 对偶性给出
$$ \widehat{f\ast g}(\omega) = \widehat{f}(\omega)\cdot \widehat{g}(\omega). $$

Walsh–Hadamard 变换（FWHT）即在基 $\{\chi_{\omega}\}$ 下的快速实现。其变换矩阵为 $(H_n = H_1^{\otimes n})$，其中
$$ H_1 = \begin{pmatrix}1 & 1\\ 1 & -1\end{pmatrix}, \quad H_n H_n^\top = N I, \; N=2^n. $$
因此逆变换为 $(H_n^{-1} = \frac{1}{N} H_n)$。算法通过“蝶形”实现，每一比特维度执行一次：

- 正变换（未归一）：对每个比特 $i$，将 $(x,y)$ 更新为 $(x+y, x-y)$。
- 逆变换：执行同样的蝶形后，整体再除以 $N$。

这将 XOR 卷积降为“变换—逐点乘—逆变换”。

---

### 6. 实现模板（C++）

以下模板以 `vector<long long>`/`vector<double>`/模数为例，复杂度均为 $O(n2^n)$。

#### 6.1 子集 Zeta 与 Möbius（对应 OR 卷积）

```cpp
#include <bits/stdc++.h>
using namespace std;

// 就地子集 Zeta: F[S] = sum_{T⊆S} f[T]
template <class T>
void zeta_subset(vector<T> &f, int n) {
    const int N = 1 << n;
    for (int i = 0; i < n; ++i) {
        for (int mask = 0; mask < N; ++mask) {
            if (mask & (1 << i)) {
                f[mask] += f[mask ^ (1 << i)];
            }
        }
    }
}

// 就地子集 Möbius（反演）
template <class T>
void mobius_subset(vector<T> &f, int n) {
    const int N = 1 << n;
    for (int i = 0; i < n; ++i) {
        for (int mask = 0; mask < N; ++mask) {
            if (mask & (1 << i)) {
                f[mask] -= f[mask ^ (1 << i)];
            }
        }
    }
}

// OR 卷积
template <class T>
vector<T> convolution_or(vector<T> f, vector<T> g, int n) {
    zeta_subset(f, n);
    zeta_subset(g, n);
    const int N = 1 << n;
    vector<T> h(N);
    for (int i = 0; i < N; ++i) h[i] = f[i] * g[i];
    mobius_subset(h, n);
    return h;
}
```

#### 6.2 超集 Zeta 与 Möbius（对应 AND 卷积）

```cpp
// 就地超集 Zeta: F[S] = sum_{T⊇S} f[T]
template <class T>
void zeta_superset(vector<T> &f, int n) {
    const int N = 1 << n;
    for (int i = 0; i < n; ++i) {
        for (int mask = 0; mask < N; ++mask) {
            if ((mask & (1 << i)) == 0) {
                f[mask] += f[mask | (1 << i)];
            }
        }
    }
}

// 就地超集 Möbius（反演）
template <class T>
void mobius_superset(vector<T> &f, int n) {
    const int N = 1 << n;
    for (int i = 0; i < n; ++i) {
        for (int mask = 0; mask < N; ++mask) {
            if ((mask & (1 << i)) == 0) {
                f[mask] -= f[mask | (1 << i)];
            }
        }
    }
}

// AND 卷积
template <class T>
vector<T> convolution_and(vector<T> f, vector<T> g, int n) {
    zeta_superset(f, n);
    zeta_superset(g, n);
    const int N = 1 << n;
    vector<T> h(N);
    for (int i = 0; i < N; ++i) h[i] = f[i] * g[i];
    mobius_superset(h, n);
    return h;
}
```

#### 6.3 Walsh–Hadamard 变换（对应 XOR 卷积）

```cpp
// 未归一的 FWHT（正变换与逆变换结构相同）
template <class T>
void fwht(vector<T> &a, bool invert) {
    int n = 0; while ((1 << n) < (int)a.size()) ++n;
    for (int len = 1; 2 * len <= (int)a.size(); len <<= 1) {
        for (int i = 0; i < (int)a.size(); i += 2 * len) {
            for (int j = 0; j < len; ++j) {
                T u = a[i + j];
                T v = a[i + j + len];
                a[i + j] = u + v;
                a[i + j + len] = u - v;
            }
        }
    }
    if (invert) {
        T invN = (T)1 / (T)a.size(); // 浮点；模数需用乘法逆元
        for (auto &x : a) x *= invN;
    }
}

template <class T>
vector<T> convolution_xor(vector<T> f, vector<T> g) {
    fwht(f, false);
    fwht(g, false);
    const int N = (int)f.size();
    vector<T> h(N);
    for (int i = 0; i < N; ++i) h[i] = f[i] * g[i];
    fwht(h, true);
    return h;
}
```

#### 6.4 模数版本要点（以 998244353 为例）

```cpp
const long long MOD = 998244353;

long long mod_pow(long long a, long long e) {
    long long r = 1 % MOD;
    while (e) { if (e & 1) r = r * a % MOD; a = a * a % MOD; e >>= 1; }
    return r;
}

// FWHT 的逆变换需乘 invN = (2^n)^{-1} mod MOD
void fwht_mod(vector<long long> &a, bool invert) {
    for (int len = 1; 2 * len <= (int)a.size(); len <<= 1) {
        for (int i = 0; i < (int)a.size(); i += 2 * len) {
            for (int j = 0; j < len; ++j) {
                long long u = a[i + j];
                long long v = a[i + j + len];
                a[i + j] = (u + v) % MOD;
                a[i + j + len] = (u - v + MOD) % MOD;
            }
        }
    }
    if (invert) {
        long long invN = mod_pow((long long)a.size(), MOD - 2);
        for (auto &x : a) x = x * invN % MOD;
    }
}
```

---

### 7. 易错点与实现细节

- **归一化因子（XOR）**：未归一 FWHT 的逆变换需整体除以 $N=2^n$。模数下应乘 $N^{-1}$（要求 $\gcd(N,MOD)=1$）。若模数为 2 的幂，会退化，不可行。
- **就地与拷贝**：Zeta/Möbius 可以就地执行，但卷积常需保留原输入，建议在函数参数处按值传递副本或自行拷贝。
- **类型选择**：整型卷积可能溢出，优先使用 `long long` 或取模。浮点 FWHT 可能带来误差，若需整数精确性建议用模数版本。
- **遍历方向**：实现 `zeta_superset`/`mobius_superset` 时条件是 `(mask & (1<<i)) == 0`；不要求倒序，只要公式一致即可。
- **复杂度**：三类快速卷积时间 $O(n2^n)$，空间 $O(2^n)$。当 $n$ 达到 20–22 时占用与时间已较大，需注意常数与缓存友好。

---

### 8. 小例子（n=2）

设 $n=2$，子集按掩码顺序 00,01,10,11。令 $f=[f_0,f_1,f_2,f_3],\ g=[g_0,g_1,g_2,g_3]$。

- 子集 Zeta：一次比特（最低位）更新后得到 `[f0, f0+f1, f2, f2+f3]`，再在第二比特更新：
  得 $[f_0, f_0+f_1, f_0+f_2, f_0+f_1+f_2+f_3]$。这对应 $\sum_{T\subseteq S} f[T]$。
- XOR 的一次蝶形：对块 `(f0,f1)` 与 `(f2,f3)` 做 `(x+y,x-y)`，再对跨块 `(f0,f2)` 与 `(f1,f3)` 做同样操作。

手算此小例能验证推导式与实现的正确性。

---

### 9. 小结

- OR/AND 卷积分别与 $\mathcal{Z}_{\subseteq}$/$\mathcal{Z}_{\supseteq}$ 的点乘等价；
- XOR 卷积与布尔群上的 Fourier（Walsh–Hadamard）等价；
- 子集格 Möbius 的本质是二项式恒等式与坐标分离（Kronecker 积）下的矩阵可逆性；
- 实现层面注意逆归一化、模数逆元、溢出与缓存友好。

以上内容可直接支撑常见“集合幂集卷积”类题目的推导与实现。



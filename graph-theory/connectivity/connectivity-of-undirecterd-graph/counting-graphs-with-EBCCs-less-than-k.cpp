/*
 * https://www.luogu.com.cn/problem/P6596
 */

#include <bits/stdc++.h>
using namespace std;

using i64 = int64_t;

template <i64 M>
class modint {
   private:
    i64 val;

   public:
    constexpr static i64 MOD = M;
    constexpr modint() : val(0) {}
    constexpr modint(i64 x) : val(x % M) {}
    constexpr i64 value() const { return val; }
    constexpr modint operator+(const modint &other) const { return modint(val + other.val); }
    constexpr modint operator-(const modint &other) const { return modint(val + M - other.val); }
    constexpr modint operator*(const modint &other) const { return modint(val * other.val); }
    constexpr modint operator/(const modint &other) const { return (*this) * other.inv(); }
    constexpr modint &operator+=(const modint &other) {
        val = (val + other.val) % M;
        return *this;
    }
    constexpr modint &operator-=(const modint &other) {
        val = (val + M - other.val) % M;
        return *this;
    }
    constexpr modint &operator*=(const modint &other) {
        val = val * other.val % M;
        return *this;
    }
    constexpr modint &operator/=(const modint &other) { return (*this *= other.inv()); }
    constexpr bool operator==(const modint &other) const { return val == other.val; }
    constexpr bool operator!=(const modint &other) const { return val != other.val; }
    modint inv() const { return pow(M - 2); }
    modint pow(i64 exp) const {
        if (exp == 0) return 1;
        if (exp == 1) return *this;
        return pow(exp % 2) * (*this * *this).pow(exp / 2);
    }
    friend ostream &operator<<(ostream &os, const modint &m) { return os << m.val; }
    friend istream &operator>>(istream &is, modint &m) {
        i64 x;
        is >> x, m = modint(x);
        return is;
    }
};

template <class Z>
class dynamic_combination {
   private:
    vector<Z> fact, ifact;

    void init(int n) {
        if (fact.empty()) fact.push_back(1);
        for (int i = fact.size(); i <= n; i++) fact.push_back(fact.back() * i);
        ifact.resize(fact.size());
        ifact.back() = fact.back().inv();
        for (int i = fact.size() - 1; i > 0; i--) ifact[i - 1] = ifact[i] * i;
    }

   public:
    Z fac(int n) {
        if (n >= fact.size()) init(2 * n);
        return fact[n];
    }

    Z ifac(int n) {
        if (n >= ifact.size()) init(2 * n);
        return ifact[n];
    }

    Z comb(int n, int k) {
        if (n < k || n < 0 || k < 0) return 0;
        return fac(n) * ifac(k) * ifac(n - k);
    }

    Z perm(int n, int k) {
        if (n < k || n < 0 || k < 0) return 0;
        return fac(n) * ifac(n - k);
    }

    Z shuf(int n, int k) { return comb(n + k, k); }

    Z catalan(int n) { return comb(2 * n, n) / (n + 1); }
};

using Z = modint<1'000'000'007>;
dynamic_combination<Z> dc;

vector<Z> mult(vector<Z> a, vector<Z> b) {
    vector<Z> c(a.size() + b.size() - 1);
    for (int i = 0; i < a.size(); i++)
        for (int j = 0; j < b.size(); j++) c[i + j] += a[i] * b[j];
    return c;
}

using u32 = uint32_t;

int main() {
    u32 n, m;
    cin >> n >> m;

    auto graph_cnt = [&](u32 nd) { return Z(2).pow(nd * (nd - 1) / 2); };

    vector<Z> g(n + 1);
    for (int i = 1; i <= n; i++) {
        g[i] = graph_cnt(i);
        for (int j = 1; j < i; j++) g[i] -= dc.comb(i - 1, j - 1) * g[j] * graph_cnt(i - j);
    }
    for (int i = 1; i <= n; i++) g[i] *= dc.ifac(i - 1) * n;
    vector<Z> f = {1}, u = {0};

    for (int i = 1; i <= n; i++) {
        f = mult(f, g), f.resize(n + 1);
        u.push_back(f[n] * dc.fac(n) / (n * n) / dc.fac(i));
    }

    vector<Z> ans(n + 1);
    for (int i = 1; i <= n; i++)
        for (int j = i; j <= n; j++) ans[i] += dc.comb(j - 1, i - 1) * u[j] * Z(-1).pow(j - i);
    partial_sum(ans.begin(), ans.end(), ans.begin());

    cout << ans[min(n, m + 1)] << endl;
}

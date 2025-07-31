/*
 * Original Problem: https://www.luogu.com.cn/problem/P3776
 */

#include <bits/stdc++.h>
using namespace std;

using i64 = int64_t;
using i32 = int32_t;
using u64 = uint64_t;
using u32 = uint32_t;

template <class T>  // basic type prefered; pair -compress> u64.
class double_vector {
   private:
    u32 n;
    vector<u32> u, deg;
    vector<T> v, data;
    bool built = false;

   public:
    double_vector(int N, int exV) : n(N), deg(N + 1, 0) { u.reserve(exV), v.reserve(exV), data.reserve(exV); }

    void push_back(u32 x, T val) { u.push_back(x), v.push_back(val), deg[x]++; }

    void build() {
        if (built) return;
        built = true;

        u32 sum = 0;
        auto fn = [&](u32 j) { sum += deg[j], deg[j] = sum; };
        u32 i;
        for (i = 0; i < n; i += 4) fn(i), fn(i + 1), fn(i + 2), fn(i + 3);
        for (; i < n; i++) fn(i);

        u32 V = u.size();
        data.resize(V);
#pragma GCC unroll 4
        for (u32 j = 0; j < V; j++) data[--deg[u[j]]] = v[j];
        deg.back() = V;
    }

    template <class _Callable>
    void for_each(u32 x, _Callable&& fn) {
        build();
        for (u32 i = deg[x]; i < deg[x + 1]; i++) fn(data[i]);
    }
};

class bi_fenwick {
   private:
    u32 n;
    array<vector<i64>, 2> tree;
    u32 lowbit(u32 x) { return x & -x; }
    void internal_add(u32 fenw_id, u32 idx, u64 val) {
        val *= (fenw_id == 0 ? 1 : idx);
        for (idx++; idx <= n; idx += lowbit(idx)) tree[fenw_id][idx] += val;
    }
    i64 internal_query(u32 fenw_id, u32 idx) {
        u64 res = 0, coeff = (fenw_id == 1 ? 1 : idx);
        for (idx = min(idx, n); idx > 0; idx -= lowbit(idx)) res += tree[fenw_id][idx];
        return res * coeff;
    }

   public:
    bi_fenwick(u32 n = 0) : n(n) { tree[0].resize(n + 1), tree[1].resize(n + 1); }
    void add(u32 l, u32 r, i64 val) {
        if (l >= r) return;
        for (int i : {0, 1}) internal_add(i, l, val), internal_add(i, r, -val);
    }
    i64 pref(u32 idx) { return internal_query(0, idx) - internal_query(1, idx); }
    i64 sum(u32 l, u32 r) {
        if (l >= r) return 0;
        return pref(r) - pref(l);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr), cout.tie(nullptr);

    u32 r, c, m, q, x, y;
    cin >> r >> c >> m >> q >> x >> y, x--, y--;
    vector<vector<u32>> vec(r);
    string s;
    if (m) cin >> s;
    vec[x].push_back(y);

    int dx[] = {-1, 1, 0, 0}, dy[] = {0, 0, 1, -1}, sign_weight[] = {1, -1, -1, 1}, corr[256];
    corr['N'] = 0, corr['S'] = 1, corr['E'] = 2, corr['W'] = 3;

    u32 p_lx = x, p_rx = x + 1, p_ly = y, p_ry = y + 1;
    for (char c : s) {
        x += dx[corr[c]], y += dy[corr[c]], vec[x].push_back(y);
        p_lx = min(p_lx, x), p_rx = max(p_rx, x + 1), p_ly = min(p_ly, y), p_ry = max(p_ry, y + 1);
    }

    vector<u64> ans(q);
    using query = array<i64, 5>;
    double_vector<query> queries(r, 8 * q);

    auto add_query = [&](u32 lx, u32 rx, auto&& gen) {
        auto gen0 = gen(1), gen1 = gen(-1);
        if (lx >= rx || gen0[0] >= gen0[1]) return;
        if (rx <= r) queries.push_back(rx - 1, gen0);
        if (lx > 0) queries.push_back(lx - 1, gen1);
    };

    for (u32 i = 0, lx, rx, ly, ry; i < q; i++) {
        cin >> lx >> ly >> rx >> ry, lx--, ly--;
        if (lx + 1 <= p_lx && p_rx <= rx - 1 && ly + 1 <= p_ly && p_ry <= ry - 1) ans[i] = 1;

        for (u32 px : {0, 1})
            for (u32 py : {0, 1})
                add_query(lx + px, rx, [&](int sign) -> query { return {ly + py, ry, sign, i, px + 2 * py}; });
    }

    array<bi_fenwick, 4> fenw;
    for (u32 i = 0; i < 4; i++) fenw[i] = bi_fenwick(c + 1);

    for (u32 i = 0; i < r; i++) {
        auto& now = vec[i];
        sort(now.begin(), now.end());
        now.push_back(c);
        u32 last = 0;
        for (u32 x : now) fenw[0].add(last, x, 1), fenw[2].add(last + 1, x, 1), last = x + 1;

        if (i > 0) {
            auto& pre = vec[i - 1];
            pre.insert(pre.end(), now.begin(), now.end());
            sort(pre.begin(), pre.end());

            last = 0;
            for (u32 x : pre) fenw[1].add(last, x, 1), fenw[3].add(last + 1, x, 1), last = x + 1;
        }

        queries.for_each(i, [&](auto& query) {
            auto& [l, r, sign, idx, px] = query;
            ans[idx] += sign * sign_weight[px] * fenw[px].sum(l, r);
        });
    }

    for (int a : ans) cout << a << "\n";
} // 快读板子不支持 cin >> string, /fn.

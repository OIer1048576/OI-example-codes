/**
 * Original Problem: https://www.luogu.com.cn/problem/P11831
 */

#include <bits/stdc++.h>
using namespace std;

using u32 = uint32_t;
using u64 = uint64_t;

namespace Fread {
const int SIZE = 1 << 21;
char buf[SIZE], *S, *T;
inline char getchar() {
    if (S == T) {
        T = (S = buf) + fread(buf, 1, SIZE, stdin);
        if (S == T) return '\n';
    }
    return *S++;
}
}  // namespace Fread
namespace Fwrite {
const int SIZE = 1 << 21;
char buf[SIZE], *S = buf, *T = buf + SIZE;
inline void flush() {
    fwrite(buf, 1, S - buf, stdout);
    S = buf;
}
inline void putchar(char c) {
    *S++ = c;
    if (S == T) flush();
}
struct NTR {
    ~NTR() { flush(); }
} ztr;
}  // namespace Fwrite
#ifdef ONLINE_JUDGE
#define getchar Fread ::getchar
#define putchar Fwrite ::putchar
#endif
namespace Fastio {
struct Reader {
    template <typename T>
    Reader& operator>>(T& x) {
        char c = getchar();
        T f = 1;
        while (c < '0' || c > '9') {
            if (c == '-') f = -1;
            c = getchar();
        }
        x = 0;
        while (c >= '0' && c <= '9') {
            x = x * 10 + (c - '0');
            c = getchar();
        }
        x *= f;
        return *this;
    }
    Reader& operator>>(char& c) {
        c = getchar();
        while (c == ' ' || c == '\n') c = getchar();
        return *this;
    }
    Reader& operator>>(char* str) {
        int len = 0;
        char c = getchar();
        while (c == ' ' || c == '\n') c = getchar();
        while (c != ' ' && c != '\n' && c != '\r') {  // \r\n in windows
            str[len++] = c;
            c = getchar();
        }
        str[len] = '\0';
        return *this;
    }
    Reader() {}
} cin;
const char endl = '\n';
struct Writer {
    template <typename T>
    Writer& operator<<(T x) {
        if (x == 0) {
            putchar('0');
            return *this;
        }
        if (x < 0) {
            putchar('-');
            x = -x;
        }
        static int sta[45];
        int top = 0;
        while (x) {
            sta[++top] = x % 10;
            x /= 10;
        }
        while (top) {
            putchar(sta[top] + '0');
            --top;
        }
        return *this;
    }
    Writer& operator<<(char c) {
        putchar(c);
        return *this;
    }
    Writer& operator<<(char* str) {
        int cur = 0;
        while (str[cur]) putchar(str[cur++]);
        return *this;
    }
    Writer& operator<<(const char* str) {
        int cur = 0;
        while (str[cur]) putchar(str[cur++]);
        return *this;
    }
    Writer() {}
} cout;
}  // namespace Fastio

const int QUERY = 3, SWAP_A = 1, SWAP_B = 2;

using Query = array<int, 4>;
using u64 = uint64_t;

Query read() {
    int op, u, v, w;
    cin >> op >> u >> v;
    if (op == 3) cin >> w;
    return {op, u - 1, v - 1, w};
}

template <typename Func>
void for_each_elem(u64 S, Func f) {
    for (int k; S;) k = __builtin_ctzll(S), S ^= 1ull << k, f(k);
}

template <u32 C>
struct graph {
    u32 n;
    vector<u32> u, v, deg;
    vector<u64> grf;
    bool built = false;

    graph(u32 V, u32 E) : n(V), deg(V + 1) { u.reserve(E), v.reserve(E), grf.reserve(E); }

    void add_edge(u32 x, u32 y, bool directed = true) {
        u.emplace_back(x), v.emplace_back(y), deg[x]++;
        if (!directed) add_edge(y, x);
    }

    void build() {
        u32 sum = 0;
        auto fn = [&](u32 j) { sum += deg[j], deg[j] = sum; };
        u32 i;
        for (i = 0; i + 4 < n; i += 4) fn(i), fn(i + 1), fn(i + 2), fn(i + 3);
        for (; i < n; i++) fn(i);
        u32 E = u.size();
        grf.resize(E);
#pragma GCC unroll 4
        for (i = 0; i < E; i++) grf[--deg[u[i]]] = v[i];
        deg[n] = E;
        built = true;
    }

    vector<u32> topo_sort() {
        u32 E = u.size();
        if (!built) build();
        vector<u32> res, in(n);
        for (u32 i = 0; i < E; i++) in[grf[i]]++;
        queue<u32> q;
        for (u32 i = 0; i < n; i++)
            if (in[i] == 0) q.push(i);
        while (!q.empty()) {
            u32 x = q.front();
            q.pop(), res.push_back(x);
            for (u32 i = deg[x]; i < deg[x + 1]; i++) {
                u32 y = grf[i];
                if (--in[y] == 0) q.push(y);
            }
        }
        return res;
    }
};

void solve() {
    int n, m, q;
    cin >> n >> m >> q;

    graph<0> g(n, m);
    for (int i = 0, u, v; i < m; ++i) cin >> u >> v, g.add_edge(u - 1, v - 1);
    g.build();

    vector<int> a(n), b(n);
    vector<Query> queries(q);

    for (int& i : a) cin >> i, i--;
    for (int& i : b) cin >> i, i--;
    for (Query& query : queries) query = read();

    auto process = [&](vector<Query>& sub_queries) {
        vector<u64> F(n), G(n + 1);
        vector<int> ans, b_inv(n);
        int all_queries =
            count_if(sub_queries.begin(), sub_queries.end(), [](auto& query) { return query[0] == QUERY; });
        ans.resize(all_queries, -1);
        for (int i = 0; i < n; ++i) b_inv[b[i]] = i;
        vector<int> modified;
        int query_cnt = 0;
        u64 pend = 0;
        for (auto [op, u, v, w] : sub_queries)
            if (op == QUERY)
                F[u] |= (1ull << query_cnt), G[v] |= (1ull << query_cnt), G[w] |= (1ull << query_cnt),
                    pend |= (1ull << query_cnt), query_cnt++;
            else
                b_inv[b[u]] = -1, b_inv[b[v]] = -1, modified.push_back(u), modified.push_back(v);
        for (int i = 0; i < n; i++)
            if (F[i])
                for (int j = g.deg[i]; j < g.deg[i + 1]; j++) F[g.grf[j]] |= F[i];
        partial_sum(G.begin(), G.end(), G.begin(), bit_xor<>{});
        query_cnt = 0;
        for (int i = n - 1; i >= 0; i--)
            if (b_inv[i] != -1) {
                int u = b_inv[i];
                u64 S = F[u] & G[a[u]] & pend;
                for_each_elem(S, [&](int k) { ans[k] = i, pend ^= 1ull << k; });
            }
        for (auto [op, u, v, w] : sub_queries)
            if (op == QUERY) {
                for (auto i : modified)
                    if (v <= a[i] && a[i] < w && (F[i] >> query_cnt & 1)) ans[query_cnt] = max(ans[query_cnt], b[i]);
                query_cnt++;
            } else if (op == SWAP_A)
                swap(a[u], a[v]);
            else
                swap(b[u], b[v]);
        return ans;
    };

    vector<Query> sub_queries;
    int query_cnt = 0;
    for (auto q : queries) {
        sub_queries.push_back(q);
        if (q[0] == QUERY) query_cnt++;
        if (sub_queries.size() == 200 || query_cnt == 64) {
            auto ans = process(sub_queries);
            for (int i : ans) cout << i + 1 << "\n";
            sub_queries.clear(), query_cnt = 0;
        }
    }
    if (!sub_queries.empty()) {
        auto ans = process(sub_queries);
        for (int i : ans) cout << i + 1 << "\n";
    }
}

int main() {
    int c, t;
    for (cin >> c >> t; t--;) solve();
}

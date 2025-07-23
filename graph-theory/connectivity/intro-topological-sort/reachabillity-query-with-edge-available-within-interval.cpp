/**
 * Original Problem: https://qoj.ac/problem/4420
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

template <u32 C>
struct graph {
    u32 n;
    vector<u32> u, v, deg;
    vector<u64> w;
    vector<u64> grf;
    bool built = false;

    graph(u32 V, u32 E) : n(V), deg(V + 1) { u.reserve(E), v.reserve(E), w.reserve(E), grf.reserve(E); }

    void add_edge(u32 x, u32 y, u64 z = 0, bool directed = true) {
        u.emplace_back(x), v.emplace_back(y), w.emplace_back(z), deg[x]++;
        if (!directed) add_edge(y, x, z);
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
        for (i = 0; i < E; i++) grf[--deg[u[i]]] = ((u64)w[i] << C) | v[i];
        deg[n] = E;
        built = true;
    }
};

void solve() {
    u32 n, m, q;
    cin >> n >> m >> q;
    graph<32> g(n, m);
    for (u32 i = 0, x, y; i < m; i++) cin >> x >> y, g.add_edge(x - 1, y - 1, i);
    g.build();
    vector<u64> need(n), avl(m + 1);
    vector<u32> to;
    to.reserve(64);

    auto solve_block = [&]() {
        for (u32 i = 0; i < m; i++) avl[i + 1] ^= avl[i];
        for (u32 i = 0; i < n; i++)
            for (u32 j = g.deg[i]; j < g.deg[i + 1]; j++) {
                u32 v = g.grf[j] & ((1ull << 32) - 1), id = g.grf[j] >> 32;
                need[v] |= (need[i] & avl[id]);
            }
        for (u32 i = 0; i < to.size(); i++) cout << (((need[to[i]] >> i) & 1) ? "YES\n" : "NO\n");
        fill(need.begin(), need.end(), 0), fill(avl.begin(), avl.end(), 0), to.clear();
    };

    for (u32 i = 0, x, y, l, r; i < q; i++) {
        u32 h = to.size();
        cin >> x >> y >> l >> r, x--, y--, l--;
        avl[l] ^= (u64(1) << h), avl[r] ^= (u64(1) << h), need[x] ^= (u64(1) << h), to.emplace_back(y);
        if (h == 63) solve_block();
    }
    solve_block();
}

int main() {
    int t;
    for (cin >> t; t; t--) solve();
}

/*
 * Oringal Problem:
 * https://www.luogu.com.cn/problem/P1954
*/

#include <bits/stdc++.h>

using namespace std;

using u32 = uint32_t;
using u64 = uint64_t;

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
};

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

#define cin Fastio ::cin
#define cout Fastio ::cout
#define endl Fastio ::endl

struct custom_bitset {
    vector<u64> v;
    u32 n;
    custom_bitset(u32 N) : n(N) { v.resize((n + 63) >> 6); }
    bool get(u32 i) const { return v[i / 64] >> (i % 64) & 1; }
    void set(u32 i, bool value) { v[i / 64] &= ~(1LLU << (i % 64)), v[i / 64] |= u64(value) << (i % 64); }
    custom_bitset& operator|=(const custom_bitset& rhs) {
        for (u32 i = 0; i < v.size(); i++) v[i] |= rhs.v[i];
        return *this;
    }
};

int main() {
    u32 n, m;
    cin >> n >> m;
    vector<u32> a(n), in(n);
    graph<0> g(n, m);
    for (u32& i : a) cin >> i;
    for (u32 _ = 0, x, y; _ < m; _++) cin >> x >> y, x--, y--, g.add_edge(y, x), in[x]++;
    g.build();

    priority_queue<u64> q;
    for (u32 i = 0; i < n; i++)
        if (in[i] == 0) q.push(i | (u64(a[i]) << 32));
    vector<u32> ord;
    ord.reserve(n);
    while (!q.empty()) {
        u64 u = q.top();
        q.pop(), ord.push_back(u & 0xFFFFFFFF);
        u32 y = u & 0xFFFFFFFF;
        for (u32 i = g.deg[y]; i < g.deg[y + 1]; i++) {
            u32 z = g.grf[i];
            if (--in[z] == 0) q.push(z | (u64(a[z]) << 32));
        }
    }

    vector<custom_bitset> f(n, n);
    for (u32 x : ord) {
        f[x].set(x, true);
        for (u32 i = g.deg[x]; i < g.deg[x + 1]; i++) f[g.grf[i]] |= f[x];
    }

    auto calc = [&](u32 i) {
        u32 c = n;
        for (u32 x : ord)
            if (!f[x].get(i)) {
                if (a[x] < c) return c;
                c--;
            }
        return c;
    };

    vector<u32> ans(n);
    for (u32 i = 0; i < n; i++) ans[i] = calc(i);
    reverse(ord.begin(), ord.end());
    for (u32 i = 0; i < n; i++) cout << ord[i] + 1 << " ";
    cout << endl;
    for (u32 i = 0; i < n; i++) cout << ans[i] << " ";
    cout << endl;
}

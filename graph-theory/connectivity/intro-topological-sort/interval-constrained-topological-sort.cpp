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
#define cin Fastio ::cin
#define cout Fastio ::cout
#define endl Fastio ::endl

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

int main() {
    u32 n, m;
    cin >> n >> m;
    graph<0> g(n, m);
    vector<u32> l(n), r(n);
    for (u32 i = 0, u, v; i < m; i++) cin >> u >> v, g.add_edge(u - 1, v - 1);
    for (u32 i = 0; i < n; i++) cin >> l[i] >> r[i], l[i]--;
    auto ord = g.topo_sort();
    if (ord.size() != n) return cout << "No" << endl, 0;
    for (u32 x : ord)
        for (u32 i = g.deg[x]; i < g.deg[x + 1]; i++) l[g.grf[i]] = max(l[g.grf[i]], l[x] + 1);
    reverse(ord.begin(), ord.end());
    for (u32 x : ord)
        for (u32 i = g.deg[x]; i < g.deg[x + 1]; i++) r[x] = min(r[x], r[g.grf[i]] - 1);
    graph<0> to_r(n + 1, n);
    for (u32 i = 0; i < n; i++) to_r.add_edge(r[i], i);
    to_r.build();
    set<u32> S = {0};
    vector<u32> ans(n);
    for (u32 i = 1; i <= n; i++) {
        for (u32 j = to_r.deg[i]; j < to_r.deg[i + 1]; j++) {
            u32 x = to_r.grf[j];
            auto it = S.lower_bound(l[x]);
            if (it == S.end()) return cout << "No" << endl, 0;
            ans[x] = *it, S.erase(it);
        }
        S.insert(i);
    }
    cout << "Yes" << endl;
    for (u32 x : ans) cout << x + 1 << " ";
}

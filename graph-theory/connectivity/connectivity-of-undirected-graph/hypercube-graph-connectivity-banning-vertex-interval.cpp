#include <bits/stdc++.h>
using namespace std;

using u32 = uint32_t;
using u64 = uint64_t;

struct dsu {
    vector<int> parsz;
    dsu(u32 n) : parsz(n, -1) {}
    u32 find(u32 x) { return parsz[x] < 0 ? x : parsz[x] = find(parsz[x]); }
    void merge(u32 x, u32 y) {
        x = find(x), y = find(y);
        if (x == y) return;
        if (parsz[x] > parsz[y]) swap(x, y);
        parsz[x] += parsz[y], parsz[y] = x;
    }
    bool same(u32 x, u32 y) { return find(x) == find(y); }
};

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

    u32 begin(u32 k) { return deg[k]; }
    u32 end(u32 k) { return deg[k + 1]; }
    u32 operator[](u32 k) { return grf[k]; }
};

u64 trunc(u64 x, u32 bit) { return (x >> bit) << bit; }

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
    Reader& operator>>(string& str) {
        char c = getchar();
        while (c == ' ' || c == '\n') c = getchar();
        str = "";
        while (c != ' ' && c != '\n' && c != '\r') {  // \r\n in windows
            str += c;
            c = getchar();
        }
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

int main() {
    u32 n, m;
    cin >> n >> m;
    vector<u64> vt = {0, u64(1) << n};
    vt.reserve(4 * (m + 2));
    vector<array<u64, 3>> queries(m);
    for (auto& [x, y, z] : queries) {
        string s;
        cin >> s >> y >> z, x = (s == "ask");
        if (!x) z++, vt.push_back(y), vt.push_back(z);
    }
    sort(vt.begin(), vt.end()), vt.erase(unique(vt.begin(), vt.end()), vt.end());
    for (u32 i = 1, m = vt.size(); i < m; i++)
        if (vt[i] - vt[i - 1] > 1) vt.push_back(trunc(vt[i] - 1, __lg(vt[i - 1] ^ (vt[i] - 1))));
    sort(vt.begin(), vt.end()), vt.erase(unique(vt.begin(), vt.end()), vt.end());

    for (auto& [x, y, z] : queries)
        y = distance(vt.begin(), prev(upper_bound(vt.begin(), vt.end(), y))),
        z = distance(vt.begin(), prev(upper_bound(vt.begin(), vt.end(), z)));

    u32 k = vt.size() - 1;
    graph<0> g(k, k * n);

    auto add = [&](u32 i, u32 j, u32 x) {
        u64 l = max(vt[i], vt[j] - (u64(1) << x)), r = min(vt[i + 1], vt[j + 1] - (u64(1) << x)) - 1;
        u64 lx = l >> x, rx = r >> x, p = lx ^ rx;
        if ((~lx & 1) || (~rx & 1) || (p != 0)) g.add_edge(i, j, false);
    };

    for (u32 i = 0; i < n; i++)
        for (u32 j0 = 0, j1 = 0; j0 < k; j0++) {
            while (j1 < k && vt[j1 + 1] <= vt[j0] + (u64(1) << i)) j1++;
            while (j1 < k && vt[j1 + 1] <= vt[j0 + 1] + (u64(1) << i)) add(j0, j1++, i);
            if (j1 < k && vt[j1] < vt[j0 + 1] + (u64(1) << i)) add(j0, j1, i);
        }
    g.build();

    vector<bool> vis(k, true);
    for (auto [x, y, z] : queries)
        if (x == 0)
            for (u32 j = y; j < z; j++) vis[j] = false;

    dsu uf(k);
    for (u32 i = 0; i < k; i++)
        for (u32 j = g.begin(i); j < g.end(i); j++)
            if (vis[i] && vis[g[j]]) uf.merge(i, g[j]);

    vector<bool> ans;
    ans.reserve(m);
    reverse(queries.begin(), queries.end());
    for (auto [x, y, z] : queries) {
        if (x)
            ans.push_back(uf.same(y, z));
        else {
            for (u32 j = y; j < z; j++) {
                vis[j] = true;
                for (u32 k = g.begin(j); k < g.end(j); k++)
                    if (vis[g[k]]) uf.merge(j, g[k]);
            }
        }
    }
    reverse(ans.begin(), ans.end());
    for (bool x : ans) cout << x << endl;
}

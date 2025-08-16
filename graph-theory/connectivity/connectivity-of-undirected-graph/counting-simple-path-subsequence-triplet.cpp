/*
 * Original Problem: https://www.luogu.com.cn/problem/P4630
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
        for (i = 0; i + 4 < n; i += 4) fn(i), fn(i + 1), fn(i + 2), fn(i + 3);
        for (; i < n; i++) fn(i);

        u32 V = u.size();
        data.resize(V);
#pragma GCC unroll 4
        for (u32 j = 0; j < V; j++) data[--deg[u[j]]] = v[j];
        deg.back() = V;
    }

    template <class _Callable>
    void for_each(u32 x, _Callable&& fn) {
        for (u32 i = deg[x]; i < deg[x + 1]; i++) fn(data[i]);
    }

    u32 size(u32 x) { return deg[x + 1] - deg[x]; }
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

struct block_cut {
    double_vector<u32> grf;
    vector<u32> dfn, low;
    stack<u32> stk;
    u32 stamp = 0, id, n;

    struct result_t {
        double_vector<u32> tr;

        result_t(u32 n) : tr(double_vector<u32>(n, n - 1)) {}
        void add(u32 x, u32 y) { tr.push_back(x, y), tr.push_back(y, x); }
        void build() { tr.build(); }
    } result = result_t(1);

    block_cut(u32 n, u32 exE = 0) : grf(n, exE) { dfn.resize(n), low.resize(n), id = n, this->n = n; }

    void add(u32 x, u32 y) { grf.push_back(x, y), grf.push_back(y, x); }

    void tarjan(u32 x, u32 fa) {
        dfn[x] = low[x] = ++stamp;
        stk.push(x);
        grf.for_each(x, [&](u32 y) {
            if (y == fa) return;
            if (!dfn[y]) {
                tarjan(y, x);
                low[x] = min(low[x], low[y]);
                if (low[y] >= dfn[x]) {
                    u32 z;
                    do z = stk.top(), stk.pop(), result.add(id, z);
                    while (z != y);
                    result.add(id, x), id++;
                }
            } else
                low[x] = min(low[x], dfn[y]);
        });
    }

    void run() {
        grf.build();
        result = result_t(n * 2);
        for (u32 i = 0; i < n; i++)
            if (!dfn[i]) tarjan(i, i);
        result.build();
    }
};

struct dsu {
    vector<int> fa;
    dsu(int n) : fa(n) { iota(fa.begin(), fa.end(), 0); }
    int find(int x) { return fa[x] == x ? x : fa[x] = find(fa[x]); }
    void merge(int x, int y) { fa[find(x)] = find(y); }
    bool same(int x, int y) { return find(x) == find(y); }
};

int main() {
    u32 n, m;
    cin >> n >> m;
    block_cut bc(n, m);
    for (u32 x, y, i = 0; i < m; i++) cin >> x >> y, bc.add(x - 1, y - 1);
    bc.run();
    u32 v = bc.id;
    auto& tr = bc.result.tr;
    vector<i64> weight(v, -1);
    dsu uf(v);
    for (u32 i = n; i < v; i++) weight[i] = tr.size(i);
    for (u32 i = 0; i < v; i++) tr.for_each(i, [&](u32 j) { uf.merge(i, j); });
    vector<i64> siz_comp(v);
    for (u32 i = 0; i < n; i++) siz_comp[uf.find(i)]++;
    vector<i64> siz(v);
    vector<bool> vis(v, false);

    auto dfs = [&](auto&& slf, u32 u, u32 fr) -> i64 {
        vis[u] = true, siz[u] = (u < n);
        i64 res = 0;
        tr.for_each(u, [&](u32 v) {
            if (v == fr) return;
            res += slf(slf, v, u), res += 2 * weight[u] * siz[u] * siz[v], siz[u] += siz[v];
        });
        return res + 2 * weight[u] * siz[u] * (siz_comp[uf.find(u)] - siz[u]);
    };

    i64 ans = 0;
    for (u32 i = 0; i < v; i++)
        if (!vis[i]) ans += dfs(dfs, i, i);
    cout << ans << endl;
}

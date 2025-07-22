/**
 * Original Problem Link: https://www.luogu.com.cn/problem/P3573
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

template <typename T, std::size_t N>
class aligned_allocator {
   public:
    using value_type = T;

    aligned_allocator() noexcept = default;

    template <class U>
    aligned_allocator(const aligned_allocator<U, N>&) noexcept {}

    T* allocate(std::size_t n) {
        if (n == 0) return nullptr;
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
            throw std::bad_alloc();
        }

        void* p = aligned_alloc(N, n * sizeof(T));
        if (p == nullptr) {
            throw std::bad_alloc();
        }
        return static_cast<T*>(p);
    }

    void deallocate(T* p, std::size_t n) noexcept { std::free(p); }

    template <typename U>
    struct rebind {
        using other = aligned_allocator<U, N>;
    };

    bool operator==(const aligned_allocator& other) const noexcept { return true; }
    bool operator!=(const aligned_allocator& other) const noexcept { return false; }
};

struct max_set {
    vector<u32, aligned_allocator<u32, 64>> tr;
    u32 m;
    max_set(u32 n) : m(2 << __lg(max(n, u32(1)))) { tr.resize(m); }
    void mod(u32 p, int v) {
        for (p = 2 * m - 1 - p; p > 1; p >>= 1) {
            if (p % 2 == 0) tr[p / 2] += v;
        }
    }
    u32 get_max() {
        u32 i = 1;
        while (i < m) {
            __builtin_prefetch(tr.data() + (i << 4));
            i = i * 2 + (tr[i] == 0);
            // cout << i << " " << tr[i] << endl;
        }
        return 2 * m - 1 - i;
    }
    void add(u32 p) { mod(p, 1); }
    void rem(u32 p) { mod(p, -1); }
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

int main() {
    u32 n, m;
    cin >> n >> m;
    vector g(2, graph<0>(n, m));
    for (u32 i = 0, x, y; i < m; i++) cin >> x >> y, g[0].add_edge(x - 1, y - 1), g[1].add_edge(y - 1, x - 1);

    auto res = g[0].topo_sort();
    g[1].build();
    vector<u32> start(n), end(n);
    for (u32 i : res)
        for (u32 j = g[0].deg[i]; j < g[0].deg[i + 1]; j++) end[g[0].grf[j]] = max(end[g[0].grf[j]], end[i] + 1);
    reverse(res.begin(), res.end());
    for (u32 i : res)
        for (u32 j = g[1].deg[i]; j < g[1].deg[i + 1]; j++) start[g[1].grf[j]] = max(start[g[1].grf[j]], start[i] + 1);
    reverse(res.begin(), res.end());
    max_set s(n);
    for (u32 x : start) s.add(x);
    u64 ans = (u64(s.get_max()) << u64(32));
    for (u32 i = 0; i < n; i++) {
        u64 mask = (u64(1) << u64(32)) - 1;
        u32 u = res[i];
        s.rem(start[u]);
        for (u32 j = g[1].deg[u]; j < g[1].deg[u + 1]; j++) s.rem(start[u] + end[g[1].grf[j]] + 1);
        ans = min(ans, (u64(s.get_max()) << u64(32)) + u);
        for (int j = g[0].deg[u]; j < g[0].deg[u + 1]; j++) s.add(end[u] + start[g[0].grf[j]] + 1);
        s.add(end[u]);
    }
    u64 mask = (u64(1) << u64(32)) - 1;
    cout << ((ans & mask) + 1) << " " << (ans >> 32) << endl;
}

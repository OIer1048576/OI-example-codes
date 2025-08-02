/*
 * Original Problem: https://www.luogu.com.cn/problem/P7054
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

template <class T>
using min_heap = priority_queue<T, vector<T>, greater<T>>;
template <class T>
using max_heap = priority_queue<T>;

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
    u32 n, m, k;
    cin >> n >> m >> k;
    double_vector<u32> grf(n, m);
    vector<u32> deg(n);
    for (u32 i = 0, u, v; i < m; i++) cin >> u >> v, grf.push_back(u - 1, v - 1), deg[v - 1]++;
    grf.build();
    max_heap<u32> P;
    min_heap<u32> Q;
    for (u32 i = 0; i < n; i++)
        if (deg[i] == 0) Q.push(i);

    vector<pair<int, int>> ans;
    vector<u32> ant;
    ans.reserve(k);

    for (int lst = -1, x; !P.empty() || !Q.empty(); lst = x) {
        while (!Q.empty() && k) {
            int u = Q.top(), v = -1;
            if (!P.empty()) v = P.top();
            if (u > v && Q.size() == 1) break;
            Q.pop(), P.push(u), k--;
        }
        if (!Q.empty())
            x = Q.top(), Q.pop();
        else {
            x = P.top(), P.pop();
            if (lst != -1) ans.push_back({lst, x});
        }
        ant.push_back(x);
        grf.for_each(x, [&](u32 v) {
            deg[v]--;
            if (deg[v] == 0) Q.push(v);
        });
    }
    for (auto i : ant) cout << i + 1 << " ";
    cout << endl;
    cout << ans.size() << endl;
    for (auto [ix, iy] : ans) cout << ix + 1 << " " << iy + 1 << endl;
}

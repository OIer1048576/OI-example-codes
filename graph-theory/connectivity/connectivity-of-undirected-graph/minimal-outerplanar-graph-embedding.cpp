/**
 * Original Problem: https://www.luogu.com.cn/problem/P11832
 */

#include <bits/stdc++.h>
using namespace std;

using i64 = int64_t;
using i32 = int32_t;
using u64 = uint64_t;
using u32 = uint32_t;

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

void solve() {
    /* Tarjan 不标准, 我有玉玉症 */

    u32 n, m;
    cin >> n >> m;

    vector<vector<u32>> g(n), bct(n + m), sub(n + m);
    u32 timer = 0, bct_id = n;
    vector<int> dfn(n), low(n), bal(n), stk;
    vector<int> nxt(n + m, n + m);

    auto link = [&](const vector<array<u32, 2>>& segs) -> array<u32, 2> {
        if (segs.empty()) return {n + m, n + m};
        for (int i = 1; i < segs.size(); i++) nxt[segs[i - 1][1]] = segs[i][0];
        return {segs.front()[0], segs.back()[1]};
    };

    for (u32 i = 0, u, v; i < m; i++) cin >> u >> v, g[u - 1].push_back(v - 1), g[v - 1].push_back(u - 1);

    auto tarjan = [&](auto&& slf, u32 v, u32 fr) -> void {
        dfn[v] = low[v] = ++timer, stk.push_back(v);
        u32 low_ref = v;
        for (int w : g[v]) {
            if (w == fr) continue;
            if (!dfn[w]) {
                slf(slf, w, v);
                low[v] = min(low[v], low[w]), bal[v] += bal[w];
                if (low[w] >= dfn[v]) {
                    u32 id = bct_id++, top;
                    bct[v].push_back(id);
                    do top = stk.back(), stk.pop_back(), bct[id].push_back(top);
                    while (top != w);
                }
                if (bal[w] < 2) sub[v].push_back(w), sub[w].push_back(v);
            } else if (dfn[w] < dfn[low_ref])
                low_ref = w;
        }
        if (dfn[low_ref] < low[v])
            bal[v]++, bal[low_ref]--, sub[v].push_back(low_ref), sub[low_ref].push_back(v), low[v] = dfn[low_ref];
    };

    vector<bool> vis(n + m);

    auto build_pair = [&](auto&& slf, u32 u) -> array<u32, 2> {
        vector<array<u32, 2>> segs = {{u, u}};
        for (u32 v : bct[u]) {
            for (u32 w : bct[v]) vis[w] = true;
            int p_s = -1;

            for (u32 w : bct[v])
                if (find(sub[w].begin(), sub[w].end(), u) != sub[w].end()) p_s = w;
            vector<array<u32, 2>> block;
            while (p_s != -1) {
                vis[p_s] = false, block.push_back(slf(slf, p_s));
                int c_p = exchange(p_s, -1);
                for (u32 w : sub[c_p])
                    if (vis[w]) p_s = w;
            }
            if (block.empty()) continue;
            if (block.front()[0] > block.back()[0]) reverse(block.begin(), block.end());  // 顺时针 / 逆时针
            segs.push_back(link(block));
        }
        sort(segs.begin(), segs.end());
        return link(segs);
    };

    vector<array<u32, 2>> ans;
    for (u32 i = 0; i < n; i++)
        if (!dfn[i]) tarjan(tarjan, i, -1), ans.push_back(build_pair(build_pair, i));

    reverse(ans.begin(), ans.end());

    auto print = [&](auto&& slf, u32 lim) -> void {
        while (!ans.empty() && ans.back()[0] < lim) {
            auto [u, v] = ans.back();
            for (ans.pop_back(); u != n + m; u = nxt[u]) {
                slf(slf, u), cout << u + 1 << " ";
                if (u == v) break;
            }
        }
    };
    print(print, n);
    cout << endl;
}

int main() {
    int id, t;
    for (cin >> id >> t; t; t--) solve();
}

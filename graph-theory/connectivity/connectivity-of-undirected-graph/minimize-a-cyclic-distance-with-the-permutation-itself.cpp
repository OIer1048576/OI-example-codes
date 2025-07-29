/**
 * Original Problem: https://luogu.com.cn/problem/CF1685D2
 */

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

void solve() {
    u32 n;
    cin >> n;
    vector<u32> p(n);
    for (u32& i : p) cin >> i, i--;
    vector<u32> cid(n, n);
    u32 ccnt = 0;
    for (u32 i = 0; i < n; i++)
        if (cid[i] == n) {
            for (u32 j = i; cid[j] == n; j = p[j]) cid[j] = ccnt;
            ccnt++;
        }

    auto check = [&](u32 x, vector<u32> q) {
        dsu uf(ccnt);
        vector<int> L(n), R(n), T(n);
        for (u32 i = 1; i < x; i++) {
            u32 f = q[i - 1], g = p[q[i]];
            if (f < g) L[f]++, L[g]--;
            if (g < f) R[g]++, R[f]--;
        }
        partial_sum(L.begin(), L.end(), L.begin());
        partial_sum(R.begin(), R.end(), R.begin());
        for (u32 i = 0; i < n; i++)
            if (L[i] > 1 || R[i] > 1) return false;
        for (u32 i = 1; i < n; i++)
            if (L[i - 1] || R[i - 1]) {
                if (uf.same(cid[i - 1], cid[i])) return false;
                uf.merge(cid[i - 1], cid[i]);
            }

        for (u32 i = 1; i < x; i++) {
            int f = q[i - 1], g = p[q[i]];
            if (f <= g) L[g]++;
            if (g <= f) R[f]++;
            T[f]++, T[g]++;
        }
        for (u32 i = 0; i < n; i++)
            if (L[i] != 0 && R[i] != 0 && T[i] == 0) return false;
        for (u32 i = 1; i < n; i++)
            if (T[i - 1] < 2 && T[i] < 2) uf.merge(cid[i - 1], cid[i]);
        for (u32 i = 1; i < ccnt; i++)
            if (!uf.same(0, i)) return false;
        return true;
    };

    vector<u32> q(n);
    vector<bool> vis(n);
    for (u32 i = 0; i < n; i++) {
        for (q[i] = 0; q[i] < n; q[i]++)
            if (!vis[q[i]] && check(i + 1, q)) break;
        vis[q[i]] = true;
    }
    for (u32 i = 0; i < n; i++) cout << q[i] + 1 << " ";
    cout << endl;
}

int main() {
    int t;
    for (cin >> t; t; t--) solve();
}

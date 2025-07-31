/**
 * Original Problem: https://www.luogu.com.cn/problem/P1173
 */

#include <bits/stdc++.h>
using namespace std;

using u32 = uint32_t;
using u64 = uint64_t;
using i64 = int64_t;

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

struct rollback_dsu {
    vector<int> parent;
    vector<u32> size;
    vector<pair<u32, u32>> mod;
    rollback_dsu(u32 n = 0, u32 exp_modcnt = 0) { parent.reserve(n), size.reserve(n), mod.reserve(exp_modcnt); }
    void push() { parent.push_back(-1), size.push_back(1); }
    u32 find(u32 x) { return parent[x] < 0 ? x : find(parent[x]); }
    void merge(u32 x, u32 y, u64& F) {
        x = find(x), y = find(y);
        if (x == y) return;
        if (size[x] < size[y]) swap(x, y);
        F--, mod.push_back({x, y}), parent[y] = x, size[x] += size[y];
    }
    bool same(u32 x, u32 y) { return find(x) == find(y); }
    u32 timestamp() { return mod.size(); }
    void rollback() {
        auto [x, y] = mod.back();
        mod.pop_back(), parent[y] = -1, size[x] -= size[y];
    }
    void rollback_to(u32 t) {
        while (timestamp() > t) rollback();
    }
    u32 length() { return parent.size(); }
};

void solve() {
    u64 n, m, c;
    cin >> n >> m >> c;
    vector<pair<int, int>> pos(c);

    auto id = [&](int x, int y) { return x * m + y; };
    unordered_set<u64> blk;
    blk.reserve(c << 1);

    for (auto& [x, y] : pos) cin >> x >> y, x--, y--;

    const array<array<int, 2>, 8> d{{{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}}};

    auto valid = [&](int x, int y) { return 0 <= x && x < n && 0 <= y && y < m; };

    u64 V = n * m - c, E = (n - 1) * m + n * (m - 1), F = (n - 1) * (m - 1) + 1;
    for (auto [x, y] : pos) {
        for (u32 dir : {0, 1, 2, 3}) {
            int nx = x + d[dir][0], ny = y + d[dir][1];
            if (valid(nx, ny) && !blk.count(id(nx, ny))) E--;
        }
        blk.insert(id(x, y));
    }
    auto dsu = rollback_dsu(6 * c + 8, 6 * c + 8);
    dsu.push();

    unordered_map<u64, u32> mappings;
    mappings.reserve(6 * c + 8);

    auto get_node = [&](int x, int y) -> int {
        if (!valid(x, y) || !valid(x + 1, y + 1)) return 0;
        u64 key = id(x, y);
        if (mappings.count(key)) return mappings[key];
        mappings[key] = dsu.length(), dsu.push();
        return mappings[key];
    };

    auto D = [&](int x, int y, auto&& fn, u64& F) {
        dsu.merge(fn(x - 1, y), fn(x, y), F), dsu.merge(fn(x, y - 1), fn(x, y), F),
            dsu.merge(fn(x - 1, y - 1), fn(x - 1, y), F);
    };

    for (auto [x, y] : pos) D(x, y, get_node, F);

    if (V - E + F > 2) return cout << 0 << endl, void();
    if (V <= 2) return cout << -1 << endl, void();
    if (n == 1 || m == 1) return cout << 1 << endl, void();

    auto check = [&](int x, int y) {
        if (!valid(x, y) || blk.count(id(x, y))) return false;
        u64 Vv = V - 1, Ev = E, Fv = F, Lv = dsu.length();
        for (u32 dir : {0, 1, 2, 3}) {
            int nx = x + d[dir][0], ny = y + d[dir][1];
            if (valid(nx, ny) && !blk.count(id(nx, ny))) Ev--;
        }
        vector<u64> created;
        auto temp_node = [&](int ax, int ay) -> int {
            if (!valid(ax, ay) || !valid(ax + 1, ay + 1)) return 0;
            u64 key = id(ax, ay);
            if (mappings.count(key)) return mappings[key];
            created.push_back(key), mappings[key] = dsu.length(), dsu.push();
            return mappings[key];
        };

        u32 tick = dsu.timestamp();
        D(x, y, temp_node, Fv);
        if (Vv - Ev + Fv > 2) return true;
        dsu.rollback_to(tick);
        for (auto key : created) mappings.erase(key);
        dsu.parent.resize(Lv), dsu.size.resize(Lv);
        return false;
    };

    bool ok = false;
    for (auto [bx, by] : pos)
        for (auto [dx, dy] : d) ok |= check(bx + dx, by + dy);
    cout << (ok ? 1 : 2) << endl;
}

int main() {
    int t;
    for (cin >> t; t; t--) solve();
}

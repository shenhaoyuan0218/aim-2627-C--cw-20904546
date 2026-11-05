#include <climits>
#include <cstddef>
#include <iostream>
#include <string>

namespace aim::b1 { bool parse_i32(const std::string& text, int& output); }
namespace aim::b2 { std::size_t compact(int* data, std::size_t n, int low, int high); }

static void show_bool(const char* tag, bool got, int got_value, bool want, int want_value) {
    bool ok = (got == want) && (!want || got_value == want_value);
    std::cout << (ok ? "PASS  " : "FAIL  ") << tag
              << " = ret=" << (got ? "true" : "false") << ",output=" << got_value;
    if (!ok) std::cout << "  预期 ret=" << (want ? "true" : "false") << ",output=" << want_value;
    std::cout << "\n";
}

static void check_parse(const char* tag, std::string text, bool want, int want_value) {
    int out = 777;                       // 哨兵：失败时必须保持不变
    bool got = aim::b1::parse_i32(text, out);
    if (text.size() > 12) text = text.substr(0, 9) + "...(" + std::to_string(text.size()) + " chars)";
    show_bool(tag, got, out, want, want_value);
}

static void show_compact(const char* tag, std::size_t got, const int* buf, std::size_t got_n,
                         std::size_t want, const int* want_buf) {
    bool ok = (got == want);
    for (std::size_t k = 0; ok && k < want; ++k) ok = (buf[k] == want_buf[k]);
    std::cout << (ok ? "PASS  " : "FAIL  ") << tag << " = n=" << got << ",[";
    for (std::size_t k = 0; k < got; ++k) std::cout << (k ? "," : "") << buf[k];
    std::cout << "]";
    if (!ok) {
        std::cout << "  预期 n=" << want << ",[";
        for (std::size_t k = 0; k < want; ++k) std::cout << (k ? "," : "") << want_buf[k];
        std::cout << "]";
    }
    std::cout << "\n";
}

// 原地压缩会改写入参，用一块固定缓冲搬运输入
static int g_in[8];

static void run_compact(const char* tag, const int* input, std::size_t n, int low, int high,
                        std::size_t want, const int* want_buf) {
    for (std::size_t k = 0; k < 8; ++k) g_in[k] = (k < n) ? input[k] : -999;
    std::size_t got = aim::b2::compact(g_in, n, low, high);
    show_compact(tag, got, g_in, n, want, want_buf);
}

int main() {
    std::cout << "== B1 parse_i32 ==\n";
    check_parse("A 上端点 2147483647 ", "2147483647", true, 2147483647);
    check_parse("B 下端点 -2147483648", "-2147483648", true, INT_MIN);
    check_parse("C 超上界 2147483648 ", "2147483648", false, 777);
    check_parse("D 超下界 -2147483649", "-2147483649", false, 777);
    check_parse("E 空串              ", "", false, 777);
    check_parse("F 只有符号 '+'      ", "+", false, 777);
    check_parse("G 负零 -0000        ", "-0000", true, 0);
    check_parse("H 前导零 000123     ", "000123", true, 123);
    check_parse("I 前导空格 ' 12'    ", " 12", false, 777);
    check_parse("J 尾随空格 '12 '    ", "12 ", false, 777);
    check_parse("K 双符号 '+-1'      ", "+-1", false, 777);
    check_parse("L 小数点 '1.5'      ", "1.5", false, 777);
    check_parse("M 尾随字母 '12x'    ", "12x", false, 777);
    check_parse("N 1000 个 9         ", std::string(1000, '9'), false, 777);

    std::cout << "\n== B2 compact ==\n";
    { int in[] = {-3, 0, 5};   int w[] = {-3, 0, 5};
      run_compact("A 全部保留          ", in, 3, -3, 5, 3, w); }
    { int in[] = {5, 6, 7};    int w[1] = {0};
      run_compact("B 全部剔除          ", in, 3, 0, 4, 0, w); }
    { int in[] = {1, 2, 3};    int w[1] = {0};
      run_compact("C low>high 返回 0   ", in, 3, 5, 3, 0, w); }
    { int in[] = {0, 100, 50}; int w[] = {0, 100, 50};
      run_compact("D 端点含在内        ", in, 3, 0, 100, 3, w); }
    { int w[1] = {0};
      run_compact("E n=0 且 data=nullptr", nullptr, 0, 0, 1, 0, w); }
    { int in[] = {7, 1, 4, 2}; int w[] = {7, 4, 2};
      run_compact("F 保序压缩          ", in, 4, 2, 7, 3, w); }
    { int in[] = {9};          int w[] = {9};
      run_compact("G 单元素保留        ", in, 1, 9, 9, 1, w); }
    { int in[] = {9};          int w[1] = {0};
      run_compact("H 单元素剔除        ", in, 1, 0, 8, 0, w); }
    { int in[] = {-5, -3, -1}; int w[] = {-3};
      run_compact("I 负数区间          ", in, 3, -4, -2, 1, w); }
    { int in[] = {INT_MIN, 0, INT_MAX}; int w[] = {INT_MIN, 0, INT_MAX};
      run_compact("J 全值域端点        ", in, 3, INT_MIN, INT_MAX, 3, w); }
    { int in[] = {INT_MIN, 0, INT_MAX}; int w[1] = {0};
      run_compact("K 全值域反序        ", in, 3, INT_MAX, INT_MIN, 0, w); }
    { int in[] = {4, 4, 4};    int w[] = {4, 4, 4};
      run_compact("L 全部相同值        ", in, 3, 4, 4, 3, w); }
    return 0;
}

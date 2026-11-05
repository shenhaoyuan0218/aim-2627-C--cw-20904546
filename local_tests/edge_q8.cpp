#include <iostream>
#include <vector>
namespace aim::q8 {
struct Telemetry { int id; int hp; };
struct Summary { int valid; int duplicate; int invalid; long long total_hp; std::vector<int> ids; };
Summary summarize(const std::vector<Telemetry>& frames);
}
static void show(const char* tag, aim::q8::Summary r, aim::q8::Summary e) {
    bool ok = r.valid==e.valid && r.duplicate==e.duplicate && r.invalid==e.invalid
              && r.total_hp==e.total_hp && r.ids==e.ids;
    std::cout << (ok ? "PASS  " : "FAIL  ") << tag
              << " = {v=" << r.valid << ",d=" << r.duplicate << ",i=" << r.invalid
              << ",hp=" << r.total_hp << ",ids=[";
    for (size_t k = 0; k < r.ids.size(); ++k) std::cout << (k ? "," : "") << r.ids[k];
    std::cout << "]}";
    if (!ok) {
        std::cout << "  预期 {v=" << e.valid << ",d=" << e.duplicate << ",i=" << e.invalid
                  << ",hp=" << e.total_hp << ",ids=[";
        for (size_t k = 0; k < e.ids.size(); ++k) std::cout << (k ? "," : "") << e.ids[k];
        std::cout << "]}";
    }
    std::cout << "\n";
}
int main() {
    using T = aim::q8::Telemetry; using S = aim::q8::Summary;
    // A 编号端点：0 和 9999 都合法
    show("A 编号端点0/9999     ", aim::q8::summarize({{0,10},{9999,20}}), S{2,0,0,30,{0,9999}});
    // B 编号越界两侧
    show("B 编号越界-1/10000   ", aim::q8::summarize({{-1,10},{10000,20}}), S{0,0,2,0,{}});
    // C hp 端点在界内
    show("C hp端点-1000/1000   ", aim::q8::summarize({{1,-1000},{2,1000}}), S{2,0,0,100,{1,2}});
    // D hp 越界只差1
    show("D hp越界-1001/1001   ", aim::q8::summarize({{1,-1001},{2,1001}}), S{0,0,2,0,{}});
    // E 已见编号又来一个非法包 → 计 invalid 不是 duplicate
    show("E 已见编号+非法包    ", aim::q8::summarize({{1,10},{1,5000}}), S{1,0,1,10,{1}});
    // F 同一编号全是非法包 → 编号从未被占用
    show("F 非法不占编号       ", aim::q8::summarize({{1,-2000},{1,30}}), S{1,0,1,30,{1}});
    // G 乱序编号 → ids 必须升序
    show("G 乱序编号排序       ", aim::q8::summarize({{9,1},{2,1},{7,1},{4,1}}), S{4,0,0,4,{2,4,7,9}});
    // H 空批次
    show("H 空批次             ", aim::q8::summarize({}), S{0,0,0,0,{}});
    // I 夹取边界：hp=100 与 101、-1 与 0
    show("I hp夹取[0,100]      ", aim::q8::summarize({{1,100},{2,101},{3,0},{4,-1}}), S{4,0,0,200,{1,2,3,4}});
    // J 同编号三条合法 → 1 valid + 2 duplicate
    show("J 三条同编号         ", aim::q8::summarize({{5,10},{5,20},{5,30}}), S{1,2,0,10,{5}});
    return 0;
}

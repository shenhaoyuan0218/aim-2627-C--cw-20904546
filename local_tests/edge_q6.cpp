#include <iostream>
#include <vector>
namespace aim::q6 {
struct Target { int id; int x; int y; int hp; bool visible; };
int select_target(const std::vector<Target>& targets, int max_range);
}
int main() {
    using T = aim::q6::Target;
    // 目标 9 站在射程边界：距离 = 1000000 = max_range，应该被选中
    std::cout << "边界目标: " << aim::q6::select_target({{9,1000000,0,10,true}}, 1000000) << "  (预期 9)\n";
    // 射程内但坐标大: (999999,0)
    std::cout << "射程内大坐标: " << aim::q6::select_target({{3,999999,0,10,true}}, 1000000) << "  (预期 3)\n";
    // 平局三级比较: 同 hp 同距离, id 小的赢
    std::cout << "平局比 id: " << aim::q6::select_target({{7,3,4,20,true},{2,0,5,20,true}}, 5) << "  (预期 2)\n";
    // 全部不可见
    std::cout << "全不可见: " << aim::q6::select_target({{1,0,0,10,false}}, 5) << "  (预期 -1)\n";
    // hp=0 算死人
    std::cout << "hp为0: " << aim::q6::select_target({{1,0,0,0,true}}, 5) << "  (预期 -1)\n";
    // 负射程
    std::cout << "负射程: " << aim::q6::select_target({{1,0,0,10,true}}, -3) << "  (预期 -1)\n";
    return 0;
}

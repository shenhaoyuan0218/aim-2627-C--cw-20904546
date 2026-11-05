#include <iostream>
#include <vector>
namespace aim::q7 {
struct FireResult { int shots; int rejected; int ammo; int heat; };
FireResult simulate(int ammo, int heat, int cool, int shot_heat, const std::vector<int>& requests);
}
static void show(const char* name, aim::q7::FireResult r, aim::q7::FireResult e) {
    std::cout << name << " = {" << r.shots << "," << r.rejected << "," << r.ammo << "," << r.heat
              << "}  预期 {" << e.shots << "," << e.rejected << "," << e.ammo << "," << e.heat << "}\n";
}
int main() {
    using F = aim::q7::FireResult;
    show("A 空请求不冷却      ", aim::q7::simulate(4,80,10,30,{}),                {0,0,4,80});
    show("B 无弹也先冷却      ", aim::q7::simulate(0,50,10,20,{1}),              {0,1,0,40});
    show("C 零冷却零射击热量  ", aim::q7::simulate(1,100,0,0,{1}),               {1,0,0,100});
    show("D 冷却后恰好100允许 ", aim::q7::simulate(1,90,0,10,{1}),              {1,0,0,100});
    show("E 超1就拒绝         ", aim::q7::simulate(1,91,0,10,{1}),              {0,1,1,91});
    show("F 拒绝帧也冷却      ", aim::q7::simulate(0,50,10,20,{1,1}),            {0,2,0,30});
    show("G 请求0不计rejected ", aim::q7::simulate(5,50,10,20,{0}),              {0,0,5,40});
    show("H 先歇一帧再开火    ", aim::q7::simulate(2,100,30,10,{0,1}),           {1,0,1,50});
    return 0;
}

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// Assessment 01 Q6; see 题面.pdf.
namespace aim::q6 {
struct Target { int id; int x; int y; int hp; bool visible; };

// BEGIN IMPLEMENTATION
int select_target(const std::vector<Target>& targets, int max_range){
    if (max_range < 0) {
        return -1;
    }
    bool      has_best = false;           // 有没有找到过候选
    long long best_hp  = 0;
    long long best_d   = 0;
    int       best_id  = 0;
    for (const Target& t : targets){
        if (!t.visible){
            continue;
        }
        if (t.hp <= 0){
            continue;
        }
        long long dx = t.x;
        long long dy = t.y;
        long long d = dx * dx + dy * dy;
        long long r = max_range;
        if (d > r * r){
            continue;
        }

    bool is_better;
    if (!has_best){
        is_better = true;
    }else if (t.hp > best_hp){
        is_better = true;
    } else if (t.hp == best_hp && d < best_d){
        is_better = true;
    } else if (t.hp == best_hp && d == best_d && t.id < best_id){
        is_better = true;
    } else {
        is_better = false;
    }
    if (is_better){
        has_best = true;
        best_hp  = t.hp;
        best_d   = d;
        best_id  = t.id;
    }
    }
    return has_best ? best_id : -1;
    
}

// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q6;
int main() {
    try {
        { CHECK(select_target({{7,3,4,20,true},{2,0,5,20,true}},5)==2); }
        { CHECK(select_target({},5)==-1); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}

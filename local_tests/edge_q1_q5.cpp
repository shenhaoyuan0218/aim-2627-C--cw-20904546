#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <limits>

namespace aim::q1 {
long long ratio(long long hp, long long max_hp);
std::string report(int id, long long hp, long long max_hp, int battery);
}
namespace aim::q2 {
struct LabelEdit { std::string from; std::string to; };
std::string edit_labels(std::string labels, const std::vector<LabelEdit>& edits);
}
namespace aim::q3 {
std::vector<int> alarms(const std::vector<int>& values, int k, long long threshold);
}
namespace aim::q4 {
struct CommandResult { long long value; int accepted; int rejected; };
CommandResult execute(const std::vector<std::string>& lines);
}
namespace aim::q5 {
std::vector<std::vector<int>> risk(const std::vector<std::string>& grid);
}

using namespace aim;

static void show(const std::vector<int>& v) {
    std::cout << "{";
    for (std::size_t i = 0; i < v.size(); ++i) std::cout << (i ? "," : "") << v[i];
    std::cout << "}";
}

static void show(const std::vector<std::vector<int>>& m) {
    std::cout << "{";
    for (std::size_t i = 0; i < m.size(); ++i) {
        if (i) std::cout << ",";
        std::cout << "{";
        for (std::size_t j = 0; j < m[i].size(); ++j) std::cout << (j ? "," : "") << m[i][j];
        std::cout << "}";
    }
    std::cout << "}";
}

static void show(const q4::CommandResult& r) {
    std::cout << "{" << r.value << "," << r.accepted << "," << r.rejected << "}";
}

int main() {
    std::cout << "--- Q1 ---\n";
    std::cout << "ratio(-5,10)      = " << q1::ratio(-5, 10) << "\n";
    std::cout << "ratio(15,10)      = " << q1::ratio(15, 10) << "\n";
    std::cout << "ratio(5,0)        = " << q1::ratio(5, 0) << "\n";
    std::cout << "ratio(0,0)        = " << q1::ratio(0, 0) << "\n";
    std::cout << "ratio(1,3)        = " << q1::ratio(1, 3) << "\n";
    std::cout << "ratio(-5,-10)     = " << q1::ratio(-5, -10) << "\n";
    std::cout << "report(42,-3,100,0)   = " << q1::report(42, -3, 100, 0) << "\n";
    std::cout << "report(7,50,100,20)   = " << q1::report(7, 50, 100, 20) << "\n";
    std::cout << "report(7,50,100,19)   = " << q1::report(7, 50, 100, 19) << "\n";
    std::cout << "report(7,50,100,60)   = " << q1::report(7, 50, 100, 60) << "\n";
    std::cout << "report(7,500,100,150) = " << q1::report(7, 500, 100, 150) << "\n";
    std::cout << "report(7,50,100,-5)   = " << q1::report(7, 50, 100, -5) << "\n";
    std::cout << "report(7,50,0,80)     = " << q1::report(7, 50, 0, 80) << "\n";

    std::cout << "\n--- Q2 ---\n";
    {
        using E = std::vector<q2::LabelEdit>;
        std::cout << "(\"cam,cam2\",{{cam,camera}})          = " << q2::edit_labels("cam,cam2", E{{"cam","camera"}}) << "\n";
        std::cout << "(\"cam,cam2\",{{cam,}})                = " << q2::edit_labels("cam,cam2", E{{"cam",""}}) << "\n";
        std::cout << "(\"imu,cam\",{{cam,}})                 = " << q2::edit_labels("imu,cam", E{{"cam",""}}) << "\n";
        std::cout << "(\"cam\",{{cam,}})                     = " << q2::edit_labels("cam", E{{"cam",""}}) << "\n";
        std::cout << "(\"\",{{,imu}})                        = " << q2::edit_labels("", E{{"","imu"}}) << "\n";
        std::cout << "(\"\",{{,}})                           = [" << q2::edit_labels("", E{{"",""}}) << "]\n";
        std::cout << "(\"a,b\",{{z,q}})                      = " << q2::edit_labels("a,b", E{{"z","q"}}) << "\n";
        std::cout << "(\"camera,cam\",{{cam,xxx}})           = " << q2::edit_labels("camera,cam", E{{"cam","xxx"}}) << "\n";
        std::cout << "(\"cam,cam\",{{cam,}})                 = " << q2::edit_labels("cam,cam", E{{"cam",""}}) << "\n";
        std::cout << "(\"cam\",{{cam,},{,imu}})              = " << q2::edit_labels("cam", E{{"cam",""},{"","imu"}}) << "\n";
        std::cout << "(\"cam,imu\",{{cam,x},{imu,y}})        = " << q2::edit_labels("cam,imu", E{{"cam","x"},{"imu","y"}}) << "\n";
    }

    std::cout << "\n--- Q3 ---\n";
    std::cout << "alarms({},2,0)          = "; show(q3::alarms({}, 2, 0)); std::cout << "\n";
    std::cout << "alarms({1,2},0,0)       = "; show(q3::alarms({1,2}, 0, 0)); std::cout << "\n";
    std::cout << "alarms({1,2},3,0)       = "; show(q3::alarms({1,2}, 3, 0)); std::cout << "\n";
    std::cout << "alarms({2,2},2,4)       = "; show(q3::alarms({2,2}, 2, 4)); std::cout << "\n";
    std::cout << "alarms({2,2},2,3)       = "; show(q3::alarms({2,2}, 2, 3)); std::cout << "\n";
    std::cout << "alarms({-5,-5},2,-11)   = "; show(q3::alarms({-5,-5}, 2, -11)); std::cout << "\n";
    std::cout << "alarms({1,2,3,4},4,10)  = "; show(q3::alarms({1,2,3,4}, 4, 10)); std::cout << "\n";
    std::cout << "alarms({1,2,3,4},2,4)   = "; show(q3::alarms({1,2,3,4}, 2, 4)); std::cout << "\n";
    {
        std::vector<int> big(200000, 1000000000);
        auto r = q3::alarms(big, 200000, 1);
        std::cout << "alarms(200000*1e9, k=200000, 1) = size " << r.size();
        if (!r.empty()) std::cout << " last=" << r.back();
        std::cout << "\n";
    }

    std::cout << "\n--- Q4 ---\n";
    std::cout << "{\"SET 1000000\",\"ADD 1\"}  = "; show(q4::execute({"SET 1000000","ADD 1"})); std::cout << "\n";
    std::cout << "{\"SET 999999\",\"ADD 1\"}   = "; show(q4::execute({"SET 999999","ADD 1"})); std::cout << "\n";
    std::cout << "{\"ADD 5x\"}                = "; show(q4::execute({"ADD 5x"})); std::cout << "\n";
    std::cout << "{\"ADD 1.5\"}               = "; show(q4::execute({"ADD 1.5"})); std::cout << "\n";
    std::cout << "{\"ADD 5 6\"}               = "; show(q4::execute({"ADD 5 6"})); std::cout << "\n";
    std::cout << "{\"RESET 5\"}               = "; show(q4::execute({"RESET 5"})); std::cout << "\n";
    std::cout << "{\"   \"}                   = "; show(q4::execute({"   "})); std::cout << "\n";
    std::cout << "{\"add 5\"}                 = "; show(q4::execute({"add 5"})); std::cout << "\n";
    std::cout << "{\"MUL 3\"}                 = "; show(q4::execute({"MUL 3"})); std::cout << "\n";
    std::cout << "{\"ADD\"}                   = "; show(q4::execute({"ADD"})); std::cout << "\n";
    std::cout << "{\"SET -1000000\",\"ADD -1\"}= "; show(q4::execute({"SET -1000000","ADD -1"})); std::cout << "\n";
    std::cout << "{\"ADD 1000000\",\"ADD 1000000\"} = "; show(q4::execute({"ADD 1000000","ADD 1000000"})); std::cout << "\n";
    std::cout << "{\"RESET\"}                 = "; show(q4::execute({"RESET"})); std::cout << "\n";
    std::cout << "{\"SET 5\",\"ADD -5\"}       = "; show(q4::execute({"SET 5","ADD -5"})); std::cout << "\n";

    std::cout << "\n--- Q5 ---\n";
    std::cout << "risk({})            = "; show(q5::risk({})); std::cout << "\n";
    std::cout << "risk({\".\"})         = "; show(q5::risk({"."})); std::cout << "\n";
    std::cout << "risk({\"#\"})         = "; show(q5::risk({"#"})); std::cout << "\n";
    std::cout << "risk({\"#.\"})        = "; show(q5::risk({"#."})); std::cout << "\n";
    std::cout << "risk({\"..\",\"..\"})   = "; show(q5::risk({"..",".."})); std::cout << "\n";
    std::cout << "risk({\"#.#\",\"...\",\"#.#\"}) = "; show(q5::risk({"#.#","...","#.#"})); std::cout << "\n";
    std::cout << "risk({\"...\",\".#.\",\"...\"}) = "; show(q5::risk({"...",".#.","..."})); std::cout << "\n";
    std::cout << "risk({\"#\",\".\"})      = "; show(q5::risk({"#","."})); std::cout << "\n";
    for (auto [name, g] : std::vector<std::pair<std::string, std::vector<std::string>>>{
             {"risk({\"\"})", {""}}, {"risk({\"..\",\".\"})", {"..", "."}}, {"risk({\"x\"})", {"x"}}}) {
        std::cout << name << "  = ";
        try { show(q5::risk(g)); std::cout << "\n"; }
        catch (const std::invalid_argument& e) { std::cout << "throw invalid_argument(" << e.what() << ")\n"; }
        catch (const std::exception& e) { std::cout << "throw " << e.what() << "\n"; }
    }
    return 0;
}

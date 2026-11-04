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

// Assessment 01 Q2; see 题面.pdf.
namespace aim::q2 {
struct LabelEdit { std::string from; std::string to; };


// BEGIN IMPLEMENTATION
std::string edit_labels(std::string labels, const std::vector<LabelEdit>& edits){
    for (const auto& edit : edits) {

        if (edit.from.empty()){
            
            if (!edit.to.empty()){
                if (labels.empty()){
                    labels += edit.to;
                } else {
                    labels += ",";
                    labels += edit.to;
                }
            }
        } else {
            
            std::string::size_type pos = labels.find(edit.from);
            while (pos != std::string::npos) {
                bool check_left  = (pos == 0 || labels[pos - 1] == ',');
                bool check_right = (pos + edit.from.size() == labels.size() ||
                                    labels[pos + edit.from.size()] == ',');
                if (check_left && check_right) {
                    break;
                }
                pos = labels.find(edit.from, pos + 1);
            }

            if (pos != std::string::npos){
                if (!edit.to.empty()){
                    
                    labels.replace(pos, edit.from.size(), edit.to);
                } else {
                    
                    std::string::size_type len = edit.from.size();
                    if (pos + len < labels.size()) {
                        labels.erase(pos, len + 1);
                    } else if (pos > 0) {
                        labels.erase(pos - 1, len + 1);
                    } else {
                        labels.erase(pos, len);
                    }
                }
            }
        }
    }
    return labels;
}
// END IMPLEMENTATION
}


#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q2;
int main() {
    try {
        { CHECK(edit_labels("cam,imu,radar",{{"imu","gyro"},{"cam",""},{"","lidar"}})=="gyro,radar,lidar"); }
        { CHECK(edit_labels("cam,cam2,cam",{{"cam","camera"},{"cam",""}})=="camera,cam2"); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}

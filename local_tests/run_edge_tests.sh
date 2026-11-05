#!/usr/bin/env bash
# 运行 README「新增边界测试」表中记录的全部用例（Q1–Q8、B1、B2）。
# 用法：bash local_tests/run_edge_tests.sh
# 各题源文件里固定的 main 会被改名成 orig_main_qN，避免与测试程序的 main 冲突。
set -u

CXX=${CXX:-clang++}
ROOT=$(cd "$(dirname "$0")/.." && pwd)
cd "$ROOT" || exit 1
BUILD=local_tests/.build
mkdir -p "$BUILD"

# 1) 八道题与 B1/B2 的实现分别编成目标文件（不改动源文件）
for q in 1 2 3 4 5 6 7 8; do
    $CXX -std=c++17 -Dmain=orig_main_q$q -c "src/main/A1_q$q.cpp" -o "$BUILD/q$q.o" \
        || { echo "编译失败：src/main/A1_q$q.cpp"; exit 1; }
done
for b in b1 b2; do
    $CXX -std=c++17 -Dmain=orig_main_$b -c "src/main/A1_$b.cpp" -o "$BUILD/$b.o" \
        || { echo "编译失败：src/main/A1_$b.cpp"; exit 1; }
done

# 2) 链接测试程序：Q1–Q5 合一个，Q6/Q7/Q8 各一个
$CXX -std=c++17 local_tests/edge_q1_q5.cpp \
    "$BUILD/q1.o" "$BUILD/q2.o" "$BUILD/q3.o" "$BUILD/q4.o" "$BUILD/q5.o" \
    -o "$BUILD/t_q1_q5" || exit 1
for q in 6 7 8; do
    $CXX -std=c++17 "local_tests/edge_q$q.cpp" "$BUILD/q$q.o" -o "$BUILD/t_q$q" || exit 1
done
$CXX -std=c++17 local_tests/edge_b1_b2.cpp "$BUILD/b1.o" "$BUILD/b2.o" \
    -o "$BUILD/t_b1_b2" || exit 1

# 3) 依次运行
"$BUILD/t_q1_q5"
for q in 6 7 8; do "$BUILD/t_q$q"; done
"$BUILD/t_b1_b2"

echo
echo "以上为实测输出，可与 README 表格中的「预期」逐行对照。"

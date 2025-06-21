#!/bin/bash

# P2765 魔术球问题 - 所有解法测试脚本
# 编译并测试三种不同的算法解法

echo "🎓 千禧年科技学院 - P2765 魔术球问题测试"
echo "=================================================="

# 测试用例
TEST_CASES=("3" "4" "5")
EXPECTED=("7" "11" "17")

# 编译所有解法
echo "📦 编译所有解法..."

echo "  编译贪心算法解法..."
g++ -o greedy_solution greedy_solution.cpp -std=c++17
if [ $? -ne 0 ]; then
    echo "❌ 贪心算法编译失败"
    exit 1
fi

echo "  编译二分+二分图匹配解法..."
g++ -o binary_search_solution binary_search_solution.cpp -std=c++17
if [ $? -ne 0 ]; then
    echo "❌ 二分搜索算法编译失败"
    exit 1
fi

echo "  编译增量式二分图匹配解法..."
g++ -o incremental_matching_solution incremental_matching_solution.cpp -std=c++17
if [ $? -ne 0 ]; then
    echo "❌ 增量式匹配算法编译失败"
    exit 1
fi

echo "✅ 所有解法编译成功！"
echo ""

# 测试所有解法
for i in "${!TEST_CASES[@]}"; do
    n=${TEST_CASES[$i]}
    expected=${EXPECTED[$i]}
    
    echo "🧪 测试案例: n=$n (期望答案: $expected 个球)"
    echo "----------------------------------------"
    
    # 测试贪心算法
    echo "  🎯 贪心算法:"
    result=$(echo "$n" | ./greedy_solution | head -1)
    if [ "$result" == "$expected" ]; then
        echo "    ✅ 通过 (结果: $result)"
    else
        echo "    ❌ 失败 (期望: $expected, 实际: $result)"
    fi
    
    # 测试二分搜索算法
    echo "  🔍 二分+二分图匹配:"
    result=$(echo "$n" | ./binary_search_solution | head -1)
    if [ "$result" == "$expected" ]; then
        echo "    ✅ 通过 (结果: $result)"
    else
        echo "    ❌ 失败 (期望: $expected, 实际: $result)"
    fi
    
    # 测试增量式匹配算法
    echo "  ⚡ 增量式二分图匹配:"
    result=$(echo "$n" | ./incremental_matching_solution | head -1)
    if [ "$result" == "$expected" ]; then
        echo "    ✅ 通过 (结果: $result)"
    else
        echo "    ❌ 失败 (期望: $expected, 实际: $result)"
    fi
    
    echo ""
done

echo "🎉 测试完成！"
echo ""
echo "📂 文件结构:"
echo "  📄 README.md                           - 详细题解文档"
echo "  🟢 greedy_solution.cpp                 - 贪心算法 O(M*N)"
echo "  🔵 binary_search_solution.cpp          - 二分+二分图匹配 O(log(M)*M^3)"
echo "  🟡 incremental_matching_solution.cpp   - 增量式二分图匹配 O(M^2*N)"
echo ""
echo "💡 算法比较:"
echo "  - 贪心算法：最简单，效率最高，适合在线计算"
echo "  - 二分搜索：在贪心基础上优化搜索过程"
echo "  - 增量匹配：理论最优，适合深入研究"

# 清理编译产物
rm -f greedy_solution binary_search_solution incremental_matching_solution

echo ""
echo "🌸 がんばって！(加油！) - 千禧年科技学院算法竞赛部" 
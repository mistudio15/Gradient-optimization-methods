#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

std::vector<std::vector<int>> m = {{1, 2, 3}, {4, 5, 6}};
std::vector<std::vector<int>> m2 = {{10, 20, 30}, {40, 50, 60}};
std::vector<std::vector<int>> r = {{0, 0, 0}, {0, 0, 0}};

int main()
{
    std::copy(&m[0], &m[m.size() - 1], &r[0]);
    std::cout << (m2[0].data()) + 1 << " " << m2[1].data() << std::endl;
    // std::transform(&m[0], &m[m.size() - 1], &m2[0], &r[0], std::minus<>{});
}
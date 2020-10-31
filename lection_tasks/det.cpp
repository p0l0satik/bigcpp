#include<array>
#include<utility>
#include<iostream>
constexpr int coef(int i){
    return (i) % 2 == 0 ? 1 : -1;
}

template<int N>
constexpr std::array<std::array<int, N - 1>, N - 1>
get_submatrix(const std::array<std::array<int, N>, N>& a, int i, int j) {
    std::array<std::array<int, N - 1>, N - 1> r{};
    for (int ii = 0; ii != N - 1; ++ii)
        for (int jj = 0; jj != N - 1; ++jj)
            (&std::get<0>(((&std::get<0>(r))[ii])))[jj] = a[ii + (ii >= i ? 1 : 0)][jj + (jj >= j ? 1 : 0)];
    return r;
}

template <int N>
constexpr int det(const std::array<std::array<int, N>, N>& a){
    int determ = 0;
    for (size_t i = 0u; i < N; ++i){
        determ += coef(i) * a[i][0] * det<N-1>(get_submatrix<N>(a, i, 0));
    }
    return determ;
}
template<>

constexpr int det<1>(const std::array<std::array<int, 1>, 1>& a){
    return a[0][0];
}


int main() {
    const std::array<std::array<int, 4>, 4> A = {{
        {5, 1, 23, 1}
       ,{1, 2, 3, 1}
       ,{2, 12, 7, 1}
       ,{4, 5, 7, 12}
    }};
    const std::array<std::array<int, 2>, 2> A1 = {{
        {1, 2}
       ,{3, 4}
    }};
    constexpr int res = det<4>(A);
    std::cout << res << std::endl;
}

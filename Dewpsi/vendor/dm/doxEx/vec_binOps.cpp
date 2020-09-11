template<length_t L, typename T> Vec<L, T> operator+(const Vec<L, T>& lhs, const Vec<L, T>& rhs);
template<length_t L, typename T> Vec<L, T> operator+(const Vec<L, T>& lhs, T rhs);
template<length_t L, typename T> Vec<L, T> operator+(T rhs, const Vec<L, T>& lhs);

template<length_t L, typename T> Vec<L, T> operator-(const Vec<L, T>& lhs, const Vec<L, T>& rhs);
template<length_t L, typename T> Vec<L, T> operator-(const Vec<L, T>& lhs, T rhs);
template<length_t L, typename T> Vec<L, T> operator-(T rhs, const Vec<L, T>& lhs);

template<length_t L, typename T> Vec<L, T> operator*(const Vec<L, T>& lhs, const Vec<L, T>& rhs);
template<length_t L, typename T> Vec<L, T> operator*(const Vec<L, T>& lhs, T rhs);
template<length_t L, typename T> Vec<L, T> operator*(T rhs, const Vec<L, T>& lhs);

template<length_t L, typename T> Vec<L, T> operator/(const Vec<L, T>& lhs, const Vec<L, T>& rhs);
template<length_t L, typename T> Vec<L, T> operator/(const Vec<L, T>& lhs, T rhs);
template<length_t L, typename T> Vec<L, T> operator/(T rhs, const Vec<L, T>& lhs);

template<length_t L, typename T> Vec<L, T> operator%(const Vec<L, T>& lhs, const Vec<L, T>& rhs);
template<length_t L, typename T> Vec<L, T> operator%(const Vec<L, T>& lhs, T rhs);
template<length_t L, typename T> Vec<L, T> operator%(T rhs, const Vec<L, T>& lhs);

#pragma once

#include <cstdint>
#include <initializer_list>
#include <type_traits> // std::enable_if

namespace math {

    constexpr double PI = 3.1415926535897932385;

    // generic math

    template<class T>
    inline T max(T x, T y) {
        return x >= y ? x : y;
    }

    template<class T>
    inline T min(T x, T y) {
        return x <= y ? x : y;
    }

    template <typename T>
    T clamp(T x, T min, T max) {
        static_assert(std::is_arithmetic<T>::value, "clamp requires a numeric type");

        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    namespace _internal {
        template<size_t N, class T>
        struct raw_vec {
        public:
            T e[N];
            
            constexpr raw_vec() = default;

            // Constructor taking exactly N arguments of type T
            template <typename... Args,
                    typename = std::enable_if_t<sizeof...(Args) == N &&
                                                (std::conjunction_v<std::is_convertible<Args, T>...>)>>
            constexpr raw_vec(Args&&... args) : e{ static_cast<T>(std::forward<Args>(args))... } {}


            T& operator[](size_t i) {
                return e[i];
            }

            const T& operator[](size_t i) const {
                return e[i];
            }

            // convenience x,y,z,w functions

            template<size_t M = N>
            std::enable_if_t<(M >= 1), T&> x() { return e[0]; }

            template<size_t M = N>
            std::enable_if_t<(M >= 1), const T&> x() const { return e[0]; }

            template<size_t M = N>
            std::enable_if_t<(M >= 1), T&> y() { return e[0]; }

            template<size_t M = N>
            std::enable_if_t<(M >= 2), const T&> y() const { return e[1]; }

            template<size_t M = N>
            std::enable_if_t<(M >= 1), T&> z() { return e[0]; }

            template<size_t M = N>
            std::enable_if_t<(M >= 3), const T&> z() const { return e[2]; }

            template<size_t M = N>
            std::enable_if_t<(M >= 1), T&> w() { return e[0]; }

            template<size_t M = N>
            std::enable_if_t<(M >= 4), const T&> w() const { return e[3]; }
        };

    };

    template<size_t N, class T>
    struct vec : public _internal::raw_vec<N, T> {
        static constexpr size_t size() { return N; }

        constexpr vec() = default;

        // Constructor taking exactly N arguments of type T
        template <typename... Args,
                typename = std::enable_if_t<sizeof...(Args) == N &&
                                            (std::conjunction_v<std::is_convertible<Args, T>...>)>>
        constexpr vec(Args&&... args) : _internal::raw_vec<N, T>( static_cast<T>(std::forward<Args>(args))... ) {}


        vec operator+(vec that) const {
            vec res;
            for (size_t i = 0; i < N; i++) {
                res[i] = (*this)[i] + that[i];
            }
            return res;
        }

        vec operator-() const {
            vec res;
            for (size_t i = 0; i < N; i++) {
                res[i] = -(*this)[i];
            }
            return res;
        }

        vec operator-(vec that) {
            return operator+(-that);
        }

        vec operator*(T s) const {
            vec res;
            for (size_t i = 0; i < N; i++) {
                res[i] = s * (*this)[i];
            }
            return res;
        }

        vec operator/(T s) const {
            vec res;
            for (size_t i = 0; i < N; i++) {
                res[i] = (*this)[i] / s;
            }
            return res;
        }

        T length_sq() const {
            return dot(*this, *this);
        }

        T length() const {
            return sqrt(length_sq());
        }

        vec normalized() const {
            return *this / length();
        }
    };

    template<size_t N, class T>
    void operator+=(vec<N, T>& a, vec<N, T> b) {
        a = a + b;
    }
    template<size_t N, class T>
    void operator-=(vec<N, T>& a, vec<N, T> b) {
        a = a - b;
    }

    template<size_t N, class T>
    void operator*=(vec<N, T>& a, T s) {
        a = a * s;
    }
    template<size_t N, class T>
    void operator/=(vec<N, T>& a, T s) {
        a = a / s;
    }

    template<size_t N, class T>
    vec<N, T> operator*(T s, vec<N, T> v) {
        return v * s;
    }

    template<size_t N, class T>
    T dot(vec<N, T> a, vec<N, T> b) {
        T acc(0);

        for (size_t i = 0; i < N; i++) {
            acc += a[i] * b[i];
        }

        return acc;
    }


    template<size_t N, class T>
    struct point : public _internal::raw_vec<N, T> {
        static constexpr size_t size() { return N; }

        constexpr point() = default;

        // Constructor taking exactly N arguments of type T
        template <typename... Args,
                typename = std::enable_if_t<sizeof...(Args) == N &&
                                            (std::conjunction_v<std::is_convertible<Args, T>...>)>>
        constexpr point(Args&&... args) : _internal::raw_vec<N, T>( static_cast<T>(std::forward<Args>(args))... ) {}


        point operator+(vec<N, T> v) const {
            point res;
            for (size_t i = 0; i < N; i++) {
                res[i] = (*this)[i] + v[i];
            }
            return res;
        }

        point operator-(vec<N, T> v) const {
            return operator+(-v);
        }

        vec<N, T> operator-(point p) const {
            vec<N, T> res;
            for (size_t i = 0; i < N; i++) {
                res[i] = (*this)[i] - p[i];
            }
            return res;
        }

        explicit operator vec<N, T>() const {
            vec<N, T> v;
            for (size_t i = 0; i < N; i++) {
                v[i] = (*this)[i];
            }
            return v;
        }
    };

    // Useful type definitions

    using vec2i = vec<2, uint32_t>;
    using vec2f = vec<2, float>;
    using vec3i = vec<3, uint32_t>;
    using vec3f = vec<3, float>;
    using vec4i = vec<4, uint32_t>;
    using vec4f = vec<4, float>;

    using point2i = point<2, uint32_t>;
    using point2f = point<2, float>;
    using point3i = point<3, uint32_t>;
    using point3f = point<3, float>;
    using point4i = point<4, uint32_t>;
    using point4f = point<4, float>;

    // Other math

    template<size_t N, class T>
    vec<N, T> lerp(T t, vec<N, T> from, vec<N, T> to) {
        return from + t * (to - from);
    }

    template<size_t N>
    bool is_zero(vec<N, float> v, float eta = 1e-8) {
        bool zero = true;
        for (size_t i = 0; i < N; i++) {
            zero &= abs(v[i]) < eta;
        }
        return zero;
    }
    
}

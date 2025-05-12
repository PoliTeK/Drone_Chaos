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

    // vectors

    template<size_t N, class T>
    struct vec {
    public:
        T e[N];

    public:
        vec() : vec(0) {}

        vec(T x) {
            for (size_t i = 0; i < N; i++) {
                e[i] = x;
            }
        }

        vec(std::initializer_list<T> list) {
            size_t i = 0;
            for (T v : list) {
                e[i] = v;
                i++;
            }
        }


        T& operator[](size_t i) {
            // assert(i < N && "index exceeds vector size");
            return e[i];
        }

        const T& operator[](size_t i) const {
            // assert(i < N && "index exceeds vector size");
            return e[i];
        }


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

        // convenience x,y,z,w functions

        template<size_t M = N>
        std::enable_if_t<(M >= 1), T> x() const { return e[0]; }

        template<size_t M = N>
        std::enable_if_t<(M >= 2), T> y() const { return e[1]; }

        template<size_t M = N>
        std::enable_if_t<(M >= 3), T> z() const { return e[2]; }

        template<size_t M = N>
        std::enable_if_t<(M >= 4), T> w() const { return e[3]; }
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
    struct point {
    public:
        T e[N];

    public:
        point() : point(0) {}

        point(T x) {
            for (size_t i = 0; i < N; i++) {
                e[i] = x;
            }
        }

        point(std::initializer_list<T> list) {
            size_t i = 0;
            for (T v : list) {
                e[i] = v;
                i++;
            }
        }


        T& operator[](size_t i) {
            // assert(i < N && "index exceeds point size");
            return e[i];
        }

        const T& operator[](size_t i) const {
            // assert(i < N && "index exceeds point size");
            return e[i];
        }

        
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
                v[i] = e[i];
            }
            return v;
        }

        // convenience x,y,z,w functions

        template<size_t M = N, std::enable_if_t<(M >= 1), bool> = true>
        T x() { return e[0]; }

        template<size_t M = N, std::enable_if_t<(M >= 2), bool> = true>
        T y() { return e[1]; }

        template<size_t M = N, std::enable_if_t<(M >= 3), bool> = true>
        T z() { return e[2]; }

        template<size_t M = N, std::enable_if_t<(M >= 4), bool> = true>
        T w() { return e[3]; }
    };


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

    //

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
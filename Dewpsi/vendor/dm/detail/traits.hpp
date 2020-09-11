#ifndef TRAITS_HPP__DEWPSI_INCLUDED
#define TRAITS_HPP__DEWPSI_INCLUDED

#include <type_traits>
#include <typeinfo>

namespace dm {
    template<typename Vector> struct VectorTraits;
    template<typename ValueType> struct TypeStringName;

    namespace std {
        using ::std::true_type;
        using ::std::false_type;
        using ::std::enable_if;
        using ::std::is_convertible;
        using ::std::is_floating_point;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Floating point type

    template<typename T>
    struct FloatType {
        //static_assert(false, "Invalid type: not a floating point!");
    };

    template<>
    struct FloatType<float> {
        typedef float type;
    };

    template<>
    struct FloatType<double> {
        typedef double type;
    };

    template<>
    struct FloatType<long double> {
        typedef long double type;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // Vector traits

    template<typename Vector>
    struct VectorTraits {
        typedef typename Vector::value_type value_type;
        typedef typename Vector::length_type length_type;
        static constexpr length_type length = Vector::Length();
        static const TypeStringName<value_type> name;
    };

    template<typename Vector>
    const TypeStringName<typename VectorTraits<Vector>::value_type> VectorTraits<Vector>::name;

    ////////////////////////////////////////////////////////////////////////////////
    // Type string name trait

    template<typename ValueType>
    struct TypeStringName {
        const char* name = "Unknown type";
    };

    template<>
    struct TypeStringName<int> {
        const char* name = "int";
    };
    template<>
    struct TypeStringName<unsigned int> {
        const char* name = "unsigned int";
    };

    template<>
    struct TypeStringName<short int> {
        const char* name = "short int";
    };
    template<>
    struct TypeStringName<unsigned short int> {
        const char* name = "unsigned short int";
    };

    template<>
    struct TypeStringName<char> {
        const char* name = "char";
    };
    template<>
    struct TypeStringName<unsigned char> {
        const char* name = "unsigned char";
    };

    template<>
    struct TypeStringName<long int> {
        const char* name = "long int";
    };
    template<>
    struct TypeStringName<unsigned long int> {
        const char* name = "unsigned long int";
    };

    template<>
    struct TypeStringName<float> {
        const char* name = "float";
    };

    template<>
    struct TypeStringName<double> {
        const char* name = "double";
    };
}

#endif /* TRAITS_HPP__DEWPSI_INCLUDED */

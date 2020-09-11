#ifndef TESTDEWPSIMATH_VEC2_HPP
#define TESTDEWPSIMATH_VEC2_HPP

#include <DewpsiMath.hpp>
#include <cstdio>
#include <cstarg>

static const char * _FmtStrings[] = {
    R"(### Testing vector constructors ###
Default constructor: %s
Constructor with %f: %s
Constructor with %f and %f: %s
)",
    ""
};

template<class Vector, class Logger>
void textOtherFunctions(const Vector& a, const Vector& b, Logger handle)
{
    const Vector diff = b - a;
    handle->info("### Testing other vector functions ###");
    handle->info("Difference between {} and {} = {}", a, b, diff);
    handle->info("Length of the difference vector : {}", diff.Length());
    handle->info("Normal-length vector: {}", dm::normalize(diff));
    handle->info("");
}

template<class Vector, typename Value, class Logger>
void testVectorBinOps(const Vector& a, const Vector& b, Value c, Logger handle)
{
    #define print(a, b, op) handle->info("{0} " #op " {1} = {2}", a, b, a op b);

    const auto& vecInfo = dm::VectorTraits<Vector>::name;

    {
        5;
    }

    handle->info("### Testing vector arithmetic for {0} ###", vecInfo.name);
    handle->info("-- Addition --");
    print(a, b, +);
    print(a, c, +);
    print(c, a, +);
    handle->info("");

    handle->info("-- Subtraction --");
    print(a, b, -);
    print(a, c, -);
    print(c, a, -);
    handle->info("");

    handle->info("-- Multiplication --");
    print(a, b, *);
    print(a, c, *);
    print(c, a, *);
    handle->info("");

    handle->info("-- Division --");
    print(a, b, /);
    print(a, c, /);
    print(c, a, /);
    handle->info("");

    handle->info("-- Modulus --");
    print(a, b, %);
    print(a, c, %);
    print(c, a, %);
    handle->info("");

    #undef print
}

template<class Vector, typename Value, class Logger>
void testVectorUnaryOps(Vector&& a, const Vector& b, Value c, Logger handle)
{
    #define print(a, b, op) { auto temp = a; \
                              handle->info("{0} " #op " {1} = {2}", temp, b, a op b); \
                          }

    const auto& vecInfo = dm::VectorTraits<Vector>::name;
    handle->info("### Testing compound assignment for {0} ###", vecInfo.name);

    handle->info("-- Addition --");
    print(a, b, +=);
    print(a, c, +=);
    handle->info("");

    handle->info("-- Subtraction --");
    print(a, b, -=);
    print(a, c, -=);
    handle->info("");

    handle->info("-- Multiplication --");
    print(a, b, *=);
    print(a, c, *=);
    handle->info("");

    handle->info("-- Division --");
    print(a, b, /=);
    print(a, c, /=);
    handle->info("");

    handle->info("-- Modulus --");
    print(a, b, %=);
    print(a, c, %=);
    handle->info("");

    #undef print
}

static int testVectors(const char* file)
{
    FILE* handle = fopen(file, "w");
    assert(handle);

    {
        dm::Vec2 vecA;
        dm::Vec2 vecB(0.5f);
        dm::Vec2 vecB(0.5f, 2.0f);
        std::string sVectors[] = {
            vecA.GetString(),
            vecB.GetString(),
            vecC.GetString()
        };

        fprintf(handle, _FmtStrings[0],
               sVectors[0].c_str(), 0.5f, sVectors[1].c_str(), 0.5f, 2.0f, sVectors[2].c_str());
    }

    testVectorBinOps(dm::Vec2(3.0f, 4.0f), dm::Vec2(3.0f, 4.0f), 5.0f, handle);
    testVectorBinOps(dm::DVec2(4.5, 49.5), dm::DVec2(4.5, 49.5), 5.0, handle);
    testVectorBinOps(dm::IVec2(4, 5), dm::IVec2(6, 7), 8, handle);
    testVectorBinOps(dm::UVec2(4, 5), dm::UVec2(3, 2), 5U, handle);

    testVectorUnaryOps(dm::Vec2(5.2f, 5.3f), dm::Vec2(1.8f, 1.7f), 0.5f, handle);
    testVectorUnaryOps(dm::DVec2(5.2, 5.3), dm::DVec2(1.8, 1.7), 0.5, handle);
    testVectorUnaryOps(dm::IVec2(5, 5), dm::IVec2(1, 1), 5, handle);
    testVectorUnaryOps(dm::UVec2(5, 5), dm::UVec2(1, 1), 5, handle);

    textOtherFunctions(dm::Vec2(1.0f), dm::Vec2(5.0f), handle);

    return 0;
}

#endif /* TESTDEWPSIMATH_VEC2_HPP */

#include "mathtests_core.h"

#define printexpr(e)        std::cout << #e " = " << (e) << std::endl;
#define printbool(expr, s)  std::cout << s ": " << (expr) << "\n";

int main(int argc, const char* argv[])
{
    auto vec1 = Dewpsi::MakeVector3D(5, 10, 0);
    auto vec2 = Dewpsi::MakeVector3D(6, 11, 8);
    
    // print vectors
    printexpr(vec1);
    printexpr(vec2);
    
    // basic arithmetic
    std::cout << vec2 << " + " << vec1 << " = " << (vec2 + vec1) << '\n';
    std::cout << vec2 << " - " << vec1 << " = " << (vec2 - vec1) << '\n';
    std::cout << vec2 << " * " << vec1 << " = " << (vec2 * vec1) << '\n';
    std::cout << vec2 << " / " << vec1 << " = " << (vec2 / vec1) << '\n';
    
    {
        auto temp = vec2;
        std::cout << temp << " += " << vec1 << " = " << (temp += vec1) << '\n';
        
        temp = vec2;
        std::cout << temp << " -= " << vec1 << " = " << (temp -= vec1) << '\n';
        
        temp = vec2;
        std::cout << temp << " *= " << vec1 << " = " << (temp *= vec1) << '\n';
        
        temp = vec2;
        std::cout << temp << " /= " << vec1 << " = " << (temp /= vec1) << std::endl;
    }
    
    // comparisons
    std::cout << std::boolalpha;
    printbool(vec1 < vec2, "vector A is less than vector B");
    printbool(vec1 <= vec2, "vector A is less than or equal to vector B");
    printbool(vec1 > vec2, "vector A is greater than vector B");
    printbool(vec1 >= vec2, "vector A is greater than or equal to vector B");
    printbool(vec1 == vec2, "vector A is equal to vector B");
    printbool(vec1 != vec2, "vector A is not equal to vector B");
    
    // complex math
    std::cout << "length of vector A = " << vec1.Length() \
    << "\nlength of vector B = " << vec2.Length() << '\n';
    
    std::cout << "normalized vector A: " << vec1.Normalized() \
    << "normalized vector B: " << vec2.Normalized() << std::endl;
    
    return 0;
}

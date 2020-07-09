#include <iostream>
#include <string>
#include <cstdlib>

/*namespace Dewpsi {
    template<typename T>
    class LogHex {
    public:
        LogHex(T val) : m_value(val)
        {  }
        
        T get() const { return m_value; }
        
    private:
        T m_value;
    };
    
    template<typename T>
    inline typename std::enable_if<std::is_arithmetic<T>::value, LogHex<T>>::type
    MakeLogHex(T val)
    {
        return LogHex<T>(val);
    }
}

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const Dewpsi::LogHex<T>& obj)
{
    os << "0x" << std::hex << obj.get() << std::dec;
}*/

#define space1      "    "
#define space2      "        "
#define space3      "            "
#define space(x)    space##x
//#define newl(x)     "\n" space(x)

#define newl(x, s)  "\n" space(x) s

int main(int argc, const char* argv[])
{
    using std::cout;
    using std::endl;
    
    
    cout << newl(1, "hi") << endl;
    
    return 0;
}

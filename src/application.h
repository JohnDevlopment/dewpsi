#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>

int main(int argc, const char** argv);

namespace Dewpsi {
    class Application {
        static Application* s_instance;
        bool m_bRunning;
        
        void Run();
        
        friend int ::main(int argc, const char** argv);
        
    public:
        Application(const std::string& sName = "Dewpsi App");
        virtual ~Application();
    };
    
    // implemented by the client
    Application* NewApplication();
}

#endif /* APPLICATION_H */


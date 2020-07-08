#ifndef APPLICATION_H
#define APPLICATION_H

/**
*   @file       application.h
*   @brief      A header for the Dewpsi engine.
*   @ingroup    core
*/

#include <string>
#include <window.h>
#include <core.h>

int main(int argc, const char** argv);

namespace Dewpsi {
    class WindowCloseEvent;
    
    /// The class that drives the client application.
    class Application {
    public:
        /// Initializes the application with a given name.
        Application(const std::string& sName = "Dewpsi App");
        virtual ~Application();
        
        /// Processes an event.
        void OnEvent(Event& e);
        
    private:
        static Application* s_instance;
        bool m_bRunning;
        
        Scope<Window> m_window;
        
        /// The main loop of the application; can only be called from main().
        void Run();
        
        /// Window close event callback.
        bool OnWindowClosed(WindowCloseEvent& e);
        
        friend int ::main(int argc, const char** argv);
    };
    
    /// Implemented by the client to create an instance of Application.
    Application* NewApplication();
    
    /// Set the window properties prior to the window creation.
    PD_CALL void SetWindowProps(const WindowProps&);
}

#endif /* APPLICATION_H */


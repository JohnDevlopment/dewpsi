#ifndef DEWPSI_APPLICATION_H
#define DEWPSI_APPLICATION_H

/**
*   @mainpage   %Dewpsi Engine
*   @tableofcontents
*   @section    intro_sec Introduction
*   This is an introduction.
*
*   @section    def_sec Definitions
*   A program that is built off of the %Dewpsi engine and uses its API
*   is the "Client".
*
*   @section    compile_sec Compiling A Client Program
*   The minimum code required in order to compile a client is the following.
*
*	The window is created by a call to Dewpsi::NewApplication(). Prior to that,
*	you must call Dewpsi::SetWindowProps() with a reference to Dewpsi::WindowProps.
*	Use this to set the properties of the window, including the initial position
*	and size of the window.
*
*   @snippet    client.cpp  Window Properties
*
*	See the documentation for @c %Dewpsi::WindowProps and %Dewpsi::SetWindowProps().
*	After the window properties are set, it is time to create the window. But
*	if the client wishes to use OpenGL on platforms that support it, add
*	WindowOpenGL to the list of flags passed to @a %WindowProps::flags. Then
*	set the OpenGL attributes with Dewpsi::SetWindowOpenGLAttribute().
*
*	@snippet    client.cpp  OpenGL Attributes
*
*   @subsection compile_sec_app_ssec Creating the Application
*   Finally, once all the options have been set, the window can be created. This
*	is done through Dewpsi::NewApplication(). The function is not implemented in
*	the library and must be implemented by the client. It returns a pointer to
*	a user-defined class that inherits from Dewpsi::Application.
*
*   @code
    Application* NewApplication(PDuserdata userdata);
*   @endcode
*
*   The function is responsible for allocating a pointer to the derived class --
*	let's say @c Sandbox.
*
*   The argument is a pointer to user-defined data that can then be passed
*   to the derived class as the client wills. It can be used, for example,
*	to hold parameters used to initialize the @c Sandbox application.
*
*   @section    flow_sec Flow of the Application
*   Calling Application::Run() initiates the main loop of the application.
*   (1) All the registered layers are processed in sequential order.
*/

/**
*   @file       Dewpsi_Application.h
*   @brief      @doxfb
*   @ingroup    core
*/

#include <Dewpsi_Core.h>
#include <Dewpsi_Window.h>
#include <Dewpsi_LayerStack.h>
#include <Dewpsi_Timestep.h>
#include <Dewpsi_Memory.h>
#include <string>

int main(int argc, const char** argv);

namespace Dewpsi {
    class WindowCloseEvent;
    class WindowResizeEvent;
    class ImGuiLayer;

    /// The class that drives the client application.
    class Application {
    public:
        /// Initializes the application with a given name.
        Application(const std::string& sName = "Dewpsi App");
        virtual ~Application();

        /// Processes an event.
        void OnEvent(Event& e);

        /// Pushes a layer onto the layer stack.
        void PushLayer(Layer* layer);

        /// Pushes a layer on top of the other layers in the layer stack.
        void PushOverlay(Layer* overlay);

        /// Returns a pointer to the window.
        Window& GetWindow()
        { return *m_window; }

        /// Returns a pointer to the application.
        static Application& Get()
        { return *s_instance; }

    private:
        static Application* s_instance;
        bool m_bRunning;
        LayerStack m_layerStack;
        float m_fLastFrameTime;

        Scope<Window> m_window;
        ImGuiLayer* m_guiLayer;

        /// The main loop of the application; can only be called from main().
        void Run();

        /// Window close event callback.
        bool OnWindowClosed(WindowCloseEvent& e);

        /// Window resize event callback.
        bool OnWindowResized(WindowResizeEvent& e);

        friend int ::main(int argc, const char** argv);

    protected:
        PDuserdata m_UserData; ///< User data (can be passed to the derived class' constructor)
    };

    /// Implemented by the client to create an instance of Application.
    Application* NewApplication(PDuserdata userdata = nullptr);

    /// Set the window properties prior to the window creation.
    PD_CALL void SetWindowProps(const WindowProps&);

    /// Returns the window properties last set by SetWindowProps().
    PD_CALL WindowProps GetWindowProperties();
}

#endif /* DEWPSI_APPLICATION_H */

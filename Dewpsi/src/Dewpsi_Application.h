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
*   @section    render_sec Setting Up The Renderer
*   @parblock
*   All these steps are done after creating the window.
*
*   The first step is to initialize the renderer by telling it what API to use. Then you
*   create the shader, as shown below:
*   @dontinclude render.cpp
*   @until delete
*
*   Dewpsi::Shader::Create() accepts two parameters, strings, that contain the source code of the
*   vertex shader and fragment shader. The function returns a pointer to the base class
*   Dewpsi::Shader. If the shader fails to compile, an exception is thrown and an error message
*   is printed to the standard error channel (see information on std::cerr or stderr). In any case,
*   if the function returns successfully then you have a valid shader object. The window must be
*   created before a shader can be compiled. That is because when the window is created in %Dewpsi,
*   a rendering context is also created. And this context loads the library the whatever API.
*
*   Now a note on shaders: The shader source code is in the language that the underlying API uses
*   for its shaders. For example, if using OpenGL the shader is written in GLSL.
*
*   The next step is creating the vertex array. Vertex arrays must be bound prior to making the
*   vertex and index buffers.
*   @skip Vertex array
*   @until Bind
*   Dewpsi::VertexArray::Create() accepts no parameters and returns a pointer to the base class
*   @doxtype{Dewpsi::VertexArray}. As noted in the documentation, the vertex array is not bound
*   automatically after creation. You must bind it yourself by calling @doxfunc{VertexArray::Bind}.
*
*   After the vertex array is created and bound, the vertex buffer can be created. As a side-note,
*   you can create a vertex buffer and index buffer in either order; they just have to be added to
*   the vertex array at roughly the same time.
*   @skip Creating vertex buffer
*   @until Dewpsi::VertexBuffer::Create
*
*   After the vertex buffer contains data, the vertex buffer must then be told the layout
*   of said data. That is done by calling Dewpsi::VertexBuffer::SetLayout(). It accepts a
*   @doxtype{BufferLayout}, a data structure that tells shaders how to interpret data that is
*   sent to them via vertex attributes.
*   @skip Vertex buffer layout
*   @until SetLayout
*
*   @doxtype{BufferElement}, the type held by @doxtype{BufferLayout}, accepts two to three
*   parameters. The first parameter is the type of data being sent to the shader. It is a
*   constant of @ref Dewpsi::ShaderDataType "ShaderDataType". The second parameter is a name for the data, as a string.
*   The third parameter is optional: a boolean that determines whether the data will be normalized.
*   It defaults to @c false. For more information, see the documentation for
*   @ref Dewpsi::BufferLayout "BufferLayout".
*
*   Define an array of indices before creating the index buffer. Then create the index buffer and
*   feed the data into it.
*   @skip indices
*   @line indices
*   @line Dewpsi::IndexBuffer::Create
*
*   After the vertex and index buffers are created, they can be added to the vertex array.
*
*   @snippet render.cpp Add buffers to array
*
*   And from there the vertex array can finally be drawn.
*
*   @endparblock
*
    @section    useRender_sec Using the Renderer
    @parblock
    aaaa
    @endparblock
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

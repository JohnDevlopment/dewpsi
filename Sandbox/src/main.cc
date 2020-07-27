#include <csignal>
#include <iostream>

// Dewpsi includes
#include <Dewpsi_Log.h>
#include <Dewpsi_WhichOS.h>
#include <Dewpsi_Window.h>
#include <Dewpsi_Application.h>
#include <Dewpsi_Layer.h>
#include <Dewpsi_KeyEvent.h>
#include <Dewpsi_Color.h>
#include <Dewpsi_String.h>
#include <Dewpsi_Vector.h>


static constexpr Dewpsi::StaticString ShaderFile = "Dewpsi/OpenGL/shaders/shaders.glsl";

static bool g_bOnce = false;

extern "C" void forcequit(int);
extern "C" void quit();

// OpenGL layer
class OpenGLLayer : public Dewpsi::Layer {
public:
    OpenGLLayer() : Layer("OpenGL"), win(Dewpsi::Application::Get().GetWindow())
    {  }
    
    virtual ~OpenGLLayer()
    {  }
    
    virtual void OnAttach() override;
    
    virtual void OnDetach() override;
    
    virtual void OnUpdate(Dewpsi::Timestep delta) override;

private:
    PDuint m_uiVertexBuffer;
    PDuint m_uiProgram;
    Dewpsi::Window& win;
};

void OpenGLLayer::OnAttach()
{
    PD_INFO("Attaching {0} layer", m_sDebugName); // TODO: delete
    
    const int iVertSize = 2;
    
    // load shader
    bool bInit = Dewpsi::OpenGL_InitShaders(ShaderFile.get());
    if (! bInit)
    {
        Dewpsi::SetError("failed to read %s", ShaderFile.get());
        throw std::runtime_error(Dewpsi::GetError());
    }
    m_uiProgram = Dewpsi::OpenGL_GetShader();
    
    // generate vertex buffer
    const float faVerts[] = {
       -0.5f, -0.5f,
        0.0f,  0.5f,
        0.5f, -0.5f
    };
    
    // use program
    glUseProgram(m_uiProgram);
    
    // generate vertex buffer and bind it
    glGenBuffers(1, &m_uiVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexBuffer);
    
    // allocate memory for the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, faVerts, GL_STATIC_DRAW);
    
    // set and enable vertex attributes
    constexpr PDuint uiVertAttrSize = sizeof(float) * 2;
    
    glVertexAttribPointer(0, iVertSize, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void OpenGLLayer::OnDetach()
{
    glDeleteBuffers(1, &m_uiVertexBuffer);
    Dewpsi::OpenGL_DeInitShaders();
}

void OpenGLLayer::OnUpdate(Dewpsi::Timestep delta)
{
    win.Clear();
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

// sandbox application layer
class SandboxLayer : public Dewpsi::Layer {
public:
    SandboxLayer() : Layer("Sandbox")
    {  }
    
    ~SandboxLayer()
    {  }
    
    virtual void OnAttach() override
    {
        PD_INFO("Attaching {0} layer", m_sDebugName); // TODO: delete
    }
    
    virtual void OnEvent(Dewpsi::Event& e) override;
};

void SandboxLayer::OnEvent(Dewpsi::Event& e)
{
    switch (e.GetEventType())
    {
    case Dewpsi::ET_KeyPressed:
        {
            using Dewpsi::KeyPressedEvent;
            KeyPressedEvent& keyEvent = reinterpret_cast<KeyPressedEvent&>(e);
            PD_INFO("{0}, key translates to '{1}'", keyEvent, char(keyEvent.GetKeyCode())); // TODO: delete
            break;
        }
        
        default: break;
    } // end switch
}

// sandbox application
class Sandbox : public Dewpsi::Application {
public:
    Sandbox()
    {
        PushLayer(new SandboxLayer());
        PushLayer(new OpenGLLayer());
    }
    
    virtual ~Sandbox()
    {  }
};

static Dewpsi::Application* App = nullptr;

int main (int argc, char const* argv[])
{
    std::signal(SIGINT, forcequit);
    std::atexit(quit);
    
    // initialize the logging system
    Dewpsi::Log::Init();
    
    {
        using Dewpsi::WindowFlags;
        using Dewpsi::SetWindowOpenGLAttribute;
        Dewpsi::WindowProps props;
        
        if (argc > 1)
        {
            if (std::strcmp(argv[1], "-l") == 0)
                props.title = "list renderers";
            else
                props.title = "Client Dewpsi Application";
        }
        else
            props.title = "Client Dewpsi Application";
        
        props.x = PD_WINDOWPOS_CENTERED;
        props.y = PD_WINDOWPOS_CENTERED;
        props.width = 640;
        props.height = 480;
        props.index = 0;
        props.flags = WindowFlags::RendererVSync | WindowFlags::WindowOpenGL;
        SetWindowOpenGLAttribute(props, Dewpsi::MajorVersion, 4);
        SetWindowOpenGLAttribute(props, Dewpsi::MinorVersion, 3);
        SetWindowOpenGLAttribute(props, Dewpsi::Depth, 24);
        SetWindowOpenGLAttribute(props, Dewpsi::DoubleBuffer, 1);
        SetWindowOpenGLAttribute(props, Dewpsi::ShareContext, 0);
        Dewpsi::SetWindowProps(props);
    }
    
    // start client application
    App = Dewpsi::NewApplication();
#ifdef PD_DEBUG
    PD_INFO("Started sandbox application");
#endif
    
    // create window
    {
        Dewpsi::Window& rWindow = App->GetWindow();
        Dewpsi::WindowModeInfo info;
        if (Dewpsi::GetWindowInformation(&rWindow, &info) < 0)
        {
            PD_ERROR("Failed to retrieve window info: {0}", Dewpsi::GetError());
            return 1;
        }
        PD_INFO("\tswap interval: {0}\n\thardware acceleration: {1}\n\twindow width: {2}\n\twindow height: {3}\n\trendering device vendor: {4}\n\trenderer: {5}\n\trender version: {6}\n\trenderer extensions: {7}",
                     info.swap, (info.accel ? "enabled" : "disabled"), info.width, info.height,
                     info.vendor, info.renderer, info.version, info.extensions);
        
        // rgba sizes
        PD_INFO("\twindow red size: {0} bits\n\twindow green size: {1} bits\n\twindow blue size: {2} bits",
                     PD_LONYBBLE(info.red), PD_LONYBBLE(info.green), PD_LONYBBLE(info.blue));
    }
    
    // run main loop
    App->Run();
    
    delete App;
    App = nullptr;
    return 0;
}

Dewpsi::Application* Dewpsi::NewApplication()
{
    return new Sandbox();
}

void quit()
{
    if (! g_bOnce)
    {
        g_bOnce = true;
        delete App;
        App = nullptr;
    }
}

void forcequit(int sig)
{
    if (! g_bOnce)
    {
        g_bOnce = true;
        std::cout << "Application received signal 11 (SIGINT)...exiting program" << std::endl;
        delete App;
        std::exit(0);
    }
}


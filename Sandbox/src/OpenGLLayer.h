#ifndef OPENGLLAYER_H
#define OPENGLLAYER_H

#include <Dewpsi_Core.h>
#include <Dewpsi_Layer.h>
#include <Dewpsi_WhichOS.h>

class OpenGLLayer : public Dewpsi::Layer {
public:
    OpenGLLayer() : Layer("OpenGL"), m_uiVertexArray(0), m_uiVertexBuffer(0),
                    m_uiProgram(0), win(Dewpsi::Application::Get().GetWindow())
    {  }
    
    virtual ~OpenGLLayer()
    {  }
    
    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(Dewpsi::Timestep delta) override;

private:
    PDuint m_uiVertexArray;
    PDuint m_uiVertexBuffer;
    PDuint m_uiProgram;
    Dewpsi::Window& win;
};

#endif /* OPENGLLAYER_H */

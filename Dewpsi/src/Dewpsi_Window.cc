#include "Dewpsi_SDLWindow.h"

namespace Dewpsi {

WindowProps::WindowProps() : title(), width(0), height(0), flags(0), index(0), openglattr {0}
{
    std::fill_n(openglattr, 10, static_cast<int32_t>(OpenGLAttributes::Empty));
}

WindowProps::WindowProps(const WindowProps& src)
    : title(src.title), width(src.width), height(src.height), flags(src.flags),
      index(src.index), openglattr {0}
{
    std::copy_n(src.openglattr, 10, openglattr);
}

Scope<Dewpsi::Window> Window::Create(const WindowProps& props)
{
#ifdef PD_PLATFORM_LINUX
    return CreateScope<SDL2Window>(props);
#else
    #error Currently only Linux is supported
#endif
}

void SetWindowOpenGLAttribute(WindowProps& props, OpenGLAttributes attr, int val)
{
    int x;
    
    for (x = 0; x < int(OpenGLAttributes::Count); ++x)
    {
        if (props.openglattr[x] == OpenGLAttributes::Empty)
        {
            props.openglattr[x] = PD_CREATEWORD(val, attr);
            break;
        }
    }
    
    PD_CORE_ASSERT(x < int(OpenGLAttributes::Count), "SetWindowOpenGLAttribute: Attribute list is full"); // TODO: remove?
}

void GetWindowOpenGLAttribute(const WindowProps& props, OpenGLAttributes& pAttr, int& pVal, unsigned int index)
{
    PD_CORE_ASSERT(index < (unsigned int)(OpenGLAttributes::Count), "Invalid index {0}", index);
    const uint32_t& uiAttr = props.openglattr[index];
    pAttr = (OpenGLAttributes) PD_HIWORD(uiAttr);
    pVal = PD_LOWORD(uiAttr);
}

}


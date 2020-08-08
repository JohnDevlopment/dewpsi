#include "Dewpsi_SDLWindow.h"
#include "Dewpsi_Application.h"

namespace Dewpsi {

WindowProps* Window::s_pWindowState = nullptr;

WindowProps::WindowProps() : title(), width(0), height(0), flags(0), index(0), openglattr {0}
{
    std::fill_n(openglattr, PD_ARRAYSIZE(openglattr), static_cast<int32_t>(OpenGLAttributes::Empty));
}

WindowProps::WindowProps(const WindowProps& src)
    : title(src.title), width(src.width), height(src.height), flags(src.flags),
      index(src.index), openglattr {0}
{
    std::copy_n(src.openglattr, PD_ARRAYSIZE(openglattr), openglattr);
}

WindowProps::WindowProps(WindowProps&& src)
    : title(PD_MOVE(src.title)), width(src.width), height(src.height),
      flags(src.flags), index(src.index), openglattr()
{
    std::copy_n(src.openglattr, PD_ARRAYSIZE(openglattr), openglattr);
}

WindowProps& WindowProps::operator=(const WindowProps& rhs)
{
    title = rhs.title;
    x = rhs.x;
    y = rhs.y;
    width = rhs.width;
    height = rhs.height;
    flags = rhs.flags;
    index = rhs.index;
    std::copy_n(openglattr, PD_ARRAYSIZE(openglattr), openglattr);
    return *this;
}

WindowProps& WindowProps::operator=(WindowProps&& rhs)
{
    title = PD_MOVE(rhs.title);
    x = rhs.x;
    y = rhs.y;
    width = rhs.width;
    height = rhs.height;
    flags = rhs.flags;
    index = rhs.index;
    std::copy_n(openglattr, PD_ARRAYSIZE(openglattr), openglattr);
    return *this;
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
            props.openglattr[x] = PD_CREATEDWORD(val, attr);
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

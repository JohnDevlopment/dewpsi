#ifndef DEWPSI_ENTRYPOINT_H
#define DEWPSI_ENTRYPOINT_H

#include <Dewpsi_Core.h>
#include <Dewpsi_Log.h>
#include <Dewpsi_Renderer.h>
#include <Dewpsi_Math.h>

int main(int argc, const char* argv[])
{
    Dewpsi::Log::Init();

    // parse arguments, if desired

    {
        using Dewpsi::SetWindowAttribute;
        Dewpsi::WindowProps props;

        props.x = 0; // PD_WINDOWPOS_CENTERED for centered y offset
        props.y = 0; // PD_WINDOWPOS_CENTERED for centered y offset
        props.width = 640;
        props.height = 480;
        props.title = "WINDOW TITLE";
        props.flags = Dewpsi::RendererVSync | Dewpsi::WindowOpenGL | Dewpsi::WindowResizable;
        SetWindowAttribute(props, Dewpsi::MajorVersion, 3);
        SetWindowAttribute(props, Dewpsi::MinorVersion, 3);
        SetWindowAttribute(props, Dewpsi::Depth, 24);
        SetWindowAttribute(props, Dewpsi::DoubleBuffer, 1);
        SetWindowAttribute(props, Dewpsi::StencilSize, 8);
        Dewpsi::SetWindowProps(props);
    }

    auto app = Dewpsi::NewApplication(); // also creates the window and rendering context

    delete app; // shuts down the application

    return 0;
}

#endif /* DEWPSI_ENTRYPOINT_H */

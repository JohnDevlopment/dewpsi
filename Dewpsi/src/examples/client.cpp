//! [Window Properties]
Dewpsi::WindowProps props;
props.x = 0;
props.y = 0;
props.width = 640;
props.height = 480;
props.flags = Dewpsi::WindowShown;
//! [Window Properties]

//! [OpenGL Attributes]
props.flags = Dewpsi::WindowShown | Dewpsi::WindowOpenGL;
Dewpsi::SetWindowOpenGLAttribute(props, Dewpsi::AccelerationRequired, 1);
Dewpsi::SetWindowOpenGLAttribute(props, Dewpsi::Depth, 24);
//! [OpenGL Attributes]

//! [Set Window Properties]
Dewpsi::SetWindowProps(props);
//! [Set Window Properties]

//! [Create Application]
Dewpsi::Application App = Dewpsi::NewApplication();
//! [Create Application]

//! [Whole Example]
Dewpsi::Log::Init();

Dewpsi::WindowProps props;
props.x = 0;
props.y = 0;
props.width = 640;
props.height = 480;
props.flags = Dewpsi::WindowShown | Dewpsi::WindowOpenGL;
Dewpsi::SetWindowOpenGLAttribute(props, Dewpsi::AccelerationRequired, 1);
Dewpsi::SetWindowOpenGLAttribute(props, Dewpsi::Depth, 24);
Dewpsi::SetWindowProps(props);
Dewpsi::Application App = Dewpsi::NewApplication();
//! [Whole Example]

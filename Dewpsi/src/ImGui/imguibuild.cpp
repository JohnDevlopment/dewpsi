#if defined(PD_PLATFORM_LINUX) || defined(__linux)
# define IMGUI_IMPL_OPENGL_LOADER_GLAD
# include "Dewpsi_ImGui_OpenGL3.cpp"
# include "Dewpsi_ImGui_SDL.cpp"
#endif

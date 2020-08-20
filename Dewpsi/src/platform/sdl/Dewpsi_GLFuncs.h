/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2020 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/* list of OpenGL functions sorted alphabetically
   If you need to use a GL function from the SDL video subsystem,
   change its entry from SDL_PROC_UNUSED to SDL_PROC and rebuild.
*/

/* Developer's note: the functions listed here were not included by GLAD; as such they
   are from the SDL video subsystem, as noted in the above comment.
*/
#define SDL_PROC_UNUSED(ret,func,params)

#if defined(PD_GL_FUNC_DECLS)
    #define SDL_PROC(ret,func,params)   extern ret (PD_APIENTRY * func) params;
    #undef PD_GL_FUNC_DECLS
#elif defined(PD_GL_FUNC_DEFS)
    #define SDL_PROC(ret,func,params)   ret (*func) params = nullptr;
    #undef PD_GL_FUNC_DEFS
#else
    #define SDL_PROC(ret, func, params) \
        do { \
            void * tptr = SDL_GL_GetProcAddress(#func); \
            if (! tptr) { \
                return SDL_SetError("Could not load GL function %s: %s", #func, SDL_GetError()); \
            } \
            func = (ret (PD_APIENTRY *) params) tptr; \
        } while(0);
#endif

#if defined(PD_GL_FUNC_DECLS) || defined(PD_GL_FUNC_DEFS)
extern "C" {
#endif

/* list of function declarations */
SDL_PROC(void, glBegin, (GLenum mode))
SDL_PROC(void, glColor3f, (GLfloat red, GLfloat green, GLfloat blue))
SDL_PROC(void, glEnd, (void))
SDL_PROC(void, glVertex2f, (GLfloat x, GLfloat y))

#if defined(PD_GL_FUNC_DECLS) || defined(PD_GL_FUNC_DEFS)
}
#endif

#undef SDL_PROC
#undef SDL_PROC_UNUSED

/* vi: set ts=4 sw=4 expandtab: */

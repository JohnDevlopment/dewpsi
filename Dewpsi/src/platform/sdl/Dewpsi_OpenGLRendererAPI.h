#ifndef DEWPSI_OPENGLRENDERERAPI_H
#define DEWPSI_OPENGLRENDERERAPI_H

#include <Dewpsi_RendererAPI.h>
#include <Dewpsi_OpenGL.h>

namespace Dewpsi {
    class OpenGLRendererAPI : public RendererAPI {
	public:
		virtual void SetClearColor(const Color& color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};
}

#endif /* DEWPSI_OPENGLRENDERERAPI_H */

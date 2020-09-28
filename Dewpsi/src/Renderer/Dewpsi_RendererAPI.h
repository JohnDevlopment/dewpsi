#ifndef DEWPSI_RENDERERAPI_H
#define DEWPSI_RENDERERAPI_H

/**
*	@file Dewpsi_RendererAPI.h
*	@brief @doxfb
*	Contains platform-agnostic renderer API class.
*	@ingroup renderer
*/

#include <Dewpsi_Memory.h>
#include <Dewpsi_Color.h>
#include <Dewpsi_VertexArray.h>

namespace Dewpsi {
	/** Rendering API abstraction.
	*	This class abstracts the low-level rendering API calls.
	*	@ingroup renderer
	*/
	class RendererAPI {
	public:
		/// Constructs an empty %RendererAPI.
		RendererAPI() {}

		/// Destroys the %RendererAPI.
		virtual ~RendererAPI() {}

		/// Rendering API
	    enum class API {
	        None,       ///< No API selected
	        OpenGL      ///< OpenGL API
	    };

		/// Initialize the rendering API.
		virtual void Init() = 0;

		/** Sets the clear color.
		*	The argument is a Color object that has the fields
		*	@a red, @a green, @a blue, and @a alpha, each field
		*	representing its respective color component.
		*/
		virtual void SetClearColor(const Color& color) = 0;

		/// Clears the window using the color set in SetClearColor().
		virtual void Clear() = 0;

		/// Draws the given vertex array.
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		/// Sets the new current API.
		static void SetAPI(API api) {s_API = api;}

		/// Returns the current API.
		static API GetAPI() {return s_API;}

	protected:
		Color m_ClearColor;

	private:
        static API s_API;
	};
}

#endif /* DEWPSI_RENDERERAPI_H */

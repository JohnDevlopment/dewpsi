#ifndef DEWPSI_IMGUILAYER_H
#define DEWPSI_IMGUILAYER_H

/**
*   @file       Dewpsi_ImGuiLayer.h
*   @brief      @doxfb
*   @ingroup    layers
*/

#include <Dewpsi_Layer.h>
#include <Dewpsi_Event.h>
#include <Dewpsi_MouseEvent.h>
#include <Dewpsi_ApplicationEvent.h>
#include <Dewpsi_KeyEvent.h>
#include <Dewpsi_WhichOS.h>

namespace Dewpsi {
    /** The debug ImGui layer.
    *   @ingroup layers
    */
    class ImGuiLayer : public Layer {
    public:
        /// Initializes the layer with the parameters found in @data.
        ImGuiLayer(const void* data);

        /// Destroy the layer.
        ~ImGuiLayer();

        /// Attaches the layer.
        virtual void OnAttach() override;

        /// Detaches the layer.
        virtual void OnDetach() override;

        /// Draws ImGui layer.
        virtual void OnImGuiRender() override;

        /// Begin an ImGui frame.
        void Begin();

        /// End an ImGui frame.
        void End();

    private:
        const void* m_vpData;
        SDL_Window* m_Window;
        SDL_GLContext m_Context;
        bool m_Init;
    };
}

#endif /* DEWPSI_IMGUILAYER_H */

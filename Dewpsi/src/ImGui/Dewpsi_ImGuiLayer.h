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

        /// Update function.
        virtual void OnUpdate(Timestep delta) override;

        /// Event processor.
        virtual void OnEvent(Event& e) override;

    private:
        bool OnMouseButtonPressed(MousePressedEvent& e);
        bool OnMouseButtonReleased(MouseReleasedEvent& e);
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnMouseMotion(MouseMovedEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);
        bool OnKeyTyped(KeyTypedEvent& e);
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnKeyReleased(KeyReleasedEvent& e);

        const void* m_vpData;
    };
}

#endif /* DEWPSI_IMGUILAYER_H */

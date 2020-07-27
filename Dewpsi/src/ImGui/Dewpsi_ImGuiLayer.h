#ifndef DEWPSI_IMGUILAYER_H
#define DEWPSI_IMGUILAYER_H

#include <Dewpsi_Layer.h>
#include <Dewpsi_Event.h>

namespace Dewpsi {
    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer();
        
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(Timestep delta) override;
        virtual void OnEvent(Event& e) override;
    
    private:
    };
}

#endif /* DEWPSI_IMGUILAYER_H */

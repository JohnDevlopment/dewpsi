#ifndef DEWPSI_LAYER_H
#define DEWPSI_LAYER_H

/**
*   @file   Dewpsi_Layer.h
*   @brief  A header for the Dewpsi engine.
*
*   Defines an abstract interface to a layer.
*
*   @defgroup   layers  Layers
*   Contains classes that represent layers and layer stacks.
*
*   @ingroup    core
*/

#include <Dewpsi_Core.h>
#include <Dewpsi_Event.h>
#include <Dewpsi_Timestep.h>

namespace Dewpsi {
    /** A layer.
    *   It can accept events.
    *   @ingroup layers
    */
    class Layer {
    public:
        /// Constructs a layer with the given name.
        Layer(const std::string& name = "Layer");

        /// Destroys the layer and frees any memory allocated by it.
        virtual ~Layer();

        /// Called when attaching a layer to a layer stack.
        virtual void OnAttach()
        {  }

        /// Called when detaching a layer from a layer stack.
        virtual void OnDetach()
        {  }

        /// Called when updating the layer.
        virtual void OnUpdate(Timestep delta)
        {  }

        /// Lets layers use ImGui functions.
        virtual void OnImGuiRender()
        {  }

        /// Called when processing events for the layer.
        virtual void OnEvent(Event& e)
        {  }

        /// Returns the name of the layer as a string (debug only).
        inline const std::string& GetName() const
        { return m_sDebugName; }

    protected:
        std::string m_sDebugName;
    };
}

/// @}

#endif /* DEWPSILAYER_H */

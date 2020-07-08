#ifndef DEWPSILAYER_H
#define DEWPSILAYER_H

/**
*   @file   DewpsiLayer.h
*   @brief  A header for the Dewpsi engine.
*
*   Defines an abstract interface to a layer.
*
*   @defgroup   layers  Layers
*   Contains classes that represent layers and layer stacks.
*
*   @ingroup    core
*
*   @{
*/

#include <core.h>
#include <event.h>

namespace Dewpsi {
    /** A layer.
    *   It can accept events.
    */
    class Layer {
    public:
        /// Constructs a layer with the given name.
        Layer(const std::string& name = "Layer");
        
        /// Destroys the layer and frees any memory allocated by it.
        virtual ~Layer();
        
        virtual void OnAttach()
        {  }
        
        /// Called when detaching a layer from a layer stack.
        virtual void OnDetach()
        {  }
        
        /// Called when updating the layer.
        virtual void OnUpdate()
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


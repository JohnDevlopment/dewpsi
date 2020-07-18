#ifndef DEWPSI_LAYERSTACK_H
#define DEWPSI_LAYERSTACK_H

/**
*   @file   Dewpsi_LayerStack.h
*   @brief  A header for the Dewpsi engine.
*   Handles layer stacks.
*/

#include <Dewpsi_Core.h>
#include <Dewpsi_Layer.h>
#include <vector>

namespace Dewpsi {
    /** A layer stack.
    *   @ingroup layers
    */
    class LayerStack {
    public:
        /// An iterator to a layer
        typedef std::vector<Layer*>::iterator Iterator;
        
        /// A reverse iterator to a layer
        typedef std::vector<Layer*>::reverse_iterator ReverseIterator;
        
        LayerStack();
        ~LayerStack();
        
        /// Pushes a layer onto the stack.
        void PushLayer(Layer* layer);
        
        /// Pushes a layer onto the end of the stack.
        void PushOverlay(Layer* overlay);
        
        /// Pulls a layer off of the stack.
        void PullLayer(Layer* layer);
        
        /// Pulls a layer off of the end portion of the stack.
        void PullOverlay(Layer* overlay);
        
        /// Returns an iterator to the beginning to the layer stack.
        Iterator begin()
        { return m_vLayers.begin(); }
        
        /// Returns an iterator to the end of the layer stack.
        Iterator end()
        { return m_vLayers.end(); }
        
        /// Returns a reverse iterator to the end to the layer stack.
        ReverseIterator rbegin()
        { return m_vLayers.rbegin(); }
        
        /// Returns a reverse iterator to the beginning of the layer stack.
        ReverseIterator rend()
        { return m_vLayers.rend(); }
        
    private:
        std::vector<Layer*> m_vLayers;
        int m_iInsertIndex;
    };
}

/// @}

#endif /* DEWPSI_LAYERSTACK_H */

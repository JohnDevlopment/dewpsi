#include "DewpsiLayerStack.h"

namespace Dewpsi {

LayerStack::~LayerStack()
{
    for (Layer* layer : m_vLayers)
    {
        layer->OnDetach();
        delete layer;
    }
}

void LayerStack::PushLayer(Layer* layer)
{
    m_vLayers.emplace(m_vLayers.begin() + m_iInsertIndex, layer);
    ++m_iInsertIndex;
}

void LayerStack::PushOverlay(Layer* overlay)
{
    m_vLayers.emplace_back(overlay);
}

void LayerStack::PullLayer(Layer* layer)
{
    auto itr = std::find(m_vLayers.begin(), m_vLayers.begin() + m_iInsertIndex, layer);
    if (itr != (m_vLayers.begin() + m_iInsertIndex))
    {
        layer->OnDetach();
        m_vLayers.erase(itr);
        --m_iInsertIndex;
    }
}

void LayerStack::PullOverlay(Layer* overlay)
{
    auto itr = std::find(m_vLayers.begin() + m_iInsertIndex, m_vLayers.end(), overlay);
    if (itr != m_vLayers.end())
    {
        overlay->OnDetach();
        m_vLayers.erase(itr);
    }
}

}


namespace {

Object::__ObjectType* Object::NewObject()
{
    int iNumObjects = 0;
    for (auto& ptr : m_AvailableObjects)
    {
        if (ptr != nullptr)
            ++iNumObjects;
    }

    return nullptr;
}

}

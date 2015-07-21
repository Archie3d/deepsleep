#include "dsObjectController.h"
#include "dsObject.h"

namespace ds {

Object::Object(Type type, Scene *pScene)
    : SceneNode(pScene),
      m_type(type),
      m_controllers()
{
}

Object::~Object()
{
    // Delete all attached controllers
    std::vector<ObjectController*> controllers = m_controllers;
    m_controllers.clear();
    for (std::vector<ObjectController*>::iterator it = controllers.begin();
         it != controllers.end();
         ++it) {
        delete (*it);
    }
}

bool Object::selectForRendering(Renderer *pRenderer)
{
    DS_UNUSED(pRenderer);
    return false;
}

void Object::render(Renderer *pRenderer)
{
    DS_UNUSED(pRenderer);
}

void Object::update(int deltaMs)
{
    updateControllers(deltaMs);
}

void Object::attachControllerPrivate(ObjectController *pController)
{
    DS_ASSERT(pController != nullptr);
    if (pController->object() != this) {
        m_controllers.push_back(pController);
    }
}

void Object::detachControllerPrivate(ObjectController *pController)
{
    DS_ASSERT(pController != nullptr);
    std::vector<ObjectController*>::iterator it = std::find(m_controllers.begin(), m_controllers.end(), pController);
    if (it != m_controllers.end()) {
        m_controllers.erase(it);
    }

}

void Object::updateControllers(int deltaMs)
{
    for (std::vector<ObjectController*>::iterator it = m_controllers.begin();
         it != m_controllers.end();
         ++it) {
        (*it)->update(deltaMs);
    }
}

} // namespace ds

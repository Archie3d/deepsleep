#ifndef DS_OBJECT_H
#define DS_OBJECT_H

#include "dsSceneNode.h"
#include "dsIUpdatable.h"
#include "dsIRenderable.h"

namespace ds {

class Renderer;
class ObjectController;

/**
 * Base class to represent objects.
 * Objects are 'material' things that can be somehow
 * manipulated.
 */
class Object : public SceneNode, public IUpdatable, public IRenderable
{
    friend class ObjectController;
public:

    /// Object type
    enum Type {
        Type_Null,              ///< Empty object
        Type_Camera,            ///< Camera
        Type_PointLight,        ///< Point light source
        Type_SpotLight,         ///< Spot light source
        Type_SpotLightShadow,   ///< Spot light with shadow
        Type_Geometry           ///< Cube primitive
    };

    Object(Type type, Scene *pScene = nullptr);
    ~Object();

    Type type() const { return m_type; }

    bool isNull() const { return false; }

    // IRenderable to be implemented by inherited class
    bool selectForRendering(Renderer *pRenderer);
    void render(Renderer *pRenderer);

    // IUpdatable
    void update(int deltaMs);

protected:

    void attachControllerPrivate(ObjectController *pController);
    void detachControllerPrivate(ObjectController *pController);
    void updateControllers(int deltaMs);

private:

    Type m_type;    ///< Object type.    

    /// Attached controllers.
    std::vector<ObjectController*> m_controllers;
};

} // namespace ds

#endif // DS_OBJECT_H

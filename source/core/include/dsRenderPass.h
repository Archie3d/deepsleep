#ifndef DS_RENDERPASS_H
#define DS_RENDERPASS_H

namespace ds {

/**
 * Abstract base class representing a rendering pass.
 * Render pass is invoked by renderer. Initialization is
 * performed upon renderer initialization as well.
 */
class RenderPass
{
public:

    enum Id {
        Id_Selection,       ///< Objects selection render pass
        Id_Geometry,        ///< Geometry render pass
        Id_AmbientLight,    ///< Ambient illumination
        Id_PointLight,      ///< Apply point lights
        Id_SpotLight,       ///< Apply spot lights
        Id_SpotLightShadow, ///< Apply spot lights with shadow maps
        Id_SpotLightShadowMap,  ///< Compute spot lights shadow maps
        Id_Texture          ///< Show a texture
    };

    RenderPass(Id id);
    virtual ~RenderPass();

    Id id() const { return m_id; }

    bool isEnabled() const { return m_enabled; }
    void setEnabled(bool v = true) { m_enabled = v; }

    /// Initialize this render pass.
    virtual void initialize() = 0;

    /// Perform the rendering pass.
    virtual void perform() = 0;

private:

    Id m_id;
    bool m_enabled;
};

} // namespace ds

#endif // DS_RENDERPASS_H

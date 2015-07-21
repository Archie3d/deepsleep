#ifndef DS_MESH_H
#define DS_MESH_H

#include <vector>
#include "dsMath.h"
#include "dsResource.h"

namespace ds {

class Mesh : public Resource
{
public:

    Mesh(const std::string &name);

    size_t memoryUsage() const;

    const std::vector<glm::vec3>& vertices() const { return m_vertices; }
    const std::vector<glm::vec3>& normals() const { return m_normals; }
    const std::vector<glm::vec2>& textureCoordinates() const { return m_textureCoordinates; }
    const std::vector<int>& faces() const { return m_faces; }

    /**
     * Generate normal vectors by considering adjacent faces.
     */
    void generateAverageNormals();

    static Mesh* loadFromObj(const std::string &name, const std::string &path);

private:

    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec2> m_textureCoordinates;
    std::vector<int> m_faces;
};

} // namespace ds

#endif // DS_MESH_H

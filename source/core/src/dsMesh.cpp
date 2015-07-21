#include <iostream>
#include <fstream>
#include <sstream>
#include "dsGlobal.h"
#include "dsMesh.h"

namespace ds {

Mesh::Mesh(const std::string &name)
    : Resource(Type_Mesh, name)
{
}

size_t Mesh::memoryUsage() const
{
    return sizeof(float) * 3 * m_vertices.capacity() +
            sizeof(float) * 3 * m_normals.capacity() +
            sizeof(float) * 2 * m_textureCoordinates.capacity() +
            sizeof(int) * m_faces.capacity();
}

void Mesh::generateAverageNormals()
{
    m_normals.clear();

    for (int i = 0; i < m_vertices.size(); i++) {
        m_normals.push_back(glm::vec3(0.0f));
    }

    for (int i = 0; i < m_faces.size(); i += 3) {
        const glm::vec3 &p1 = m_vertices[m_faces[i]];
        const glm::vec3 &p2 = m_vertices[m_faces[i + 1]];
        const glm::vec3 &p3 = m_vertices[m_faces[i + 2]];

        glm::vec3 a = p2 - p1;
        glm::vec3 b = p3 - p1;
        glm::vec3 n = glm::normalize(glm::cross(a, b));

        m_normals[m_faces[i]] += n;
        m_normals[m_faces[i + 1]] += n;
        m_normals[m_faces[i + 2]] += n;
    }

    for (int i = 0; i < m_normals.size(); i++) {
        m_normals[i] = glm::normalize(m_normals[i]);
    }
}

Mesh* Mesh::loadFromObj(const std::string &name, const std::string &path)
{
    std::ifstream objStream(path, std::ios::in);
    if (!objStream) {
        dsError() << "Unable to read file " << path;
        return nullptr;
    }

    std::string line, token;
    int nFaces = 0;
    std::vector<int> face;

    Mesh *pMesh = new Mesh(name);

    std::getline(objStream, line);
    while (!objStream.eof()) {
        line = trim(line);
        if (line.length() > 0 && line.at(0) != '#') {
            std::istringstream lineStream(line);

            lineStream >> token;
            if (token == "v") {
                float x, y, z;
                lineStream >> x >> y >> z;
                pMesh->m_vertices.push_back(glm::vec3(x, y, z));
            } else if (token == "vt") {
                float s, t;
                lineStream >> s >> t;
                pMesh->m_textureCoordinates.push_back(glm::vec2(s, t));
            } else if (token == "vn") {
                float x, y, z;
                lineStream >> x >> y >> z;
                pMesh->m_normals.push_back(glm::vec3(x, y, z));
            } else if (token == "f") {
                nFaces++;

                face.clear();
                size_t slash1, slash2;
                while (lineStream.good()) {
                    std::string vertString;

                    lineStream >> vertString;
                    int pIndex = -1, nIndex = -1, tcIndex = -1;

                    slash1 = vertString.find("/");
                    if (slash1 == std::string::npos) {
                        pIndex = std::atoi(vertString.c_str()) - 1;
                    } else {
                        slash2 = vertString.find("/", slash1 + 1);
                        pIndex = std::atoi(vertString.substr(0, slash1).c_str()) - 1;
                        if (slash2 > slash1 + 1) {
                            tcIndex = std::atoi(vertString.substr(slash1 + 1, slash2).c_str()) - 1;
                        }
                        nIndex = std::atoi(vertString.substr(slash2 + 1, vertString.length()).c_str()) - 1;
                    }
                    if (pIndex == -1) {
                        dsError() << "Missing point index";
                    } else {
                        face.push_back(pIndex);
                    }

                }

                // Transform faces into triangles
                if (face.size() > 3) {
                    int v0 = face[0];
                    int v1 = face[1];
                    int v2 = face[2];

                    pMesh->m_faces.push_back(v0);
                    pMesh->m_faces.push_back(v1);
                    pMesh->m_faces.push_back(v2);
                    for (unsigned i = 3; i < face.size(); i++) {
                        v1 = v2;
                        v2 = face[i];
                        pMesh->m_faces.push_back(v0);
                        pMesh->m_faces.push_back(v1);
                        pMesh->m_faces.push_back(v2);
                    }
                } else {
                    // Already a triange
                    DS_ASSERT(face.size() == 3);
                    pMesh->m_faces.push_back(face[0]);
                    pMesh->m_faces.push_back(face[1]);
                    pMesh->m_faces.push_back(face[2]);
                }
            }
        }
        std::getline(objStream, line);
    }

    objStream.close();

    //if (pMesh->m_normals.size() == 0) {
        pMesh->generateAverageNormals();
    //}

#if 1
    dsDebug() << "Loaded OBJ mesh from " << path;
    dsDebug() << "    " << pMesh->m_vertices.size() << " vertices";
    dsDebug() << "    " << nFaces << " faces";
    dsDebug() << "    " << pMesh->m_faces.size() / 3 << " triangles";
    dsDebug() << "    " << pMesh->m_normals.size() << " normals";
    dsDebug() << "    " << pMesh->m_textureCoordinates.size() << " texture coordinates";
#endif
    return pMesh;

}


} // namespace ds

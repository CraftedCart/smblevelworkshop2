/**
 * @file
 * @brief Header for the WS2::Scene::MeshSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_SCENE_MESHSCEHENODE_HPP
#define SMBLEVELWORKSHOP2_SCENE_MESHSCEHENODE_HPP

#include "scene/SceneNode.hpp"
#include "resource/ResourceMesh.hpp"

namespace WS2 {
    namespace Scene {
        class MeshSceneNode : public SceneNode {
            protected:
                Resource::ResourceMesh *mesh;

            public:
                /**
                 * @brief Constructs a MeshSceneNode with no mesh
                 *
                 * @param name The name of the node
                 */
                MeshSceneNode(const QString name);

                /**
                 * @brief Constructs a MeshSceneNode with a mesh
                 *
                 * @param name The name of the node
                 * @param mesh A pointer to the mesh to set
                 */
                MeshSceneNode(const QString name, Resource::ResourceMesh *mesh);

                /**
                 * @brief Getter for WS2::Scene::MeshSceneNode::mesh
                 *
                 * @return A pointer to the mesh
                 */
                const Resource::ResourceMesh* getMesh() const;
        };
    }
}

#endif


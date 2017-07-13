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
                 */
                MeshSceneNode();

                /**
                 * @brief Constructs a MeshSceneNode with a mesh
                 *
                 * @param mesh A pointer to the mesh to set
                 */
                MeshSceneNode(Resource::ResourceMesh *mesh);

                /**
                 * @brief Getter for WS2::Scene::MeshSceneNode::mesh
                 *
                 * @return A pointer to the mesh
                 */
                Resource::ResourceMesh* getMesh();
        };
    }
}

#endif


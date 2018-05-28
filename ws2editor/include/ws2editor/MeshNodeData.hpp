/**
 * @file
 * @brief Heder for the MeshNodeData class WS2EDITOR_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_MESHNODEDATA_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_MESHNODEDATA_HPP

#include "ws2editor_export.h"
#include "ws2editor/physics/PhysicsContainer.hpp"
#include "ws2common/resource/ResourceMesh.hpp"

namespace WS2Editor {
    class WS2EDITOR_EXPORT MeshNodeData {
        protected:
            const WS2Common::Scene::SceneNode *node;
            WS2Common::Resource::ResourceMesh *mesh;
            Physics::PhysicsContainer *physicsContainer;

        public:
            MeshNodeData(WS2Common::Scene::SceneNode *node, WS2Common::Resource::ResourceMesh *mesh);

            const WS2Common::Scene::SceneNode* getNode() const;
            WS2Common::Resource::ResourceMesh* getMesh();
            const WS2Common::Resource::ResourceMesh* getMesh() const;
            Physics::PhysicsContainer* getPhysicsContainer();
            const Physics::PhysicsContainer* getPhysicsContainer() const;
    };
}

#endif


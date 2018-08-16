/**
 * @file
 * @brief Heder for the MeshNodeData class
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
            QVector<WS2Common::Resource::ResourceMesh*> meshes;
            Physics::PhysicsContainer *physicsContainer;

        public:
            MeshNodeData(WS2Common::Scene::SceneNode *node, WS2Common::Resource::ResourceMesh *mesh);
            MeshNodeData(WS2Common::Scene::SceneNode *node, QVector<WS2Common::Resource::ResourceMesh*>& meshes);

            const WS2Common::Scene::SceneNode* getNode() const;
            QVector<WS2Common::Resource::ResourceMesh*>& getMeshes();
            const QVector<WS2Common::Resource::ResourceMesh*>& getMeshes() const;
            QVector<WS2Common::Model::MeshSegment*> getMeshSegments();
            const QVector<WS2Common::Model::MeshSegment*> getMeshSegments() const;
            Physics::PhysicsContainer* getPhysicsContainer();
            const Physics::PhysicsContainer* getPhysicsContainer() const;
            const WS2Common::AABB3 getAabb() const;
    };
}

#endif


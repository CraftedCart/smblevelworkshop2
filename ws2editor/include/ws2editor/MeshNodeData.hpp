/**
 * @file
 * @brief Heder for the MeshNodeData class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_MESHNODEDATA_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_MESHNODEDATA_HPP

#include "ws2editor/physics/PhysicsContainer.hpp"
#include "ws2common/resource/ResourceMesh.hpp"

namespace WS2Editor {
    using namespace WS2Editor::Physics;
    using namespace WS2Common::Resource;

    class MeshNodeData {
        protected:
            ResourceMesh *mesh;
            PhysicsContainer *physicsContainer;

        public:
            MeshNodeData(SceneNode *node, ResourceMesh *mesh);

            ResourceMesh* getMesh();
            const ResourceMesh* getMesh() const;
            PhysicsContainer* getPhysicsContainer();
            const PhysicsContainer* getPhysicsContainer() const;
    };
}

#endif


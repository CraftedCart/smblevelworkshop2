/**
 * @file
 * @brief Header for the RenderGlobals namespace
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_GIZMOSPLUGIN_RENDERGLOBALS_HPP
#define SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_GIZMOSPLUGIN_RENDERGLOBALS_HPP

#include "ws2common/resource/ResourceMesh.hpp"

namespace WS2EditorPlugins {
    namespace GizmosPlugin {
        namespace RenderGlobals {
            //Default models
            extern WS2Common::Model::MeshSegment *lineMeshSegment;
            extern QVector<WS2Common::Resource::ResourceMesh*> gizmoConeMesh;
            extern QVector<WS2Common::Resource::ResourceMesh*> cubeMesh;

            void loadDefaultModels();
            void unloadDefaultModels();
        }
    }
}

#endif


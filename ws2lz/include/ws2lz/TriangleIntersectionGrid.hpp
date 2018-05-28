/**
 * @file
 * @brief Header for the TriangleIntersrectionGrid class WS2LZ_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2LZ_TRIANGLEINTERSECTIONGRID_HPP
#define SMBLEVELWORKSHOP2_WS2LZ_TRIANGLEINTERSECTIONGRID_HPP

#include "ws2lz_export.h"
#include "ws2common/CollisionGrid.hpp"
#include "ws2common/model/Vertex.hpp"
#include "ws2common/AABB.hpp"
#include <QVector>

namespace WS2Lz {
    class WS2LZ_EXPORT TriangleIntersectionGrid {
        protected:
            /**
             * @brief 2D vector, containing triangles that intersect with the correspoding grid tile
             *
             * @note Not the same kind of indices passsed to by the constructor!
             *
             * (Yes I know it's a 3D vector, but you would use typically it like a 2D one to get a vector out of it)
             *
             * Stored Y, then X
             */
            QVector<QVector<QVector<quint16>>> indicesGrid;

            /**
             * @brief A vector containing all vertices used in a model in no specific order
             *
             * @note Every 3 vertices does not necessarily correspond to a triangle!
             */
            QVector<WS2Common::Model::Vertex> vertices;

        protected:
            bool doAABBsIntersect(WS2Common::AABB2 a, WS2Common::AABB2 b);

        public:
            /**
             * @brief Constructs a TriangleIntersectionGrid without sorting triangles into intersecting grid tiles
             *
             * @param vertices All vertices
             * @param grid The collision grid used to set the initial size of the indicesGrid vector
             */
            TriangleIntersectionGrid(
                    QVector<WS2Common::Model::Vertex> &vertices,
                    WS2Common::CollisionGrid &grid
                    );

            /**
             * @brief Constructs a TriangleIntersectionGrid without and sorts triangles into intersecting grid tiles
             *
             * @param vertices All vertices
             * @param indices Every 3 integers here corresponds to vertices for a triangle in the vertices vector
             * @param grid The collision grid to check triangles against
             */
            TriangleIntersectionGrid(
                    QVector<WS2Common::Model::Vertex> &vertices,
                    QVector<unsigned int> &indices,
                    WS2Common::CollisionGrid &grid
                    );

            /**
             * @brief Checks every triangle against every grid tile to see if they intersect, and if they do, add it
             *        to the appropriate location within indicesGrid
             *
             * @param indices Every 3 integers here corresponds to vertices for a triangle in the vertices vector
             * @param grid The collision grid to check triangles against
             */
            void sortIntersections(
                    QVector<unsigned int> &indices,
                    WS2Common::CollisionGrid &grid
                    );

            /**
             * @brief Getter for indicesGrid
             *
             * @return A 2D vector, containing triangles that intersect with the correspoding grid tile
             *
             * (Yes I know it's a 3D vector, but you would use typically it like a 2D one to get a vector out of it)
             */
            QVector<QVector<QVector<quint16>>>& getIndicesGrid();

            /**
             * @brief Const etter for indicesGrid
             *
             * @return A 2D vector, containing triangles that intersect with the correspoding grid tile
             *
             * (Yes I know it's a 3D vector, but you would use typically it like a 2D one to get a vector out of it)
             */
            const QVector<QVector<QVector<quint16>>>& getIndicesGrid() const;
    };
}

#endif

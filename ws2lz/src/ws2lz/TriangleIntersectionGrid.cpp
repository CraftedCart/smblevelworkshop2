#include "ws2lz/TriangleIntersectionGrid.hpp"
#include <QElapsedTimer>
#include <algorithm>
#include <QDebug>
//#include <cstdio>

namespace WS2Lz {
    TriangleIntersectionGrid::TriangleIntersectionGrid(
            QVector<WS2Common::Model::Vertex> &vertices,
            WS2Common::CollisionGrid &grid
            ) {
        this->vertices = vertices;

        //Resize the indicesGrid vectors
        indicesGrid.resize(grid.getGridStepCount().y);
        for (unsigned int i = 0; i < grid.getGridStepCount().y; i++) indicesGrid[i].resize(grid.getGridStepCount().x);
    }

    TriangleIntersectionGrid::TriangleIntersectionGrid(
            QVector<WS2Common::Model::Vertex> &vertices,
            QVector<unsigned int> &indices,
            WS2Common::CollisionGrid &grid
            ) : TriangleIntersectionGrid(vertices, grid) {
        sortIntersections(indices, grid);
    }

    void TriangleIntersectionGrid::sortIntersections(
            QVector<unsigned int> &indices,
            WS2Common::CollisionGrid &grid
            ) {
        //Loop over all grid tiles
        //xi/yi = X iterator/Y iterator
        //TODO: Parallelise this maybe? There's probably no need to, but maybe?
        unsigned int totalTiles = grid.getGridStepCount().x * grid.getGridStepCount().y;
        for (unsigned int xi = 0; xi < grid.getGridStepCount().x; xi++) {
            for (unsigned int yi = 0; yi < grid.getGridStepCount().y; yi++) {
                //Can't seem to overwrite the same line with QDebug
                //unsigned int tilesDone = (grid.getGridStepCount().y * xi) + (yi + 1);
                //fprintf(stdout, "\rChecking tile: %03d, %03d (%04d / %04d | %06.2f%%)",
                        //xi + 1,
                        //yi + 1,
                        //tilesDone,
                        //totalTiles,
                        //tilesDone / (double) totalTiles * 100.0
                        //);

                //The grid tile spans from topLeft to bottomRight
                //Add some padding around the grid, in case any triangles lie on the grid tile edge
                const float PADDING = 0.7f;
                glm::vec2 topLeft = grid.getGridStart();
                topLeft.x += grid.getGridStep().x * xi - PADDING;
                topLeft.y += grid.getGridStep().y * yi - PADDING;
                glm::vec2 bottomRight = topLeft;
                bottomRight.x += grid.getGridStep().x + PADDING;
                bottomRight.y += grid.getGridStep().y + PADDING;
                WS2Common::AABB2 tileAABB(topLeft, bottomRight);

                //Now iterate over every triangle, give the triangle an AABB, and check if they intersect
                for (int i = 0; i < indices.size(); i += 3) {
                    //va/b/c = Vertex A/B/C (Positions)
                    const glm::vec3 va = vertices.at(indices.at(i)).position;
                    const glm::vec3 vb = vertices.at(indices.at(i + 1)).position;
                    const glm::vec3 vc = vertices.at(indices.at(i + 2)).position;

                    //Ignore Y - The collision grid only factors X/Z axes
                    glm::vec2 topLeft(
                            std::min(std::min(va.x, vb.x), vc.x),
                            std::min(std::min(va.z, vb.z), vc.z)
                            );
                    glm::vec2 bottomRight(
                            std::max(std::max(va.x, vb.x), vc.x),
                            std::max(std::max(va.z, vb.z), vc.z)
                            );
                    WS2Common::AABB2 triangleAABB(topLeft, bottomRight);

                    if (doAABBsIntersect(tileAABB, triangleAABB)) {
                        indicesGrid[yi][xi].append(i / 3);
                    }
                }
            }
        }

        //New line, so new log messages aren't printed on the same line as the Checking tile line
        //fprintf(stdout, "\n");
    }

    QVector<QVector<QVector<quint16>>>& TriangleIntersectionGrid::getIndicesGrid() {
        return indicesGrid;
    }

    const QVector<QVector<QVector<quint16>>>& TriangleIntersectionGrid::getIndicesGrid() const {
        return indicesGrid;
    }

    bool TriangleIntersectionGrid::doAABBsIntersect(WS2Common::AABB2 a, WS2Common::AABB2 b) {
        if (a.b.x < b.a.x) return false; //a is left of b
        if (a.a.x > b.b.x) return false; //a is right of b
        if (a.b.y < b.a.y) return false; //a is above b
        if (a.a.y > b.b.y) return false; //a is below b
        return true; //Boxes overlap
    }
}


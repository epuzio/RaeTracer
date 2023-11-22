#ifndef AABB_H
#define AABB_H

#include "vec3.h"
#include "triangle.h"
class AABB {
    public:
    vec3 minPoint = vec3(INT_MAX, INT_MAX, INT_MAX);
    vec3 maxPoint = vec3(INT_MIN, INT_MIN, INT_MIN);
    vector<triangle> triangles;
    AABB(){}
    void add(triangle t){
        triangles.push_back(t);
    }
    void calculateMinAndMaxPoints(){
        for (auto t: triangles){
            minPoint = vec3(min(minPoint.x, t.triangleMin().x), min(minPoint.y, t.triangleMin().y), min(minPoint.z, t.triangleMin().z));
            maxPoint = vec3(max(maxPoint.x, t.triangleMax().x), max(maxPoint.y, t.triangleMax().y), max(maxPoint.z, t.triangleMax().z));
        }
    }

    vector<vec3> calculateBoundingBox(const std::vector<triangle>& triangles) {
        if (triangles.empty()) {
            // Return an invalid bounding box or handle the case according to your requirements
            return vector<vec3>(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0));
        }

        calculateMinAndMaxPoints();

        return vector<vec3>(minPoint, maxPoint);
    }
};

struct BVHNode {
    AABB boundingBox;
    BVHNode* leftChild;
    BVHNode* rightChild;
};


string longestAxis(const vec3& minPoint, const vec3& maxPoint) {
    vec3 boxExtent = maxPoint - minPoint;

    if (boxExtent.x > boxExtent.y && boxExtent.x > boxExtent.z) {
        return "x";
    } else if (boxExtent.y > boxExtent.x && boxExtent.y > boxExtent.z) {
        return "y";
    } else {
        return "z";
    }
}

BVHNode* buildBVH(const std::vector<triangle>& triangles, int depth = 3) {
    if (triangles.empty() || depth <= 0) {
        return nullptr;
    }

    BVHNode* node = new BVHNode();
    node->boundingBox.triangles = triangles;
    node->boundingBox.calculateMinAndMaxPoints();

    if (triangles.size() == 1) {
        // Create a leaf node and store the single triangle
        // Set node->leftChild and node->rightChild to nullptr
        // Other necessary operations for leaf nodes
        return node;
    } else {
        // Split triangles into left and right child groups
        // For simplicity, let's consider a simple median split
        vector<triangle> leftTriangles;
        vector<triangle> rightTriangles;

        // Determine the longest axis of the bounding box
        string longest = longestAxis(node->boundingBox.minPoint, node->boundingBox.maxPoint);

        // Sort triangles based on the longest axis
        sort(node->boundingBox.triangles.begin(), node->boundingBox.triangles.end(), [longest](const triangle& a, const triangle& b) {
            // Compare centroids along the longest axis
            vec3 centroidA = a.centroid();
            vec3 centroidB = b.centroid();

            if (longest == "x") {
                return centroidA.x < centroidB.x;
            } else if (longest == "y") {
                return centroidA.y < centroidB.y;
            } else {
                return centroidA.z < centroidB.z;
            }
        });

        // Split the triangles into left and right groups
        size_t const median = triangles.size() / 2;
        leftTriangles.insert(leftTriangles.end(), node->boundingBox.triangles.begin(), node->boundingBox.triangles.begin() + median);
        rightTriangles.insert(rightTriangles.end(), node->boundingBox.triangles.begin() + median, node->boundingBox.triangles.end());

        // Recursively build left and right subtrees
        node->leftChild = buildBVH(leftTriangles , depth - 1);
        node->rightChild = buildBVH(rightTriangles, depth - 1);

        // Update node's bounding box to encapsulate left and right subtrees
        AABB leftBox = node->leftChild->boundingBox;
        AABB rightBox = node->rightChild->boundingBox;
        node->boundingBox.minPoint = vec3(min(leftBox.minPoint, rightBox.minPoint));
        node->boundingBox.maxPoint = vec3(max(leftBox.maxPoint, rightBox.maxPoint));
        return node;
    }
}

#endif
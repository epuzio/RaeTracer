#ifndef BOX_H
#define BOX_H

#include "vec3.h"
#include "triangle.h"
class box {
    public:
    vec3 minPoint = vec3(INT_MAX, INT_MAX, INT_MAX);
    vec3 maxPoint = vec3(INT_MIN, INT_MIN, INT_MIN);
    vector<triangle> triangles;

    box(){}
    void add(triangle t){
        triangles.push_back(t);
    }
    
    void calculateMinAndMaxPoints(){
        for (auto t: triangles){
            minPoint = vec3(min(minPoint.x, t.triangleMin().x), min(minPoint.y, t.triangleMin().y), min(minPoint.z, t.triangleMin().z));
            maxPoint = vec3(max(maxPoint.x, t.triangleMax().x), max(maxPoint.y, t.triangleMax().y), max(maxPoint.z, t.triangleMax().z));
        }
    }

    bool hit(const ray& r, double ray_min, double ray_max, hit_record& rec) const {
        for (auto t : triangles){
            if (t.hit(r, ray_min, ray_max, rec)){
                return true;
            }
        }
        return false;
    };
    
};

class BVHNode : public shape{
    public:
        box boundingBox;
        BVHNode* leftChild;
        BVHNode* rightChild;

        bool hit(const ray& r, double ray_min, double ray_max, hit_record& rec) const {
            if(leftChild == nullptr && rightChild == nullptr){ //if reached child node
                return boundingBox.hit(r, ray_min, ray_max, rec); //hopefully works with closest_so_far
            }

            float tMin = (boundingBox.minPoint.x - r.origin().x) / r.direction().x;
            float tMax = (boundingBox.minPoint.x - r.origin().x) / r.direction().x;

            if (tMin > tMax) swap(tMin, tMax);

            float tyMin = (boundingBox.minPoint.y - r.origin().y) / r.direction().y;
            float tyMax = (boundingBox.maxPoint.y - r.origin().y) / r.direction().y;

            if (tyMin > tyMax) swap(tyMin, tyMax);

            if ((tMin > tyMax) || (tyMin > tMax)) return false;

            if (tyMin > tMin) tMin = tyMin;
            if (tyMax < tMax) tMax = tyMax;

            float tzMin = (boundingBox.minPoint.z - r.origin().z) / r.direction().z;
            float tzMax = (boundingBox.maxPoint.z - r.origin().z) / r.direction().z;

            if (tzMin > tzMax) swap(tzMin, tzMax);

            if ((tMin > tzMax) || (tzMin > tMax)) return false;

            //Call hit on left child, then right child.
            if(leftChild != nullptr){leftChild->hit(r, ray_min, ray_max, rec);}
            if(rightChild != nullptr){rightChild->hit(r, ray_min, ray_max, rec);}
        }

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
                BVHNode* leafNode = new BVHNode();
                
                // Store the single triangle in the leaf node's bounding box
                leafNode->boundingBox.triangles.push_back(triangles[0]);
                leafNode->boundingBox.calculateMinAndMaxPoints(); // Calculate min and max points
                
                // Set node's children to nullptr since it's a leaf node
                leafNode->leftChild = nullptr;
                leafNode->rightChild = nullptr;
                
                return leafNode;
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
                box leftBox = node->leftChild->boundingBox;
                box rightBox = node->rightChild->boundingBox;
                node->boundingBox.minPoint = vec3(min(leftBox.minPoint, rightBox.minPoint));
                node->boundingBox.maxPoint = vec3(max(leftBox.maxPoint, rightBox.maxPoint));
                return node;
            }
        }
};




#endif
#ifndef QUADTREE_H
#define QUADTREE_H
#include <stdlib.h>
#include <vector>
#include <utility>
#include "object.h"
using namespace std;

class QuadTree{
public:
    QuadTree(float _centerX, float _centerY, float _width, float _height, int _nodeCapacity);
    ~QuadTree();
    bool insert(Object* obj);
    void subdivide();
    bool containObject(Object* obj);
    vector<Object*> queryRectangle(float _rectX, float _rectY, float _rectW, float _rectH);
    vector<Object*> queryCircle(Object* circle, float maxRadius);
    vector<Object*> inCollision(Object* circle, float maxRadius);

    QuadTree* getQtreeAtObject(Object* obj);
    bool intersects(float _rectX, float _rectY, float _rectW, float _rectH);

    void draw();
    void clean();
    //----- Getters and Setters -----//
    pair<float,float> getCenter() const;
    float getWidth() const;
    float getHeight() const;
    vector<Object*> getObjects() const;
    bool getIsSubdivaded() const;

private:
    // Rect info
    float centerX, centerY;
    float width;
    float height;
    // Points info
    int nodeCapacity;
    vector<Object*> objects;
    // Children
    QuadTree* northEast;
    QuadTree* northWest;
    QuadTree* southEast;
    QuadTree* southWest;
    bool isSubdivaded;
    bool drawn;
};

#endif // QUADTREE_H

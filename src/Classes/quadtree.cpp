#include "quadtree.h"
#include "utils.h"
#include <GL/glut.h>
#include <iostream>
using namespace std;

QuadTree::QuadTree(float _centerX, float _centerY, float _width, float _height, int _nodeCapacity):
    centerX(_centerX), centerY(_centerY), width(_width), height(_height),
    nodeCapacity(_nodeCapacity),isSubdivaded(false),drawn(false)
{
  northEast = NULL;
  northWest = NULL;
  southEast = NULL;
  southWest = NULL;
}

QuadTree::~QuadTree()
{
    if(isSubdivaded){
        delete northEast;
        delete northWest;
        delete southEast;
        delete southWest;
    }
}

bool QuadTree::insert(Object* obj)
{
  if(!containObject(obj)){
      return false;
  }

  if(int(objects.size())<nodeCapacity){
      objects.push_back(obj);
      return true;
  }else{
      if(!isSubdivaded) subdivide();

      if(northEast->insert(obj))return true;
      if(northWest->insert(obj))return true;
      if(southEast->insert(obj))return true;
      if(southWest->insert(obj))return true;
      return false;
  }
}

void QuadTree::subdivide()
{
    isSubdivaded = true;
    float subCenterX, subCenterY;

    subCenterX = (centerX+width/4);
    subCenterY = (centerY+height/4);
    northEast = new QuadTree(subCenterX, subCenterY, width/2, height/2, nodeCapacity);

    subCenterX = (centerX-width/4);
    subCenterY = (centerY+height/4);
    northWest = new QuadTree(subCenterX, subCenterY, width/2, height/2, nodeCapacity);

    subCenterX = (centerX+width/4);
    subCenterY = (centerY-height/4);
    southEast = new QuadTree(subCenterX, subCenterY, width/2, height/2, nodeCapacity);

    subCenterX = (centerX-width/4);
    subCenterY = (centerY-height/4);
    southWest = new QuadTree(subCenterX, subCenterY, width/2, height/2, nodeCapacity);
}

bool QuadTree::containObject(Object* obj)
{
    return ((obj->getX()>=centerX-width/2)  && (obj->getX()<=centerX+width/2)) &&
           ((obj->getY()>=centerY-height/2) && (obj->getY()<=centerY+height/2));
}

QuadTree* QuadTree::getQtreeAtObject(Object* obj){
    if(isSubdivaded){
        if(northEast->containObject(obj))
            return northEast->getQtreeAtObject(obj);
        else if(northWest->containObject(obj))
            return northWest->getQtreeAtObject(obj);
        else if(southEast->containObject(obj))
            return southEast->getQtreeAtObject(obj);
        else if(southWest->containObject(obj))
            return southWest->getQtreeAtObject(obj);
    }else{
        return this;
    }
    return this;
}

bool QuadTree::intersects(float _rectX, float _rectY, float _rectW, float _rectH)
{
    return !(centerX+width/2  <= _rectX-_rectW/2 ||
             centerX-width/2  >= _rectX+_rectW/2 ||
             centerY+height/2 <= _rectY-_rectH/2 ||
             centerY-height/2 >= _rectY+_rectH/2
             );
}

vector<Object*> QuadTree::queryRectangle(float _rectX, float _rectY, float _rectW, float _rectH){
  vector<Object*> result;
  if(intersects(_rectX, _rectY, _rectW*4, _rectH*4)){
    if(isSubdivaded){
      vector<Object*> resultNE = northEast->queryRectangle(_rectX, _rectY, _rectW, _rectH);
      result.insert(result.end(), resultNE.begin(), resultNE.end());

      vector<Object*> resultNW = northWest->queryRectangle(_rectX, _rectY, _rectW, _rectH);
      result.insert(result.end(), resultNW.begin(), resultNW.end());

      vector<Object*> resultSE = southEast->queryRectangle(_rectX, _rectY, _rectW, _rectH);
      result.insert(result.end(), resultSE.begin(), resultSE.end());

      vector<Object*> resultSW = southWest->queryRectangle(_rectX, _rectY, _rectW, _rectH);
      result.insert(result.end(), resultSW.begin(), resultSW.end());
    }else{
      result.insert(result.end(), objects.begin(), objects.end());
    }
  }

  return result;
}

vector<Object*> QuadTree::queryCircle(Object* circle, float maxRadius){
  vector<Object*> nearObjects;
  vector<Object*> resultObjects;
  nearObjects = queryRectangle(circle->getX(), circle->getY(), maxRadius*4, maxRadius*4);
  for(auto obj : nearObjects){
    if( distanceTwoObjects(circle,obj) <= maxRadius){
      resultObjects.push_back(obj);
    }
  }
  return resultObjects;
}

vector<Object*> QuadTree::inCollision(Object* circle, float maxRadius){
  vector<Object*> nearObjects;
  vector<Object*> collidingObjects;
  nearObjects = queryRectangle(circle->getX(), circle->getY(), maxRadius*4, maxRadius*4);
  for(auto obj : nearObjects){
    if( distanceTwoObjects(circle,obj) <= circle->getRadius()+obj->getRadius() && obj->getId()!=circle->getId()){
      collidingObjects.push_back(obj);
    }
  }
  return collidingObjects;
}

void QuadTree::draw()
{
  if(!drawn){
    glLineWidth(1);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f((centerX-width/2)/10, (centerY-height/2)/10,0);
    glVertex3f((centerX-width/2)/10, (centerY+height/2)/10,0);
    glEnd();glBegin(GL_LINES);
    glVertex3f((centerX-width/2)/10, (centerY+height/2)/10,0);
    glVertex3f((centerX+width/2)/10, (centerY+height/2)/10,0);
    glEnd();glBegin(GL_LINES);
    glVertex3f((centerX+width/2)/10, (centerY+height/2)/10,0);
    glVertex3f((centerX+width/2)/10, (centerY-height/2)/10,0);
    glEnd();glBegin(GL_LINES);
    glVertex3f((centerX+width/2)/10, (centerY-height/2)/10,0);
    glVertex3f((centerX-width/2)/10, (centerY-height/2)/10,0);
    glEnd();

    if(isSubdivaded){
        northEast->draw();
        northWest->draw();
        southEast->draw();
        southWest->draw();
    }
  }
  drawn=true;
}

void QuadTree::clean()
{
  objects.clear();
    if(isSubdivaded){
        northEast->clean();
        northWest->clean();
        southEast->clean();
        southWest->clean();
    }
}

//----- Getters and Setters -----//
pair<float,float> QuadTree::getCenter() const
{
    return make_pair(centerX, centerY);
}

float QuadTree::getWidth() const
{
    return width;
}

float QuadTree::getHeight() const
{
    return height;
}

vector<Object*> QuadTree::getObjects() const
{
    return objects;
}

bool QuadTree::getIsSubdivaded() const{
  return isSubdivaded;
}

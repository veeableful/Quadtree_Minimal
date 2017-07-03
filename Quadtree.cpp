#include "Quadtree.h"
#include "Object.h"

using namespace std;

Quadtree::Quadtree(float _x, float _y, float _width, float _height, int _level, int _maxLevel) :
    x       (_x),
    y       (_y),
    width   (_width),
    height  (_height),
    level   (_level),
    maxLevel(_maxLevel)
{
    if (level == maxLevel)
        return;

    const float halfWidth  = width * 0.5f;
    const float halfHeight = height * 0.5f;

    NW = new Quadtree(x, y, halfWidth, halfHeight, level+1, maxLevel);
    NE = new Quadtree(x + halfWidth, y, halfWidth, halfHeight, level+1, maxLevel);
    SW = new Quadtree(x, y + halfHeight, halfWidth, halfHeight, level+1, maxLevel);
    SE = new Quadtree(x + halfWidth, y + halfHeight, halfWidth, halfHeight, level+1, maxLevel);
}

Quadtree::~Quadtree() {
    if (level == maxLevel)
        return;

    delete NW;
    delete NE;
    delete SW;
    delete SE;
}

void Quadtree::AddObject(Object *object) {
    if (level == maxLevel)
        objects.push_back(object);
    else if (contains(NW, object))
        NW->AddObject(object);
    else if (contains(NE, object))
        NE->AddObject(object);
    else if (contains(SW, object))
        SW->AddObject(object);
    else if (contains(SE, object))
        SE->AddObject(object);
    else if (contains(this, object))
        objects.push_back(object);
}

vector<Object*> Quadtree::GetObjectsAt(float _x, float _y) {
    vector<Object*> returnObjects, childReturnObjects;

    if (level == maxLevel)
        return objects;

    if (!objects.empty())
        returnObjects = objects;

    const float halfWidth  = width * 0.5f;
    const float halfHeight = height * 0.5f;

    if (_x > x + halfWidth && _x < x + width)
        if (_y > y + halfHeight && _y < y + height)
            childReturnObjects = SE->GetObjectsAt(_x, _y);
        else if (_y > y && _y <= y + halfHeight)
            childReturnObjects = NE->GetObjectsAt(_x, _y);
    else if (_x > x && _x <= x + halfWidth)
        if (_y > y + halfHeight && _y < y + height)
            childReturnObjects = SW->GetObjectsAt(_x, _y);
        else if (_y > y && _y <= y + halfHeight)
            childReturnObjects = NW->GetObjectsAt(_x, _y);

    if (childReturnObjects.size() > 0)
        returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());

    return returnObjects;
}

void Quadtree::Clear() {
    if (level < maxLevel) {
        NW->Clear();
        NE->Clear();
        SW->Clear();
        SE->Clear();
    }

    if (!objects.empty())
        objects.clear();
}

bool Quadtree::contains(Quadtree *child, Object *object) {
    return !(object->x < child->x ||
             object->y < child->y ||
             object->x > child->x + child->width  ||
             object->y > child->y + child->height ||
             object->x + object->width  < child->x ||
             object->y + object->height < child->y ||
             object->x + object->width  > child->x + child->width ||
             object->y + object->height > child->y + child->height);
}

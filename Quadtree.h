#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include <vector>

using namespace std;

class Quadtree;
class Object;

class Quadtree {
public:
						Quadtree(float x, float y, float width, float height, int level, int maxLevel);

						~Quadtree();

	void					AddObject(Object *object);
	vector<Object*>				GetObjectsAt(float x, float y);
	void					Clear();

private:
	float					x;
	float					y;
	float					width;
	float					height;
	int					level;
	int					maxLevel;
	vector<Object*>				objects;

	Quadtree *				parent;
	Quadtree *				NW;
	Quadtree *				NE;
	Quadtree *				SW;
	Quadtree *				SE;

	bool					contains(Quadtree *child, Object *object);
};

#endif

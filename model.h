#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometrie.h"

class Model {
private:
 std::vector<Vec3f> verts_;
 std::vector<Vec3f> norms_;
 std::vector<std::vector<int> > faces_;
public:
 Model(const char *filename);
 int nverts();
 int nfaces();
 Vec3f vert(int i);
	Vec3f norm(int i);
 std::vector<int> face(int idx);
};

#endif

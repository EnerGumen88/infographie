/** \file model.h
 * \brief Functions for model
 * \author CARON Frédéric
 */

#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometrie.h"

class Model {
private:
 std::vector<Vec3f> verts_;
 std::vector<Vec3f> norms_;
 std::vector<Vec2f> textures_;
 std::vector<std::vector<Vec3i> > faces_;
public:
 Model(const char *filename);
 int nverts();
 int nfaces();
 Vec3f vert(int i);
 Vec3f norm(int i);
 Vec2f texture(int i);
 std::vector<Vec3i> face(int idx);
};

#endif

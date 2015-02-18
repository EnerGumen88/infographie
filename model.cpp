#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"


Model::Model (const char *filename) : verts_(), norms_(), textures_(), faces_(){
  std::ifstream file(filename, std::ios::in);
  
  if (file){
    std::string line;
    while(!file.eof()){
      std::getline(file,line);
      std::istringstream line_stream(line.c_str());
      char trash_char;
      if (line.compare(0, 2, "v ") == 0){
	line_stream >> trash_char;
	Vec3f v;
	for (int i=0;i<3;i++) line_stream >> v[i];
	verts_.push_back(v);
      }
      else if (line.compare(0, 2, "vt") == 0){
	line_stream >> trash_char >> trash_char;
	Vec2f vt;
	for (int i=0;i<2;i++) line_stream >> vt[i];
	textures_.push_back(vt);
      }
      else if (line.compare(0, 2, "vn") == 0){
	line_stream >> trash_char >> trash_char;
	Vec3f vn;
	for (int i=0;i<3;i++) line_stream >> vn[i];
	norms_.push_back(vn);
      }
      else if (line.compare(0, 1, "f") == 0){
	std::vector<Vec3i> f;
	line_stream >> trash_char;
	Vec3i fi;
	for (int i=0;i<3;i++) {
	  line_stream >> fi[0] >> trash_char >> fi[1] >> trash_char >> fi[2];
	  fi[0]--;
	  fi[1]--;
	  fi[2]--;
	  f.push_back(fi);
	}
	faces_.push_back(f);
      }
    }
  }
  else
    std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;

}


int Model::nverts() {
 return (int)verts_.size();
}


int Model::nfaces() {
 return (int)faces_.size();
}


std::vector<Vec3i> Model::face(int idx) {
 return faces_[idx];
}

Vec3f Model::vert(int i) {
 return verts_[i];
}

Vec3f Model::norm(int i) {
 return norms_[i];
}

Vec2f Model::texture(int i) {
 return textures_[i];
}

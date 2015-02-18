#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"


Model::Model (const char *filename) : verts_(), faces_(){
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
	for (int i=0;i<3;i++) line_stream >> v.raw[i];
	verts_.push_back(v);
      }
      else if (line.compare(0, 1, "f") == 0){
	std::vector<int> f;
	line_stream >> trash_char;
	int trash_int,fi;
	for (int i=0;i<3;i++) {
	  line_stream >> fi >> trash_char >> trash_int >> trash_char >> trash_int;
	  fi--;
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


std::vector<int> Model::face(int idx) {
 return faces_[idx];
}

Vec3f Model::vert(int i) {
 return verts_[i];
}
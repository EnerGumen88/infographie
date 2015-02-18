#include "tgaimage.h"
#include "model.h"
#include <cmath>



void line (TGAImage &image, int x0, int y0, int x1, int y1){
  
  bool steep = false;
  if (std::abs(x0-x1)<std::abs(y0-y1)) {
    std::swap(x0, y0);
    std::swap(x1, y1);
    steep = true;
  }
  if (x0>x1) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }
  int dx = x1-x0;
  int dy = y1-y0;
  int derrordx = 2*std::abs(dy);
  int errordx = 0;
  int y = y0;
  for (int x=x0; x<=x1; x++) {
    if (steep) {
      image.set(y, x, TGAColor(128, 1));
    } else {
      image.set(x, y, TGAColor(128, 1));
    }
    errordx += derrordx;
    if (errordx>dx) {
      y += (y1>y0?1:-1);
      errordx -= 2*dx;
    }
  }
}


void triangle_plein (TGAImage &image,int x0, int y0, int x1, int y1, int x2, int y2){
 
  if(x0>x1){
    std::swap(x0,x1);
    std::swap(y0,y1);
  }
  if(x1>x2){
    std::swap(x1,x2);
    std::swap(y1,y2);
  }
  if(x0>x1){
    std::swap(x0,x1);
    std::swap(y0,y1);
  }
  
  
  float dx1 = x1-x0;
  float dx2 = x2-x0;
  float dy1 = y1-y0;
  float dy2 = y2-y0;
  float dx3 = x2-x1;
  float dy3 = y2-y1;
  float y11=y0;
  float y12=y0;
  
  if(dx2==0){
    line(image,x0,y0,x1,y1);
    line(image,x1,y1,x2,y2);
  }
  
  else {
    if(dx1!=0){
      for(int x=x0;x<=x1;x++){
	y11 += dy1/std::abs(dx1);
	y12 += dy2/std::abs(dx2);
	
	line(image, x, y11,x, y12); 
      }
    }
     
    if(dx3!=0){
      y11 = y1;
      for (int x=x1;x<=x2;x++){
	y11 += dy3/std::abs(dx3);
	y12 += dy2/std::abs(dx2);
	
	line(image, x, y11,x, y12); 
      }
    }
  }
}

void triangle (TGAImage &image,int x0, int y0, int x1, int y1, int x2, int y2){
  line(image, x0, y0, x1, y1);
  line(image, x0, y0, x2, y2);
  line(image, x1, y1, x2, y2);
}

int main() {
	
	TGAImage image (1000,1000,1);
	Model *model = new Model("./african_head.obj");
	
	int zbuffer[1000][1000];
	
	for (int i=0;i<1000;i++)
	  for(int j=0;j<1000;j++)
	    zbuffer[i][j]=255;
	  
	for (int i=0;i<model->nfaces();i++){
	  std::vector<int> face = model->face(i);
	  Vec3f v0 = model->vert(face[0]);
	  Vec3f v1 = model->vert(face[1]);
	  Vec3f v2 = model->vert(face[2]);
	  int x0 = (v0.x+1)*500;
	  int y0 = (v0.y+1)*500;
	  int z0 = (v0.z+1)*127.5;
	  int x1 = (v1.x+1)*500;
	  int y1 = (v1.y+1)*500;
	  int z1 = (v1.z+1)*127.5;
	  int x2 = (v2.x+1)*500;
	  int y2 = (v2.y+1)*500;
	  int z2 = (v2.z+1)*127.5;
	  triangle(image, x0, y0, x1, y1, x2, y2);
	  
	}
	
	image.flip_vertically();
	image.write_tga_file("tete.tga");
	
	
	return 0;
}

#include "tgaimage.h"
#include "model.h"
#include <cmath>
#include <limits> 



void line (TGAImage &image, int x0, int y0, int x1, int y1, TGAColor color){
  
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
      image.set(y, x, color);
    } else {
      image.set(x, y, color);
    }
    errordx += derrordx;
    if (errordx>dx) {
      y += (y1>y0?1:-1);
      errordx -= 2*dx;
    }
  }
}


void triangle_plein (TGAImage &image,int x0, int y0, int x1, int y1, int x2, int y2, TGAColor color){
 
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
    line(image, x0, y0, x1, y1, color);
    line(image, x1, y1, x2, y2, color);
  }
  
  else {
    if(dx1!=0){
      for(int x=x0;x<=x1;x++){
	y11 += dy1/std::abs(dx1);
	y12 += dy2/std::abs(dx2);
	
	line(image, x, y11,x, y12, color); 
      }
    }
     
    if(dx3!=0){
      y11 = y1;
      for (int x=x1;x<=x2;x++){
	y11 += dy3/std::abs(dx3);
	y12 += dy2/std::abs(dx2);
	
	line(image, x, y11,x, y12, color); 
      }
    }
  }
}

void triangle (TGAImage &image,int x0, int y0, int x1, int y1, int x2, int y2, TGAColor color){
  line(image, x0, y0, x1, y1, color);
  line(image, x0, y0, x2, y2, color);
  line(image, x1, y1, x2, y2, color);
}

void triangle_z (TGAImage &image,int x0, int y0, int z0, int x1, int y1, int z1, int x2, int y2, int z2, TGAColor color, int *zbuffer){
	if (y0==y1 && y0==y2)
		return;
	if (y0>y1){
		std::swap(x0,x1);
		std::swap(y0,y1);
		std::swap(z0,z1);
	}

	if (y0>y2){
		std::swap(x0,x2);
		std::swap(y0,y2);
		std::swap(z0,z2);
	}

	if (y1>y2){
		std::swap(x1,x2);
		std::swap(y1,y2);
		std::swap(z1,z2);
	}

	int height = y2 - y0;
	int height1 = y1 - y0;
	int height2 = y2 - y1;

	if (height1 != 0){
		for (int i=0; i<height1; i++){
			float Ax = x0 + i*(x2-x0)/height;
			float Az = z0 + i*(z2-z0)/height;
			float Bx = x0 + i*(x1-x0)/height1;
			float Bz = z0 + i*(z1-z0)/height1;

			if (Ax > Bx){
				std::swap(Ax,Bx);
				std::swap(Az,Bz);
			}

			for (int j=0; j<=Bx-Ax; j++){
				float z = Az + j*(Bz-Az)/(Bx-Ax); 
				if (zbuffer[j+(int)Ax+(y0+i)*1000] < z){
					zbuffer[j+(int)Ax+(y0+i)*1000] = z;
					image.set(j+Ax,y0+i, color);
				}
			}
		}
	}
	
	if (height2 != 0){
		for (int i=0; i<height2; i++){
			float Ax = x0 + (i+height1)*(x2-x0)/height;
			float Az = z0 + (i+height1)*(z2-z0)/height;
			float Bx = x1 + i*(x2-x1)/height2;
			float Bz = z1 + i*(z2-z1)/height2;

			if (Ax > Bx){
				std::swap(Ax,Bx);
				std::swap(Az,Bz);
			}

			for (int j=0; j<=Bx-Ax; j++){
				float z = Az + j*(Bz-Az)/(Bx-Ax); 
				if (zbuffer[j+(int)Ax+(y0+i+height1)*1000] < z){
					zbuffer[j+(int)Ax+(y0+i+height1)*1000] = z;
					image.set(j+Ax,y0+i+height1, color);
				}
			}
		}
	}

}



int main() {
	
	int width = 1000;
	int height = 1000;
	int depth = 255;
	TGAImage image (width,height,3);
	Vec3f light = Vec3f(0,0,-1);

	Model *model = new Model("./african_head.obj");
	int *zbuffer = new int [width*height];
	for (int i=0; i<width*height; i++){
		zbuffer[i] = std::numeric_limits<int>::min();
	}
	
		  
	for (int i=0;i<model->nfaces();i++){
	  std::vector<int> face = model->face(i);
	  Vec3f v0 = model->vert(face[0]);
	  Vec3f v1 = model->vert(face[1]);
	  Vec3f v2 = model->vert(face[2]);
	  int x0 = (v0.x+1)*(width/2);
	  int y0 = (v0.y+1)*(height/2);
	  int z0 = (v0.z+1)*(depth/2);
	  int x1 = (v1.x+1)*(width/2);
	  int y1 = (v1.y+1)*(height/2);
	  int z1 = (v1.z+1)*(depth/2);
	  int x2 = (v2.x+1)*(width/2);
	  int y2 = (v2.y+1)*(height/2);
	  int z2 = (v2.z+1)*(depth/2);

	Vec3f normal = (v2-v0)^(v1-v0);
	normal.normalize();
	if (normal*light > 0)
		triangle_z(image,x0,y0,z0,x1,y1,z1,x2,y2,z2,TGAColor(normal*light*255,normal*light*255,normal*light*255,255), zbuffer);

	  
	}
	
	image.flip_vertically();
	image.write_tga_file("tete.tga");
	
	
	
	return 0;
}

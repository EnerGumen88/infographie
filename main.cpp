#include "tgaimage.h"
#include "model.h"
#include <cmath>
#include <limits> 
#include <cstring>
#include <stdio.h>
#include <string>

int width = 1024;
int height = 1024;
int depth = 255;
Vec3f light(0,0,-1);
Vec3f origine(0,0,0);
Vec3f camera(0,0,1);
Vec3f u(0,1,0);



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


void triangle_d (TGAImage &image,int x0, int y0, int z0, int x1, int y1, int z1, int x2, int y2, int z2, float c0, float c1, float c2, int *zbuffer) {
  if (y0==y1 && y0==y2)
		return;
	if (y0>y1){
		std::swap(x0,x1);
		std::swap(y0,y1);
		std::swap(z0,z1);
		std::swap(c0,c1);
	}

	if (y0>y2){
		std::swap(x0,x2);
		std::swap(y0,y2);
		std::swap(z0,z2);
		std::swap(c0,c2);
	}

	if (y1>y2){
		std::swap(x1,x2);
		std::swap(y1,y2);
		std::swap(z1,z2);
		std::swap(c1,c2);
	}

	int height = y2 - y0;
	int height1 = y1 - y0;
	int height2 = y2 - y1;
	float color = (c2 - c0)/height;
	
	

	if (height1 != 0){
		float color1 = (c1 - c0)/height1;
		for (int i=0; i<height1; i++){
			float Ax = x0 + i*(x2-x0)/height;
			float Az = z0 + i*(z2-z0)/height;
			float Ac = c0 + i*color;
			float Bx = x0 + i*(x1-x0)/height1;
			float Bz = z0 + i*(z1-z0)/height1;
			float Bc = c0 + i*color1;

			if (Ax > Bx){
				std::swap(Ax,Bx);
				std::swap(Az,Bz);
				std::swap(Ac,Bc);
				
			}
			for (int j=0; j<=Bx-Ax; j++){
				float z = Az + j*(Bz-Az)/(Bx-Ax); 
				if (zbuffer[j+(int)Ax+(y0+i)*1000] < z){
					float color_point = Ac + j*(Bc-Ac)/(Bx-Ax);
					zbuffer[j+(int)Ax+(y0+i)*1000] = z;
					image.set(j+Ax,y0+i, TGAColor(color_point*255,color_point*255,color_point*255,255));
				}
			}
		}
	}
	
	if (height2 != 0){
		float color2 = (c2 - c1)/height2;
		for (int i=0; i<height2; i++){
			float Ax = x0 + (i+height1)*(x2-x0)/height;
			float Az = z0 + (i+height1)*(z2-z0)/height;
			float Ac = c0 + (i+height1)*color;
			float Bx = x1 + i*(x2-x1)/height2;
			float Bz = z1 + i*(z2-z1)/height2;
			float Bc = c1 + i*color2;

			if (Ax > Bx){
				std::swap(Ax,Bx);
				std::swap(Az,Bz);
				std::swap(Ac,Bc);
			}
			for (int j=0; j<=Bx-Ax; j++){
				float z = Az + j*(Bz-Az)/(Bx-Ax); 
				if (zbuffer[j+(int)Ax+(y0+i+height1)*1000] < z){
					float color_point = Ac + j*(Bc-Ac)/(Bx-Ax);
					zbuffer[j+(int)Ax+(y0+i+height1)*1000] = z;
					image.set(j+Ax,y0+i+height1, TGAColor(color_point*255,color_point*255,color_point*255,255));
				}
			}
		}
	}
  
}


Matrix viewport	(int width, int height, int depth){
	Matrix vp = Matrix::identity();
	vp[0][0] = width/2.;
	vp[1][1] = height/2.;
	vp[2][2] = depth/2.;
	vp[0][3] = width/2.;
	vp[1][3] = height/2.;
	vp[2][3] = depth/2.;

	return vp;
}

Matrix projection (float r){
	Matrix projec = Matrix::identity();
	projec[2][3] = r;

	return projec;
}


Matrix lookat (Vec3f o, Vec3f camera, Vec3f u){
	Vec3f z = (camera - o).normalize();
	Vec3f x = cross(u,z).normalize();
	Vec3f y = cross(z,x).normalize();

	Matrix modelview = Matrix::identity();
	for (int i=0;i<3;i++){
		modelview[0][i] = x[i];
		modelview[1][i] = y[i];
		modelview[2][i] = z[i];
		modelview[i][3] = -o[i];
	}

	return modelview;
}


void triangle_m (TGAImage &image,Vec3f v0, Vec3f v1, Vec3f v2, TGAColor color){
  line(image, v0[0], v0[1], v1[0], v1[1], color);
  line(image, v0[0], v0[1], v2[0], v2[1], color);
  line(image, v1[0], v1[1], v2[0], v2[1], color);
}



void triangle_t (TGAImage &image,int x0, int y0, int z0, int x1, int y1, int z1, int x2, int y2, int z2, Vec2f t0 , Vec2f t1, Vec2f t2, int *zbuffer, TGAImage &texture ) {
  if (y0==y1 && y0==y2)
		return;
	if (y0>y1){
		std::swap(x0,x1);
		std::swap(y0,y1);
		std::swap(z0,z1);
		std::swap(t0,t1);
	}

	if (y0>y2){
		std::swap(x0,x2);
		std::swap(y0,y2);
		std::swap(z0,z2);
		std::swap(t0,t2);
	}

	if (y1>y2){
		std::swap(x1,x2);
		std::swap(y1,y2);
		std::swap(z1,z2);
		std::swap(t1,t2);
	}

	int height = y2 - y0;
	int height1 = y1 - y0;
	int height2 = y2 - y1;
	//int color = (c2 - c0)/height;
	Vec2f t20 = t2 - t0;
	Vec2f t21 = t2 - t1;
	Vec2f t10 = t1 - t0;
	

	if (height1 != 0){
		//float color1 = (c1 - c0)/height1;
		for (int i=0; i<height1; i++){
			float Ax = x0 + i*(x2-x0)/height;
			float Az = z0 + i*(z2-z0)/height;
			float At = t0[0] + i*(t20[0])/t20[1];
			float Bx = x0 + i*(x1-x0)/height1;
			float Bz = z0 + i*(z1-z0)/height1;
			float Bt = t0[0] + i*(t10[0])/t10[1];

			if (Ax > Bx){
				std::swap(Ax,Bx);
				std::swap(Az,Bz);
				std::swap(At,Bt);
				
			}
			for (int j=0; j<=Bx-Ax; j++){
				float z = Az + j*(Bz-Az)/(Bx-Ax); 
				if (zbuffer[j+(int)Ax+(y0+i)*1000] < z){
					float texture_point = At + j*(Bt-At)/(Bx-Ax);
					zbuffer[j+(int)Ax+(y0+i)*1000] = z;
					TGAColor color = texture.get(texture_point,t0[1]+i);
					image.set(j+Ax,y0+i, color);
				}
			}
		}
	}
	
	if (height2 != 0){
		//float color2 = (c2 - c1)/height2;
		for (int i=0; i<height2; i++){
			float Ax = x0 + (i+height1)*(x2-x0)/height;
			float Az = z0 + (i+height1)*(z2-z0)/height;
			float At = t0[0] + (i+height1)*(t20[0])/t20[1];
			float Bx = x1 + i*(x2-x1)/height2;
			float Bz = z1 + i*(z2-z1)/height2;
			float Bt = t1[0] + i*(t21[0])/t21[1];

			if (Ax > Bx){
				std::swap(Ax,Bx);
				std::swap(Az,Bz);
				std::swap(At,Bt);
			}
			for (int j=0; j<=Bx-Ax; j++){
				float z = Az + j*(Bz-Az)/(Bx-Ax); 
				if (zbuffer[j+(int)Ax+(y0+i+height1)*1000] < z){
					float texture_point = At + j*(Bt-At)/(Bx-Ax);
					zbuffer[j+(int)Ax+(y0+i+height1)*1000] = z;
					TGAColor color = texture.get(texture_point,t0[1]+i+t10[1]);
					image.set(j+Ax,y0+i+height1, color);
				}
			}
		}
	}
  
}

int main(int argc, char** argv) {
	
	const char* cmd;

	if (2==argc) 
		cmd = argv[1];	
	
	 else 
		cmd = "fil";


	
	TGAImage image (width,height,3);
	
      
	Model *model = new Model("./obj/african_head.obj");
	TGAImage texture;
	texture.read_tga_file("./obj/african_head_diffuse.tga");
	texture.flip_vertically();
	
	
	int *zbuffer = new int [width*height];
	for (int i=0; i<width*height; i++){
		zbuffer[i] = std::numeric_limits<int>::min();
	}
	
	Matrix transfo = Matrix::identity();
	Matrix vp = viewport(width, height, depth);
	Matrix projec = projection(1/(camera-origine).norm());
	Matrix modelview = lookat(origine,camera,u);
	light = proj<3>(projec*modelview*embed<4>(light,0.f));
	transfo = vp*projec*modelview;
	
	  
	for (int i=0;i<model->nfaces();i++){
	  std::vector<Vec3i> face = model->face(i);
	  Vec3f v0 = model->vert(face[0][0]);
	  Vec3f v1 = model->vert(face[1][0]);
	  Vec3f v2 = model->vert(face[2][0]);
	  int x0 = (v0.x+1)*(width/2);
	  int y0 = (v0.y+1)*(height/2);
	  int z0 = (v0.z+1)*(depth/2);
	  int x1 = (v1.x+1)*(width/2);
	  int y1 = (v1.y+1)*(height/2);
	  int z1 = (v1.z+1)*(depth/2);
	  int x2 = (v2.x+1)*(width/2);
	  int y2 = (v2.y+1)*(height/2);
	  int z2 = (v2.z+1)*(depth/2);

	if (strcmp(cmd,"fil") == 0) {
		triangle(image,x0,y0,x1,y1,x2,y2, TGAColor(255,255,255,255));
	}
	else if (strcmp(cmd,"z") == 0 || strcmp(cmd,"gouraud") == 0){
		Vec3f normal = cross((v2-v0),(v1-v0));
		normal.normalize();
		if (normal*light > 0){
			if (strcmp(cmd,"z") == 0){
				triangle_z(image,x0,y0,z0,x1,y1,z1,x2,y2,z2,TGAColor(normal*light*255,normal*light*255,normal*light*255,255), zbuffer);
			}
			else {
				Vec3f vn0 = model->norm(face[0][2]).normalize();
				Vec3f vn1 = model->norm(face[1][2]).normalize();
				Vec3f vn2 = model->norm(face[2][2]).normalize();
				triangle_d(image,x0,y0,z0,x1,y1,z1,x2,y2,z2,vn0*light,vn1*light,vn2*light, zbuffer);
			}
		}
	}
	else if (strcmp(cmd,"film") == 0 || strcmp(cmd,"zm") == 0 || strcmp(cmd,"gouraudm") == 0 || strcmp(cmd,"text") == 0){
		
		Vec3f v0 = proj<3>(transfo*(embed<4>(model->vert(face[0][0]))));
	  	Vec3f v1 = proj<3>(transfo*(embed<4>(model->vert(face[1][0]))));
	  	Vec3f v2 = proj<3>(transfo*(embed<4>(model->vert(face[2][0]))));

		if (strcmp(cmd,"film") == 0)
			triangle_m(image,v0, v1, v2, TGAColor(255,255,255,255));
		else {
			
			Vec3f normal = cross((v2-v0),(v1-v0));
			normal.normalize();
			if (normal*light > 0){
				if (strcmp(cmd,"zm") == 0)
					triangle_z(image,v0[0],v0[1],v0[2],v1[0],v1[1],v1[2],v2[0],v2[1],v2[2],TGAColor(normal*light*255,normal*light*255,normal*light*255,255), zbuffer);
				else if (strcmp(cmd,"gouraudm") == 0) {
					Vec3f vn0 = model->norm(face[0][2]).normalize();
					Vec3f vn1 = model->norm(face[1][2]).normalize();
					Vec3f vn2 = model->norm(face[2][2]).normalize();
					triangle_d(image,v0[0],v0[1],v0[2],v1[0],v1[1],v1[2],v2[0],v2[1],v2[2],vn0*light,vn1*light,vn2*light, zbuffer);
				}
				else if (strcmp(cmd,"text") == 0){
					Vec2f t0 = model->texture(face[0][1])*1024;
	  				Vec2f t1 = model->texture(face[1][1])*1024;
	  				Vec2f t2 = model->texture(face[2][1])*1024;				
					triangle_t(image,v0[0],v0[1],v0[2],v1[0],v1[1],v1[2],v2[0],v2[1],v2[2],t0,t1,t2,zbuffer,texture);
					  
					
				}
			}

		
	  
	  
		}
		
	
	}
	}
	
	image.flip_vertically();
	image.write_tga_file("tete.tga");
	
	
	
	return 0;
}

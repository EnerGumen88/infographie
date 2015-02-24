/** \file main.cpp
* \brief Function for draw model
* \author CARON Frédéric
*/
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

Matrix vp;
Matrix projec;
Matrix modelview;

float coeff_diff = 1.5;
float coeff_spec = 1.3;
float add_spec = 20.;



/** \fn Matrix viewport (int width, int height, int depth)
* \brief Create matrix viewport
*
* \param width height and depth of the image
* \return Matrix viewport
*/
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


/** \fn Matrix projection (float r)
* \brief Create matrix projection
*
* \param r inverse of distance between camera and origine
* \return Matrix projection
*/
Matrix projection (float r){
	Matrix projec = Matrix::identity();
	projec[2][3] = r;
	return projec;
}


/** \fn Matrix lookat (Vec3f o, Vec3f camera, Vec3f u)
* \brief Create matrix lookat
*
* \param o coordinates of origine
* \param camera coordinates of camera
* \param u coordinates of u
* \return Matrix lookat
*/
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


/** \fn void triangle_t (TGAImage &image,int x0, int y0, int z0, int x1, int y1, int z1, int x2, int y2, int z2, Vec2f t0 , Vec2f t1, Vec2f t2, int *zbuffer, TGAImage &texture, TGAImage &nm, TGAImage &spec)
* \brief Draw a triangle with texture (diffuse, normal maps and specular) and respect z-buffer
*
* \param image the image where you draw
* \param x0,y0,z0,x1,y1,z1,x2,y2,z2 the coordinates of vertex
* \param t0,t1,t2 the coordinates of each vertex in texture
* \param zbuffer
* \param texture image of texture
* \param nm image of normal maps
* \param spec image of specular
* \return void
*/
void triangle_t (TGAImage &image,int x0, int y0, int z0, int x1, int y1, int z1, int x2, int y2, int z2, Vec2f t0 , Vec2f t1, Vec2f t2, int *zbuffer, TGAImage &texture, TGAImage &nm, TGAImage &spec) {
	

	if (y0==y1 && y0==y2) return;
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
	
	if (height1 != 0){
		for (int i=0; i<height1; i++){
			float Ax = x0 + i*(x2-x0)/height;
			float Az = z0 + i*(z2-z0)/height;
			Vec2f At = t0 + (t2-t0)*(i/(float)height);
			float Bx = x0 + i*(x1-x0)/height1;
			float Bz = z0 + i*(z1-z0)/height1;
			Vec2f Bt = t0 + (t1-t0)*(i/(float)height1);
			if (Ax > Bx){
				std::swap(Ax,Bx);
				std::swap(Az,Bz);
				std::swap(At,Bt);
			}
			for (int j=0; j<=Bx-Ax; j++){
				float z = Az + j*(Bz-Az)/(Bx-Ax);
				if (zbuffer[j+(int)Ax+(y0+i)*1000] < z){
					Vec2f Ct = At + (Bt-At)*(j/(float)(Bx-Ax));
					zbuffer[j+(int)Ax+(y0+i)*1000] = z;
					TGAColor color = texture.get(Ct.x,Ct.y);
					TGAColor color_nm = nm.get(Ct.x,Ct.y);
					TGAColor color_spec = spec.get(Ct.x,Ct.y);
					Vec3f vecolor_nm(color_nm.r/255.f*2.f-1.f,color_nm.g/255.f*2.f-1.f,color_nm.b/255.f*2.f-1.f);
					Vec3f n = vecolor_nm.normalize();
					Vec3f l = light.normalize();
					Vec3f r = ((n*2.*(n*l))-l).normalize() ;
					float s = std::max(0.f,r*camera.normalize()) ;
					float spe = std::sqrt(color_spec.r*color_spec.r+color_spec.g*color_spec.g+color_spec.b*color_spec.b) ;
					float diff = std::max(0.f,vecolor_nm*light);
					TGAColor truecolor(color.r*(coeff_diff*diff+coeff_spec*std::pow(s,spe+add_spec)),color.g*(coeff_diff*diff+coeff_spec*std::pow(s,spe+add_spec)),color.b*(coeff_diff*diff+coeff_spec*std::pow(s,spe+add_spec)),255);
					image.set(j+Ax,y0+i, truecolor);
				}
			}
		}
	}
	if (height2 != 0){
		for (int i=0; i<height2; i++){
			float Ax = x0 + (i+height1)*(x2-x0)/height;
			float Az = z0 + (i+height1)*(z2-z0)/height;
			Vec2f At = t0 + (t2-t0)*((i+height1)/(float)height);
			Vec2f Bt = t1 + (t2-t1)*(i/(float)height2);
			float Bx = x1 + i*(x2-x1)/height2;
			float Bz = z1 + i*(z2-z1)/height2;
			if (Ax > Bx){
				std::swap(Ax,Bx);
				std::swap(Az,Bz);
				std::swap(At,Bt);
			}
			for (int j=0; j<=Bx-Ax; j++){
			    Vec2f Ct = At + (Bt-At)*(j/(float)(Bx-Ax));
				float z = Az + j*(Bz-Az)/(Bx-Ax);
				if (zbuffer[j+(int)Ax+(y0+i+height1)*1000] < z){
					zbuffer[j+(int)Ax+(y0+i+height1)*1000] = z;
					TGAColor color = texture.get(Ct.x,Ct.y);
					TGAColor color_nm = nm.get(Ct.x,Ct.y);
					TGAColor color_spec = spec.get(Ct.x,Ct.y);
					Vec3f vecolor_nm(color_nm.r/255.f*2.f-1.f,color_nm.g/255.f*2.f-1.f,color_nm.b/255.f*2.f-1.f);
					Vec3f n = vecolor_nm.normalize();
					Vec3f l = light.normalize();
					Vec3f r = ((n*2.*(n*l))-l).normalize() ;
					float s = std::max(0.f,r*camera.normalize()) ;
					float spe = std::sqrt(color_spec.r*color_spec.r+color_spec.g*color_spec.g+color_spec.b*color_spec.b) ;
					float diff = std::max(0.f,vecolor_nm*light);
					TGAColor truecolor(color.r*(coeff_diff*diff+coeff_spec*std::pow(s,spe+add_spec)),color.g*(coeff_diff*diff+coeff_spec*std::pow(s,spe+add_spec)),color.b*(coeff_diff*diff+coeff_spec*std::pow(s,spe+add_spec)),255);
					image.set(j+Ax,y0+i+height1, truecolor);
				}
			}
		}
	}
}


int main(int argc, char** argv) {

	//Initilization of images and model
	TGAImage image (width,height,3);
	Model *model = new Model("./obj/diablo.obj");
	TGAImage texture;
	texture.read_tga_file("./obj/diablo3_pose_diffuse.tga");
	texture.flip_vertically();
	TGAImage nm;
	nm.read_tga_file("./obj/diablo3_pose_nm.tga");
	nm.flip_vertically();
	TGAImage spec;
	nm.read_tga_file("./obj/diablo3_pose_spec.tga");
	nm.flip_vertically();

	//Initialisation of zbuffer
	int *zbuffer = new int [width*height];
	for (int i=0; i<width*height; i++){
		zbuffer[i] = std::numeric_limits<int>::min();
	}

	//Initialization of matrix
	vp = viewport(width, height, depth);
	projec = projection(1.f/(camera-origine).norm());
	modelview = lookat(origine,camera,u);
	light = proj<3>(projec*modelview*embed<4>(light,0.f)).normalize();

	//Let's go
	for (int i=0;i<model->nfaces();i++){
		std::vector<Vec3i> face = model->face(i);
		
		//Get Coords
		Vec3f v0 = proj<3>(vp*projec*modelview*(embed<4>(model->vert(face[0][0]))));
		Vec3f v1 = proj<3>(vp*projec*modelview*(embed<4>(model->vert(face[1][0]))));
		Vec3f v2 = proj<3>(vp*projec*modelview*(embed<4>(model->vert(face[2][0]))));

		Vec3f normal = cross((v2-v0),(v1-v0));
		normal.normalize();
		//Juste the good faces
		if (normal*light > 0){

			Vec2f t0 = model->texture(face[0][1])*1024;
			Vec2f t1 = model->texture(face[1][1])*1024;
			Vec2f t2 = model->texture(face[2][1])*1024;
			triangle_t(image,v0[0],v0[1],v0[2],v1[0],v1[1],v1[2],v2[0],v2[1],v2[2],t0,t1,t2,zbuffer,texture,nm,spec);
		}
	}
	image.flip_vertically();
	image.write_tga_file("image.tga");
	return 0;
}

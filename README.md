# infographie

Thx to Master Sokolov for his infography class.
Thx to Mickael Charles for his help with nm and spec coefficient.


Command :
	- make for compile
	- make clean for clean
	- make doc for create doc

Afer compile :
	- ./main and ./main fil : draw wire-frame model
	- ./main z : draw model with z-buffer
	- ./main gouraud : draw model with gouraud shading (not work very well)
	- ./main film : draw wire-frame model with matrix
	- ./main zm : draw model with z-buffer with matrix
	- ./main gouraudm : draw model with gouraud shading with matrix (not work very well)
	- ./main text : draw model with diffuse,normal maps and specular (some problems)

For draw model with just diffuse, change :
	- line 466 : "image.set(j+Ax,y0+i, truecolor);" by "image.set(j+Ax,y0+i, color);"
	- line 500 : "image.set(j+Ax,y0+i+height1, truecolor);" by "image.set(j+Ax,y0+i+height1, color);"

Frow draw model without specular, change :
	- line 465 : "TGAColor truecolor(color.r*(0.5f*diff+1.f*spe),color.g*(0.5f*diff+1.f*spe),color.b*(0.5f*diff+1.f*spe),255);" by "TGAColor truecolor(color.r*(1.f*diff+0.f*spe),color.g*(1.f*diff+0.f*spe),color.b*(1.f*diff+0.f*spe),255);"
	- line 500 : "TGAColor truecolor(color.r*(0.5f*diff+1.f*spe),color.g*(0.5f*diff+1.f*spe),color.b*(0.5f*diff+1.f*spe),255);" by "TGAColor truecolor(color.r*(1.f*diff+0.f*spe),color.g*(1.f*diff+0.f*spe),color.b*(1.f*diff+0.f*spe),255);"

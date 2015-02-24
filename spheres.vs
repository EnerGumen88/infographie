#version 120
uniform vec4 ogf_uniform_0; // viewport
attribute float attr;
varying float pointSize;
varying vec3 position;




void main() {
	gl_FrontColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_PointSize = 700*attr;
	pointSize = gl_PointSize/2;
	vec3 ndc = (gl_Position.xyz/gl_Position.w);
	
	
	position.x = ogf_uniform_0.z/2*ndc.x+(ogf_uniform_0.x+ogf_uniform_0.z/2);
	position.y = ogf_uniform_0.w/2*ndc.y+(ogf_uniform_0.y+ogf_uniform_0.w/2);
	position.z = (gl_DepthRange.far-gl_DepthRange.near)/2*ndc.z+((gl_DepthRange.far+gl_DepthRange.near)/2);
}


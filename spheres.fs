#version 120

uniform vec4 ogf_uniform_0;
varying float pointSize;
varying vec3 position;




void main(void) {
	gl_FragColor = gl_Color;
	
	
	float distance = distance(position,gl_FragCoord.xyz);
	float z = sqrt(pow(pointSize,2)-pow(distance,2));
	
	float far = gl_DepthRange.far;
	float near = gl_DepthRange.near;

	//Don't know what is the good gl_FragDepth
	//gl_FragDepth = 1/z + position.z;
	//gl_FragDepth = (1/z-near) / (far-near) + position.z;
	if (position.z < 0)
		gl_FragDepth = (far-near) / (z-near) - position.z;
	else
		 gl_FragDepth = (far-near) / (z-near) + position.z;
	//gl_FragDepth = (far-near) / (z-near) + position.z;
	//gl_FragDepth = (1/z+position.z) / (far-near);


	if (distance > pointSize) discard;
	
	
	vec3 point = gl_FragCoord.xyz;
	point.z = z;

	vec3 normal = point - position;

	vec3 n = normalize(normal);
	vec3 l = normalize(gl_LightSource[0].position.xyz);

	float diffuse  = .2+clamp(dot(l,n), 0, 1);
	vec3 r = normalize(2*n*dot(l,n)-l);
	float spec = pow(max(0,r.z),5);
	gl_FragColor = gl_Color*diffuse+spec;
}


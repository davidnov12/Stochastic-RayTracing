#version 440


#extension GL_ARB_bindless_texture : enable

struct Material{
  //sampler2D diffuseTex;
  //sampler2D normalMap;
  vec3 diffuseCol;
  float refIndex;
};

struct Triangle{
	vec3 pos_a;
	vec3 pos_b;
	vec3 pos_c;
	vec3 nor_a;
	vec3 nor_b;
	vec3 nor_c;
	vec2 uv_a;
	vec2 uv_b;
	vec2 uv_c;
	int mat_id;
};

struct Ray{
	vec3 origin;
	vec3 direction;
	float energy;
};

struct CollisionPoint{
	float dist;
	vec3 position;
	vec3 color;
	vec3 normal;

};

layout(local_size_x = 1, local_size_y = 1) in;

// Materials buffer
//layout (std430, binding = 0) buffer mat_buffer {
	Material materials[];
//} material_buffer;


// Renderbuffer
layout(rgba32f, binding = 0) uniform image2D img_output;
/*
layout (std430, binding = 1) buffer out_frame {
	float frame[];
};*/

uniform vec3 screen_plane[3];
uniform vec3 view_pos;

// Funkce pro ziskani barycentrickych souradnic
void barycentricInterpolation(Triangle tr, vec3 point, out vec3 bar_coords){
	vec3 v0 = tr.pos_b - tr.pos_a;
	vec3 v1 = tr.pos_c - tr.pos_a;
	vec3 v2 = point - tr.pos_a;

	float d00 = dot(v0, v0);
	float d01 = dot(v0, v1);
	float d11 = dot(v1, v1);
	float d20 = dot(v2, v0);
	float d21 = dot(v2, v1);

	float den = d00 * d11 - d11 * d01;
	bar_coords.y = (d11 * d20 - d01 * d21) / den;
	bar_coords.z = (d00 * d21 - d01 * d20) / den;
	bar_coords.x = 1.0 - bar_coords.y - bar_coords.z;
}

// Prusecik paprsku s trojuhelnikem
bool rayTriangleIntersection(Ray ray, Triangle tr, out CollisionPoint inter){

	vec3 e1 = tr.pos_b - tr.pos_a;
	vec3 e2 = tr.pos_c - tr.pos_a;

	vec3 s1 = cross(ray.direction, e2);

	float div = dot(s1, e1);
	if(div == 0) return false;

	float invdiv = 1 / div;
	vec3 dis = ray.origin - tr.pos_a;
	vec3 s2 = cross(dis, e1);

	float t = dot(e2, s2) * invdiv;

	if(t < 0.009) return false;

	float b1 = dot(dis, s1) * invdiv;
	if (b1 < 0.0 || b1 > 1.0) return false;

	float b2 = dot(ray.direction, s2) * invdiv;
	if (b2 < 0.0 || (b1 + b2) > 1.0) return false;

	inter.dist = t;
	inter.position = ray.origin + t * ray.direction;

	vec3 bar_coords;
	barycentricInterpolation(tr, inter.position, bar_coords);
	inter.normal = normalize(tr.nor_a * bar_coords.x + tr.nor_b * bar_coords.y + tr.nor_c * bar_coords.z);

	//inter.color = materials[tr.mat_id].diffuseCol;
	inter.color = vec3(0.8, 0.1, 0.3);

	return true;
}

void main(){

	vec2 uv = vec2(float(gl_GlobalInvocationID.x) / gl_NumWorkGroups.x * gl_WorkGroupSize.x, float(gl_GlobalInvocationID.y) / gl_NumWorkGroups.y * gl_WorkGroupSize.y);
	vec3 dir = screen_plane[2] + (uv.x * screen_plane[0]) + (uv.y * screen_plane[1]);

	Ray r;
	r.origin = view_pos;
	r.direction = normalize(dir - view_pos);
	r.energy = 1.0;


	Triangle t;
	t.pos_a = vec3(1,0,1); t.pos_b = vec3(0,1,1); t.pos_c = vec3(1,1,0);
	CollisionPoint cp;

	vec4 pixel = vec4(0.8, 0.1, 0.3, 1.0);

	if(rayTriangleIntersection(r, t, cp))
		pixel = vec4(cp.color, 1);
	else
		pixel = vec4(1);


	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);

	imageStore(img_output, pixel_coords, pixel);

}

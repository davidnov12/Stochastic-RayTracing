#version 440

#define MAX_DEPTH 1
#define EPS 0.0001
#define PI 3.14159

struct Material{
  vec3 diffuseCol;
  float refIndex;
  vec3 gap;
  float metalness;
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
  float refractionIndex;
  float metalness;
};

struct Node{
  vec4 _min;
  vec4 _max;
  int first;
  int last;
  int left;
  int right;
  int parent;
  int gapa, gapb, gapc;
};

layout(local_size_x = 8, local_size_y = 8) in;

// Renderbuffer
layout(rgba32f, binding = 0) uniform image2D img_output;

// Triangles
layout (std430, binding = 1) buffer in_data {
	Triangle datas[];
};

// Materials
layout (std430, binding = 2) buffer in_mats {
	Material materials[];
};
/*
layout (std430, binding = 3) buffer in_nodes {
	Node bvh[];
};

layout (std430, binding = 4) buffer in_index {
	int indices[];
};*/

Triangle data;

uniform vec3 screen_plane[3];
uniform vec3 view_pos;
//uniform vec3 light_pos = vec3(4, 7, 1);
uniform int width;
uniform int height;

bool findCollision(Ray r, out CollisionPoint c);

float randm(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec3 computeLighting(CollisionPoint col, float fresnell){

  return col.color;

}

bool rayTriangleIntersection(Ray ray, Triangle tr, out CollisionPoint inter){

	vec3 e1 = tr.pos_b - tr.pos_a;
	vec3 e2 = tr.pos_c - tr.pos_a;

	vec3 s1 = cross(ray.direction, e2);

	float div = dot(s1, e1);
	if(div < 1e-7) return false;

	float invdiv = 1.0 / div;
	vec3 dis = ray.origin - tr.pos_a;

	float u = dot(dis, s1) * invdiv;
  if (u < 0.0 || u > 1.0) return false;

  vec3 qv = cross(dis, e1);
	float v = dot(ray.direction, qv) * invdiv;
	if (v < 0.0 || u + v > 1.0) return false;


  float t = dot(e2, qv) * invdiv;
  if(t < 0.0000001) return false;

	inter.dist = t;
	inter.position = ray.origin + t * ray.direction;
	//inter.color = vec3(0,0,1);
	//inter.normal = tr.nor_a;
	inter.color = materials[tr.mat_id].diffuseCol;
	//inter.refractionIndex = materials[tr.mat_id].refIndex;
	//inter.metalness = materials[tr.mat_id].metalness;

	return true;
}

bool findCollision(Ray r, out CollisionPoint c){

  float closest = 10000.0f; CollisionPoint hitPoint; bool res = false;

  for(int i = 0; i < 222; i++){
    if(rayTriangleIntersection(r, datas[i], hitPoint)){
      if(hitPoint.dist < closest){
        c = hitPoint;
        closest = hitPoint.dist;
        res = true;
      }
    }
  }

  return res;

}


vec4 rayTrace(Ray r){

  vec4 pixel = vec4(1);
  uint depth = 0;
  float refIndex = 1.0;
  CollisionPoint cp;
  vec3 fres;

  while(depth < MAX_DEPTH){

    if(findCollision(r, cp)){
      pixel *= vec4(r.energy * computeLighting(cp, fres.x), 1.0);
    }

    else
      return r.energy * vec4(0.95);

    depth += 1;
  }

  return pixel;
}

void main(){

	vec2 uv = vec2(float(gl_GlobalInvocationID.x) / width /*gl_NumWorkGroups.x * gl_WorkGroupSize.x*/,
                 float(gl_GlobalInvocationID.y) / height /*gl_NumWorkGroups.y * gl_WorkGroupSize.y*/);
	vec3 dir = screen_plane[2] + (uv.x * normalize(screen_plane[0])) + (uv.y * normalize(screen_plane[1]));

	Ray r;
	r.origin = view_pos;
	r.direction = normalize(dir - view_pos);
	r.energy = 1.0;

	
	
	data.pos_a = vec3(2, 1, 0);
	data.pos_b = vec3(4, 1, 0);
	data.pos_c = vec3(1, 2, 0);
	
	vec4 pixel = vec4(0.0f);
 
    pixel += rayTrace(r);
   
	//if(datas[0].pos_a.x == 0.344800) pixel = vec4(0,1,1,1);
   
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);

	imageStore(img_output, pixel_coords, pixel);

}

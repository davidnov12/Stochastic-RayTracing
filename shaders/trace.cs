#version 440

/*
#extension GL_ARB_bindless_texture : enable

struct material{
  sampler2D diffuseTex;
  sampler2D normalMap;
  vec3 diffuseCol;
  float refIndex;
};*/

struct triangle{
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


layout(local_size_x = 1, local_size_y = 1) in;

// Materials buffer
//layout (std430, binding = 0) buffer mat_buffer {
//	material materials[];
//} material_buffer;


// Renderbuffer
layout(rgba32f, binding = 0) uniform image2D img_output;
/*
layout (std430, binding = 1) buffer out_frame {
	float frame[];
};*/


void main(){
	
	vec4 pixel = vec4(0.8, 0.1, 0.3, 1.0);
  
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
  
	imageStore(img_output, pixel_coords, pixel);
	barrier();
	
}

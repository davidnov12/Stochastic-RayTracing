#version 440

out vec2 uv;

void main(){
	if(gl_VertexID == 0){ gl_Position = vec4(-1,-1,0,1); uv = vec2(0, 0);}
	if(gl_VertexID == 1){ gl_Position = vec4(-1,1,0,1); uv = vec2(0, 1);}
	if(gl_VertexID == 2){ gl_Position = vec4(1,-1,0,1); uv = vec2(1, 0);}
	if(gl_VertexID == 3){ gl_Position = vec4(1,1,0,1); uv = vec2(1, 1);}
}
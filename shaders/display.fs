#version 440


//layout(binding=0)uniform sampler2D screen;
uniform sampler2D screen;

in vec2 uv;

layout (std430, binding = 4) buffer out_frame {
	float frame[];
};

out vec4 color;

void main(){
	
	//if(frame[int(gl_FragCoord.y * 1000 + gl_FragCoord.x)] == 0)
	//	color = vec4(1);
	//else
	//	color = vec4(frame[4 * int(gl_FragCoord.y * 1000 + gl_FragCoord.x)], frame[int(4 * gl_FragCoord.y * 1000 + gl_FragCoord.x) + 1], frame[int(4 * gl_FragCoord.y * 1000 + gl_FragCoord.x) + 2], frame[int(4 * gl_FragCoord.y * 1000 + gl_FragCoord.x) + 3] + 3);
	
	color = texture(screen, uv);
		
}
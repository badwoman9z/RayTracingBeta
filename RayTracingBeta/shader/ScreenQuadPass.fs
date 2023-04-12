#version 430 core

in  vec2 v2f_TexCoords;
out vec4 color;

layout(binding = 6,rgba32f) uniform image2D lastFrame;
uniform sampler2D filteredFrame;
float Luminance(vec3 color)
{
    return 0.25 * color.r + 0.5 * color.g + 0.25 * color.b;
}

vec3 toneMapping(in vec3 c, float exposure) {
    vec3 mapped = vec3(1.0) - exp(-c * exposure);
	return mapped;
}
vec3 toneMapping2(in vec3 c, float limit) {
    float luminance = 0.3*c.x + 0.6*c.y + 0.1*c.z;
    return c * 1.0 / (1.0 + luminance / limit);
}


void main()		  
{	
	const float gamma = 2.2;
	vec4 Color_ = vec4(texture(filteredFrame,v2f_TexCoords).rgb,1.0);

	color = Color_;
	color.rgb = color.rgb / (color.rgb + vec3(1.0));
	color.rgb = pow(color.rgb, vec3(1.0 / 2.2));

	//color = Color_;
	//color.rgb = Color_.rgb;
	imageStore(lastFrame,ivec2(v2f_TexCoords.xy*512.0),Color_);
	
}
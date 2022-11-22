
#version 430
in vec2 tc;      // 输入插值过的材质坐标
in vec4 varyingColor;
uniform mat4 mv_matrix; 
uniform mat4 proj_matrix;
layout (binding=0) uniform sampler2D samp;
out vec4 color;

void main(void)  
{ 
    // color = texture(samp, tc);
    color = texture(samp, tc);
    // color = varyingColor;
}
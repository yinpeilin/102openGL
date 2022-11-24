#version 430 
// uniform float offset;
layout (location=0) in vec3 position;
// layout (location=1) in vec2 texCoord;
// out vec2 tc;        // 纹理坐标输出到光栅着色器用于插值
// layout (binding=0) uniform sampler2D samp;
// uniform mat4 mv_matrix;
uniform mat4 m_matrix;            // 这些是分开的模型和视图矩阵
uniform mat4 v_matrix;
uniform mat4 proj_matrix;
// uniform float tf;                 // 用于动画和放置立方体的时间因子
// uniform mat4 proj_matrix;

out vec4 varyingColor;

mat4 buildRotateX(float rad);     // 矩阵变换工具函数的声明
mat4 buildRotateY(float rad);     // GLSL 要求函数先声明后调用
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x, float y, float z);

void main(void)
{ 
    // 构建模型矩阵，然后是模型-视图矩阵
    mat4 mv_matrix = v_matrix * m_matrix;
    gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
    varyingColor =  vec4(position.y/10,position.y/10,position.y/10,1.0);

    
}
mat4 buildTranslate(float x, float y, float z)
{
    mat4 trans = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0, 
        0.0, 0.0, 1.0, 0.0,
        x,   y,   z, 1.0 
    );

    return trans;
}

mat4 buildRotateX(float rad)
{
    mat4 xrot = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, cos(rad), -sin(rad), 0.0, 
        0.0, sin(rad), cos(rad), 0.0,
        0.0,   0.0,   0.0, 1.0 
    );

    return xrot;
}

mat4 buildRotateY(float rad)
{
    mat4 yrot = mat4(
        cos(rad), 0.0, sin(rad), 0.0,
        0.0, 1.0, 0.0, 0.0, 
        -sin(rad), 0.0, cos(rad), 0.0,
        0.0,   0.0,   0.0, 1.0 
    );

    return yrot;
}

mat4 buildRotateZ(float rad)
{
    mat4 zrot = mat4(
        cos(rad), -sin(rad), 0.0, 0.0,
        sin(rad), cos(rad), 0.0, 0.0, 
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0 
    );


    return zrot;
}

mat4 buildScale(float x, float y, float z)
{
    mat4 scale = mat4(
        x, 0.0, 0.0, 0.0,
        0.0, y, 0.0, 0.0, 
        0.0, 0.0, z, 0.0,
        0.0,   0.0,   0.0, 1.0 
    );

    return scale;
}

#shader vertex
#version 330 core
        
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texID;

out vec2 v_texCoord;
out float v_texID;

uniform  mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * position;
    v_texCoord = texCoord;
    v_texID = texID;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 f_color;

in vec2 v_texCoord;
in float v_texID;

uniform sampler2D u_Textures[2];

void main()
{
    vec4 texColor = texture(u_Textures[int(v_texID)], v_texCoord);
    f_color = texColor;
};
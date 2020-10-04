#type vertex
#version 430 core
layout(location = 0) in vec2 in_Position;
layout(location = 1) in vec2 in_TexCoord;
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
out vec2 v_TexCoord;

void main() {
    v_TexCoord =  in_TexCoord;
    gl_Position = u_ViewProjection * u_Transform * vec4(in_Position, 0, 1);
}

#type fragment
#version 430 core
in vec2 v_TexCoord;
uniform sampler2D u_Texture;
out vec4 FragColor;

void main() {
    FragColor = texture(u_Texture, v_TexCoord.st);
}

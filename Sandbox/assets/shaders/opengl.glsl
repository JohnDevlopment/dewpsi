#shader vertex

#version 430 core
layout(location = 0) in vec2 In_Position;
layout(location = 1) in vec2 In_UV;
uniform mat4 U_ViewProjection;
out vec2 V_UV;

void main() {
    V_UV = In_UV;
    gl_Position = U_ViewProjection * vec4(In_Position, 0, 1);
}

#shader fragment

#version 430 core
in vec2 V_UV;
uniform vec4 U_Color;
uniform sampler2D U_Texture;
out vec4 FragColor;

void main() {
    FragColor = texture2D(U_Texture, V_UV.st);
}

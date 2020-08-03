#shader vertex

#version 430 core

layout(location = 0) in vec3 In_position;

void main() {
    gl_Position = vec4(In_position, 0.9);
}

#shader fragment

#version 430 core

layout(location = 0) out vec4 FragColor;

void main() {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}

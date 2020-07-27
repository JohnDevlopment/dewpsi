#shader vertex
#version 430
//layout(location = 0) in vec4 In_position;
layout(location = 0) in vec2 In_position;
void main() {
    gl_Position.xy = In_position;
    gl_Position.z = 1.0;
    gl_Position.w = 1.0;
}

#shader fragment
#version 430
out vec4 Out_color;
void main() {
    Out_color = vec4(0.5, 0.5, 0.5, 0.5);
}

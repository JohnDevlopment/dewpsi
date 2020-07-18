#shader vertex
#version 430
layout(location = 0) in vec4 In_position;
void main() {
    gl_Position = In_position;
}

#shader fragment
#version 430
out vec4 Out_color;
void main() {
    Out_color = vec4(1.0, 1.0, 1.0, 1.0);
}

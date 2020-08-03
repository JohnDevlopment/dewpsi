#shader vertex

#version 120

uniform mat4 ProjMtx;

attribute vec2 Position;
attribute vec2 UV;
attribute vec4 Color;

varying vec2 Frag_UV;
varying vec4 Frag_Color;

void main()
{
    Frag_UV = UV;
    Frag_Color = Color;
    gl_Position = ProjMtx * vec4(Position.xy, 0.0f, 1.0f);
};

#shader fragment

uniform sampler2D Texture;

varying vec2 Frag_UV;
varying vec4 Frag_Color;

void main()
{
    gl_FragColor = Frag_Color * texture2D(Texture, Frag_UV.st);
};

// Attaching a layer
typedef std::string PDstring;
PDstring vertexSource = ...;
PDstring fragmentSource = ...;
Dewpsi::Renderer::SetAPI(...);
Dewpsi::Shader* program = Dewpsi::Shader::Create(vertexSource, fragmentSource);

// Detaching the layer
delete program;

// Vertex array
Dewpsi::VertexArray* vertexArray = Dewpsi::VertexArray::Create();
vertexArray->Bind();

// Creating vertex buffer
using Dewpsi::DefineColor;
Dewpsi::FColor colors[] = {
    DefineColor(127, 0, 127),
    DefineColor(0, 127, 127),
    DefineColor(127, 127, 0),
};
const float vertices[] = {
   -0.5f, -0.5f,   PD_COLOR_UNPACK3(colors[0]),
    0.5f, -0.5f,   PD_COLOR_UNPACK3(colors[1]),
    0.0f,  0.5f,   PD_COLOR_UNPACK3(colors[1])
};
Dewpsi::VertexBuffer* vertexBuffer = Dewpsi::VertexBuffer::Create(sizeof(vertices), vertices);

// Vertex buffer layout
Dewpsi::BufferLayout layout = {
    { ShaderDataType::Float2, "In_Position" },
    { ShaderDataType::Float3, "In_Color", true }
};
vertexBuffer->SetLayout(layout);

// Index buffer
const PDuint indices[] = {0, 1, 2};
indexBuffer = Dewpsi::IndexBuffer::Create(sizeof(indcies), indices);

//! [Add buffers to array]
vertexArray->AddVertexBuffer(m_VertexBuffer);
vertexArray->SetIndexBuffer(m_IndexBuffer);
//! [Add buffers to array]

//! [Layer update]

//! [Layer update]

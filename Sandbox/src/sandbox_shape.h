#ifndef SANDBOX_SHAPE_H
#define SANDBOX_SHAPE_H

#include <Dewpsi_OrthoCamera.h>
#include <Dewpsi_Buffer.h>
#include <Dewpsi_VertexArray.h>
#include <Dewpsi_Texture.h>
#include <Dewpsi_Array.h>

template<PDsizei Nv, PDsizei Ni>
struct SandboxShape {
    glm::vec3                         position;
    float                             rotation;
    Dewpsi::Ref<Dewpsi::VertexArray>  vao;
    Dewpsi::Ref<Dewpsi::VertexBuffer> vbo;
    Dewpsi::Ref<Dewpsi::IndexBuffer>  ibo;
    Dewpsi::Ref<Dewpsi::Texture>      texture;
    Dewpsi::Array<float, Nv>          vertices;
    Dewpsi::Array<PDuint, Ni>         indices;

    SandboxShape() : position(0.0f), rotation(0.0f), vao(), vbo(), ibo() {}

    // initialize data
    void Init(const Dewpsi::BufferLayout& layout)
    {
        // vertex array
        vao = Dewpsi::VertexArray::Create();
        vao->Bind();
        // vertex buffer
        vbo = Dewpsi::VertexBuffer::Create(vertices.Size(), vertices.Data());
        vbo->SetLayout(layout);
        // index buffer
        ibo = Dewpsi::IndexBuffer::Create(
            indices.Length(), indices.Data()
        );
        vao->AddVertexBuffer(vbo);
        vao->SetIndexBuffer(ibo);
        vao->UnBind();
    }

    // return the transform
    glm::mat4 Transform() const
    {
        return glm::translate(glm::mat4(1.0f), position);
                //* glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    }
};

#endif /* SANDBOX_SHAPE_H */

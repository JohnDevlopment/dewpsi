#ifndef SANDBOX_INTERNAL_H
#define SANDBOX_INTERNAL_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Dewpsi_Renderer.h>
#include <Dewpsi_Buffer.h>
#include <Dewpsi_VertexArray.h>
#include <Dewpsi_Texture.h>
#include <Dewpsi_Camera.h>
#include <Dewpsi_Memory.h>
#include <Dewpsi_Pair.h>

struct SandboxInternalModel {
    glm::mat4 transform;
    Dewpsi::Ref<Dewpsi::VertexArray>  vao;
    Dewpsi::Ref<Dewpsi::VertexBuffer> vbo;
    Dewpsi::Ref<Dewpsi::IndexBuffer>  ibo;
    Dewpsi::Ref<Dewpsi::Texture>      tex;
};

#endif /* SANDBOX_INTERNAL_H */

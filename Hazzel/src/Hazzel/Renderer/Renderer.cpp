#include "hzpch.h"
#include "Renderer.h"

#include "Platform/OpenGl/OpenGLShader.h"

namespace Hazzel {

	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;


	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ModelMatrix", transform);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
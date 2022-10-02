#include <Hazzel.h>

#include "imgui/imgui.h"

class ExampleLayer : public Hazzel::Layer 
{
public:
	ExampleLayer()
		:Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		m_VertexArray.reset(Hazzel::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.1f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.8f, 0.1f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.2f, 0.1f, 0.8f, 1.0f,
		};

		std::shared_ptr<Hazzel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazzel::VertexBuffer::Create(vertices, sizeof(vertices)));
		Hazzel::BufferLayout layout = {
			{Hazzel::ShaderDataType::Float3, "a_Position"},
			{Hazzel::ShaderDataType::Float4, "a_Color"},
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Hazzel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazzel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			#version 330 core

			uniform mat4 u_ViewProjection;

			layout(location = 0) in  vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0f);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";
		m_Shader.reset(Hazzel::Shader::Create(vertexSrc, fragmentSrc));
	}

	void OnUpdate() override
	{

		if(Hazzel::Input::IsKeyPressed(HZ_KEY_LEFT))
			m_CameraPosition.x -= c_CameraSpeed;
		else if (Hazzel::Input::IsKeyPressed(HZ_KEY_RIGHT))
			m_CameraPosition.x += c_CameraSpeed;

		if (Hazzel::Input::IsKeyPressed(HZ_KEY_UP))
			m_CameraPosition.y += c_CameraSpeed;
		else if (Hazzel::Input::IsKeyPressed(HZ_KEY_DOWN))
			m_CameraPosition.y -= c_CameraSpeed;

		if (Hazzel::Input::IsKeyPressed(HZ_KEY_A))
			m_CameraRotation -= c_CameraRotationSpeed;
		else if (Hazzel::Input::IsKeyPressed(HZ_KEY_D))
			m_CameraRotation += c_CameraRotationSpeed;

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Hazzel::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		Hazzel::RenderCommand::Clear();

		{
			Hazzel::Renderer::BeginScene(m_Camera);

			Hazzel::Renderer::Submit(m_Shader, m_VertexArray);

			Hazzel::Renderer::EndScene();
		}
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Test!");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(Hazzel::Event& event) override
	{
		Hazzel::EventDispatcher dispatcher(event);
		//dispatcher.Dispatch <Hazzel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::OnKeyPressed)); 
	}

	bool OnKeyPressed(Hazzel::KeyPressedEvent& event)
	{
		switch (event.GetKeyCode())
		{
		case HZ_KEY_LEFT:
			m_CameraPosition.x -= c_CameraSpeed;
			break;
		case HZ_KEY_RIGHT:
			m_CameraPosition.x += c_CameraSpeed;
			break;
		case HZ_KEY_UP:
			m_CameraPosition.y += c_CameraSpeed;
			break;
		case HZ_KEY_DOWN:
			m_CameraPosition.y -= c_CameraSpeed;
			break;
		case HZ_KEY_A:
			m_CameraRotation -= c_CameraRotationSpeed;
			break;
		case HZ_KEY_D:
			m_CameraRotation += c_CameraRotationSpeed;
			break;
		default:
			break;
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
		return false;
	}

private:
	std::shared_ptr<Hazzel::Shader> m_Shader;
	std::shared_ptr<Hazzel::VertexArray> m_VertexArray;

	Hazzel::OrthographicCamera m_Camera;
	
	glm::vec3 m_CameraPosition = { 0.f, 0.f, 0.f };
	const float c_CameraSpeed = .025f;

	float m_CameraRotation = 0.f;
	const float c_CameraRotationSpeed = 2.0f;

};

class Sandbox : public Hazzel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer()); 			// TESTING: layers/layerstack and HZ keycodes
	}

	~Sandbox()
	{

	}
};

Hazzel::Application* Hazzel::CreateApplication()
{
	return new Sandbox();
}
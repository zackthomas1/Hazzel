#include "hzpch.h"
#include "VertexArray.h"

#include "Renderer.h" 
#include "Platform/OpenGl/OpenGLVertexArray.h"

namespace Hazzel
{
    Ref<VertexArray> VertexArray::Create()
    {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RenderAPI::None currently not supported.");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		default:
			HZ_CORE_ASSERT(false, "Unknown Renderer API");
			return nullptr;
		}
    }
}


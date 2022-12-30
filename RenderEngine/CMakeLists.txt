set(RenderEngine_SRC 
	"Private/VKImpl/RenderEngine.cpp" 
	"Private/VKImpl/Renderer.cpp")

set(RenderEngine_VULKAN_RHI
	"Private/VKImpl/VulkanRHI.cpp"
	"Private/VKImpl/OpaquePass.cpp"
	"Private/VKImpl/RenderPass.cpp"
 )

add_library(RenderEngine SHARED ${RenderEngine_SRC} ${RenderEngine_VULKAN_RHI} )
target_precompile_headers(RenderEngine PUBLIC "pch.hpp")
target_include_directories(RenderEngine 
	PUBLIC 
		"Public" 
	PRIVATE 
		"Public/RenderEngine" 
		"Private"
		"Private/VKImpl"
)
target_compile_definitions(RenderEngine PRIVATE RENDERENGINE_IMPL)
target_link_libraries(RenderEngine PUBLIC DirectXMath ImGui Platform)

if(WIN32)
	target_compile_definitions(RenderEngine PUBLIC WIN32_LEAN_AND_MEAN)
	target_compile_definitions(RenderEngine PRIVATE PLATFORM_WIN32)
endif()

find_package(Vulkan REQUIRED)
if(Vulkan_FOUND)
	target_include_directories(RenderEngine PUBLIC ${Vulkan_INCLUDE_DIRS})
	target_link_libraries(RenderEngine PUBLIC ${Vulkan_LIBRARIES})

	if(WIN32)
		target_compile_definitions(RenderEngine PUBLIC VK_USE_PLATFORM_WIN32_KHR)
	endif()
endif()

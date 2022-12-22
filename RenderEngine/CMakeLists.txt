set(RenderEngine_SRC 
	"Private/RenderEngine.cpp" 
	"Private/VKRHI/Renderer.cpp")

set(RenderEngine_VULKAN_RHI
	"Private/VKRHI/VulkanRHI.cpp"
	"Private/VKRHI/OpaquePass.cpp"
	"Private/VKRHI/RenderPass.cpp"
 "Private/VKRHI/RenderContext.cpp")

add_library(RenderEngine SHARED ${RenderEngine_SRC} ${RenderEngine_VULKAN_RHI} )
target_precompile_headers(RenderEngine PUBLIC "pch.hpp")
target_include_directories(RenderEngine 
	PUBLIC 
		"Public" 
	PRIVATE 
		"Public/RenderEngine" 
		"Private"
		"Private/VKRHI"
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

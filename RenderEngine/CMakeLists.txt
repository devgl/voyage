set(RenderEngine_SRC 
	"Private/VulkanRHI.cpp"
	"Private/RenderEngine.cpp")

add_library(RenderEngine SHARED ${RenderEngine_SRC})
target_precompile_headers(RenderEngine PUBLIC "pch.hpp")
target_include_directories(RenderEngine PUBLIC "Public" PRIVATE "Public/RenderEngine")
target_compile_definitions(RenderEngine PRIVATE RENDERENGINE_IMPL)

if(WIN32)
	target_compile_definitions(RenderEngine PUBLIC WIN32_LEAN_AND_MEAN)
endif()

find_package(Vulkan REQUIRED)
if(Vulkan_FOUND)
	target_include_directories(RenderEngine PUBLIC ${Vulkan_INCLUDE_DIRS})
	target_link_libraries(RenderEngine PUBLIC ${Vulkan_LIBRARIES})

	if(WIN32)
		target_compile_definitions(RenderEngine PUBLIC VK_USE_PLATFORM_WIN32_KHR)
	endif()
endif()

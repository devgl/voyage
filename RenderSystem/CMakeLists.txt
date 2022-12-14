set(RenderSystem_SRC 
	"Private/VulkanRHI.cpp"
	 )

add_library(RenderSystem SHARED ${RenderSystem_SRC})
target_precompile_headers(RenderSystem PUBLIC "pch.h")
target_include_directories(RenderSystem PUBLIC "Public" PRIVATE "Public/RenderSystem")
target_compile_definitions(RenderSystem PRIVATE RENDERSYSTEM_IMPL)

if(WIN32)
	target_compile_definitions(RenderSystem PUBLIC WIN32_LEAN_AND_MEAN)
endif()

find_package(Vulkan REQUIRED)
if(Vulkan_FOUND)
	target_include_directories(RenderSystem PUBLIC ${Vulkan_INCLUDE_DIRS})
	target_link_libraries(RenderSystem PUBLIC ${Vulkan_LIBRARIES})

	if(WIN32)
		target_compile_definitions(RenderSystem PUBLIC VK_USE_PLATFORM_WIN32_KHR)
	endif()
endif()

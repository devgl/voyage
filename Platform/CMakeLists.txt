set(Platform_SRC 
	"Private/Window.cpp"
	"Private/MSWindow.cpp"
)

add_library(Platform SHARED ${Platform_SRC})
target_precompile_headers(Platform PUBLIC "pch.hpp")
target_link_libraries(Platform PUBLIC DirectXMath)
target_compile_definitions(Platform PRIVATE PLATFORM_IMPL)
target_include_directories(Platform 
	PUBLIC 
		"Public" 
	PRIVATE
		"Private"
		"Public/Platform"
)
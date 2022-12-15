set(Framework_SRC 
	"Private/Components/CameraComponent.cpp"
 "Private/Framework.cpp" "Private/Application.cpp" "Private/Components/StaticMeshComponent.cpp" "Private/GameObject.cpp")

add_library(Framework SHARED ${Framework_SRC})
target_link_libraries(Framework PUBLIC RenderEngine)
target_include_directories(Framework 
	PUBLIC 
		"Public" 
	PRIVATE 
		"Public/Framework" 
		"Public/Framework/Components"
		"Private/"
)
target_compile_definitions(Framework PRIVATE FRAMEWORK_IMPL)
target_precompile_headers(Framework PUBLIC "pch.hpp")
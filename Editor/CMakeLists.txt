set(Editor_SRC "Private/main.cpp")

add_executable(Editor ${Editor_SRC})
target_link_libraries(Editor PRIVATE RenderSystem)
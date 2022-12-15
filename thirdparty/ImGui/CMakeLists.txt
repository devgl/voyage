add_library(ImGui STATIC)

target_sources(ImGui PRIVATE
    "imgui.cpp"
    "imgui_demo.cpp"
    "imgui_draw.cpp"
    "imgui_tables.cpp"
    "imgui_widgets.cpp"
)

if(WIN32)
target_sources(ImGui PRIVATE
    "backends/imgui_impl_win32.cpp"
    "backends/imgui_impl_dx12.cpp"
)
endif()

target_include_directories(ImGui PUBLIC ".")

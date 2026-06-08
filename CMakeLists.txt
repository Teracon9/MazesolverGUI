cmake_minimum_required(VERSION 3.16)

project(proj)
set_source_files_properties(main.c PROPERTIES LANGUAGE CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
add_executable(proj main.c)


target_include_directories(proj PRIVATE C:/programming/libraries/CSFML-v1/install-release/include)
#target_include_directories(proj PRIVATE C:/programming/libraries/SFML-v1/install-debug/include)
target_link_directories(proj PRIVATE C:/programming/libraries/CSFML-v1/install-release/lib)
target_link_directories(proj PRIVATE C:/programming/libraries/SFML-v1/install-release/lib)

target_compile_definitions(proj PRIVATE CSFML_STATIC)
target_compile_definitions(proj PRIVATE SFML_STATIC)

target_link_libraries(proj
    csfml-graphics-s
    csfml-window-s
    csfml-system-s
    sfml-graphics-s
    sfml-window-s
    sfml-system-s
    opengl32
    winmm
    gdi32
)


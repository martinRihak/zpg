cmake_minimum_required(VERSION 3.10)
project(ZPG)

set(OpenGL_GL_PREFERENCE GLVND)

if(POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)  

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLM REQUIRED glm)

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(SHADER_DIR "${CMAKE_SOURCE_DIR}/shaders")

file(GLOB_RECURSE SOURCE_FILES
    "${SOURCE_DIR}/*.cpp"
    "${SOURCE_DIR}/*.hpp"
)
file(GLOB SHADER_FILES "${SHADER_DIR}/*.frag" "${SHADER_DIR}/*.vert" "${SHADER_DIR}/*.glsl")
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/shaders)
foreach(SHADER ${SHADER_FILES})
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    configure_file(${SHADER} ${CMAKE_BINARY_DIR}/shaders/${SHADER_NAME} COPYONLY)
endforeach()

# Přidání spustitelného souboru (předpokládám main.cpp v src/)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SOURCE_DIR}
    ${GLM_INCLUDE_DIRS} 
    ${OPENGL_INCLUDE_DIR} 
    ${GLEW_INCLUDE_DIRS} # Pokud GLM není systémové, přidejte explicitní cestu
)
# Linkování knihoven
target_link_libraries(${PROJECT_NAME} PRIVATE glfw GLEW::GLEW OpenGL::GL)
cmake_minimum_required(VERSION 3.10)
project(ZPG)

# Nastav preferenci OpenGL na GLVND (moderní, pro core profile)
set(OpenGL_GL_PREFERENCE GLVND)

# Nastav politiku pro potlačení warningu (NEW preferuje GLVND)
if(POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)  # Bez uvozovek!

# Pro GLM použij pkg-config (header-only, bez linking)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLM REQUIRED glm)

add_executable(main main.cpp Model.cpp App.cpp Scene.cpp DrawableObject.cpp shaderProgram/Shader.cpp shaderProgram/ShaderProgram.cpp )

target_include_directories(main PRIVATE ${GLM_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})
target_link_libraries(main PRIVATE glfw GLEW::GLEW OpenGL::GL)
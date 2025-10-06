cmake_minimum_required(VERSION 3.10)
project(ZPG)

set(OpenGL_GL_PREFERENCE GLVND)

if(POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)  # Bez uvozovek!

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLM REQUIRED glm)

add_executable(main main.cpp Model.cpp App.cpp Scene.cpp DrawableObject.cpp Transformation.cpp Transformation/ITransformation.cpp shaderProgram/Shader.cpp shaderProgram/ShaderProgram.cpp Controller.cpp )

target_include_directories(main PRIVATE ${GLM_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})
target_link_libraries(main PRIVATE glfw GLEW::GLEW OpenGL::GL)
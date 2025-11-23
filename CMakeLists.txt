cmake_minimum_required(VERSION 3.10)
project(ZPG)

set(OpenGL_GL_PREFERENCE GLVND)

if(POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED) 

# Find SOIL
find_path(SOIL_INCLUDE_DIR SOIL.h PATH_SUFFIXES SOIL)
find_library(SOIL_LIBRARY SOIL)

if(NOT SOIL_INCLUDE_DIR OR NOT SOIL_LIBRARY)
    message(FATAL_ERROR "SOIL not found. Please install libsoil-dev")
endif()
# Přidej cestu k include složce stb-master přímo (relativně k CMakeLists.txt)


# Pokud chceš použít find_path pro větší flexibilitu (např. pro budoucí změny)
find_path(STB_INCLUDE_DIR stb_image.h
    PATHS ${CMAKE_SOURCE_DIR}/stb-master  # Hledej v této složce
    NO_DEFAULT_PATH  # Ignoruj systémové cesty, abys to udržel lokální
)

if(NOT STB_INCLUDE_DIR)
    message(FATAL_ERROR "Nepodařilo se najít stb_image.h! Zkontroluj složku stb-master.")
endif()

# Přidání podpory pro tiny_obj_loader (header-only, hledá se v lokální složce)
find_path(TINYOBJLOADER_INCLUDE_DIR tiny_obj_loader.h
    PATHS ${CMAKE_SOURCE_DIR}/tinyobjloader  # Hledej v této složce (uprav, pokud je jinde)
    NO_DEFAULT_PATH  # Ignoruj systémové cesty
)

if(NOT TINYOBJLOADER_INCLUDE_DIR)
    message(FATAL_ERROR "Nepodařilo se najít tiny_obj_loader.h! Zkontroluj složku tinyobjloader.")
endif()


find_package(PkgConfig REQUIRED)
pkg_check_modules(GLM REQUIRED glm)
pkg_check_modules(assimp REQUIRED assimp)
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
    ${OPENGL_INCLUDE_DIR} 
    ${GLEW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${SOIL_INCLUDE_DIR}
    ${STB_INCLUDE_DIR}
    ${TINYOBJLOADER_INCLUDE_DIR}
)

# Linkování knihoven
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    GLEW::GLEW
    OpenGL::GL
    ${assimp_LIBRARIES}
    ${SOIL_LIBRARY}
)
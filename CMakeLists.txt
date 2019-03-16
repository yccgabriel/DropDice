cmake_minimum_required(VERSION 3.10)
project(DropDice)

# "Pure" C++17 and no extensions.
set(CMAKE_CXX_STANDARD 11)      # TODO: upgrade to 11 later
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default debug and optimize flags.
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

if(NOT EMSCRIPTEN)
    # GLFW
    # handle opengl dependencies
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/)
    find_package(OpenGL REQUIRED)
    find_package(OpenAL REQUIRED)
endif()

# Dependency libraries
# these libraries have their own CMakeLists.txt
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/qu3e)     # todo specify qu3e build location. right now it's ${source}/lib/

# build qu3e

if(EMSCRIPTEN)

    # TODO: 

else()
    file(GLOB SOURCES_GLFW_APP  ${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/camera.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/Instance.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/RayTracer.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/RectangularPrism.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/RubiksCore.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/Scene.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/ShaderManager.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/SpawnMachine.cpp)
    add_executable( glfw_app
                    ${SOURCES_GLFW_APP}
                    libs/glad/src/glad.c
                    libs/lodepng/lodepng.cpp)
    target_include_directories( glfw_app PUBLIC
                                ${OPENGL_INCLUDE_DIR}
                                ${CMAKE_CURRENT_SOURCE_DIR}/include
                                ${CMAKE_CURRENT_SOURCE_DIR}/libs
                                ${CMAKE_CURRENT_SOURCE_DIR}/libs/qu3e/src # bad pattern but it is qu3e's fault
                                ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include
                                ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/include
                                ${CMAKE_CURRENT_SOURCE_DIR}/libs/glm/
                                ${CMAKE_CURRENT_SOURCE_DIR}/libs/lodepng
                                ${CMAKE_CURRENT_SOURCE_DIR}/
                                ${OPENAL_INCLUDE_DIR})
    target_link_libraries(  glfw_app
                            glfw
                            qu3e
                            ${OPENGL_LIBRARIES}
                            ${OPENGL_gl_LIBRARY}
                            ${OPENAL_LIBRARY})

    add_custom_command(TARGET glfw_app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
        COMMENT "Copying files from src-dir/assets/ to build-dir/assets")

    add_custom_command(TARGET glfw_app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/shaders ${CMAKE_BINARY_DIR}/shaders
        COMMENT "Copying files from src-dir/src/shaders/ to build-dir/shaders")
endif()
cmake_minimum_required(VERSION 3.4.1)
project(app)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_C_COMPILER "/Users/janei/Desktop/OpenGLLearn/HelloWorld/Dependency/mpich/bin/mpicc")
set (CMAKE_CXX_COMPILER "/Users/janei/Desktop/OpenGLLearn/HelloWorld/Dependency/mpich/bin/mpic++")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependency/
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependency/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependency/assimp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependency/glfw/lib-macos
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependency/assimp/lib
)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/Dependency/glfw/lib-macos/libglfw3.a)
link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/Dependency/assimp/lib/libassimp.5.dylib)

aux_source_directory(. SRC_LIST) # 搜索当前目录下的所有.cpp文件
aux_source_directory(./Dependency/glad/src GLAD_SRC_LIST) # 搜索当前目录下的所有.cpp文件
aux_source_directory(./Dependency/imgui IMGUI_SRC_LIST) # 搜索当前目录下的所有.cpp文件
add_executable(${PROJECT_NAME} ${SRC_LIST} ${GLAD_SRC_LIST} ${IMGUI_SRC_LIST})
MESSAGE(${SRC_LIST})
target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
target_link_libraries(${PROJECT_NAME} "-framework IOKit")

add_custom_target(run make && mpiexec -n 4 ./${PROJECT_NAME})

set (source "${CMAKE_SOURCE_DIR}/Resources")
set (destination "${CMAKE_CURRENT_BINARY_DIR}/Resources")
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination} 
    DEPENDS ${destination}
    COMMENT "symbolic link resources folder from ${source} => ${destination}"
)

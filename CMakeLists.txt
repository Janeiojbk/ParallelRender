cmake_minimum_required(VERSION 3.4.1)
project(app)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependency/
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependency/MPI/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependency/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependency/assimp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

find_package(OpenGL REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Dependency/glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Dependency/assimp)

aux_source_directory(. SRC_LIST) # 搜索当前目录下的所有.cpp文件
aux_source_directory(./Dependency/glad/src GLAD_SRC_LIST) # 搜索当前目录下的所有.cpp文件
aux_source_directory(./Dependency/imgui IMGUI_SRC_LIST) # 搜索当前目录下的所有.cpp文件
add_executable(${PROJECT_NAME} ${SRC_LIST} ${GLAD_SRC_LIST} ${IMGUI_SRC_LIST})
target_link_libraries(${PROJECT_NAME} glfw assimp ${CMAKE_CURRENT_SOURCE_DIR}/Dependency/MPI/Lib/x64/msmpi.lib)
MESSAGE(${SRC_LIST})

add_custom_target(run make && mpiexec -n 4 ./${PROJECT_NAME})

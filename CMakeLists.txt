cmake_minimum_required(VERSION 3.5.2)
set(PROJECT_NAME "meshtool")
set(CMAKE_CXX_STANDARD 14)

project(${PROJECT_NAME})

find_package(Eigen3 REQUIRED)
find_package(OpenMP)

if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_subdirectory(lib)

set(SOURCES
    src/vertex.cpp      src/vertex.h
    src/face.cpp        src/face.h
    src/colmap.cpp      src/colmap.h
    src/meshtool.cpp    src/meshtool.h
)

include_directories(
    src
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${EIGEN3_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${LIBS})

add_subdirectory(bin2ascii)

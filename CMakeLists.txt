cmake_minimum_required(VERSION 3.16)
project(problem1 CXX)
set(CXX_STANDARD_REQUIRED 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXTRA 
    "cpp/src/graph.cpp"
)

include_directories(
    "cpp/include"
)

add_executable(graph_generator "cpp/src/graph_generator.cpp" ${EXTRA})
add_executable(vertex "cpp/src/vertex.cpp" ${EXTRA})
add_executable(vertex_approx "cpp/src/vertex_approx.cpp" ${EXTRA})


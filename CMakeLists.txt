cmake_minimum_required(VERSION 3.13.4)
project(cost_model)
set(CMAKE_CXX_STANDARD 23)

set(COST_MODEL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories(${COST_MODEL_INCLUDE_DIR})

add_subdirectory(src)


add_executable(${PROJECT_NAME} cost_model.cpp)
target_link_libraries(${PROJECT_NAME} route_lib layer_lib chip_lib network_lib)


include(CTest)
add_subdirectory(test)



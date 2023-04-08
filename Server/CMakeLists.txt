# Set the name of the project
set(target_name Server)
project(${target_name} C CXX)


# Use C20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_compile_definitions(_WIN32_WINNT=0x0601)


file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp*
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

file(GLOB_RECURSE HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/src/*.hpp
)


add_executable(${target_name} ${SOURCES} ${HEADERS})

source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES
    ${SOURCES}
    ${HEADERS}
)

get_filename_component(PARENT_DIR ../ ABSOLUTE)
target_include_directories(${target_name} PRIVATE ${PARENT_DIR}/boost_1_81_0)
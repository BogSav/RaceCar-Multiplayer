# Set the name of the project
set(target_name Server)
project(${target_name} C CXX)

# Use C20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif()

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

set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost 1.82.0 REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(${target_name} ${Boost_LIBRARIES})
  target_link_libraries(${target_name} PRIVATE ${Boost_LIBRARY_DIRS}/*.lib)
  message(STATUS "Libraria BOOST incarcata cu succes in " ${target_name})
endif()
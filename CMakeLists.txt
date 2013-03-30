cmake_minimum_required (VERSION 2.4)
project (two_joins)
set(CMAKE_BUILD_TYPE Release)
add_subdirectory(src)

add_executable (leapfrog leapfrog.cpp)
target_link_libraries (leapfrog src)
target_link_libraries(leapfrog -lrt)

add_executable (sortmerge sortmerge.cpp)
target_link_libraries (sortmerge src)
target_link_libraries(sortmerge -lrt)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "-static")

project(fileManager LANGUAGES CXX)
add_executable (fileManager main_fm.cpp filemanager.h filemanager.cpp)
target_link_libraries(fileManager pthread)

project(fileManagerClient LANGUAGES CXX)
add_executable (fileManagerClient main_fm.cpp filemanager_stub.h filemanager_stub.cpp utils.h utils.cpp serialize.h serialize.cpp)
target_link_libraries(fileManagerClient pthread)

project(fileManagerServer LANGUAGES CXX)
add_executable (fileManagerServer main_fmServer.cpp filemanager_imp.h filemanager_imp.cpp utils.h utils.cpp serialize.h serialize.cpp filemanager.h filemanager.cpp)
target_link_libraries(fileManagerServer pthread)
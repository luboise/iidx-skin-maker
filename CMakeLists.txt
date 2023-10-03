cmake_minimum_required(VERSION 3.5)

project(iidx-skin-maker VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

find_package(wxWidgets REQUIRED COMPONENTS net core base)
if(wxWidgets_USE_FILE) # not defined in CONFIG mode
    include(${wxWidgets_USE_FILE})
endif()

file(GLOB FILESYSTEM_SOURCES "files/*.cpp" "files/*.h")
add_executable(${CMAKE_PROJECT_NAME} main.cpp ${FILESYSTEM_SOURCES})


target_link_libraries(${CMAKE_PROJECT_NAME} ${wxWidgets_LIBRARIES})




set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

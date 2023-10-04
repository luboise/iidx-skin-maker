cmake_minimum_required(VERSION 3.5)

project(iidx-skin-maker VERSION 0.1.0 LANGUAGES C CXX)

# set(CMAKE_BUILD_TYPE RelWithDebInfo)


include(CTest)
enable_testing()

find_package(wxWidgets REQUIRED COMPONENTS net core base)
if(wxWidgets_USE_FILE) # not defined in CONFIG mode
    include(${wxWidgets_USE_FILE})
endif()

# Extra Folders
file(GLOB FILESYSTEM_SOURCES "files/*.cpp" "files/*.h")
file(GLOB GUI_SOURCES "gui/*.cpp" "gui/*.h")


add_executable(${CMAKE_PROJECT_NAME} main.cpp ${FILESYSTEM_SOURCES} ${GUI_SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE gui file)
target_link_libraries(${CMAKE_PROJECT_NAME} ${wxWidgets_LIBRARIES})




set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

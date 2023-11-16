cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project(iidx-skin-maker VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

if (WIN32)
    set(wxWidgets_ROOT_DIR $ENV{WXWIN})
    set(wxWidgets_LIB_DIR "C:/wx/3.2.4/lib/gcc1030TDM_x64_dll")
endif()

find_package(wxWidgets REQUIRED COMPONENTS net core base)
if(wxWidgets_USE_FILE) # not defined in CONFIG mode
    include(${wxWidgets_USE_FILE})
endif()

find_package(portaudio REQUIRED)

# Extra Folders
file(GLOB FILESYSTEM_SOURCES "files/*.cpp" "files/*.h")
file(GLOB GUI_SOURCES "gui/*.cpp" "gui/*.h")
file(GLOB AUDIO_SOURCES "audio/*.cpp" "audio/*.h")

add_executable(${PROJECT_NAME} main.cpp ${FILESYSTEM_SOURCES} ${GUI_SOURCES} ${AUDIO_SOURCES})

if (WIN32)
    target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE gui file "C:/Program Files (x86)/portaudio/include/")
    # Link wxWidgets
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES} "C:/Program Files (x86)/portaudio/bin/libportaudio.dll")
else()
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES} -lportaudio)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

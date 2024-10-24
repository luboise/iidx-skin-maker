cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
  iidx-skin-maker
  VERSION 0.1.0
  LANGUAGES C CXX)

include(CTest)
enable_testing()

if(WIN32)
  set(wxWidgets_ROOT_DIR $ENV{WXWIN})
  set(wxWidgets_LIB_DIR "C:/wx/3.2.4/lib/gcc1030TDM_x64_dll")
endif()

find_package(wxWidgets REQUIRED COMPONENTS net core base)
if(wxWidgets_USE_FILE) # not defined in CONFIG mode
  include(${wxWidgets_USE_FILE})
endif()

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
  # Update submodules as needed
  option(GIT_SUBMODULE "Check submodules during build" ON)
  if(GIT_SUBMODULE)
    message(STATUS "Submodule update")
    execute_process(
      COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
      message(
        FATAL_ERROR
          "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules"
      )
    endif()
  endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/portaudio/CMakeLists.txt")
  message(
    FATAL_ERROR
      "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again."
  )
endif()

add_subdirectory(extern/portaudio)

find_package(SndFile REQUIRED)

# Extra Folders
add_executable(iidx-skin-maker main.cpp utils.cpp)
target_precompile_headers(iidx-skin-maker PRIVATE pch.h)

add_subdirectory(gui)
add_subdirectory(files)
add_subdirectory(audio)
add_subdirectory(mod_manager)

# target_sources(iidx-skin-maker <INTERFACE|PUBLIC|PRI...> [items1...]
# [<INTERFACE|PUBLIC|PRI...> [items2...] ...])

# find_package(portaudio REQUIRED)
if(WIN32)
  target_include_directories(
    ${CMAKE_PROJECT_NAME} PRIVATE "C:/Program Files (x86)/portaudio/include/")
  target_link_libraries(${CMAKE_PROJECT_NAME}
                        "C:/Program Files (x86)/portaudio/bin/libportaudio.dll")
endif()

target_include_directories(iidx-skin-maker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
  ${CMAKE_PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES} SndFile::sndfile
                                -lportaudio)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME}) set(CPACK_PROJECT_VERSION
# ${PROJECT_VERSION}) include(CPack)

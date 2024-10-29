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

set(BASE_OUTPUT_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE})

# Static libs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BASE_OUTPUT_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BASE_OUTPUT_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_OUTPUT_DIR})

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

add_subdirectory(extern/doctest)

if(WIN32)
  set(CMAKE_MSVC_RUNTIME_LIBRARY, MultiThreaded)

  set(wxBUILD_SHARED OFF)
  add_subdirectory(extern/wxwidgets)

  add_subdirectory(extern/portaudio)
  include_directories(extern/portaudio/include)

  set(BUILD_PROGRAMS OFF)
  set(BUILD_EXAMPLES OFF)
  set(BUILD_TESTING OFF)
  add_subdirectory(extern/libsndfile)
else()

  find_package(wxWidgets REQUIRED COMPONENTS net core base)
  if(wxWidgets_USE_FILE)
    include(${wxWidgets_USE_FILE})
  endif()

  find_package(SndFile REQUIRED)
  # find_package(portaudio_static REQUIRED)
endif()

# set(wxWidgets_LIBRARIES)

if(WIN32)
  add_executable(iidx-skin-maker WIN32 src/main.cpp)

  set(WX_IMPORTS wx::net wx::core wx::base)
  set(PORTAUDIO_IMPORTS PortAudio)
else()
  add_executable(iidx-skin-maker src/main.cpp)

  set(WX_IMPORTS ${wxWidgets_LIBRARIES})
  set(PORTAUDIO_IMPORTS -lportaudio)
endif()

target_link_libraries(
  iidx-skin-maker PRIVATE ${WX_IMPORTS} ${PORTAUDIO_IMPORTS} SndFile::sndfile)

add_subdirectory(src)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)

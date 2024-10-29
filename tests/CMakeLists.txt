enable_testing()

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src/)

add_executable(
  Tests
  test_main.cpp
  test_mod.cpp
  ${SRC_DIR}/../mod_manager/Mod.cpp
  ${SRC_DIR}/../mod_manager/Overrides/SD9Override.cpp
  ${SRC_DIR}/../files/FileHandler.cpp
  ${SRC_DIR}/../audio/SD9File.cpp
  ${SRC_DIR}/../audio/SoundFile.cpp
  ${SRC_DIR}/../utils.cpp)

target_include_directories(Tests PRIVATE $SRC_DIR)
target_link_libraries(Tests PRIVATE ${WXWIDGETS_MODULES} SndFile::sndfile
                                    doctest PortAudio)

target_precompile_headers(Tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../pch.h)

add_test(NAME Tests COMMAND Tests)

add_dependencies(iidx-skin-maker Tests)
# add_dependencies(${CMAKE_PROJECT_NAME} Tests)

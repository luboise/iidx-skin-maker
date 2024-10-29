enable_testing()

add_executable(
  Tests
  test_main.cpp
  test_mod.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../mod_manager/Mod.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../mod_manager/Overrides/SD9Override.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../files/FileHandler.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../audio/SD9File.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../audio/SoundFile.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../utils.cpp)

target_include_directories(Tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_link_libraries(Tests PRIVATE ${WXWIDGETS_MODULES} SndFile::sndfile
                                    doctest PortAudio)

target_precompile_headers(Tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../pch.h)

add_test(NAME Tests COMMAND Tests)

add_dependencies(iidx-skin-maker Tests)
# add_dependencies(${CMAKE_PROJECT_NAME} Tests)

enable_testing()

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src/)

add_executable(Tests main.cpp)

# Compile all source files needed
file(GLOB_RECURSE SkinMakerSources "${SRC_DIR}/*.cpp")
list(FILTER SkinMakerSources EXCLUDE REGEX ".*main.cpp")

file(GLOB TestSources ${CMAKE_CURRENT_SOURCE_DIR}/test*.cpp)

# test_mod.cpp ${SRC_DIR}/../mod_manager/Mod.cpp
# ${SRC_DIR}/../mod_manager/Overrides/SD9Override.cpp
# ${SRC_DIR}/../files/FileHandler.cpp ${SRC_DIR}/../audio/SD9File.cpp
# ${SRC_DIR}/../audio/SoundFile.cpp ${SRC_DIR}/../utils.cpp)

target_sources(Tests PRIVATE ${SkinMakerSources} ${TestSources})

target_include_directories(Tests PRIVATE ${SRC_DIR})
target_link_libraries(Tests PRIVATE ${WX_IMPORTS} ${PORTAUDIO_IMPORTS}
                                    SndFile::sndfile doctest)

target_precompile_headers(Tests PRIVATE ${SRC_DIR}/pch.h)

add_test(NAME Tests COMMAND Tests)

add_dependencies(iidx-skin-maker Tests)
# add_dependencies(${CMAKE_PROJECT_NAME} Tests)

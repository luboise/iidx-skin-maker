# Extra Folders
target_precompile_headers(iidx-skin-maker PRIVATE pch.h)

target_include_directories(iidx-skin-maker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(iidx-skin-maker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
                                       ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp)

add_subdirectory(gui)
add_subdirectory(files)
add_subdirectory(audio)
add_subdirectory(mod_manager)

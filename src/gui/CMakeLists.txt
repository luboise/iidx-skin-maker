target_sources(
  iidx-skin-maker
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/MainFrame.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/MainMenuBar.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/Panels/FilePanel.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/Panels/OverrideEditor.cpp
          # ${CMAKE_CURRENT_SOURCE_DIR}/Panels/OverrideEditor.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/Popups/ModSettingsPopup.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/Components/VolumeControl.cpp)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Forms)

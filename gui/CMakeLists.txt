target_sources(
  iidx-skin-maker
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/MainFrame.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/MainMenuBar.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/Panels/FilePanel.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/Form/TextCallbackBox.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/Form/ModLoopChanger.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/Panels/EditPanel.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/Popups/ModSettingsPopup.cpp)

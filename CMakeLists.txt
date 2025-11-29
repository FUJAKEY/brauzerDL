cmake_minimum_required(VERSION 3.16)

project(SweetBrowser VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets WebEngineCore WebEngineWidgets Network)

add_executable(SweetBrowser
    resources/resources.qrc
    src/main.cpp
    src/BrowserWindow.cpp
    src/BrowserWindow.h
    src/WebView.cpp
    src/WebView.h
    src/HistoryManager.cpp
    src/HistoryManager.h
    src/DownloadManager.cpp
    src/DownloadManager.h
    src/SettingsManager.cpp
    src/SettingsManager.h
    src/SettingsDialog.cpp
    src/SettingsDialog.h
    src/UiAnimator.h
)

target_link_libraries(SweetBrowser PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::WebEngineCore
    Qt6::WebEngineWidgets
    Qt6::Network
)

# Handle Windows specific settings for deployment
if(WIN32)
    set_property(TARGET SweetBrowser PROPERTY WIN32_EXECUTABLE ON)
endif()

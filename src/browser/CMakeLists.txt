
add_executable(CandyBrowser
    main.cpp
    mainwindow.cpp
    mainwindow.h
    tabwidget.cpp
    tabwidget.h
    webpage.cpp
    webpage.h
    downloadmanager.cpp
    downloadmanager.h
    historymanager.cpp
    historymanager.h
    resources.qrc
)

target_link_libraries(CandyBrowser PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::WebEngineCore
    Qt6::WebEngineWidgets
    Qt6::Network
    Qt6::Sql
)

# Copy styles to build directory for testing
configure_file(${CMAKE_SOURCE_DIR}/src/resources/styles/style.qss style.qss COPYONLY)

if(WIN32)
    set_target_properties(CandyBrowser PROPERTIES WIN32_EXECUTABLE ON)
endif()

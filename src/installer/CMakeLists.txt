
add_executable(CandyInstaller
    main.cpp
    installwizard.cpp
    installwizard.h
)

target_link_libraries(CandyInstaller PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

if(WIN32)
    set_target_properties(CandyInstaller PROPERTIES WIN32_EXECUTABLE ON)
    target_link_options(CandyInstaller PRIVATE /MANIFEST:NO) # Sometimes needed for custom installers
endif()

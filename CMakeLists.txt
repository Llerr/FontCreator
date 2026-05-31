cmake_minimum_required(VERSION 3.10)
project(AdafruitFontCreator VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#find_package(Qt5 REQUIRED COMPONENTS Gui )
find_package(Qt5 REQUIRED COMPONENTS Widgets Core)

# Указываем где искать .ui файлы для AUTOUIC
set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

# Добавляем пути к заголовочным файлам (include директории)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(AdafruitFontCreator WIN32 MACOSX_BUNDLE
    include/drawCharactersWidget.h
    include/drawEditWidget.h
    include/drawGlyphsWidget.h
    include/editWidget.h
    include/fontWidget.h
    include/glyph.h
    include/glyphsWidget.h
    include/highlighter.h
    include/ioFontCode.h
    include/mainwindow.h
    include/settings.h
    include/unicoderanges.h
    src/drawCharactersWidget.cpp
    src/drawEditWidget.cpp
    src/drawGlyphsWidget.cpp
    src/editWidget.cpp
    src/fontWidget.cpp
    src/glyph.cpp
    src/glyphsWidget.cpp
    src/highlighter.cpp
    src/ioFontCode.cpp
    src/main.cpp
    src/mainwindow.cpp
    src/settings.cpp
    src/unicoderanges.cpp
    ui/editWidget.ui
    ui/fontWidget.ui
    ui/glyphsWidget.ui
    ui/mainwindow.ui
    ui/settings.ui
)

target_include_directories(AdafruitFontCreator PRIVATE
    include
)

target_compile_definitions(AdafruitFontCreator PRIVATE
    QT_DEPRECATED_WARNINGS
)

target_link_libraries(AdafruitFontCreator PRIVATE
    Qt5::Core
    Qt5::Gui
)

target_link_libraries(AdafruitFontCreator PRIVATE
    Qt5::Widgets
)

install(TARGETS AdafruitFontCreator
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Consider using qt_generate_deploy_app_script() for app deployment if
# the project can use Qt 6.3. In that case rerun qmake2cmake with
# --min-qt-version=6.3.

cmake_minimum_required(VERSION 3.16)
project(LabProject VERSION 1.0 LANGUAGES CXX)

# Find Qt packages 
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

# Set C++ standard 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable automatic moc, uic, and rcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Source files 
set(APP_SOURCES
    adminwindow.cpp
    classes.cpp
    globals.cpp
    main.cpp
    loginwindow.cpp
    managerwindow.cpp
    staffwindow.cpp
)

# Header files 
set(APP_HEADERS
    adminwindow.h
    classes.h
    globals.h
    loginwindow.h
    managerwindow.h
    staffwindow.h
)

# UI forms 
set(APP_FORMS
    adminwindow.ui
    loginwindow.ui
    managerwindow.ui
    staffwindow.ui
)

# Create executable
add_executable(LabProject
    ${APP_SOURCES}
    ${APP_HEADERS}
    ${APP_FORMS}
)

# Link Qt libraries
target_link_libraries(LabProject PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)

# Optional: Disable deprecated APIs (uncomment if needed)
##target_compile_definitions(LabProject PRIVATE QT_DISABLE_DEPRECATED_BEFORE=0x060000)

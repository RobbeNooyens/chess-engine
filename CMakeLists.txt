cmake_minimum_required(VERSION 3.6)
project(schaken)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

#=====================================================
# Windows
#=====================================================
set(Qt5Core_DIR "C:\\Qt64\\5.15.2\\mingw81_64\\lib\\cmake\\Qt5Core")
set(Qt5Widgets_DIR "C:\\Qt64\\5.15.2\\mingw81_64\\lib\\cmake\\Qt5Widgets")
set(Qt5Gui_DIR "C:\\Qt64\\5.15.2\\mingw81_64\\lib\\cmake\\Qt5Gui")

find_package( Qt5Core REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Gui REQUIRED )


set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp mainwindow.cpp mainwindow.h chessboard.cpp chessboard.h game.cpp game.h schaakStuk.cpp schaakStuk.h resources.h exceptions/KingNotFoundException.h config.h)
add_executable(schaken ${SOURCE_FILES})

qt5_use_modules(schaken Core Widgets Gui)

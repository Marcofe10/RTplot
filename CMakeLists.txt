project(plotxyfltk)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
find_package(FLTK REQUIRED)

# include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})
include_directories(${FLTK_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})


add_executable(demoxyfltk main.cpp plotxyfltk.cpp)

#Share and Static Library
add_library(plotxyfltk SHARED plotxyfltk.cpp)
add_library(plotxyfltks STATIC plotxyfltk.cpp)

target_link_libraries(demoxyfltk ${FLTK_LIBRARIES})


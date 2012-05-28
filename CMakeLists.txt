project(plotxyfltk)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
find_package(FLTK REQUIRED)

# include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})
include_directories(${FLTK_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(demoxyfltk demomain.cpp plotxyfltk.cpp)

#Share and Static Library
# add_library(plotxyfltk SHARED plotxyfltk.cpp)
# add_library(plotxyfltks STATIC plotxyfltk.cpp)

target_link_libraries(demoxyfltk ${FLTK_LIBRARIES})


SET(HDRS plotxyfltk.h )
SET(SRC plotxyfltk.cpp )
SET(CMAKEPLOTXYFLTK FindPlotXYFLTK.cmake)

SET (LIBRARYNAME plotxyfltk)
SET (LIB_MAJOR 0)
SET (LIB_MINOR 0)
SET (LIB_RELEASE 1)

add_library (${LIBRARYNAME} SHARED ${HDRS} ${SRC})
SET (_soversion ${LIB_MAJOR}.${LIB_MINOR}.${LIB_RELEASE})
set_target_properties(${LIBRARYNAME} PROPERTIES SOVERSION ${LIB_MAJOR} VERSION ${_soversion})
target_link_libraries(${LIBRARYNAME})
SET (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
# INSTALL(FILES ${HDRS} DESTINATION /usr/local/include/ns/mindset)
#INSTALL(TARGETS ${LIBRARYNAME} LIBRARY DESTINATION /usr/local/lib/ns/mindset)

INSTALL(FILES ${CMAKEPLOTXYFLTK} DESTINATION /usr/share/cmake-2.8/Modules)
INSTALL(FILES ${HDRS} DESTINATION include)
INSTALL(TARGETS ${LIBRARYNAME} LIBRARY DESTINATION lib PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )

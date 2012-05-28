# - Try to find the Plotxyfltk includes and library
# which defines
#
# PlotXYFLTK_FOUND - system has Plotxyfltk
# PlotXYFLTK_INCLUDE_DIR - where to find plotxyfltk.h
# PlotXYFLTK_LIBRARIES - the libraries to link against to use Plotxyfltk
# PlotXYFLTK_LIBRARY - where to find the Plotxyfltk library (not for general use)

# copyright (c) 2012 Marco Ferrara marco.ferrara10@gmail.com
#
# Redistribution and use is allowed according to the terms of the BSD license.

IF(NOT FLTK_FOUND)
    INCLUDE(FindFLTK)
ENDIF(NOT FLTK_FOUND)

SET(PlotXYFLTK_FOUND "NO")

IF(FLTK_FOUND)
    FIND_PATH(PlotXYFLTK_INCLUDE_DIR plotxyfltk.h
    /usr/local/plotxyfltk/include
    /usr/local/include
    /usr/include/plotxyfltk
    /usr/include
    )

    SET(PlotXYFLTK_NAMES ${PlotXYFLTK_NAMES} plotxyfltk libplotxyfltk)
    FIND_LIBRARY(PlotXYFLTK_LIBRARY
        NAMES ${PlotXYFLTK_NAMES}
        PATHS /usr/local/lib/plotxyfltk /usr/local/lib /usr/lib)

    IF (PlotXYFLTK_LIBRARY)
        SET(PlotXYFLTK_LIBRARIES ${PlotXYFLTK_LIBRARY})
        SET(PlotXYFLTK_FOUND "YES")

#         IF (CYGWIN)
#             IF(BUILD_SHARED_LIBS)
#             # No need to define PlotXYFLTK_USE_DLL here, because it's default for Cygwin.
#             ELSE(BUILD_SHARED_LIBS)
#             SET (PlotXYFLTK_DEFINITIONS -DPlotXYFLTK_STATIC)
#             ENDIF(BUILD_SHARED_LIBS)
#         ENDIF (CYGWIN)

    ENDIF (PlotXYFLTK_LIBRARY)
ENDIF(FLTK_FOUND)

IF (PlotXYFLTK_FOUND)
  IF (NOT PlotXYFLTK_FIND_QUIETLY)
    MESSAGE(STATUS "Found Plotxyfltk: ${PlotXYFLTK_LIBRARY}")
  ENDIF (NOT PlotXYFLTK_FIND_QUIETLY)
ELSE (PlotXYFLTK_FOUND)
  IF (PlotXYFLTK_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find Plotxyfltk library")
  ENDIF (PlotXYFLTK_FIND_REQUIRED)
ENDIF (PlotXYFLTK_FOUND)

MARK_AS_ADVANCED(PlotXYFLTK_INCLUDE_DIR PlotXYFLTK_LIBRARY)
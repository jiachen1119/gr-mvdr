find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_MVDR gnuradio-mvdr)

FIND_PATH(
    GR_MVDR_INCLUDE_DIRS
    NAMES gnuradio/mvdr/api.h
    HINTS $ENV{MVDR_DIR}/include
        ${PC_MVDR_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_MVDR_LIBRARIES
    NAMES gnuradio-mvdr
    HINTS $ENV{MVDR_DIR}/lib
        ${PC_MVDR_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-mvdrTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_MVDR DEFAULT_MSG GR_MVDR_LIBRARIES GR_MVDR_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_MVDR_LIBRARIES GR_MVDR_INCLUDE_DIRS)

find_package(Armadillo REQUIRED)

set(MATLAB_DIR /home/jiachen/MATLAB)
add_library(matlab_lib matlab.cpp)
install(TARGETS matlab_lib DESTINATION include/gnuradio/mvdr EXPORT MATLAB_LIB)
target_link_libraries(matlab_lib
        PUBLIC
        ${ARMADILLO_LIBRARIES}
        "${MATLAB_DIR}/bin/glnxa64/libeng.so"
        "${MATLAB_DIR}/bin/glnxa64/libmx.so"
        "${MATLAB_DIR}/bin/glnxa64/libmat.so"
        )
target_include_directories(matlab_lib
        PUBLIC
        ${ARMADILLO_INCLUDE_DIRS}
        "${MATLAB_DIR}/extern/include"
        )
target_link_directories(matlab_lib
        PUBLIC
        "${MATLAB_DIR}/bin/glnxa64")
install(TARGETS matlab_lib EXPORT matlab_lib_target ARCHIVE DESTINATION lib)
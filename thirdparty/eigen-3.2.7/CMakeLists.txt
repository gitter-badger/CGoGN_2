set(EIGEN_ROOT "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH "Directory for find_package(Eigen3)")
set(EIGEN3_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH "Include directory for find_package(Eigen3)")

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Eigen
        COMPONENT Eigen_headers
        DESTINATION include
        PATTERN "*.in" EXCLUDE
        PATTERN "*.txt" EXCLUDE
        PATTERN "*.cpp" EXCLUDE)
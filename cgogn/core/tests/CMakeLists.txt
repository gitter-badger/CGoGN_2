project(cgogn_core_test
	LANGUAGES CXX
)

set(SOURCE_FILES
	basic/dart_test.cpp
	basic/cell_test.cpp
	container/chunk_array_container_test.cpp
	cmap/cmap1_test.cpp
	utils/name_types_test.cpp

	main.cpp
)


add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(cgogn_core_test gtest gmock cgogn_core)

target_include_directories(cgogn_core_test PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty/googletest-master/googletest/include ${CMAKE_SOURCE_DIR}/thirdparty/googletest-master/googlemock/include)
link_directories(${CMAKE_SOURCE_DIR}/thirdparty/googletest-master/googletest/lib)
link_directories(${CMAKE_SOURCE_DIR}/thirdparty/googletest-master/googlemock/lib)


add_test(NAME "${PROJECT_NAME}" WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" COMMAND ${PROJECT_NAME})

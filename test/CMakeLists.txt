cmake_minimum_required(VERSION 3.9)

file(GLOB SRC_CXX *.cpp)
file(GLOB_RECURSE INCLUDE_CXX ${CMAKE_SOURCE_DIR}/common/include/*.h)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

foreach(code_test ${SRC_CXX})
	get_filename_component(MEXECUTABLE ${code_test} NAME_WE)
	include_directories(${CMAKE_SOURCE_DIR}/common/include)
	add_executable(${MEXECUTABLE} ${code_test})
	target_sources(${MEXECUTABLE}
		PRIVATE
		"${INCLUDE_CXX}")
	target_link_libraries(${MEXECUTABLE} common)
	install(TARGETS ${MEXECUTABLE}
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		RUNTIME DESTINATION bin
		PUBLIC_HEADER DESTINATION include
		)
endforeach()

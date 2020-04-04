cmake_minimum_required(VERSION 3.9)

file(GLOB SRC_CXX *.cpp)
file(GLOB_RECURSE INCLUDE_CXX *.h)
file(GLOB_RECURSE INC_CXX *.inc)

include_directories(include)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
#Compile to static library
add_library(common STATIC ${SRC_CXX} ${INCLUDE_CXX} ${INC_CXX})
install(TARGETS common
		LIBRARY DESTINATION lib
        	ARCHIVE DESTINATION lib
		)
install(FILES ${INCLUDE_CXX}
		DESTINATION include/common
		)

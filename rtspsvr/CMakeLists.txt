cmake_minimum_required(VERSION 3.9)

file(GLOB SRC_CXX *.cpp)
file(GLOB_RECURSE INCLUDE_CXX *.h)
file(GLOB_RECURSE INC_CXX *.inc)
file(GLOB_RECURSE INCLUDE_CXX ${CMAKE_SOURCE_DIR}/common/include/*.h)

string(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/common/include)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
#Compile to static library
add_library(${CURRENT_FOLDER} STATIC ${SRC_CXX} ${INCLUDE_CXX} ${INC_CXX})
target_link_libraries(${CURRENT_FOLDER} common)
install(TARGETS ${CURRENT_FOLDER}
		LIBRARY DESTINATION lib
        	ARCHIVE DESTINATION lib
		)
install(FILES ${INCLUDE_CXX}
		DESTINATION include/${CURRENT_FOLDER}
		)

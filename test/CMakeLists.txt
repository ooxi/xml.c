# xml.c / test
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR) 



# Example
add_executable(
	"${PROJECT_NAME}-example"
	"${CMAKE_CURRENT_LIST_DIR}/example.c"
)

target_compile_options(
	"${PROJECT_NAME}-example"
	PRIVATE
		-std=c11
)

target_link_libraries(
	"${PROJECT_NAME}-example"
	PRIVATE
		xml
)

add_test(
	NAME "${PROJECT_NAME}-example"
	COMMAND "${PROJECT_NAME}-example"
)



# Test case
FILE(	COPY "${CMAKE_CURRENT_LIST_DIR}/test.xml"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
)



# Test (C)
add_executable(
	"${PROJECT_NAME}-test-c"
	"${CMAKE_CURRENT_LIST_DIR}/test-xml-c.c"
)

target_compile_options(
	"${PROJECT_NAME}-test-c"
	PRIVATE
		-std=c11
)

target_link_libraries(
	"${PROJECT_NAME}-test-c"
	PRIVATE
		xml
)


add_test(
	NAME "${PROJECT_NAME}-test-c"
	COMMAND "${PROJECT_NAME}-test-c"
)

add_test(
	NAME "${PROJECT_NAME}-test-c-valgrind"
	COMMAND valgrind --tool=memcheck --leak-check=full --track-origins=yes -v "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-test-c"
)



# Test (C++)
add_executable(
	"${PROJECT_NAME}-test-cpp"
	"${CMAKE_CURRENT_LIST_DIR}/test-xml-cpp.cpp"
)

target_compile_options(
	"${PROJECT_NAME}-test-cpp"
	PRIVATE
		-std=c++11
)

target_link_libraries(
	"${PROJECT_NAME}-test-cpp"
	PRIVATE
		xml
)


add_test(
	NAME "${PROJECT_NAME}-test-cpp"
	COMMAND "${PROJECT_NAME}-test-cpp"
)


add_test(
	NAME "${PROJECT_NAME}-test-cpp-valgrind"
	COMMAND valgrind --tool=memcheck --leak-check=full --track-origins=yes -v "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-test-cpp"
)


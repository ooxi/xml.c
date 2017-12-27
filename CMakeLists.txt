# Project setup
project(xml C CXX)
set(VERSION_MAJOR "0")
set(VERSION_MINOR "2")
set(VERSION_PATCH "0")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR) 


# Define main library target
add_library(xml STATIC "")


# Compiler setup
target_compile_options(
	xml
	PRIVATE
		-std=c11
)


# Options
option(XML_PARSER_VERBOSE "Enable to be told everything the xml parser does" OFF)

if(XML_PARSER_VERBOSE)
	target_compile_definitions(
		xml
		PRIVATE
			XML_PARSER_VERBOSE
	)
endif(XML_PARSER_VERBOSE)


# Sources
target_sources(
	xml
	PRIVATE
		"${CMAKE_CURRENT_LIST_DIR}/src/xml.c"
)


target_include_directories(
	xml
	PUBLIC
		"${CMAKE_CURRENT_LIST_DIR}/src/"
)


# Build unit cases
enable_testing()
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/test")


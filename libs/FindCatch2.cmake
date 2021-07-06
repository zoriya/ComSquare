cmake_minimum_required(VERSION 3.11)


include(FetchContent)
FetchContent_Declare(Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        v3.0.0-preview3
)
FetchContent_GetProperties(Catch2)
if(NOT Catch2_POPULATED)
	FetchContent_Populate(Catch2)
	add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

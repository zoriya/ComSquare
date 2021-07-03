CMAKE_MINIMUM_REQUIRED(VERSION 3.11)


if (NOT Catch2_FOUND)
	Include(FetchContent)

	FetchContent_Declare(
		Catch2
		GIT_REPOSITORY https://github.com/catchorg/Catch2.git
		GIT_TAG        v2.13.6)

	FetchContent_MakeAvailable(Catch2)
endif()

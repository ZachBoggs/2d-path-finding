cmake_minimum_required(VERSION 3.14)

project(CPPProject VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(RAYLIB_VERSION 5.5.0)


if(UNIX)
    set(CMAKE_GENERATOR "Unix Makefiles")
endif()


# Find Raylib
find_package(raylib ${RAYLIB_VERSION} QUIET)
if( NOT raylib_FOUND)
	include(FetchContent)
	FetchContent_Declare(
		raylib 
		DOWNLOAD_EXTRACT_TIMESTAMP OFF 
		#URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
		URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
	)
	FetchContent_GetProperties(raylib)
	if(NOT raylib_POPULATED)
		set(FETCHCONTENT_QUIET NO)
		FetchContent_MakeAvailable(raylib)
		set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	endif()
endif()

include_directories(header)

file(GLOB SOURCES
	src/*.cpp
)

# Create an executable
add_executable(projectExecutable ${SOURCES})

# Link Raylib to the executable
target_link_libraries(projectExecutable raylib)

cmake_minimum_required(VERSION 3.28)
project(Typr)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(exec Typr)
set(src
	src/main.cpp
)

add_executable(${exec} ${src})
target_include_directories(${exec} PRIVATE
	${CMAKE_SOURCE_DIR}/include
)

if(UNIX)
	target_include_directories(${exec} SYSTEM PUBLIC
		${CMAKE_SOURCE_DIR}/vendor/raylib/unix/include
	)
	target_link_directories(${exec} PUBLIC
		${CMAKE_SOURCE_DIR}/vendor/raylib/unix/lib
	)
	if(RELEASE)
		target_compile_options(${exec} PRIVATE
			-fpermissive
			-O3
		)
	endif()
	target_link_libraries(${exec} PUBLIC
		raylib
	)
endif()

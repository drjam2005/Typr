cmake_minimum_required(VERSION 3.28)
project(Typr)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(exec Typr)
set(src
	src/main.cpp
	src/objects.cpp
	src/events.cpp
	src/updater.cpp
	src/renderer.cpp
	src/game.cpp
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
elseif(WIN32)
	target_include_directories(${exec} SYSTEM PUBLIC
		${CMAKE_SOURCE_DIR}/vendor/raylib/windows/include
	)
	target_link_directories(${exec} PUBLIC
		${CMAKE_SOURCE_DIR}/vendor/raylib/windows/lib
	)
	if(RELEASE)
		target_link_options(${exec} PRIVATE "-mwindows")
		target_compile_options(${exec} PRIVATE
			-fpermissive
			-O3
		)
	endif()
	target_link_libraries(${exec} PUBLIC
		raylib
		gdi32
		winmm
	)
endif()

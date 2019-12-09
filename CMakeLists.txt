cmake_minimum_required( VERSION 2.8 )

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/utils.cmake)

project( libnoise )

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Prevent creates default "CMakePredefinedTargets" folder
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "")

set( CMAKE_BUILD_TYPE Release CACHE STRING "Build Type." FORCE )
set( LIBNOISE_BUILD_SHARED_LIBS FALSE CACHE BOOL "Build shared libraries." )
set( LIBNOISE_BUILD_DOC FALSE CACHE BOOL "Build Doxygen documentation." )
set( LIBNOISE_BUILD_EXAMPLE TRUE CACHE BOOL "Build Examples." )

set( LIBNOISE_INCLUDE_DIR_NAME "noise" CACHE STRING "Define the name of the include directory for libnoise." )
set( LIBNOISE_SKIP_INSTALL FALSE CACHE BOOL "Don't install libnoise." )
mark_as_advanced( LIBNOISE_INCLUDE_DIR_NAME )
mark_as_advanced( LIBNOISE_SKIP_INSTALL )

if( LIBNOISE_BUILD_SHARED_LIBS )
	set( LIB_TYPE SHARED )
else()
	set( LIB_TYPE STATIC )
endif()

set( SRC_DIR "src" )
set( INC_DIR "include" )

set( SOURCES
include/noise/basictypes.h
include/noise/exception.h
include/noise/interp.h
include/noise/latlon.h
include/noise/mathconsts.h
include/noise/misc.h
include/noise/noise.h
include/noise/noisegen.h
include/noise/scalarparameter.h
include/noise/vectortable.h
include/noise/model/cylinder.h
include/noise/model/line.h
include/noise/model/model.h
include/noise/model/plane.h
include/noise/model/sphere.h
include/noise/module/abs.h
include/noise/module/add.h
include/noise/module/billow.h
include/noise/module/blend.h
include/noise/module/cache.h
include/noise/module/checkerboard.h
include/noise/module/clamp.h
include/noise/module/const.h
include/noise/module/curve.h
include/noise/module/cylinders.h
include/noise/module/displace.h
include/noise/module/exponent.h
include/noise/module/gradient.h
include/noise/module/invert.h
include/noise/module/max.h
include/noise/module/min.h
include/noise/module/module.h
include/noise/module/modulebase.h
include/noise/module/multiply.h
include/noise/module/perlin.h
include/noise/module/power.h
include/noise/module/ridgedmulti.h
include/noise/module/rotatedomain.h
include/noise/module/scalebias.h
include/noise/module/scaledomain.h
include/noise/module/select.h
include/noise/module/spheres.h
include/noise/module/terrace.h
include/noise/module/translatedomain.h
include/noise/module/turbulence.h
include/noise/module/voronoi.h
src/latlon.cpp
src/noisegen.cpp
src/scalarparameter.cpp
src/model/cylinder.cpp
src/model/line.cpp
src/model/plane.cpp
src/model/sphere.cpp
src/module/abs.cpp
src/module/add.cpp
src/module/billow.cpp
src/module/blend.cpp
src/module/cache.cpp
src/module/checkerboard.cpp
src/module/clamp.cpp
src/module/const.cpp
src/module/curve.cpp
src/module/cylinders.cpp
src/module/displace.cpp
src/module/exponent.cpp
src/module/gradient.cpp
src/module/invert.cpp
src/module/max.cpp
src/module/min.cpp
src/module/modulebase.cpp
src/module/multiply.cpp
src/module/perlin.cpp
src/module/power.cpp
src/module/ridgedmulti.cpp
src/module/rotatedomain.cpp
src/module/scalebias.cpp
src/module/scaledomain.cpp
src/module/select.cpp
src/module/spheres.cpp
src/module/terrace.cpp
src/module/translatedomain.cpp
src/module/turbulence.cpp
src/module/voronoi.cpp
)

group_sources("${SOURCES}")

add_library( libnoise ${LIB_TYPE} ${SOURCES} )

target_include_directories(libnoise PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/noise>
	$<INSTALL_INTERFACE:include>
)

# GCC will automatically add the prefix lib
if( CMAKE_COMPILER_IS_GNUCXX )
	set_target_properties( libnoise PROPERTIES OUTPUT_NAME noise )
endif()

if(LIBNOISE_BUILD_DOC)
	add_subdirectory( doc )
endif()

if( NOT LIBNOISE_SKIP_INSTALL )
	install(
		TARGETS libnoise
		EXPORT libnoise-targets
		RUNTIME DESTINATION bin COMPONENT bin
		LIBRARY DESTINATION lib COMPONENT bin
		ARCHIVE DESTINATION lib COMPONENT dev
	)

	install(EXPORT libnoise-targets
		FILE libnoise-config.cmake
		DESTINATION lib/cmake/libnoise
	)

	install(
		DIRECTORY include/noise/
		DESTINATION include/${LIBNOISE_INCLUDE_DIR_NAME}
	)
endif()

if (LIBNOISE_BUILD_EXAMPLE)
add_subdirectory(utils)
add_subdirectory(examples)
endif ()

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

set( SOURCES
source/noise/module/abs.cpp
source/noise/module/abs.h
source/noise/module/add.cpp
source/noise/module/add.h
source/noise/basictypes.h
source/noise/module/billow.cpp
source/noise/module/billow.h
source/noise/module/blend.cpp
source/noise/module/blend.h
source/noise/module/cache.cpp
source/noise/module/cache.h
source/noise/module/checkerboard.cpp
source/noise/module/checkerboard.h
source/noise/module/clamp.cpp
source/noise/module/clamp.h
source/noise/module/const.cpp
source/noise/module/const.h
source/noise/module/curve.cpp
source/noise/module/curve.h
source/noise/model/cylinder.cpp
source/noise/model/cylinder.h
source/noise/module/cylinders.cpp
source/noise/module/cylinders.h
source/noise/module/displace.cpp
source/noise/module/displace.h
source/noise/exception.h
source/noise/module/exponent.cpp
source/noise/module/exponent.h
source/noise/module/gradient.cpp
source/noise/module/gradient.h
source/noise/interp.h
source/noise/module/invert.cpp
source/noise/module/invert.h
source/noise/latlon.cpp
source/noise/latlon.h
source/noise/model/line.cpp
source/noise/model/line.h
source/noise/mathconsts.h
source/noise/module/max.cpp
source/noise/module/max.h
source/noise/module/min.cpp
source/noise/module/min.h
source/noise/misc.h
source/noise/model/model.h
source/noise/module/module.h
source/noise/module/modulebase.cpp
source/noise/module/modulebase.h
source/noise/module/multiply.cpp
source/noise/module/multiply.h
source/noise/noise.h
source/noise/noisegen.cpp
source/noise/noisegen.h
source/noise/module/perlin.cpp
source/noise/module/perlin.h
source/noise/model/plane.cpp
source/noise/model/plane.h
source/noise/module/power.cpp
source/noise/module/power.h
source/noise/module/ridgedmulti.cpp
source/noise/module/ridgedmulti.h
source/noise/module/rotatedomain.cpp
source/noise/module/rotatedomain.h
source/noise/scalarparameter.cpp
source/noise/scalarparameter.h
source/noise/module/scalebias.cpp
source/noise/module/scalebias.h
source/noise/module/scaledomain.cpp
source/noise/module/scaledomain.h
source/noise/module/select.cpp
source/noise/module/select.h
source/noise/model/sphere.cpp
source/noise/model/sphere.h
source/noise/module/spheres.cpp
source/noise/module/spheres.h
source/noise/module/terrace.cpp
source/noise/module/terrace.h
source/noise/module/translatedomain.cpp
source/noise/module/translatedomain.h
source/noise/module/turbulence.cpp
source/noise/module/turbulence.h
source/noise/vectortable.h
source/noise/module/voronoi.cpp
source/noise/module/voronoi.h
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

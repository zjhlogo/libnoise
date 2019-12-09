macro(group_sources SOURCE_FILES)
	foreach(FILE ${SOURCE_FILES})
		get_filename_component(PARENT_DIR "${FILE}" PATH)

		# skip source and changes /'s to \\'s
		string(REGEX REPLACE "(\\./)?(source)/?" "" GROUP "${PARENT_DIR}")
		string(REPLACE "/" "\\" GROUP "${GROUP}")
		source_group("${GROUP}" FILES "${FILE}")
	endforeach()
endmacro()

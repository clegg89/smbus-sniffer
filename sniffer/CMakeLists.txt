cmake_minimum_required (VERSION 3.27)

set(APP_NAME smbus-sniffer)

add_executable(${APP_NAME})

set_target_properties(${APP_NAME}
  PROPERTIES
    C_STANDARD 23
    CXX_STANDARD 23
)

add_subdirectory(app)
add_subdirectory(gcc)
add_subdirectory(submodules)

get_target_property(ALL_SOURCES ${APP_NAME} SOURCES)
foreach(SOURCE_FILE IN LISTS ALL_SOURCES)
	cmake_path(GET SOURCE_FILE EXTENSION SOURCE_TYPE)
	if(SOURCE_TYPE STREQUAL ".c" OR SOURCE_TYPE STREQUAL ".cpp")
		cmake_path(GET SOURCE_FILE STEM SOURCE_NAME)

		cmake_path(
			GET SOURCE_FILE
			PARENT_PATH SOURCE_DIR
		)

		cmake_path(
			RELATIVE_PATH SOURCE_DIR
			BASE_DIRECTORY "${CMAKE_SOURCE_DIR}"
			OUTPUT_VARIABLE SOURCE_PATH
		)

		set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/${SOURCE_PATH}")

		if(NOT EXISTS ${OUTPUT_DIR})
			file(MAKE_DIRECTORY ${OUTPUT_DIR})
		endif()

		# Unclear if this is actually necessary,
		# the set_source_file_properties _may_ just append compiler options
		# but the documentation is vague
		get_source_file_property(COMPILE_OPTIONS ${SOURCE_FILE}
			TARGET_DIRECTORY ${APP_NAME}
			COMPILE_OPTIONS
		)

		if(NOT COMPILE_OPTIONS)
			set(COMPILE_OPTIONS "")
		endif()


		list(APPEND COMPILE_OPTIONS -dumpdir "${OUTPUT_DIR}/")
		list(APPEND COMPILE_OPTIONS "-Wa,-adhlns=${OUTPUT_DIR}/${SOURCE_NAME}.lst")

		set_source_files_properties(${SOURCE_FILE}
			TARGET_DIRECTORY ${APP_NAME}
			PROPERTIES COMPILE_OPTIONS "${COMPILE_OPTIONS}"
		)
	endif()
endforeach()

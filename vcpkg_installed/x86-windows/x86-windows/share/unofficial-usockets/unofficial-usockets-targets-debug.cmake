#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "unofficial::usockets::usockets" for configuration "Debug"
set_property(TARGET unofficial::usockets::usockets APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(unofficial::usockets::usockets PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/debug/lib/uSockets.lib"
  )

list(APPEND _cmake_import_check_targets unofficial::usockets::usockets )
list(APPEND _cmake_import_check_files_for_unofficial::usockets::usockets "${_IMPORT_PREFIX}/debug/lib/uSockets.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

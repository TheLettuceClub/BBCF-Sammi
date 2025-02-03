#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "unofficial::usockets::usockets" for configuration "Release"
set_property(TARGET unofficial::usockets::usockets APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(unofficial::usockets::usockets PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/uSockets.lib"
  )

list(APPEND _cmake_import_check_targets unofficial::usockets::usockets )
list(APPEND _cmake_import_check_files_for_unofficial::usockets::usockets "${_IMPORT_PREFIX}/lib/uSockets.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

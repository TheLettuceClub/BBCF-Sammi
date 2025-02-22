include(CMakeFindDependencyMacro)
find_dependency(unofficial-usockets CONFIG)
if(NOT "1")
    find_dependency(ZLIB)
endif()

if(NOT TARGET unofficial::uwebsockets::uwebsockets)
    add_library(unofficial::uwebsockets::uwebsockets INTERFACE IMPORTED)
    target_compile_features(unofficial::uwebsockets::uwebsockets INTERFACE cxx_std_17)
    get_filename_component(_uws_include_dir "../../include" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_LIST_DIR}")
    target_include_directories(unofficial::uwebsockets::uwebsockets INTERFACE "${_uws_include_dir}")
    unset(_uws_include_dir)
    target_link_libraries(unofficial::uwebsockets::uwebsockets INTERFACE $<LINK_ONLY:unofficial::usockets::usockets>)
    if("1")
        target_compile_definitions(unofficial::uwebsockets::uwebsockets INTERFACE UWS_NO_ZLIB)
    else()
        target_link_libraries(unofficial::uwebsockets::uwebsockets INTERFACE ZLIB::ZLIB)
    endif()
endif()

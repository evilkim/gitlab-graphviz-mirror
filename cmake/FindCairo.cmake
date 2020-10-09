include(FindPackageHandleStandardArgs)

if (WIN32)
    find_path(Cairo_INCLUDE_DIR cairo/cairo.h)

    find_library(Cairo_LIBRARY NAMES cairo)

    find_program(Cairo_RUNTIME_LIBRARY NAMES cairo.dll)
    find_program(EXPAT_RUNTIME_LIBRARY NAMES expat.dll)
    find_program(FONTCONFIG_RUNTIME_LIBRARY NAMES fontconfig.dll)
    find_program(PIXMAN_RUNTIME_LIBRARY NAMES pixman-1.dll)

    find_package_handle_standard_args(Cairo DEFAULT_MSG
        Cairo_INCLUDE_DIR

        Cairo_LIBRARY

        Cairo_RUNTIME_LIBRARY
        EXPAT_RUNTIME_LIBRARY
        FONTCONFIG_RUNTIME_LIBRARY
        PIXMAN_RUNTIME_LIBRARY
    )

    set(Cairo_INCLUDE_DIRS ${Cairo_INCLUDE_DIR})

    set(Cairo_LIBRARIES ${Cairo_LIBRARY})

    set(Cairo_LINK_LIBRARIES ${Cairo_LIBRARY})

    set(Cairo_RUNTIME_LIBRARIES
        ${Cairo_RUNTIME_LIBRARY}
        ${EXPAT_RUNTIME_LIBRARY}
        ${FONTCONFIG_RUNTIME_LIBRARY}
        ${PIXMAN_RUNTIME_LIBRARY}
    )
else()
    find_package(PkgConfig)
    pkg_check_modules(Cairo cairo)

    # prior to CMake 3.12, PkgConfig does not set *_LINK_LIBRARIES, so do it
    # manually
    if (${CMAKE_VERSION} VERSION_LESS 3.12)
        find_library(Cairo_LINK_LIBRARIES cairo PATHS ENV LIBRARY_PATH)
    endif()

    find_package_handle_standard_args(Cairo DEFAULT_MSG
        Cairo_INCLUDE_DIRS
        Cairo_LIBRARIES
        Cairo_LINK_LIBRARIES
    )
endif()

include(FindPackageHandleStandardArgs)

if (WIN32)
    find_path(Cairo_INCLUDE_DIR cairo/cairo.h)

    find_library(Cairo_LIBRARY NAMES cairo)

    find_file(Cairo_RUNTIME_LIBRARY NAMES cairo.dll)
    find_file(EXPAT_RUNTIME_LIBRARY NAMES expat.dll)
    find_file(FONTCONFIG_RUNTIME_LIBRARY NAMES fontconfig.dll)
    find_file(PIXMAN_RUNTIME_LIBRARY NAMES pixman-1.dll)

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

    find_package_handle_standard_args(Cairo DEFAULT_MSG
        Cairo_INCLUDE_DIRS
        Cairo_LIBRARIES
        Cairo_LINK_LIBRARIES
    )
endif()

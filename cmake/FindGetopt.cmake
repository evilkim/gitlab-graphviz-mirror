include(FindPackageHandleStandardArgs)

find_path(Getopt_INCLUDE_DIR getopt.h)
find_library(Getopt_LIBRARY NAMES getopt)
find_program(Getopt_RUNTIME_LIBRARY NAMES getopt.dll)

if (WIN32)
    find_package_handle_standard_args(Getopt DEFAULT_MSG
        Getopt_INCLUDE_DIR
        Getopt_LIBRARY
        Getopt_RUNTIME_LIBRARY
    )
else()
    find_package_handle_standard_args(Getopt DEFAULT_MSG
        Getopt_INCLUDE_DIR
        Getopt_LIBRARY
    )
endif()

set(Getopt_INCLUDE_DIRS ${Getopt_INCLUDE_DIR})
set(Getopt_LIBRARIES ${Getopt_LIBRARY})
set(Getopt_LINK_LIBRARIES ${Getopt_LIBRARY})
set(Getopt_RUNTIME_LIBRARIES ${Getopt_RUNTIME_LIBRARY})

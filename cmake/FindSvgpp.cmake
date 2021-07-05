find_path(Svgpp_INCLUDE_DIR svgpp/svgpp.hpp)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Svgpp DEFAULT_MSG Svgpp_INCLUDE_DIR)

set(Svgpp_INCLUDE_DIRS ${Svgpp_INCLUDE_DIR})

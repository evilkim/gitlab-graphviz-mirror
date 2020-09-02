find_path(RxSpencer_INCLUDE_DIR regex.h)
find_library(RxSpencer_LIBRARY NAMES rxspencer)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RxSpencer DEFAULT_MSG
    RxSpencer_LIBRARY
    RxSpencer_INCLUDE_DIR
)

set(RxSpencer_INCLUDE_DIRS ${RxSpencer_INCLUDE_DIR})
set(RxSpencer_LIBRARIES ${RxSpencer_LIBRARY})

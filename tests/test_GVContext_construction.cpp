#include <utility>

#include <catch2/catch.hpp>

#include <gvc++/GVContext.h>

TEST_CASE("GVContext can be constructed with built-in plugins and its "
          "underlying C data structure can be retrieved") {
  const auto demand_loading = false;
  GVC::GVContext gvc{lt_preloaded_symbols, demand_loading};
  REQUIRE(gvc.c_struct() != nullptr);
}

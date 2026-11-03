#include "prx/libSceLibreSsl3/include/Api.hpp"
#include "prx/libSceLibreSsl3/libSceLibreSsl3.hpp"

#include <cstdio>
#include <cstdlib>
#include <source_location>
#include <stdexcept>

namespace {

void Require(bool condition, std::source_location location = std::source_location::current()) {
    if (condition) return;
    std::fprintf(stderr, "LibreSSL3 EVP_PKEY_base_id check failed at line %u\n", location.line());
    std::abort();
}

int BaseOf(std::int32_t type) {
    LibreSsl3EvpPkey key{};
    key.type = type;
    key.references = 1;
    return EVP_PKEY_base_id_nid_postfix(&key);
}

}

int main() {
    Require(BaseOf(6) == 6);
    Require(BaseOf(19) == 6);
    Require(BaseOf(912) == 912);
    Require(BaseOf(116) == 116);
    Require(BaseOf(67) == 116);
    Require(BaseOf(66) == 116);
    Require(BaseOf(113) == 116);
    Require(BaseOf(70) == 116);
    Require(BaseOf(28) == 28);
    Require(BaseOf(408) == 408);
    Require(BaseOf(855) == 855);
    Require(BaseOf(894) == 894);
    Require(BaseOf(950) == 950);
    Require(BaseOf(952) == 952);
    Require(BaseOf(0) == 0);
    Require(BaseOf(920) == 0);
    Require(BaseOf(954) == 0);
    Require(BaseOf(-1) == 0);

    bool threw = false;
    try {
        EVP_PKEY_base_id_nid_postfix(nullptr);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    Require(threw);

    std::puts("LibreSSL3 EVP_PKEY_base_id checks passed");
    return 0;
}

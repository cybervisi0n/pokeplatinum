#pragma once

#include "elf_object.hpp"

#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace ps5::module
{

struct Options
{
    std::string entry = "_start";
    std::string file_name = "eboot.elf";
    std::uint32_t module_sdk = 0x02000009;
    std::uint32_t companion_sdk = 0x08050001;
    std::vector<std::string> version_components;
};

[[nodiscard]] elf::Bytes write_executable(const elf::Image &image, std::span<const elf::Stub> stubs,
                                          const Options &options = {});

} // namespace ps5::module

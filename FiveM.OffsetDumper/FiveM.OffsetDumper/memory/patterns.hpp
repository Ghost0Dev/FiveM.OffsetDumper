#ifndef PATTERNS_HPP
#define PATTERNS_HPP

#include <string>
#include <vector>

namespace patterns {
    struct Pattern {
        std::string name;
        std::string signature;
        bool should_dereference;
    };

    const std::vector<Pattern> offsets = {
        {"camera", "4C 8B 35 ? ? ? ? 33 FF 32 DB", true},
        {"replay_interface", "48 8D 0D ? ? ? ? 48 ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? 8A D8 E8 ? ? ? ? 84 DB 75 13 48 8D 0D ? ? ? ? 48 8B D7 E8 ? ? ? ? 84 C0 74 BC 8B 8F", false},
        {"world", "48 8B 05 ? ? ? ? 33 D2 48 8B 40 08 8A CA 48 85 C0 74 16 48 8B", true},
        {"viewport", "48 8B 15 ? ? ? ? 48 8D 2D ? ? ? ? 48 8B CD", true},
        {"blip_list", "4C 8D 05 ? ? ? ? 0F B7 C1", false},
        {"bullet", "F3 41 0F 10 19 F3 41 0F 10 41 04", true},
        {"c_sky_settings", "48 8D 0D ? ? ? ? E8 ? ? ? ? 83 25 ? ? ? ? 00 48 8D 0D ? ? ? ? F3", true},
        {"aim_cped", "48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B 0D ? ? ? ? 48 85 C9 74 05 E8 ? ? ? ? 8A CB", true},
        {"set_ped_in_to_vehicle", "48 8B C4 44 89 48 ? 44 89 40 ? 48 89 50 ? 48 89 48 ? 55 53 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 83 BA", true}
    };
}

#endif // PATTERNS_HPP
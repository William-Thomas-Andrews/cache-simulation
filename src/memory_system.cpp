#include "memory_system.hpp"

MemorySystem::MemorySystem() {
    Cache L1, L2, L3;
    RAM ram;

    L1.lower_level_ = &L2;
    L2.lower_level_ = &L3;
    L3.lower_level_ = &ram;
}

uint8_t MemorySystem::read_zero() {
    Address address = Address{0, 0, 0};
    uint8_t found = *(L1_cache_.read(address));
    std::cout << found << std::endl;
    return found;
}
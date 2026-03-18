#include "cache.hpp"
#include <iostream>

class MemorySystem {
    public:
        Cache L1_cache_;
        Cache L2_cache_;
        Cache L3_cache_;
        RAM ram_;

        MemorySystem();

        uint8_t read_zero();

    private:
};
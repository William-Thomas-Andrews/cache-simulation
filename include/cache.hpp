#include <cstdint>
#include <vector>
#include <random>
#include <limits>

// Pentium 4 L1 cache: 
// 4 way associative
// 8KB cache size
// 64 byte cache blocks

// Hence, there are 8KB / 64B = 128 cache blocks.
// There are 128 / 4(way) = 32 sets.
// Where 32 = 2^5 different indices and,
// 64 = 2^6 different offsets.
// Since the CPU address is 32 bits, 32 = 5 + 6 + 21 implies that there are 21 bits of the tag field.

struct Address {
    uint32_t tag;           // 21 bits
    uint8_t set;            // 5 bits
    uint8_t block_offset    // 6 bits
};

struct CacheLine {
    bool valid;
    uint32_t tag;
    uint8_t data[64];
};

class L1Cache {
    public:
        L1Cache() = default;
        L1Cache(std::vector<uint8_t> input_data);

        uint8_t read(Address address);
        std::vector<uint8_t> batch_read(std::vector<Address> addresses);

    private:
        CacheLine lines_[128];
        std::random_device rd_;                     // for random eviction (obtains a seed)
        std::mt19937 gen_;                         // Standard mersenne twister engine seeded with rd()
        std::uniform_int_distribution<> distrib_;  // Define the desired distribution


        void write(uint8_t input);
        void batch_write(std::vector<uint8_t> input_data);
        void evict_and_replace(uint8_t set_index);
};
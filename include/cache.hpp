#include <cstdint>
#include <vector>
#include <random>
#include <limits>
#include <cstring>
#include <stdexcept>


// Pentium 4 L1 cache: 
// 4 way associative
// 8KB cache size
// 64 byte cache blocks

// Hence, there are 8KB / 64B = 128 cache blocks.
// There are 128 / 4(way) = 32 sets.
// Where 32 = 2^5 different indices and,
// 64 = 2^6 different offsets.
// Since the CPU address is 32 bits, 32 = 5 + 6 + 21 implies that there are 21 bits of the tag field.

#define WAY 4
#define CACHE_SIZE_BYTES 8192
#define CACHE_BLOCK_SIZE_BYTES 64
#define MAX_INT_VAL 255
#define ADDRESS_BITS 32
#define ADDRESS_FIELD_PRECISION 32

struct Address {
    uint32_t tag;           // 21 bits
    uint32_t set;            // 5 bits
    uint32_t block_offset;   // 6 bits
};

struct CacheLine {
    bool valid;
    uint32_t tag;
    uint8_t data[CACHE_BLOCK_SIZE_BYTES];
};

class Cache {
    public:
        Cache();
        Cache(std::vector<Address> input_data);
        ~Cache();

        uint8_t read(Address address);
        std::vector<uint8_t> batch_read(std::vector<Address> addresses);

    private:
        static constexpr int nblocks_ = CACHE_SIZE_BYTES / CACHE_BLOCK_SIZE_BYTES;
        static constexpr int nsets_ = nblocks_ / WAY;
        static constexpr int nset_bits_ = std::log2(nsets_);
        static constexpr int nblock_offset_bits_ = std::log2(CACHE_BLOCK_SIZE_BYTES);
        static constexpr int ntag_bits_ = ADDRESS_BITS - nblock_offset_bits_ - nset_bits_;
        static constexpr size_t ram_size_ = 1ULL << 18; 
        static constexpr size_t ram_bytes_ = ram_size_ * CACHE_BLOCK_SIZE_BYTES;

        CacheLine lines_[nblocks_];
        uint8_t* ram_;
        std::random_device rd_;
        std::mt19937 gen_{rd_()};                  // seeded once at construction

        // void write(uint8_t input);
        // void batch_write(std::vector<uint8_t> input_data);
        uint8_t* fetch_lower(uint32_t ram_index);
        uint8_t gen_random();
        int get_ram_block_index(Address address);
        uint8_t evict_and_replace(Address address);
        
};
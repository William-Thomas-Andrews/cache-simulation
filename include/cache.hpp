#include <cstdint>
#include <vector>
#include <random>
#include <limits>
#include <cstring>
#include <stdexcept>
#include <memory>
#include <type_traits>



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

class IMemory {
public:
    virtual uint8_t* read(Address address) = 0;
    virtual ~IMemory() = default;
};

struct CacheLine {
    bool valid;
    uint32_t tag;
    uint8_t data[CACHE_BLOCK_SIZE_BYTES];
};

struct RAM : public IMemory {
    static constexpr int nblocks = CACHE_SIZE_BYTES / CACHE_BLOCK_SIZE_BYTES;
    static constexpr int nsets = nblocks / WAY;
    static constexpr int nset_bits = std::log2(nsets);
    static constexpr size_t ram_size = 1ULL << 18; 
    static constexpr size_t ram_bytes = ram_size * CACHE_BLOCK_SIZE_BYTES;

    RAM();
    ~RAM();

    uint8_t* read(Address address);
    uint8_t* data;
};

enum class Type {
    L1,
    L2,
    L3
};

class Cache : public IMemory {
    public:
        IMemory* lower_level_;

        Cache() = default;
        Cache(std::vector<Address> input_data);
        ~Cache() = default;

        uint8_t* read(Address address);
        std::vector<uint8_t> batch_read(std::vector<Address> addresses);

    private:
        static constexpr int nblocks_ = CACHE_SIZE_BYTES / CACHE_BLOCK_SIZE_BYTES;
        static constexpr int nsets_ = nblocks_ / WAY;
        static constexpr int nset_bits_ = std::log2(nsets_);
        static constexpr int nblock_offset_bits_ = std::log2(CACHE_BLOCK_SIZE_BYTES);
        static constexpr int ntag_bits_ = ADDRESS_BITS - nblock_offset_bits_ - nset_bits_;

        CacheLine lines_[nblocks_];

        std::random_device rd_;
        std::mt19937 gen_{rd_()};                  // seeded once at construction

        uint8_t* fetch_lower(Address address);
        uint8_t gen_random();
        uint8_t* evict_and_replace(Address address);
};
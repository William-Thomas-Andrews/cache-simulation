#include "cache.hpp"

L1Cache::L1Cache() {
    // allocate ram
    for (int i = 0; i < ram_size_; i++) 
        ram_[i] = new uint8_t[CACHE_BLOCK_SIZE_BYTES];

    // no data to read in yet
}

L1Cache::L1Cache(std::vector<Address> input_data) {
    // allocate ram
    for (int i = 0; i < ram_size_; i++) 
        ram_[i] = new uint8_t[CACHE_BLOCK_SIZE_BYTES];
    
    // read in data to cache
    batch_read(input_data);
}

uint8_t L1Cache::read(Address address) {
    CacheLine line; 
    // iterate through blocks, cache WAY times
    for (uint32_t i = 0; i < WAY; i++) {
        line = lines_[address.set + i];
        // if tag match
        if (address.tag == line.tag) {
            uint8_t element = line.data[address.block_offset];
            return element; // return element found
        }
    }
    // if no tag match found in cache, evict and replace.
    return evict_and_replace(address);
}

std::vector<uint8_t> L1Cache::batch_read(std::vector<Address> addresses) {
    std::vector<uint8_t> result;
    for (Address address : addresses)
        result.push_back(read(address));
}

// void L1Cache::write(uint8_t input) {

// }

// void L1Cache::batch_write(std::vector<uint8_t> input_data) {

// }

uint8_t* L1Cache::fetch_data(uint32_t ram_index) {
    if (ram_index >= ram_size_) throw std::out_of_range("Error in RAM data fetch: index out of range.");
    return ram_[ram_index];
}

uint8_t L1Cache::gen_random() {
    // set up random number generator for random eviction
    std::mt19937 gen(rd_());                         // Standard mersenne twister engine seeded with rd()
    std::uniform_int_distribution<uint8_t> distrib(0, MAX_INT_VAL);  // Define the desired distribution (e.g., integers between 0 and 255 which is uint8_t max)
    return distrib(gen);
}

uint8_t L1Cache::evict_and_replace(Address address) {
    // Generate the random number mod WAY
    uint8_t r = gen_random() % WAY;
    
    // replace the metadata
    lines_[address.set + r].valid = true;                               // valid bit set to true
    lines_[address.set + r].tag = address.tag;                          // tag bit set
    memcpy(lines_[address.set + r].data, fetch_data(address.tag), CACHE_BLOCK_SIZE_BYTES);  // data copied from lower level
    
    // return the found uint8_t
    return lines_[address.set + r].data[address.block_offset];
}
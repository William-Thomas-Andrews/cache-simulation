#include "cache.hpp"

Cache::Cache() {
    // allocate ram
    ram_ = new uint8_t[ram_bytes_];

    // no data to read in yet
}

Cache::LCache(std::vector<Address> input_data) {
    // allocate ram
    ram_ = new uint8_t[ram_bytes_];
    
    // read in data to cache
    batch_read(input_data);
}

Cache::~Cache() {
    free(ram_);
}

uint8_t Cache::read(Address address) {
    CacheLine line; 
    // iterate through blocks, cache WAY times
    for (uint32_t i = 0; i < WAY; i++) {
        line = lines_[address.set * WAY + i];
        // if tag match
        if (address.tag == line.tag and line.valid == true) {
            uint8_t element = line.data[address.block_offset];
            return element; // return element found
        }
    }
    // if no tag match found in cache, evict and replace.
    return evict_and_replace(address);
}

std::vector<uint8_t> Cache::batch_read(std::vector<Address> addresses) {
    std::vector<uint8_t> result;
    for (Address address : addresses)
        result.push_back(read(address));
    return result;
}

// void Cache::write(uint8_t input) {

// }

// void Cache::batch_write(std::vector<uint8_t> input_data) {

// }

uint8_t* Cache::fetch_lower(uint32_t ram_index) {
    if (ram_index >= ram_bytes_) throw std::out_of_range("Error in data fetch: index out of range.");
    return &ram_[ram_index];
}

uint8_t Cache::gen_random() {
    std::uniform_int_distribution<uint8_t> distrib(0, MAX_INT_VAL);
    return distrib(gen_);
}

int Cache::get_ram_block_index(Address address) {
    return (address.tag << nset_bits_) | address.set;
}

uint8_t Cache::evict_and_replace(Address address) {
    // Generate the random number mod WAY
    uint8_t r = gen_random() % WAY;
    
    // replace the metadata
    lines_[address.set * WAY + r].valid = true;                               // valid bit set to true
    lines_[address.set * WAY + r].tag = address.tag;                          // tag bit set
    int ram_index = get_ram_block_index(address) % ram_bytes_;

    memcpy(lines_[address.set * WAY + r].data, fetch__lower(ram_index), CACHE_BLOCK_SIZE_BYTES);  // data copied from lower level
    
    // return the found uint8_t
    return lines_[address.set * WAY + r].data[address.block_offset];
}
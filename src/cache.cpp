#include "cache.hpp"

RAM::RAM() {
    data = new uint8_t[ram_bytes]; // an array of 64B segments
    uint8_t entry = 0;
    for (size_t i = 0; i < ram_bytes; i++) {
        data[i] = entry;
        entry = (entry + 1) % 64;
    }
}

RAM::~RAM() {
    free(data);
}

uint8_t* RAM::read(Address address) {
    size_t ram_index = ((address.tag << nset_bits) | address.set) % ram_bytes;
    return &data[ram_index * CACHE_BLOCK_SIZE_BYTES];
}

Cache::Cache(std::vector<Address> input_data) {
    // read in data to cache
    batch_read(input_data);
}

uint8_t* Cache::read(Address address) {
    CacheLine* line; 
    // iterate through blocks, cache WAY times
    for (uint32_t i = 0; i < WAY; i++) {
        line = &lines_[address.set * WAY + i];
        // if tag match
        if (address.tag == line->tag and line->valid == true)
            return &line->data[address.block_offset]; // return address of element found
    }
    // if no tag match found in cache, evict and replace.
    return evict_and_replace(address);
}

std::vector<uint8_t> Cache::batch_read(std::vector<Address> addresses) {
    std::vector<uint8_t> result;
    for (Address address : addresses)
        result.push_back(*read(address));
    return result;
}

uint8_t* Cache::fetch_lower(Address address) {
    return lower_level_->read(address);
}

uint8_t Cache::gen_random() {
    std::uniform_int_distribution<uint8_t> distrib(0, MAX_INT_VAL);
    return distrib(gen_);
}

uint8_t* Cache::evict_and_replace(Address address) {
    // Generate the random number mod WAY
    uint8_t r = gen_random() % WAY;
    
    // replace the metadata
    lines_[address.set * WAY + r].valid = true;                               // valid bit set to true
    lines_[address.set * WAY + r].tag = address.tag;                          // tag bit set
    memcpy(lines_[address.set * WAY + r].data, fetch_lower(address), CACHE_BLOCK_SIZE_BYTES);  // data copied from lower level
    
    // return the found uint8_t
    return &lines_[address.set * WAY + r].data[address.block_offset];
}
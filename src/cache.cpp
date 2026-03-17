#include "cache.hpp"

L1Cache::L1Cache() : nblocks_(CACHE_SIZE_BYTES / CACHE_BLOCK_SIZE_BYTES), 
    nsets_(nblocks_ / WAY), 
    nindices_(std::log2(nset_bits_)),
    nblock_offset_bits_(std::log2(CACHE_BLOCK_SIZE_BYTES)),
    ntag_bits_(nsets_ - nblock_offset_bits_ - nindices_)
{
    // allocate ram
    for (int i = 0; i < ram_size; i++) 
        ram_[i] = new int[CACHE_BLOCK_SIZE_BYTES];

    // set up random number generator for random eviction
    gen_(rd_());
    distrib_(0, 255); // Define the desired distribution (e.g., integers between 0 and 255 which is uint8_t max)
}

L1Cache::L1Cache(std::vector<Address> input_data) {
    // allocate ram
    for (int i = 0; i < ram_size; i++) 
        ram_[i] = new int[CACHE_BLOCK_SIZE_BYTES];
    
    // set up random number generator for random eviction
    gen_(rd_());
    distrib_(0, 255); // Define the desired distribution (e.g., integers between 0 and 255 which is uint8_t max)
    
    // read in data to cache
    batch_read(input_data);
}

uint8_t L1Cache::read(Address address) {
    CacheLine line; 
    // iterate through blocks, cache WAY times
    for (uint8_t i = 0; i < WAY; i++) {
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
    for (Address address : addresses)
        read(address);
}

// void L1Cache::write(uint8_t input) {

// }

// void L1Cache::batch_write(std::vector<uint8_t> input_data) {

// }

uint8_t* fetch_data(uint32_t ram_index) {
    if (ram_index >= ram_size) throw std::out_of_range("Error in RAM data fetch: index out of range.");
    return ram_[ram_index];
}

uint8_t L1Cache::evict_and_replace(Address address) {
    // Generate the random number mod WAY
    uint8_t r = static_cast<uint8_t>(distrib_(gen_)) % WAY;
    
    // replace the metadata
    lines_[address.set + r].valid = true;                               // valid bit set to true
    lines_[address.set + r].tag = address.tag;                          // tag bit set
    memcpy(lines_[address.set + r].data, fetch_data(address.tag), 64);  // data copied from lower level
    
    // return the found uint8_t
    return lines_[address.set + r].data[address.block_offset];
}
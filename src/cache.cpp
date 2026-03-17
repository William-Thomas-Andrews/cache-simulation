#include "cache.hpp"

#define WAY 4

L1Cache::L1Cache(std::vector<Address> input_data) {
    gen_(rd_());
    distrib_(0, 255); // Define the desired distribution (e.g., integers between 1 and 100)
    

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
    evict_and_replace(address.set);
}

std::vector<uint8_t> L1Cache::batch_read(std::vector<Address> addresses) {
    for (Address address : addresses)
        read(address);
}

void L1Cache::write(uint8_t input) {

}

void L1Cache::batch_write(std::vector<uint8_t> input_data) {

}

void L1Cache::evict_and_replace(uint8_t set_index) {
    

    // 2. 

    // 3. 
    // 4. Generate and print the random number
    int random_number = distrib(gen);
}
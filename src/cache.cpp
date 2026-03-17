#include "cache.hpp"

L1Cache::L1Cache(std::vector<Address> input_data) {
    batch_read(input_data);
}

uint8_t L1Cache::read(Address address) {
    
}

std::vector<uint8_t> L1Cache::batch_read(std::vector<Address> addresses) {
    for (Address address : addresses)
        read(address);
}

void L1Cache::write(uint8_t input) {

}

void L1Cache::batch_write(std::vector<uint8_t> input_data) {

}

void L1Cache::evict(int set_index) {

}
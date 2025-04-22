#include "spi.hpp"
#include <iostream>

void SPI::init() {
    std::cout << "[SPI] Initialized." << std::endl;
}

void SPI::dmaCallback() {
    std::cout << "[SPI] DMA receive complete." << std::endl;
}

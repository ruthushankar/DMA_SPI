#include "uart.hpp"
#include <iostream>

void UART::init() {
    std::cout << "[UART] Initialized." << std::endl;
}

void UART::dmaCallback() {
    std::cout << "[UART] DMA transfer complete." << std::endl;
}

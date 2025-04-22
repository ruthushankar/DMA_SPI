#include "dma.hpp"
#include <iostream>

void DMAController::init() {
    std::cout << "[DMA] Initialized." << std::endl;
}

void DMAController::transfer(const std::string& data, std::function<void()> callback) {
    std::cout << "[DMA] Transferring to UART: ";
    for (char c : data) {
        std::cout << c;
    }
    std::cout << std::endl;
    if (callback) callback();
}

void DMAController::receive(std::string& buffer, int length, std::function<void()> callback) {
    std::cout << "[DMA] Receiving dummy data from SPI..." << std::endl;
    buffer.clear();
    for (int i = 0; i < length - 1; ++i) {
        buffer += static_cast<char>('A' + (i % 26));
    }
    buffer += '\0';
    if (callback) callback();
}

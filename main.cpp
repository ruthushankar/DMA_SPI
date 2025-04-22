#include <iostream>
#include "dma.hpp"
#include "uart.hpp"
#include "spi.hpp"

int main() {
    std::cout << "System initializing..." << std::endl;

    UART uart;
    SPI spi;
    DMAController dma;

    uart.init();
    spi.init();
    dma.init();

    std::string txData = "Hello via UART using DMA!";
    std::string spiRxBuffer;

    dma.transfer(txData, [&uart]() {
        uart.dmaCallback();
    });

    dma.receive(spiRxBuffer, 32, [&spi]() {
        spi.dmaCallback();
    });

    std::cout << "SPI Received: " << spiRxBuffer << std::endl;
    std::cout << "Main loop complete." << std::endl;
    return 0;
}

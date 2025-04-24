#include <iostream>
#include <string>
#include <functional>
#include <thread>
#include <chrono>
// 

// Common transfer sizes
constexpr int CHAR_LEN = 5;
constexpr int INT_LEN  = 3;
// UART class simulating a terminal output
class UART {
public:
    void init() {
        std::cout << "[UART] Initialized.\n";
    }
    void dmaCallback() {
        std::cout << "[UART] DMA transfer complete.\n";
    }
};

// SPI class simulating a sensor sending back data
class SPI {
public:
    void init() {
        std::cout << "[SPI] Initialized.\n";
    }
    void dmaCallback() {
        std::cout << "[SPI] DMA receive complete.\n";
    }
};

// DMA controller simulating memory-peripheral interaction
class DMAController {
public:
    void init() {
        std::cout << "[DMA] Initialized.\n";
    }

    // Transfer char data to UART
    void transferChar(char* data, int len, std::function<void()> callback) {
        std::cout << "[DMA] Transferring char data to UART: ";
        for (int i = 0; i < len; i++) {
            std::cout << data[i];
        }
        std::cout << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // simulate DMA delay
        if (callback) callback();
    }

    // Transfer int data to UART
    void transferInt(int* data, int len, std::function<void()> callback) {
        std::cout << "[DMA] Transferring int data to UART: ";
        for (int i = 0; i < len; i++) {
            std::cout << data[i] << " ";
        }
        std::cout << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // simulate DMA delay
        if (callback) callback();
    }

    // Receive char data from SPI
    void receiveChar(char* buffer, int length, std::function<void()> callback) {
        std::cout << "[DMA] Receiving char data from SPI...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // simulate DMA delay
        for (int i = 0; i < length; i++) {
            buffer[i] = 'A' + i; // Dummy characters
        }
        if (callback) callback();
    }

    // Receive int data from SPI
    void receiveInt(int* buffer, int length, std::function<void()> callback) {
        std::cout << "[DMA] Receiving int data from SPI...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // simulate DMA delay
        for (int i = 0; i < length; i++) {
            buffer[i] = 100 + i; // buffer[i] gets populated from SPI->DR. DMA handles
        }
        if (callback) callback();
    }
};

int main() {
    std::cout << "System initializing (DMA)...\n";

    UART uart;
    SPI spi;
    DMAController dma;

    uart.init();
    spi.init();
    dma.init();

    // --- CHAR TRANSFER ---
    char charTxData[CHAR_LEN] = {'W', 'o', 'r', 'l', 'd'};
    char charRxBuffer[CHAR_LEN] = {};

    dma.transferChar(charTxData, CHAR_LEN, [&uart]() {
        uart.dmaCallback();
    });

    dma.receiveChar(charRxBuffer, CHAR_LEN, [&spi]() {
        spi.dmaCallback();
    });

    std::cout << "SPI Received (char): ";
    for (int i = 0; i < CHAR_LEN; ++i) {
        std::cout << charRxBuffer[i];
    }
    std::cout << "\n";

    // --- INT TRANSFER ---
    int intTxData[INT_LEN] = {10, 20, 30};
    int intRxBuffer[INT_LEN] = {};

    dma.transferInt(intTxData, INT_LEN, [&uart]() {
        uart.dmaCallback();
    });

    dma.receiveInt(intRxBuffer, INT_LEN, [&spi]() {
        spi.dmaCallback();
    });

    std::cout << "SPI Received (int): ";
    for (int i = 0; i < INT_LEN; ++i) {
        std::cout << intRxBuffer[i] << " ";
    }
    std::cout << "\n";

    return 0;
}


#include <iostream>
#include <string>
#include <thread>
#include <chrono>

// Common transfer sizes
constexpr int CHAR_LEN = 5;
constexpr int INT_LEN  = 3;

//The essence is that without DMA, CPU load is high as the data is manually being copied.
// UART class
class UART {
public:
    void init() {
        std::cout << "[UART] Initialized.\n";
    }
    // CPU manually sends data one-by-one
    void sendChar(char* data, int len) {
        std::cout << "[UART] Sending char data: ";
        for (int i = 0; i < len; ++i) {
            std::cout << data[i];  // Send each char manually
            // The delay simulates the time the CPU might spend servicing each transfer.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::cout << "\n[UART] Transfer complete.\n";
    }

    void sendInt(int* data, int len) {
        std::cout << "[UART] Sending int data: ";
        for (int i = 0; i < len; ++i) {
            std::cout << data[i] << " ";  // Send each int manually
            // The delay simulates the time the CPU might spend servicing each transfer.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::cout << "\n[UART] Transfer complete.\n";
    }
};

// SPI class
class SPI {
public:
    void init() {
        std::cout << "[SPI] Initialized.\n";
    }

    // CPU manually "receives" dummy char data
    void receiveChar(char* buffer, int len) {
        std::cout << "[SPI] Receiving char data manually...\n";
        for (int i = 0; i < len; ++i) {
            buffer[i] = 'X' + i;  // Dummy sensor values
            // The delay simulates the time the CPU might spend servicing each transfer.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::cout << "[SPI] Receive complete.\n";
    }

    void receiveInt(int* buffer, int len) {
        std::cout << "[SPI] Receiving int data manually...\n";
        for (int i = 0; i < len; ++i) {
            buffer[i] = 200 + i;  // Dummy sensor values
            // The delay simulates the time the CPU might spend servicing each transfer.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
 
        std::cout << "[SPI] Receive complete.\n";
    }
};

int main() {
    std::cout << "System initializing (No DMA)...\n";

    UART uart;
    SPI spi;

    uart.init();
    spi.init();

    // --- CHAR TRANSFER ---
    char charTxData[CHAR_LEN] = {'W', 'o', 'r', 'l', 'd'};
    char charRxBuffer[CHAR_LEN] = {};

    auto startChar = std::chrono::steady_clock::now();
    uart.sendChar(charTxData, CHAR_LEN);
    spi.receiveChar(charRxBuffer, CHAR_LEN);
    auto endChar = std::chrono::steady_clock::now();
    auto charDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endChar - startChar).count();

    std::cout << "SPI Received (char): ";
    for (int i = 0; i < CHAR_LEN; ++i) {
        std::cout << charRxBuffer[i];
    }
    std::cout <<"Duration taken is "<< charDuration<< " ms \n";

    // --- INT TRANSFER ---

    int intTxData[INT_LEN] = {10, 20, 30};
    int intRxBuffer[INT_LEN] = {};

    auto startInt = std::chrono::steady_clock::now();
    uart.sendInt(intTxData, INT_LEN);
    spi.receiveInt(intRxBuffer, INT_LEN);
    auto endInt = std::chrono::steady_clock::now();
    auto intDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endInt - startInt).count();

    std::cout << "SPI Received (int): ";
    for (int i = 0; i < INT_LEN; ++i) {
        std::cout << intRxBuffer[i] << " ";
    }
    std::cout <<"Duration taken is "<< intDuration<< " ms \n";

    return 0;
}

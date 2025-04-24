# Peripheral Sim in C++: DMAC, UART, and SPI

This project simulates DMA-controlled UART and SPI communication using modern C++.

## Build

```bash
make
```

## Run

```bash
./main
```

Hi, Welcome to my project. I am trying to emulate UART, SPI protocols and incorporating DMA as well. 


// notes

//??Why are we not using namespace - prevents name clashes

using namespace std;

class MyLogger {
    string file;  // is this std::string or something else? Later, if you or a library defines a custom string, you now have an ambiguous identifier.
};

Things I have done

In main.cpp, I've implemented DMA transfer from and to the memory and callback functions.

In cpu.cpp, I've emulated when data transfer is done without a DMA controller.
 - Codewise, it's kinda chunky because bot SPI and UART need to have transfer and recieve methods defined.
 - On a much lower level, there is lot of pressure being put on the CPU wasting cycles on the transfer part while the task/callback function is waiting for the transfer to be complete.


Code breakdown:
main.cpp

constexpr int CHAR_LEN = 5;
constexpr int INT_LEN  = 3;
// ?? Difference between const and constexpr
We are trying to send a 5 ledder word over UART and receive 3 integers over SPI from a sensor. (example, temperature, humidity, pressure)

class UART
init() - Initialise params( baud rate, word length, parity). This happens in UART registers.

//?? Be more specific about UART registers
USARTx_BRR – Baud rate register

USARTx_CR1 – Control register 1 (enables UART, sets word length)

USARTx_CR2/CR3 – Additional config (parity, interrupts)

USARTx_DR – Data register (used for transmit/receive)

dmaCallback() - Triggered when DMA controller finishes copying data from the memory.

//?? Help me understand how exactly this is non- blocking.
- Once DMA starts, the CPU is free to perform other tasks.

DMA automatically moves data in the background.

When done, it raises a DMA complete interrupt (IRQ).

ISR for this interrupt then triggers dmaCallback().


class SPI
init() - sets master/slave, clock, data frame size

dmaCallback() - triggered once DMA fills the Rx buffer

class DMAController
init() - gets initialised
//?? didnt understand this part

void transferChar(char* data, int len, std::function<void()> callback)
// ?? does void() mean its returning void??

- Memory -> UART -> serial monitor.
- Source : RAM address of charTxData 
- Destination : UARTTx->DR. Data register.//?? isnt UARTTx->DR same as charTxData
- After transfer, DMA might fire IRQ -> NVIC calls ISR -> ISR Calls dmaCallback()


void receiveChar(char* buffer, int length, stf::function<void()>callback)
- Peripheral->memory
- DMA channel is configured to write incoming SPI data into charRxBuffer

//?? is charRxBuffer memory mapped? Like #DEFINE charRxBuffer *(volatile uint8_t*)0x40004000

- Sensor starts sending 'A' 'B' 'C' 'D' 'E'.
//?? Does IRQ get called after every byte aka every char?

void transferInt(int* data, int len, std::function<void()> callback)
- larger payload as compared to char defined earlier

┌──────────┐         ┌────────────┐        ┌────────────┐
│ SPI Slave│──MISO──▶│ SPIx->DR   │──DMA──▶│ buffer[i]  │
└──────────┘         └────────────┘        └────────────┘
                       (Memory-mapped)       (In RAM)


Step 1: Data resides in RAM buffer
----------------------------------

          RAM (your array)
        ┌────────────────────┐
        │ TxBuffer[0] = 0xA5 │
        │ TxBuffer[1] = 0xB2 │
        │ TxBuffer[2] = 0xC3 │
        └────────────────────┘
                 │
                 │ (Step 2: DMA reads this)
                 ▼

Step 2: DMA reads from RAM and writes to SPIx->DR
-------------------------------------------------

        ┌────────────────────┐
        │     DMA Engine     │
        └────────────────────┘
                 │
                 ▼
        ┌────────────────────┐
        │   SPIx->DR (Tx)    │
        └────────────────────┘
                 │
                 ▼

Step 3: SPI shifts out data bit by bit on MOSI line
----------------------------------------------------

        ┌─────────────┐         MOSI        ┌──────────────┐
        │   MCU SPI   │ ──────────────────▶ │ SPI Receiver │
        │   (Master)  │                    │  (e.g., DAC)  │
        └─────────────┘                    └──────────────┘


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

Why are we not using namespace

using namespace std;

class MyLogger {
    string file;  // is this std::string or something else? Later, if you or a library defines a custom string, you now have an ambiguous identifier.
};

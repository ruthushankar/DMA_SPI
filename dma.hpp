#ifndef DMA_HPP
#define DMA_HPP

#include <string>
#include <functional>

class DMAController {
public:
    void init();
    void transfer(const std::string& data, std::function<void()> callback);
    void receive(std::string& buffer, int length, std::function<void()> callback);
};

#endif

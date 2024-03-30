#include "itch_handler.h"
#include <sys/time.h>
using namespace TradingEngine::ITCH;

int main()
{
    std::ifstream file("../data/itch/sample.itch", std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }
    file.seekg(0, std::ios::end);
    std::streamsize file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Allocate memory for the buffer
    void *buffer = malloc(file_size);
    if (!file.read(static_cast<char *>(buffer), file_size))
    {
        std::cerr << "Failed to read file!" << std::endl;
        free(buffer); // Free allocated memory
        return 1;
    }
    file.close();
    // std::cout << file_size << std::endl;
    uint8_t *data = (uint8_t *)buffer;
    // std::cout << (*data) << std::endl;
    struct timeval start, end;
    gettimeofday(&start, NULL);
    std::ios_base::sync_with_stdio(false);
    ITCH_Handler itch_handler;
    itch_handler.process(buffer, file_size);
    gettimeofday(&end, NULL);

    double time_taken;

    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec -
                                start.tv_usec)) *
                 1e-6;

    std::cout << "Time taken by program is : " << std::fixed
              << time_taken << std::setprecision(6);
    std::cout << " sec" << std::endl;
    std::cout << "Total number of messages processed: " << itch_handler.get_total_messages() << std::endl;
    std::cout << "Throughput: " << itch_handler.get_total_messages() / time_taken << " msgs/s" << std::endl;
    std::cout << "Latency: " << time_taken / itch_handler.get_total_messages() * 1e9 << " ns" << std::endl;

    // Don't forget to free the allocated memory
    free(buffer);
    return 0;
}

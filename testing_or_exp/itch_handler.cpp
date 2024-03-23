#include <iostream>
#include <fstream>

struct ITCHMessage {
    char messageType;
};

void process_itch_messages(std::vector<char> &buffer) {
    size_t offset = 0;
    while(offset < buffer.size()) {
        ITCHMessage message;
        message.messageType = buffer[offset];
        offset += sizeof(char);
        switch (message.messageType)
        {
        case 'A':
            std::cout << "A" << std::endl;
            break;
        case 'E':
            std::cout << "E" << std::endl;
            break;
        default:
            std::cerr << "Unknown message type: " << message.messageType << std::endl;
            break;
        }
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::ifstream file("../data/itch/sample.itch", std::ios::binary);
    if(!file.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }
    file.seekg(0, std::ios::end);
    std::streamsize file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(file_size);
    if(!file.read(buffer.data(), file_size)) {
        std::cerr << "Failed to read file!" << std::endl;
        return 1;
    }
    file.close();
    process_itch_messages(buffer);
    return 0;
}
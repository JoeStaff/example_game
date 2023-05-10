
#include <fstream>
#include <map>
#include <string>
#include "../logger.h"

namespace engine {
namespace file {

    std::map<std::string, std::string> loadKeyValuePairs(const std::string& filename) {
        std::map<std::string, std::string> keyValuePairs;

        std::ifstream file(filename);
        if (!file.is_open()) {
            return keyValuePairs;
        }

        std::string line;
        while (std::getline(file, line)) {
            // Split the line into key and value
            size_t delimiterPos = line.find('=');
            if (delimiterPos != std::string::npos) {
                std::string key = line.substr(0, delimiterPos);
                std::string value = line.substr(delimiterPos + 1);
                keyValuePairs[key] = value;
            }
        }

        file.close();

        return keyValuePairs;
    }
    void saveKeyValuePairs(const std::string& filename, const std::map<std::string, std::string>& keyValuePairs) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        // Create the file if it doesn't exist
        file.open(filename, std::ios::out);
        if(!file.is_open())
            log(ERROR) << "Error opening file: " << filename;
    }

    for (const auto& pair : keyValuePairs) {
        file << pair.first << "=" << pair.second << std::endl;
    }

    file.close();
}

}
}
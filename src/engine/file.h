#ifndef ENGINE_FILE_H
#define ENGINE_FILE_H
#include <map>
namespace engine{
namespace file{
    std::map<std::string, std::string> loadKeyValuePairs(const std::string& filename);
    void saveKeyValuePairs(const std::string& filename, const std::map<std::string, std::string>& keyValuePairs);
}
}
#endif
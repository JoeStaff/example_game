#include <chrono>
#include "time.h"
namespace engine{
namespace time{

    long long getCurrentTimeMillis() {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    }
    
}
}
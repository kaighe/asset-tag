#include <stdint.h>
#include <string>
#define GM02SP_BUFFER_SIZE 1024
#define GM02SP_ENABLE_LOG

struct ModemFlags{
    bool ready = false;
    bool line_error = false;
};

class Modem{
    char buffer[GM02SP_BUFFER_SIZE + 1];
    size_t buffer_length = 0;
    ModemFlags flags;
    #ifdef GM02SP_ENABLE_LOG
    void (*log_func)(const char*) = nullptr;
    #endif
    
    public:
    void set_logger(void (*log_func)(const char*));
    void log(const char* message);
    void encode(int byte);
    void parse_buffer(char* buffer);
    bool is_ready();
};
#ifndef GM02SP_H
#define GM02SP_H

#define GM02SP_BUFFER_SIZE 1024
#define GM02SP_ENABLE_LOG

#ifdef ARDUINO
#include <Arduino.h>
#else
class HardwareSerial{
    public:
    int available();
    int read();
    size_t write(int n);
    void println(const char*);
};
#endif

namespace GM02SP{

    class Modem{
        HardwareSerial* serial;
        void (*reset_callback)();
        void (*logger_callback)(const char*);
        unsigned long (*timer_callback)();
        char buffer[GM02SP_BUFFER_SIZE + 1];
        
        public:
        Modem(HardwareSerial* serial, void (*reset_callback)());
        
        bool read_line();
        bool read_line(int timeout);

        bool reset(int timeout);
        bool reset();

        void set_timer(unsigned long (*timer)());
        unsigned long timer();
        void set_logger(void (*logger)(const char*));
        void log(const char* message);
    };

}

#endif
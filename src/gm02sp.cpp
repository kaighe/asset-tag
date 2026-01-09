#include "gm02sp.h"

using namespace GM02SP;

bool string_cmp(const char* a, const char* b){
    size_t c = 0;
    while(true){
        if(a[c] == '\0' && b[c] == '\0') return true;

        if(a[c] == b[c]){
            c++;
        }else{
            return false;
        }
    }
}

unsigned long default_timer(){
    return 0;
}

Modem::Modem(HardwareSerial* serial, void (*reset_callback)()){
    this->serial = serial;
    this->reset_callback = reset_callback;
}

void Modem::set_timer(unsigned long (*timer_callback)()){
    this->timer_callback = timer_callback;
}
unsigned long Modem::timer(){
    if(timer_callback != nullptr){
        return timer_callback();
    }
}

void Modem::set_logger(void (*logger_callback)(const char*)){
    #ifdef GM02SP_ENABLE_LOG
    this->logger_callback = logger_callback;
    #endif
}
void Modem::log(const char* message){
    #ifdef GM02SP_ENABLE_LOG
    if(logger_callback != nullptr){
        logger_callback(message);
    }
    #endif
}

bool Modem::reset(int timeout){
    reset_callback();

    unsigned long start_time = timer();
    while(true){
        if(timeout >= 0 && timer() - start_time > timeout) return false;
        if(read_line(10000)){
            if(string_cmp(buffer, "+SYSSTART")){
                return true;
            }
        }else{
            return false;
        }
    }
}
bool Modem::reset(){
    Modem::reset(-1);
}

bool Modem::read_line(int timeout){
    unsigned long start_time = timer();

    size_t counter = 0;
    while(true){
        while(!serial->available()){
            if(timeout >= 0 && timer() - start_time > timeout) return false;
        }

        char c = serial->read();
        if(c == '\r' || c == '\n' || c == '\0'){
            if(counter > 0){
                buffer[counter] = '\0';
                log(buffer);
                return true;
            }
            counter = 0;
        }else if(counter < GM02SP_BUFFER_SIZE){
            buffer[counter] = c;
            counter++;
        }else{
            return false;
        }
    }
}
bool Modem::read_line(){
    return read_line(-1);
}
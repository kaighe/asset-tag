#include "gm02sp.h"

void Modem::set_logger(void (*func)(const char*)){
    this->log_func = func;
}

void Modem::log(const char* message){
    #ifdef GM02SP_ENABLE_LOG
    if(log_func != nullptr) log_func(message);
    #endif
}

void Modem::encode(int byte){
    if(byte == '\n' || byte == '\r' || byte == '\0'){
        buffer[buffer_length] = '\0';

        if(!flags.line_error && buffer_length > 0){
            parse_buffer(buffer);
        }

        buffer_length = 0;
        flags.line_error = false;
    }else if(buffer_length < GM02SP_BUFFER_SIZE){
        buffer[buffer_length] = byte;
        buffer_length++;
    }else{
        buffer_length = 0;
        flags.line_error = true;
    }
}

void Modem::parse_buffer(char* buffer){
    std::string message(buffer);

    log(buffer);
    if(message == "+SYSSTART"){
        flags.ready = true;
    }
}

bool Modem::is_ready(){
    return flags.ready;
}
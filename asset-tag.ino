#include <src/gm02sp.h>

#define MODEM_RX 14
#define MODEM_TX 48
#define MODEM_RTS 21
#define MODEM_CTS 47
#define MODEM_RESET 45
#define MODEM_BAUD 115200
#define MODEM_BUFFER 128
#define MODEM_BUFFER_THRESHOLD 122

void callback_modem_log(const char* message){
  // Serial.print(">");
  // char a = ' ';
  // size_t counter = 0;
  // while(a != '\0'){
  //   a = message[counter];
  //   counter++;
  //   Serial.print(a, HEX);
  //   Serial.print(" ");
  // }
  // Serial.println();
  Serial.print("[MODEM] ");
  Serial.println(message);
}

void callback_modem_reset(){
  pinMode(MODEM_RESET, OUTPUT);
  digitalWrite(MODEM_RESET, LOW);
  delay(1000);
  digitalWrite(MODEM_RESET, HIGH);
}

GM02SP::Modem modem(&Serial2, callback_modem_reset);

void setup() {
  Serial.begin(9600);
  while(!Serial);

  Serial.println("Initializing... ");

  Serial2.setRxBufferSize(MODEM_BUFFER * 2);
  Serial2.begin(MODEM_BAUD, SERIAL_8N1, MODEM_RX, MODEM_TX);
  Serial2.setPins(MODEM_RX, MODEM_TX, MODEM_CTS, MODEM_RTS);
  Serial2.setHwFlowCtrlMode(UART_HW_FLOWCTRL_CTS_RTS, MODEM_BUFFER_THRESHOLD);
  
  modem.set_timer(millis);
  modem.set_logger(callback_modem_log);
  if(!modem.reset(10000)){
    Serial.println("Failed to initialize modem.");
  }

  Serial.println("Done");
}

void loop() {
  modem.read_line(0);
  
  while(Serial.available() > 0) {
    Serial2.write(Serial.read());
  }
}
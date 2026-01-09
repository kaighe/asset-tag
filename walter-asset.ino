#include <SoftwareSerial.h>

#define MODEM_RX 14
#define MODEM_TX 48
#define MODEM_RTS 21
#define MODEM_CTS 47
#define MODEM_RESET 45
#define MODEM_BAUD 115200
#define MODEM_BUFFER 128
#define MODEM_BUFFER_THRESHOLD 122

HardwareSerial modem = Serial2;

void init_modem(){
  modem.setRxBufferSize(MODEM_BUFFER * 2);
  modem.begin(MODEM_BAUD, SERIAL_8N1, MODEM_RX, MODEM_TX);
  modem.setPins(MODEM_RX, MODEM_TX, MODEM_CTS, MODEM_RTS);
  modem.setHwFlowCtrlMode(UART_HW_FLOWCTRL_CTS_RTS, MODEM_BUFFER_THRESHOLD);

  pinMode(MODEM_RESET, OUTPUT);
  digitalWrite(MODEM_RESET, LOW);
  delay(1000);
  digitalWrite(MODEM_RESET, HIGH);
  delay(1000);
}

void setup() {
  Serial.begin(9600);
  while(!Serial);

  Serial.print("Initializing... ");
  init_modem();
  Serial.println("Done");
}

void loop() {
  while(Serial.available() > 0) {
    modem.write(Serial.read());
  }

  while (modem.available() > 0) {
    Serial.write(modem.read()); 
  }
}
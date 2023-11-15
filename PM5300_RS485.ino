#include <ModbusMaster.h>
#define baud 19200
#define timeout 100
#define polling 100
#define retry_count 10
ModbusMaster node;

#define ID_meter 1
#define Total_of_Reg 2

#define Reg_LN 3109  //  1
#define Reg_FQ 3027  //  2

uint16_t Reg_addr[2] = {
  Reg_LN,
  Reg_FQ,
};
float DATA_METER[Total_of_Reg];

float HexTofloat(uint32_t x) {
  return (*(float*)&x);
}

uint32_t FloatTohex(float x) {
  return (*(uint32_t*)&x);
}
float Read_Meter_float(char addr, uint16_t REG) {
  float i = 0;
  uint8_t result, j;

  uint16_t data[2];
  uint32_t value = 0;
  node.begin(ID_meter, Serial2);
  // node.preTransmission(preTransmission);
  // node.postTransmission(postTransmission);

  result = node.readHoldingRegisters(REG, 2);  ///< Modbus function 0x03 Read Holding Registers
  delay(500);
  if (result == node.ku8MBSuccess) {
    for (j = 0; j < 2; j++) {
      data[j] = (node.getResponseBuffer(j));
    }
    //uint16_t x = data[1]; // para o valor da frequência tenho que ler algo em torno de 426F xxxx;
    //uint16_t y = data[0];

    Serial.print(data[1], HEX);
    Serial.println(data[0], HEX);

    value = data[0];
    value = value << 16;
    value = value + data[1];
    i = HexTofloat(value);
    //Serial.println("Connect modbus Ok.");
    return i;
  } else {
    Serial.print("Connect modbus fail. REG >>> ");
    Serial.println(REG);  // Debug
    delay(1000);
    return 0;
  }
}
void GET_METER() {
  // Update read all data
  delay(1000);
  for (char i = 0; i < Total_of_Reg; i++) {
    DATA_METER[i] = Read_Meter_float(ID_meter, Reg_addr[i]);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(baud);
  Serial2.begin(baud, SERIAL_8E1);
}

void loop() {
  // put your main code here, to run repeatedly:
  GET_METER();
  Serial.println();
  Serial.print("frequency = ");
  Serial.print(DATA_METER[0], 3);
  Serial.println(" Hz");
  Serial.print("Vavg =  ");
  Serial.print(DATA_METER[1], 3);
  Serial.println(" V");
  delay(3000);
}

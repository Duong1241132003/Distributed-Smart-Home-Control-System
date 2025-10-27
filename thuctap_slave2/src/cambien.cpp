#include "cambien.h"
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
int water_level;
int turbidity_val;
int pH_val;
int temperature;
void Init(void)
{
  pinMode(turbidity_sensor_pin, INPUT);
  pinMode(temperature_sensor_pin, INPUT);
  pinMode(water_pumper, OUTPUT);
  sensors.begin();
}

void get_Val(void)
{
    // water_level     = get_waterLV();
    turbidity_val   = map(get_NTU(),0,1000,1000,0);
    temperature     = get_tem();        // gây lỗi trong truyền nhận uart mềm
}

int get_waterLV(void)
{
  return measurement_point + water_sensor_length * (float)analogRead(water_sensor_pin) / resolution;
}

int get_NTU(void)
{
  return NTU_resolution * (float)analogRead(turbidity_sensor_pin) / resolution;
}

int get_tem(void)
{
  sensors.requestTemperatures(); 
  return sensors.getTempCByIndex(0);
}


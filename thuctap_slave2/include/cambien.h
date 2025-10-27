#include <OneWire.h>
#include <DallasTemperature.h>


#define Vref                3.3
#define resolution          1023
#define turbidity_sensor_pin    A0
#define water_sensor_pin        D3
#define temperature_sensor_pin  D2
#define water_pumper            D4
#define NTU_resolution              1000.0                    //for turbidity sensor
#define measurement_point           10.0                      //The height begins to measure
#define conversion_coefficient      (Vref/resolution)         //for MCU
#define water_sensor_length         10.0                      //the length of water sensor
#define lower_pressure_coefficient  3.0/2.0                     //for lower_pressure_circut
#define pH_conversion_coefficient   14.0/5.0                    //coefficient convert pressure to pH
extern int water_level;
extern int turbidity_val;
extern int temperature;

// Data wire is connected 
#define ONE_WIRE_BUS        temperature_sensor_pin

void Init(void);
void get_Val(void);
int get_NTU(void);
int get_tem(void);
int get_pH(void);
int get_waterLV(void);
//https://github.com/re3lex/SDM220-Modbus-RTU-ESP8266/blob/master/EastronModbusESP8266.ino

// * //SDM 230 registers
//#define SDM230_VOLTAGE                      0x0000                              //V
//#define SDM230_CURRENT                      0x0006                              //A
//#define SDM230_POWER                        0x000C                              //W
//#define SDM230_ACTIVE_APPARENT_POWER        0x0012                              //VA
//#define SDM230_REACTIVE_APPARENT_POWER      0x0018                              //VAR
//#define SDM230_POWER_FACTOR                 0x001E                              //
//#define SDM230_PHASE_ANGLE                  0x0024                              //DEGREE
//#define SDM230_FREQUENCY                    0x0046                              //Hz
//#define SDM230_IMPORT_ACTIVE_ENERGY         0x0048                              //Wh
//#define SDM230_EXPORT_ACTIVE_ENERGY         0x004A                              //Wh
//#define SDM230_IMPORT_REACTIVE_ENERGY       0x004C                              //VARh
//#define SDM230_EXPORT_REACTIVE_ENERGY       0x004E                              //VARh
//#define SDM230_TOTAL_SYSTEM_POWER_DEMAND    0x0054                              //W
//#define SDM230_MAXIMUM_SYSTEM_POWER_DEMAND  0x0056                              //W
//#define SDM230_CURRENT_POSITIVE_POWER_DEMAND 0x0058                             //W
//#define SDM230_MAXIMUM_POSITIVE_POWER_DEMAND 0x005A                             //W
//#define SDM230_CURRENT_REVERSE_POWER_DEMAND 0x005C                              //W
//#define SDM230_MAXIMUM_REVERSE_POWER_DEMAND 0x005E                              //W
//#define SDM230_CURRENT_DEMAND               0x0102                              //Amps
//#define SDM230_MAXIMUM_CURRENT_DEMAND       0x0108                              //Amps
//#define SDM230_TOTAL_ACTIVE_ENERGY          0x0156                              //kwh
//#define SDM230_TOTAL_REACTIVE_ENERGY        0x0158                              //kvarh
//#define SDM230_CURRENT_RESETTABLE_TOTAL_ACTIVE_ENERGY   0x0180                  //Wh
//#define SDM230_CURRENT_RESETTABLE_TOTAL_REACTIVE_ENERGY 0x0182                  //VARh


// definimos el lugar, sun y time de acuerdo con sunpos.h
// cLocation lugar;
// cSunCoordinates sun;
// cTime fecha_hora_PSA;

char thingspeak_api_key[20]="BTWBFPS831JV3KZX";         
long      time_thingspeak = 30000;
char thingspeak_api_key2[20]="1I7AFQQ708LMPOIL";         
long      time_thingspeak2 = 30000;

//boolean pzem_lect_valid;
 spa_data spa;
//************************************************//
//*******************SDM230*********************//
//************************************************//

typedef struct {
  int code;
  char* label;
  char* uom;
} Code;

//https://github.com/reaper7/SDM_Energy_Meter/blob/master/SDM.h
//para ver los códigos del SDM230

static const Code codes[] = {
  {0x00, "Voltage", "V"},
  {0x06, "Current", "A"},
  {0x0C, "Active Power", "W"},
  {0x12, "Apparent Power", "VA"},
  {0x18, "Reactive Power", "VAr"},
  {0x1e, "Power Factor", ""},
  {0x24, "Phase Angle", "º"},
  {0x46, "Frequency", "Hz"},
  {0x48, "Import active energy", "kwh"},
  {0x4a, "Export active energy", "kwh"},
  {0x4c, "Import reactive energy", "kvarh"},
  {0x4e, "Export reactive energy", "kvarh"},
  {0x54, "TOTAL_SYSTEM_POWER_DEMAND", "W"},
  {0x56, "MAXIMUM_SYSTEM_POWER_DEMAND", "W"},
  {0x58, "CURRENT_POSITIVE_POWER_DEMAND", "W"},
  {0x5a, "MAXIMUM_POSITIVE_POWER_DEMAND", "W"},
  {0x5c, "CURRENT_REVERSE_POWER_DEMAND", "W"},
  {0x5e, "MAXIMUM_REVERSE_POWER_DEMAND", "W"},
  {0x102, "CURRENT_DEMAND", "A"},
  {0x108, "MAXIMUM_CURRENT_DEMAND", "A"},
  {0x156, "TOTAL_ACTIVE_ENERGY", "kWh"},
  {0x158, "TOTAL_REACTIVE_ENERGY", "kVAr"},
  {0x180, "CURRENT_RESETTABLE_TOTAL_ACTIVE_ENERGY", "Wh"},
  {0x182, "CURRENT_RESETTABLE_TOTAL_REACTIVE_ENERGY", "VARh"},
  
};

#define CODES_SIZE sizeof codes / sizeof codes[0]

enum {
  IDLE_STATE = 0,
  DO_REQUEST_STATE = 1,
  RECEIVING_STATE = 2,
  DATA_READY_STATE = 3
};
float data[24];

#define MAX485_DE      D1
#define MAX485_RE_NEG  D2


unsigned long u32wait;
uint8_t u8RequestState;
bool state = true;
int currentParamIdx = 0;


#define RDELAY 1000

struct datos_SDM230 {
   float Voltage;
   float Current;
   float Active_Power;
   float Apparent_Power;
   float Reactive_Power;
   float Power_Factor;
   float Phase_Angle;
   float Frequency;
   float Import_active_energy;
   float Export_active_energy;
   float Import_reactive_energy;
   float Export_reactive_energy;
   float TOTAL_SYSTEM_POWER_DEMAND;
   float MAXIMUM_SYSTEM_POWER_DEMAND;
   float CURRENT_POSITIVE_POWER_DEMAND;
   float MAXIMUM_POSITIVE_POWER_DEMAND;
   float CURRENT_REVERSE_POWER_DEMAND;
   float MAXIMUM_REVERSE_POWER_DEMAND;
   float CURRENT_DEMAND;
   float MAXIMUM_CURRENT_DEMAND;
   float TOTAL_ACTIVE_ENERGY;
   float TOTAL_REACTIVE_ENERGY;
   float CURRENT_RESETTABLE_TOTAL_ACTIVE_ENERGY;
   float CURRENT_RESETTABLE_TOTAL_REACTIVE_ENERGY;
} SDM230;

struct datos_pzem016 {
      double voltage;
      double current ;
      double power;
      double power2;
      double energy ;
      double frequency;
      double power_factor;
} pzem;

struct datos_tasmotas{
  float Total;
  float Today;;
  float Yesterday;
  float Power;
  float ApparentPower;
  float ReactivePower;
  float Factor;
  float Voltage;
  float Current;
  int   estado;
 
} inversor, aire,bombeo;


String payload_inversor;
String payload_aire;
String payload_bombeo;
String payload_pzem;
 
      

boolean     aire_estado_anterior;
boolean     inversor_estado_anterior;
boolean     bombeo_estado_anterior;
boolean   estados[6]; //estados de los relés //se añaden los D0 y D4 calentador y ventilador
int       i,j,k;
long      time_old[4];



struct StoreStruct2 {
  char          version[4];
  int           EEPROM_chk;        // EEPROM Check
  char          DevName[30];       // Device name, i.e Sistema 3 - Aireador
  float         Latitud;
  float         Longitud;
  int           TimeZone;
  int           IP_1;
  int           IP_2;
  int           IP_3;
  int           IP_4;
  char          pwd[8];
  int           sunrise;
  int           sunset;
  float         temp_max;
  float         temp_min;
  int           temp_mode;
  char          ip_inversor[16];
  char          ip_aire[16];
  char          ip_bombeo[16];
  char          ip_pzem[16];
  long          time_thingspeak;
  char          thingspeak_api_key[20];
  long          time_thingspeak2;
  char          thingspeak_api_key2[20];
  int           control_aire; 
  int           sendthing1; 
  int           sendthing2; 
} conf ;



unsigned long lastMillis = 0;

// Variables
//----------
#define      ErrMsg            "<font color=\"red\"> < < Error !</font>"
boolean      ResetWiFi       = false;
long         timeNow         = 0;
long         timeOld         = 300000;
long         timeOld2         = 0;
long         timeOld3         = 0;
long         timeOld4         = 0;
long         timeOld5         = 0;
long         timeOld6         = 0;

boolean      TimeOk          = false;
boolean      NTPtimeOk       = false;
String       request         = "";
byte         Page            = 1;
boolean      TimeCheck       = false;
int          NewHH           = 0;
int          NewMM           = 0;
int          Newdd           = 0;
int          Newmm           = 0;
int          Newyy           = 0;
int          PgmNr           = 0;
int          LastHH          = 0;
int          LastMM          = 0;
int          Lastdd          = 0;
int          Lastmm          = 0;
int          Lastyy          = 0;
byte         old_sec         = 0;
long         old_ms          = 0;
boolean      PgmPrev         = false;
boolean      PgmNext         = false;
boolean      PgmSave         = false;



 
// Defines
#define      DefaultName       "SDM230"  // Default device name
#define      NTPfastReq        10                 // NTP time request in seconds when  time not set
#define      NTPslowReq        3600               // NTP time request in seconds after time is  set
#define      Version           "1.00"             // Firmware version
String Argument_Name;

// NTP Server details
//-------------------
IPAddress timeServer(129, 6, 15, 28);              // time.nist.gov NTP server
WiFiUDP Udp;
unsigned int localPort       = 3232;               // Local port to listen for UDP packets
bool shouldSaveConfig = false;

ESP8266WebServer server ( 80 );

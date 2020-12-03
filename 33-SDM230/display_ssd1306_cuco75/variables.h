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

//boolean pzem_lect_valid;
String hora;
String Fecha;
boolean lect_valid;
  
 spa_data spa;
//************************************************//
//*******************SDM230*********************//
//************************************************//
// ************  Modbus RS485 settings ************
const int rxPin = D5;//12;
const int txPin = D6;//13;
const int derePin = D1;//4;
const long baud = 4800;


      float Active_Power;
      float PowerFactor;
      float ImpActEnergy;
      float ExpActEnergy;
      float TotSysPowDemand;
      float TotActEnergy;
      float TotRctEnergy;
      float PhaseAngle;
      

struct datos_sdm230 {
      double Voltage;
      double Current ;
      double Active_Power;
      double Freq;
      double PowerFactor;
      double PhaseAngle ;
      double ActApparentPwr;
      double RctApparentPwr;
      double ImpActEnergy;
      double ExpActEnergy;
      double TotRctEnergy;
      double TotSysPowDemand;
      double MaxSysPowDemand;
      double CurPosPowDemand;
      double MaxPosPowDemand;
      double CurRevPowDemand;
      double MaxRevPowDemand;
      double CurDemand;
      double MaxCurDemand;
      double CurResTotActEn;
      double CurResTotReactEn;
} sdm230;

//{"Voltage":"235.53 V","Current":"7.04 A","Active_Power":"-1549.42 A","Freq":"50.00 Hz",
//"PowerFactor":"-0.93 ","PhaseAngle":"202.74 º","ActApparentPwr":"1687.03 W",
//"RctApparentPwr":"-511.13 W","ImpActEnergy":"23.35 kWh","ExpActEnergy":"97.06 kWh",
//"ImpRctEnergy":"0.00 kVArh","ExpRctEnergy":"54.73 kVArh","TotActEnergy":"120.40 kWh",
//"TotRctEnergy":"54.73 kVArh","TotSysPowDemand":"760.04 W","MaxSysPowDemand":"-2372.91 W",
//"CurPosPowDemand":"15.81 W","MaxPosPowDemand":"1640.80 W","CurRevPowDemand":"744.23 W",
//"MaxRevPowDemand":"2372.91 W","CurDemand":"3.83 A","MaxCurDemand":"10.18 A","CurResTotActEn":"120.40 W",
//"CurResTotReactEn":"54.73 VAr"}

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
String payload_sdm230;
 
      

boolean     aire_estado_anterior;
boolean     inversor_estado_anterior;
boolean     bombeo_estado_anterior;
boolean     estados[6]; //estados de los relés //se añaden los D0 y D4 calentador y ventilador
int         i,j,k;
long        time_old[4];



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
  char          ip_sdm230[16];
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
long         timeOld7         = 0;


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
#define      DefaultName       "Display_Cuco75"  // Default device name
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

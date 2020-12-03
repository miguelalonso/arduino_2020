
// definimos el lugar, sun y time de acuerdo con sunpos.h
// cLocation lugar;
// cSunCoordinates sun;
// cTime fecha_hora_PSA;

char thingspeak_api_key[20]="1J8HKOKXIW0ZBHVR";
long      time_thinkspeak = 30000;

char ip_emoncms[16] = "192.168.8.100";
char url_base[50] = "http://192.168.8.100/emoncms";
char node[20]="Sistema_2_PH";
char apikey[50]="0aff5c530d2da22a8c8cdc1a3d56aeee";
long      time_emoncms_ant = 32000;
String Hora;
String Fecha;
boolean pactivo_ph=0;
    

 spa_data spa;
 
  //D1 y D2 con el ADS1115
  
const uint8_t GPIOPIN[4] =  {D0,D3,D4,D8};  //   D0 es el el estado de la puerta, (abierta o cerrada)

#define      Relay_D3        D3   // (D6) 
#define      Relay_D4        D4   // (D7) 
#define      Relay_D8        D8   // (D8) 

#define      Relay_D0_EstadoPuerta          D0     
    

#define      Input_D6_puerta_alta         D6     
#define      Input_D7_puerta_baja         D7    
#define SERVO_PIN D5
#define FULLY_OPEN_SENSOR_PIN D6
#define FULLY_CLOSE_SENSOR_PIN D7

//float     t = 0 ;
//float     h = 0 ;
//float     pa = 0;
boolean   estados[4]; //estados de los relés //se añaden los D0 y D4 
int       Num_reles=4;
int       i,j,k;
long      time_old[4];

struct StoreStruct {
  int           Mode;                       
  boolean       Estado_rele;
  char          ReleName[30];     
  int           ON_Time[7];     //tiempo de cada programa, puede ser para ON u OFF en función de "onoff"
  int           OFF_Time[7];    //ya no se utiliza, reserva
  boolean       ON_Days[7][8];
  char          progName[7][30];
  int           sun_set[7];       //0:timer, 1: sunrise, 2:sunset
  int           sign_delay[7];    //0 es +; 1 es -
  int           onoff[7];         // en caso de sunrise o sunset pone en on(1) o en off(0)
  int           delay_time[7];    // variable de tiempo de retraso/adelanto para sunrise y sunset.
  int           aleatorio_Time[7]; // variable de tiempo aleatorio.
  int           Tiempo_procesado[7]; //tiempos aleatorios procesados para control
 } timers[4] ;

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
  long          time_thinkspeak;
  char          thingspeak_api_key[20];
  char          pwd[8];
  int           sunrise;
  int           sunset;
  float         PH_max;
  float         PH_min;
  int           PH_mode;
  int           Relleno_mode;
  float         a; //hora inicio ciclado
  float         b; //hora fin ciclado
  float         c; //angulo cenital fin del ciclado, p.e 95
  char          ip_emoncms[16] ;
  char          url_base[50] ;
  char          node[20];
  char          apikey[50];
  long          time_emoncms;
} conf ;


boolean ciclo_actual=1;
unsigned long t_abierta, t_cerrada;
unsigned long t_inicio_abierta,t_inicio_cerrada;

unsigned long lastMillis = 0;

// Variables
//----------
boolean      ResetWiFi       = false;
long         timeNow         = 0;
long         timeOld         = 300000;
long         timeOld2         = 0;
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

int          PgmNrG           = 0;
int          OnHour          = 0;
int          OnMinute        = 0;
int          OffHour         = 0;
int          OffMinute       = 0;
int          ds_anterior      =-5;

 
// Defines
#define      DefaultName       "PuertaGallienroWeb"  // Default device name
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

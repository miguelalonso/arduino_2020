
// definimos el lugar, sun y time de acuerdo con sunpos.h
// cLocation lugar;
// cSunCoordinates sun;
// cTime fecha_hora_PSA;

char thingspeak_api_key[20]="1J8HKOKXIW0ZBHVR";
long      time_thinkspeak = 30000;

char ip_emoncms[50] = "miguelalonso.eu3.org";
char url_base[50] = "http://miguelalonso.eu3.org/emoncms";
char node[20]="Puerta_stepper";
char apikey[50]="49f90831aa8cfbf9953e7a500f8ad065";
long      time_emoncms_ant = 32000;

String Hora;
String Fecha;
boolean pactivo_ph=0;
boolean estado_anterior;    

 spa_data spa;
 


//float     t = 0 ;
//float     h = 0 ;
//float     pa = 0;
boolean   estados[1]; 
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
 } timers[1] ;

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
  int           ciclos; //numero de ciclos del stepper
  char          ip_emoncms[50] ;
  char          url_base[50] ;
  char          node[20];
  char          apikey[50];
  long          time_emoncms;
} conf ;


boolean ciclo_actual=1;
unsigned long t_abierta, t_cerrada;
unsigned long t_inicio_abierta,t_inicio_cerrada;
unsigned long t_ini;

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

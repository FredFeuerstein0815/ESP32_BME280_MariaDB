#include <Wire.h>
#include "time.h"
#include "sntp.h"
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <MySQL_Generic.h>
#include "Credentials.h"
#define MYSQL_DEBUG_PORT      Serial
#define _MYSQL_LOGLEVEL_      0
#define USING_HOST_NAME     false
RTC_DATA_ATTR int bootCount = 0;
#if USING_HOST_NAME
  char server[] = "your_account.ddns.net";
#else
  IPAddress server(192, 168, 0, 10);
#endif

const char* ntpServer1 = "de.pool.ntp.org";
const char* time_zone = "CET-1CEST,M3.5.0,M10.5.0/3";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

char default_database[] = "datenbankname";
char default_table[]    = "daten";
String db = String(default_database);
String table = String(default_table);

Adafruit_BME280 bme; // I2C

MySQL_Connection conn((Client *)&client);
MySQL_Query *query_mem;

void setup()
{
  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  MYSQL_DISPLAY1("\nStarte auf Geraet\n", ARDUINO_BOARD);
  MYSQL_DISPLAY(MYSQL_MARIADB_GENERIC_VERSION);
  Serial.print("###############################\n");
  MYSQL_DISPLAY1("Verbinde zu WLAN ", ssid);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    
  }

  MYSQL_DISPLAY1("IP:", WiFi.localIP());
  Serial.print("#########################################################################\r\n");
  MYSQL_DISPLAY3("Verbinde zu SQL Server @", server, ", Port =", server_port);
  MYSQL_DISPLAY5("SQLBenutzername =", user, ", PW =", password, ", DB =", default_database);
  Serial.print("#########################################################################\r\n");
}

void loop()
{

  bool status;
  status = bme.begin(0x76);  
  if (!status) {
    Serial.println("Keinen BME280 gefunden!");
    while (1);
  }
float temperatur = bme.readTemperature();
    char tempchar[8];
    dtostrf(temperatur, 1, 2, tempchar);
    Serial.print("\r\nTemperatur : ");
    Serial.print(tempchar);
    Serial.print(" °C\r\n");
float luftdruck = bme.readPressure()/ 100.0F;
    char druckchar[8];
    dtostrf(luftdruck, 1, 2, druckchar);
    Serial.print("Luftdruck : ");
    Serial.print(druckchar);
    Serial.print(" hPa\r\n");
float luftfeuchtigkeit = bme.readHumidity();
    char feuchtchar[8];
    dtostrf(luftfeuchtigkeit, 1, 2, feuchtchar);
    Serial.print("Luftfeuchtigkeit : ");
    Serial.print(feuchtchar);
    Serial.print(" %\r\n");
struct tm timeinfo;
configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1);
  getLocalTime(&timeinfo);
  char timeStringBuff[30];
  strftime(timeStringBuff, sizeof(timeStringBuff), "%y-%m-%d %H:%M:%S", &timeinfo);
  Serial.println(timeStringBuff);
  String datumString = String(timeStringBuff);
  String tempString = String(tempchar);
  String druckString = String(druckchar);
  String feuchtString = String(feuchtchar);
  Serial.println(tempString);
  Serial.print("\r\n");
  Serial.println(druckString);
  Serial.print("\r\n");
  Serial.println(feuchtString);
  Serial.print("\r\n");
  String alles = String("INSERT INTO ") + db + "." + table + " VALUES (" + tempString + "," + druckString + "," + feuchtString + ", " "'" + datumString + "'" ")";
  MYSQL_DISPLAY("Verbinde zur DB...");
  if (conn.connectNonBlocking(server, server_port, user, password) != RESULT_FAIL)
  {
  MySQL_Query query_mem = MySQL_Query(&conn);
  if (conn.connected())
  {
    if ( !query_mem.execute(alles.c_str()) )
    {
      MYSQL_DISPLAY("Insert Fehler");
    }
    else
    {
      MYSQL_DISPLAY("Insert erfolgreich.");
      conn.close();
      delay(1000);
    }
  }
  else
  {
    MYSQL_DISPLAY("Verbindung getrennt. Insert Fehler.");
  }
  
  } 
  else 
  {
    MYSQL_DISPLAY("\nVerbindungsfehler.");
  }

  MYSQL_DISPLAY("\r\nGehe in Tiefschlaf...");
  MYSQL_DISPLAY("=======================");
  delay(500);
  esp_sleep_enable_timer_wakeup(uint64_t 1800000000);
  esp_deep_sleep_start();
  }

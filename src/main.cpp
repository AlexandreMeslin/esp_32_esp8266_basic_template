#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ArduinoOTA.h>

#ifdef ESP32
#define LED_BUILTIN 2
#endif

void setupArduinoOTA(void);
void setupWiFiManager(void);

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

  setupWiFiManager();
  setupArduinoOTA();

  pinMode(LED_BUILTIN, OUTPUT); // led blink
}

void loop() {
  ArduinoOTA.handle();

  // led blink
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
}





/**
 * setupWiFiManager
 * Configura o WiFi Manager
 * Utiliza o endereço IP 192.168.0.1
 * SSID OTA-Template
 * Não tem senha 
 */
void setupWiFiManager(void) {
  // WiFiManager, Local initialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  // wm.resetSettings();

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result

  bool res;
  // Apagar as credenciais Wi-Fi armazenadas (descomente a linha abaixo para limpar)
  // wm.resetSettings();
  //set custom ip for portal
  wm.setAPStaticIPConfig(IPAddress(192,168,0,1), IPAddress(192,168,0,1), IPAddress(255,255,255,0));
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  // res = wm.autoConnect("OTA-Template", "password"); // password protected ap
  res = wm.autoConnect("OTA-Template"); // no password protected

  if (!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  }
  else {
    // if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
  }
}



/**
 * setupArduinoOTA
 * Configura o Arduino OTA para upload de código via Wi-Fi
 * Porta 8266 (default)
 * Host OTA-Template
 * Sem senha
 */
void setupArduinoOTA(void) {
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("OTA-Template");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type); 
  });

  ArduinoOTA.onEnd([]() { Serial.println("\nEnd"); });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) { 
    Serial.printf("Progress: %u%%\r", (progress / (total / 100))); 
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    } 
  });

  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
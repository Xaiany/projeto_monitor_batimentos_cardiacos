/***************************************************
 * Monitoramento da Saúde Materna com IoT (versão POT)
 * ESP32 + DHT22 + OLED + LEDs + Buzzer + MQTT + POT como HR
 ***************************************************/

// ----- Bibliotecas -----
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Wire.h>

// ----- Wi-Fi / MQTT -----
#define WIFI_SSID      "SSID"
#define WIFI_PASSWORD  "SENHA_WIFI"
#define MQTT_SERVER    "CLUSTER"
#define MQTT_USER      "USUARIO"
#define MQTT_PASS      "SENHA"
#define MQTT_PORT      8883
#define DEVICE_ID      "gestacao_device_01"

// ----- Pinos -----
#define DHTPIN     4
#define DHTTYPE    DHT22
#define LED_GREEN  14
#define LED_YELLOW 27
#define LED_RED    26
#define BUZZER     25

// >>> Potenciômetro: 
#define POT_PIN    34   

// ----- OLED -----
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// ----- Objetos -----
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WiFiClientSecure espClient;
static const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";
PubSubClient mqttClient(espClient);

// ----- Variáveis -----
unsigned long lastRead = 0;
#define READ_INTERVAL 3000

// ----- Limites -----
const float TEMP_LOW = 35.0;
const float TEMP_HIGH = 38.0;
const float HUMID_LOW = 30.0;
const float HUMID_HIGH = 70.0;
const int   HR_LOW = 50;
const int   HR_HIGH = 110;

// ----- Protótipos -----
void connectWiFi();
void connectMQTT();
void publishTelemetry(float t, float h, int hr, float spo2, const char* status);
String buildJSON(float t, float h, int hr, float spo2, const char* status);
void setStatusIndicators(const char* status);

// ===== SETUP =====
void setup() {
  Serial.begin(115200);
  Wire.begin(); // SDA=21, SCL=22 no ESP32

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(BUZZER, LOW);

  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Falha ao iniciar o display OLED!");
  }

  // ADC para o potenciômetro
  analogReadResolution(12);                   // 0..4095
  analogSetPinAttenuation(POT_PIN, ADC_11db); // ~0..3,3V

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Monitoramento Materno");
  display.println("Iniciando WiFi...");
  display.display();

  espClient.setCACert(root_ca);
  connectWiFi();
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  connectMQTT();

  display.println("Sistema pronto!");
  display.display();
  delay(800);
}

// ===== LOOP =====
void loop() {
  if (!mqttClient.connected()) connectMQTT();
  mqttClient.loop();

  unsigned long now = millis();
  if (now - lastRead >= READ_INTERVAL) {
    lastRead = now;

    float temp = dht.readTemperature();
    float hum  = dht.readHumidity();
    if (isnan(temp) || isnan(hum)) {
      Serial.println("Erro ao ler DHT22");
      return;
    }

    // ---- Leitura do potenciômetro (HR simulado) ----
    int potRaw = analogRead(POT_PIN);        // 0..4095
    float potV = (potRaw / 4095.0f) * 3.3f;  // Volts aproximados

    // Mapeia para uma faixa plausível de batimentos
    int hr = map(potRaw, 0, 4095, 50, 160);

    // (Opcional) suavização simples
    // static int hrPrev = hr; hr = (hrPrev*3 + hr)/4; hrPrev = hr;

    float spo2 = 0.0f; // não estamos medindo SpO2

    const char* status = "NORMAL";
    bool tempWarn = (temp < TEMP_LOW) || (temp > TEMP_HIGH);
    bool humWarn  = (hum  < HUMID_LOW) || (hum  > HUMID_HIGH);
    bool hrWarn   = (hr   < HR_LOW)   || (hr   > HR_HIGH);

    int warnCount = (tempWarn?1:0) + (humWarn?1:0) + (hrWarn?1:0);
    if (warnCount == 0)      status = "NORMAL";
    else if (warnCount == 1) status = "ATENCAO";
    else                     status = "CRITICO";

    setStatusIndicators(status);

    // OLED
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(1);
    display.printf("Temp: %.1f C\n", temp);
    display.printf("Umid: %.1f %%\n", hum);
    display.printf("HR (pot): %d bpm\n", hr);
    display.printf("POT: %4d (%.2f V)\n", potRaw, potV);
    display.printf("Status: %s\n", status);
    display.display();

    // MQTT
    publishTelemetry(temp, hum, hr, spo2, status);

    // ======== BUZZER ========
    if (strcmp(status, "CRITICO") == 0) {
      // 3 bipes curtos (frequência alta)
      for (int i = 0; i < 3; i++) {
        tone(BUZZER, 1000, 150); // 1000 Hz por 150 ms
        delay(200);
      }
      noTone(BUZZER);
    }
    else if (strcmp(status, "ATENCAO") == 0) {
      // 1 bip curto (frequência média)
      tone(BUZZER, 700, 100); // 700 Hz por 100 ms
      delay(150);
      noTone(BUZZER);
    }
    else {
      // Estado NORMAL → buzzer silencioso
      noTone(BUZZER);
    }
  }
}

// ===== AUXILIARES =====
void connectWiFi() {
  Serial.printf("Conectando a WiFi: %s\n", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nWiFi conectado!");
  Serial.print("IP: "); Serial.println(WiFi.localIP());
}

void connectMQTT() {
  while (!mqttClient.connected()) {
    String clientId = "ESP32Materna-" + String(random(0xffff), HEX);
    if (mqttClient.connect(clientId.c_str(),MQTT_USER,MQTT_PASS)) {
      Serial.println("Conectado ao broker MQTT");
    } else {
      Serial.println("Tentando reconectar MQTT...");
      delay(2000);
    }
  }
}

void publishTelemetry(float t, float h, int hr, float spo2, const char* status) {
  String topic = String("gestacao/") + DEVICE_ID + "/dados";
  String payload = buildJSON(t, h, hr, spo2, status);
  mqttClient.publish(topic.c_str(), payload.c_str());
  Serial.println("MQTT enviado: " + payload);
}

String buildJSON(float t, float h, int hr, float spo2, const char* status) {
  String j = "{";
  j += "\"device\":\"" + String(DEVICE_ID) + "\",";
  j += "\"temperature\":" + String(t,1) + ",";
  j += "\"humidity\":" + String(h,1) + ",";
  j += "\"heartrate\":" + String(hr) + ",";
  j += "\"spo2\":" + String(spo2,1) + ",";
  j += "\"status\":\"" + String(status) + "\"";
  j += "}";
  return j;
}

void setStatusIndicators(const char* status) {
  if (strcmp(status,"NORMAL")==0) {
    digitalWrite(LED_GREEN,HIGH);
    digitalWrite(LED_YELLOW,LOW);
    digitalWrite(LED_RED,LOW);
  } else if (strcmp(status,"ATENCAO")==0) {
    digitalWrite(LED_GREEN,LOW);
    digitalWrite(LED_YELLOW,HIGH);
    digitalWrite(LED_RED,LOW);
  } else {
    digitalWrite(LED_GREEN,LOW);
    digitalWrite(LED_YELLOW,LOW);
    digitalWrite(LED_RED,HIGH);
  }
}

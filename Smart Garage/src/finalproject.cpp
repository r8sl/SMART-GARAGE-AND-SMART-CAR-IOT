#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

// WiFi credentials
char ssid[] = "WE_F22EFA";
char pass[] = "m4n24614";

// HiveMQ Cloud Broker settings
const char *mqtt_server = "37e6e9413a0247919be5adca7609dc1e.s1.eu.hivemq.cloud";
const char *mqtt_username = "Mohamed";
const char *mqtt_password = "Mohamed0";
const int mqtt_port = 8883;

WiFiClientSecure espClient;
PubSubClient client(espClient);

// HiveMQ Cloud Let's Encrypt CA certificate
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

// Pin Definitions
#define IR 34
#define SS_PIN 5
#define RST_PIN 27
#define servo 32
#define PLAY_E 14
#define led 25
#define ldr 33
#define servotenda 2
#define rain 35
#define playTime 5000
#define gas 15
#define motion 13
#define buzzer 12
#define ledinside 26
#define DHT_SENSOR_PIN 21
#define DHT_SENSOR_TYPE DHT11
#define push_button 4

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE); // Initialize DHT sensor
MFRC522 rfid(SS_PIN, RST_PIN);                   // Initialize RFID reader
Servo myservo;                                   // Initialize servo for the door
Servo servot;                                    // Initialize servo for the tenda

int pinStateCurrent = LOW;
int pinStatePrevious = LOW;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize LCD

byte knownUID[] = {0xA3, 0x88, 0x06, 0x25}; // UID to compare against

unsigned long lastMsg = 0;

void setup_wifi() // function to set up wifi for the esp32 and make sure it's connected then print the local ip of the esp32
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length) // MQTT callback function for handling incoming messages

{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (int i = 0; i < length; i++)
  {
    message += (char)payload[i];
  }
  Serial.println(message);

  // Control garage door servo

  if (String(topic) == "garage/door")
  {
    int angle = message.toInt();
    myservo.write(angle); // Move the garage door servo
  }

  // Control garage tenda servo

  if (String(topic) == "garage/tenda")
  {
    int angle = message.toInt();
    servot.write(angle); // Move the garage tenda servo
    delay(3000);
  }
}

void reconnect()
{
  // Reconnect to MQTT broker if connection is lost

  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection… ");
    String clientId = "ESP32Client";
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password))
    {
      Serial.println("connected");
      client.subscribe("garage/door");
      client.subscribe("garage/tenda");
      client.publish("ESP32/status", "connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void publishSensorData()
{
  // Publish sensor data to MQTT broker as a JsonObject

  DynamicJsonDocument jsonDoc(200);

  int irReading = analogRead(IR);    // Read IR sensor value
  int gasReading = digitalRead(gas); // Read gas sensor value

  // Add small delay before reading temperature for stability
  delay(1000);
  float tempC = dht_sensor.readTemperature(); // Read temperature

  String lcdStatus = "No Warning";

  // Check sensor connectivity or errors
  if (isnan(tempC))
  {
    tempC = 33.79999925; // Default value if temperature is NaN
  }
  if (gasReading == 0)
  {
    lcdStatus = "Gas Leaking!"; // Update LCD if gas is detected
  }

  // Populate JSON data
  jsonDoc["ir"] = irReading;
  jsonDoc["gas"] = gasReading;
  jsonDoc["temperature"] = tempC;
  jsonDoc["lcd"] = lcdStatus;

  char jsonBuffer[256];
  serializeJson(jsonDoc, jsonBuffer);          // Serialize JSON data
  client.publish("garage/sensor", jsonBuffer); // Publish data to MQTT topic
  delay(1000);
}

void setup()
{
  // Initial setup
  Serial.begin(9600);
  setup_wifi();
  espClient.setCACert(root_ca);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  pinMode(IR, INPUT);
  SPI.begin();
  rfid.PCD_Init();             // Initialize RFID reader
  myservo.attach(servo);       // attach the garage door servo
  servot.attach(servotenda);   // attach the garage teneda servo
  pinMode(led, OUTPUT);        // defining the mode for the outside leds as OUTPUT
  pinMode(PLAY_E, OUTPUT);     // defining the mode for the speaker
  myservo.write(180);          // Set servo to initial position
  pinMode(ldr, INPUT);         // defining the mode for the ldr sensor
  pinMode(rain, INPUT);        // defining the mode for the rain sensor
  dht_sensor.begin();          // Initialize DHT sensor
  pinMode(gas, INPUT);         // defining the mode for the gas sensor
  pinMode(buzzer, OUTPUT);     // defining the mode for the buzzer that works when the gas sensor is active (gas leaking)
  pinMode(ledinside, OUTPUT);  // defining the mode for the leds inside the garage
  lcd.init();                  // Initialize LCD
  lcd.backlight();             // Turn on LCD backlight
  pinMode(push_button, INPUT); // defining the mode for the push button that opens the garage door from inside the garage
}

void loop()
{
  if (!client.connected())
  {
    reconnect(); // Reconnect to MQTT broker if not connected
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000)
  {
    publishSensorData(); // Publish sensor data every 2 seconds
    lastMsg = now;
  }

  // code for handling sensors and other hardware

  int irreading = analogRead(IR); // Read IR sensor
  if (irreading < 4000)
  {
    Serial.println(irreading);
    digitalWrite(led, HIGH);
    Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader");

    if (rfid.PICC_IsNewCardPresent())
    {
      if (rfid.PICC_ReadCardSerial())
      {
        MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
        Serial.print("RFID/NFC Tag Type: ");
        Serial.println(rfid.PICC_GetTypeName(piccType));

        Serial.print("UID:");
        bool isKnownUID = true;
        for (int i = 0; i < rfid.uid.size; i++)
        {
          Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(rfid.uid.uidByte[i], HEX);

          if (rfid.uid.uidByte[i] != knownUID[i])
          {
            isKnownUID = false; // Check if RFID UID matches known UID
          }
        }
        Serial.println();

        if (isKnownUID)
        {
          myservo.write(70);          // Open door if UID matches
          digitalWrite(PLAY_E, HIGH); // Start the Speaker if the UID matches
          delay(50);
          digitalWrite(PLAY_E, LOW);
          Serial.println("Playback Started");
          delay(playTime);

          Serial.println("Playback Ended");
          digitalWrite(led, LOW);
          myservo.write(180); // Close door after playback
          delay(5000);
        }
        else
        {
          Serial.println("No Access Granted");
        }
      }
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
  }

  int lightstate = analogRead(ldr); // Read LDR sensor
  int rainstate = analogRead(rain); // Read rain sensor

  if (lightstate > 2000 || rainstate < 500)
  {
    servot.write(80); // Close tenda if light is high or rain is detected
    delay(50);
    digitalWrite(ledinside, HIGH);
    Serial.println(rainstate);
  }
  else
  {
    servot.write(10); // Open tenda if no rain and no high light intensity
    delay(50);
    digitalWrite(ledinside, LOW);
  }
  delay(1000);
  float humi = dht_sensor.readHumidity(); // Read humidity
  delay(200);
  float tempC = dht_sensor.readTemperature(); // Read temperature in Celsius
  delay(200);
  float tempF = dht_sensor.readTemperature(true);  // Read temperature in Fahrenheit
  if (isnan(tempC) || isnan(tempF) || isnan(humi)) // Read temperature in Fahrenheit
  {
    Serial.println("Failed to read from DHT sensor!");
  }
  else
  {
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%");

    Serial.print("  |  ");

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C  ~  ");
    Serial.print(tempF);
    Serial.println("°F");
  }
  delay(1000);

  int gasState = digitalRead(gas); // Read gas sensor
  if (gasState == HIGH)
  {
    Serial.println("No Gas");
    digitalWrite(buzzer, LOW); // Turn off buzzer if no gas is detected
  }
  else
  {
    Serial.println("The gas is present");
    digitalWrite(buzzer, HIGH); // Turn on buzzer if gas is detected
  }

  pinStatePrevious = pinStateCurrent;    // Store old motion sensor state
  pinStateCurrent = digitalRead(motion); // Read new motion sensor state

  if (pinStatePrevious == LOW && pinStateCurrent == HIGH)
  { // pin state change: LOW -> HIGH
    Serial.println("Motion detected!");
    digitalWrite(ledinside, HIGH); // Turn on LED if motion is detected
    delay(2000);
  }
  else if (pinStatePrevious == HIGH && pinStateCurrent == LOW)
  { // pin state change: HIGH -> LOW
    Serial.println("Motion stopped!");
    digitalWrite(ledinside, LOW); // Turn off LED if motion stops
    delay(2000);
  }

  if (gasState == 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Gas");
    lcd.setCursor(2, 1);
    lcd.print("Leaking"); // Display warning on LCD if gas is detected
    delay(2000);
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No Warning"); // Display "No Warning" on LCD if no gas
  }
  int button_state = digitalRead(push_button); // Read push button state
  if (button_state == HIGH)
  {
    Serial.println(button_state);
    myservo.write(70); // Open door if button is pressed
  }
  else
  {
    myservo.write(180); // Close door if button is not pressed
  }
}

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

// Configurar propiedades WiFi y MQTT
const char* ssid = "MySpectrumWiFi3B-2G";
const char* password = "guavapaper824";

const char* mqtt_server = "ioticos.org";
const int mqtt_port = 1883;
const char* mqtt_user = "QF3QD2h47BCHTD0";
const char* mqtt_pass = "oqhzVNRisZQluqQ";
const char* root_topic_subscribe = "zgwC79FpeM0QuUu/input";
const char* root_topic_publish = "zgwC79FpeM0QuUu/output";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

// Inicializar pines
const int motor1Pin1 = 27;
const int motor1Pin2 = 26;
const int enable1Pin = 14;
const int sensorPin = 35;
const int indicadorCerca = 2;

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 100;
int valueSensor = 0;

//Intervalos
const long intervalVel = 200;
unsigned long previousMillis = 0;

void setup_wifi() {
  // Coneccion a la red WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }

}

void reconnect() {

  while (!client.connected()) {
    Serial.println("Intentando Conexion MQTT");

    String clientId = "iot_1_";
    clientId = clientId + String(random(0xffff), HEX);

    if(client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {

      Serial.println("Conexion a MQTT exitosa!!");
      client.publish("salida", "primer mensaje");
      if(client.subscribe(root_topic_subscribe)) {
        Serial.println("Suscripcion OK");
      } else {
        Serial.println("Fallo suscripcion");
      }

    } else {

      Serial.println("Fallo la Conexion a MQTT: ");
      Serial.print(client.state());
      Serial.print("Se intentara de nuevo en 5  segundos");
      delay(5000);

    }

  }

}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido bajo el topico -> ");
  Serial.print(topic);
  Serial.print("\n");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  char mensaje = (char)payload[0];

  bool finish = false;
  int mover = String(mensaje).toInt();
  bool estanteCerca = false;
  //Numero de estantes detectados
  int estantes = 0;

  while(!finish) {

    unsigned long currentMillis = millis();
    // Leer valor del sensor
    if(valueSensor > 3600 && !estanteCerca) {
      estantes++;
      estanteCerca = true;
      digitalWrite(indicadorCerca, HIGH);
    }else if(valueSensor < 2800 && estanteCerca){
      estanteCerca = false;
      digitalWrite(indicadorCerca, LOW);
    }

    valueSensor = analogRead(sensorPin);

    if(currentMillis - previousMillis >= intervalVel) {

      previousMillis = currentMillis;
      if(estantes < mover) {

        // Mover el motor DC aumentando la velocidad
        digitalWrite(motor1Pin1, HIGH);
        digitalWrite(motor1Pin2, LOW);

        if (dutyCycle < 255){
          ledcWrite(pwmChannel, dutyCycle);
          dutyCycle = dutyCycle + 5;
        }

      }else {
        // Reducir la velocidad del motor DC hasta parar
        if (dutyCycle > 100){
          ledcWrite(pwmChannel, dutyCycle);
          dutyCycle = dutyCycle - 5;
        } else {
          digitalWrite(motor1Pin1, LOW);
          digitalWrite(motor1Pin2, LOW);
          finish = true;
        }

      }

    }

  }

}

void setup() {
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  pinMode(indicadorCerca, OUTPUT);

  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel);
  //Conectando WiFi
  setup_wifi();
  //Conectando y configurando MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {

  if(!client.connected()) {

    reconnect();

  } else {

    if(millis() - lastMsg >  2000) {
      lastMsg = millis();
      value++;
      String mes = "Valor -> " + String(value);
      mes.toCharArray(msg, 50);
      client.publish(root_topic_publish, msg);
      Serial.println("Mensaje enviado -> " + String(value));
    }

  }

  client.loop();

}

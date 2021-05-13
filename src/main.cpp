#include <Arduino.h>
#include <PubSubClient.h>
#include "MQTTConnection.h"
#include "Alacena.h"

char* server = (char*)"z2d95e64.en.emqx.cloud";
int port = 11185;
char* user = (char*)"K2SxWcwBK327gQq";
char* pass = (char*)"Q4HXUAu8pkAgTLa";
char* topic_subscribe = (char*)"YvdR293zKOCMVSD/input";
char* topic_publish = (char*)"YvdR293zKOCMVSD/output";
MQTTConnection* mqtt;
WiFiClient espClient;
PubSubClient* client = new PubSubClient(espClient);
Alacena* alacena = new Alacena();

const int sensorUp = 34;
const int sensorDown = 39;

void recibirMensaje(char* topic, byte* payload, unsigned int length) {

  char accion = (char)payload[0];
  char firstArgument;
  char secondArgument;
  int auxConversion;
  char* respuesta;

  switch(accion) {
	case '1':
    firstArgument = (char)payload[1];
    secondArgument = (char)payload[2];
    respuesta = (char*)"OK 1";
    mqtt->publicar(client, respuesta);
    alacena->moverEstante(firstArgument, secondArgument);
		break;
  case '2':
    firstArgument = (char)payload[1];
    respuesta = (char*)"OK 2";
    mqtt->publicar(client, respuesta);
		alacena->encontrarProducto(firstArgument);
		break;
  case '3':
    firstArgument = (char)payload[1];
    auxConversion = firstArgument - '0';
    alacena->setPosicionActual(auxConversion);
	default:
    respuesta = (alacena->getPosicionActual() == 0) ? (char*)"0" : (char*)"OK 0";
    mqtt->publicar(client, respuesta);
		break;
  }

}

void setup() {
  Serial.begin(115200);
  mqtt = new MQTTConnection(client, server, port, user, pass,
    topic_subscribe, topic_publish, recibirMensaje);
  mqtt->conectar(client);
  alacena->calibrar();
}

void loop() {

  if(!client->connected()) {
    mqtt->conectar(client);
  }

  int valueUp = analogRead(sensorUp);
  int valueDown = analogRead(sensorDown);

  if(valueUp > 2000) {
    alacena->moverEstante('1', '0');
  } else if(valueDown > 2000) {
    alacena->moverEstante('1', '1');
  }

  client->loop();
}

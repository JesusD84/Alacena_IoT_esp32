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

void recibirMensaje(char* topic, byte* payload, unsigned int length) {

  char accion = (char)payload[0];
  char nPasos;
  char direccion;
  char* respuesta;

  switch(accion) {
	case '1':
    nPasos = (char)payload[1];
    direccion = (char)payload[2];
    respuesta = (char*)"Recibido";
    mqtt->publicar(client, respuesta);
    alacena->moverEstante(nPasos, direccion);
		break;
	case '2':
		alacena->listarProductos();
		break;
  case '3':
		alacena->encontrarProducto();
		break;
  case '4':
		alacena->guardarProducto();
		break;
  case '5':
		alacena->sacarProducto();
		break;
	default:
    respuesta = (char*)"OK";
    mqtt->publicar(client, respuesta);
		break;
  }

}

void setup() {
  Serial.begin(115200);
  mqtt = new MQTTConnection(client, server, port, user, pass,
    topic_subscribe, topic_publish, recibirMensaje);
    mqtt->conectar(client);
}

void loop() {

  if(!client->connected()) {
    mqtt->conectar(client);
  }

  client->loop();
}

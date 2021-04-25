#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "WiFiConnection.h"

class MQTTConnection {
	private:
    char* server;
    int port;
    char* user;
    char* pass;
    char* root_topic_subscribe;
    char* root_topic_publish;
    String clientId;
		int status;

	public:

		MQTTConnection(PubSubClient* client, char* server, int port, char* user,
			char* pass,char* topic_subscribe, char* topic_publish,
			void (callback)(char*, byte*, unsigned int)) {
			char* ssid = (char *)"MySpectrumWiFi3B-2G";
			char* passWiFi = (char *)"guavapaper824";

			WiFiConnection* wifi = new WiFiConnection(ssid, passWiFi);

			this->server = server;
      this->port = port;
      this->user = user;
      this->pass = pass;
      this->root_topic_subscribe = topic_subscribe;
      this->root_topic_publish = topic_publish;

      wifi->conectar();
      client->setServer(server, port);
      client->setCallback(callback);
			this->comprobarEstado(client);

		}

    void conectar(PubSubClient* client) {

		  while (this->comprobarEstado(client) != 0) {
		    Serial.println("Intentando Conexion MQTT");

		    this->clientId = "iot_1_";
		    this->clientId = this->clientId + String(random(0xffff), HEX);

		    if(client->connect(clientId.c_str(), this->user, this->pass)) {

		      Serial.println("Conexion a MQTT exitosa!!");
		      client->publish(this->root_topic_publish, "primer mensaje");
		      if(this->suscribir(client)) {
		        Serial.println("Suscripcion OK");
		      } else {
		        Serial.println("Fallo suscripcion");
		      }

		    } else {

		      Serial.println("Fallo la Conexion a MQTT: ");
		      Serial.print(this->comprobarEstado(client));
		      Serial.print("Se intentara de nuevo en 5  segundos");
		      delay(5000);

		    }

		  }

    }

    void desconectar(PubSubClient* client) {
      client->disconnect();
    }

		bool suscribir(PubSubClient* client) {
			return client->subscribe(this->root_topic_subscribe);
		}

		void publicar(PubSubClient* client, char* mensaje) {
			 client->publish(this->root_topic_publish, mensaje);
		}

    int comprobarEstado(PubSubClient* client) {
      status = client->state();
      return this->status;
    }

		void bucle(PubSubClient* client) {
			client->loop();
		}

};

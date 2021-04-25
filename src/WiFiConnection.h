#include <Arduino.h>
#include <WiFi.h>

class WiFiConnection {
	private:
    char* ssid;
    char* password;
    int status;
    IPAddress ip;

	public:

		WiFiConnection(char* ssid, char* password) {
			this->ssid = ssid;
      this->password = password;
		}

    void conectar() {
      WiFi.begin(this->ssid, this->password);
      Serial.print("\nConectando a ");
      Serial.print(this->ssid);
      while(this->comprobarEstado() != WL_CONNECTED) {
        delay(500);
        Serial.print(" .");
      }
      this->ip = WiFi.localIP();
			Serial.println(this->ip);
    }

    void desconectar() {
      WiFi.disconnect();
			Serial.println("Tarjeta Desconectada");
    }

    int comprobarEstado() {
      this->status = WiFi.status();
      return this->status;
    }

    void setSsid(char* ssid) {
      if(comprobarEstado() != WL_CONNECTED ) {
        this->ssid = ssid;
      } else {
        Serial.println("No se cambiar el ssid ya que ya se encuentra conectada a una actualmente");
      }
    }

    void setPassword(char* password) {
      if(comprobarEstado() != WL_CONNECTED ) {
        this->password = password;
      } else {
        Serial.println("No se cambiar el password ya que ya se encuentra conectada a una red actualmente");
      }
    }

    IPAddress getIp() {
      if(comprobarEstado() == WL_CONNECTED ) {
        return this->ip;
      } else {
        Serial.println("No se encuentra conectado a una red actualmente");
      }
    }

};

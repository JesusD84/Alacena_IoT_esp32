#include <Arduino.h>

class Motor {
	private:
    int pin1;
    int pin2;
    int enablePin;
    int sensorPin;
    int ledIndicadorCerca;
    int frecuenciaPWM;
    int canalPWM;
    int resolucionPWM;
    int dutyCycle;

	public:

		Motor(int pin1, int pin2, int enablePin, int sensorPin) {
			this->pin1 = pin1;
			pinMode(this->pin1, OUTPUT);
      this->pin2 = pin2;
			pinMode(this->pin2, OUTPUT);
      this->enablePin = enablePin;
			pinMode(this->enablePin, OUTPUT);
      this->sensorPin = sensorPin;
      this->ledIndicadorCerca = 2;
			pinMode(this->ledIndicadorCerca, OUTPUT);
      this->frecuenciaPWM = 30000;
      this->canalPWM = 0;
      this->resolucionPWM = 8;
			ledcSetup(this->canalPWM, this->frecuenciaPWM, this->resolucionPWM);
			ledcAttachPin(this->enablePin, this->canalPWM);
      this->dutyCycle = 100;
		}

    void mover(int nPasos, int direccion) {
      //Intervalos
      const unsigned long intervalVel = 300;
      unsigned long previousMillis = 0;
      //direccion. 0 - adelante. 1 - atras
      bool finish = false;
      bool estanteCerca = false;
      int valueSensor = analogRead(sensorPin);
      int estantes = (valueSensor > 3600) ? -1 : 0;

      while(!finish) {

        unsigned long currentMillis = millis();

        if(currentMillis - previousMillis >= intervalVel) {
					previousMillis = currentMillis;

					// Leer valor del sensor
					if(valueSensor > 3600 && !estanteCerca) {
						estantes++;
						estanteCerca = true;
						digitalWrite(this->ledIndicadorCerca, HIGH);
					}else if(valueSensor < 2800 && estanteCerca){
						estanteCerca = false;
						digitalWrite(this->ledIndicadorCerca, LOW);
					}

					valueSensor = analogRead(sensorPin);

          if(estantes < nPasos) {

            // Mover el motor DC aumentando la velocidad
            if(direccion == 0) {
              digitalWrite(this->pin1, HIGH);
              digitalWrite(this->pin2, LOW);
            } else {
              digitalWrite(this->pin1, LOW);
              digitalWrite(this->pin2, HIGH);
            }

            if (this->dutyCycle < 255){
              ledcWrite(this->canalPWM, this->dutyCycle);
              this->dutyCycle = this->dutyCycle + 5;
            }

          }else {
            // Reducir la velocidad del motor DC hasta parar
            if (this->dutyCycle > 100){
              ledcWrite(this->canalPWM, this->dutyCycle);
              this->dutyCycle = this->dutyCycle - 5;
            } else {
              digitalWrite(this->pin1, LOW);
              digitalWrite(this->pin2, LOW);
              finish = true;
            }

          }

        }

      }

    }

};

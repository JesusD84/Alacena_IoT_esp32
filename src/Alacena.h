#include <Arduino.h>
#include "Motor.h"

class Alacena {
  private:
    int posicionActual;
    Motor* motor;

  public:
    Alacena() {
       motor = new Motor(27, 26, 14, 35);
       posicionActual = 0;
    }

    void calibrar() {
      motor->mover(0, 0);
    }

    void moverEstante(char nPasos, char direccion) {
      int pasos = nPasos - '0';
      int dir = direccion - '0';
      motor->mover(pasos, dir);

      if(this->posicionActual != 0) {
          if(direccion == 0) {
            this->posicionActual += pasos;
            if(this->posicionActual > 8) {
              this->posicionActual -= 8;
            }
          } else {
            this->posicionActual -= pasos;
            if(this->posicionActual < 1) {
              this->posicionActual += 8;
            }
          }
      }

    }

    void encontrarProducto(char pos) {
      int posicion = pos - '0';
      int diffSuperior = 0;
      int diffInferior = 0;

      if(this->posicionActual < posicion) {
        diffSuperior = this->posicionActual - posicion + 8;
        diffInferior = posicion - this->posicionActual;
      } else if(this->posicionActual > posicion) {
        diffSuperior = this->posicionActual - posicion;
        diffInferior = posicion - this->posicionActual + 8;
      }

      if(diffInferior < diffSuperior) {
        motor->mover(diffInferior, 1);
      } else if(diffSuperior < diffInferior) {
        motor->mover(diffSuperior, 0);
      }

      this->posicionActual = posicion;

    }

    int getPosicionActual() {
      return this->posicionActual;
    }

    void setPosicionActual(int posicionActual) {
      this->posicionActual = posicionActual;
    }

};

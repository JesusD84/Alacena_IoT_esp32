#include <Arduino.h>
#include "Motor.h"

class Alacena {
  private:
    int posicionActual;
    Motor* motor;

  public:
    Alacena() {
       motor = new Motor(27, 26, 14, 35);
    }

    void moverEstante(char nPasos, char direccion) {
      int pasos = nPasos - '0';
      int dir = direccion - '0';
      motor->mover(pasos, dir);
    }

    void listarProductos() {

    }

    void encontrarProducto() {

    }


    void guardarProducto() {

    }

    void sacarProducto() {

    }

};

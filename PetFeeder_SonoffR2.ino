// PETFEEDER V0.12 15/05/2020


#include <Servo.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


//CONSTANTES
#define Pulsador 0
#define PinServo 3 //GPIO3 -> RX en el sonoff
#define LedInterno 13

//OBJETOS
Servo myservo;  // Creamos un objeto llamado "myservo" para nuestro servo rotación continua

//VARIABLES
byte servoDerecha = 40 ;
byte servoIzquierda = 140;
int cantidad = 6; //Por defecto 6. Luego se puede modificar con blynk
int v0;  //Variable Blynk para activar alimentación remota
unsigned long initMillis;
unsigned long lastMillis;
char ssid[] = "XXXXX"; // Your WiFi credentials
char pass[] = "XXXXX";     // Your WiFi password
char auth[] = "XXXXX"; //Token Blynk




/**********************************************************FUCIONNES************************************************************************/

/********************************ALIMENTAR**********************************/
void alimentar() {

  //Accionar motor
  myservo.attach(PinServo);   // Ligamos nuestro servo al GPIO2 del sonoff

  digitalWrite(LedInterno, HIGH);

  for (int i = 0; i < cantidad; i++) {
    myservo.write(servoDerecha);
    delay(200); // Tiempo de rotación inversa
    myservo.write(servoIzquierda);
    delay(600); // Tiempo de rotación directa
  }

  myservo.detach(); //Desligamos el servo

  lastMillis = millis();
}

/****************************FIN ALIMENTAR**********************************/


/****************************BLYNK LECTURA VO*******************************/
BLYNK_WRITE(V0) //Button Widget is writing to pin V0
{
  v0 = param.asInt();
}
/****************************FIN BLYNK LECTURA VO***************************/


/****************************BLYNK LECTURA V1*******************************/
BLYNK_WRITE(V1) //Button Widget is writing to pin V0
{
  cantidad = param.asInt();
}
/****************************FIN BLYNK LECTURA VO***************************/

/********************************************************FIN FUNCIONES*********************************************************************/




/**********************************************************PROGRAMA************************************************************************/

void setup() {

  //INICIALIZACIONES
  Blynk.begin(auth, ssid, pass);

  //Configuracion de pines
  pinMode(LedInterno, OUTPUT);
  pinMode(Pulsador, INPUT); // Configuramos como entrada el pulsador

  delay(2000); //Esperamos 2s

  initMillis = millis();

  lastMillis = millis();

}


void loop() {

  Blynk.run();

  //Encendemos el Led interno 10 segundos para saber que todo se ha iniciado bien
  if (millis() - initMillis < 10000) {
    digitalWrite(LedInterno, LOW);
  } else {
    digitalWrite(LedInterno, HIGH);
  }

  //Accionamiento del Pettfeeder si VO o pulsador físico o si han pasado 14h sin entrar en la función Alimentar
  if (v0 || !digitalRead(Pulsador) || millis() - lastMillis > 50400000) {
    //Rotar el servo si han pasado más de 10s desde la última vez
    if (millis()-lastMillis > 10000 ) {
      alimentar();
    }
  }

}


/*******************************************************FIN PROGRAMA************************************************************************/

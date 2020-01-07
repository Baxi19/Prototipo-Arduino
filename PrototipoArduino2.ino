/*---------------------------------------------------------------------------------------------------------*/
//Instituto Tecnológico de Costa Rica
//Campus Tecnológico Local San Carlos
//Escuela de Ingeniería en computación
//Proyecto en Arduino
//Randald Villegas

/*---------------------------------------------------------------------------------------------------------*/
//import y variables para el sistema
#include <LiquidCrystal.h>
#include <Wire.h>
#include "RTClib.h"
/*---------------------------------------------------------------------------------------------------------*/
RTC_DS3231 rtc;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
bool portada = false;
int relay = 6;
bool riegoActivado = false;
//int contador = 0;
/*---------------------------------------------------------------------------------------------------------*/
/*VARIABLES QUE SE PUEDEN REGULAR*/
String daysOfTheWeek[7] = { "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado" };
int tiempoRiego = 4;
int horaPrimerRiego = 8;
int minutoPrimerRiego =20;
int horaSegundoRiego = 16;
int minutoSegundoRiego = 20;

/*---------------------------------------------------------------------------------------------------------*/
//funcion para imprimir en pantalla
void show(int columna, int fila, String texto ) {
  lcd.setCursor(fila, columna);
  lcd.print(texto);
}

/*---------------------------------------------------------------------------------------------------------*/
//funcion para imprimir portada   = 10 segundos
String imprimePortada() {
  lcd.clear();
  String texto1;
  String texto2; 
  texto1 = "Tecnologico de";
  show(0, 0, texto1);
  texto2 = "Costa Rica";
  show(1, 0, texto2);
  delay(1000);

  lcd.clear();
  texto1 = "Ingenieria en";
  show(0, 0, texto1);
  texto2 = "Computacion";
  show(1, 0, texto2);
  delay(1000);

  lcd.clear();
  texto1 = "Campus TEC";
  show(0, 0, texto1);
  texto2 = "Local San Carlos";
  show(1, 0, texto2);
  delay(1000);

  lcd.clear();
  texto1 = "   Profesor:    ";
  show(0, 0, texto1);
  delay(1000);

  lcd.clear();
  texto1 = "Jorge";
  show(0, 0, texto1);
  texto2 = "Alfaro Velasco";
  show(1, 0, texto2);
  delay(1000);

  lcd.clear();
  texto1 = "   Estudiante:   ";
  show(0, 0, texto1);
  delay(1000);

  lcd.clear();
  texto1 = "Randald";
  show(0, 0, texto1);
  texto2 = "Villegas Brenes";
  show(1, 0, texto2);
  delay(2000);

  lcd.clear();
  texto1 = "Sistema de Riego";
  show(0, 0, texto1);
  texto2 = "**Automatizado**";
  show(1, 0, texto2);
  delay(2000);
  lcd.clear();

  portada = true;
  return "";
}
/*---------------------------------------------------------------------------------------------------------*/
//funcion para obtener la fecha
String getDia(DateTime now) {
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  String dia =  (String)(daysOfTheWeek[now.dayOfTheWeek()]);
  return dia;
}
/*---------------------------------------------------------------------------------------------------------*/
//funcion para obtener la fecha
String getFecha(DateTime now) {
  String fecha = (String)(now.day()) + "/" + (String)(now.month()) + "/" + (String)(now.year());
  return fecha;
}
/*---------------------------------------------------------------------------------------------------------*/
//funcion para obtener el tiempo
String getHora(DateTime now) {
  String hora = (String)(now.hour()) + ":" + (String)( now.minute()) + ":" + (String)(now.second());
  return hora;
}
/*---------------------------------------------------------------------------------------------------------*/
//funcion para imprimir el tiempo
void imprimeTiempo(DateTime now) {
  lcd.clear();
  String fecha = getFecha(now);
  String hora = getHora(now);
  hora = hora + " " + ((String)(daysOfTheWeek[now.dayOfTheWeek()]));
  show(0, 0, hora);
  show(1, 0, fecha);
}
/*---------------------------------------------------------------------------------------------------------*/
//Funcion para configuraciones
void setup() {
  Serial.begin(9600);
  delay(1000);
  if (!rtc.begin()) {
    Serial.println(F("No se encontro el RTC"));
    while (1);
  }
  // Si desconecto de corriente tomamos la hora de compilacion
  if (rtc.lostPower()) {
    // Fijar a fecha y hora de compilacion
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  //rtc.adjust(DateTime(2020, 1, 7, 11,57, 0));
  lcd.begin(16, 2);
  pinMode(relay, OUTPUT);
}
/*---------------------------------------------------------------------------------------------------------*/
//ciclo de aplicacion
void loop() {
  digitalWrite(relay, LOW);
  lcd.clear();
  DateTime now = rtc.now(); //Leer datos del RTC y mostrar
  imprimeTiempo(now);  
  if ((int)(now.hour()) == horaPrimerRiego) {/*Si es la hora del primer riego*/
    if (((int)(now.minute()) == minutoPrimerRiego) && ((int)(now.second() >= 0)) && ((int)(now.second() < tiempoRiego))) {
      lcd.clear();
      digitalWrite(relay, HIGH);
      int cont = 0;
      while (cont < tiempoRiego) {
        cont+=1;
        delay((1000));
      }
      digitalWrite(relay, LOW);
      asm("jmp 0x0000");
      //Serial.println("Segundos del Primer Riego =" + ((String)(now.second()))  );
    }
  } else if ((int)(now.hour()) == horaSegundoRiego) { /*Si es la hora del segundo riego*/
    if (((int)(now.minute()) == minutoSegundoRiego) && ((int)(now.second() >= 0)) && ((int)(now.second() < tiempoRiego))) {
      lcd.clear();
      digitalWrite(relay, HIGH);
      int cont = 0;
      while (cont < tiempoRiego) {
        cont+=1;
        delay((1000));
      }
      digitalWrite(relay, LOW);
      asm("jmp 0x0000");
      //Serial.println("Segundos del Segundo Riego =" + ((String)(now.second())) );
    }
  }
  if ((portada == false ) ) {
    digitalWrite(relay, LOW);
    imprimePortada();
  }
  //contador += 1;
  //if (contador == 60) {
    if(((int)(now.second() == 15))){
    asm("jmp 0x0000");
    //portada = false;
    //contador = 0;
  }
  delay(1000);
}

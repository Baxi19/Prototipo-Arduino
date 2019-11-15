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
int contador = 0;
/*---------------------------------------------------------------------------------------------------------*/
/*VARIABLES QUE SE PUEDEN REGULAR*/
String daysOfTheWeek[7] = { "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado" };
int tiempoRiego = 3;
int horaPrimerRiego = 8;
int minutoPrimerRiego =0;
int horaSegundoRiego = 16;
int minutoSegundoRiego = 0;

/*---------------------------------------------------------------------------------------------------------*/
//funcion para imprimir en pantalla
void show(int columna, int fila, String texto ) {
  lcd.setCursor(fila, columna);
  lcd.print(texto);
}

/*---------------------------------------------------------------------------------------------------------*/
//funcion para imprimir portada   = 14 segundos
String imprimePortada() {
  lcd.clear();
  String texto = "Tecnologico de";
  show(0, 0, texto);
  String aux = "Costa Rica";
  show(1, 0, aux);
  delay(2000);

  lcd.clear();
  String texto1 = "Ingenieria en";
  show(0, 0, texto1);
  String aux1 = "Computacion";
  show(1, 0, aux1);
  delay(2000);

  lcd.clear();
  String texto3 = "Campus TEC";
  show(0, 0, texto3);
  String aux3 = "Local San Carlos";
  show(1, 0, aux3);
  delay(2000);

  lcd.clear();
  String texto10 = "Profesor: ";
  show(0, 0, texto10);
  delay(1000);

  lcd.clear();
  String texto11 = "Jorge";
  show(0, 0, texto11);
  String aux11 = "Alfaro V.";
  show(1, 0, aux11);
  delay(2000);

  lcd.clear();
  String texto4 = "Estudiante: ";
  show(0, 0, texto4);
  delay(1000);

  lcd.clear();
  String texto5 = "Randald";
  show(0, 0, texto5);
  String aux5 = "Villegas B.";
  show(1, 0, aux5);
  delay(2000);

  lcd.clear();
  String texto2 = "Sistema de Riego";
  show(0, 0, texto2);
  String aux2 = "Automatizado";
  show(1, 0, aux2);
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
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.println();
  String fecha = (String)(now.day()) + "/" + (String)(now.month()) + "/" + (String)(now.year());
  return fecha;
}
/*---------------------------------------------------------------------------------------------------------*/
//funcion para obtener el tiempo
String getHora(DateTime now) {
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
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

  //rtc.adjust(DateTime(2019, 11, 13, 10,37, 0));
  
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
      Serial.println("Hora Primer Riego activada" );
      digitalWrite(relay, HIGH);
      int cont = 0;
      while (cont < tiempoRiego) {
        lcd.clear();
        String texto = "Riego Activado";
        show(0, 0, texto);
        cont+=1;
        delay((1000));
      }
      digitalWrite(relay, LOW);
      Serial.println("Segundos del Primer Riego =" + ((String)(now.second()))  );
    }
  } else if ((int)(now.hour()) == horaSegundoRiego) { /*Si es la hora del segundo riego*/
    if (((int)(now.minute()) == minutoSegundoRiego) && ((int)(now.second() >= 0)) && ((int)(now.second() < tiempoRiego))) {
      Serial.println("Hora Segundo Riego activada" );
      digitalWrite(relay, HIGH);
      int cont = 0;
      while (cont < tiempoRiego) {
        lcd.clear();
        String texto = "Riego Activado";
        show(0, 0, texto);
        cont+=1;
        delay((1000));
      }
      digitalWrite(relay, LOW);
      Serial.println("Segundos del Segundo Riego =" + ((String)(now.second())) );
    }
  }
  if ((portada == false ) ) {
    digitalWrite(relay, LOW);
    imprimePortada();
  }

  contador += 1;
  if (contador == 60) {
    portada = false;
    contador = 0;
  }

  delay(1000);

}

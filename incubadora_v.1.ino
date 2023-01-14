//-----------------------------INCLUIR LIBRERIAS PARA LCD-----------------------------
#include <Wire.h>
#include <LiquidCrystal_I2C.h>          // Libreria para LCD con I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);     // Crear objeto del tipo Liqui... y se le pasa direccion y tipo.

//----------------------------INCLUIR LIBRERIAS PARA SENSOR TEMPERATURA-----------------

#include <OneWire.h>            //Incluimos las librerías necesarias OneWire y DallasTemperature
#include <DallasTemperature.h>
#define pinDeEntrada 2          // Conectamos el pin de datos del sensor al pin digital 2 del arduino 
OneWire oneWire(pinDeEntrada);  //Configuramos una instancia oneWire para comunicarnos con cualquier dispositivo OneWire
DallasTemperature sensorTemperatura(&oneWire);    //Pasamos nuestra resferencia oneWire a la instancia de DallasTemperature

float temperaturaIncubacion =(37.7);    // Asignamos una temperatura control
float temperaturaSensor = sensorTemperatura.getTempCByIndex(0); // Asigno a temperatura la lectura del sensor para que me la de en float
boolean resistencia = 8;
//----------------------------INCLUIR LIBRERIAS PARA SENSOR HUMEDAD-----------------
#include "DHT.h";

DHT sensorHumedad= DHT(2, DHT11);
byte humedadSensor=0;
byte humedadIncubacion = 65;
boolean humidificador=10;


//-----------------------------DECLARACION DE VARIABLES-----------------------------------
unsigned long tiempo = 0;   // variable para trabajar con millis
boolean motor = 13;             // variable para controlar el motor de volteo
int contador2 = 0;          // variable para controlar la periodicidad y el tiempo de volteo
byte segundos = 0;          // variable para control del tiempo
byte minutos = 0;           // variable para control del tiempo
byte horas = 0;             // variable para control del tiempo
byte dias = 0;              // variable para control del tiempo

void setup(){
//-------------------------------INICIAMOS LCD--------------------------------------------

      lcd.init();         // Iniciamos LCD
      lcd.backlight();    // Encendemos backlight
      
//-------------------------INICIAMOS SENSOR TEMPERATURA Y HUMEDAD--------------------------------------------
      
      sensorTemperatura.begin();  //Inicializamos SENSOR TEMP 

      sensorHumedad.begin();      //Inicializamos SENSOR HUM 
      
//-------------------------------INICIAMOS MONITOR SERIE-------------------------------------------- 
     
      Serial.begin(9600);                // inicializar el puerto serie
      pinMode (motor, OUTPUT);           // poner el pin digital en modo salida
      pinMode (resistencia, OUTPUT);     // poner el pin digital en modo salida 
      pinMode (humidificador, OUTPUT);   // poner el pin digital en modo salida 

      sensorTemperatura.requestTemperatures();                //Enviamos el comando para obtener la temperatura
      Serial.print(sensorTemperatura.getTempCByIndex(0));     // Avisa al sensor que vamos a leer
      temperaturaSensor=sensorTemperatura.getTempCByIndex(0); // Tomamos la temperatura del indice 0 
      Serial.println(" °C");
      controlTemperatura();
    }
    
void loop(){
       if(millis() >= tiempo + 1000){   // se crea un contador de tiempo para poder medirlo y controlarlo
          segundos ++;                  // contador para usar en la funcion contador
          contador2++;                  // contador para usar el la funcion volteo
          tiempo = millis();
          contador();                   // se llama a la funcion contador
          volteo();                     // se llama a la funcion volteo
          controlTemperatura();         // se llama a la funcion controlTemperatura
          controlHumedad();
          nacedora();
       }       
  }

  void contador(){                  // FUNCION CONTADOR -> convierte el contador en formato dias, horas...
    if(segundos == 60){
               segundos = 0;        // pasa de segundos a minutos
               minutos ++; 
             }
             if(minutos == 60){
               minutos = 0;         // pasa de minutos a horas
               horas ++;
             }   
             if(horas == 24){
               horas = 0;           // pasa de horas a dias
               dias ++;           
       }
      fecha();
      porPantalla();
    }
//-------------------------------------FUNCION PARA VER EL TIEMPO---------------------------------------     
  void fecha(){            
    Serial.print (dias);            // imprime el tiempo transcurrido desde que se encendio el programa
    Serial.print (":");
    Serial.print (horas);
    Serial.print (":");
    Serial.print (minutos);
    Serial.print (":");
    Serial.println (segundos);
   }
//-------------------------------------FUNCION PARA CONTROLAR EL VOLTEO----------------------------------  
  void volteo(){                        
    if(contador2 >= 8){
        digitalWrite (motor, HIGH);
        lcd.setCursor (14, 0);
        lcd.print("on");
        if(contador2 >= 12){             //gestiona el volteo cada 4 horas y durante 6 segundos         
        digitalWrite (motor, LOW);
        lcd.setCursor (14, 0);
        lcd.print("  "); 
        contador2 = 0; 
      }
    }
  }
//-------------------------------------FUNCION PARA CONTROLAR LA TEMPERATURA----------------------------------            
  void controlTemperatura(){
    if(temperaturaSensor <= temperaturaIncubacion){
          digitalWrite(resistencia, HIGH);
    }else{
          digitalWrite(resistencia, LOW);   
      }
                           
      lcd.setCursor (0, 1);
      lcd.print("t");
      lcd.setCursor (1, 1);             
      lcd.print(temperaturaSensor);
      
    }
   
//-------------------------------------FUNCION PARA CONTROLAR LA HUMEDAD----------------------------------        
  void controlHumedad(){
    humedadSensor = sensorHumedad.readHumidity();
  Serial.println(humedadSensor);
  if(humedadSensor <= humedadIncubacion){
    digitalWrite(humidificador, HIGH);
    }else {
      digitalWrite(humidificador, LOW);
      }
      lcd.setCursor (12, 1);
      lcd.print("h");
      lcd.setCursor (13, 1);             
      lcd.print(humedadSensor);
      lcd.setCursor (15, 1);
      lcd.print("%");
    }
     
 //----------------------------------- FUNCION PARA VER DATOS POR LCD--------------------------------------                    
  void porPantalla(){
  lcd.clear();             // limpia la pantalla         
  lcd.setCursor (9, 0);    // coloca el cursor         
  lcd.print(segundos);     // escribe en pantalla    
  lcd.setCursor (8, 0);
  lcd.print(":"); 
  lcd.setCursor (6, 0);             
  lcd.print(minutos);         
  lcd.setCursor (5, 0);
  lcd.print (":"); 
  lcd.setCursor (3, 0);             
  lcd.print(horas);       
  lcd.setCursor (2, 0);
  lcd.print (":"); 
  lcd.setCursor (0, 0);            
  lcd.print(dias);        
  
}  
//----------------------------------- FUNCION PARA CAMBIAR PARAMETROS-------------------------------------- 
 void nacedora(){
  if(dias == 19){
    temperaturaIncubacion = 37.2;
    humedadIncubacion= 70;
    contador2=0;
  }if(dias == 20){
    temperaturaIncubacion = 36.7;
    humedadIncubacion= 75;
    contador2=0;
  }if(dias == 21){
    temperaturaIncubacion = 36.3;
    humedadIncubacion= 80;
    contador2=0;
  }if(dias >= 20){
    temperaturaIncubacion = 35,8;
    humedadIncubacion= 85;
    contador2=0;
  }













  
 }

  

#include <Servo.h>  // Incluye la librería para controlar el servomotor
#include <Adafruit_NeoPixel.h>  // Incluye la librería para controlar los LEDs WS2812

const int tempPin = A0;  // Pin al que está conectado el sensor de temperatura
const int gasPin = A1;   // Pin al que está conectado el sensor de gas
const int piezoPin = 11; // Pin al que está conectado el piezoeléctrico
const int servoPin = 9;  // Pin al que está conectado el servomotor

const int ledPin = 6;   // Pin al que está conectado el pin de datos de la tira de LEDs WS2812
const int numLEDs = 8;  // Número de LEDs en la tira WS2812

const int gasThreshold = 400; // Umbral de detección de gas (ajusta según tu sensor)

Servo myServo;  // Crea un objeto servo para controlar el servomotor
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLEDs, ledPin, NEO_GRB + NEO_KHZ800);  // Crea un objeto para controlar la tira de LEDs WS2812

void setup() {
  Serial.begin(9600);  // Inicia la comunicación serie a 9600 baudios
  myServo.attach(servoPin);  // Asigna el pin del servomotor al objeto servo
  myServo.write(0);  // Coloca el servomotor en la posición inicial (0 grados)
  pinMode(piezoPin, OUTPUT);  // Configura el pin del piezoeléctrico como salida

  strip.begin();  // Inicializa la tira de LEDs WS2812
  strip.show();  // Apaga todos los LEDs al inicio
}

void loop() {
  // Lectura y procesamiento del sensor de temperatura
  int tempAnalogValue = analogRead(tempPin);  // Lee el valor analógico del sensor de temperatura
  float tempVoltage = tempAnalogValue * (5.0 / 1023.0);  // Convierte el valor analógico a voltaje
  float temperatureC = tempVoltage * 100;  // Convierte el voltaje a temperatura en grados Celsius
  
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" C");

  // Lectura y procesamiento del sensor de gas
  int gasAnalogValue = analogRead(gasPin);  // Lee el valor analógico del sensor de gas
  
  Serial.print("Gas level: ");
  Serial.println(gasAnalogValue);

  // Activa el piezoeléctrico y los LEDs si se detecta gas y la temperatura supera los 40°C
  if (temperatureC > 40 && gasAnalogValue > gasThreshold) {
    digitalWrite(piezoPin, HIGH);  // Enciende el piezoeléctrico
    for (int i = 0; i < numLEDs; i++) {
      strip.setPixelColor(i, strip.Color(255, 0, 0));  // Establece el color del LED en rojo
      strip.show();  // Actualiza la tira de LEDs
      delay(250);  // Espera 250 ms antes de encender el siguiente LED
    }
    myServo.write(90);  // Mueve el servomotor a 90 grados
  } else {
    digitalWrite(piezoPin, LOW);  // Apaga el piezoeléctrico
    for (int i = 0; i < numLEDs; i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));  // Apaga el LED
      strip.show();  // Actualiza la tira de LEDs
      delay(250);  // Espera 250 ms antes de apagar el siguiente LED
    }
    myServo.write(0);  // Coloca el servomotor en la posición inicial (0 grados)
  }

  delay(1000);  // Espera 1 segundo antes de la siguiente lectura
}

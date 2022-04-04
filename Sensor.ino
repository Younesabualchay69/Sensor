
#include <Ultrasonic.h>
#include <Adafruit_NeoPixel.h>

// Definerar ett object för varje ultrasonic sensor
Ultrasonic Blis_Left(8, 9); // pinnar för sensorn
Ultrasonic Blis_Right(5, 6); // pinnar för sensorn
Ultrasonic ultrasonic(12, 13); // pinnar för sensorn
int distance;
const int led = 7;
const int buzzer = 10;




//Definiera NeoPixel Pin och antal LED's
#define PIN 4
#define NUM_LEDS 24

//create a NeoPixel strip // Kod exempel
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

//här anropar jag strip.begin(); och  strip.show(); för att starta neopixel ringen
void setup() {
  Serial.begin(9600); // Startar kommunikation med datorn med en hastighet på 9600
  pinMode(led, OUTPUT); // initierar led pinnen
  strip.begin(); // initierar strip pinnarna 
  strip.show(); // skickar data till led stripen 

}

void loop() {
  // Här anropar jag min funktion för back sensorn, BLIS höger och BLIS vänster
  ultra_rear();
  Blis_L();
  Blis_R();


}

void ultra_rear() {
  // Få avståndet
  distance = ultrasonic.read();
  //skriva ut avståndet i terminalen med en delay på 300ms för att det inte ska lagga
  Serial.print("Distance in CM: ");
  Serial.println(distance);
  delay(300);

  //när sensorn upptäcker att den är i inom 100 cm då börjar den pipa långsamt
  if (distance > 100) {
    digitalWrite(led, LOW);
    noTone(buzzer);
  }

  // mellan 50 och 100 lite fortare pip
  if (50 < distance && distance <= 100) {
    digitalWrite(led, HIGH);
    tone(buzzer, 1000);
    delay(300);

    digitalWrite(led, LOW);
    noTone(buzzer);
    delay(distance * 4);


  }
  // mellan 25 och 50, ännu fortare
  if (25 < distance && distance <= 50) {
    digitalWrite(led, HIGH);
    tone(buzzer, 1000);
    delay(150);

    digitalWrite(led, LOW);
    noTone(buzzer);
    delay(distance * 3);


  }

  // mellan 13 och 25 Fortast
  if (13 < distance && distance <= 25) {
    digitalWrite(led, HIGH);
    tone(buzzer, 1000);
    delay(75);

    digitalWrite(led, LOW);
    noTone(buzzer);
    delay(distance * 2);


  }

  // här tjuter den konstant för att varna föraren att inte backa mer
  if (0 < distance && distance <= 13) {
    digitalWrite(led, HIGH);
    tone(buzzer, 1000);
  }
}



void Blis_L() {
  // Få avståndet
  distance = Blis_Left.read();

  Serial.print("Distance_2 in CM: ");
  Serial.println(distance);
  delay(300);

  //om avståndet är mer än 50 då ska alla leds på vänster sida vara av
  if (distance > 50) {
    for (int i = 0; i < NUM_LEDS / 2; i++) {
      strip.setPixelColor (i, 0, 0, 0);
      strip.show();
    }
  }

  // varje incriment på definitionsmängden är uppdelat för att representeras av 12 LED lampor var
  if (distance <= 50 && distance > 0) {

    //Cler neo gör så att alla lampor startas om inför varje avläsning så att när objektet går längre ur sikt så kan lamporna släckas i förhållande till det nya avståndet
    clear_neo(0, NUM_LEDS);
    for (int i = 0; i < map(distance, 50, 0, 0, NUM_LEDS / 2 ); i++) {
      strip.setPixelColor (i, 255, 35, 0);
      strip.show();
    }


  }
}
void Blis_R() {
  // Få avståndet
  distance = Blis_Right.read();

  Serial.print("Distance_Right in CM: ");
  Serial.println(distance);
  delay(300);

  //om avståndet är mer än 50 då ska alla leds på vänster sida vara av
  if (distance > 50) {
    // varje incriment på definitionsmängden är uppdelat för att representeras av 12 LED lampor var

    for (int i = 0; i < NUM_LEDS / 2; i++) {
      strip.setPixelColor (23 - i, 0, 0, 0);
      strip.show();
    }
  }

  // varje incriment på definitionsmängden är uppdelat för att representeras av 12 LED lampor var
  if (distance <= 50 && distance > 0) {

    clear_neo(NUM_LEDS / 2, NUM_LEDS);
    for (int i = 0; i < map(distance, 50, 0, 0, NUM_LEDS / 2 ); i++) {
      strip.setPixelColor (23 - i, 255, 35, 0);
      strip.show();
    }


  }
}

//Stänger av alla lampor
void clear_neo(int min, int max) {
  for (int i = min; i < max; i++)
    strip.setPixelColor(i, 0, 0, 0);
  strip.show();
}

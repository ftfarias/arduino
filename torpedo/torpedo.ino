#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

# define FLOOD_BUTTON 11
# define FLOOD_LED_RED A0
# define FLOOD_LED_GREEN A1
# define FLOOD_LED_BLUE 13

# define EQ_BUTTON 10
# define EQ_LED_RED A3
# define EQ_LED_GREEN A2
# define EQ_LED_BLUE 8

# define MUZZLE_BUTTON 9
# define MUZZLE_LED_RED 7
# define MUZZLE_LED_GREEN 3
# define MUZZLE_LED_BLUE 2

# define BUTTON_FIRE 4
# define BUTTON_UP 6
# define BUTTON_DOWN 5

unsigned long blink_timer = millis();
unsigned long action_timer = millis();

# define BLINKING_INTERVAL 600
# define ACTION_INTERVAL 200

# define TIME_TO_FLOOD 33
# define TIME_TO_EQ 21
# define TIME_TO_MUZZLE 18

byte flood_counter = 0;
byte eq_counter = 0;
byte muzzle_counter = 0;

static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000
};

int blink = LOW;
bool action = false;
#define ACTION_NONE 0
#define ACTION_FLOODING 1
#define ACTION_FLOODING_R 2
#define ACTION_EQ 3
#define ACTION_EQ_R 4
#define ACTION_MUZZLE 5
#define ACTION_MUZZLE_R 6

void setup() {
  pinMode(FLOOD_BUTTON, INPUT_PULLUP);
  pinMode(FLOOD_LED_RED, OUTPUT);
  pinMode(FLOOD_LED_GREEN, OUTPUT);
  pinMode(FLOOD_LED_BLUE, OUTPUT);

  pinMode(EQ_BUTTON, INPUT_PULLUP);
  pinMode(EQ_LED_RED, OUTPUT);
  pinMode(EQ_LED_GREEN, OUTPUT);
  pinMode(EQ_LED_BLUE, OUTPUT);

  pinMode(MUZZLE_BUTTON, INPUT_PULLUP);
  pinMode(MUZZLE_LED_RED, OUTPUT);
  pinMode(MUZZLE_LED_GREEN, OUTPUT);
  pinMode(MUZZLE_LED_BLUE, OUTPUT);

  pinMode(BUTTON_FIRE, INPUT_PULLUP);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);

  Serial.begin(9600);

  display.setRotation(2);


  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds
  // Clear the buffer
  display.clearDisplay();
  testdrawchar();

}


void loop() {
  int action_type = 0;
  bool flood_button = (digitalRead(FLOOD_BUTTON) == LOW);
  bool eq_button = (digitalRead(EQ_BUTTON) == LOW);
  bool muzzle_button = (digitalRead(MUZZLE_BUTTON) == LOW);
  bool fire_button = (digitalRead(BUTTON_FIRE) == LOW);
  bool up_button = (digitalRead(BUTTON_UP) == LOW);
  bool down_button = (digitalRead(BUTTON_DOWN) == LOW);



  if (flood_button) {
    // up - fill tube
    if (flood_counter < TIME_TO_FLOOD) {
      action_type = ACTION_FLOODING;
      if (action) {
        flood_counter += 1;
        action = false;
      };
    }
  } else {
    // down
    if ((flood_counter > 0) && (eq_counter == 0)) {
      action_type = ACTION_FLOODING_R;
      if (action) {
        flood_counter -= 1;
        action = false;
      };
    }
  }

  if (eq_button) {
    // up - fill tube
    if ((eq_counter < TIME_TO_EQ) &&
        (flood_counter == TIME_TO_FLOOD)) {
      action_type = ACTION_EQ;

      if (action) {
        eq_counter += 1;
        action = false;
      };
    }
  } else {
    // down
    if ((eq_counter > 0) && (muzzle_counter == 0)) {
      action_type = ACTION_EQ_R;

      if (action) {
        eq_counter -= 1;
        action = false;
      };
    }
  }


  if (muzzle_button) {
    // up
    if ((muzzle_counter < TIME_TO_MUZZLE) &&
        (eq_counter == TIME_TO_EQ)) {
      action_type = ACTION_MUZZLE;
      if (action) {
        muzzle_counter += 1;
        action = false;
      };
    }
  } else {
    // down
    if ((muzzle_counter > 0)) {
      action_type = ACTION_MUZZLE_R;
      if (action) {
        muzzle_counter -= 1;
        action = false;
      };
    }
  }

  if (flood_counter == 0) {
    digitalWrite(FLOOD_LED_RED, HIGH);
    digitalWrite(FLOOD_LED_BLUE, LOW);
    digitalWrite(FLOOD_LED_GREEN, LOW);
  } else if (flood_counter == TIME_TO_FLOOD) {
    digitalWrite(FLOOD_LED_RED, LOW);
    digitalWrite(FLOOD_LED_BLUE, LOW);
    digitalWrite(FLOOD_LED_GREEN, HIGH);
  } else {
    if (action_type == ACTION_FLOODING) {
      digitalWrite(FLOOD_LED_RED, blink);
      digitalWrite(FLOOD_LED_BLUE, LOW);
      digitalWrite(FLOOD_LED_GREEN, LOW);
    } else {
      digitalWrite(FLOOD_LED_RED, LOW);
      digitalWrite(FLOOD_LED_BLUE, blink);
      digitalWrite(FLOOD_LED_GREEN, LOW);
    }
  }


  if (eq_counter == 0) {
    digitalWrite(EQ_LED_RED, HIGH);
    digitalWrite(EQ_LED_BLUE, LOW);
    digitalWrite(EQ_LED_GREEN, LOW);
  } else if (eq_counter == TIME_TO_EQ) {
    digitalWrite(EQ_LED_RED, LOW);
    digitalWrite(EQ_LED_BLUE, LOW);
    digitalWrite(EQ_LED_GREEN, HIGH);
  } else {
    if (action_type == ACTION_EQ) {
      digitalWrite(EQ_LED_BLUE, LOW);
      digitalWrite(EQ_LED_RED, blink);
      digitalWrite(EQ_LED_GREEN, LOW);
    } else {
      digitalWrite(EQ_LED_BLUE, blink);
      digitalWrite(EQ_LED_RED, LOW);
      digitalWrite(EQ_LED_GREEN, LOW);
    }
  }

  if (muzzle_counter == 0) {
    digitalWrite(MUZZLE_LED_RED, HIGH);
    digitalWrite(MUZZLE_LED_BLUE, LOW);
    digitalWrite(MUZZLE_LED_GREEN, LOW);
  } else if (muzzle_counter == TIME_TO_MUZZLE) {
    digitalWrite(MUZZLE_LED_RED, LOW);
    digitalWrite(MUZZLE_LED_BLUE, LOW);
    digitalWrite(MUZZLE_LED_GREEN, HIGH);
  } else {
    digitalWrite(MUZZLE_LED_RED, blink);
    digitalWrite(MUZZLE_LED_BLUE, LOW);
    digitalWrite(MUZZLE_LED_GREEN, LOW);
  }


  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setTextWrap(false);
  display.setCursor(0, 0);     // Start at top-left corner

  if (flood_counter == 0) {
    display.println("Loaded and dry");
  } else if (action_type == ACTION_FLOODING) {
    display.print(F("Flooding tube: "));
    display.print(String(flood_counter * 100 / TIME_TO_FLOOD));
    display.println(F("%"));
  } else if (action_type == ACTION_FLOODING_R) {
    display.print(F("Draining tube: "));
    display.print(String(flood_counter * 100 / TIME_TO_FLOOD));
    display.println(F("%"));
  } else if ((flood_counter == TIME_TO_FLOOD) &&
             (eq_counter == 0)) {
    display.println("Flooded");
  } else if ((flood_counter == TIME_TO_FLOOD) &&
             (eq_counter > 0) &&
             (eq_counter < TIME_TO_EQ)) {
    display.print(F("Equalizing: "));
    display.print(String(eq_counter * 100 / TIME_TO_EQ));
    display.println(F("%"));
  } else if ((flood_counter == TIME_TO_FLOOD) &&
             (eq_counter == TIME_TO_EQ)) {
    display.println(F("Flood and equalized"));
  }

  if (muzzle_counter == 0) {
    display.println("Muzzled door close");
  } else if (muzzle_counter == TIME_TO_MUZZLE ) {
    display.println("Muzzled door open");
  } else {
    display.print(F("Opening door: "));
    display.print(String(muzzle_counter * 100 / TIME_TO_MUZZLE));
    display.println(F("%"));
  }


  //  display.print(F("Water level: "));
  //  display.print(String(flood_counter * 10));
  //  display.println(F("%"));
  //  display.print(F("Eq level: "));
  //  display.print(String(eq_counter * 10));
  //  display.println(F("%"));
  //  display.print(F("Muzzle level: "));
  //  display.print(String(muzzle_counter * 10));
  //  display.println(F("%"));

  display.display();

  if (millis() - blink_timer > BLINKING_INTERVAL) {
    blink_timer = millis();
    if (blink == LOW) {
      blink = HIGH;
    } else {
      blink = LOW;
    }
  }

  if (millis() - blink_timer > ACTION_INTERVAL) {
    action_timer = millis();
    action = true;
  }

  delay(100);
}



void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for (int16_t i = 0; i < 256; i++) {
    if (i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

void testdrawstyles(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 0);            // Start at top-left corner
  display.println(F("Hello, world!"));

  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  display.println(3.141592);

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.print(F("0x")); display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}

void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("scroll"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}

void testdrawbitmap(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2

void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  int8_t f, icons[NUMFLAKES][3];

  // Initialize 'snowflake' positions
  for (f = 0; f < NUMFLAKES; f++) {
    icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
    icons[f][YPOS]   = -LOGO_HEIGHT;
    icons[f][DELTAY] = random(1, 6);
    Serial.print(F("x: "));
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(F(" y: "));
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(F(" dy: "));
    Serial.println(icons[f][DELTAY], DEC);
  }

  for (;;) { // Loop forever...
    display.clearDisplay(); // Clear the display buffer

    // Draw each snowflake:
    for (f = 0; f < NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SSD1306_WHITE);
    }

    display.display(); // Show the display buffer on the screen
    delay(200);        // Pause for 1/10 second

    // Then update coordinates of each flake...
    for (f = 0; f < NUMFLAKES; f++) {
      icons[f][YPOS] += icons[f][DELTAY];
      // If snowflake is off the bottom of the screen...
      if (icons[f][YPOS] >= display.height()) {
        // Reinitialize to a random position, just off the top
        icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
        icons[f][YPOS]   = -LOGO_HEIGHT;
        icons[f][DELTAY] = random(1, 6);
      }
    }
  }
}

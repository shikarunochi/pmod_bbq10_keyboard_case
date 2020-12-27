#include <BBQ10Keyboard.h>
#include <M5Stack.h>
BBQ10Keyboard keyboard;

#define CFG_OVERFLOW_ON  (1 << 0)
#define CFG_OVERFLOW_INT (1 << 1)
#define CFG_CAPSLOCK_INT (1 << 2)
#define CFG_NUMLOCK_INT  (1 << 3)
#define CFG_KEY_INT      (1 << 4)
#define CFG_PANIC_INT    (1 << 5)
#define CFG_REPORT_MODS    (1 << 6)
#define CFG_USE_MODS    (1 << 7)

#define _REG_CFG 2

#define KEY_ALT    0x11
#define KEY_SHIFT   0x12
#define KEY_SYM   0x13

bool alt = false;
bool shift = false;
bool sym = false;

void setup()
{
  M5.begin();
  M5.Lcd.setTextFont(4);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setRotation(3);
  keyboard.begin();
  keyboard.setBacklight(0.5f);

  keyboard.writeRegister(_REG_CFG ,  CFG_OVERFLOW_INT | CFG_KEY_INT | CFG_USE_MODS | CFG_REPORT_MODS);
}

void loop()
{
  const int keyCount = keyboard.keyCount();
  if (keyCount == 0)
    return;

  const BBQ10Keyboard::KeyEvent key = keyboard.keyEvent();
  String state = "pressed";
  if (key.state == BBQ10Keyboard::StateLongPress)
    state = "held down";
  else if (key.state == BBQ10Keyboard::StateRelease){
    state = "released";
    if(key.key != KEY_ALT && key.key != KEY_SHIFT && key.key != KEY_SYM){
      M5.Lcd.print(key.key);
    }
    switch(key.key){
      case KEY_ALT:alt = false;break;
      case KEY_SHIFT:shift = false;break;
      case KEY_SYM:sym = false;break;
    }
  }else if(key.state == BBQ10Keyboard::StatePress){
     switch(key.key){
      case KEY_ALT:alt = true;break;
      case KEY_SHIFT:shift = true;break;
      case KEY_SYM:sym = true;break;
    }
  }

  if(key.key == KEY_ALT || key.key == KEY_SHIFT || key.key == KEY_SYM ){
    redrawStatus();
  }
  Serial.printf("key: '%c' (dec %d, hex %02x) %s\r\n", key.key, key.key, key.key, state.c_str());

  // pressing 'b' turns off the backlight and pressing Shift+b turns it on
  if (key.state == BBQ10Keyboard::StatePress) {
    if (key.key == 'b') {
      keyboard.setBacklight(0);
    } else if (key.key == 'B') {
      keyboard.setBacklight(1.0);
    }
  }
}

void redrawStatus(){
  M5.Lcd.drawChar(0,220,'A', alt?RED:WHITE, BLACK,2);
  M5.Lcd.drawChar(80,220,'S', shift?RED:WHITE, BLACK,2);
  M5.Lcd.drawChar(240,220,'@', sym?RED:WHITE, BLACK,2);

}

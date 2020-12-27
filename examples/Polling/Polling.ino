#include <BBQ10Keyboard.h>
#include <M5Stack.h>
BBQ10Keyboard keyboard;

void setup()
{
  M5.begin();
  M5.Lcd.setTextFont(4);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setRotation(3);
  keyboard.begin();
  keyboard.setBacklight(0.5f);
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
    M5.Lcd.print(key.key);
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

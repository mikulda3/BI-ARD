#include <Esplora.h>
#include <Mouse.h>
#include <SPI.h>
#include <TFT.h>

// inicializace mysi a displeje
void setup(void) {
  Mouse.begin();
  EsploraTFT.begin();
  set_display();
}

// globalni promenne pro zapamatovani si stavu MMB a casu
int oldScrollUp = 0;
int oldScrollDown = 0;
int stateTime = 0;
int threshold = 20;
// funkce pro nastaveni displeje
void set_display() {
  EsploraTFT.background(0, 0, 0);
  EsploraTFT.stroke(0, 255, 140);
  EsploraTFT.setTextSize(2);
  EsploraTFT.text("ESPLORA", 43, 15);
  EsploraTFT.text("CONTROLLER", 25, 35);
  EsploraTFT.fill(0, 255, 140);
  EsploraTFT.stroke(255, 255, 255); // outline the rectangle with a white line
  EsploraTFT.rect(0, 90, EsploraTFT.width(), 20);
  EsploraTFT.setTextSize(1);
  EsploraTFT.stroke(0, 255, 140);
  EsploraTFT.text("Enjoy Your Experience", 22, 55);
  EsploraTFT.stroke(0, 0, 0);
  EsploraTFT.text("CONNECTED", 55, 97);
}

// hlavni cast programu
void loop(void) {
  // zjisteni pozice joysticku
  int x = Esplora.readJoystickX();
  int y = Esplora.readJoystickY();

  // leve tlacitko na Esplore nebo stisknuti joysticku = LMB
  int leftButton = Esplora.readJoystickSwitch() && Esplora.readButton(SWITCH_LEFT);
  // prave tlacitko na Esplore = RMB
  int rightButton = Esplora.readButton(SWITCH_RIGHT);

  // inicializace skrolovani pomoci tlacitka nahoru a tlacitka dolu
  int scrollUp = Esplora.readButton(SWITCH_UP);
  int scrollDown = Esplora.readButton(SWITCH_DOWN);

  // zjisteni pozice slideru pro ovladani citlivosti joysticku
  int sensitivity = Esplora.readSlider();

  // nastaveni pohybu mysi
  int mouseSensitivity = map(sensitivity, 0, 1023, 2, 10);
  int mouseX, mouseY;
  if ( x < threshold && x > -threshold) {
    mouseX = 0;
  } else {
    mouseX = map(x, -512, 512, mouseSensitivity, - mouseSensitivity);
  }
  if ( y < threshold && y > -threshold) {
    mouseY = 0;
  } else {
    mouseY = map(y, -512, 512, -mouseSensitivity, mouseSensitivity);
  }

  // ovladani kolecka mysi(MMB) pomoci tlacitka nahoru a dolu
  int wheel = 0;
  if (oldScrollUp != scrollUp || oldScrollDown != scrollDown) {
    if (scrollUp != scrollDown) {
      if (scrollUp) {
        wheel = -mouseSensitivity;
      } else {
        wheel = mouseSensitivity;
      }
    } else {
      wheel = 0;
    }
    oldScrollUp = scrollUp;
    oldScrollDown = scrollDown;
    stateTime = millis();
  } else if ((millis() - stateTime) % 100 == 0 && (millis() - stateTime) >= 1000) {
    if (scrollUp != scrollDown) {
      if (scrollUp) {
        wheel = -mouseSensitivity;
      } else {
        wheel = mouseSensitivity;
      }
    } else {
      wheel = 0;
    }
  }

  // pohyb mysi
  Mouse.move(mouseX, mouseY, wheel);

  // LMB
  if (!leftButton) {
    Mouse.press(MOUSE_LEFT);
  } else {
    Mouse.release(MOUSE_LEFT);
  }
  // RMB
  if (!rightButton) {
    Mouse.press(MOUSE_RIGHT);
  } else {
    Mouse.release(MOUSE_RIGHT);
  }
}

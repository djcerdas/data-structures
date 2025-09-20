/*
  Academic purpose:
  - Learn the stack (LIFO) concept in Data Structures.
  - Practice C++ programming with Arduino Playground.

  Hardware:
  - Adafruit Circuit Playground Express (10 onboard NeoPixels).

  Behavior:
  - Stack implemented with an array of size 10; top starts at -1 (empty).
  - Button A (left): PUSH (adds value to stack).
  - Button B (right): POP  (removes value from stack).
  - PUSH lights next LED blue and prints stack content.
  - POP turns off top LED and prints stack content.
  - If FULL on PUSH → all LEDs turn RED + error.
  - If EMPTY on POP → error message.
*/

#include <Adafruit_CircuitPlayground.h>

// -------------------
// 1) Stack definition
// -------------------
const int CAPACITY = 10;     // maximum size of the stack
int stack[CAPACITY];         // array to hold stack elements
int top = -1;                // top of stack (-1 means empty)

// -------------------
// 2) Helper functions
// -------------------
bool isEmpty() { return top == -1; }                 
bool isFull()  { return top == CAPACITY - 1; }       

// Print all items in the stack (bottom -> top)
void printStack() {
  Serial.print("Stack: [ ");
  for (int i = 0; i <= top; i++) {
    Serial.print(stack[i]);
    if (i < top) Serial.print(", ");
  }
  Serial.println(" ]");
}

// Show stack state on NeoPixels
void showStackLEDs() {
  // Clear all
  for (int i = 0; i < CAPACITY; i++) {
    CircuitPlayground.setPixelColor(i, 0, 0, 0);
  }
  // Light up used positions
  for (int i = 0; i <= top; i++) {
    CircuitPlayground.setPixelColor(i, 80, 160, 255); // light blue
  }
}

// -------------------
// 3) PUSH operation
// -------------------
void push(int value) {
  if (isFull()) {
    // Full -> turn all LEDs red
    for (int i = 0; i < CAPACITY; i++) {
      CircuitPlayground.setPixelColor(i, 255, 0, 0);
    }
    Serial.println("ERROR: Stack is FULL");
    CircuitPlayground.playTone(220, 300);
  } else {
    top++;
    stack[top] = value;
    Serial.print("PUSH -> ");
    Serial.println(value+1);
    showStackLEDs();
    printStack();
    CircuitPlayground.playTone(880, 100); // confirmation beep
  }
}

// -------------------
// 4) POP operation
// -------------------
void pop() {
  if (isEmpty()) {
    Serial.println("ERROR: Stack is EMPTY");
    CircuitPlayground.playTone(400, 150);
  } else {
    int value = stack[top];
    Serial.print("POP -> ");
    Serial.println(value);
    top--;
    showStackLEDs();
    printStack();
    CircuitPlayground.playTone(660, 100); // confirmation beep
  }
}

// -------------------
// 5) Arduino setup()
// -------------------
void setup() {
  CircuitPlayground.begin();
  CircuitPlayground.setBrightness(40);
  Serial.begin(9600);        // open Serial Monitor
  Serial.println("Initial stack: EMPTY");
}

// -------------------
// 6) Arduino loop()
// -------------------
void loop() {
  // Button A = PUSH
  if (CircuitPlayground.leftButton()) {
    push(top + 1);  // simple value (0,1,2,...)
    delay(300);     // debounce
  }

  // Button B = POP
  if (CircuitPlayground.rightButton()) {
    pop();
    delay(300);     // debounce
  }
}
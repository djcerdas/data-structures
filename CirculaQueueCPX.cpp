/*
  Circular Queue on (CPX) 

  Hardware:
  Adafruit Circuit Playground Express (Arduino)

  Goal
    - Understand how circular queue (FIFO) works on CPX hardware.
    - Clarify that if we try to insert when the queue is FULL, it could cause buffer overflow.

  Controls
    - Button A (left): insert one value
    - Button B (right): remove one value

  Visuals
    - Soft Amber LEDs = items in the queue
    - All RED         = queue is full (insert blocked)
*/

#include <Adafruit_CircuitPlayground.h>

// ---------- Queue Model ----------
struct CircularQueue {
  static const int CAP = 10;   // CPX has 10 NeoPixels (MAX size)
  int data[CAP];               // fixed storage
  int front = 0;               // index of first element
  int rear  = 0;               // index where next insert goes
  int size  = 0;               // number of elements (0..CAP)

  bool isEmpty() const { return size == 0; }
  bool isFull()  const { return size == CAP; }

  // Move forward one step and wrap to 0 if needed
  int nextIndex(int i) const {
    i += 1;
    if (i >= CAP) i = 0;
    return i;
  }

  // Insert at rear
  bool insertQ(int value) {
    if (isFull()) return false;  // Block to prevent overflow
    data[rear] = value;
    rear = nextIndex(rear);
    size++;
    return true;
  }

  // Remove at front
  bool removeQ(int &removed) {
    if (isEmpty()) return false;
    removed = data[front];
    front = nextIndex(front);
    size--;
    return true;
  }
};

// ---------- Device / UI ----------
CircularQueue Q;      // our queue instance
int nextValue = 0;    // demo value to insert

// Softer Amber color (dimmed)
const uint8_t AMBER_R = 120;
const uint8_t AMBER_G = 60;
const uint8_t AMBER_B = 0;

void clearAll() {
  for (int i = 0; i < CircularQueue::CAP; ++i) {
    CircuitPlayground.setPixelColor(i, 0, 0, 0);
  }
}

void showQueueLEDs() {
  clearAll();
  int pos = Q.front;
  for (int k = 0; k < Q.size; ++k) {
    CircuitPlayground.setPixelColor(pos, AMBER_R, AMBER_G, AMBER_B);
    pos = Q.nextIndex(pos);
  }
}

void drawFullError() {
  for (int i = 0; i < CircularQueue::CAP; ++i) {
    CircuitPlayground.setPixelColor(i, 255, 0, 0);
  }
}

// Print queue with details to console
void printQueue() {
  Serial.print("Queue size=");
  Serial.print(Q.size);
  Serial.print(" | front front=");
  Serial.print(Q.front);
  Serial.print(" | rear rear=");
  Serial.print(Q.rear);
  Serial.print(" | elements=[");
  if (!Q.isEmpty()) {
    int pos = Q.front;
    for (int k = 0; k < Q.size; ++k) {
      Serial.print(Q.data[pos]);
      if (k < Q.size - 1) Serial.print(", ");
      pos = Q.nextIndex(pos);
    }
  }
  Serial.println("]");
}

void setup() {
  CircuitPlayground.begin();
  CircuitPlayground.setBrightness(20); // dim brightness
  Serial.begin(9600);
  Serial.println("Circular Queue: ready");
  showQueueLEDs();
}

void loop() {
  // Insert on Button A (left)
  if (CircuitPlayground.leftButton()) {
    if (Q.insertQ(nextValue++)) {
      Serial.println("Action: INSERT a new value at REAR");
      CircuitPlayground.playTone(880, 90);
      showQueueLEDs();
      printQueue();
    } else {
      drawFullError();
      Serial.println("Action: INSERT blocked — Queue FULL (MAX reached, prevent buffer overflow)");
      CircuitPlayground.playTone(220, 220);
    }
    delay(280); // simple debounce
  }

  // Remove on Button B (right)
  if (CircuitPlayground.rightButton()) {
    int removed = 0;
    if (Q.removeQ(removed)) {
      Serial.print("Action: REMOVE value from FRONT -> ");
      Serial.println(removed);
      CircuitPlayground.playTone(660, 90);
      showQueueLEDs();
      printQueue();
    } else {
      Serial.println("Action: REMOVE blocked — Queue EMPTY");
      CircuitPlayground.playTone(220, 220);
    }
    delay(280); // simple debounce
  }
}

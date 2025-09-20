/*
  Academic purpose:
  - Learn the queue (FIFO) concept in Data Structures.
  - Practice C++ programming with Arduino Playground.

  Hardware:
  - Adafruit Circuit Playground Express (10 onboard NeoPixels).

  Behavior (Simple Queue - array, movable front):
  - Button A (left): insertQ → add an element at the REAR (end).
  - Button B (right): removeQ → remove an element from the FRONT.
  - insertQ lights the next LED GREEN and prints the queue.
  - removeQ turns off the FRONT LED and prints the queue.
  - If FULL on insertQ → all LEDs RED + error tone.
  - If EMPTY on removeQ → error message + tone.
*/

#include <Adafruit_CircuitPlayground.h>

// ===================================================
// 1) Queue definition
// ===================================================

// Maximum number of elements that the queue can hold
const int CAPACITY = 10;

// Array that stores the elements of the queue
int queueArr[CAPACITY];

// Index of the front element (the one that will be removed next)
int frontIdx = 0;

// Index of the last inserted element (rear of the queue)
int rearIdx  = -1;

// Counter to generate simple demo values (0, 1, 2, ...)
int nextValue = 0;


// ===================================================
// 2) Queue status helpers
// ===================================================

// isEmpty: returns true if the queue has no elements
// Condition: front has moved past rear
bool isEmpty() {
  return frontIdx > rearIdx;
}

// isFull: returns true if the queue has no more space
// Condition: rear has reached the last position in the array
bool isFull() {
  return rearIdx == CAPACITY - 1;
}


// ===================================================
// 3) Display helpers (LEDs + Serial)
// ===================================================

// Turn OFF all LEDs
void clearAllPixels() {
  for (int i = 0; i < CAPACITY; i++) {
    CircuitPlayground.setPixelColor(i, 0, 0, 0); // RGB = 0,0,0 → OFF
  }
}

// Show the current state of the queue using LEDs
// LEDs from "front" to "rear" will be lit GREEN
void showQueueLEDs() {
  clearAllPixels();
  if (!isEmpty()) {
    for (int i = frontIdx; i <= rearIdx; i++) {
      CircuitPlayground.setPixelColor(i, 0, 200, 80); // RGB → GREEN
    }
  }
}

// Print the contents of the queue in the Serial Monitor
void printQueue() {
  Serial.print("Queue: [ ");
  if (!isEmpty()) {
    for (int i = frontIdx; i <= rearIdx; i++) {
      Serial.print(queueArr[i]);   // print each element
      if (i < rearIdx) Serial.print(", ");
    }
  }
  Serial.println(" ]");
}


// ===================================================
// 4) INSERT operation (Insertar)
// ===================================================
// Add an element at the REAR (end) of the queue.
void insertQ(int value) {
  if (isFull()) {
    // Queue is FULL → all LEDs turn RED and play error tone
    for (int i = 0; i < CAPACITY; i++) {
      CircuitPlayground.setPixelColor(i, 255, 0, 0); // RED
    }
    Serial.println("ERROR: Queue is FULL");
    CircuitPlayground.playTone(220, 300); // low tone for error
    return;
  }

  // There is space → advance rear, then insert value
  rearIdx++;
  queueArr[rearIdx] = value;

  // Feedback for the student
  Serial.print("INSERT -> ");
  Serial.println(value);

  showQueueLEDs();   // update LED visualization
  printQueue();      // show contents in Serial
  CircuitPlayground.playTone(880, 100); // high tone = success
}


// ===================================================
// 5) REMOVE operation (Eliminar)
// ===================================================
// Remove the element at the FRONT of the queue.
void removeQ() {
  if (isEmpty()) {
    // Queue is EMPTY → cannot remove
    Serial.println("ERROR: Queue is EMPTY");
    CircuitPlayground.playTone(400, 150); // mid tone for error
    return;
  }

  // Read the value at the front (for printing)
  int value = queueArr[frontIdx];

  // Show which element is being removed
  Serial.print("REMOVE -> ");
  Serial.println(value);

  // Move the front one position forward
  frontIdx++;

  showQueueLEDs();   // update LED visualization
  printQueue();      // show contents in Serial
  CircuitPlayground.playTone(660, 100); // confirmation tone
}


// ===================================================
// 6) Reset helper
// ===================================================
// Reset the indices when the queue becomes empty.
// This keeps the demo simple and avoids running out of array space.
void resetIfEmpty() {
  if (isEmpty()) {
    frontIdx = 0;
    rearIdx  = -1;
  }
}


// ===================================================
// 7) Arduino setup()
// ===================================================
void setup() {
  CircuitPlayground.begin();       // start the Playground
  CircuitPlayground.setBrightness(40); // limit LED brightness
  Serial.begin(9600);              // open Serial Monitor
  Serial.println("Initial queue: EMPTY");
  showQueueLEDs();                 // show empty queue on LEDs
}


// ===================================================
// 8) Arduino loop()
// ===================================================
void loop() {
  // Button A = INSERT
  if (CircuitPlayground.leftButton()) {
    insertQ(nextValue++);  // insert a new demo value
    delay(300);            // debounce delay
  }

  // Button B = REMOVE
  if (CircuitPlayground.rightButton()) {
    removeQ();             // remove the front element
    resetIfEmpty();        // reset indices if queue became empty
    delay(300);            // debounce delay
  }
}

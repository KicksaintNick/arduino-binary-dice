#include <Bounce2.h> // Useful library to help with pushbutton debouncing

// Pin setup
// LED pins
int LED0 = 13;
int LED1 = 12;
int LED2 = 11;

// Misc Pins
int RANDOMBUTTON = 7; // Pin that controls the pushbutton for randomisation
int STARTBUTTON = 6; // Pin that controls the pushbutton to start the game
int BAUDRATE = 9600; // Set the BAUD for serial communication

int DELAY = 20; // Millisecond delay for debounce on buttons

// Store the random value for the game generated when pushing the random button
int randomNum = 0;

// Bounce buttons using the Bounce2 library to simplify bounce concerns
// Read more about Bounce2 here:
// https://github.com/thomasfredericks/Bounce2/wiki
Bounce startButton;
Bounce randomButton;

void setup() {
  // Setting output mode for LED pins
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  // Setting input mode for pushbutton pins
  pinMode(STARTBUTTON, INPUT);
  pinMode(RANDOMBUTTON, INPUT);

  // Setup for the start button, attaching it to the pushbutton pin and setting the button debounce delay
  startButton.attach(STARTBUTTON);
  startButton.interval(DELAY);

  // Setup for the random button, attaching it to the pushbutton pin and setting the button debounce delay
  randomButton.attach(RANDOMBUTTON);
  randomButton.interval(DELAY);

  // Generating a random seed so the randomisation is always unique at each boot
  randomSeed(analogRead(14));

  // Serial monitor is required to display the random number and result of the game
  Serial.begin(BAUDRATE);
}

// Loop function checks both buttons for interactivity
void loop() {
  loop_randomButton();
  loop_startButton();
}

// Random button functionality
void loop_randomButton() {
  if (randomButton.update()) {
    // If the button is pressed, output the result to the LEDs and to the Serial monitor
    if (randomButton.read() == HIGH) {
      randomNum = random(1, 7); // Generate a random number
      led_display(randomNum); // Run the led display function to display the random number in binary on the LEDs
      
      // Display the guess in the Serial monitor
      Serial.print("Random Guess: ");
      Serial.println(randomNum);
    }
  }
}

// Start button functionality
void loop_startButton() {
  if (startButton.update()) {
    // If the button is pressed, generate a random number and display the result on the LEDS and the Serial monitor
    // Also check if the result is the same as the random result and run the player win function to indicate that the player has won
    if (startButton.read() == HIGH) {
      int result = random(1, 7); // Generate a random number between 1 and 7 (Notice that the binary operations in led_display only go up to 7
      led_display(result); // Run the output function to display the result in binary on the LEDs

      // Display the result in the Serial monitor
      Serial.print("Result: ");
      Serial.println(result);

      // Check the result against the random value
      if (randomNum > 0) {
        if (result == randomNum) { // If the result and random match then the player wins
          Serial.println("Player win!"); // Display win status in the Serial Monitor
          playerWins(); // Flash the LEDS using the player win function
        } else { // If the the random number and result don't match then the player loses 
          Serial.println("Player loses!");
        }
      }
      delay(2000); // Delay by 2 seconds to prevent 
      randomNum = 0; // Reset randomNum to 0, this ensures that a player can only attempt to match the randomNum once
    }
  }
}

// Display the resulting valued in binary on the LEDS
// More information on Bitwise AND operations are available here:
// http://playground.arduino.cc/Code/BitMath#bitwise_and
// Converting the result into binary. The & checks the result determining if the binary version is 1 or 0 which sets the LED status accordingly
// Notice the changes to the B001, B010, B100
// The binary values used are:
// B001 = 1; B010 = 2; B011 = 3; B100 = 4; B101 = 5; B110 = 6; B111 = 7; 
// Now you can see that the binary dice goes up to 7 values, you can add more LEDs and increase the random value range to make a more complex dice
void led_display(const long result) {
  digitalWrite(LED0, result & B001);
  digitalWrite(LED1, result & B010);
  digitalWrite(LED2, result & B100);
}

// This function just flashes the lights indicating that the player has won
void playerWins() {
  // Run this function 5 times
  for (unsigned int i = 0; i < 5; i++) {
    led_display(7); // Sets all LEDs on using the output_result function which is B111 meaning that all LEDs are enabled
    delay(100);
    led_display(0); // Turns all the LEDs off
    delay(100);
  }
}

// Imports
// I import express, which allows me to create a web app, as well as GPIO which allows me to interact with the pins on Raspberry PI
const express = require('express'); 
const Gpio = require('onoff').Gpio;

// Variables
// I first initalize some varaibles for the express app, and then I assign the 3 GPIO pins that I will use for the stop light. I also create a isFlashing variable
// which allows me to check to see if the stoplight is flashing and turn it off if it is when the button is pushed
const app = express();
const port = 3000;
let isFlashing = false;
const ledRed = new Gpio(25, 'out');
const ledYellow = new Gpio(8, 'out');
const ledGreen = new Gpio(7, 'out');

// This function is what allows me to turn on and off the LEDs via the writeSync function
function toggleLED(led, value) {
  try {
    led.writeSync(value);
  } catch (error) {
    console.error('Error in toggleLED:', error);
  }
}

// Set the view engine to ejs and add the static path for images
app.set('view engine', 'ejs');
app.use(express.static('public'));


// Define the root route. This is where you are brought when you access the pi's root website
app.get('/', (req, res) => {
  res.render('index');
});

// This path is used to turn on the red LED
app.get('/red', (req,res) => {
  // Turn on and off the LEDs
  console.log('Turning red on')
  toggleLED(ledRed, 1);
  console.log('Turning red off')
  setTimeout(() => toggleLED(ledRed, 0), 2000);
});

// This path is used to turn on the yellow LED
app.get('/yellow', (req,res) => {
  // Turn on and off the LEDs
  console.log('Turning yellow on')
  toggleLED(ledYellow, 1);
  console.log('Turning yellow off')
  setTimeout(() => toggleLED(ledYellow, 0), 2000);

});

// This path is used to turn on the green LED
app.get('/green', (req,res) => {
  // Turn on and off the LEDs
  console.log('Turning green on')
  toggleLED(ledGreen, 1);
  console.log('Turning green off')
  setTimeout(() => toggleLED(ledGreen, 0), 2000);

});

// This path is used to begin the flashing of the stoplight, as it would in an actual stoplight
app.get('/startf', (req, res) => {
  isFlashing = true;
  flashLights();
  console.log('Flashing started');
});

// This function checks to see if the isFlashing variable is false, and if it is (meaning I clicked the turn off flashing button), it calls a function to turn off all the LEDs
function flashLights() {
  if (!isFlashing) {
    turnOffAllLeds();
    return;
  }

  // Start with Green LED
  toggleLED(ledGreen, 1);
  setTimeout(() => { //The setTimeout function allows my code to run after a certain delay, which is specified down bellow
    toggleLED(ledGreen, 0);

    // Then Yellow LED
    if (isFlashing) {
      toggleLED(ledYellow, 1);
      setTimeout(() => {
        toggleLED(ledYellow, 0);

        // Finally, Red LED
        if (isFlashing) {
          toggleLED(ledRed, 1);
          setTimeout(() => {
            toggleLED(ledRed, 0);

            // Repeat the cycle
            if (isFlashing) {
              setTimeout(flashLights, 0); 
            } else {
              turnOffAllLeds();
            }
          }, 5000); // Red duration
        }
      }, 2000); // Yellow duration
    }
  }, 4000); // Green duration
}

// Here is the function to turn off all the LEDs
function turnOffAllLeds() {
  toggleLED(ledRed, 0);
  toggleLED(ledYellow, 0);
  toggleLED(ledGreen, 0);
}

// This is the path that is called when I click the button to stop the flashing, it sets the variable isFlashing to false, which then allows the startf path to stop immediately
app.get('/stopf', (req, res) => {
  isFlashing = false;

  toggleLED(ledRed, 0);
  toggleLED(ledYellow, 0);
  toggleLED(ledGreen, 0);
});

// Start the server
app.listen(port, () => {
  console.log(`Server listening at http://localhost:${port}`);
});

// This code listens for the server to be stopped, and then safetly exports the LEDs from the program
process.on('SIGINT', () => {
  ledRed.unexport();
  ledYellow.unexport();
  ledGreen.unexport();
});

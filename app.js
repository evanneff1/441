const express = require('express');
const app = express();
const port = 3000;

const Gpio = require('onoff').Gpio;

const ledRed = new Gpio(25, 'out');
const ledYellow = new Gpio(8, 'out');
const ledGreen = new Gpio(7, 'out');

function toggleLED(led, value) {
  try {
    led.writeSync(value);
  } catch (error) {
    console.error('Error in toggleLED:', error);
  }
}


let isFlashing = false;

// Set the view engine to ejs
app.set('view engine', 'ejs');
app.use(express.static('public'));


// Define the root route
app.get('/', (req, res) => {
  res.render('index');
});

app.get('/red', (req,res) => {
  // Turn on and off the LEDs
  console.log('Turning red on')
  toggleLED(ledRed, 1);
  console.log('Turning red off')
  setTimeout(() => toggleLED(ledRed, 0), 2000);
});


app.get('/yellow', (req,res) => {
  // Turn on and off the LEDs
  console.log('Turning yellow on')
  toggleLED(ledYellow, 1);
  console.log('Turning yellow off')
  setTimeout(() => toggleLED(ledYellow, 0), 2000);

});


app.get('/green', (req,res) => {
  // Turn on and off the LEDs
  console.log('Turning green on')
  toggleLED(ledGreen, 1);
  console.log('Turning green off')
  setTimeout(() => toggleLED(ledGreen, 0), 2000);

});



app.get('/startf', (req, res) => {
  isFlashing = true;
  flashLights();
  console.log('Flashing started');
});

function flashLights() {
  if (!isFlashing) {
    turnOffAllLeds();
    return;
  }

  // Start with Green LED
  toggleLED(ledGreen, 1);
  setTimeout(() => {
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
              setTimeout(flashLights, 0); // Adjust this delay to control the gap between cycles
            } else {
              turnOffAllLeds();
            }
          }, 5000); // Red duration
        }
      }, 2000); // Yellow duration
    }
  }, 4000); // Green duration
}

function turnOffAllLeds() {
  toggleLED(ledRed, 0);
  toggleLED(ledYellow, 0);
  toggleLED(ledGreen, 0);
}


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

process.on('SIGINT', () => {
  ledRed.unexport();
  ledYellow.unexport();
  ledGreen.unexport();
});
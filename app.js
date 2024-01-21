const express = require('express');
const app = express();
const port = 3000;

// Set the view engine to ejs
app.set('view engine', 'ejs');
app.use(express.static('public'));


// Define the root route
app.get('/', (req, res) => {
  res.render('index');
});


function toggleLED(led, value) {
  try {
    led.writeSync(value);
  } catch (error) {
    console.error('Error in toggleLED:', error);
  }
}

app.get('/startf', (req, res) => {
  if (!isFlashing) {
    isFlashing = true;
    flashLights();
  }
  const flashLights = () => {
    if (!isFlashing) return;

    toggleLED(ledRed, 1);
    setTimeout(() => toggleLED(ledRed, 0), 1000);

    setTimeout(() => {
      toggleLED(ledYellow, 1);
      setTimeout(() => toggleLED(ledYellow, 0), 1000);
    }, 1000);

    setTimeout(() => {
      toggleLED(ledGreen, 1);
      setTimeout(() => toggleLED(ledGreen, 0), 1000);
    }, 2000);

    setTimeout(flashLights, 3000);
  };

  flashLights();

});

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
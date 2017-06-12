import processing.serial.*;   // import serial lib

int[] sensorValues = {0, 0, 0};   // array to store values from Arduino

Serial myPort; // create instance of serial lib

Ship myShip; // create a spaceship

int xChange, yChange; // variables for the ship
int increment = 1; // variable to change the rate of movement
int xDirection;
int yDirection;

Star[] stars; // array of stars
int starNum = 20; // number of stars

void setup() {
  size(800, 600);

  println(Serial.list()); 
  //assigning the port to connect to
  myPort = new Serial(this, Serial.list()[3], 9600);
  //buffer as one packet until we get a line change
  myPort.bufferUntil('\n');

  myShip = new Ship(); // instantiate the ship
  stars = new Star[starNum]; // new array of stars

  for (int x=0; x<starNum; x++) { 
    stars[x] = new Star(); // instatntiate the stars
  }
}

void draw() {
  background(0); // clear the background

  // directions
  String words = "use the arrows to move, 1-9 for ship speed";
  fill(255);
  text(words, 10, 30);

  // loop through all the stars
  for (int x=0; x< starNum; x++) {
    stars[x].update(); // update their position
    stars[x].collisionCheck(myShip.xPos, myShip.yPos); // check if colliding with the ship
    stars[x].render(); // draw the stars
  }

  myShip.update(xChange, yChange); // update the ship's position & shield size
  myShip.render(); // render the ship
  Direction_control();
  // reset vars if you want
  //yChange  = 0;
  //xChange = 0;

  // ************
  // change the speed of the ship
  // this will be replaced by serial code
  // ************
}

// move the ship
// this will be replaced by serial code
void Direction_control() {
  if (xDirection != 0) {
    if (xDirection == 1) {
      xChange = increment;
    }
    if (xDirection == -1) {
     xChange = increment * -1;
    }
  }else{
      xChange = 0;
  }
  if (yDirection != 0) {
    if (yDirection == 1) {
      yChange = increment;
    }  
    else if (yDirection == -1) {
      yChange = increment * -1;
    }
  }else{
    yChange = 0;
  }
}

//************** Star class
class Star {
  float xPos, yPos, starSize, speed; // variables
  boolean collision; // check for collision 

  // star constructor
  Star() { // initial state
    speed = random(1, 10);
    starSize = random(10, 100);
    xPos = random(0, width);
    yPos = random(100, width) * -1;
    collision = false;
  }

  void update() {  // update star position

    yPos += speed;

    if (yPos > height+starSize/2) {
      yPos = random(100, width) * -1;
      speed = random(1, 10);
      starSize = random(10, 50);
      xPos = random(0, width);
    }
  }

  void collisionCheck(int _x, int _y) { // check for a collision

    int shipX = _x;
    int shipY = _y;

    float dx = shipX - xPos;
    float dy = shipY - yPos;
    float d = sqrt(sq(dx)+sq(dy)); // distance between star and ship

    if (d < starSize/2 + 10) { // if distance is less than the radius of the star & ship
      collision = !collision;  // there's a crash
    }
  }

  void render() {
    // if there's no collision
    if (!collision) {
      noStroke();
      fill(220, 160, 0);
      ellipse(xPos, yPos, starSize, starSize);
    } else { // if there is a colliison, supernova
      strokeWeight(5);
      stroke(255);
      fill(220, 100, 0);
      ellipse(xPos, yPos, starSize*1.5, starSize*1.5);
      collision = !collision; // reset the collison state for the next iteration
    }
  }
}



//************** Ship class
class Ship {
  int xPos;
  int yPos;
  int shieldSize;

  Ship() { // ship constructor
    xPos = width/2;
    yPos = height - 100;
    shieldSize = 0;
  }

  void update(int _xDelta, int _yDelta) {
    xPos += _xDelta;
    yPos += _yDelta;
  }

  void render() {
    if (yPos > height-10) {
      yPos = height-10;
    }
    if (yPos < height-200) {
      yPos = height-200;
    }

    if (xPos > width-10) {
      xPos = width-10;
    }
    if (xPos < 10) {
      xPos = 10;
    }

    noStroke();
    fill(180, 10, 200);
    rectMode(CENTER);
    rect(xPos, yPos, 20, 20);
  }
}

void serialEvent(Serial myPort) {
  // the first couple things we read will be 0,0,0
  // after that we're golden

  // get the ASCII string by reading from the port until a newline char is reached
  String inString = myPort.readStringUntil('\n');

  if (inString != null) { // if you have stuff in the string
    // trim off any whitespace:
    inString = trim(inString);

    // split the string at the commas and convert the
    // resulting substrings into your array:
    sensorValues = int(split(inString, ","));
  }

  // print out the values you got:
  for (int sensorNum = 0; sensorNum < sensorValues.length; sensorNum++) {
    print("Sensor " + sensorNum + ": " + sensorValues[sensorNum] + "\t");
  }
  // add a linefeed after all the sensor values are printed:
  println();

  xDirection = sensorValues[0];
  yDirection = sensorValues[1];
  increment = sensorValues[2];


  // send a byte to ask for more data:
  myPort.write("A");
}
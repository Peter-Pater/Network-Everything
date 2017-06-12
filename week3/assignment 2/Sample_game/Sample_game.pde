import processing.serial.*;   // import serial lib

int[] sensorValues = {0, 0, 0, 0, 0, 0};   // array to store values from Arduino

Serial myPort; // create instance of serial lib

Ship myShip; // create a spaceship
Ship myShip1;

int xChange, yChange; // variables for the ship
int increment = 1; // variable to change the rate of movement
int xDirection;
int yDirection;

int xChange1, yChange1;
int increment1 = 1;
int xDirection1;
int yDirection1;


Star[] stars; // array of stars
int starNum = 30; // number of stars
PImage background;
PImage plane1;
PImage plane2;
PImage fire;

void setup() {
  size(1440, 800);

  println(Serial.list()); 
  //assigning the port to connect to
  myPort = new Serial(this, Serial.list()[0], 9600);
  //buffer as one packet until we get a line change
  myPort.bufferUntil('\n');

  myShip = new Ship(width/2 - 300, height/2 + 100); // instantiate the ship
  myShip1 = new Ship(width/2 + 300, height/2 + 100);
  stars = new Star[starNum]; // new array of stars

  for (int x=0; x<starNum; x++) { 
    stars[x] = new Star(); // instatntiate the stars
  }
  background = loadImage("star3.jpg");
  plane1 = loadImage("Plane1.png");
  plane2 = loadImage("Plane2.png");
  fire = loadImage("fire1.png");
}

void draw() {
  background(0); // clear the background
  image(background, 0, 0, width, height);
  //rectMode(CENTER);
  //stroke(255);
  //noFill();
  //rect(width-100,height-150,70,increment1*28);
  //print(increment);
  //print(increment1);
  //rect(100,height-150,70,increment*28);
  
  if (myShip1.counter > 0 && myShip.counter > 0) {
    // directions
    String words = "use the arrows to move, 1-9 for ship speed" + "Player1: " + myShip.counter + " ; " + "Player2: " + myShip1.counter;
    fill(255);
    textSize(20);
    text(words, 10, 30);

    // loop through all the stars
    for (int x=0; x< starNum; x++) {
      stars[x].update(); // update their position
      if (stars[x].collisionCheck(myShip.xPos, myShip.yPos)) {
        myShip.counter--;
      } // check if colliding with the ship
      if (stars[x].collisionCheck(myShip1.xPos, myShip1.yPos)) {
        myShip1.counter--;
      }
      stars[x].render(); // draw the stars
    }

    myShip.update(xChange, yChange); // update the ship's position & shield size
    myShip.render1(); // render the ship
    Direction_control();
    myShip1.update(xChange1, yChange1);
    myShip1.render2();
    Direction_control1();
  } else {
    if (myShip1.counter == 0) {
      String words = "Player 1 wins! Press any key to restart";
      fill(255);
      textSize(30);
      text(words, width/2 - 300, height/2);
    } else {
      String words = "Player 2 wins! Press any key to restart";
      fill(255);
      textSize(30);
      text(words, width/2 - 300, height/2);
    }
    if (keyPressed) {
      myShip.counter = 100;
      myShip1.counter = 100;
      myShip.xPos = width/2 - 300;
      myShip.yPos = height/2 + 100;
      myShip1.xPos = width/2 + 300;
      myShip1.yPos = height/2 + 100;
      for (int x=0; x< starNum; x++) {
        stars[x].xPos = random(0, width);
        stars[x].yPos = random(100, width) * -1;
      }
    }
  }

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
  } else {
    xChange = 0;
  }
  if (yDirection != 0) {
    if (yDirection == 1) {
      yChange = increment;
    } else if (yDirection == -1) {
      yChange = increment * -1;
    }
  } else {
    yChange = 0;
  }
}

void Direction_control1() {
  if (xDirection1 != 0) {
    if (xDirection1 == 1) {
      xChange1 = increment1;
    }
    if (xDirection1 == -1) {
      xChange1 = increment1 * -1;
    }
  } else {
    xChange1 = 0;
  }
  if (yDirection1 != 0) {
    if (yDirection1 == 1) {
      yChange1 = increment1;
    } else if (yDirection1 == -1) {
      yChange1 = increment1 * -1;
    }
  } else {
    yChange1 = 0;
  }
}

//************** Star class
class Star {
  float xPos, yPos, starSize, speed; // variables
  boolean collision; // check for collision 

  // star constructor
  Star() { // initial state
    speed = random(15, 30);
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
      starSize = random(20, 100);
      xPos = random(0, width);
    }
  }

  boolean collisionCheck(int _x, int _y) { // check for a collision

    int shipX = _x;
    int shipY = _y;

    float dx = shipX - xPos;
    float dy = shipY - yPos;
    float d = sqrt(sq(dx)+sq(dy)); // distance between star and ship

    if (d < starSize/2 + 65) { // if distance is less than the radius of the star & ship
      collision = !collision;  // there's a crash
      return true;
    }

    return false;
  }

  void render() {
    // if there's no collision
    if (!collision) {
      noStroke();
      fill(220, 160, 0);
      image(fire, xPos, yPos, starSize, starSize);
      //  ellipse(xPos, yPos, starSize, starSize);
    } else { // if there is a colliison, supernova
      strokeWeight(5);
      stroke(255);
      fill(220, 100, 0);
      image(fire, xPos, yPos, starSize*1.5, starSize*1.5);
      // ellipse(xPos, yPos, starSize*1.5, starSize*1.5);
      collision = !collision; // reset the collison state for the next iteration
    }
  }
}



//************** Ship class
class Ship {
  int xPos;
  int yPos;
  int shieldSize;
  int counter;

  Ship(int x, int y) { // ship constructor
    this.xPos = x;//width/2;
    this.yPos = y;//height - 100;
    this.counter = 100;
    shieldSize = 0;
  }

  void update(int _xDelta, int _yDelta) {
    xPos += _xDelta;
    yPos += _yDelta;
  }

  void render1() {
    if (yPos > height-150) {
      yPos = height-150;
    }
    if (yPos < height-650) {
      yPos = height-650;
    }

    if (xPos > width-150) {
      xPos = width-150;
    }
    if (xPos < 0) {
      xPos = 0;
    }

//    noStroke();
//    fill(180, 10, 200);
    image(plane1, xPos, yPos, 120, 150);
  }
  void render2() {
    if (yPos > height-150) {
      yPos = height-150;
    }
    if (yPos < height-650) {
      yPos = height-650;
    }

    if (xPos > width-150) {
      xPos = width-150;
    }
    if (xPos < 0) {
      xPos = 0;
    }

    //noStroke();
    //fill(180, 10, 200);
    image(plane2, xPos, yPos, 120, 150);
    // rect(xPos, yPos, 20, 20);
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
    //   print(inString);
    sensorValues = int(split(inString, ","));
  }

  // print out the values you got:
  for (int sensorNum = 0; sensorNum < sensorValues.length; sensorNum++) {
    print("Sensor " + sensorNum + ": " + sensorValues[sensorNum] + "\t");
  }
  // add a linefeed after all the sensor values are printed:
  println();
  if (sensorValues.length == 6) {
    xDirection = sensorValues[1];
    yDirection = sensorValues[0];
    increment = sensorValues[2] * 5;
    xDirection1 = sensorValues[4];
    yDirection1 = sensorValues[3];
    increment1 = sensorValues[5] * 5;
  }

  // send a byte to ask for more data:
  myPort.write("A");
}
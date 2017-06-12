import processing.serial.*;  
Serial myPort; 
ArrayList<Circle> circles;
PImage img;
import ddf.minim.*;
Minim minim;
AudioPlayer[] player;
float r = 0.1;
//int m_cur = 0;
//int m_pre = m_cur;
boolean triger = false;
int counter;

int command = 0;
int count = 0;


void setup() {
  size(1440, 810);
  img = loadImage("02.png");
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[3], 9600);
  myPort.bufferUntil('\n');
  background(255);
  circles = new ArrayList<Circle>();
  circles.add(new Circle(1, int(random(75, 1365)), int(random(75, 825))));
  minim= new Minim(this);
  player = new AudioPlayer[36];
  player[0] = minim.loadFile("0.mp3");
  player[1] = minim.loadFile("1.mp3");
  player[2] = minim.loadFile("2.mp3");
  player[3] = minim.loadFile("3.mp3");
  player[4] = minim.loadFile("4.mp3");
  player[5] = minim.loadFile("5.mp3");
  player[6] = minim.loadFile("6.mp3");
  player[7] = minim.loadFile("7.mp3");
  player[8] = minim.loadFile("8.mp3");
  player[9] = minim.loadFile("9.mp3");
  player[10] = minim.loadFile("10.mp3");
  player[11] = minim.loadFile("11.mp3");
  player[12] = minim.loadFile("12.mp3");
  player[13] = minim.loadFile("13.mp3");
  player[14] = minim.loadFile("14.mp3");
  player[15] = minim.loadFile("15.mp3");
  player[16] = minim.loadFile("16.mp3");
  player[17] = minim.loadFile("17.mp3");
  player[18] = minim.loadFile("18.mp3");
  player[19] = minim.loadFile("19.mp3");
  player[20] = minim.loadFile("20.mp3");
  player[21] = minim.loadFile("21.mp3");
  player[22] = minim.loadFile("22.mp3");
  player[23] = minim.loadFile("23.mp3");
  player[24] = minim.loadFile("24.mp3");
  player[25] = minim.loadFile("25.mp3");
  player[26] = minim.loadFile("26.mp3");
  player[27] = minim.loadFile("27.mp3");
  player[28] = minim.loadFile("28.mp3");
  player[29] = minim.loadFile("29.mp3");
  player[30] = minim.loadFile("30.mp3");
  player[31] = minim.loadFile("31.mp3");
  player[32] = minim.loadFile("32.mp3");
  player[33] = minim.loadFile("33.mp3");
  player[34] = minim.loadFile("34.mp3");
  player[35] = minim.loadFile("35.mp3");
  
}


void draw() {
  background(255);
  image(img, 0, 0);
  fill(0,80);
  rect(0,0,1440,810);
  //m_cur = millis();
  if (command == 1) {
    circles.add(new Circle(random(30, 90), int(random(50, 1390)), int(random(50, 850))));
    counter= int(random(0,35));
    player[counter].play();
    player[counter].rewind();
    //m_pre = m_cur;
    count++;
  }
  for (int i = circles.size()-1; i >= 0; i--) { 
    // An ArrayList doesn't know what it is storing so we have to cast the object coming out
    Circle circle = circles.get(i);
    circle.update();
    //if (circle.finished()) {
    //  // Items can be deleted with remove()
    //  circles.remove(i);
    //}
  }
}

class Circle {
  float radius;
  int xPos;
  int yPos;
  int r, g, b, a;
  float R;
  int speed = 1;
  float speed1 = 1.5;
  int range = 20;
  boolean judge1 = true;
  boolean judge2 = false;

  Circle(float _radius, int _xPos, int _yPos) {
    this.radius = _radius;
    this.xPos = _xPos;
    this.yPos = _yPos;
    this.r = 100;
    this.g = 229;
    this.b = 228;
   // this.a = int(random(90, 100));
  }



  void update() {
    noStroke();
    fill(r, g, b);
    ellipse(this.xPos, this.yPos, this.radius, this.radius);
    R = this.radius;
    if (judge1 == true) {
      this.radius += speed;
      speed +=0.5;
      if (this.radius > int(R/3)) {
        speed +=2;
      }
    }

    if (this.radius >= R) {
      judge1 = false;
      judge2 = true;
    }

    if (judge2 == true) {
      this.radius = this.radius - speed1;
      speed1 =0.2;
    }

    if (this.radius <= 0) {
      // judge1 = false;
      judge2 = false;
      speed1 = 0;
    }
  }
}

void serialEvent(Serial myPort) {
  // read the serial buffer:
  String myString = myPort.readStringUntil('\n');
  //if you got any bytes other than the linefeed:
  myString = trim(myString);
  int sensors[] = int(split(myString, ','));

  for (int sensorNum = 0; sensorNum < sensors.length; sensorNum++) {
    print("Sensor " + sensorNum + ": " + sensors[sensorNum] + "\t");
  }
  println();
  if (sensors.length > 1) {
    command = sensors[0];
  }
  //send a byte to ask for more data:
  myPort.write("A");
}
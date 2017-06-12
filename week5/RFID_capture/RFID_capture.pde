import gab.opencv.*;
import processing.video.*;
import java.awt.*;
import javax.mail.*;
import javax.mail.internet.*;
import javax.mail.Authenticator;
import javax.mail.PasswordAuthentication;
import java.util.*;
import java.io.*;
import javax.activation.*;
import processing.serial.*;

Serial myPort;
String inBuffer = " ";


Capture video;
OpenCV opencv;
PImage photo;
PImage img;
boolean captured = false;
boolean addressReceived = false;
float textPos = 0.2 * width / 2 + 370;
String note = "Here is your photo!";
String note1 = "Waiting for a card!";
String note2 = "";
String Note = "Super Cool Photo Taker";
String emailAddress;
PFont font;

void setup() {
  size(960, 720);
  video = new Capture(this, 960, 720);
  opencv = new OpenCV(this, 960, 720);
  frameRate(10);
  println(Serial.list()); // list all the serial ports on this computer
  myPort = new Serial(this, Serial.list()[3], 9600);
  font = createFont("bebasneue.TTF", 50);
  video.start();
  img = loadImage("bg.jpg");
}

void draw() {
  if (inBuffer != " ") { // if there's a valid tag
    test(inBuffer);
  }
  if (addressReceived == false) {
    background(#FFC0CB);
  //  image(img,0,0,width,height);
    fill(0);
    textFont(font);
    textSize(65);
    text(Note, 0.2 * width / 2 + 140, height/2 - 140);
    textSize(30);
    text(note1, textPos, height/2+50);
    text(note2, 0.2 * width / 2, height/2 + 100);
  } else {
    if (captured == false) {
      scale(-1.0, 1.0);
      opencv.loadImage(video);
      image(video, -video.width, 0);
    } else {
      scale(0.8);
      background(#FFC0CB);
      //image(img,0,0,width,height);
      image(photo, 0.2 * width / 2 + 30, 0.2 * height / 2 + 20);
      fill(0);
      textSize(55);
      text(note, 0.2 * width / 2 + 330, 58);
      textSize(30);
      text("Press space to send, or press any other key to start over!", 0.2 * width / 2+29, 0.2 * height / 2 + 790);
    }
  }
}


void captureEvent(Capture c) {
  c.read();
}

void mouseClicked() {
  if (addressReceived == true) {
    if (captured == false) {
      captured = true;
      saveFrame("Your_photo.jpg");
      loadTimeOut();
    }
  }
}

void serialEvent(Serial myPort) {
  while (myPort.available() > 13) { // read the values from the tag
    inBuffer = myPort.readString();
    inBuffer = inBuffer.trim();
    if (inBuffer != null) {
      println(inBuffer);  // print the value to the console
    }
  }
}

void loadTimeOut() {
  float t = millis();
  while (millis() - t < 1000) {
    photo = loadImage("Your_photo.jpg");
  }
}

void keyReleased() {
  if (captured == true) {
    if (key == ' ') {
      sendEmail();
      note = "Your photo is sent!";
    } else {
      captured = false;
      addressReceived = false;
      textPos = 0.2 * width / 2 + 350;
      note = "Here is your photo!";
      note1 = "Waiting for a card!";
      note2 = "";
    }
  }
}

//void serialEvent(Serial myPort) {
//  if (addressReceived == false) {
//    if (keyPressed == false) {
//      emailAddress = myPort.readStringUntil('\n');
//      emailAddress = trim(emailAddress);
//      //if you got any bytes other than the linefeed:
//      if (emailAddress.length() > 5) {
//        textPos = 0.2 * width / 2;
//        println("Your email adress is: " + emailAddress);
//        note1 = "Your email adress is: " + emailAddress;
//        note2 = "If correct, press any key to continue, otherwise reswipe!";
//      }
//    } else {
//      addressReceived = true;
//    }
//  }
//  myPort.write("A");
//}

void test(String tagVal) {
  if (addressReceived == false) {
    if (keyPressed == false) {
      if (tagVal.equals("0000ACB7C6DD")) {
        emailAddress = "yx999@nyu.edu";
      }
      if (tagVal.equals("180026358A81")) {
        emailAddress = "xh721@nyu.edu";
      }
      if (tagVal.equals("010017BA77DB")) {
        emailAddress = "shf220@nyu.edu";
      }
      emailAddress = trim(emailAddress);
      if (emailAddress.length() > 5) {
        textPos = 0.2 * width / 2;
        //println("Your email address is: " + emailAddress);
        note1 = "Your email address is: " + emailAddress;
        note2 = "If correct, press any key to continue, otherwise reswipe!";
      }
    } else {
      addressReceived = true;
      println("ready to capture!");
    }
  }
}



/*-------------------------------------------Email sender----------------------------------------------------*/
public class Auth extends Authenticator {

  public Auth() {
    super();
  }

  public PasswordAuthentication getPasswordAuthentication() {
    String username, password;
    username = "humansofnyush@gmail.com";
    password = "humansNYUSH";
    System.out.println("authenticating... ");
    return new PasswordAuthentication(username, password);
  }
}
// Example functions that send mail (smtp)
// You can also do imap, but that's not included here

// A function to check a mail account
// A function to send mail
void sendEmail() {
  // Create a session
  String host="smtp.gmail.com";
  Properties props=new Properties();

  // SMTP Session
  props.put("mail.transport.protocol", "smtp");
  props.put("mail.smtp.host", host);
  props.put("mail.smtp.port", "587");
  props.put("mail.smtp.auth", "true");
  // We need TTLS, which gmail requires
  props.put("mail.smtp.starttls.enable", "true");

  // Create a session
  Session session = Session.getDefaultInstance(props, new Auth());

  try
  {
    MimeMessage msg=new MimeMessage(session);
    msg.setFrom(new InternetAddress("humansofnyush@gmail.com", "humansNYUSH"));
    msg.addRecipient(Message.RecipientType.TO, new InternetAddress(emailAddress));
    msg.setSubject("Super cool photo");
    BodyPart messageBodyPart = new MimeBodyPart();
    // Fill the message
    messageBodyPart.setText("Checkout the photo you have just taken!");
    Multipart multipart = new MimeMultipart();
    multipart.addBodyPart(messageBodyPart);
    // Part two is attachment
    messageBodyPart = new MimeBodyPart();
    DataSource source = new FileDataSource("/Users/xuyaming/Desktop/RFID_capture/Your_photo.jpg");
    messageBodyPart.setDataHandler(new DataHandler(source));
    messageBodyPart.setFileName("Hey_You.jpg");
    multipart.addBodyPart(messageBodyPart);
    msg.setContent(multipart);
    msg.setSentDate(new Date());
    Transport.send(msg);
    println("Mail sent!");
  }
  catch(Exception e)
  {
    e.printStackTrace();
  }
}

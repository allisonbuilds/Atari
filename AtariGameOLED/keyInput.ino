//for reading keyboard input instead of atari controller
int incomingByte = 0;

int keyInput(){
 if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    // if it's an s, move left
    if (incomingByte == 's') {
      return(-1);
    }
    // if it's a d, move right
    if (incomingByte == 'd') {
      return(1);
    }
  }
}

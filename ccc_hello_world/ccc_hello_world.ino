//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
//
// end license header
//
// This sketch is a good place to start if you're just getting started with 
// Pixy and Arduino.  This program simply prints the detected object blocks 
// (including color codes) through the serial console.  It uses the Arduino's precision
// ICSP SPI port.  For more information go here:
//
// https://docs.pixycam.com/wiki/doku.php?id=wiki:v2:hooking_up_pixy_to_a_microcontroller_-28like_an_arduino-29
//
  
#include <Pixy2.h>
#include <SPI.h>
#include <stdlib.h>
#include <Wire.h>
#define DPP (1/3779.5275591)
#define PRECISION 1000.0

// This is the main Pixy object 
Pixy2 pixy;
float axis_x, axis_y, pixel_x, pixel_y, distance, offset;
double blocks[200];
Block largestBlock;

void setup()
{
  Serial.begin(115200);
  Serial.print("Starting...\n");
  Serial.flush();
  pixy.init();
  Wire.begin();
}

void lampOn()
{
  pixy.setLamp((byte) 1, (byte) 1);
}

void lampOff()
{
  pixy.setLamp((byte) 0, (byte) 0);
}

void LEDOn() 
{
  pixy.setLED(100,100,100);
}

void LEDOff() 
{
  pixy.setLED(0,0,0);
}

void loop()
{ 
  lampOff();
  getBiggestBlock();
  sendI2C();
  getDistance();
  /*Serial.print("Distance is ");
  Serial.print(distance);
  Serial.print(", offset is ");
  Serial.println(offset);
  Serial.flush();*/
  
  // Ax = Axis X, Ay = Axis Y, Px = Pixel X, Py = Pixel Y, Rx = Resolution X, Ry = Resolution Y
  // Ax = , Ay = , Px = , Py = , Rx = 316, Ry = 208
  // Ax = (Px-Rx/2)/(Rx/2);
  // Ay = (Py-Ry/2)/(Ry/2);
  // 39 inches wide, 29 tall
  /**
   * Width - int
   * Height - int
   * X - int
   * Y - int
   * distance - double
   * angle - double
   */
}

Block getBiggestBlock()
{
  int blockCount = pixy.ccc.getBlocks();
  largestBlock = pixy.ccc.blocks[0];
  for (int i = 0; i <= blockCount; i++) {
    if (pixy.ccc.blocks[i].m_signature == 2) {
      if (largestBlock.m_width < pixy.ccc.blocks[i].m_width)
        largestBlock = pixy.ccc.blocks[i];
    }
  }
  return largestBlock;
}

float getBiggestX()
{
  return (float)largestBlock.m_x;
}

float getBiggestY()
{
  return (float)largestBlock.m_y;
}

float getDistance()
{
  distance = (3 * 736.6 * largestBlock.m_height);
  distance = distance / (2784 * 100UL); // Denominator not exact
  return distance;
  /*pixel_x = getBiggestX();
  pixel_y = getBiggestY();
  axis_x = (pixel_x - (316.0/2.0)) / (316.0/2.0);
  axis_y = (pixel_y - (208.0/2.0)) / (208.0/2.0);
  //distance = ((38 + 9/16) * largestBlock.m_width) / (2 * (39/12 * 1152.0001451339) * tan(largestBlock.m_angle));
  float pt1 = ((38.0 + 9.0/16.0) * (float)largestBlock.m_width);
  float pt2 = (2.0 * (39.0/12.0 * 1152.0) * (float)tan(largestBlock.m_angle));
  distance = pt1 / pt2;*/
}

float getOffset() {
  offset = (largestBlock.m_x + largestBlock.m_width/2) * 0.0002645833;
  return offset;
}

void sendI2C() {
  Wire.beginTransmission(0);
  Wire.write((int)(offset * PRECISION));
  Serial.print("Offset is ");
  Serial.println(offset);
  Wire.write((int)(distance * PRECISION));
  Serial.print("Distance is ");
  Serial.println(distance);
  Wire.endTransmission();
}
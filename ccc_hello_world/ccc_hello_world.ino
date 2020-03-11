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
// (including color codes) through the serial console.  It uses the Arduino's 
// ICSP SPI port.  For more information go here:
//
// https://docs.pixycam.com/wiki/doku.php?id=wiki:v2:hooking_up_pixy_to_a_microcontroller_-28like_an_arduino-29
//
  
#include <Pixy2.h>
#include <SPI.h>
#include <stdlib.h>

// This is the main Pixy object 
Pixy2 pixy;
double axis_x, axis_y, pixel_x, pixel_y, distance;
double blocks[200];
Block largestBlock;

void setup()
{
  Serial.begin(115200);
  Serial.print("Starting...\n");
  pixy.init();
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
  int i = pixy.ccc.getBlocks();
  if (i > 0) {
    lampOff();
    lampOn();
  }
  for (int x = i; x <= pixy.ccc.numBlocks; x++) {
    pixy.ccc.blocks[i].print();
  }

  
  // Ax = Axis X, Ay = Axis Y, Px = Pixel X, Py = Pixel Y, Rx = Resolution X, Ry = Resolution Y
  // Ax = , Ay = , Px = , Py = , Rx = 316, Ry = 208
  // Ax = (Px-Rx/2)/(Rx/2);
  // Ay = (Py-Ry/2)/(Ry/2);
  // 39 inches wide, 29 tall
}

Block getBiggestBlock()
{
  int blockCount = pixy.ccc.getBlocks();
  largestBlock = pixy.ccc.blocks[0];
  for (int i = 0; i <= blockCount; i++) {
    if (pixy.ccc.blocks[i].m_signature == 1) {
      if (largestBlock.m_width < pixy.ccc.blocks[i].m_width)
        largestBlock = pixy.ccc.blocks[i];
    }
  }
  return largestBlock;
}

double getBiggestX()
{
  return largestBlock.m_x;
}

double getBiggestY()
{
  return largestBlock.m_y;
}

double getDistance()
{
  pixel_x = getBiggestX();
  pixel_y = getBiggestY();
  axis_x = (pixel_x - (316/2)) / (316/2);
  axis_y = (pixel_y - (208/2)) / (208/2);
  distance = ((38 + 9/16) * largestBlock.m_width) / (2 * (39/12 * 1152.0001451339) * tan(largestBlock.m_angle));
}

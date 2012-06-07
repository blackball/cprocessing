#include <cprocessing/cprocessing.hpp>
using namespace cprocessing;

/**
 * Array 2D. 
 * 
 * Demonstrates the syntax for creating a two-dimensional (2D) array.
 * Values in a 2D array are accessed through two index values.  
 * 2D arrays are useful for storing images. In this example, each dot 
 * is colored in relation to its distance from the center of the image. 
 */

float * dArray[height];
float ** distances;

float maxDistance;
int spacer;

void setup() {
    size(640, 360);
    maxDistance = dist(width/2, height/2, width, height);
    
    for (int y = 0; y < height; y++) {

        
        for (int x = 0; x < width; x++) {
            distances = dArray[y][x];
            float distance = dist(width/2, height/2, x, y);
            distances = dArray[y][x];
            distances = distance/maxDistance * 255;
        }
    }
    
    spacer = 10;
}

void draw() {
  background(0);
  for (int y = 0; y < height; y += spacer) {
    for (int x = 0; x < width; x += spacer) {
      stroke(distances[x][y]);
      point(x + spacer/2, y + spacer/2);
    }
  }
  noLoop();  // Run once and stop
}




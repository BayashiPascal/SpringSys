#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "springsys.h"
#include "tgapaint.h"

// Function to draw the SpringSys of the first example in one dimension
void DrawLegendTGA_1D(SpringSys *sys, TGA *tga, float t, float dt,
  float tMax, float lMax, float lPixel, int margin) {
  // Create a pencil with black color
  TGAPencil *pen = TGAGetPencil();
  TGAPixel *black = TGAGetBlackPixel();
  TGAPencilSetColor(pen, black);
  TGAPencilSetAntialias(pen, true);
  TGAFreePixel(&black);
  // Create a font
  TGAFont *font = TGAFontCreate(tgaFontDefault);
  TGAFontSetSize(font, 0.5 * (float)margin);
  // Declare variables for tracing
  float p[2];
  char s[100] = {0};
  float q[2];
  // Draw the abciss
  q[1] = p[1] = 0.25 * (float)margin;
  for (p[0] = 0.0; p[0] < tMax; p[0] += 5.0) {
    // Update the label
    sprintf(s, "%.1f", p[0]);
    // Print the label
    q[0] = p[0] / dt + 0.5 * (float)margin;
    TGAPrintString(tga, pen, font, (unsigned char*)s, q);
  }
  // Draw the ordinate
  q[0] = p[0] = 0.25 * (float)margin;
  for (p[1] = 0.0; p[1] < lMax; p[1] += 1.0) {
    // Update the label
    sprintf(s, "%d", (int)round(p[1]));
    // Print the label
    q[1] = p[1] / lPixel + (float)margin;
    TGAPrintString(tga, pen, font, (unsigned char*)s, q);
  }
  p[0] = q[0] = (float)margin;
  p[1] = (float)margin;
  q[1] = lMax / lPixel + (float)margin;
  TGAPencilSetShapePixel(pen);
  TGAPencilSetAntialias(pen, false);
  TGADrawLine(tga, p, q, pen);
  // Free the pencil
  TGAFreePencil(&pen);
  // Free the font
  TGAFreeFont(&font);
}

// Function to draw the SpringSys of the first example in one dimension
void DrawTGA_1D(SpringSys *sys, TGA *tga, float t, float dt, 
  float lPixel, int margin) {
  // Create a pencil with black color
  TGAPencil *pen = TGAGetPencil();
  TGAPixel *black = TGAGetBlackPixel();
  TGAPencilSetColor(pen, black);
  TGAPencilSetShapePixel(pen);
  TGAFreePixel(&black);
  // Declare variables for tracing
  float p[2];
  // Position in abciss is the center of the pixel corresponding to
  // the time (scale by dt and shift by margin)
  p[0] = t / dt + 0.5 * dt + (float)margin;
  // Draw the masses of the SpringSys
  int nbMass = SpringSysGetNbMass(sys);
  for (int iMass = 0; iMass < nbMass; ++iMass) {
    // Get the mass
    SpringSysMass *m = SpringSysGetMass(sys, iMass);
    // If the mass is not null
    if (m != NULL) {
      p[1] = m->_pos[0] / lPixel + (float)margin;
      TGAStrokePix(tga, p, pen);
    }
  }
  // Free the pencil
  TGAFreePencil(&pen);
}

// Function to draw the SpringSys of the second example in two dimensions
void DrawLegendTGA_2D(SpringSys *sys, TGA *tga, float lMax, 
  float lPixel, int margin, float slope, float k) {
  // Create a pencil with black color
  TGAPencil *pen = TGAGetPencil();
  TGAPixel *black = TGAGetBlackPixel();
  TGAPencilSetColor(pen, black);
  TGAPencilSetAntialias(pen, true);
  TGAFreePixel(&black);
  // Create a font
  TGAFont *font = TGAFontCreate(tgaFontDefault);
  TGAFontSetSize(font, 0.25 * (float)margin);
  // Declare variables for tracing
  float p[2];
  char s[100] = {0};
  float q[2];
  // Draw the abciss
  q[1] = p[1] = 0.25 * (float)margin;
  for (p[0] = 0.0; p[0] < lMax; p[0] += 0.5) {
    // Update the label
    sprintf(s, "%.1f", p[0]);
    // Print the label
    q[0] = p[0] / lPixel + 0.75 * (float)margin;
    TGAPrintString(tga, pen, font, (unsigned char*)s, q);
  }
  // Draw the ordinate
  q[0] = p[0] = 0.25 * (float)margin;
  for (p[1] = 0.0; p[1] < lMax; p[1] += 1.0) {
    // Update the label
    sprintf(s, "%.1f", p[1]);
    // Print the label
    q[1] = p[1] / lPixel + (float)margin;
    TGAPrintString(tga, pen, font, (unsigned char*)s, q);
  }
  // Draw the k coefficient
  TGAFontSetSize(font, 0.5 * (float)margin);
  sprintf(s, "k=%.1f", k);
  q[0] = 0.25 / lPixel + (float)margin;
  q[1] = 3.0 / lPixel + (float)margin;
  TGAPrintString(tga, pen, font, (unsigned char*)s, q);
  // Draw the axis
  TGAPencilSetShapePixel(pen);
  TGAPencilSetAntialias(pen, false);
  p[0] = p[1] = (float)margin;
  q[0] = (float)margin;
  q[1] = lMax / lPixel + (float)margin;
  TGADrawLine(tga, p, q, pen);
  q[0] = lMax / lPixel + (float)margin;
  q[1] = (float)margin;
  TGADrawLine(tga, p, q, pen);
  // Draw the ground
  q[1] = slope * lMax / lPixel + (float)margin;
  TGADrawLine(tga, p, q, pen);
  // Free the pencil
  TGAFreePencil(&pen);
  // Free the font
  TGAFreeFont(&font);
}

// Function to draw the SpringSys of the second example in two dimensions
void DrawTGA_2D(SpringSys *sys, TGA *tga, float lPixel, int margin) {
  // Create a pencil with black color
  TGAPencil *pen = TGAGetPencil();
  TGAPixel *black = TGAGetBlackPixel();
  TGAPencilSetColor(pen, black);
  TGAPencilSetShapePixel(pen);
  TGAFreePixel(&black);
  // Declare variables for tracing
  float p[2];
  float q[2];
  // Set the color of each mass
  unsigned char rgba[16] = 
    {255,0,0,255, 0,255,0,255, 0,0,255,255, 255,255,0,255};
  for (int iColor = 0; iColor < 4; ++iColor) {
    TGAPencilSelectColor(pen, iColor);
    TGAPencilSetColRGBA(pen, rgba + iColor * 4);
  }
  // Draw the border of the square
  int nbSpring = 4;
  for (int iSpring = 0; iSpring < nbSpring; ++iSpring) {
    // Get the spring
    SpringSysSpring *s = SpringSysGetSpring(sys, iSpring);
    // If the spring is not null
    if (s != NULL) {
      // Get the masses
      SpringSysMass *mA = SpringSysGetMass(sys, s->_mass[0]);
      SpringSysMass *mB = SpringSysGetMass(sys, s->_mass[1]);
      // Draw the line between the mass
      p[0] = mA->_pos[0] / lPixel + (float)margin;
      p[1] = mA->_pos[1] / lPixel + (float)margin;
      q[0] = mB->_pos[0] / lPixel + (float)margin;
      q[1] = mB->_pos[1] / lPixel + (float)margin;
      TGAPencilSetModeColorBlend(pen, s->_mass[0], s->_mass[1]);
      TGADrawLine(tga, p, q, pen);
    }
  }
  // Free the pencil
  TGAFreePencil(&pen);
}

int main(int argc, char **argv) {
  // Create a first example in one dimension, 
  // a chain of spring aligned and fixed at one extermity,
  // initially compressed and with no velocity,
  // default values for masses and springs
  fprintf(stdout, " -------- 1D example -----------\n");
  int nbDim = 1;
  // Allocate memory for the system
  SpringSys *theSpringSys = SpringSysCreate(nbDim);
  // Declare a variable to create the masses
  SpringSysMass *mass = SpringSysCreateMass();
  if (mass == NULL) {
    fprintf(stderr, "Couldn't allocate memory for mass\n");
  }
  // Declare a variable to create the springs
  SpringSysSpring *spring = SpringSysCreateSpring();
  if (spring == NULL) {
    fprintf(stderr, "Couldn't allocate memory for spring\n");
  }
  // Create the masses
  int nbMass = 5;
  for (int iMass = 0; iMass < nbMass; ++iMass) {
    // Set the mass properties
    mass->_id = iMass;
    mass->_pos[0] = 0.5 * (float)iMass;
    mass->_mass = 0.0;
    if (iMass == 0)
      mass->_fixed = true;
    else
      mass->_fixed = false;
    // Add the mass to the system
    bool ret = SpringSysAddMass(theSpringSys, mass);
    if (ret == false) {
      // Something went wrong when adding the mass
      fprintf(stderr, "Couldn't add the mass\n");
      // Print the mass for debugging
      SpringSysMassPrint(mass, stderr);
      fprintf(stderr, "\n");
      // Free memory
      SpringSysFree(&theSpringSys);
      SpringSysMassFree(&mass);
      SpringSysSpringFree(&spring);
      // Stop
      return 1;
    }
  }
  // Create the springs
  int nbSpring = nbMass - 1;
  for (int iSpring = 0; iSpring < nbSpring; ++iSpring) {
    // Set the spring properties
    spring->_id = iSpring;
    spring->_mass[0] = iSpring;
    spring->_mass[1] = iSpring + 1;
    // Add the spring to the system
    bool ret = SpringSysAddSpring(theSpringSys, spring);
    if (ret == false) {
      // Something went wrong when adding the spring
      fprintf(stderr, "Couldn't add the spring\n");
      // Print the spring for debugging
      SpringSysSpringPrint(spring, stderr);
      fprintf(stderr, "\n");
      // Free memory
      SpringSysFree(&theSpringSys);
      SpringSysMassFree(&mass);
      SpringSysSpringFree(&spring);
      // Stop
      return 1;
    }
  }
  // Print the SpringSys
  fprintf(stdout, "SpringSys:\n");
  SpringSysPrint(theSpringSys, stdout);
  // Declare some variable to memorize the parameters of the simulation
  float t = 0.0;
  float tMax = 100.0;
  float dt = 0.1;
  float lMax = (float)(1 + nbMass);
  float lPixel = 0.01;
  int margin = 20;
  // Create a TGA to draw the SpringSys
  TGAPixel *white = TGAGetWhitePixel();
  short dim[2];
  dim[0] = (int)round(tMax / dt) + margin * 2;
  dim[1] = (int)round(lMax / lPixel) + margin * 2;
  TGA* tga = TGACreate(dim, white);
  TGAFreePixel(&white);
  // If the TGA couldn't be created
  if (tga == NULL) {
    // Free memory
    SpringSysFree(&theSpringSys);
    SpringSysMassFree(&mass);
    SpringSysSpringFree(&spring);
    // Stop
    return 1;
  }
  // Draw the legend of the TGA
  DrawLegendTGA_1D(theSpringSys, tga, t, dt, tMax, lMax, lPixel, margin);
  // Draw the intial state to the TGA
  DrawTGA_1D(theSpringSys, tga, t, dt, lPixel, margin);
  // Simulate the SpringSys behaviour
  while (t < tMax) {
    // Step the SpringSys
    SpringSysStep(theSpringSys, dt);
    // Draw the new state to the TGA
    DrawTGA_1D(theSpringSys, tga, t, dt, lPixel, margin);
    // Display masses position
    fprintf(stdout, "%.3f: ", t);
    for (int iMass = 0; iMass < nbMass; ++iMass) {
      SpringSysMass *m = SpringSysGetMass(theSpringSys, iMass);
      float p = m->_pos[0];
      fprintf(stdout, "%.3f, ", p);
    }
    fprintf(stdout, "\n");
    // Increment time
    t += dt;
  }
  // Save the TGA
  TGASave(tga, "./springSys1D.tga");
  // Reset the initial position of the mass
  for (int iMass = 0; iMass < nbMass; ++iMass) {
    SpringSysMass *m = SpringSysGetMass(theSpringSys, iMass);
    m->_pos[0] = 0.5 * (float)iMass;
  }
  // Get the equilibrium
  tMax = 1000.0;
  t = SpringSysStepToRest(theSpringSys, dt, tMax);
  // If we could reach the equilibrium
  if (t <= tMax) {
    fprintf(stderr,"Equilibrium reach in %.3f second\n", t);
    // Display masses position
    fprintf(stdout, "Masses position: ");
    for (int iMass = 0; iMass < nbMass; ++iMass) {
      SpringSysMass *m = SpringSysGetMass(theSpringSys, iMass);
      float p = m->_pos[0];
      fprintf(stdout, "%.3f, ", p);
    }
    fprintf(stdout, "\n");
  // Else, we couldn't reach the equilibrium
  } else {
    fprintf(stderr,"Coudln't reach the equilibrium\n");
  }
  // Free the TGA
  TGAFree(&tga);
  // Free memory
  SpringSysFree(&theSpringSys);


  // Create a second example in two dimensions, 
  // a 2d square of masses (link side by side and diagonals)
  // falling under gravity onto an inclined ground
  fprintf(stdout, " -------- 2D example -----------\n");
  nbDim = 2;
  // Allocate memory for the system
  theSpringSys = SpringSysCreate(nbDim);
  // Create the masses and add them to the system
  mass->_mass = 0.1;
  mass->_fixed = false;
  nbMass = 4;
  // Shift the upper right corner to create the SpringSys in 
  // an instable state
  // Then we'll let it reach equilibrium before starting the simulation
  // This is a trick to position the square slightly rotated relatively
  // to the horizontal and avoid corner to be aligned during fall 
  // in the TGA
  float posMass[8] = {1.1, 3.0, 2.1, 3.0, 0.9, 2.0, 1.9, 2.0};
  for (int iMass = 0; iMass < nbMass; ++iMass) {
    // Set the mass position
    mass->_id = iMass;
    mass->_pos[0] = posMass[2 * iMass];
    mass->_pos[1] = posMass[2 * iMass + 1];
    // Add the mass to the system
    bool ret = SpringSysAddMass(theSpringSys, mass);
    if (ret == false) {
      // Something went wrong when adding the mass
      fprintf(stderr, "Couldn't add the mass\n");
      // Print the mass for debugging
      SpringSysMassPrint(mass, stderr);
      fprintf(stderr, "\n");
      // Free memory
      SpringSysFree(&theSpringSys);
      SpringSysMassFree(&mass);
      SpringSysSpringFree(&spring);
      // Stop
      return 1;
    }
  }
  // Create the springs
  nbSpring = 6;
  int springDef[12] = {0, 1, 1, 3, 2, 3, 0, 2, 1, 2, 0, 3};
  float k = 1000.0;
  spring->_k = k;
  for (int iSpring = 0; iSpring < nbSpring; ++iSpring) {
    // Set the spring properties
    spring->_id = iSpring;
    spring->_mass[0] = springDef[iSpring * 2];
    spring->_mass[1] = springDef[iSpring * 2 + 1];
    if (iSpring < 4)
      spring->_length = spring->_restLength = 1.0;
    else
      spring->_length = spring->_restLength = sqrt(2.0);
    // Add the spring to the system
    bool ret = SpringSysAddSpring(theSpringSys, spring);
    if (ret == false) {
      // Something went wrong when adding the spring
      fprintf(stderr, "Couldn't add the spring\n");
      // Print the spring for debugging
      SpringSysSpringPrint(spring, stderr);
      fprintf(stderr, "\n");
      // Free memory
      SpringSysFree(&theSpringSys);
      SpringSysMassFree(&mass);
      SpringSysSpringFree(&spring);
      // Stop
      return 1;
    }
  }
  // Reach equilibrium
  dt = 1.0 / 50.0;
  t = SpringSysStepToRest(theSpringSys, dt, tMax);
  if (t <= tMax) {
    fprintf(stderr,"Equilibrium reach in %.3f second\n", t);
  // Else, we couldn't reach the equilibrium
  } else {
    fprintf(stderr,"Coudln't reach the equilibrium, start anyway the \
simulation with current state\n");
  }
  // Print the SpringSys
  fprintf(stdout, "SpringSys:\n");
  SpringSysPrint(theSpringSys, stdout);
  // Declare a variable to memorize the slope of the ground
  float slope = 0.1;
  // Create a TGA to draw the SpringSys
  white = TGAGetWhitePixel();
  lMax = 3.0;
  margin = 30;
  dim[0] = (int)round(lMax / lPixel) + margin * 2;
  dim[1] = (int)round(lMax / lPixel) + margin * 2;
  tga = TGACreate(dim, white);
  // If the TGA couldn't be created
  if (tga == NULL) {
    // Free memory
    SpringSysFree(&theSpringSys);
    SpringSysMassFree(&mass);
    SpringSysSpringFree(&spring);
    TGAFreePixel(&white);
    // Stop
    return 1;
  }
  // Draw the legend of the TGA
  DrawLegendTGA_2D(theSpringSys, tga, lMax, lPixel, margin, slope, k);
  // Draw the intial state to the TGA
  DrawTGA_2D(theSpringSys, tga, lPixel, margin);
  // Declare a variable to memorize the vector used during bouncing
  float v[2];
  v[0] = -1.0 * slope / sqrt(1.0 + pow(slope, 2.0));
  v[1] = 1.0 / sqrt(1.0 + pow(slope, 2.0));
  // Run the simulation
  t = 0.0;
  tMax = 30.0;
  int iFrame = 0;
  while (t < tMax) {
    // For each mass
    for (int iMass = 0; iMass < nbMass; ++iMass) {
      // Get the mass
      SpringSysMass *m = SpringSysGetMass(theSpringSys, iMass);
      // If the mass is not null
      if (m != NULL) {
        // Apply gravity
        m->_speed[1] -= 0.5 * m->_mass;
      }
    }
    // Step the SpringSys
    SpringSysStep(theSpringSys, dt);
    // For each mass
    for (int iMass = 0; iMass < nbMass; ++iMass) {
      // Get the mass
      SpringSysMass *m = SpringSysGetMass(theSpringSys, iMass);
      // If the mass is not null and has collided with the ground
      if (m != NULL && m->_pos[1] < m->_pos[0] * slope) {
        // Correct the mass position
        m->_pos[1] = m->_pos[0] * slope + 0.001;
        // Bounce the mass off the ground
        float ls = sqrt(pow(m->_speed[0], 2.0) + pow(m->_speed[1], 2.0));
        if (ls > SPRINGSYS_EPSILON) {
          float s[2];
          s[0] = m->_speed[0] / ls;
          s[1] = m->_speed[1] / ls;
          float dissip = 0.3 * (s[0] * v[0] + s[1] * v[1]);
          float w[2];
          w[0] = s[0] + v[0] * 2.0;
          w[1] = s[1] + v[1] * 2.0;
          float lw = sqrt(pow(w[0], 2.0) + pow(w[1], 2.0));
          m->_speed[0] = dissip * w[0] / lw * ls;
          m->_speed[1] = dissip * w[1] / lw * ls;
        } else {
          m->_speed[0] = 0.0;
          m->_speed[1] = 0.0;
        }
      }
    }
    // Draw the SpringSys
    DrawTGA_2D(theSpringSys, tga, lPixel, margin);
    // Save the frame for animation
    char fileName[100];
    sprintf(fileName, "./Frames/%02d.tga", iFrame);
    TGA* tgaFrame = TGACreate(dim, white);
    if (tgaFrame != NULL) {
      DrawLegendTGA_2D(theSpringSys, tgaFrame, 
        lMax, lPixel, margin, slope, k);
      DrawTGA_2D(theSpringSys, tgaFrame, lPixel, margin);
      TGASave(tgaFrame, fileName);
      TGAFree(&tgaFrame);
    }
    // Increment time and frame index
    t += dt;
    iFrame++;
  }
  // Save the TGA
  TGASave(tga, "./springSys2D.tga");
  // Free the TGA
  TGAFree(&tga);
  // Search the nearest mass to (1.0,1.0)
  printf("Nearest mass to (1.0,1.0):\n");
  float pos[2] = {1.0, 1.0};
  SpringSysMass *nearestMass = SpringSysGetMassByPos(theSpringSys, pos);
  printf("#%d %.3f,%.3f\n", nearestMass->_id, nearestMass->_pos[0],
    nearestMass->_pos[1]);
  // Search the nearest spring to (1.0,1.0)
  printf("Nearest spring to (1.0,1.0):\n");
  SpringSysSpring *nearestSpring = 
    SpringSysGetSpringByPos(theSpringSys, pos);
  printf("#%d %d,%d\n", nearestSpring->_id, nearestSpring->_mass[0],
    nearestSpring->_mass[1]);
  // Save the SpringSys
  printf("Saved SpringSys:\n");
  SpringSysPrint(theSpringSys, stdout);
  FILE *stream = fopen("./springsys.dat", "w");
  int ret = SpringSysSave(theSpringSys, stream);
  fclose(stream);
  if (ret != 0) {
    fprintf(stderr, "Couldn't save the SpringSys (%d)\n", ret);
  } else {
    stream = fopen("./springsys.dat", "r");
    SpringSys *loadSys = NULL;
    ret = SpringSysLoad(&loadSys, stream);
    if (ret != 0) {
      fprintf(stderr, "Couldn't load the SpringSys (%d)\n", ret);
    } else {
      printf("Loaded SpringSys:\n");
      SpringSysPrint(loadSys, stdout);
      SpringSysFree(&loadSys);
    }
    fclose(stream);
  }
  // Remove one spring 
  printf("Remove spring #4:\n");
  SpringSysRemoveSpring(theSpringSys, 4);
  SpringSysPrint(theSpringSys, stdout);
  // Remove one node 
  printf("Remove mass #3:\n");
  SpringSysRemoveMass(theSpringSys, 3);
  SpringSysPrint(theSpringSys, stdout);
  // Free memory
  TGAFreePixel(&white);
  SpringSysFree(&theSpringSys);
  SpringSysMassFree(&mass);
  SpringSysSpringFree(&spring);
  // Free memory
  SpringSysFree(&theSpringSys);
  return 0;
}


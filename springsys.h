// ============ SPRINGSYS.H ================

#ifndef SPRINGSYS_H
#define SPRINGSYS_H

// ================= Include =================

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "gset.h"

// ================= Define ==================

#define SPRINGSYS_EPSILON 0.0000001

// ================= Data structure ===================

typedef struct SpringSysMass {
  // ID
  int _id;
  // Position
  float _pos[3];
  // Speed
  float _speed[3];
  // Stress (acceleration due to the springs)
  float _stress[3];
  // Mass 
  float _mass;
  // Fixed flag, if true the mass doesn't move
  bool _fixed;
} SpringSysMass;

typedef struct SpringSysSpring {
  // ID
  int _id;
  // Current length
  float _length;
  // K coefficient
  float _k;
  // Length at rest
  float _restLength;
  // Stress (positive = extension, negative = compression)
  float _stress;
  // Limit stress (compression/extension)
  // If the current stress get over the limits the spring breaks (it
  // is removed from the list of springs)
  float _maxStress[2];
  // ID of the masses at the extremities of the spring
  int _mass[2];
  // Breakable flag, if true the spring breaks if its stress goes over 
  // the limit _maxStress
  bool _breakable;
} SpringSysSpring;

typedef struct SpringSys {
  // List of masses
  GSet *_masses;
  // List of springs
  GSet *_springs;
  // Number of dimension of the system (in [1, 3])
  int _nbDim;
  // Dissipation coefficient (applied to speed of masses at each step,
  // 0.0 = no dissipation, 1.0 = total dissipation)
  float _dissip;
} SpringSys;

// ================ Functions declaration ====================

// Create a new SpringSys with number of dimensions 'nbDim' (in [1,3])
// Default dissipation coefficient _dissip = 0.1
// Return NULL if we couldn't create the Springsys
SpringSys* SpringSysCreate(int nbDim);

// Clone the SpringSys 'sys'
// Return NULL if we couldn't clone the Springsys
SpringSys* SpringSysClone(SpringSys *sys);

// Load the SpringSys 'sys' from the stream 'stream'
// If 'sys' is already allocated, it is freed before loading
// Return 0 in case of success, or:
// 1: invalid arguments
// 2: can't allocate memory
// 3: invalid data
int SpringSysLoad(SpringSys **sys, FILE *stream);

// Save the SpringSys 'sys' to the stream
// Return 0 upon success, else
int SpringSysSave(SpringSys *sys, FILE *stream);

// Create a default mass, default properties' values are:
// _id = 0;
// _pos[0] = _pos[1] = _pos[2] = 0.0;
// _nextPos[0] = _nextPos[1] = _nextPos[2] = 0.0;
// _speed[0] = _speed[1] = _speed[2] = 0.0;
// _stress[0] = _stress[1] = _stress[2] = 0.0;
// _mass = 1.0;
// _fixed = false;
// Return NULL if memory allocation failed
SpringSysMass* SpringSysCreateMass(void);

// Create a default spring, default properties' values are:
// _id = 0;
// _length = 1.0;
// _k = 1.0;
// _restLength = 1.0;
// _stress = 0.0;
// _maxLength[0] = 0.0;
// _maxLength[1] = 100.0;
// _maxStress[0] = -1000000.0;
// _maxStress[1] = 1000000.0;
// _mass[0] = 0;
// _mass[1] = 0;
// _breakable = false
// Return NULL if memory allocation failed
SpringSysSpring* SpringSysCreateSpring(void);

// Free the memory used by a SpringSys
// Do nothing if arguments are invalid
void SpringSysFree(SpringSys **sys);

// Free the memory used by a SpringSysMass
// Do nothing if arguments are invalid
void SpringSysMassFree(SpringSysMass **m);

// Free the memory used by a SpringSysSpring
// Do nothing if arguments are invalid
void SpringSysSpringFree(SpringSysSpring **s);

// Print the SpringSys on 'stream'
// Do nothing if arguments are invalid
void SpringSysPrint(SpringSys *sys, FILE *stream);

// Print the SpringSysMass on 'stream'
// Do nothing if arguments are invalid
void SpringSysMassPrint(void *m, FILE *stream);

// Print the SpringSysSpring on 'stream'
// Do nothing if arguments are invalid
void SpringSysSpringPrint(void *s, FILE *stream);

// Set the dissipation coefficient of the SpringSys to 'dissip' 
// in [0.0,1.0]
// Do nothing if arguments are invalid
void SpringSysSetDissip(SpringSys *sys, float dissip);

// Get the mass identified by 'id'
// Return NULL if arguments are invalid or if there is no mass 
// with this id
SpringSysMass* SpringSysGetMass(SpringSys *sys, int id);

// Get the spring identified by 'id'
// Return NULL if arguments are invalid or if there is no spring 
// with this id
SpringSysSpring* SpringSysGetSpring(SpringSys *sys, int id);

// Get the number of mass in the SpringSys
// Return -1 if the argument are invalid
int SpringSysGetNbMass(SpringSys *sys);

// Get the number of spring in the SpringSys
// Return -1 if the argument are invalid
int SpringSysGetNbSpring(SpringSys *sys);

// Add a copy of the mass 'm' to the SpringSys
// Return false if the arguments are invalid or memory allocation failed
// else return true
bool SpringSysAddMass(SpringSys *sys, SpringSysMass *m);

// Add a copy of the spring 's' to the SpringSys
// Return false if the arguments are invalid or memory allocation failed
// else return true
bool SpringSysAddSpring(SpringSys *sys, SpringSysSpring *s);

// Remove the mass identified by 'id'
// Springs connected to this mass are removed as well
// Do nothing if arguments are invalids
void SpringSysRemoveMass(SpringSys *sys, int id);

// Remove spring idenitfied by 'id'
// Do nothing if argument are invalids
void SpringSysRemoveSpring(SpringSys *sys, int id);

// Step in time by 'dt' the SpringSys
// 'dt' must be carefully choosen, if too big inaccuracy of the 
// simulation leads to divergence and then to rupture of springs,
// especially if springs have a high mk coefficient 
// Do nothing if arguments are invalid
void SpringSysStep(SpringSys *sys, float dt);

// Step in time by 'dt' the SpringSys until it is in equilibrium 
// or 'tMax' has been reached
// 'dt' must be carefully choosen, if too big inaccuracy of the 
// simulation leads to divergence and then to rupture of springs,
// especially if springs have a high mk coefficient 
// Return a value > tMax if the arguments are invalid or the equilibrium
// couldn't be reached, else return the time it took to 
// reach equilibrium 
float SpringSysStepToRest(SpringSys *sys, float dt, float tMax);

// Get the momentum (sum of norm(v) of masses) of the SpringSys
// Return 0.0 if the arguments are invalid
float SpringSysGetMomentum(SpringSys *sys);

// Get the stress (sum of abs(stress) of springs) of the SpringSys
// Return 0.0 if the arguments are invalid
float SpringSysGetStress(SpringSys *sys);

// Get the nearest mass to 'pos' in the SpringSys 'sys'
// Return NULL if arguments are invalids
SpringSysMass* SpringSysGetMassByPos(SpringSys *sys, float *pos);

// Get the nearest spring to 'pos' in the SpringSys 'sys'
// Return NULL if arguments are invalids
SpringSysSpring* SpringSysGetSpringByPos(SpringSys *sys, float *pos);

#endif

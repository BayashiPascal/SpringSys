// ============ SPRINGSYS.C ================

#include "springsys.h"

// ================= Include =================

// Create a new SpringSys with number of dimensions 'nbDim' (in [1,3])
// Default dissipation coefficient _dissip = 0.01
// Return NULL if we couldn't create the Springsys
SpringSys* SpringSysCreate(int nbDim) {
  // Check arguments
  if (nbDim < 1 || nbDim > 3)
    return NULL;
  // Allocate memory for the new SpringSys
  SpringSys *ret = (SpringSys*)malloc(sizeof(SpringSys));
  // If we could allocate memory
  if (ret != NULL) {
    // Set the number of dimensions
    ret->_nbDim = nbDim;
    // Set the dissipation coefficient
    ret->_dissip = 0.1;
    // Create the gset of masses
    ret->_masses = GSetCreate();
    // If we couldn't create the gset
    if (ret->_masses == NULL) {
      // Free memory
      free(ret);
      // Return NULL
      return NULL;
    }
    // Create the gset of springs
    ret->_springs = GSetCreate();
    // If we couldn't create the gset
    if (ret->_springs == NULL) {
      // Free memory
      GSetFree(&(ret->_masses));
      free(ret);
      // Return NULL
      return NULL;
    }
  }
  return ret;
}

// Clone the SpringSys 'sys'
// Return NULL if we couldn't clone the Springsys
SpringSys* SpringSysClone(SpringSys *sys) {
  // Check arguments
  if (sys == NULL)
    return NULL;
  // Allocate memory for the new SpringSys
  SpringSys *ret = (SpringSys*)malloc(sizeof(SpringSys));
  // If we could allocate memory
  if (ret != NULL) {
    // Set the number of dimensions
    ret->_nbDim = sys->_nbDim;
    // Set the dissipation coefficient
    ret->_dissip = sys->_dissip;
    // Initialize the pointer to gsets of masses and springs
    ret->_masses = NULL;
    ret->_springs = NULL;
    // Create the gset of masses
    ret->_masses = GSetCreate();
    // If we couldn't create the gset
    if (ret->_masses == NULL) {
      // Free memory
      free(ret);
      // Return NULL
      return NULL;
    }
    // Create the gset of springs
    ret->_springs = GSetCreate();
    // If we couldn't create the gset
    if (ret->_springs == NULL) {
      // Free memory
      GSetFree(&(ret->_masses));
      free(ret);
      // Return NULL
      return NULL;
    }
    // If there is a gset of masses
    if (sys->_masses != NULL) {
      // Copy the masses
      GSetElem *m = sys->_masses->_head;
      while (m != NULL) {
        // Declare a variable to create the clone of the mass
        SpringSysMass *mass = NULL;
        // If the mass is not null in the SpringSys
        if (m->_data != NULL) {
          // Allocate memory for the clone of the mass
          mass = (SpringSysMass*)malloc(sizeof(SpringSysMass));
          // If we couldn't allocate memory
          if (mass == NULL) {
            // Free the memory
            SpringSysFree(&ret);
            // Return NULL
            return NULL;
          }
          // Clone the mass
          memcpy(mass, m->_data, sizeof(SpringSysMass));
        }
        // Get the current number of masses
        int nbMass = ret->_masses->_nbElem;
        // Append the mass
        GSetAppend(ret->_masses, mass);
        // If we couldn't append the mass
        if (nbMass + 1 != ret->_masses->_nbElem) {
          // Free the memory
          free(mass);
          SpringSysFree(&ret);
          // Return NULL
          return NULL;
        }
        // Move to the next element
        m = m->_next;
      }
    }
    // If there is a gset of springs
    if (sys->_springs != NULL) {
      // Copy the springs
      GSetElem *s = sys->_springs->_head;
      while (s != NULL) {
        // Declare a variable to create the clone of the spring
        SpringSysSpring *spring = NULL;
        // If the spring is not null in the SpringSys
        if (s->_data != NULL) {
          // Allocate memory for the clone of the spring
          spring = (SpringSysSpring*)malloc(sizeof(SpringSysSpring));
          // If we couldn't allocate memory
          if (spring == NULL) {
            // Free the memory
            SpringSysFree(&ret);
            // Return NULL
            return NULL;
          }
          // Clone the spring
          memcpy(spring, s->_data, sizeof(SpringSysSpring));
        }
        // Get the current number of springs
        int nbSpring = ret->_springs->_nbElem;
        // Append the spring
        GSetAppend(ret->_springs, spring);
        // If we couldn't append the spring
        if (nbSpring + 1 != ret->_springs->_nbElem) {
          // Free the memory
          free(spring);
          SpringSysFree(&ret);
          // Return NULL
          return NULL;
        }
        // Move to the next element
        s = s->_next;
      }
    }
  }
  return ret;
}

// Load the SpringSys 'sys' from the stream 'stream'
// If 'sys' is already allocated, it is freed before loading
// Return 0 in case of success, or:
// 1: invalid arguments
// 2: can't allocate memory
// 3: invalid data
int SpringSysLoad(SpringSys **sys, FILE *stream) {
  // Check arguments
  if (sys == NULL || stream == NULL)
    return 1;
  // If the SpringSys is already allocated
  if (*sys != NULL)
    // Free memory
    SpringSysFree(sys);
  // Read the number of dimension
  int nbDim;
  int ret = fscanf(stream, "%d\n", &nbDim);
  // Allocate memory for the SpringSys
  *sys = SpringSysCreate(nbDim);
  // If we couldn't allocate memory
  if (*sys == NULL)
    // Stop here
    return 2;
  // Create a mass to read the data
  SpringSysMass *mass = SpringSysCreateMass();
  if (mass == NULL) {
    SpringSysFree(sys);
    return 2;
  }
  // Create a spring to read the data
  SpringSysSpring *spring = SpringSysCreateSpring();
  if (spring == NULL) {
    SpringSysMassFree(&mass);
    SpringSysFree(sys);
    return 2;
  }
  // Read the number of mass
  int nbMass;
  ret = fscanf(stream, "%d\n", &nbMass);
  // For each mass
  for (int iMass = 0; iMass < nbMass; ++iMass) {
    // Read the properties of the mass
    ret = fscanf(stream, "%d\n", &(mass->_id));
    ret = fscanf(stream, "%f %f %f\n", &(mass->_pos[0]), 
      &(mass->_pos[1]), &(mass->_pos[2]));
    ret = fscanf(stream, "%f %f %f\n", &(mass->_speed[0]), 
      &(mass->_speed[1]), &(mass->_speed[2]));
    ret = fscanf(stream, "%f %f %f\n", &(mass->_stress[0]), 
      &(mass->_stress[1]), &(mass->_stress[2]));
    ret = fscanf(stream, "%f\n", &(mass->_mass));
    int b;
    ret = fscanf(stream, "%d\n", &b);
    mass->_fixed = b;
    // Add the mass
    bool ok = SpringSysAddMass(*sys, mass);
    // If we couldn't add the mass
    if (ok == false) {
      SpringSysMassFree(&mass);
      SpringSysSpringFree(&spring);
      SpringSysFree(sys);
      return 3;
    }
  }
  // Read the number of spring
  int nbSpring;
  ret = fscanf(stream, "%d\n", &nbSpring);
  // For each spring
  for (int iSpring = 0; iSpring < nbSpring; ++iSpring) {
    // Read the properties of the spring
    ret = fscanf(stream, "%d\n", &(spring->_id));
    ret = fscanf(stream, "%f\n", &(spring->_length));
    ret = fscanf(stream, "%f\n", &(spring->_k));
    ret = fscanf(stream, "%f\n", &(spring->_restLength));
    ret = fscanf(stream, "%f\n", &(spring->_stress));
    ret = fscanf(stream, "%f %f\n", &(spring->_maxStress[0]), 
      &(spring->_maxStress[1]));
    ret = fscanf(stream, "%d %d\n", &(spring->_mass[0]), 
      &(spring->_mass[1]));
    int b;
    ret = fscanf(stream, "%d\n", &b);
    spring->_breakable = b;
    // Add the spring
    bool ok = SpringSysAddSpring(*sys, spring);
    // If we couldn't add the spring
    if (ok == false) {
      SpringSysMassFree(&mass);
      SpringSysSpringFree(&spring);
      SpringSysFree(sys);
      return 3;
    }
  }
  // TODO don't ignore the return value
  ret = ret;
  // Free memory
  SpringSysMassFree(&mass);
  SpringSysSpringFree(&spring);
  // Return success code
  return 0;
}

// Save the SpringSys 'sys' to the stream
// Return 0 upon success, else
// 1: invalid argument
// 2: invalid SpringSys
int SpringSysSave(SpringSys *sys, FILE *stream) {
  // Check arguments
  if (sys == NULL || sys->_masses == NULL || 
    sys->_springs == NULL || stream == NULL)
    return 1;
  // Write the number of dimensions
  fprintf(stream, "%d\n", sys->_nbDim);
  // Write the number of masses
  fprintf(stream, "%d\n", sys->_masses->_nbElem);
  // Get a pointer to the first element of the list of masses
  GSetElem *e = sys->_masses->_head;
  // While we are not at the end of the list
  while (e != NULL) {
    // Get a pointer to the mass
    SpringSysMass *m = (SpringSysMass*)(e->_data);
    // If the pointer is not null
    if (m != NULL) {
      // Write the mass properties
      fprintf(stream, "%d\n", m->_id);
      fprintf(stream, "%f %f %f\n", m->_pos[0], m->_pos[1], m->_pos[2]);
      fprintf(stream, "%f %f %f\n", m->_speed[0], m->_speed[1], 
        m->_speed[2]);
      fprintf(stream, "%f %f %f\n", m->_stress[0], m->_stress[1], 
        m->_stress[2]);
      fprintf(stream, "%f\n", m->_mass);
      fprintf(stream, "%d\n", m->_fixed);
    // Else, the pointer is null
    } else {
      // This should never happen
      return 2;
    }
    // Move to the next element
    e = e->_next;
  }
  // Write the number of springs
  fprintf(stream, "%d\n", sys->_springs->_nbElem);
  // Get a pointer to the first element of the list of springs
  e = sys->_springs->_head;
  // While we are not at the end of the list
  while (e != NULL) {
    // Get a pointer to the spring
    SpringSysSpring *s = (SpringSysSpring*)(e->_data);
    // If the pointer is not null
    if (s != NULL) {
      // Write the spring properties
      fprintf(stream, "%d\n", s->_id);
      fprintf(stream, "%f\n", s->_length);
      fprintf(stream, "%f\n", s->_k);
      fprintf(stream, "%f\n", s->_restLength);
      fprintf(stream, "%f\n", s->_stress);
      fprintf(stream, "%f %f\n", s->_maxStress[0], s->_maxStress[1]);
      fprintf(stream, "%d %d\n", s->_mass[0], s->_mass[1]);
      fprintf(stream, "%d\n", s->_breakable);
    // Else, the pointer is null
    } else {
      // This should never happen
      return 2;
    }
    // Move to the next element
    e = e->_next;
  }
  return 0;
}

// Create a default mass
// Return NULL if memory allocation failed
SpringSysMass* SpringSysCreateMass(void) {
  // Allocate memory
  SpringSysMass *ret = (SpringSysMass*)malloc(sizeof(SpringSysMass));
  // If we could allocate memory
  if (ret != NULL) {
    // Set default values for the properties
    ret->_id = 0;
    ret->_pos[0] = ret->_pos[1] = ret->_pos[2] = 0.0;
    ret->_speed[0] = ret->_speed[1] = ret->_speed[2] = 0.0;
    ret->_stress[0] = ret->_stress[1] = ret->_stress[2] = 0.0;
    ret->_mass = 1.0;
    ret->_fixed = false;
  }
  // Return the new mass
  return ret;
}

// Create a default spring
// Return NULL if memory allocation failed
SpringSysSpring* SpringSysCreateSpring(void) {
  // Allocate memory
  SpringSysSpring *ret = 
    (SpringSysSpring*)malloc(sizeof(SpringSysSpring));
  // If we could allocate memory
  if (ret != NULL) {
    // Set default values for the properties
    ret->_id = 0;
    ret->_length = 1.0;
    ret->_k = 1.0;
    ret->_restLength = 1.0;
    ret->_stress = 0.0;
    ret->_maxStress[0] = -1000000.0;
    ret->_maxStress[1] = 1000000.0;
    ret->_mass[0] = 0;
    ret->_mass[1] = 0;
    ret->_breakable = false;
  }
  // Return the new spring
  return ret;
}

// Free the memory used by a SpringSys
// Do nothing if arguments are invalid
void SpringSysFree(SpringSys **sys) {
  // Check arguments
  if (sys == NULL || *sys == NULL)
    return;
  // If there is a gset of masses
  if ((*sys)->_masses != NULL) {
    // Free the memory used by masses
    GSetElem *m = (*sys)->_masses->_head;
    while (m != NULL) {
      SpringSysMassFree((SpringSysMass**)(&(m->_data)));
      m = m->_next;
    }
  }
  // If there is a gset of springs
  if ((*sys)->_springs != NULL) {
    // Free the memory used by springs
    GSetElem *s = (*sys)->_springs->_head;
    while (s != NULL) {
      SpringSysSpringFree((SpringSysSpring**)(&(s->_data)));
      s = s->_next;
    }
  }
  // Free the gsets
  GSetFree(&((*sys)->_masses));
  GSetFree(&((*sys)->_springs));
  // Free memory
  free(*sys);
  *sys = NULL;
}

// Free the memory used by a SpringSysMass
// Do nothing if arguments are invalid
void SpringSysMassFree(SpringSysMass **m) {
  // Check arguments
  if (m == NULL || *m == NULL)
    return;
  // Free the memory
  free(*m);
  *m = NULL;
}

// Free the memory used by a SpringSysSpring
// Do nothing if arguments are invalid
void SpringSysSpringFree(SpringSysSpring **s) {
  // Check arguments
  if (s == NULL || *s == NULL)
    return;
  // Free the memory
  free(*s);
  *s = NULL;
}

// Print the SpringSys on 'stream'
// Do nothing if arguments are invalid
void SpringSysPrint(SpringSys *sys, FILE *stream) {
  // Check arguments
  if (sys == NULL || stream == NULL)
    return;
  // Print the number of dimension
  fprintf(stream, "Number of dimension: %d\n", sys->_nbDim);
  // Print the dissipation
  fprintf(stream, "Dissipation: %.3f\n", sys->_dissip);
  // Print the masses
  fprintf(stream, "Masses:\n");
  GSetPrint(sys->_masses, stream, &SpringSysMassPrint, (char*)"\n");
  fprintf(stream, "\n");
  // Print the springs
  fprintf(stream, "Springs:\n");
  GSetPrint(sys->_springs, stream, &SpringSysSpringPrint, (char*)"\n");
  fprintf(stream, "\n");
}

// Print the SpringSysMass on 'stream'
// Do nothing if arguments are invalid
void SpringSysMassPrint(void *m, FILE *stream) {
  // Check arguments
  if (m == NULL || stream == NULL)
    return;
  // Print the mass properties
  fprintf(stream, "#%d, ", ((SpringSysMass*)m)->_id);
  fprintf(stream, "pos(%.3f,%.3f,%.3f), ", 
    ((SpringSysMass*)m)->_pos[0], ((SpringSysMass*)m)->_pos[1], 
    ((SpringSysMass*)m)->_pos[2]);
  fprintf(stream, "speed(%.3f,%.3f,%.3f), ", 
    ((SpringSysMass*)m)->_speed[0], ((SpringSysMass*)m)->_speed[1], 
    ((SpringSysMass*)m)->_speed[2]);
  fprintf(stream, "stress(%.3f,%.3f,%.3f), ", 
    ((SpringSysMass*)m)->_stress[0], ((SpringSysMass*)m)->_stress[1], 
    ((SpringSysMass*)m)->_stress[2]);
  fprintf(stream, "mass(%.3f), ", ((SpringSysMass*)m)->_mass);
  fprintf(stream, "fixed(%d)", ((SpringSysMass*)m)->_fixed);
}

// Print the SpringSysSpring on 'stream'
// Do nothing if arguments are invalid
void SpringSysSpringPrint(void *s, FILE *stream) {
  // Check arguments
  if (s == NULL || stream == NULL)
    return;
  // Print the spring properties
  fprintf(stream, "#%d, ", ((SpringSysSpring*)s)->_id);
  fprintf(stream, "%d-%d, ", ((SpringSysSpring*)s)->_mass[0], 
    ((SpringSysSpring*)s)->_mass[1]);
  fprintf(stream, "length(%.3f), ", ((SpringSysSpring*)s)->_length);
  fprintf(stream, "stress(%.3f), ", ((SpringSysSpring*)s)->_stress);
  fprintf(stream, "k(%.3f), ", ((SpringSysSpring*)s)->_k);
  fprintf(stream, "restLength(%.3f), ", 
    ((SpringSysSpring*)s)->_restLength);
  fprintf(stream, "maxStress(%.3f,%.3f), ", 
    ((SpringSysSpring*)s)->_maxStress[0],
    ((SpringSysSpring*)s)->_maxStress[1]);
  fprintf(stream, "breakable(%d)", ((SpringSysSpring*)s)->_breakable);
}

// Set the dissipation coefficient of the SpringSys to 'dissip' 
// in [0.0,1.0]
// Do nothing if arguments are invalid
void SpringSysSetDissip(SpringSys *sys, float dissip) {
  // Check arguments
  if (sys == NULL || dissip < 0.0 || dissip > 1.0)
    return;
  // Set the dissipation
  sys->_dissip = dissip;
}

// Get the mass identified by 'id'
// Return NULL if arguments are invalid or if there is no mass 
// with this id
SpringSysMass* SpringSysGetMass(SpringSys *sys, int id) {
  // Check arguments
  if (sys == NULL)
    return NULL;
  // Declare a pointer to memorize the searched mass
  SpringSysMass *ret = NULL;
  // Get a pointer to the first element in list of masses
  GSetElem *m = sys->_masses->_head;
  // While we are not at the end of the list
  while (m != NULL) {
    // If the current mass is the searched mass
    if (((SpringSysMass*)(m->_data))->_id == id) {
      // Update the result pointer
      ret = (SpringSysMass*)(m->_data);
      // Set the pointer to element to null to end the loop
      m = NULL;
    // Else, the current ass is not the searched mass
    } else
      // Move to the next element
      m = m->_next;
  }
  // Return the result pointer
  return ret;
}

// Get the spring identified by 'id'
// Return NULL if arguments are invalid or if there is no spring 
// with this id
SpringSysSpring* SpringSysGetSpring(SpringSys *sys, int id) {
  // Check arguments
  if (sys == NULL || sys->_springs == NULL)
    return NULL;
  // Declare a pointer to memorize the searched spring
  SpringSysSpring *ret = NULL;
  // Get a pointer to the first element of the list of springs
  GSetElem *m = sys->_springs->_head;
  // While we are not at the end of the list
  while (m != NULL) {
    // If the current spring is the searched one
    if (((SpringSysSpring*)(m->_data))->_id == id) {
      // Update the result pointer
      ret = (SpringSysSpring*)(m->_data);
      // Set the pointer to element to null to end the loop
      m = NULL;
    // Else, the current spring is not the searched one
    } else {
      // Move to the next element
      m = m->_next;
    }
  }
  // Return the result pointer
  return ret;
}

// Get the number of mass in the SpringSys
// Return -1 if the argument are invalid
int SpringSysGetNbMass(SpringSys *sys) {
  // Check arguments
  if (sys == NULL || sys->_masses == NULL)
    return -1;
  // Return the number of masses
  return sys->_masses->_nbElem;  
}

// Get the number of spring in the SpringSys
// Return -1 if the argument are invalid
int SpringSysGetNbSpring(SpringSys *sys) {
  // Check arguments
  if (sys == NULL || sys->_springs == NULL)
    return -1;
  // Return the number of springs
  return sys->_springs->_nbElem;
}

// Add a copy of the mass 'm' to the SpringSys
// Return false if the arguments are invalid or memory allocation failed
// else return true
bool SpringSysAddMass(SpringSys *sys, SpringSysMass *m) {
  // Check arguments
  if (sys == NULL || m == NULL)
    return false;
  // If the mass properties are incorrect
  if (m->_mass < 0.0)
    // Return false
    return false;
  // Allocate memory for the new mass
  SpringSysMass *mass = (SpringSysMass*)malloc(sizeof(SpringSysMass));
  // If we could allocate memory
  if (mass != NULL) {
    // Copy the properties of the mass
    memcpy(mass, m, sizeof(SpringSysMass));
    // Add the mass
    GSetAppend(sys->_masses, mass);
  // Else, we couldn't allocate the memory
  } else
    // Return false
    return false;
  // Return true
  return true;
}

// Add a copy of the spring 's' to the SpringSys
// Return false if the arguments are invalid or memory allocation failed
// else return true
bool SpringSysAddSpring(SpringSys *sys, SpringSysSpring *s) {
  // Check arguments
  if (sys == NULL || s == NULL)
    return false;
  // If the spring properties are incorrect
  if (s->_mass[0] == s->_mass[1] || s->_length < 0.0 || 
    s->_k < 0.0 || s->_restLength < 0.0 ||
    s->_maxStress[0] >= 0.0 || s->_maxStress[1] <= 0.0)
    // Return false
    return false;
  SpringSysMass *m[2];
  m[0] = SpringSysGetMass(sys, s->_mass[0]);
  m[1] = SpringSysGetMass(sys, s->_mass[1]);
  if (m[0] == NULL || m[1] == NULL)
    // Return false
    return false;
  // Allocate memory for the new spring
  SpringSysSpring *spring = 
    (SpringSysSpring*)malloc(sizeof(SpringSysSpring));
  // If we could allocate memory
  if (spring != NULL) {
    // Copy the properties of the spring
    memcpy(spring, s, sizeof(SpringSysSpring));
    // Add the spring to the list of springs
    GSetAppend(sys->_springs, spring);
  // Else, we couldn't allocate the memory
  } else
    // Return false
    return false;
  // Return true
  return true;
}

// Remove the mass identified by 'id'
// Springs connected to this mass are removed as well
// Do nothing if arguments are invalids
void SpringSysRemoveMass(SpringSys *sys, int id) {
  // Check arguments
  if (sys == NULL)
    return;
  // Get a pointer to the first element in the list of mass
  GSetElem *e = sys->_masses->_head;
  // While we are not at the end of the list
  while (e != NULL) {
    // Get a pointer to the mass
    SpringSysMass *m = (SpringSysMass*)(e->_data);
    // Move to next mass
    e = e->_next;
    // If the pointer is not null and this is the searched mass
    if (m != NULL && m->_id == id) {
      // Remove the element
      GSetRemoveFirst(sys->_masses, m);
      // Free memory used by the mass
      SpringSysMassFree(&m);
    }
  }
  // Get a pointer to the first element in the list of spring
  e = sys->_springs->_head;
  // While we are not at the end of the list
  while (e != NULL) {
    // Get a pointer to the spring
    SpringSysSpring *s = (SpringSysSpring*)(e->_data);
    // Move to next spring
    e = e->_next;
    // If the pointer is not null and this is a spring connected to 
    // the searched mass
    if (s != NULL && (s->_mass[0] == id || s->_mass[1] == id)) {
      // Remove the element
      GSetRemoveFirst(sys->_springs, s);
      // Free memory used by the spring
      SpringSysSpringFree(&s);
    }
  }
}

// Remove spring idenitfied by 'id'
// Do nothing if argument are invalids
void SpringSysRemoveSpring(SpringSys *sys, int id) {
  // Check arguments
  if (sys == NULL)
    return;
  // Get a pointer to the first element in the list of spring
  GSetElem *e = sys->_springs->_head;
  // While we are not at the end of the list
  while (e != NULL) {
    // Get a pointer to the spring
    SpringSysSpring *s = (SpringSysSpring*)(e->_data);
    // Move to next spring
    e = e->_next;
    // If the pointer is not null and this is the searched spring
    if (s != NULL && s->_id == id) {
      // Remove the element
      GSetRemoveFirst(sys->_springs, s);
      // Free memory used by the spring
      SpringSysSpringFree(&s);
    }
  }
}

// Step in time by 'dt' the SpringSys
// Do nothing if arguments are invalid
void SpringSysStep(SpringSys *sys, float dt) {
  // Check arguments
  if (sys == NULL || dt <= 0.0 || sys->_masses == NULL || 
    sys->_springs == NULL)
    return;
  // Reset the stress for each unfixed mass
  // Get a pointer to the first element in the list of mass
  GSetElem *e = sys->_masses->_head;
  // While we are not at the end of the list
  while (e != NULL) {
    // Get a pointer to the mass
    SpringSysMass *m = (SpringSysMass*)(e->_data);
    // If the pointer is not null and the mass is not fixed
    if (m != NULL && m->_fixed == false)
      // For each dimension
      for (int iDim = 0; iDim < sys->_nbDim; ++iDim)
        // Reset the stress
        m->_stress[iDim] = 0.0;
    // Move to next mass
    e = e->_next;
  }
  // Update length and stress of each springs
  e = sys->_springs->_head;
  while (e != NULL) {
    // Get a pointer to the spring
    SpringSysSpring *s = (SpringSysSpring*)(e->_data);
    // If the pointer is not null
    if (s != NULL) {
      // Declare a variable to memorize if there has been a rupture
      bool flagRupture = false;
      // Get the two masses at extremities of the spring
      SpringSysMass* m[2];
      m[0] = SpringSysGetMass(sys, s->_mass[0]);
      m[1] = SpringSysGetMass(sys, s->_mass[1]);
      // If both masses are not null
      if (m[0] != NULL && m[1] != NULL) {
        // Get the distance between the masses
        float l = 0.0;
        for (int iDim = 0; iDim < sys->_nbDim; ++iDim)
          l += pow(m[0]->_pos[iDim] - m[1]->_pos[iDim], 2.0);
        s->_length = sqrt(l);
        // Get the stress 
        s->_stress = (s->_length - s->_restLength) * s->_k;
        // If the spring is breakable, check for rupture
        if (s->_breakable == true &&
          ((s->_stress > 0.0 && s->_stress >= s->_maxStress[1]) ||
          (s->_stress < 0.0 && s->_stress <= s->_maxStress[0]))) {
          // Memorize there has been a rupture
          flagRupture = true;
          // Remove this spring from the sets of spring
          GSetRemoveFirst(sys->_springs, s);
          // Free memory for the spring
          SpringSysSpringFree(&s);
        } else {
          // Update the stress to the masses which are not fixed
          for (int iDim = 0; iDim < sys->_nbDim; ++iDim) {
            for (int iMass = 0; iMass < 2; ++iMass) {
              float d = s->_length * (1.0 + m[iMass]->_mass);
              if (m[iMass]->_fixed == false && d > SPRINGSYS_EPSILON)
                m[iMass]->_stress[iDim] += s->_stress *
                  (m[1 - iMass]->_pos[iDim] - m[iMass]->_pos[iDim]) / d;
            }
          }
        }
      }
      // If there has been no rupture
      if (flagRupture == false) {
        // Move to the next spring
        e = e->_next;
      }
      // Else, the pointer is yet on the following element
    // Else, the pointer to the spring is null
    } else {
      // Move to the next element in list
      e = e->_next;
    }
  }
  // Apply speed to masses which are not fixed
  // Get a pointer to the first element of the list of mass
  e = sys->_masses->_head;
  // While we are not at the end of the list
  while (e != NULL) {
    // Get a pointer to the mass
    SpringSysMass *m = (SpringSysMass*)(e->_data);
    // If the pointer is not null and the mass is not fixed
    if (m != NULL && m->_fixed == false)
      // For each dimension
      for (int iDim = 0; iDim < sys->_nbDim; ++iDim) {
        // Apply the dissipation to the speed
        m->_speed[iDim] *= pow(1.0 - sys->_dissip, dt);
        // Apply the stress to the speed
        m->_speed[iDim] += m->_stress[iDim] * dt;
        // Apply the speed to the position
        m->_pos[iDim] += m->_speed[iDim] * dt;
      }
    // Move to next mass
    e = e->_next;
  }
}

// Step in time by 'dt' the SpringSys until it is in equilibrium 
// or 'tMax' has been reached
// Return a value > tMax if the arguments are invalid or the equilibrium
// couldn't be reached, else return the time it took to 
// reach equilibrium 
float SpringSysStepToRest(SpringSys *sys, float dt, float tMax) {
  // Declare a variable to memorize time
  float t = tMax + dt; 
  // If arguments are valid
  if (sys != NULL && dt > 0.0 && tMax > dt) {
    // Declare a variable to memorize the momentum of the system
    float m = 0.0;
    // Declare variables to memorize the stress of the system at current
    // step and previous step
    float s = 0.0;
    float sp = 0.0;
    // Loop until the momentum is null and the stress stops varying or 
    // tMax is reached
    t = 0.0;
    do {
      // Update current stress
      s = sp;
      // Step the SpringSys
      SpringSysStep(sys, dt);
      // Get the momentum
      m = SpringSysGetMomentum(sys);
      // Get the stress
      sp = SpringSysGetStress(sys);
      // Increment time
      t += dt;
    } while ((m > SPRINGSYS_EPSILON || 
      fabs(sp - s) > SPRINGSYS_EPSILON) && t <= tMax);
  }
  // Return the time
  return t;
}

// Get the momentum (sum of norm(v) of masses) of the SpringSys
// Return 0.0 if the arguments are invalid
float SpringSysGetMomentum(SpringSys *sys) {
  // Check arguments
  if (sys == NULL || sys->_masses == NULL)
    return 0.0;
  // Declare a variable to memorize the sum
  float sum = 0.0;
  // Declare a pointer to the first element of the list of masses
  GSetElem *e = sys->_masses->_head;
  // While we are not at the end of the list
  while (e != NULL) {
    // Declare a pointer to the mass
    SpringSysMass *m = (SpringSysMass*)(e->_data);
    // If the pointer is not null
    if (m != NULL) {
      // Declare a variable to calculate the norm of the speed 
      // of the mass
      float v = 0.0;
      // Calculate the norm of the speed of the mass and sum it
      for (int iDim = 0; iDim < sys->_nbDim; ++iDim)
        v += pow(m->_speed[iDim], 2.0);
      sum += sqrt(v);
    }
    // Move to the next mass
    e = e->_next;
  }
  // Return the sum
  return sum;
}

// Get the stress (sum of abs(stress) of springs) of the SpringSys
// Return 0.0 if the arguments are invalid
float SpringSysGetStress(SpringSys *sys) {
  // Check arguments
  if (sys == NULL || sys->_springs == NULL)
    return 0.0;
  // Declare a variable to memorize the sum
  float sum = 0.0;
  // Declare a pointer to the first element of the list of springs
  GSetElem *e = sys->_springs->_head;
  // While we are not at the end of the list
  while (e != NULL) {
    // Get a pointer toward the spring
    SpringSysSpring *s = (SpringSysSpring*)(e->_data);
    // If the pointer is not null
    if (s != NULL)
      // Add the absolute value of the stress of the spring
      sum += fabs(s->_stress);
    // Move to the next spring
    e = e->_next;
  }
  // Return the sum
  return sum;  
}

// Get the nearest mass to 'pos' in the SpringSys 'sys'
// Return NULL if arguments are invalids
SpringSysMass* SpringSysGetMassByPos(SpringSys *sys, float *pos) {
  // Check arguments
  if (sys == NULL || pos == NULL || sys->_masses == NULL)
    return NULL;
  // Declare a pointer to memorize the nearest mass
  SpringSysMass *ret = NULL;
  // Declare a variable to memorize the distance to nearest mass
  float d = 0.0;
  // Declare a pointer to the first element of the list of masses
  GSetElem *e = sys->_masses->_head;
  // While we are not at the end of the list
  while (e != NULL) {
    // Declare a pointer to the mass
    SpringSysMass *m = (SpringSysMass*)(e->_data);
    // If the pointer is not null
    if (m != NULL) {
      // Declare a variable to calculate the distance
      float v = 0.0;
      // Calculate the distance
      for (int iDim = 0; iDim < sys->_nbDim; ++iDim)
        v += pow(m->_pos[iDim] - pos[iDim], 2.0);
      v = sqrt(v);
      // If the distance is shorter than the current one
      if (ret == NULL || d > v) {
        // Update the distance
        d = v;
        // Update the nearest mass
        ret = m;
      }
    }
    // Move to the next mass
    e = e->_next;
  }
  // Return the nearest mass
  return ret;
}

// Get the nearest spring to 'pos' in the SpringSys 'sys'
// Return NULL if arguments are invalids
SpringSysSpring* SpringSysGetSpringByPos(SpringSys *sys, float *pos) {
  // Check arguments
  if (sys == NULL || pos == NULL || sys->_springs == NULL || 
    sys->_masses == NULL)
    return NULL;
  // Declare a pointer to memorize the nearest spring
  SpringSysSpring *ret = NULL;
  // Declare a variable to memorize the distance to nearest mass
  float d = 0.0;
  // Declare a pointer to the first element of the list of springs
  GSetElem *e = sys->_springs->_head;
  // While we are not at the end of the list
  while (e != NULL) {
    // Get a pointer toward the spring
    SpringSysSpring *s = (SpringSysSpring*)(e->_data);
    // If the pointer is not null
    if (s != NULL) {
      // Get the two masses at the extremity of the spring
      SpringSysMass *mA = SpringSysGetMass(sys, s->_mass[0]);
      SpringSysMass *mB = SpringSysGetMass(sys, s->_mass[1]);
      if (mA != NULL && mB != NULL) {
        // Declare a variable to memorize the center of the spring
        float center[2];
        // Calculate the center of the spring
        center[0] = 0.5 * (mA->_pos[0] + mB->_pos[0]);
        center[1] = 0.5 * (mA->_pos[1] + mB->_pos[1]);
        // Declare a variable to calculate the distance
        float v = 0.0;
        // Calculate the distance
        for (int iDim = 0; iDim < sys->_nbDim; ++iDim)
          v += pow(center[iDim] - pos[iDim], 2.0);
        v = sqrt(v);
        // If the distance is shorter than the current one
        if (ret == NULL || d > v) {
          // Update the distance
          d = v;
          // Update the nearest spring
          ret = s;
        }
      }
    }
    // Move to the next spring
    e = e->_next;
  }
  // Return the nearest spring
  return ret;  
}


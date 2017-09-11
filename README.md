# SpringSys
SpringSys is a C library to simulate systems of masses connected by springs.

The number of dimensions of the system can be 1,2 or 3. It has a dissipation coefficient used to simulate dissipation of energy and dampen the system behaviour. A mass is defined by its mass, position and speed. A spring is defined by its rigidity coefficient, length (min, max, current and at rest), and the 2 masses it connects. A spring an be unbreakable or breakable (under stress limit condition).

SpringSys offers functions to create the system by adding/removing masses and springs or by cloning another SpringSys, to step in time the system, to step it until it reach equilibrium, to print it, to get the total stress and momentum of the system, to load ans save the system to a text file, to get the nearest mass or spring to a given position.

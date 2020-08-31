# exercises
Solved oi problems

## Mistakes

Mistakes in code.

### Generator
 1. Generate random array should like this:
 ``` cpp
 // default_rnd is random number generator, dis is Random number distribution.
 generate_n(ostream_iterator<unsigned int>(in, " "), m, bind(dis, ref(default_rnd)));
 ```
 
 instead of
 
 ``` cpp
 // default_rnd is random number generator, dis is Random number distribution.
 generate_n(ostream_iterator<unsigned int>(in, " "), m, bind(dis, default_rnd));
 ```

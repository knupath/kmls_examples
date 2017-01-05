# Knureon Job Queue Example: kpi_pirand

## Description:
  Uses a randomized number seeded against time

  > // Randomizes how many increments of the power of 2 to run

  > srand(time(NULL)); // Seed the time

  > int max = 31; // 2^30 max power of 2 for int coordsPerRank

  > int min = 2; // Minimum, do 1 run

  > int runLimit = rand()%(max-min + 1) + min;


## Code Description:
  The kpi_pirand example program uses the KPI Host Runtime C++ wrapper library to
  estimate a value for pi via a Monte Carlo method.

  The host application runs through several iterations with different numbers
  of (x, y) coordinates which will be set as an argument for the piRandRation
  kernel, found in the source file *pirand.knucpp*.

  Each tDSP kernel (or rank) will initialize the hardware random number generator
  using seed values that are dependent on the specific rank. The tDSP will
  then generate pairs of random values, and, using a technique known as
  rejection sampling, estimate a value for pi based on determining whether
  or not a point with those values in (x,y) coordinates is located within
  a unit circle.

## Expected output:
  Output can be found in *example_output.csv*.

  > Operating on 64 clusters.

  > Iteration, coordsPerRank, # Samples,  Time,  Pi Value

  > 1, 64, 2,  1024,  0.004429,  3.13894

  > Total Time: 0.164694

## Build and run:
  1. Make sure your SDB environmental variable is set:

    > export KNU_DEVICE=sdb045

  2. Run *make* in example application directory
  3. Execute the compiled example code using *./[example application]* in example application directory
    * Note: This application is compiled as *example*

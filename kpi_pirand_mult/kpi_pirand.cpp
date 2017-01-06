/******************************************************************************
  Copyright 2016
  All rights reserved.
  KNUPATH Proprietary and Confidential

  File: kpi_pirand.cpp

  Description: Host code for KPI pi estimation example using Monte Carlo method

******************************************************************************/

#include "kpi/kpi_runtime.hpp"
#include <iostream>
#include <fstream>
#include <ctime>

int main(int argc, char* argv[])
{
  // Get start of entire runtime
  std::clock_t begin_run = std::clock();

  // Create a context for complex 0 (the default)
  kpi::Context        ctx;

  // Create a command queue to submit tasks to the complex
  kpi::CommandQueue   cq = ctx.CreateCommandQueue();

  // Allocate all available clusters within the complex (SDB = 64)
  kpi::ClusterBlock   cb = ctx.AllocClusterBlock();

  // Write to an output file
  std::ofstream output;
  output.open("example_output.csv");

  // Output Info
  output << "Operating on " << cb.Size() << " clusters." << std::endl << std::endl;
  output << "Iteration, World Size, coordsPerRank, # Samples, Iteration RunTime (Sec), Pi Value" << std::endl;

  // Run Limit (10 = 1 billion per tdsp)
  // Current system only works on runLimit = 2
  int runLimit = 1;

  // Set first coordsPerRank to 1
  int coordsPerRank = 1;

  // Run in increments of the power of 2
  for (int i=1; i<=runLimit; i+=1)
  {
    // Get start of each iteration
    std::clock_t begin = std::clock();

    // Calculates # of coordinates per rank
    coordsPerRank *= 10;

    // Load a kernel and set the argument to the coordsPerRank
    kpi::KernelWithArgs kwa = ctx.CreateKernelWithArgs("piRandRatio", coordsPerRank);

    // Submit the launch configuration to the queue. Use the returned
    // host connection to communicate with the kernels during while
    // the launch is running.
    kpi::HostConn hc =  cq.SubmitWithHostConn(
                          ctx.CreateLaunchConfig(
                            cb.Size(),
				                    kpi::ProcGroup(cb),
                            kwa),
                           cb);

    // Wait to receive a flit from RANK 0
    std::vector<float> pi_estimate(1);
    hc.ReceiveCount(pi_estimate);

    std::vector<int> world_size(1);
    hc.ReceiveCount(world_size);

    // Output Run Info
    output << i << ", " << (world_size[0]-1) << ", " << coordsPerRank << ",  " << double(coordsPerRank)*cb.Size()*8-1<< ",  "
      << double(std::clock() - begin)/CLOCKS_PER_SEC << ",  " << pi_estimate[0] << std::endl;
  }

  // Write total time it took to run program
  output << std::endl << "Total Time (sec): " << double(std::clock() - begin_run)/CLOCKS_PER_SEC << std::endl;

  // Close file and exit
  output.close();
  return 0;
}

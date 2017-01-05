/******************************************************************************
  Copyright 2016 KNUPATH
  All rights reserved.
  KnuPath Proprietary and Confidential

  File: main.cpp

  Description: Host code for KPI Adder. Host will send kernel:adder each element of
  data[] to add up. Kernel:adder will add each element and return.

******************************************************************************/
#include <kpi/kpi_runtime.hpp>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{

  // ****************************************************************************
  // Code to Complex Mapping Initialization
  // ****************************************************************************

    // Create a context for complex 0 (the default)
    kpi::Context ctx;

    // Load the "echo" kernel
    kpi::Kernel k = ctx.CreateKernel("adder");

    // Create a processor group with 1 tDSP on 1 cluster
    kpi::ProcGroup pg(0, 1, 1,            // cluster start, stride, count
                      0, 1, 1);           // proc    start, stride, count

    // Allocate a cluster block with 1 clusters, and locate it at the
    // start of the complex
    kpi::ClusterBlock cb = ctx.AllocClusterBlockAt(1,         // num clusters
                                                   0);        // offset

    // Initialize Kernel Arguments. Needed even if no Kernel Args exist
    kpi::KernelArgs           ka = k.CreateKernelArgs();

    // Create a command queue to submit launches
    kpi::CommandQueue         cq = ctx.CreateCommandQueue();

    // Create a launch configuration
    kpi::LaunchConfig         lc = ctx.CreateLaunchConfig(cb.Size());

    // Add the kernel to the launch configuration
    lc.Add(pg, k, ka);

    // Submit the launch configuration to the queue. Use the returned
    // host connection to communicate with the kernels during while
    // the launch is running.
    kpi::HostConn             hc = cq.SubmitWithHostConn(lc, cb);


  // ****************************************************************************
  // Host Code
  // ****************************************************************************

  // Initialize and define values
  int i;
  int data[] = {1, 1, 1, 2, 2, 2, 3, 4, 5, 6, 7, -1, -33};

  // Define flit_buffer to size 1. In this example, we only communicate with
  // 1 tDSP, with an expected receive of 1 packet.
  std::vector<kpi_flit_max> flit_buffer(1);

  // Send data[] elements one-by-one to the Kernel for processing
  // and print result
  for (i = 0; i < (sizeof(data)/sizeof(data[0])); i++)
  {
    hc.Send(&data[i], 1, 0);
    hc.ReceiveCount(flit_buffer);
    printf("%i -- Sent = %i -- Total = %i\n", i, data[i], (flit_buffer[0].payload.i[0]));
  }

  printf("Done!\n");

  return 0;
}

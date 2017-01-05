/******************************************************************************
  Copyright 2016 KNUPATH
  All rights reserved.
  KnuPath Proprietary and Confidential

  File: main.cpp

  Description: Host code for KPI Echo. Host will send kernel:echo a message
  and kernel:echo will return it.

******************************************************************************/
#include <kpi/kpi_runtime.hpp>

int main(int argc, char* argv[])
{

// ****************************************************************************
// Code to Complex Mapping Initialization
// ****************************************************************************

  // Create a context for complex 0 (the default)
  kpi::Context ctx;

  // Load the "echo" kernel
  kpi::Kernel k = ctx.CreateKernel("echo");

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

  // Define 'Hello World' in hex and backwards due to endians.
  // x86 is little-endian.
  int helloWorld[] = {0x6c6c6548, 0x6f57206f, 0x00646c72}; // {lleH, oW o, \0dlr}

  // Send tDSP 0 'int helloWorld[]'
  hc.Send(&helloWorld[0], 3, 0);

  // Define flit_buffer to size 1. In this example, we only communicate with
  // 1 tDSP, with an expected receive of 1 packet.
  std::vector<kpi_flit_max> flit_buffer(1);

  // Blocks until a packet is received.
  hc.ReceiveCount(flit_buffer);

  // A non-blocking receive. It waits for 1 or more messages until buffer size
  // is reached.
  //hc.ReceiveUpTo(flit_buffer);

  // Displays received packet as a character array.
  printf("%s\n", flit_buffer[0].payload.c);

  return 0;
}

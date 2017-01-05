/******************************************************************************
  Copyright 2016 KNUPATH
  All rights reserved.
  KnuPath Proprietary and Confidential

  File: main.cpp

  Description: Starts up all ping and pong kernels.  Displays the results.

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

	// Load each kernel
	kpi::Kernel k_ping = ctx.CreateKernel("ping");
	kpi::Kernel k_pong = ctx.CreateKernel("pong");

	// Set number of 'pings' to max number of tDSPs per Cluster (8)
	int num_pings = kpi::Complex(0).ProcessorsPerCluster();

	// Create a processor group with "num_pings" tDSPs on a cluster
	kpi::ProcGroup pg_ping(0, 1, 1,            // cluster start, stride, count
						   0, 1, num_pings);           // proc    start, stride, count

	// Create a processor group with 1 tDSP on a seperate cluster
	kpi::ProcGroup pg_pong(1, 1, 1,            // cluster start, stride, count
						   0, 1, 1);           // proc    start, stride, count

	// Allocate a cluster block with 1 cluster for "ping" kernels and
	//   another for the single "pong" kernel. Locate the clusters at the
	//   start of the complex
	kpi::ClusterBlock cb = ctx.AllocClusterBlockAt(2,         // num clusters
												   0);        // offset

	// Initialize Kernel Arguments. Needed even if no Kernel Args exist
	kpi::KernelArgs           ka_ping = k_ping.CreateKernelArgs();
	kpi::KernelArgs           ka_pong = k_pong.CreateKernelArgs();

	// Create a command queue to submit launches
	kpi::CommandQueue         cq = ctx.CreateCommandQueue();

	// Create a launch configuration
	kpi::LaunchConfig         lc = ctx.CreateLaunchConfig(cb.Size());

	// Add the kernel to the launch configuration
	lc.Add(pg_ping, k_ping, ka_ping);
	lc.Add(pg_pong, k_pong, ka_pong);

	// Submit the launch configuration to the queue. Use the returned
	// host connection to communicate with the kernels during while
	// the launch is running.
	kpi::HostConn             hc = cq.SubmitWithHostConn(lc, cb);

	// ****************************************************************************
	// Host Code
	// ****************************************************************************

	// Define flit_buffer to number of "ping" kernels. In this example, we expected to
	//   receive of 1 packet per ping kernel.
	std::vector<kpi_flit_max> flit_buffer(num_pings);

	// Receive all packets (blocking)
	hc.ReceiveCount(flit_buffer);

	// For each "ping" kernel, print each tDSP along the trip
	for (int flit_id = 0; flit_id < flit_buffer.size(); flit_id++)
	{
		printf("Ping rank: %d, ", flit_buffer[flit_id].payload.i[0]);
		printf("Pong rank: %d, ", flit_buffer[flit_id].payload.i[1]);
		printf("Relay rank: %d\n", flit_buffer[flit_id].payload.i[2]);
	}

	return 0;
}

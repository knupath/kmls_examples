/******************************************************************************
  Copyright 2016
  All rights reserved.
  KNUPATH Proprietary and Confidential

  File: kpi_getrank.cpp

  Description: Example program showing the use of the KPI C++ wrapper classes
               and the Kernel rank functions
******************************************************************************/

#include "kpi/kpi_runtime.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    int return_value = 0;
    try
    {
        // Create a context for complex 0 (the default)
        kpi::Context ctx;

        // Load the "getrank" kernel
        kpi::Kernel k = ctx.CreateKernel("getrank");

        // Create a processor group with 2 clusters
        kpi::ProcGroup pg(0, 1, 2,                                       // cluster start, stride, count
                          0, 1, kpi::Complex(0).ProcessorsPerCluster()); // proc    start, stride, count

        // Allocate a cluster block with 2 clusters, and locate it at the
        // center of the complex.
        kpi::ClusterBlock cb = ctx.AllocClusterBlockAt(2,                                  // num clusters
                                                       kpi::Complex(0).NumClusters() / 2); // offset

        // Set the value of the first argument
        int                       arg_value = 79;
        kpi::KernelArgs           ka = k.CreateKernelArgs();
        ka.SetValue(0, arg_value);

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

#if 1
        // Wait to receive one flit for each processor by setting
        // the vector size and calling ReceiveCount().
        std::vector<kpi_flit_max> flit_buffer(pg.TotalProcessors());
        hc.ReceiveCount(flit_buffer);

        // Display the flit contents
        for(int i = 0; i < flit_buffer.size(); ++i)
        {
            printf("Flit %4i: Rank = %4i, Physical ID = %4u\n",
                   i,
                   flit_buffer[i].payload.i[0],
                   static_cast<unsigned int>(flit_buffer[i].header & 0xFFFFFFFF));
        }
#else
        // If we aren't interested in the flit header, we can receive
        // the payload integers directly.
        // Wait to receive one flit for each processor by setting
        // the vector size and calling ReceiveCount().
        std::vector<int> rank_buffer(pg.TotalProcessors());
        hc.ReceiveCount(rank_buffer);

        // Display the flit contents
        for(auto r : rank_buffer)
        {
            printf("%i\n", r);
        }
#endif
    }
    catch(std::exception& e)
    {
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
    }
    return return_value;
}

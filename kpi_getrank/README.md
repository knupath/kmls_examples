# Example Program: kpi_getrank

The kpi_getrank example uses the KPI Host Runtime C++ wrapper library to create
KPI objects, load a kernel, and run it. The KPI cluster block, which dictates
what part of the KPI complex is used to run kernels, is chosen at the center
of the complex, to illustrate how the KPI rank refers to the base offset of the
cluster block. The size of the cluster block in this example is 2 clusters
(which together contain 16 total tDSPs).

The kernel code from getrank.knucpp is shown below.

~~~~~~~~~~~~{.cpp}
#include "kpi/kpi_kernel.h"

extern "C" __knu_kernel void getrank(int test_arg)
{
    int rank = KPI_Rank();

    // Send a single word to the host, containing the tDSP "rank" within
    // the launch.
    // 
    // When sending to the host, we can put whatever we would like in the
    // flit address bits. Here, we use the physical ID.
    KPI_Send_Host(&rank, 1, KPI_Physical_ID());
}
~~~~~~~~~~~~

Each tDSP determines its rank by calling the KPI_Rank() function, and sends
that value to the host (along with is physical ID, obtained using the
KPI_Physical_ID() function).

We can run kpi_getrank as follows:

~~~~~~~~~~~~{.bash}
export KNU_DEVICE=sdb045
/opt/knupath/examples/kpi/bin/kpi_getrank
~~~~~~~~~~~~

The output is shown below.

~~~~~~~~~~~~{.bash}
Flit    0: Rank =    0, Physical ID =  256
Flit    1: Rank =    1, Physical ID =  257
Flit    2: Rank =    2, Physical ID =  258
Flit    3: Rank =    3, Physical ID =  259
Flit    4: Rank =    4, Physical ID =  260
Flit    5: Rank =    5, Physical ID =  261
Flit    6: Rank =    6, Physical ID =  262
Flit    7: Rank =    7, Physical ID =  263
Flit    8: Rank =    8, Physical ID =  264
Flit    9: Rank =    9, Physical ID =  265
Flit   10: Rank =   10, Physical ID =  266
Flit   11: Rank =   11, Physical ID =  267
Flit   12: Rank =   12, Physical ID =  268
Flit   13: Rank =   13, Physical ID =  269
Flit   14: Rank =   14, Physical ID =  270
Flit   15: Rank =   15, Physical ID =  271
~~~~~~~~~~~~

As can be seen from the output above, the rank (reported by the KPI_Rank()
function) begins at 0, whereas the physical ID (obtained from the tDSP IDR
register) shows that the kernel was launched at offset 256 (in a complex
of 512 tDSPs). (Note that the flits may arrive at the host out of order,
but the relationship between each rank and physical ID should be the same
as shown above.)

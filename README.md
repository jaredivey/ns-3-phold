# ns-3-phold

<h1>Applying patch to ns-3.21</h1>
As necessary, additional instructions for installing ns-3 may be found here:
        http://www.nsnam.org/wiki/Installation
(1) Download ns-3.21 (If you do not already have it.)
(2) Copy patch to ns-3.21 directory and apply patch with the following command:
        patch -p 1 -i phold.patch
(3) Configure ns-3 to enable MPI:
        ./waf configure -d optimized --enable-mpi
(4) Build ns-3:
        ./waf
(5) Update PATH and LD_LIBRARY_PATH:
        export PATH=$(PATH):$(HOME)/repos/ns-3-allinone/ns-3.21/build/
        export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(HOME)/repos/ns-3-allinone/ns-3.21/build/
(6) Build and run phold application in a separate directory:
        make
        TRIALRUN="phold --simtype=0 --maxPackets=1024 --lookAhead=0.020 --remote=0.5 --groupNum=2"
        mpirun -np 8 $TRIALRUN

NOTE: The patch also contains a distributed termination detection scheme introduced by Rana:
S.P. Rana, A distributed solution of the distributed termination problem, Information Processing Letters, Volume 17, Issue 1, 19 July 1983, Pages 43-46, ISSN 0020-0190, http://dx.doi.org/10.1016/0020-0190(83)90089-3.
(http://www.sciencedirect.com/science/article/pii/0020019083900893)
Keywords: Distributed programming; termination detection; decentralized control

This code only applies to the Null Message implementation but will not be utilized when a stop time is set, which was the case for the performance model experiments.

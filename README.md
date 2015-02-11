# ns-3-phold

<h2>Code Description</h2>
<ul>
    <li>phold.patch: Patch file with modifications to the distributed simulator implementations in ns-3 to operate without the simulated network overhead.</li>
    <li>phold.cc: Application and main method for implementing the PHOLD performance model.</li>
    <li>InputImage.cc, InputImage.h: Simple file reading class to obtain seed values from randText.txt for the ns-3 random number generator.</li>
    <li>randText.txt: A list of 1024 random values. If not provided, a new version of this file will be created with a different set of 1024 random values.</li>
</ul>
<h2>Applying patch to ns-3.21</h2>
As necessary, additional instructions for installing ns-3 may be found here:<br>
        <a href=http://www.nsnam.org/wiki/Installation>http://www.nsnam.org/wiki/Installation</a><br>
<ol>
    <li>Download ns-3.21 (If you do not already have it.)</li>
    <li>Copy patch to ns-3.21 directory and apply patch with the following command:<br>
        patch -p 1 -i phold.patch</li>
    <li>Configure ns-3 to enable MPI:<br>
        ./waf configure -d optimized --enable-mpi</li>
    <li>Build ns-3:<br>
        ./waf</li>
    <li>Update PATH and LD_LIBRARY_PATH:<br>
        export PATH=$(PATH):$(HOME)/repos/ns-3-allinone/ns-3.21/build/<br>
        export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(HOME)/repos/ns-3-allinone/ns-3.21/build/</li>
    <li>Build and run phold application in a separate directory:<br>
        make<br>
        TRIALRUN="phold --simtype=0 --maxPackets=1024 --lookAhead=0.020 --remote=0.5 --groupNum=2"<br>
        mpirun -np 8 $TRIALRUN</li>
</ol>
<br>
NOTE: The patch also contains a distributed termination detection scheme introduced by Rana:<br>
S.P. Rana, A distributed solution of the distributed termination problem, Information Processing Letters, Volume 17, Issue 1, 19 July 1983, Pages 43-46, ISSN 0020-0190, <a href=http://dx.doi.org/10.1016/0020-0190(83)90089-3>http://dx.doi.org/10.1016/0020-0190(83)90089-3</a>.<br>
(<a href=http://www.sciencedirect.com/science/article/pii/0020019083900893>http://www.sciencedirect.com/science/article/pii/0020019083900893</a>)<br>
Keywords: Distributed programming; termination detection; decentralized control<br>
This code only applies to the Null Message implementation but will not be utilized when a stop time is set, which was the case for the performance model experiments.<br>

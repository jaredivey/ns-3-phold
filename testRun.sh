#!/bin/bash
THISRUN="send-receive --simtype=0 --maxPackets=131072 --lookAhead=0.001 --remote=0.5 --groupNum=1"
mpirun -np 8 $THISRUN
THISRUN="send-receive --simtype=0 --maxPackets=131072 --lookAhead=0.002 --remote=0.5 --groupNum=1"
mpirun -np 8 $THISRUN
THISRUN="send-receive --simtype=0 --maxPackets=131072 --lookAhead=0.003 --remote=0.5 --groupNum=1"
mpirun -np 8 $THISRUN
THISRUN="send-receive --simtype=0 --maxPackets=131072 --lookAhead=0.004 --remote=0.5 --groupNum=1"
mpirun -np 8 $THISRUN
THISRUN="send-receive --simtype=0 --maxPackets=131072 --lookAhead=0.005 --remote=0.5 --groupNum=1"
mpirun -np 8 $THISRUN
THISRUN="send-receive --simtype=0 --maxPackets=131072 --lookAhead=0.006 --remote=0.5 --groupNum=1"
mpirun -np 8 $THISRUN
THISRUN="send-receive --simtype=0 --maxPackets=131072 --lookAhead=0.007 --remote=0.5 --groupNum=1"
mpirun -np 8 $THISRUN
THISRUN="send-receive --simtype=0 --maxPackets=131072 --lookAhead=0.008 --remote=0.5 --groupNum=1"
mpirun -np 8 $THISRUN
THISRUN="send-receive --simtype=0 --maxPackets=131072 --lookAhead=0.009 --remote=0.5 --groupNum=1"
mpirun -np 8 $THISRUN
THISRUN="send-receive --simtype=0 --maxPackets=131072 --lookAhead=0.010 --remote=0.5 --groupNum=1"
mpirun -np 8 $THISRUN
THISRUN="send-receive --simtype=0 --maxPackets=131072 --lookAhead=0.015 --remote=0.5 --groupNum=1"
mpirun -np 8 $THISRUN
THISRUN="send-receive --simtype=0 --maxPackets=131072 --lookAhead=0.020 --remote=0.5 --groupNum=1"
mpirun -np 8 $THISRUN
THISRUN="send-receive --simtype=0 --maxPackets=131072 --lookAhead=0.025 --remote=0.5 --groupNum=1"
mpirun -np 8 $THISRUN

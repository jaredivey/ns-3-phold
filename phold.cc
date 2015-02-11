/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#ifndef NS3_MPI
#define NS3_MPI
#define NS3_OPENMPI
#endif

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mpi-module.h"

#include "InputImage.h"

#ifdef NS3_MPI
#include <mpi.h>
#endif

using namespace std;
using namespace ns3;

#define NS3_INFINITY 0x7fffffffffffffffLL 
//576460752303423500

double clLookAhead;
double expMean;
double remote = 0.0;
uint32_t initialRun = 100;
uint32_t runSeed = 0;
uint32_t groupNum = 1;

Ptr<MpiReceiver> rcvr;

unsigned long 
ReportMemoryUsage()
{
  pid_t pid;
  char work[4096];
  FILE* f;
  char* pCh;

  pid = getpid();
  sprintf(work, "/proc/%d/stat", (int)pid);
  f = fopen(work, "r");
  if (f == NULL)
    {
      std::cout <<"Can't open " << work << std::endl;
      return(0);
    }
  if(fgets(work, sizeof(work), f) == NULL)
    std::cout << "Error with fgets" << std::endl;
  fclose(f);
  strtok(work, " ");
  for (int i = 1; i < 23; i++)
    {
      pCh = strtok(NULL, " ");
    }
  return(atol(pCh));
}

long long
ReportMemoryUsageMB()
{
  long long u = ReportMemoryUsage();
  return ((u + 500000) / 1000000 );
}

class Phold : public Application 
{
public:

  Phold ();
  virtual ~Phold();

  void Setup (uint32_t packetSize, uint32_t sysId, uint32_t sysCount, double delay, uint32_t maxPackets);
  void SendPacket (Ptr<Packet> packet, uint32_t);

private:
  virtual void StartApplication (void);
  virtual void StopApplication (void);
  void Progress (void);

  EventId         m_sendEvent;
  bool            m_running;
  uint32_t        m_packetSize;
  uint32_t        m_systemId;
  uint32_t        m_systemCount;
  uint32_t        m_numRun;
  Time            m_delay;

  uint32_t        m_maxPackets;
};

Phold::Phold ()
  : m_sendEvent (), 
    m_running (false),
    m_packetSize (0),
    m_systemId (0),
    m_systemCount (1),
    m_maxPackets (0)
{
}

Phold::~Phold()
{
}

void
Phold::Setup (uint32_t packetSize, uint32_t sysId, uint32_t sysCount, double delay, uint32_t maxPackets)
{
  m_packetSize = packetSize;
  m_systemId = sysId;
  m_systemCount = sysCount;
  m_delay = Seconds(delay);
  m_maxPackets = maxPackets;
}

void
Phold::StartApplication (void)
{
  m_running = true;
  m_numRun = 0;
  SeedManager::SetRun(runSeed);

  Ptr<Packet> packet = Create<Packet> (m_packetSize);
  for (uint32_t i = 0; i < initialRun; ++i)
    {
      SendPacket (packet, i+1);
    }
  if (m_systemId == 0)
    {
//      Simulator::Schedule (m_delay, &Phold::Progress, this);
    }
}

void 
Phold::StopApplication (void)
{
  m_running = false;

  if (m_sendEvent.IsRunning ())
    {
      Simulator::Cancel (m_sendEvent);
    }
}

void
Phold::Progress (void)
{
  std::cout << "Node: " << m_systemId << " Simulator Time: " << Simulator::Now().GetSeconds() << std::endl;
  Simulator::Schedule (m_delay, &Phold::Progress, this);
}

void 
Phold::SendPacket (Ptr<Packet> packet, uint32_t firstRun)
{
  if ((m_running) && 
      ((m_maxPackets > MpiInterface::GetTxCount()) || 
       !m_maxPackets))
    {
      Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable> ();
      uv->SetAttribute ("Min", DoubleValue (m_systemId));
      uv->SetAttribute ("Max", DoubleValue (m_systemId + groupNum));
      Ptr<UniformRandomVariable> rm = CreateObject<UniformRandomVariable> ();
      rm->SetAttribute ("Min", DoubleValue (0.0));
      rm->SetAttribute ("Max", DoubleValue (1.0));
      Ptr<ExponentialRandomVariable> ev = CreateObject<ExponentialRandomVariable> ();
      ev->SetAttribute ("Mean", DoubleValue (expMean));
      ev->SetAttribute ("Bound", DoubleValue (0.0));

      uint32_t uv_int = uv->GetInteger(m_systemId, m_systemId + groupNum) % m_systemCount;
      double ev_val = ev->GetValue();
      double rm_val = rm->GetValue( 0.0, 1.0 );
      if ((firstRun))// || (ev_val <= clLookAhead))
        {
          uv_int = m_systemId;
        }
      else
        {
          if ((rm_val < remote) && (uv_int != m_systemId))
            {
              ev_val += clLookAhead; // + for lookahead
            }
          else
            {
              uv_int = m_systemId;
            }
        }

      Time tNext(Seconds (ev_val));
      tNext += Simulator::Now();
      //cout << Simulator::Now().GetSeconds() << " " << m_systemId << " sending to " << uv_int << " at\t" << tNext.GetSeconds() << 
      //      " " << MpiInterface::GetTxCount() << " " << MpiInterface::GetRxCount() << endl;

      char testText[m_packetSize];
      sprintf (testText, "LP%05u-%ld", m_systemId, tNext.GetInteger());
      Ptr<Packet> p = Create<Packet> ((uint8_t*)testText, m_packetSize);

      if (uv_int == m_systemId)
        {
          MpiInterface::SendLocal(p, tNext);
          //cout << "Node: " << m_systemId << " Tx: " << MpiInterface::GetTxCount() << " Rx: " << MpiInterface::GetRxCount() << endl;
        }
      else 
        {
          tNext += Seconds (clLookAhead);
          MpiInterface::SendPacket(p, tNext, uv_int, 0);
        }
    }
}

NS_LOG_COMPONENT_DEFINE ("Phold");

int
main (int argc, char *argv[])
{
#ifdef NS3_MPI
  // Command line attributes
  CommandLine cmd;

  uint32_t fileId = 0;
  uint32_t packetSize = 32;
  uint32_t seedValue = 1;
  double stopTime = 0.0;
  expMean = 0.9;
  bool longReport = 0;
  bool header = 0;
  uint32_t maxPackets = 0;
  int simtype = 0;

  clLookAhead = 0.1;

  cmd.AddValue ("fileId", "Simple identifier for filename; default=0", fileId);
  cmd.AddValue ("packetSize", "Size of packets to send; default=32", packetSize);
  cmd.AddValue ("runSeed", "Random number generator run; default=0", runSeed);
  cmd.AddValue ("stopTime", "Stop time in seconds; default=60", stopTime);
  cmd.AddValue ("lookAhead", "Lookahead in seconds; default=0.1", clLookAhead);
  cmd.AddValue ("expMean", "Mean send time (exponential) in seconds; default=0.9", expMean);
  cmd.AddValue ("longReport", "Provide Tx,Rx for each LP; default=0", longReport);
  cmd.AddValue ("header", "Logs header; default=0", header);
  cmd.AddValue ("initialRun", "Number of packets to send upon start; default=100", initialRun);
  cmd.AddValue ("remote", "Percentage of remote sends; default=0.0", remote);
  cmd.AddValue ("maxPackets", "Maximum number of packets to send; default=0 (no maximum)", maxPackets);
  cmd.AddValue ("groupNum", "Number of LPs to which an LP may transmit messages", groupNum);
  cmd.AddValue ("simtype", "Select which distributed simulator to use", simtype);

  cmd.Parse(argc, argv);

  if (remote > 1.0) remote = 1.0;

  // Distributed simulation setup
  if (simtype == 0)
    {
      GlobalValue::Bind ("SimulatorImplementationType",
                         StringValue ("ns3::DistributedSimulatorImpl"));
    }
  else if (simtype == 1)
    {
      GlobalValue::Bind ("SimulatorImplementationType",
                         StringValue ("ns3::NullMessageSimulatorImpl"));

      Config::SetDefault ("ns3::NullMessageSimulatorImpl::GroupNumber",
                          UintegerValue (groupNum));
    }
  MpiInterface::Enable (&argc, &argv);

  uint32_t systemId = MpiInterface::GetSystemId ();
  uint32_t systemCount = MpiInterface::GetSize ();

  uint32_t arraySize = 1024;
  if (systemId == 0)
    {
      ifstream ifs("randText.txt");
      if (!ifs)
        {
          srand (time(NULL));
          int *randValues = (int*)malloc (arraySize*sizeof(int));
          for (uint32_t i=0; i<arraySize; ++i)
          {
            randValues[i] = rand() % arraySize + 1;
          }
          InputImage::SaveImageData("randText.txt", randValues, arraySize);
          free (randValues);
        }
    }
  MPI_Barrier(MPI_COMM_WORLD);
  InputImage randFile = InputImage("randText.txt");
  int* seedValues = (int*)malloc(arraySize*sizeof(int));
  seedValues = randFile.GetImageData();

  // Set the seed for the RNG
  SeedManager::SetSeed(seedValues[systemId]);
  free (seedValues);

  NodeContainer procNodes;
  for (uint32_t i = 0; i < systemCount; i++)
    {
      Ptr<Node> node = CreateObject<Node> (i);
      procNodes.Add (node);
    }

  MpiInterface::SetDelay (Seconds(clLookAhead));

  Ptr<Phold> app = CreateObject<Phold> ();
  app->Setup (packetSize, systemId, systemCount, 5.0, maxPackets);
  app->SetStartTime(Seconds(0.));

  if (stopTime)
    app->SetStopTime(Seconds(stopTime));

  rcvr = CreateObject<MpiReceiver> ();
  Callback <void, Ptr<Packet>, uint32_t> cb;
  cb = MakeCallback (&Phold::SendPacket, app);
  rcvr->SetReceiveCallback1 (cb);

  procNodes.Get(systemId)->AggregateObject(rcvr);
  procNodes.Get(systemId)->AddApplication(app);

  if (stopTime)
    Simulator::Stop (Seconds (stopTime));

  struct timeval startTime, doneTime;

  MPI_Barrier(MPI_COMM_WORLD);
  gettimeofday(&startTime, NULL);
  unsigned long startUSec = startTime.tv_sec * 1000000 + startTime.tv_usec;
  Simulator::Run ();
  MPI_Barrier(MPI_COMM_WORLD);
  gettimeofday(&doneTime, NULL);
  unsigned long endUSec = doneTime.tv_sec * 1000000 + doneTime.tv_usec;

  long long memUsed = ReportMemoryUsageMB();

  uint32_t myTx = MpiInterface::GetTxCount();
  uint32_t myRx = MpiInterface::GetRxCount();
  double simTime = Simulator::Now().GetSeconds();
  unsigned long diffUSec = endUSec - startUSec;

  uint32_t gatheredTx;
  uint32_t gatheredRx;
  double totalSimTime;
  unsigned long maxDiffUSec;
  long long totalMem;

  MPI_Allreduce (&myTx, &gatheredTx, 1, MPI_UNSIGNED, MPI_SUM, MPI_COMM_WORLD);
  MPI_Allreduce (&myRx, &gatheredRx, 1, MPI_UNSIGNED, MPI_SUM, MPI_COMM_WORLD);
  MPI_Allreduce (&simTime, &totalSimTime, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  MPI_Allreduce (&diffUSec, &maxDiffUSec, 1, MPI_UNSIGNED_LONG, MPI_MAX, MPI_COMM_WORLD);
  MPI_Allreduce (&memUsed, &totalMem, 1, MPI_LONG_LONG, MPI_SUM, MPI_COMM_WORLD);

  if (systemId == 0)
    {
      cout << simtype << "\t" << systemCount << "\t" << MpiInterface::GetDelay().GetSeconds() << "\t" << groupNum << "\t" << remote 
          << "\t" << expMean << "\t" << initialRun << "\t" << packetSize << "\t" << gatheredTx << "\t" << gatheredRx << "\t" 
          << totalSimTime << "\t" << (double)(maxDiffUSec)/1000000.0 << "\t" << totalMem << "\t" << endl;
    }
  Simulator::Destroy ();
  MPI_Barrier(MPI_COMM_WORLD);
  // Exit the MPI execution environment
  MpiInterface::Disable ();
  return 0;
#else
  NS_FATAL_ERROR ("Can't use distributed simulator without MPI compiled in");
#endif
}

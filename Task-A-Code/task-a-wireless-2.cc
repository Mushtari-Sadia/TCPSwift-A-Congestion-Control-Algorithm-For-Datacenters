/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 ResiliNets, ITTC, University of Kansas
 *
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
 * Authors: Justin P. Rohrer, Truc Anh N. Nguyen <annguyen@ittc.ku.edu>, Siddharth Gangadhar <siddharth@ittc.ku.edu>
 *
 * James P.G. Sterbenz <jpgs@ittc.ku.edu>, director
 * ResiliNets Research Group  http://wiki.ittc.ku.edu/resilinets
 * Information and Telecommunication Technology Center (ITTC)
 * and Department of Electrical Engineering and Computer Science
 * The University of Kansas Lawrence, KS USA.
 *
 * Work supported in part by NSF FIND (Future Internet Design) Program
 * under grant CNS-0626918 (Postmodern Internet Architecture),
 * NSF grant CNS-1050226 (Multilayer Network Resilience Analysis and Experimentation on GENI),
 * US Department of Defense (DoD), and ITTC at The University of Kansas.
 *
 * “TCP Westwood(+) Protocol Implementation in ns-3”
 * Siddharth Gangadhar, Trúc Anh Ngọc Nguyễn , Greeshma Umapathi, and James P.G. Sterbenz,
 * ICST SIMUTools Workshop on ns-3 (WNS3), Cannes, France, March 2013
 */

#include <fstream>
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/mobility-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/spectrum-module.h"
#include "ns3/propagation-module.h"
#include "ns3/sixlowpan-module.h"
#include "ns3/lr-wpan-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv6-flow-classifier.h"
#include "ns3/flow-monitor-helper.h"
#include <ns3/lr-wpan-error-model.h>
using namespace ns3;

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TcpVariantsComparison");

static bool firstCwnd = true;
static bool firstSshThr = true;
static bool firstRtt = true;
static bool firstRto = true;
static Ptr<OutputStreamWrapper> cWndStream;
static Ptr<OutputStreamWrapper> ssThreshStream;
static Ptr<OutputStreamWrapper> rttStream;
static Ptr<OutputStreamWrapper> rtoStream;
static uint32_t cWndValue;
static uint32_t ssThreshValue;


static void
CwndTracer (uint32_t oldval, uint32_t newval)
{
  if (firstCwnd)
    {
      *cWndStream->GetStream () << "0.0 " << oldval << std::endl;
      firstCwnd = false;
    }
  *cWndStream->GetStream () << Simulator::Now ().GetSeconds () << " " << newval << std::endl;
  cWndValue = newval;

  if (!firstSshThr)
    {
      *ssThreshStream->GetStream () << Simulator::Now ().GetSeconds () << " " << ssThreshValue << std::endl;
    }
}

static void
SsThreshTracer (uint32_t oldval, uint32_t newval)
{
  if (firstSshThr)
    {
      *ssThreshStream->GetStream () << "0.0 " << oldval << std::endl;
      firstSshThr = false;
    }
  *ssThreshStream->GetStream () << Simulator::Now ().GetSeconds () << " " << newval << std::endl;
  ssThreshValue = newval;

  if (!firstCwnd)
    {
      *cWndStream->GetStream () << Simulator::Now ().GetSeconds () << " " << cWndValue << std::endl;
    }
}

static void
RttTracer (Time oldval, Time newval)
{
  if (firstRtt)
    {
      *rttStream->GetStream () << "0.0 " << oldval.GetSeconds () << std::endl;
      firstRtt = false;
    }
  *rttStream->GetStream () << Simulator::Now ().GetSeconds () << " " << newval.GetSeconds () << std::endl;
}

static void
RtoTracer (Time oldval, Time newval)
{
  if (firstRto)
    {
      *rtoStream->GetStream () << "0.0 " << oldval.GetSeconds () << std::endl;
      firstRto = false;
    }
  *rtoStream->GetStream () << Simulator::Now ().GetSeconds () << " " << newval.GetSeconds () << std::endl;
}


static void
TraceCwnd (std::string cwnd_tr_file_name)
{
  AsciiTraceHelper ascii;
  cWndStream = ascii.CreateFileStream (cwnd_tr_file_name.c_str ());
  Config::ConnectWithoutContext ("/NodeList/0/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow", MakeCallback (&CwndTracer));
}

static void
TraceSsThresh (std::string ssthresh_tr_file_name)
{
  AsciiTraceHelper ascii;
  ssThreshStream = ascii.CreateFileStream (ssthresh_tr_file_name.c_str ());
  Config::ConnectWithoutContext ("/NodeList/0/$ns3::TcpL4Protocol/SocketList/0/SlowStartThreshold", MakeCallback (&SsThreshTracer));
}

static void
TraceRtt (std::string rtt_tr_file_name)
{
  AsciiTraceHelper ascii;
  rttStream = ascii.CreateFileStream (rtt_tr_file_name.c_str ());
  Config::ConnectWithoutContext ("/NodeList/0/$ns3::TcpL4Protocol/SocketList/0/RTT", MakeCallback (&RttTracer));
}

static void
TraceRto (std::string rto_tr_file_name)
{
  AsciiTraceHelper ascii;
  rtoStream = ascii.CreateFileStream (rto_tr_file_name.c_str ());
  Config::ConnectWithoutContext ("/NodeList/0/$ns3::TcpL4Protocol/SocketList/0/RTO", MakeCallback (&RtoTracer));
}




int main (int argc, char *argv[])
{
  std::cout << "1" << std::endl;
  std::string transport_prot = "TcpVegas";
  double error_p = 0.00001;
  //params
  uint16_t num_flows = 80; //10,20,30,40,60,80
  uint16_t num_nodes = 100; //20, 40, 60, 80, and 100
  int packets_per_second = 100; //100, 200, 300, 400, and 500
  int packetsize = 536;
  int coverage_area=1;
  
  // int dr = (number_of_packets*packetsize)/1000000;
  // std::cout << "datarate " << std::to_string(dr) << std::endl;
  // std::cout << "datarate*5 " << std::to_string(5*dr) << std::endl;
  // std::string datarate = std::to_string(dr) + "Mbps";
  // std::string bandwidth = datarate;
  // std::string access_bandwidth = std::to_string(5*dr);
  std::string bandwidth = "2Mbps";
  std::string access_bandwidth = "10Mbps";
  std::string delay = "0.01ms";
  std::string access_delay = "45ms";
  uint64_t data_mbytes = 0;
  uint32_t mtu_bytes = 180;
  uint32_t tcp_adu_size;

  
  double duration = 100.0;
  uint32_t run = 0;
  bool sack = true;
  std::string queue_disc_type = "ns3::PfifoFastQueueDisc";
  std::string recovery = "ns3::TcpClassicRecovery";
  
  // Ptr<LrWpanErrorModel>  lrWpanError;
  // int number_of_packets = 500000;
  // int packetsize = 1000;
  // int dr = (number_of_packets*packetsize)/1000000;
  // std::string datarate = std::to_string(dr) + "Mbps";

  std::ofstream flowmonitorOutput;
  std::ofstream summaryOutput;
  std::ofstream datarate_summaryOutput;
  std::ofstream cxArea_summaryOutput;

// Naming the output directory using local system time
  time_t rawtime;
  struct tm * timeinfo;
  char buffer [80];
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer, sizeof (buffer), "%d-%m-%Y-%I-%M-%S", timeinfo);
  std::string currentTime (buffer);
  CommandLine cmd (__FILE__);
  cmd.AddValue ("transport_prot", "Transport protocol to use: TcpNewReno, TcpLinuxReno, "
                "TcpHybla, TcpHighSpeed, TcpHtcp, TcpVegas, TcpScalable, TcpVeno, "
                "TcpBic, TcpYeah, TcpIllinois, TcpWestwood, TcpWestwoodPlus, TcpLedbat, "
		"TcpLp, TcpDctcp, TcpCubic, TcpBbr", transport_prot);
  cmd.AddValue ("error_p", "Packet error rate", error_p);
  cmd.AddValue ("bandwidth", "Bottleneck bandwidth", bandwidth);
  cmd.AddValue ("delay", "Bottleneck delay", delay);
  cmd.AddValue ("access_bandwidth", "Access link bandwidth", access_bandwidth);
  cmd.AddValue ("access_delay", "Access link delay", access_delay);
  cmd.AddValue ("data", "Number of Megabytes of data to transmit", data_mbytes);
  cmd.AddValue ("mtu", "Size of IP packets to send in bytes", mtu_bytes);
  cmd.AddValue ("num_flows", "Number of flows", num_flows);
  cmd.AddValue ("num_nodes", "Number of flows", num_nodes);
  cmd.AddValue ("packets_per_second", "Number of flows", packets_per_second);
  cmd.AddValue ("coverage_area", "Number of flows", coverage_area);
  cmd.AddValue ("duration", "Time to allow flows to run in seconds", duration);
  cmd.AddValue ("run", "Run index (for setting repeatable seeds)", run);
  cmd.AddValue ("queue_disc_type", "Queue disc type for gateway (e.g. ns3::CoDelQueueDisc)", queue_disc_type);
  cmd.AddValue ("sack", "Enable or disable SACK option", sack);
  cmd.AddValue ("recovery", "Recovery algorithm type to use (e.g., ns3::TcpPrrRecovery", recovery);
  cmd.Parse (argc, argv);

  transport_prot = std::string ("ns3::") + transport_prot;

  SeedManager::SetSeed (1);
  SeedManager::SetRun (run);


  // Calculate the ADU size
  Header* temp_header = new Ipv6Header ();
  uint32_t ip_header = temp_header->GetSerializedSize ();
  delete temp_header;
  temp_header = new TcpHeader ();
  uint32_t tcp_header = temp_header->GetSerializedSize ();
  delete temp_header;
  tcp_adu_size = mtu_bytes - 20 - (ip_header + tcp_header);

  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (tcp_adu_size));

  // Set the simulation start and stop time
  double start_time = 0.1;
  double stop_time = start_time + duration;

  // 2 MB of TCP buffer
  Config::SetDefault ("ns3::TcpSocket::RcvBufSize", UintegerValue (1 << 21));
  Config::SetDefault ("ns3::TcpSocket::SndBufSize", UintegerValue (1 << 21));
  Config::SetDefault ("ns3::TcpSocketBase::Sack", BooleanValue (sack));

  Config::SetDefault ("ns3::TcpL4Protocol::RecoveryType",
                      TypeIdValue (TypeId::LookupByName (recovery)));
  // Select TCP variant
  if (transport_prot.compare ("ns3::TcpWestwoodPlus") == 0)
    { 
      // TcpWestwoodPlus is not an actual TypeId name; we need TcpWestwood here
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpWestwood::GetTypeId ()));
      // the default protocol type in ns3::TcpWestwood is WESTWOOD
      Config::SetDefault ("ns3::TcpWestwood::ProtocolType", EnumValue (TcpWestwood::WESTWOODPLUS));
    }
  else
    {
      TypeId tcpTid;
      NS_ABORT_MSG_UNLESS (TypeId::LookupByNameFailSafe (transport_prot, &tcpTid), "TypeId " << transport_prot << " not found");
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TypeId::LookupByName (transport_prot)));
    }

//  lrWpanError = CreateObject<LrWpanErrorModel> ();

  // Create gateways, sources, and sinks
  
  NodeContainer sources;
  sources.Create (num_nodes);
  NodeContainer sinks;
  sinks.Create (1);
  NodeContainer All;
  All.Add (sinks);
  All.Add (sources);
  

  // Configure the error model
  // Here we use RateErrorModel with packet error rate
  MobilityHelper mobility;
  
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (20*coverage_area),
                                 "DeltaY", DoubleValue (20*coverage_area),
                                 "GridWidth", UintegerValue (10),
                                 "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (All);
  // creating a channel with range propagation loss model  
  Config::SetDefault ("ns3::RangePropagationLossModel::MaxRange", DoubleValue (80));
  Ptr<SingleModelSpectrumChannel> channel = CreateObject<SingleModelSpectrumChannel> ();
  Ptr<RangePropagationLossModel> propModel = CreateObject<RangePropagationLossModel> ();
  Ptr<ConstantSpeedPropagationDelayModel> delayModel = CreateObject<ConstantSpeedPropagationDelayModel> ();
  channel->AddPropagationLossModel (propModel);
  channel->SetPropagationDelayModel (delayModel);
 
  LrWpanHelper lrWpanHelper;
  // setting the channel in helper
  lrWpanHelper.SetChannel(channel);
  NetDeviceContainer lrwpanDevices = lrWpanHelper.Install (All);

  lrWpanHelper.AssociateToPan (lrwpanDevices, 0);

  InternetStackHelper internetv6;
  internetv6.Install (All);

  SixLowPanHelper sixLowPanHelper;
  NetDeviceContainer sixLowPanDevices = sixLowPanHelper.Install (lrwpanDevices);

  // CsmaHelper csma;
  // // csma.SetDeviceAttribute ("DataRate", StringValue (access_bandwidth));
  // // csma.SetChannelAttribute ("Delay", StringValue (access_delay));
  // NetDeviceContainer devices;
  // NodeContainer wiredNodes;
  // wiredNodes.Add(gateways);
  // wiredNodes.Add(sinks);
  // devices = csma.Install (wiredNodes);
      
  // Ipv6AddressHelper ipv6;
  // std::cout << "1" << std::endl;
  // ipv6.SetBase (Ipv6Address ("2001:cafe::"), Ipv6Prefix (64));
  // std::cout << "2" << std::endl;
  // Ipv6InterfaceContainer wiredDeviceInterfaces;
  // wiredDeviceInterfaces = ipv6.Assign (devices.Get (0));
  // //(
  // std::cout << "3" << std::endl;
  // wiredDeviceInterfaces.SetForwarding (0, true);
  // std::cout << "4" << std::endl;
  // wiredDeviceInterfaces.SetDefaultRouteInAllNodes (0);
  // std::cout << "5" << std::endl;

  // ipv6.SetBase (Ipv6Address ("2001:0DB8:1::"), Ipv6Prefix (64));
  // Ipv6InterfaceContainer wiredDeviceInterfaces2;
  // wiredDeviceInterfaces2 = ipv6.Assign (devices.Get (1));
  // wiredDeviceInterfaces2.SetForwarding (0, true);
  // wiredDeviceInterfaces2.SetDefaultRouteInAllNodes (0);
  Ipv6AddressHelper ipv6;
  ipv6.SetBase (Ipv6Address ("2001:f00d::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer wsnDeviceInterfaces;
  wsnDeviceInterfaces = ipv6.Assign (sixLowPanDevices);
  wsnDeviceInterfaces.SetForwarding (0, true);
  wsnDeviceInterfaces.SetDefaultRouteInAllNodes (0);

  for (uint32_t i = 0; i < sixLowPanDevices.GetN (); i++) {
    Ptr<NetDevice> dev = sixLowPanDevices.Get (i);
    dev->SetAttribute ("UseMeshUnder", BooleanValue (true));
    dev->SetAttribute ("MeshUnderRadius", UintegerValue (10));
  }
  std::cout << "2" << std::endl;

  

  for (uint16_t i = 0; i < num_flows; i++)
    {
      // std::cout << "\t" << i << std::endl;
      uint16_t port = 9+i;
      PacketSinkHelper sinkApp ("ns3::TcpSocketFactory",
      Inet6SocketAddress (Ipv6Address::GetAny (), port));
      sinkApp.SetAttribute ("Protocol", TypeIdValue (TcpSocketFactory::GetTypeId ()));
      ApplicationContainer sinkApps = sinkApp.Install (sinks.Get(0));

      BulkSendHelper sourceApp ("ns3::TcpSocketFactory",
                              Inet6SocketAddress (wsnDeviceInterfaces.GetAddress (0, 1), 
                              port));
      sourceApp.SetAttribute ("SendSize", UintegerValue (packets_per_second*packetsize*8));
      // sourceApp.SetAttribute ("MaxBytes", UintegerValue (data_mbytes * 1000000));
      ApplicationContainer sourceApps = sourceApp.Install (sources.Get (i%num_nodes));
      sourceApps.Start (Seconds (start_time * i));
      sourceApps.Stop (Seconds (stop_time - 3));

      
      sinkApps.Start (Seconds (start_time * i));
      sinkApps.Stop (Seconds (stop_time));
    }
  // Create a new directory to store the output of the program
  std::string dir = "Calculations/task-a2/my-"+transport_prot +"-calculations/" + currentTime + "/";
  std::string dirToSave = "mkdir -p " + dir;
  if (system (dirToSave.c_str ()) == -1)
    {
      exit (1);
    }
  std::string dir_summary = "Calculations/task-a2/my-"+transport_prot +"-calculations/nodes="+std::to_string(num_nodes)+",";
  
  // Set up tracing if enabled
  
  // std::ofstream ascii;
  // Ptr<OutputStreamWrapper> ascii_wrap;
  std::string filePrefix = "taskA2";
  AsciiTraceHelper ascii;
  // lrWpanHelper.EnableAsciiAll (ascii.CreateFileStream ( filePrefix + ".tr"));
  // lrWpanHelper.EnablePcapAll (filePrefix, false);

  // csma.EnableAsciiAll (ascii.CreateFileStream (filePrefix + ".tr"));
  // csma.EnablePcapAll (filePrefix, false);
  Simulator::Schedule (Seconds (0.00001), &TraceCwnd, dir + "-cwnd.dat");
  Simulator::Schedule (Seconds (0.00001), &TraceSsThresh, dir + "-ssth.dat");
  Simulator::Schedule (Seconds (0.00001), &TraceRtt, dir + "-rtt.dat");
  Simulator::Schedule (Seconds (0.00001), &TraceRto, dir + "-rto.dat");
  FlowMonitorHelper flowmon;

  Ptr<FlowMonitor> monitor = flowmon.InstallAll ();
  Simulator::Stop (Seconds(stop_time));

  flowmonitorOutput.open (dir+"flowSummary.dat", std::ios::out);
  summaryOutput.open (dir_summary+"flow_summary.dat", std::ios_base::app);
  datarate_summaryOutput.open (dir_summary+"dr_summary.dat", std::ios_base::app);
  cxArea_summaryOutput.open (dir_summary+"cx_summary.dat", std::ios_base::app);
std::cout << "3" << std::endl;
  Simulator::Run ();
std::cout << "4" << std::endl;

  int j=0;
  float AvgThroughput = 0;
  Time Jitter;
  Time Delay;
  // variables for output measurement
  uint32_t SentPackets = 0;
  uint32_t ReceivedPackets = 0;
  uint32_t LostPackets = 0;

  Ptr<Ipv6FlowClassifier> classifier = DynamicCast<Ipv6FlowClassifier> (flowmon.GetClassifier6 ());
  FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats ();
// std::cout << "5" << std::endl;
// int x=0;
  for (auto iter = stats.begin (); iter != stats.end (); ++iter) {
    // std::cout << x << std::endl;
    // x++;
      Ipv6FlowClassifier::FiveTuple t = classifier->FindFlow (iter->first); 
            // classifier returns FiveTuple in correspondance to a flowID

      flowmonitorOutput << "----Flow ID:" <<iter->first << std::endl;
      flowmonitorOutput << "Src Addr" <<t.sourceAddress << " -- Dst Addr "<< t.destinationAddress<< std::endl;
      flowmonitorOutput << "Sent Packets=" <<iter->second.txPackets<< std::endl;
      flowmonitorOutput << "Received Packets =" <<iter->second.rxPackets<< std::endl;
      flowmonitorOutput << "Lost Packets =" <<iter->second.txPackets-iter->second.rxPackets<< std::endl;
      flowmonitorOutput << "Packet delivery ratio =" <<iter->second.rxPackets*100.0/iter->second.txPackets << "%"<< std::endl;
      flowmonitorOutput << "Packet loss ratio =" << (iter->second.txPackets-iter->second.rxPackets)*100.0/iter->second.txPackets << "%"<< std::endl;
      // flowmonitorOutput << "Packet lost diff way = "<< iter->second.lostPackets;
      flowmonitorOutput << "Delay =" <<iter->second.delaySum<< std::endl;
      flowmonitorOutput << "Jitter =" <<iter->second.jitterSum<< std::endl;
      flowmonitorOutput << "Throughput =" <<iter->second.rxBytes * 8.0/(iter->second.timeLastRxPacket.GetSeconds()-iter->second.timeFirstTxPacket.GetSeconds())/1024<<"Kbps"<< std::endl;

      SentPackets = SentPackets +(iter->second.txPackets);
      ReceivedPackets = ReceivedPackets + (iter->second.rxPackets);
      LostPackets = LostPackets + (iter->second.txPackets-iter->second.rxPackets);
      AvgThroughput = AvgThroughput + (iter->second.rxBytes * 8.0/(iter->second.timeLastRxPacket.GetSeconds()-iter->second.timeFirstTxPacket.GetSeconds())/1024);
      Delay = Delay + (iter->second.delaySum);
      Jitter = Jitter + (iter->second.jitterSum);

      j = j + 1;

  }
std::cout << "loop exits" << std::endl;
    AvgThroughput = AvgThroughput/j;
    std::cout << "1" << std::endl;
    flowmonitorOutput << "--------Total Results of the simulation----------"<<std::endl;
    flowmonitorOutput << "Total sent packets  =" << SentPackets<< std::endl;
    flowmonitorOutput << "Total Received Packets =" << ReceivedPackets<< std::endl;
    std::cout << "Total Received Packets =" << ReceivedPackets<< std::endl;
    // std::cout << "2" << std::endl;
    flowmonitorOutput << "Total Lost Packets =" << LostPackets<< std::endl;
    flowmonitorOutput << "Packet Loss ratio =" << ((LostPackets*100.00)/SentPackets)<< "%"<< std::endl;
    flowmonitorOutput << "Packet delivery ratio =" << ((ReceivedPackets*100.00)/SentPackets)<< "%"<< std::endl;
    // std::cout << "3" << std::endl;
    flowmonitorOutput << "Average Throughput =" << AvgThroughput<< "Kbps"<< std::endl;
    // std::cout << "-1" << std::endl;
    flowmonitorOutput << "End to End Delay =" << Delay<< std::endl;
    // std::cout << "-2" << std::endl;
    flowmonitorOutput << "End to End Jitter delay =" << Jitter<< std::endl;
    // std::cout << "4" << std::endl;
    flowmonitorOutput << "Total Flow id " << j<< std::endl;

    summaryOutput << std::to_string(num_flows) << "\t" << AvgThroughput<< "Kbps"<< "\t" << Delay << "\t" << ((ReceivedPackets*100.00)/SentPackets) << "\t" << ((LostPackets*100.00)/SentPackets) << std::endl;
    summaryOutput.close();
    // std::cout << "5" << std::endl;
    datarate_summaryOutput << packets_per_second << "\t" << AvgThroughput<< "Kbps"<< "\t" << Delay << "\t" << ((ReceivedPackets*100.00)/SentPackets) << "\t" << ((LostPackets*100.00)/SentPackets) << std::endl;
    datarate_summaryOutput.close();
    // std::cout << "6" << std::endl;
    cxArea_summaryOutput << coverage_area*20 << "\t" << AvgThroughput<< "Kbps"<< "\t" << Delay << "\t" << ((ReceivedPackets*100.00)/SentPackets) << "\t" << ((LostPackets*100.00)/SentPackets) << std::endl;
    cxArea_summaryOutput.close();
    flowmonitorOutput.close();
    std::cout << "output directory " << dir << std::endl;
    Simulator::Destroy ();
    // std::cout << "7" << std::endl;
    return 0;
  }

/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 */


#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/mobility-module.h>
#include <ns3/internet-module.h>
#include <ns3/point-to-point-module.h>
#include <ns3/lte-module.h>
#include <ns3/wifi-module.h>
#include <ns3/config-store-module.h>
#include <ns3/spectrum-module.h>
#include <ns3/applications-module.h>
#include <ns3/flow-monitor-module.h>
#include <ns3/propagation-module.h>
#include <ns3/scenario-helper.h>
#include <ns3/laa-wifi-coexistence-helper.h>
//#include "ns3/gtk-config-store.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("lena-simple");

static ns3::GlobalValue g_cellConfigA ("cellConfigA",
                                       "Laa, Lte, or Wifi",
                                        ns3::EnumValue (LAA),
                                        ns3::MakeEnumChecker (WIFI, "Wifi",
                                                              LTE, "Lte",
                                                              LAA, "Laa"));
static ns3::GlobalValue g_cellConfigB ("cellConfigB",
                                       "Laa, Lte, or Wifi",
                                        ns3::EnumValue (WIFI),
                                        ns3::MakeEnumChecker (WIFI, "Wifi",
                                                              LTE, "Lte",
                                                              LAA, "Laa"));

static ns3::GlobalValue g_generateRem ("generateRem",
                                       "if true, will generate a REM and then abort the simulation;"
                                       "if false, will run the simulation normally (without generating any REM)",
                                       ns3::BooleanValue (false),
                                       ns3::MakeBooleanChecker ());

static ns3::GlobalValue g_simTag ("simTag",
                                  "tag to be appended to output filenames to distinguish simulation campaigns",
                                  ns3::StringValue ("default"),
                                  ns3::MakeStringChecker ());

static ns3::GlobalValue g_outputDir ("outputDir",
                                     "directory where to store simulation results",
                                     ns3::StringValue ("./"),
                                     ns3::MakeStringChecker ());

static ns3::GlobalValue g_transport ("transport",
                                     "whether to use 3GPP Ftp, Udp, or Tcp",
                                     ns3::EnumValue (UDP),
                                     ns3::MakeEnumChecker (FTP, "Ftp",
                                                           UDP, "Udp",
                                                           TCP, "Tcp"));
static ns3::GlobalValue g_indoorLossModel ("indoorLossModel",
                                           "TypeId string of indoor propagation loss model",
                                            ns3::StringValue ("ns3::LogDistancePropagationLossModel"),
                                            ns3::MakeStringChecker ());


static ns3::GlobalValue g_channelAccessManager ("ChannelAccessManager",
                                                "Default, DutyCycle, Lbt",
                                                ns3::EnumValue (Lbt),
                                                ns3::MakeEnumChecker (Default, "Default",
                                                                      DutyCycle, "DutyCycle",
                                                                      Lbt, "Lbt"));
void SetupPhyListener (Ptr<SpectrumWifiPhy> phy);
void SetWifiPhy (Ptr<SpectrumWifiPhy> phy);

int main (int argc, char *argv[])
{	
  CommandLine cmd;
  cmd.Parse (argc, argv);
	
  // to save a template default attribute file run it like this:
  // ./waf --command-template="%s --ns3::ConfigStore::Filename=input-defaults.txt --ns3::ConfigStore::Mode=Save --ns3::ConfigStore::FileFormat=RawText" --run src/lte/examples/lena-first-sim
  //
  // to load a previously created default attribute file
  // ./waf --command-template="%s --ns3::ConfigStore::Filename=input-defaults.txt --ns3::ConfigStore::Mode=Load --ns3::ConfigStore::FileFormat=RawText" --run src/lte/examples/lena-first-sim

  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults ();

  DoubleValue doubleValue;
  EnumValue enumValue;
  BooleanValue booleanValue;
  StringValue stringValue;

  GlobalValue::GetValueByName ("cellConfigA", enumValue);
  enum Config_e cellConfigA = (Config_e) enumValue.Get ();
  GlobalValue::GetValueByName ("cellConfigB", enumValue);
  enum Config_e cellConfigB = (Config_e) enumValue.Get ();
  //GlobalValue::GetValueByName ("lteDutyCycle", doubleValue);
  //double lteDutyCycle = doubleValue.Get ();
  GlobalValue::GetValueByName ("generateRem", booleanValue);
  bool generateRem = booleanValue.Get ();
  GlobalValue::GetValueByName ("simTag", stringValue);
  std::string simTag = stringValue.Get ();
  GlobalValue::GetValueByName ("outputDir", stringValue);
  std::string outputDir = stringValue.Get ();
  GlobalValue::GetValueByName ("transport", enumValue);
  enum Transport_e transport = (Transport_e) enumValue.Get ();
  GlobalValue::GetValueByName ("indoorLossModel", stringValue);
   std::string indoorLossModel = stringValue.Get ();
   GlobalValue::GetValueByName ("ChannelAccessManager", enumValue);
   enum Config_ChannelAccessManager channelAccessManager = (Config_ChannelAccessManager) enumValue.Get ();


  // Parse again so you can override default values from the command line
  cmd.Parse (argc, argv);

  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();

/*
  LaaWifiCoexistenceHelper laaWifiCoexistenceHelper;
  laaWifiCoexistenceHelper.ConfigureEnbDevicesForLbt(bsDevices, phyParams);

  Ptr<BasicLbtAccessManager> lbtAccessManager = Create<BasicLbtAccessManager>();
  lbtAccessManager->SetupPhyListener(wifiPhy);
  rrc->SetChannelAccessManager(lbtAccessManager);
*/


  Config::SetDefault ("ns3::ChannelAccessManager::EnergyDetectionThreshold", DoubleValue (-72.0));
  switch (channelAccessManager)
    {
    case Lbt:
    	Config::SetDefault ("ns3::LaaWifiCoexistenceHelper::ChannelAccessManagerType",StringValue ("ns3::BasicLbtAccessManager"));
    	Config::SetDefault ("ns3::BasicLbtAccessManager::TxTime", TimeValue (Seconds (10.0/1000.0)));
    	Config::SetDefault ("ns3::BasicLbtAccessManager::WaitTime", TimeValue (Seconds (150.0/1000.0)));
      break;

    default:
      //default LTE channel access manager will be used, LTE always transmits
      break;
    }

  LogComponentEnableAll (LOG_PREFIX_TIME);
  LogComponentEnableAll (LOG_PREFIX_FUNC);
  LogComponentEnableAll (LOG_PREFIX_NODE);


  // Create nodes and containers
  NodeContainer bsNodesA, bsNodesB;  // for APs and eNBs
  NodeContainer ueNodesA, ueNodesB;  // for STAs and UEs
  NodeContainer allWirelessNodes;  // container to hold all wireless nodes
  // Each network A and B gets one type of node each
  bsNodesA.Create (1);
  bsNodesB.Create (1);
  ueNodesA.Create (1);
  ueNodesB.Create (1);
  allWirelessNodes = NodeContainer (bsNodesA, bsNodesB, ueNodesA, ueNodesB);

  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));   // eNB1/AP in cell 0
  positionAlloc->Add (Vector (50, 10, 0.0)); // AP in cell 1
  positionAlloc->Add (Vector (0.0, 10, 0.0));  // UE1/STA in cell 0
  positionAlloc->Add (Vector (50, 0.0, 0.0));  // STA in cell 1
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (allWirelessNodes);

  bool disableApps = false;
  Time durationTime = Seconds (1);


  // REM settings tuned to get a nice figure for this specific scenario
  Config::SetDefault ("ns3::RadioEnvironmentMapHelper::OutputFile", StringValue ("laa-wifi-simple.rem"));
  Config::SetDefault ("ns3::RadioEnvironmentMapHelper::XMin", DoubleValue (-50));
  Config::SetDefault ("ns3::RadioEnvironmentMapHelper::XMax", DoubleValue (250));
  Config::SetDefault ("ns3::RadioEnvironmentMapHelper::YMin", DoubleValue (-50));
  Config::SetDefault ("ns3::RadioEnvironmentMapHelper::YMax", DoubleValue (250));
  Config::SetDefault ("ns3::RadioEnvironmentMapHelper::XRes", UintegerValue (600));
  Config::SetDefault ("ns3::RadioEnvironmentMapHelper::YRes", UintegerValue (600));
  Config::SetDefault ("ns3::RadioEnvironmentMapHelper::Z", DoubleValue (1.5));

  // we want deterministic behavior in this simple scenario, so we disable shadowing
  Config::SetDefault ("ns3::Ieee80211axIndoorPropagationLossModel::Sigma", DoubleValue (0));

  // Enable aggregation for AC_BE; see bug 2471 in tracker
  Config::SetDefault ("ns3::RegularWifiMac::BE_BlockAckThreshold", UintegerValue (2));

  // Specify some physical layer parameters that will be used below and
  // in the scenario helper.
  PhyParams phyParams;
  phyParams.m_bsTxGain = 5; // dB antenna gain
  phyParams.m_bsRxGain = 5; // dB antenna gain
  phyParams.m_bsTxPower = 18; // dBm
  phyParams.m_bsNoiseFigure = 5; // dB
  phyParams.m_ueTxGain = 0; // dB antenna gain
  phyParams.m_ueRxGain = 0; // dB antenna gain
  phyParams.m_ueTxPower = 18; // dBm
  phyParams.m_ueNoiseFigure = 9; // dB

  // calculate rx power corresponding to d2 for logging purposes
  // note: a separate propagation loss model instance is used, make
  // sure the settings are consistent with the ones used for the
  // simulation
  const uint32_t earfcn = 255444;
  double dlFreq = LteSpectrumValueHelper::GetCarrierFrequency (earfcn);
  Ptr<PropagationLossModel> plm = CreateObject<Ieee80211axIndoorPropagationLossModel> ();
  plm->SetAttribute ("Frequency", DoubleValue (dlFreq));
  double txPowerFactors = phyParams.m_bsTxGain + phyParams.m_ueRxGain +
                          phyParams.m_bsTxPower;
  double rxPowerDbmD1 = plm->CalcRxPower (txPowerFactors,
                                          bsNodesA.Get (0)->GetObject<MobilityModel> (),
                                          ueNodesA.Get (0)->GetObject<MobilityModel> ());
  double rxPowerDbmD2 = plm->CalcRxPower (txPowerFactors,
                                          bsNodesA.Get (0)->GetObject<MobilityModel> (),
                                          ueNodesB.Get (0)->GetObject<MobilityModel> ());


  std::ostringstream simulationParams;

  simulationParams << 10 << " " << 50 << " "
                   << rxPowerDbmD1 << " "
                   << rxPowerDbmD2 << " "
                   << 1.0 << " ";

 // ConfigureAndRunScenario (bsNodesA, bsNodesB, ueNodesA, ueNodesB, phyParams, durationTime);
 // ConfigureAndRunScenario (cellConfigA, cellConfigB, bsNodesA, bsNodesB, ueNodesA, ueNodesB, phyParams, durationTime, simulationParams.str ());

  ConfigureAndRunScenario (cellConfigA, cellConfigB, bsNodesA, bsNodesB, ueNodesA, ueNodesB, phyParams, durationTime, transport, indoorLossModel, disableApps, 1.0, generateRem, outputDir + "/laa_wifi_simple_" + simTag, simulationParams.str ());

  return 0;
}

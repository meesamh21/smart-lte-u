/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
 * Copyright (c) 2015 University of Washington
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
 *  Author: Biljana Bojovic <bbojovic@cttc.es> and Tom Henderson <tomh@tomh.org>
 */

#include "basic-lbt-access-manager.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/spectrum-wifi-phy.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "iostream"
#include "iomanip"
#include "string"
#include "fstream"

namespace ns3 {



NS_LOG_COMPONENT_DEFINE ("BasicLbtAccessManager");

NS_OBJECT_ENSURE_REGISTERED (BasicLbtAccessManager);

TypeId
BasicLbtAccessManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BasicLbtAccessManager")
    .SetParent<LbtAccessManager> ()
    .SetGroupName ("lte")
    .AddConstructor<BasicLbtAccessManager> ()
    .AddAttribute ("TxTime",
                   "Duration of channel access grant.",
                   TimeValue (MilliSeconds (100)),
                   MakeTimeAccessor (&BasicLbtAccessManager::SetTxTime,
                                     &BasicLbtAccessManager::GetTxTime),
                   MakeTimeChecker ())
    .AddAttribute ("WaitTime",
                   "Duration of time to wait before checking again the channel state.",
                   TimeValue (MilliSeconds (100)),
                   MakeTimeAccessor (&BasicLbtAccessManager::SetWaitTime,
                                     &BasicLbtAccessManager::GetWaitTime),
                   MakeTimeChecker ())
  ;
  return tid;

}


void BasicLbtAccessManager::SetTxTime (Time txTime)
{
  m_txTime = txTime;
}

void BasicLbtAccessManager::SetWaitTime (Time waitTime)
{
  m_waitTime = waitTime;
}

Time BasicLbtAccessManager::GetTxTime () const
{
  return m_txTime;
}

Time BasicLbtAccessManager::GetWaitTime () const
{
  return m_waitTime;
}


void
BasicLbtAccessManager::DoInitialize ()
{
	m_learningRate = 0.85;
	m_discountFactor = 0.99;


}


BasicLbtAccessManager::BasicLbtAccessManager ()
  : m_wifiPhy (0),
    m_txTime (10),
    m_waitTime (100)
{
  NS_LOG_FUNCTION (this);
  m_timeout = Simulator::Now ();
  m_waitTimeout = Simulator::Now ();
  idle_count = 0;
}


void
BasicLbtAccessManager::SetWifiPhy (Ptr<SpectrumWifiPhy> phy)
{
  NS_LOG_FUNCTION (this);
  SetupPhyListener (phy);
  m_wifiPhy = phy;
  // Configure the WifiPhy to treat each incoming signal as a foreign signal
  // (energy detection only)
  m_wifiPhy->SetAttribute ("DisableWifiReception", BooleanValue (true));
}

BasicLbtAccessManager::~BasicLbtAccessManager ()
{
  NS_LOG_FUNCTION (this);

}

void BasicLbtAccessManager::DoRequestAccess ()
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG (!m_accessGrantedCallback.IsNull (), "Access granted callback is not initialized!");
  NS_ASSERT_MSG (m_wifiPhy != 0, "WifiPhy not set!");
  // The approach taken here is that, if channel is in IDLE state then channel access is granted, otherwise the request is postponed
  // predetermined amount of time specified by waitTime attribute.

  if (m_wifiPhy->IsStateIdle () and !IsWaiting ())
    {
	  idle_count = idle_count + 1000000;
      m_accessGrantedCallback (m_txTime);
      Simulator::Schedule (m_txTime, &BasicLbtAccessManager::Wait, this);
    }
  else
    {
      if (IsWaiting ())
        {
          Simulator::Schedule (m_waitTimeout - Simulator::Now (), &BasicLbtAccessManager::DoRequestAccess, this);

        }
      else
        {
          Simulator::Schedule (MilliSeconds (1), &BasicLbtAccessManager::DoRequestAccess, this);
        }
     // result = GetTxTime();
    }

  MachineLearning();
}


void BasicLbtAccessManager::Wait ()
{
  NS_LOG_FUNCTION (this);
  m_waitTimeout = Simulator::Now () + m_waitTime;
  result = m_waitTime;
}

bool BasicLbtAccessManager::IsWaiting ()
{
  if (m_waitTimeout > Simulator::Now ())
    {
      return true;
    }
  else
    {
      return false;
    }
}

void BasicLbtAccessManager:: MachineLearning ()
{
	//Generate Q Table where TxTime Values ranging from 1 and 200ms
	Time **Q = new Time*[100];
	for(int i = 0; i < 100; ++i) {
	    Q[i] = new Time[2];
	    Q[i][0] = Time (x/10.0);
	    x = x + 500000.0;
	}
			//Pick a random txTime from Q table
			int random =(rand()%100);
			a = Q[random][0];
			SetTxTime(a);
			//std::cout << a.As (Time::MS) << std::endl;

			//Get new result
			s1 = (Time)idle_count;


			//Compare and get reward
			//Reward algorithm is tricky so this function simply allow only higher result values in the Q Table
			if(s1 > s0){
				s0 = s1;
				//Update Q table
				Q[random][1] = s1;
			//std::cout << Q[random][1].As (Time::MS) << std::endl;
			}



}

}







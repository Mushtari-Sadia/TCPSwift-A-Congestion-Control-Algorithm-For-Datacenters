/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 ResiliNets, ITTC, University of Kansas
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
 * Author: Truc Anh N. Nguyen <annguyen@ittc.ku.edu>
 *
 * James P.G. Sterbenz <jpgs@ittc.ku.edu>, director
 * ResiliNets Research Group  http://wiki.ittc.ku.edu/resilinets
 * Information and Telecommunication Technology Center (ITTC)
 * and Department of Electrical Engineering and Computer Science
 * The University of Kansas Lawrence, KS USA.
 */

#include "tcp-swift.h"
#include "tcp-socket-state.h"
#include "ns3/simulator.h"
#include "rtt-estimator.h"
#include "ns3/nstime.h"
#include "tcp-socket-base.h"

#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TcpSwift");
NS_OBJECT_ENSURE_REGISTERED (TcpSwift);

TypeId
TcpSwift::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpSwift")
    .SetParent<TcpNewReno> ()
    .AddConstructor<TcpSwift> ()
    .SetGroupName ("Internet")
    .AddAttribute ("Alpha", "Lower bound of packets in network",
                   UintegerValue (2),
                   MakeUintegerAccessor (&TcpSwift::m_alpha),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Beta", "Upper bound of packets in network",
                   UintegerValue (4),
                   MakeUintegerAccessor (&TcpSwift::m_beta),
                   MakeUintegerChecker<uint32_t> ())
    
    

  ;
  return tid;
}

TcpSwift::TcpSwift (void)
  : TcpNewReno (),
    m_alpha (2),
    m_beta (4),
    m_baseRtt (Time::Max ()),
    m_minRtt (Time::Max ()),
    m_cntRtt (0),
    m_doingSwiftNow (true),
    m_begSndNxt (0)
{
  NS_LOG_FUNCTION (this);
  std::cout << "swift running" << std::endl;
}

TcpSwift::TcpSwift (const TcpSwift& sock)
  : TcpNewReno (sock),
    m_alpha (sock.m_alpha),
    m_beta (sock.m_beta),
    m_baseRtt (sock.m_baseRtt),
    m_minRtt (sock.m_minRtt),
    m_cntRtt (sock.m_cntRtt),
    m_doingSwiftNow (true),
    m_begSndNxt (0)
{
  NS_LOG_FUNCTION (this);
  std::cout << "swift running - from copy constructor" << std::endl;
}

TcpSwift::~TcpSwift (void)
{
  NS_LOG_FUNCTION (this);
}

Ptr<TcpCongestionOps>
TcpSwift::Fork (void)
{
  return CopyObject<TcpSwift> (this);
}

void
TcpSwift::PktsAcked (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked,
                     const Time& rtt)
{
  tcb->retransmit_count=0;
  NS_LOG_FUNCTION (this << tcb << segmentsAcked << rtt);

  if (rtt.IsZero ())
    {
      return;
    }

  m_minRtt = std::min (m_minRtt, rtt);
  NS_LOG_DEBUG ("Updated m_minRtt = " << m_minRtt);

  m_baseRtt = std::min (m_baseRtt, rtt);
  NS_LOG_DEBUG ("Updated m_baseRtt = " << m_baseRtt);

  // Update RTT counter
  m_cntRtt++;
  NS_LOG_DEBUG ("Updated m_cntRtt = " << m_cntRtt);
  
  
  
}

void
TcpSwift::EnableSwift (Ptr<TcpSocketState> tcb)
{
  NS_LOG_FUNCTION (this << tcb);

  m_doingSwiftNow = true;
  m_begSndNxt = tcb->m_nextTxSequence;
  m_cntRtt = 0;
  m_minRtt = Time::Max ();
}

void
TcpSwift::DisableSwift ()
{
  NS_LOG_FUNCTION (this);

  m_doingSwiftNow = false;
}

void
TcpSwift::CongestionStateSet (Ptr<TcpSocketState> tcb,
                              const TcpSocketState::TcpCongState_t newState)
{
  NS_LOG_FUNCTION (this << tcb << newState);
  if (newState == TcpSocketState::CA_OPEN)
    {
      EnableSwift (tcb);
    }
  else
    {
      DisableSwift ();
    }
}

void
TcpSwift::IncreaseWindow (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
{
  NS_LOG_FUNCTION (this << tcb << segmentsAcked);

  if (!m_doingSwiftNow)
    {
      // If Swift is not on, we follow NewReno algorithm
      // NS_LOG_LOGIC ("Swift is not turned on, we follow NewReno algorithm.");
      //std::cout << "Swift is not turned on, we follow NewReno algorithm." << std::endl;
      TcpNewReno::IncreaseWindow (tcb, segmentsAcked);
      return;
    }

  if (tcb->m_lastAckedSeq >= m_begSndNxt)
    { // A Swift cycle has finished, we do Swift cwnd adjustment every RTT.

      // NS_LOG_LOGIC ("A Swift cycle has finished, we adjust cwnd once per RTT.");
      //std::cout << "A Swift cycle has finished, we adjust cwnd once per RTT." << std::endl;
      
      //modification-1
      if (Simulator::Now().GetSeconds()-tcb->t_last_decrease.GetSeconds()>=m_baseRtt.GetSeconds())
      {
        //std::cout << "canDecrease set" << std::endl;
        tcb->m_canDecrease = true;
      }
      else
      {
        tcb->m_canDecrease = false;
      }

      // Save the current right edge for next Swift cycle
      m_begSndNxt = tcb->m_nextTxSequence;

      /*
       * We perform Swift calculations only if we got enough RTT samples to
       * insure that at least 1 of those samples wasn't from a delayed ACK.
       */
      if (m_cntRtt <= 2)
        {  // We do not have enough RTT samples, so we should behave like Reno
          // NS_LOG_LOGIC ("We do not have enough RTT samples to do Swift, so we behave like NewReno.");
          //std::cout << "We do not have enough RTT samples to do Swift, so we behave like NewReno." << std::endl;
      
          TcpNewReno::IncreaseWindow (tcb, segmentsAcked);
        }
      else
        {
          // NS_LOG_LOGIC ("We have enough RTT samples to perform Swift calculations");
          //std::cout << "We have enough RTT samples to do Swift" << std::endl;
      
          
          if (tcb->m_cWnd < tcb->m_ssThresh)
            {     // Slow start mode
              // NS_LOG_LOGIC ("We are in slow start and diff < m_gamma, so we "
                            // "follow NewReno slow start");
              //std::cout << "We are in slow start and diff < m_gamma, so we follow NewReno slow start" << std::endl;
                    
              TcpNewReno::SlowStart (tcb, segmentsAcked);
              NS_LOG_LOGIC ("Cond0=> cwnd = "+std::to_string(tcb->m_cWnd)+"\n");
            }
          else
            {     // Linear increase/decrease mode
              // NS_LOG_LOGIC ("We are in linear increase/decrease mode");
              //std::cout << "We are in linear increase/decrease mode" << std::endl;
               
              uint32_t segCwnd = tcb->GetCwndInSegments ();
              double target_delay = TargetDelay();
              if (m_baseRtt.GetSeconds() < target_delay)
              {
                if( segCwnd >= tcb->m_segmentSize)
                {
                    tcb->m_cWnd = tcb->m_cWnd + (m_alpha/tcb->m_cWnd)*segmentsAcked;
                    // tcb->updateMaxMinCwnd();
                    NS_LOG_LOGIC ("Cond1.1=> cwnd = "+std::to_string(tcb->m_cWnd)+"\n");
                }
                else
                {
                    tcb->m_cWnd = tcb->m_cWnd + m_alpha*segmentsAcked;  
                    // tcb->updateMaxMinCwnd();
                    NS_LOG_LOGIC ("Cond1.2=> cwnd = "+std::to_string(tcb->m_cWnd)+"\n");
                }
              }
              else
              {
                if(tcb->m_canDecrease)
                {
                  double value1 = m_beta*(m_baseRtt.GetSeconds()-target_delay)/m_baseRtt.GetSeconds();
                  value1=1-value1;
                  double value2 = (1-tcb->m_gamma)*tcb->m_segmentSize*segCwnd;
                  tcb->m_cWnd = std::max(value1,value2);
                  // tcb->updateMaxMinCwnd();
                  NS_LOG_LOGIC ("Cond2=> cwnd = "+std::to_string(tcb->m_cWnd)+"\n");
                }
              }
            }
          tcb->m_ssThresh = std::max (tcb->m_ssThresh, 3 * tcb->m_cWnd / 4);
          NS_LOG_DEBUG ("Updated ssThresh = " << tcb->m_ssThresh);
        }
    

      // Reset cntRtt & minRtt every RTT
      m_cntRtt = 0;
      m_minRtt = Time::Max ();
    }
    
    else if (tcb->m_cWnd < tcb->m_ssThresh)
      {
        TcpNewReno::SlowStart (tcb, segmentsAcked);
      }
}

std::string
TcpSwift::GetName () const
{
  return "TcpSwift";
}

uint32_t
TcpSwift::GetSsThresh (Ptr<const TcpSocketState> tcb,
                       uint32_t bytesInFlight)
{
  NS_LOG_FUNCTION (this << tcb << bytesInFlight);
  return std::max (std::min (tcb->m_ssThresh.Get (), tcb->m_cWnd.Get () - tcb->m_segmentSize), 2 * tcb->m_segmentSize);
}


//modification-2
double
TcpSwift::TargetDelay()
{
    double target_delay=0.025;
    return target_delay;
}

 // namespace ns3
}
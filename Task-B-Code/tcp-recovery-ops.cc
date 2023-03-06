/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018 NITK Surathkal
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
 * Author: Viyom Mittal <viyommittal@gmail.com>
 *         Vivek Jain <jain.vivek.anand@gmail.com>
 *         Mohit P. Tahiliani <tahiliani@nitk.edu.in>
 *
 */
#include "tcp-recovery-ops.h"
#include "tcp-socket-state.h"
#include "ns3/simulator.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TcpRecoveryOps");

NS_OBJECT_ENSURE_REGISTERED (TcpRecoveryOps);

TypeId
TcpRecoveryOps::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpRecoveryOps")
    .SetParent<Object> ()
    .SetGroupName ("Internet")
  ;
  return tid;
}

TcpRecoveryOps::TcpRecoveryOps () : Object ()
{
  NS_LOG_FUNCTION (this);
}

TcpRecoveryOps::TcpRecoveryOps (const TcpRecoveryOps &other) : Object (other)
{
  NS_LOG_FUNCTION (this);
}

TcpRecoveryOps::~TcpRecoveryOps ()
{
  NS_LOG_FUNCTION (this);
}

void
TcpRecoveryOps::UpdateBytesSent (uint32_t bytesSent)
{
  NS_LOG_FUNCTION (this << bytesSent);
}

// Classic recovery

NS_OBJECT_ENSURE_REGISTERED (TcpClassicRecovery);

TypeId
TcpClassicRecovery::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpClassicRecovery")
    .SetParent<TcpRecoveryOps> ()
    .SetGroupName ("Internet")
    .AddConstructor<TcpClassicRecovery> ()
  ;
  return tid;
}

TcpClassicRecovery::TcpClassicRecovery (void) : TcpRecoveryOps ()
{
  NS_LOG_FUNCTION (this);
}

TcpClassicRecovery::TcpClassicRecovery (const TcpClassicRecovery& sock)
  : TcpRecoveryOps (sock)
{
  NS_LOG_FUNCTION (this);
}

TcpClassicRecovery::~TcpClassicRecovery (void)
{
  NS_LOG_FUNCTION (this);
}

void
TcpClassicRecovery::EnterRecovery (Ptr<TcpSocketState> tcb, uint32_t dupAckCount,
                                   uint32_t unAckDataCount, uint32_t deliveredBytes)
{
  NS_LOG_FUNCTION (this << tcb << dupAckCount << unAckDataCount);
  NS_UNUSED (unAckDataCount);
  NS_UNUSED (deliveredBytes);
  tcb->m_cWnd = tcb->m_ssThresh;
  tcb->m_cWndInfl = tcb->m_ssThresh + (dupAckCount * tcb->m_segmentSize);
}

void
TcpClassicRecovery::DoRecovery (Ptr<TcpSocketState> tcb, uint32_t deliveredBytes)
{
  NS_LOG_FUNCTION (this << tcb << deliveredBytes);
  NS_UNUSED (deliveredBytes);
  tcb->m_cWndInfl += tcb->m_segmentSize;
}

void
TcpClassicRecovery::ExitRecovery (Ptr<TcpSocketState> tcb)
{
  NS_LOG_FUNCTION (this << tcb);
  // Follow NewReno procedures to exit FR if SACK is disabled
  // (RFC2582 sec.3 bullet #5 paragraph 2, option 2)
  // In this implementation, actual m_cWnd value is reset to ssThresh
  // immediately before calling ExitRecovery(), so we just need to
  // reset the inflated cWnd trace variable
  tcb->m_cWndInfl = tcb->m_ssThresh.Get ();
}

std::string
TcpClassicRecovery::GetName () const
{
  return "TcpClassicRecovery";
}

Ptr<TcpRecoveryOps>
TcpClassicRecovery::Fork ()
{
  return CopyObject<TcpClassicRecovery> (this);
}

// Swift recovery

NS_OBJECT_ENSURE_REGISTERED (TcpSwiftRecovery);

TypeId
TcpSwiftRecovery::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpSwiftRecovery")
    .SetParent<TcpRecoveryOps> ()
    .SetGroupName ("Internet")
    .AddConstructor<TcpSwiftRecovery> ()
  ;
  return tid;
}

TcpSwiftRecovery::TcpSwiftRecovery (void) : TcpRecoveryOps ()
{
  NS_LOG_FUNCTION (this);
}

TcpSwiftRecovery::TcpSwiftRecovery (const TcpSwiftRecovery& sock)
  : TcpRecoveryOps (sock)
{
  NS_LOG_FUNCTION (this);
}

TcpSwiftRecovery::~TcpSwiftRecovery (void)
{
  NS_LOG_FUNCTION (this);
}

void
TcpSwiftRecovery::EnterRecovery (Ptr<TcpSocketState> tcb, uint32_t dupAckCount,
                                   uint32_t unAckDataCount, uint32_t deliveredBytes)
{
  NS_LOG_FUNCTION (this << tcb << dupAckCount << unAckDataCount);
  NS_UNUSED (unAckDataCount);
  NS_UNUSED (deliveredBytes);
  tcb->m_cWnd = tcb->m_ssThresh;
  tcb->m_cWndInfl = tcb->m_ssThresh + (dupAckCount * tcb->m_segmentSize);

  //modification
  tcb->retransmit_count = 0;
  uint32_t cwnd_prev = tcb->m_cWnd;
  if (tcb->m_canDecrease)
    {
      uint32_t segCwnd = tcb->GetCwndInSegments ();
      tcb->m_cWnd = (1-tcb->m_gamma)*tcb->m_cWnd*tcb->m_segmentSize*segCwnd;
      
    }
  tcb->Clamp();
  if(tcb->m_cWnd<=cwnd_prev)
  {
    tcb->t_last_decrease = Simulator::Now();
  }

}

void
TcpSwiftRecovery::DoRecovery (Ptr<TcpSocketState> tcb, uint32_t deliveredBytes)
{
  NS_LOG_FUNCTION (this << tcb << deliveredBytes);
  NS_UNUSED (deliveredBytes);
  tcb->m_cWndInfl += tcb->m_segmentSize;
}

void
TcpSwiftRecovery::ExitRecovery (Ptr<TcpSocketState> tcb)
{
  NS_LOG_FUNCTION (this << tcb);
  // Follow NewReno procedures to exit FR if SACK is disabled
  // (RFC2582 sec.3 bullet #5 paragraph 2, option 2)
  // In this implementation, actual m_cWnd value is reset to ssThresh
  // immediately before calling ExitRecovery(), so we just need to
  // reset the inflated cWnd trace variable
  tcb->m_cWndInfl = tcb->m_ssThresh.Get ();
}

std::string
TcpSwiftRecovery::GetName () const
{
  return "TcpSwiftRecovery";
}

Ptr<TcpRecoveryOps>
TcpSwiftRecovery::Fork ()
{
  return CopyObject<TcpSwiftRecovery> (this);
}


} // namespace ns3


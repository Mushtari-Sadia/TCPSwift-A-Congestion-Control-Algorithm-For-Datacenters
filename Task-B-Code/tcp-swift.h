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

#ifndef TCPSWIFT_H
#define TCPSWIFT_H

#include "tcp-congestion-ops.h"
#include "tcp-socket-base.h"

namespace ns3 {

class TcpSocketState;

/**
 * \ingroup congestionOps
 *
 * \brief An implementation of TCP Swift
 *
 * TCP Swift is a pure delay-based congestion control algorithm implementing a proactive
 * scheme that tries to prevent packet drops by maintaining a small backlog at the
 * bottleneck queue.
 *
 * Swift continuously measures the actual throughput a connection achieves as shown in
 * Equation (1) and compares it with the expected throughput calculated in Equation (2).
 * The difference between these 2 sending rates in Equation (3) reflects the amount of
 * extra packets being queued at the bottleneck.
 *
 *              actual = cwnd / RTT        (1)
 *              expected = cwnd / BaseRTT  (2)
 *              diff = expected - actual   (3)
 *
 * To avoid congestion, Swift linearly increases/decreases its congestion window to ensure
 * the diff value fall between the 2 predefined thresholds, alpha and beta.
 * diff and another threshold, gamma, are used to determine when Swift should change from
 * its slow-start mode to linear increase/decrease mode.
 *
 * Following the implementation of Swift in Linux, we use 2, 4, and 1 as the default values
 * of alpha, beta, and gamma, respectively.
 *
 * More information: http://dx.doi.org/10.1109/49.464716
 */

class TcpSwift : public TcpNewReno//, public TcpSocketBase
{ 
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * Create an unbound tcp socket.
   */
  TcpSwift (void);

  /**
   * \brief Copy constructor
   * \param sock the object to copy
   */
  TcpSwift (const TcpSwift& sock);
  virtual ~TcpSwift (void);

  virtual std::string GetName () const;

  /**
   * \brief Compute RTTs needed to execute Swift algorithm
   *
   * The function filters RTT samples from the last RTT to find
   * the current smallest propagation delay + queueing delay (minRtt).
   * We take the minimum to avoid the effects of delayed ACKs.
   *
   * The function also min-filters all RTT measurements seen to find the
   * propagation delay (baseRtt).
   *
   * \param tcb internal congestion state
   * \param segmentsAcked count of segments ACKed
   * \param rtt last RTT
   *
   */
  virtual void PktsAcked (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked,
                          const Time& rtt);

  /**
   * \brief Enable/disable Swift algorithm depending on the congestion state
   *
   * We only start a Swift cycle when we are in normal congestion state (CA_OPEN state).
   *
   * \param tcb internal congestion state
   * \param newState new congestion state to which the TCP is going to switch
   */
  virtual void CongestionStateSet (Ptr<TcpSocketState> tcb,
                                   const TcpSocketState::TcpCongState_t newState);

  /**
   * \brief Adjust cwnd following Swift linear increase/decrease algorithm
   *
   * \param tcb internal congestion state
   * \param segmentsAcked count of segments ACKed
   */
  virtual void IncreaseWindow (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked);

  /**
   * \brief Get slow start threshold following Swift principle
   *
   * \param tcb internal congestion state
   * \param bytesInFlight bytes in flight
   *
   * \return the slow start threshold value
   */
  virtual uint32_t GetSsThresh (Ptr<const TcpSocketState> tcb,
                                uint32_t bytesInFlight);

  // virtual Ptr<TcpSocketBase> Fork ();
  virtual Ptr<TcpCongestionOps> Fork ();
  // virtual void ReTxTimeout ();
  

protected:
private:
  /**
   * \brief Enable Swift algorithm to start taking Swift samples
   *
   * Swift algorithm is enabled in the following situations:
   * 1. at the establishment of a connection
   * 2. after an RTO
   * 3. after fast recovery
   * 4. when an idle connection is restarted
   *
   * \param tcb internal congestion state
   */
  void EnableSwift (Ptr<TcpSocketState> tcb);

  /**
   * \brief Stop taking Swift samples
   */
  void DisableSwift ();

  double TargetDelay ();
  // void ReTxTimeout ()
  // {
  //   TcpSocketBase::ReTxTimeout ();
  //   std::cout << "retxtimeout called" << std::endl;
  // }

private:
  uint32_t m_alpha;                  //!< Alpha threshold, lower bound of packets in network
  uint32_t m_beta;                   //!< Beta threshold, upper bound of packets in network
                  //!< Gamma threshold, limit on increase
  Time m_baseRtt;                    //!< Minimum of all Swift RTT measurements seen during connection
  Time m_minRtt;                     //!< Minimum of all RTT measurements within last RTT
  uint32_t m_cntRtt;                 //!< Number of RTT measurements during last RTT
  bool m_doingSwiftNow;              //!< If true, do Swift for this RTT
  SequenceNumber32 m_begSndNxt;      //!< Right edge during last RTT
  
};

} // namespace ns3

#endif // TCPSWIFT_H

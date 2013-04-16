//
//   Copyright (C) 1998-2011 by Galerna Project, the University of
//   Cantabria, Spain.
//
//   This file is part of the TOPAZ network simulator, originallty developed
//   at the Unviersity of Cantabria
//
//   TOPAZ shares a large proportion of code with SICOSYS which was 
//   developed by V.Puente and J.M.Prellezo
//
//   TOPAZ has been developed by P.Abad, L.G.Menezo, P.Prieto and
//   V.Puente
// 
//  --------------------------------------------------------------------
//
//  If your use of this software contributes to a published paper, we
//  request that you (1) cite our summary paper that appears on our
//  website (http://www.atc.unican.es/topaz/) and (2) e-mail a citation
//  for your published paper to topaz@atc.unican.es
//  
//  If you redistribute derivatives of this software, we request that
//  you notify us and either (1) ask people to register with us at our
//  website (http://www.atc.unican.es/topaz/) or (2) collect registration
//  information and periodically send it to us.
//  
//   --------------------------------------------------------------------
//
//   TOPAZ is free software; you can redistribute it and/or
//   modify it under the terms of version 2 of the GNU General Public
//   License as published by the Free Software Foundation.
//
//   TOPAZ is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//   General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with the TOPAZ simulator; if not, write to the Free Software
//   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
//   02111-1307, USA
//
//   The GNU General Public License is contained in the file LICENSE.
//
// 	
//*************************************************************************
//:
//    File: TPZMessage.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZMessage_HPP__
#define __TPZMessage_HPP__

//*************************************************************************

#ifndef __TPZPosition_HPP__
#include <TPZPosition.hpp>
#endif

#ifndef __TPZPair_HPP__
#include <TPZPair.hpp>
#endif

#ifndef __TPZSequence_HPP__
#include <TPZSequence.hpp>
#endif

typedef TPZPair<unsigned,unsigned> TUPair;
typedef TPZSequence<TUPair> TPortList;

//*************************************************************************
//   # Define TPZMSGTRACE (msg, text) court <<(msg) -> getIdentifier () <<text <<endl;
#define TPZMSGTRACE(msg,text) 

class TPZMessage {

public:
	enum MESSAGETYPE {INVALID = 0,
		HEADER,
		INFO,
		TAIL,
		eREADY,
		STOP,
		REQ,
		BUNDLE,
		HEADTAIL,
		MREQ};

	TPZMessage();
	TPZMessage(const TPZMessage& aMessage);
	virtual ~TPZMessage();

	TPZMessage& operator=(const TPZMessage& aMessage);
	Boolean operator==(const TPZMessage& aMessage) const;
	virtual TPZString asString() const;

	static void assignUniqueId(TPZMessage* msg);

	MESSAGETYPE type() const {
		return m_Type;
	}

	unsigned long getIdentifier() const {
		return m_Identifier;
	}

	void setType(const MESSAGETYPE& tipo) {
		m_Type = tipo;
	}

	unsigned short flitNumber() const {
		return m_FlitNumber;
	}

	void setFlitNumber(unsigned order) {
		m_FlitNumber = order;
	}

	unsigned short packetNumber() const {
		return m_PacketNumber;
	}

	void setPacketNumber(unsigned order) {
		m_PacketNumber = order;
	}

	unsigned short messageSize() const {
		return m_MessageSize;
	}

	void setMessageSize(unsigned packets) {
		m_MessageSize = packets;
	}

	unsigned short packetSize() const {
		return m_PacketSize;
	}

	void setPacketSize(unsigned flits) {
		m_PacketSize = flits;
	}

	uTIME generationTime() const {
		return m_GenerationTime;
	}

	void setGenerationTime(const uTIME& aTime) {
		m_GenerationTime = aTime;
	}

	uTIME packetInjectionTime() const {
		return m_InjectionTime;
	}

	void setPacketInjectionTime(const uTIME& aTime) {
		m_InjectionTime = aTime;
	}

	TPZPosition& source() const {
		return (TPZPosition&)m_SourcePosition;
	}

	void setSource(const TPZPosition& org) {
		m_SourcePosition = org;
	}

	TPZPosition& destiny() const {
		return (TPZPosition&)m_DestinyPosition;
	}

	void setDestiny(const TPZPosition& dst) {
		m_DestinyPosition = dst;
	}

	TPZROUTINGTYPE getRoutingPort() const {
		return m_RoutingPort;
	}

	void setRoutingPort(TPZROUTINGTYPE port) {
		m_RoutingPort = port;
	}

	Boolean isHeader() const {
		return m_Type==HEADER;
	}

	Boolean isTail() const {
		return m_Type==TAIL;
	}
	Boolean isHeadTail() const { 
	        return m_Type==HEADTAIL; 
	}

	Boolean isBundle() const {
		return m_Type==BUNDLE;
	}

	int short delta(int short header=0) const {
		return m_Delta[header];
	}

	int short deltaAbs(int short header=0) const {
		return (m_Delta[header]>=0) ? m_Delta[header] : (-m_Delta[header]);
	}

	void setDelta(int short delt, int short header=0) {
		m_Delta[header] = delt;
	}

	Boolean getLastMissRouted() {
		return m_lastMissRouted;
	}

	void setLastMissRouted(Boolean flag) {
		m_lastMissRouted=flag;
	}

	void incDistance() {
		m_distance++;
	}

	void initDistance() {
		m_distance=0;
	}

	unsigned long getDistance() const {
		return m_distance;
	}

	unsigned short getChannel() const {
		return m_Channel;
	}

	void setChannel(unsigned channel) {
		m_Channel = channel;
	}

	TPortList* getPortList() const {
		return m_PortList;
	}

	void setPortList(TPortList* portList) {
		m_PortList = portList;
	}

	void* getExternalInfo() const {
		return m_ExternalInfo;
	}

	void setExternalInfo(void* ptr) {
		m_ExternalInfo = ptr;
	}

	void destroyPortList();

	unsigned getMessageLength() const {
		return m_MessageSize;
	}

	unsigned getPacketLength() const {
		return m_PacketSize;
	}

	void setVnet(int vnet) {
		m_vnet=vnet;
	}

	int getVnet() const {
		return m_vnet;
	}

	Boolean isOrdered() const {
		return m_nonAdap;
	}

	void setOrdered() {
		m_nonAdap=true;
	}

	void clearOrdered() {
		m_nonAdap=true;
	}
        
	Boolean isOnScape() const {
		return m_onScape;
	}

	void setOnScape() {
		m_onScape=true;
	}
	
	unsigned getTimesMiss() const {
	        return m_timesMiss;
	}
	
	void incrTimesMiss() {
	        m_timesMiss++;
	}
        
	void setMulticast() {
		m_multicast=true;
	}

	void clearMulticast() {
		m_multicast=false;
	}

	Boolean isMulticast() const {
		return m_multicast;
	}

	void setMsgmask(unsigned long long mascara) {
		m_Msgmask=mascara;
	}

	unsigned long long getMsgmask() const {
		return m_Msgmask;
	}
	
	unsigned long getHopCount() const {
		return m_HopCount;
	}
	void incrHopCount() {
		m_HopCount++;
	}
	void flushHopCount() {
		m_HopCount=0;
	}
	void setHopCount(unsigned long HopCo) {
		m_HopCount=HopCo;
	}

	uTIME getDelayMultiport() const {
		return m_DelayMultiport;
	}

	void setDelayMultiport(const uTIME& delay) {
		m_DelayMultiport=delay;
	}

	TPZROUTINGTYPE getLastDirMiss() const {
		return m_LastDirMiss;
	}

	void setLastDirMiss(TPZROUTINGTYPE port) {
		m_LastDirMiss = port;
	}

	void resetMultiportNumber() {
		m_multiportNumber=0;
	}
	
	void incrMultiportNumber() {
		m_multiportNumber++;
	}

	unsigned long getMultiportNumber() const {
		return m_multiportNumber;
	}

	void setInOrderInputDirection(TPZROUTINGTYPE direction) {
		m_InOrderInputDirection = direction;
	}

	TPZROUTINGTYPE getInOrderInputDirection() {
		return m_InOrderInputDirection;
	}
	
	void setInputInterfaz(unsigned interfaz) {
		m_inputInterfaz = interfaz;
	}

	unsigned getInputInterfaz() {
		return m_inputInterfaz;
	}
#ifdef PTOPAZ
	unsigned getPoolIndex() const
        {
           return m_PoolIndex;
        }

        void setPoolIndex(unsigned n)
        {
           m_PoolIndex = n;
        }
#endif

	// Run time information
	DEFINE_RTTI(TPZMessage);

private:
	MESSAGETYPE m_Type;
	unsigned short m_FlitNumber;
	unsigned short m_PacketNumber;
	unsigned short m_MessageSize;
	unsigned short m_PacketSize;
	uTIME m_GenerationTime;
	uTIME m_InjectionTime;
	uTIME m_DelayMultiport;
	TPZPosition m_SourcePosition;
	TPZPosition m_DestinyPosition;
	unsigned long m_distance;
	TPZROUTINGTYPE m_RoutingPort;
	int short m_Delta[3];
	unsigned short m_Channel;
	int m_vnet;
	Boolean m_nonAdap;
	unsigned long m_Identifier;
	static unsigned long sm_Identifier;
	TPortList* m_PortList;
	void* m_ExternalInfo;
	
	//Miss Routing Suport.
	TPZROUTINGTYPE m_LastDirMiss;
	unsigned long m_HopCount;
	unsigned long m_multiportNumber;
	Boolean m_lastMissRouted;
	Boolean m_onScape;
	unsigned m_timesMiss;
		
	TPZROUTINGTYPE m_InOrderInputDirection;
	unsigned       m_inputInterfaz;
	
	//Multicast Support
	Boolean m_multicast;
	unsigned long long m_Msgmask;	
	
#ifdef PTOPAZ
	unsigned  m_PoolIndex;
#endif	
	
};

//*************************************************************************

#define TPZMESSAGE_READY  TPZMessage(TPZMessage::READY)
#define TPZMESSAGE_STOP   TPZMessage(TPZMessage::STOP)

//*************************************************************************

#endif

// end of file

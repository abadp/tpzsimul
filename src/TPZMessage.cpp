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
//    File: TPZMessage.cpp
//
//    Class:  TPZMessage
//
//    Inherited from: 
// :
//*************************************************************************
//end of header

#include <TPZMessage.hpp>

#ifndef __TPZPosition_HPP__
#include <TPZPosition.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_BASE(TPZMessage);

unsigned long TPZMessage::sm_Identifier = 0;

//*************************************************************************
//:
//  f: inline MessageType type () const;
//  d:
//
//  f: inline void settype (const MessageType & type);
//  d:
//
//  f: getIdentifier inline unsigned long () const;
//  d:
//
//  f: flitNumber inline unsigned short () const;
//  d:
//
//  f: inline void setFlitNumber (unsigned order);
//  d:
//
//  f: packetNumber inline unsigned short () const;
//  d:
//
//  f: inline void setPacketNumber (unsigned order);
//  d:
//
//  f: messageSize inline unsigned short () const;
//  d:
//
//  f: inline void setMessageSize (unsigned packets);
//  d:
//
//  f: PacketSize inline unsigned short () const;
//  d:
//
//  f: inline void setPacketSize (unsigned flits)
//  d:
//
//  f: inline generationTime utime () const;
//  d:
//
//  f: inline void setGenerationTime (const utime & atime);
//  d:
//
//  f: inline packetInjectionTime utime () const;
//  d:
//
//  f: inline void setPacketInjectionTime (const utime & atime);
//  d:
//
//  f: inline TPZPosition & source () const;
//  d:
//
//  f: inline void SetSource (const TPZPosition & org);
//  d:
//
//  f: inline TPZPosition & Destiny () const;
//  d:
//
//  f: inline void setDestiny (const TPZPosition & dst);
//  d:
//
//  f: inline TPZROUTINGTYPE getRoutingPort () const;
//  d:
//
//  f: inline void setRoutingPort (TPZROUTINGTYPE port);
//  d:
//
//  f: inline isHeader Boolean () const;
//  d:
//
//  f: inline isTail Boolean () const;
//  d:
//
//  f: inline short int delta () const;
//  d:
//
//  f: inline void setDelta (short int delta);
//  d:
//
//  f: getChannel inline unsigned short () const;
//  d:
//
//  f: inline void setChannel (unsigned channel);
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: TPZMessage ();
//
//  d:
//:
//*************************************************************************

TPZMessage::TPZMessage() :
    m_Type(INFO),
            m_Identifier(0),
            m_FlitNumber(1),
            m_PacketNumber(1),
            m_MessageSize(0),
            m_PacketSize(0),
            m_GenerationTime((uTIME)0.0),
            m_InjectionTime((uTIME)0.0),
            m_DelayMultiport((uTIME)0.0),
            m_RoutingPort(_Unknow_), m_Channel(1), m_HopCount(0), m_PortList(0),
            m_ExternalInfo(0), m_distance(0), m_lastMissRouted(false),
            m_vnet(1), m_nonAdap(false), m_onScape(false), m_timesMiss(0),
            m_multicast(false), m_Msgmask(0),
            m_multiportNumber(0) {
 
#ifndef NO_TRAZA 
    m_Type=INFO;
    m_Identifier=0;
    m_FlitNumber=1;
    m_PacketNumber=1;
    m_MessageSize=0;
    m_PacketSize=0;
    m_GenerationTime=0;
    m_InjectionTime=0;
    m_DelayMultiport=0;
    m_SourcePosition=TPZPosition(0, 0, 0);
    m_DestinyPosition=TPZPosition(0, 0, 0);
    m_RoutingPort=_Unknow_;
    m_Channel=1;
    m_PortList=0;
    m_ExternalInfo=0;
    //m_distance (0),

#endif
    m_Delta[0] = m_Delta[1] = m_Delta[2] = -10;    
#ifdef PTOPAZ
    m_PoolIndex=599;
#endif    
}

//*************************************************************************
//:
//  f: TPZMessage (const TPZMessage & amessage)
//
//  d: Copy Constructor.
//:
//*************************************************************************

TPZMessage::TPZMessage(const TPZMessage& aMessage) :
    m_Type(aMessage.type() ),
            m_Identifier(aMessage.getIdentifier() ),
            m_FlitNumber(aMessage.flitNumber() ),
            m_PacketNumber(aMessage.packetNumber() ),
            m_MessageSize(aMessage.messageSize() ),
            m_PacketSize(aMessage.packetSize() ), 
            m_GenerationTime(aMessage.generationTime() ),
            m_InjectionTime(aMessage.packetInjectionTime() ),
            m_SourcePosition(aMessage.source() ),
            m_DestinyPosition(aMessage.destiny() ),
            m_RoutingPort(aMessage.getRoutingPort() ),
            m_PortList(aMessage.getPortList() ), m_Channel(aMessage.getChannel()),
            m_ExternalInfo(aMessage.getExternalInfo()),
            m_lastMissRouted(aMessage.m_lastMissRouted),
            m_HopCount(aMessage.getHopCount()), m_distance(aMessage.getDistance()),
            m_vnet(aMessage.getVnet()), m_nonAdap(aMessage.isOrdered()),
	    m_onScape(aMessage.isOnScape()), m_timesMiss(aMessage.getTimesMiss()),
            m_multicast(aMessage.isMulticast()),
            m_Msgmask(aMessage.getMsgmask()), 
            m_multiportNumber(aMessage.getMultiportNumber()) {

    m_Delta[0] = aMessage.delta(0);
    m_Delta[1] = aMessage.delta(1);
    m_Delta[2] = aMessage.delta(2);
    
}

//*************************************************************************
//:
//  f: ~ TPZMessage ();
//
//  d:
//:
//*************************************************************************

TPZMessage::~TPZMessage() {

    
}

//*************************************************************************
//:
//  f: TPZMessage & operator = (const TPZMessage & amessage)
//
//  d: Operator assignment. Both requests to the list of ports,
//     to external information, the pointers are copied and not the
//     content.
//:
//*************************************************************************

TPZMessage& TPZMessage::operator=(const TPZMessage& aMessage)
{
    m_Type = aMessage.type();
    m_Identifier = aMessage.getIdentifier();
    m_FlitNumber = aMessage.flitNumber();
    m_PacketNumber = aMessage.packetNumber();
    m_MessageSize = aMessage.messageSize();
    m_PacketSize = aMessage.packetSize();
    m_GenerationTime = aMessage.generationTime();
    m_InjectionTime = aMessage.packetInjectionTime();
    m_SourcePosition = aMessage.source();
    m_DestinyPosition = aMessage.destiny();
    m_RoutingPort = aMessage.getRoutingPort();
    m_Delta[0] = aMessage.delta(0);
    m_Delta[1] = aMessage.delta(1);
    m_Delta[2] = aMessage.delta(2);
    m_Channel = aMessage.getChannel();
    m_PortList = aMessage.getPortList();
    m_ExternalInfo = aMessage.getExternalInfo();
    m_HopCount = aMessage.m_HopCount;
    m_distance = aMessage.m_distance;
    m_lastMissRouted = aMessage.m_lastMissRouted;
    m_vnet = aMessage.getVnet();
    m_nonAdap = aMessage.isOrdered();
    m_onScape = aMessage.isOnScape();
    m_timesMiss = aMessage.getTimesMiss();
    m_multicast = aMessage.isMulticast();
    m_Msgmask = aMessage.getMsgmask();
    m_multiportNumber = aMessage.getMultiportNumber();
    return *this;
}

//*************************************************************************
//:
//  f: void destroyPortList ();
//
//  d: Delete the whole list of applications ports and reset.
//:
//*************************************************************************

void TPZMessage::destroyPortList() {
    if (m_PortList) {
        m_PortList->removeAllElements();
        delete m_PortList;
        m_PortList = 0;
    }
}

//*************************************************************************
//:
//  f: Boolean operator == (const amessage TPZMessage &) const;
//
//  d: comparison operator. It does not compare pointers port list
//     and external information.
//:
//*************************************************************************

Boolean TPZMessage::operator==(const TPZMessage& aMessage) const
{
    if( ( type() == aMessage.type() ) &&
            ( getIdentifier() == aMessage.getIdentifier() ) &&
            ( flitNumber() == aMessage.flitNumber() ) &&
            ( packetNumber() == aMessage.packetNumber() ) &&
            ( messageSize() == aMessage.messageSize() ) &&
            ( packetSize() == aMessage.packetSize() ) &&
            ( generationTime() == aMessage.generationTime() ) &&
            ( packetInjectionTime() == aMessage.packetInjectionTime() ) &&
            ( getDelayMultiport() == aMessage.getDelayMultiport() ) &&
            ( source() == aMessage.source() ) &&
            ( destiny() == aMessage.destiny() ) &&
            ( getRoutingPort() == aMessage.getRoutingPort() ) &&
            ( getChannel() == aMessage.getChannel() ) )             
    return true;

    return false;
}

//*************************************************************************
//:
//  f: static void assignUniqueId (TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

void TPZMessage::assignUniqueId(TPZMessage* aMessage) {
    TPZMessage::sm_Identifier++;
    aMessage->m_Identifier = TPZMessage::sm_Identifier;
}

//*************************************************************************
//:
//  f: virtual TPZString AsString () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZMessage::asString() const {
    TPZString rs = "Message: flit=";
    rs += TPZString(flitNumber()) + "/" + TPZString(packetSize()) + " " 
       + " ID=" + TPZString(getIdentifier()) 
       + " ptr=" + TPZString((long)this) 
       + " src=" + source().asString() 
       + " dst=" + destiny().asString() 
       + " Delta=" + TPZString(delta(0)) + "/" + TPZString(delta(1)) + "/" + TPZString(delta(2)) 
       + " Vnet=" + TPZString(getVnet());
    if (isOrdered())
    {
       rs+=TPZString(" IsOrdered ")+ "/";
    }
    if (isOnScape())
    {
       rs+=TPZString(" OnScape ")+ "/";
    }
    if (m_multicast) {
        rs+=TPZString(" IsMulticast ")+ "/";
        rs+=TPZString(" Mask=")+ TPZString(m_Msgmask) + "/";
        
    }
    //rs += TPZString("ExternalInfo=") + (long long unsigned)(m_ExternalInfo) + "/";
    return rs;
}

//************************************************************************


// end of file

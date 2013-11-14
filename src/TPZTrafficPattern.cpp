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
//    File: TPZTrafficPattern.cpp
//
//    Class:  TPZTrafficPattern
//
//    Inherited from: 
// :
//*************************************************************************
//end of header
#include <TPZTrafficPattern.hpp>

#ifndef __TPZTag_HPP__
#include <TPZTag.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

#ifndef __TPZConst_H__
#include <TPZConst.hpp>
#endif

#include <math.h>

//*************************************************************************

IMPLEMENT_RTTI_BASE(TPZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(TPZTrafficPatternEmpty, TPZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(TPZTrafficPatternRandom, TPZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(TPZTrafficPatternPermutation, TPZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(TPZTrafficPatternPerfectShuffle, TPZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(TPZTrafficPatternBitReversal, TPZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(TPZTrafficPatternLocal, TPZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(TPZTrafficPatternHotSpot, TPZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(TPZTrafficPatternFile, TPZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(TPZTrafficPatternTornado, TPZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(TPZTrafficPatternRandom3D, TPZTrafficPatternRandom);
IMPLEMENT_RTTI_DERIVED(TPZTrafficPatternPermutation3D, TPZTrafficPatternPermutation);
IMPLEMENT_RTTI_DERIVED(TPZTrafficPatternPerfectShuffle3D, TPZTrafficPatternPerfectShuffle);
IMPLEMENT_RTTI_DERIVED(TPZTrafficPatternBitReversal3D, TPZTrafficPatternBitReversal);
IMPLEMENT_RTTI_DERIVED(TPZTrafficPatternTornado3D, TPZTrafficPatternTornado);
IMPLEMENT_RTTI_DERIVED(TPZTrafficPatternLocal3D, TPZTrafficPatternLocal);

//*************************************************************************
//:
//  f: virtual TPZString AsString () const = 0;
//  d:
//
//  f: inline void setParameter1 (double param);
//  d:
//
//  f: inline void setParameter2 (double param);
//  d:
//
//  f: inline void setParameter3 (double param);
//  d:
//
//  f: inline double getParameter1 () const;
//  d:
//
//  f: inline double getParameter2 () const;
//  d:
//
//  f: inline double getParameter3 () const;
//  d:
//
//  f: inline TPZSimulation & getSimulation () const;
//  d:
//
//  f: inline void setProbBimodal (double prob);
//  d:
//
//  f: inline void setMessagesBimodal (unsigned nMsg)
//  d:
//
//  f: inline void setBimodal ();
//  d:
//
//  f: inline isBimodal Boolean () const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: TPZTrafficPattern (TPZSimulation & simulation)
//
//  d:
//:
//*************************************************************************

TPZTrafficPattern::TPZTrafficPattern(TPZSimulation& simul) :
            m_Simulation(simul), m_Parameter1(0.0), m_Parameter2(0.0), m_Parameter3(0.0),
            m_ProbMulticast(0.0), m_LengthMulticast(1), m_NumMsgBimodal(0), m_ProbBimodal(0.0),
            m_Bimodal(false), m_reactive(false), m_Multicast(false), m_IsFile(false) 
{
    m_SizeX = getSimulation().getNetwork()->getSizeX();
    m_SizeY = getSimulation().getNetwork()->getSizeY();
    m_SizeZ = getSimulation().getNetwork()->getSizeZ();
    m_TotalNodes = m_SizeX*m_SizeY*m_SizeZ;
}

//*************************************************************************
//:
//  f: void generateMessages (double q);
//
//  d:
//:
//*************************************************************************

void TPZTrafficPattern::generateMessages(double q) 
{
   for (register int i=0; i<m_SizeX; i++)
      for (register int j=0; j<m_SizeY; j++)
         for (register int k=0; k<m_SizeZ; k++) 
	 {
            TPZPosition pos(i, j, k);
            if (getSimulation().getisRafagaMode()) 
	    {
               if (drand48() < getInjectFactor(pos)*q && getSimulation().getStopSimulMessages()!=0) 
               {
                  injectMessage(pos);
                  getSimulation().decrStopSimulMessages();
               }
            } 
	    else 
	    {
               if (drand48() < getInjectFactor(pos)*q) 
               {
                  injectMessage(pos);
                  getSimulation().decrStopSimulMessages();
               }
            }
         }
}
//*************************************************************************
//:
//  f: void generateMessages (double q);
//
//  d:
//:
//*************************************************************************

void TPZTrafficPattern::generateMessagesXsegment(double q, int from, int to) 
{
   for (register int i=from; i<=to; i++)
        // for (register int k=0; k<m_SizeZ; k++) 
	 {
            int Z=i/(m_SizeX*m_SizeY);
            int Y=(i-Z*m_SizeX*m_SizeY)/m_SizeX;
            int X=(i-Z*m_SizeX*m_SizeY)%m_SizeX;
            TPZPosition pos(X, Y, Z);

            if (getSimulation().getisRafagaMode()) 
	    {
               if (drand48() < getInjectFactor(pos)*q && getSimulation().getStopSimulMessages()!=0) 
               {
                  injectMessage(pos);
                  getSimulation().decrStopSimulMessages();
               }
            } 
	    else 
	    {
               if (drand48() < getInjectFactor(pos)*q) 
               {
                  injectMessage(pos);
                  getSimulation().decrStopSimulMessages();
               }
            }
         }
}
//*************************************************************************
//:
//  f: TPZMessage generateBasicMessage (const TPZPosition & source);
//
//  d:
//:
//*************************************************************************

TPZMessage* TPZTrafficPattern::generateBasicMessage(const TPZPosition& source) 
{
   TPZMessage* message=new TPZMessage;
   message->setMessageSize(getSimulation().getMessageLength() );
   message->setPacketSize(getSimulation().getPacketLength() );
   int VirtualNet = (rand()%m_NumMessTypes)+1;
   message->setVnet(VirtualNet);
   message->setSource(source);
   message->setGenerationTime(getSimulation().getCurrentTime());
   if (isBimodal())
   {
      if (drand48()<=m_ProbBimodal)
      {
           message->setPacketSize(m_NumMsgBimodal);
      }
   } 
   else if (isReactive()) 
   {
      message->setVnet(1);
   }
   else if (isMulticast())
   {
      if (drand48()<m_ProbMulticast)
      {
         unsigned srcX= source.valueForCoordinate(TPZPosition::X);
	 unsigned srcY= source.valueForCoordinate(TPZPosition::Y);
	 unsigned srcZ= source.valueForCoordinate(TPZPosition::Z);
	 unsigned sizex=getSimulation().getNetwork()->getSizeX();
         unsigned sizey=getSimulation().getNetwork()->getSizeY();
	 unsigned sizez=getSimulation().getNetwork()->getSizeZ();
         unsigned Numnodes= sizex*sizey*sizez;
         unsigned absPos= srcZ*sizex*sizey+srcY*sizex+srcX;
	 unsigned long long masksrc=1;
	 masksrc= masksrc << absPos;
	 message->setMulticast();
         unsigned long long mask=0;
          
         //Random multicast destination
         unsigned counter=m_LengthMulticast;
	 do
	 {
	    unsigned candidate=(rand())%Numnodes;
	    unsigned long long maskaux=1;
            maskaux= maskaux << candidate;
	    if ( (maskaux & mask)==0 && (maskaux & masksrc)==0 ) 
	    {
	       //This destination was not previously selected and is different to src
	       mask=mask|maskaux;
	       counter--;		
	    }
	 }
	 while (counter!=0);
	  
         message->setMsgmask(mask);
         message->setPacketSize(1);
       }
   }
   return message;
}

//*************************************************************************
//:
//  f: static TPZTrafficPattern * createTrafficPattern (const TPZTag & tag,
//                                                     TPZsimulation & simultaneous)
//
//  d:
//:
//*************************************************************************

TPZTrafficPattern* TPZTrafficPattern::createTrafficPattern(const TPZTag& tag, TPZSimulation& simul) 
{
   TPZString type;

   if ( !tag.getAttributeValueWithName(TPZ_TAG_TYPE, type) ) 
   {
      TPZString err;
      err.sprintf(ERR_TPZTRAFI_001, (char*)TPZ_TAG_TYPE );
      EXIT_PROGRAM(err);
   }

   TPZTrafficPattern* pattern = 0;
   //problematic when dealing with a single router
   Boolean red3D = (simul.getNetwork()->getSizeZ()>1) ? true : false;

   if (type == TPZ_TAG_EMPTY) pattern = new TPZTrafficPatternEmpty(simul);
   else if (type == TPZ_TAG_RANDOM) pattern = (red3D) ? new TPZTrafficPatternRandom3D(simul) : new TPZTrafficPatternRandom(simul);
   else if (type == TPZ_TAG_PERMUTATION) pattern = (red3D) ? new TPZTrafficPatternPermutation3D(simul) : new TPZTrafficPatternPermutation(simul);
   else if (type == TPZ_TAG_PSUFFLE) pattern = (red3D) ? new TPZTrafficPatternPerfectShuffle3D(simul) : new TPZTrafficPatternPerfectShuffle(simul);
   else if (type == TPZ_TAG_BREVERSAL) pattern = (red3D) ? new TPZTrafficPatternBitReversal3D(simul) : new TPZTrafficPatternBitReversal(simul);
   else if (type == TPZ_TAG_TORNADO) pattern = (red3D) ? new TPZTrafficPatternTornado3D(simul) : new TPZTrafficPatternTornado(simul);
       
   else if (type == TPZ_TAG_LOCALTRAFFIC) 
   {
      pattern = (red3D) ? new TPZTrafficPatternLocal3D(simul) : new TPZTrafficPatternLocal(simul);
      TPZString sigma;
      if (tag.getAttributeValueWithName(TPZ_TAG_SIGMA, sigma) ) 
      {
         pattern -> setParameter1(sigma.asDouble());
      } 
      else if (tag.getAttributeValueWithName(TPZ_TAG_PARAM1, sigma) ) 
      {
         pattern -> setParameter1(sigma.asDouble());
      } 
      else 
      {
         TPZString err;
         err.sprintf(ERR_TPZTRAFI_001, (char*)TPZ_TAG_SIGMA );
         EXIT_PROGRAM(err);
      }
   }
   else if (type == TPZ_TAG_HOTSPOT) 
   {
      pattern = new TPZTrafficPatternHotSpot(simul);
      TPZString nodosHot, probHot, distHot;

      if (tag.getAttributeValueWithName(TPZ_TAG_NODOSHOT, nodosHot) ) 
      {
         pattern -> setParameter1(nodosHot.asInteger());
      } 
      else if (tag.getAttributeValueWithName(TPZ_TAG_PARAM1, nodosHot) ) 
      {
         pattern -> setParameter1(nodosHot.asInteger());
      } 
      else 
      {
         TPZString err;
         err.sprintf(ERR_TPZTRAFI_001, (char*)TPZ_TAG_NODOSHOT );
         EXIT_PROGRAM(err);
      }

      if (tag.getAttributeValueWithName(TPZ_TAG_PROBHOT, probHot) ) 
      {
         pattern -> setParameter2(probHot.asInteger());
      } 
      else if (tag.getAttributeValueWithName(TPZ_TAG_PARAM2, probHot) ) 
      {
         pattern -> setParameter2(probHot.asInteger());
      } 
      else 
      {
         TPZString err;
         err.sprintf(ERR_TPZTRAFI_001, (char*)TPZ_TAG_PROBHOT );
         EXIT_PROGRAM(err);
      }

      if (tag.getAttributeValueWithName(TPZ_TAG_DISTHOT, distHot) ) 
      {
         pattern -> setParameter3(distHot.asInteger());
      } 
      else if (tag.getAttributeValueWithName(TPZ_TAG_PARAM3, distHot) ) 
      {
         pattern -> setParameter3(distHot.asInteger());
      } 
      else 
      {
         TPZString err;
         err.sprintf(ERR_TPZTRAFI_001, (char*)TPZ_TAG_DISTHOT );
         EXIT_PROGRAM(err);
      }
   }
    
   //Trace driven simulation
   else if (type == TPZ_TAG_TRACE) 
   {
      TPZString traceFile;
      if (!tag.getAttributeValueWithName(TPZ_TAG_TRACE_FILE, traceFile))
      {
         TPZString err;
         err.sprintf("Trace File name must be provided\n");
         EXIT_PROGRAM(err);
      }
      pattern = new TPZTrafficPatternFile(traceFile, simul);
      pattern->setFromFile();
      
   }

   else 
   {
      TPZString err;
      err.sprintf(ERR_TPZTRAFI_002, (char*)(type) );
      EXIT_PROGRAM(err);
   }

   if (tag.tagId() == TPZ_TAG_BIMODAL) 
   {
      pattern->setBimodal();
      TPZString prob, numMsg;
      if (tag.getAttributeValueWithName(TPZ_TAG_PROBBIMODAL, prob) ) 
      {
         pattern -> setProbBimodal(prob.asDouble());
      } 
      else 
      {
         TPZString err;
         err.sprintf(ERR_TPZTRAFI_001, (char*)TPZ_TAG_PROBBIMODAL );
         EXIT_PROGRAM(err);
      }
      if (tag.getAttributeValueWithName(TPZ_TAG_MSGBIMODAL, numMsg) ) 
      {
         pattern -> setMessagesBimodal(numMsg.asInteger());
      } 
      else 
      {
         TPZString err;
         err.sprintf(ERR_TPZTRAFI_001, (char*)TPZ_TAG_MSGBIMODAL );
         EXIT_PROGRAM(err);
      }
      
      TPZString types;
      if (tag.getAttributeValueWithName(TPZ_TAG_NUM_MESS_TYPES, types)) 
      {
         pattern -> setNumMessTypes(types.asInteger());
         simul.setProtocolMessTypes(types.asInteger() );
      }
      else 
      {
         TPZString err;
         err.sprintf(ERR_TPZTRAFI_001, (char*)TPZ_TAG_NUM_MESS_TYPES );
         EXIT_PROGRAM(err);
      }
   } 
   else if (tag.tagId() == TPZ_TAG_REACTIVE) 
   {
      pattern->setReactive();
      TPZString types;
      if (tag.getAttributeValueWithName(TPZ_TAG_NUM_MESS_TYPES, types)) 
      {
         pattern -> setNumMessTypes(types.asInteger());
         simul.setProtocolMessTypes(types.asInteger() );
      } 
      else 
      {
         TPZString err;
         err.sprintf(ERR_TPZTRAFI_001, (char*)TPZ_TAG_NUM_MESS_TYPES );
         EXIT_PROGRAM(err);
      }
   } 
   else if (tag.tagId()== TPZ_TAG_MULTICAST) 
   {
      pattern->setMulticast();
      TPZString types, probmcast, lengmcast;
      if (tag.getAttributeValueWithName(TPZ_TAG_NUM_MESS_TYPES, types)) 
      {
         pattern -> setNumMessTypes(types.asInteger());
         simul.setProtocolMessTypes(types.asInteger() );
      } 
      else 
      {
         TPZString err;
         err.sprintf(ERR_TPZTRAFI_001, (char*)TPZ_TAG_NUM_MESS_TYPES );
         EXIT_PROGRAM(err);
      }
      if (tag.getAttributeValueWithName(TPZ_TAG_PROB_MULTICAST, probmcast)) 
      {
         pattern->setMulticast();
         pattern->setProbMulticast(probmcast.asDouble());
      } 
      else 
      {
         TPZString err;
         err.sprintf(ERR_TPZTRAFI_001, (char*)TPZ_TAG_PROB_MULTICAST );
         EXIT_PROGRAM(err);
      }
      if (tag.getAttributeValueWithName(TPZ_TAG_LENGTH_MULTICAST, lengmcast)) 
      {
         pattern->setMulticast();
         pattern->setLengthMulticast(lengmcast.asInteger());
      } 
      else 
      {
         TPZString err;
         err.sprintf(ERR_TPZTRAFI_001, (char*)TPZ_TAG_LENGTH_MULTICAST );
         EXIT_PROGRAM(err);
      }
   } 
   else if (tag.tagId() == TPZ_TAG_MODAL) 
   {
      TPZString types;
      if (tag.getAttributeValueWithName(TPZ_TAG_NUM_MESS_TYPES, types)) 
      {
         pattern -> setNumMessTypes(types.asInteger());
         simul.setProtocolMessTypes(types.asInteger() );
      }
   } 
   else 
   {
      TPZString err;
      err.sprintf(ERR_TPZTRAFI_003, (char*)(tag.tagId()) );
      EXIT_PROGRAM(err);
   }

   return pattern;
}

//*************************************************************************
//:
//  f: Boolean TPZTrafficPattern: injectRequestMessage (TPZPosition const & source, const TPZPosition & destiny);
//
//  d:
//:
//*************************************************************************

Boolean TPZTrafficPattern::injectProtocolMessage(const TPZPosition& source, const TPZPosition& destiny, unsigned type) 
{
   TPZMessage* msg = generateBasicMessage(source);
   msg->setDestiny(destiny);
   msg->setVnet(type);
   getSimulation().getNetwork()->sendMessage(msg);
   return true;
}

//*************************************************************************
//:
//  f: Boolean TPZTrafficPattern: injectRequestMessage (TPZPosition const & source, const TPZPosition & destiny);
//
//  d:
//:
//*************************************************************************

Boolean TPZTrafficPattern::injectProtocolRandomMessage(const TPZPosition& source, unsigned type) 
{
   TPZMessage* msg = generateBasicMessage(source);

   unsigned x, y;
   TPZPosition destiny;

   do {
       x = rand() % getX();
       y = rand() % getY();
       destiny = TPZPosition(x, y, 0);
   } while (source == destiny);
   msg->setDestiny(destiny);
   msg->setVnet(type);
   getSimulation().getNetwork()->sendMessage(msg);
   return true;
}

//*************************************************************************
//:
//  f: static TPZTrafficPattern * createTrafficPattern (const TPZString & fileName,
//                                                     TPZsimulation & simultaneous)
//
//  d:
//:
//*************************************************************************

TPZTrafficPattern* TPZTrafficPattern::createTrafficPattern(const TPZString& fileName, TPZSimulation& simul) 
{
   TPZTrafficPattern* pattern = new TPZTrafficPatternFile(fileName,simul);
   return pattern;
}

//*************************************************************************
//*************************************************************************

//*************************************************************************
//:
//  f: virtual Boolean TPZTrafficPatternEmpty: injectMessage (const TPZPosition & source);
//
//  d:
//:
//*************************************************************************

Boolean TPZTrafficPatternEmpty::injectMessage(const TPZPosition& source) 
{
    return true;
}

//*************************************************************************
//:
//  f: virtual Boolean TPZTrafficPatternRandom: injectMessage (const TPZPosition & source);
//
//  d:
//:
//*************************************************************************

Boolean TPZTrafficPatternRandom::injectMessage(const TPZPosition& source) 
{
   TPZMessage* msg = generateBasicMessage(source);

   // The destination is chosen randomly among all nodes
   // with the only condition that source! = destination.

   unsigned x, y;
   TPZPosition destiny;

   do {
      x = rand() % getX();
      y = rand() % getY();
      destiny = TPZPosition(x, y, 0);
   } while (source == destiny);
   msg->setDestiny(destiny);

   //If the network returns false means that we are trying to send
   //a packet from a node that has no injector or to a node that 
   //has no consumer.

    
   // In the rest of traffic types this filtering is not necessary
   //because source-destination pairs are fixed, and eliminating source eliminates destination
   while (!getSimulation().getNetwork()->sendMessage(msg)) {
      x = rand() % getX();
      y = rand() % getY();
      destiny = TPZPosition(x, y, 0);
      msg->setDestiny(destiny);
   };
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean TPZTrafficPatternRandom3D: injectMessage (const TPZPosition & source);
//
//  d:
//:
//*************************************************************************

Boolean TPZTrafficPatternRandom3D::injectMessage(const TPZPosition& source) 
{
   TPZMessage* msg = generateBasicMessage(source);

   // The destination is chosen randomly among all possible
   // with the only condition that source! = destination.
   unsigned x, y, z;

   TPZPosition destiny;
   do {
      x = rand() % getX();
      y = rand() % getY();
      z = rand() % getZ();
      destiny = TPZPosition(x, y, z);
   } while (source == destiny);

   msg->setDestiny(destiny);
   getSimulation().getNetwork()->sendMessage(msg);
   return true;
}

//*************************************************************************
//*************************************************************************

//*************************************************************************
//:
//  f: virtual Boolean TPZTrafficPatternPermutation: injectMessage (const TPZPosition & source);
//
//  d:
//:
//*************************************************************************

Boolean TPZTrafficPatternPermutation::injectMessage(const TPZPosition& source) 
{
   TPZMessage* msg = generateBasicMessage(source);
   unsigned x = source.valueForCoordinate(TPZPosition::X);
   unsigned y = source.valueForCoordinate(TPZPosition::Y);

   msg->setDestiny(TPZPosition(y, x, 0));

   unsigned byteSize = (unsigned)ceil( (float)(log((float)numberOfNodes())/log(2.0)));

   unsigned nodoOrigen = x + y*getX();
   unsigned nodoDestino = 0;

   unsigned kk=nodoOrigen<<((int)floor(((float)byteSize/2)));
   unsigned temp=nodoOrigen>>((int)ceil(((float)byteSize/2)));
   
   nodoDestino=kk|temp;

   unsigned destX = nodoDestino % getX();
   unsigned destY = (nodoDestino / getX())%getY();
    
   if (destX == x && destY == y) return false;
   
   msg->setDestiny(TPZPosition(destX, destY, 0));
   getSimulation().getNetwork()->sendMessage(msg);
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean TPZTrafficPatternPermutation3D: injectMessage (const TPZPosition & source);
//
//  d:
//:
//*************************************************************************

Boolean TPZTrafficPatternPermutation3D::injectMessage(const TPZPosition& source) 
{
   TPZMessage* msg = generateBasicMessage(source);
   unsigned x = source.valueForCoordinate(TPZPosition::X);
   unsigned y = source.valueForCoordinate(TPZPosition::Y);
   unsigned z = source.valueForCoordinate(TPZPosition::Z);

   if ( (x==y) && (y==z)) return false;
   msg->setDestiny(TPZPosition(z, x, y));
   getSimulation().getNetwork()->sendMessage(msg);

   return true;
}

//*************************************************************************
//*************************************************************************
//*************************************************************************
//:
//  f: virtual Boolean TPZTrafficPatternPerfectShuffle: injectMessage (const TPZPosition & source);
//
//  d:
//:
//*************************************************************************

Boolean TPZTrafficPatternPerfectShuffle::injectMessage(const TPZPosition& source) 
{
   TPZMessage* msg = generateBasicMessage(source);

   unsigned x = source.valueForCoordinate(TPZPosition::X);
   unsigned y = source.valueForCoordinate(TPZPosition::Y);

   unsigned nodoOrigen = x + y*getX();
   unsigned nodoDestino = nodoOrigen << 1;

   if (nodoDestino >= numberOfNodes() ) nodoDestino = nodoDestino - numberOfNodes() + 1;
   if (nodoDestino == nodoOrigen) return false;

   unsigned destX = nodoDestino % getX();
   unsigned destY = nodoDestino / getY();

   if (destY>getY()-1) destY=nodoDestino/getX();

   msg->setDestiny(TPZPosition(destX, destY, 0));
   getSimulation().getNetwork()->sendMessage(msg);

   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean TPZTrafficPatternPerfectShuffle3D: injectMessage (const TPZPosition & source);
//
//  d:
//:
//*************************************************************************

Boolean TPZTrafficPatternPerfectShuffle3D::injectMessage( const TPZPosition& source) 
{
   TPZMessage* msg = generateBasicMessage(source);

   unsigned x = source.valueForCoordinate(TPZPosition::X);
   unsigned y = source.valueForCoordinate(TPZPosition::Y);
   unsigned z = source.valueForCoordinate(TPZPosition::Z);

   unsigned sizePlaneXY = getX() * getY();
   unsigned nodoOrigen = x + y*getX() + z*sizePlaneXY;
   unsigned nodoDestino = nodoOrigen << 1;

   if (nodoDestino >= numberOfNodes() ) nodoDestino = nodoDestino - numberOfNodes() + 1;
   if ( (nodoOrigen==nodoDestino) || (nodoDestino>numberOfNodes())) return false;

   unsigned destZ = unsigned(nodoDestino/sizePlaneXY);
   unsigned planeXY = nodoDestino - destZ*sizePlaneXY;
   unsigned destX = planeXY % getX();
   unsigned destY = planeXY / getY();
   msg->setDestiny(TPZPosition(destX, destY, destZ));
   getSimulation().getNetwork()->sendMessage(msg);

   return true;
}

//*************************************************************************
//*************************************************************************

//*************************************************************************
//:
//  f: virtual Boolean TPZTrafficPatternBitReversal: injectMessage (const TPZPosition & source);
//
//  d:
//:
//*************************************************************************

Boolean TPZTrafficPatternBitReversal::injectMessage(const TPZPosition& source) 
{
   TPZMessage* msg = generateBasicMessage(source);

   unsigned x = source.valueForCoordinate(TPZPosition::X);
   unsigned y = source.valueForCoordinate(TPZPosition::Y);

   unsigned byteSize =   (unsigned)ceil((float)(log((float)numberOfNodes())/log(2.0)));
    
   static unsigned mask[] = { 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020,
   0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000 };

   unsigned nodoOrigen = x + y*getX();
   unsigned nodoDestino = 0;
   
   //Midimew Protect:
   if (getY()==1) nodoOrigen=x;
   for (int i=0; i<byteSize; i++) 
   {
      if (nodoOrigen & mask[i]) nodoDestino |= mask[byteSize-i-1];
   }

   unsigned destX = nodoDestino % getX();
   unsigned destY = (nodoDestino / getX())%getY();
   
   if ( (nodoOrigen==nodoDestino) || (nodoDestino>numberOfNodes())) return false;

   msg->setDestiny(TPZPosition(destX, destY, 0));
   getSimulation().getNetwork()->sendMessage(msg);

   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean TPZTrafficPatternBitReversal3D: injectMessage (const TPZPosition & source);
//
//  d:
//:
//*************************************************************************

Boolean TPZTrafficPatternBitReversal3D::injectMessage(const TPZPosition& source) 
{
   TPZMessage* msg = generateBasicMessage(source);

   unsigned x = source.valueForCoordinate(TPZPosition::X);
   unsigned y = source.valueForCoordinate(TPZPosition::Y);
   unsigned z = source.valueForCoordinate(TPZPosition::Z);
   unsigned sizePlaneXY = getX() * getY();

   unsigned byteSize = (unsigned)ceil(log((float)numberOfNodes()) / log(2.0) );
   static unsigned mask[] = { 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020,
   0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000 };

   unsigned nodoOrigen = x + y*getX() + z*sizePlaneXY;
   unsigned nodoDestino = 0;

   for (int i=0; i<byteSize; i++) 
   {
      if (nodoOrigen & mask[i]) nodoDestino |= mask[byteSize-i-1];
   }

   if ( (nodoOrigen==nodoDestino) || (nodoDestino>numberOfNodes())) return false;

   unsigned destZ = unsigned(nodoDestino/sizePlaneXY);
   unsigned planeXY = nodoDestino - destZ*sizePlaneXY;
   unsigned destX = planeXY % getX();
   unsigned destY = planeXY / getY();

   msg->setDestiny(TPZPosition(destX, destY, destZ));
   getSimulation().getNetwork()->sendMessage(msg);

   return true;
}

//*************************************************************************
//*************************************************************************

//*************************************************************************
//:
//  f: virtual Boolean TPZTrafficPatternTornado: injectMessage (const TPZPosition & source);
//
//  d: Traffic almost worst case for kn networks. Traffic generated so
//     that is only used half of the network links. Only positive
//     this case.
//:
//*************************************************************************

Boolean TPZTrafficPatternTornado::injectMessage(const TPZPosition& source) 
{
   unsigned x = source.valueForCoordinate(TPZPosition::X);
   unsigned y = source.valueForCoordinate(TPZPosition::Y);

   unsigned destX=(x+getX()/2-(getX()%2==0 ? -1 : 0))%getX();
   unsigned destY=(y+getY()/2-(getY()%2==0 ? -1 : 0))%getY();
   if (destX == x && destY ==y) return false;
   TPZMessage* msg = generateBasicMessage(source);
   msg->setDestiny(TPZPosition(destX, destY, 0));
   getSimulation().getNetwork()->sendMessage(msg);
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean TPZTrafficPatternTornado3D: injectMessage (const TPZPosition & source);
//
//  d: Traffic almost worst case for kn networks. Traffic generated so
//     that is only used half of the network links. Only positive
//     this case.
//:
//*************************************************************************

Boolean TPZTrafficPatternTornado3D::injectMessage(const TPZPosition& source) 
{
   unsigned x = source.valueForCoordinate(TPZPosition::X);
   unsigned y = source.valueForCoordinate(TPZPosition::Y);
   unsigned z = source.valueForCoordinate(TPZPosition::Z);
   unsigned destX=(x+getX()/2-(getX()%2==0 ? -1 : 0))%getX();
   unsigned destY=(y+getY()/2-(getY()%2==0 ? -1 : 0))%getY();
   unsigned destZ=(z+getZ()/2-(getZ()%2==0 ? -1 : 0))%getZ();
   
   if (destX == x && destY ==y && destZ==z) return false;
   TPZMessage* msg = generateBasicMessage(source);
   msg->setDestiny(TPZPosition(destX, destY, destZ));
   getSimulation().getNetwork()->sendMessage(msg);
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean TPZTrafficPatternLocal: injectMessage (const TPZPosition & source);
//
//  d:
//:
//*************************************************************************

Boolean TPZTrafficPatternLocal::injectMessage(const TPZPosition& source) 
{
   TPZMessage* msg = generateBasicMessage(source);

   double n1, n2, modulo2;
   unsigned distancia;
   // Here it is suposing that the network is squared
   unsigned radio = getSimulation().getRadius();

   do {
      do {
         n1 = drand48();
         n2 = drand48();
         modulo2 = n1*n1 + n2*n2;
      } while (modulo2 > 1.0);
      distancia = (unsigned)(1+abs(int(getParameter1()*n1*sqrt(-2.0*log((float)modulo2)/modulo2))));
   } while (distancia > unsigned(radio/2));

   int signoX = (drand48() < 0.5 ) ? 1 : -1;
   int signoY = (drand48() < 0.5 ) ? 1 : -1;

   unsigned distX = rand()%(1+distancia);
   unsigned distY = distancia - distX;

   unsigned orgX = source.valueForCoordinate(TPZPosition::X);
   unsigned orgY = source.valueForCoordinate(TPZPosition::Y);

   unsigned destX = (distX*signoX + orgX + radio ) % radio;
   unsigned destY = (distY*signoY + orgY + radio ) % radio;

   msg->setDestiny(TPZPosition(destX, destY, 0));
   getSimulation().getNetwork()->sendMessage(msg);
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean TPZTrafficPatternLocal3D: injectMessage (const TPZPosition & source);
//
//  d:
//:
//*************************************************************************

Boolean TPZTrafficPatternLocal3D::injectMessage(const TPZPosition& source) 
{
   TPZMessage* msg = generateBasicMessage(source);

   double n1, n2, modulo2;
   unsigned distancia;
   unsigned radio = getSimulation().getRadius();
   do {
      do {
         n1 = drand48();
         n2 = drand48();
         modulo2 = n1*n1 + n2*n2;
      } while (modulo2 > 1.0);
      distancia = 1 + abs(int(getParameter1()*n1*sqrt(-2.0*log((float)modulo2)/modulo2)));
   } while (distancia > unsigned(radio/2));

   int signoX = (drand48() < 0.5 ) ? 1 : -1;
   int signoY = (drand48() < 0.5 ) ? 1 : -1;
   int signoZ = (drand48() < 0.5 ) ? 1 : -1;

   unsigned distX = rand() % (1+distancia);
   unsigned distY = rand() % (1+distancia-distX);
   unsigned distZ = rand() % (1+distancia-distX-distY);

   unsigned orgX = source.valueForCoordinate(TPZPosition::X);
   unsigned orgY = source.valueForCoordinate(TPZPosition::Y);
   unsigned orgZ = source.valueForCoordinate(TPZPosition::Z);

   unsigned destX = (distX*signoX + orgX + radio ) % radio;
   unsigned destY = (distY*signoY + orgY + radio ) % radio;
   unsigned destZ = (distZ*signoZ + orgZ + radio ) % radio;

   msg->setDestiny(TPZPosition(destX, destY, destZ));
   getSimulation().getNetwork()->sendMessage(msg);
   return true;
}

//*************************************************************************
//*************************************************************************

//*************************************************************************
//:
//  f: virtual void TPZTrafficPatternHotSpot:: initialize ();
//
//  d:
//:
//*************************************************************************

void TPZTrafficPatternHotSpot::initialize() 
{
   if ( !m_HotSpotList) 
   {
      // Initializes the list of nodes that will be "hot spot" randomly
      // This list contains all nodes, those hot initialized to
      // "true" value. Those not hot initialized with the value "false".

      unsigned totalNodes = getX()*getY();
      if (totalNodes < getParameter1() ) 
      {
         TPZString err;
         err.sprintf(ERR_TPZTRAFI_004, (int)getParameter1() );
         EXIT_PROGRAM(err);
      }
      if (getParameter3() > getSimulation().getRadius() ) 
      {
         TPZString err;
         err.sprintf(ERR_TPZTRAFI_006, (int)getParameter3(), getSimulation().getRadius() );
         EXIT_PROGRAM(err);
      }
      if ( (getParameter3() != 0.0) && (getParameter1()>2)) 
      {
         TPZString err;
         err.sprintf(ERR_TPZTRAFI_007);
         EXIT_PROGRAM(err);
      }

      m_HotSpotList = new THotSpotList(totalNodes);
      m_HotSpotList -> initialize(false);

      if (getParameter1() == 2.0 && getParameter3() ) 
      {
         unsigned index = rand() % totalNodes;
         unsigned xo, xi = index % getX();
         unsigned yo, yi = index / getY();
         m_HotSpotList->setValueAt(index+1, true);
         do {
            index = rand() % totalNodes;
            xo = index % getX();
            yo = index / getY();
         } while (getSimulation().getNetwork()->
         distance(TPZPosition(xi, yi), TPZPosition(xo, yo)) != int(getParameter3()));

         m_HotSpotList->setValueAt(index+1, true);
      } 
      else
      {
         for (int i=0; i<int(getParameter1()); i++) 
	 {
            unsigned index;
            Boolean mark = false;
            do {
               index = (rand()%totalNodes) + 1;
               m_HotSpotList->valueAt(index, mark);
            } while (mark==true);
            m_HotSpotList->setValueAt(index, true);
         }
      }
   }
}

//*************************************************************************
//:
//  f: virtual TPZString TPZTrafficPatternHotSpot: AsString () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZTrafficPatternHotSpot::asString() const 
{
   TPZString rs = Inhereited::asString() + "Hot spot: n=" + TPZString(getParameter1()) + ", p=" + TPZString(getParameter2());
   for (int i=0; i<m_HotSpotList->numberOfElements(); i++) 
   {
      Boolean isHotSpot;
      m_HotSpotList->valueAt(i+1, isHotSpot);
      if (isHotSpot) rs += TPZString(" (") + TPZString(i%getX()) + "," + TPZString(i/getY()) + ")";
   }
   return rs;
}

//*************************************************************************
//:
//  f: virtual double TPZTrafficPatternHotSpot: getInjectFactor (const TPZPosition & pos);
//
//  d:
//:
//*************************************************************************

double TPZTrafficPatternHotSpot::getInjectFactor(const TPZPosition& pos) 
{
   if ( !m_HotSpotList) initialize();
   unsigned x = pos.valueForCoordinate(TPZPosition::X);
   unsigned y = pos.valueForCoordinate(TPZPosition::Y);
   unsigned compose = x*getX() + y + 1;
   Boolean isHotSpot;
   m_HotSpotList->valueAt(compose, isHotSpot);
   return (isHotSpot) ? getParameter2() : 1.0;
}

//*************************************************************************
//:
//  f: virtual Boolean TPZTrafficPatternHotSpot: injectMessage (const TPZPosition & source);
//
//  d:
//:
//*************************************************************************

Boolean TPZTrafficPatternHotSpot::injectMessage(const TPZPosition& source) 
{
   TPZMessage* msg = generateBasicMessage(source);

   // The destination is chosen randomly among all possible
   // with the only condition that source! = destination.
   unsigned x, y, z=0;
   TPZPosition destiny;

    do {
       x = rand() % getX();
       y = rand() % getY();
       destiny = TPZPosition(x, y, z);
   } while (source == destiny);

   msg->setDestiny(destiny);

   getSimulation().getNetwork()->sendMessage(msg);
   return true;
}

//*************************************************************************
//*************************************************************************

//*************************************************************************
//:
//  f: TPZTrafficPatternFile (const TPZString & fileName, TPZSimulation & s);
//
//  d:
//:
//*************************************************************************

TPZTrafficPatternFile::TPZTrafficPatternFile(const TPZString& fileName, TPZSimulation& s) :
    TPZTrafficPattern(s), m_FileName(fileName), m_File(0) 
{
   setFromFile(true);
   m_File = new ifstream(fileName);
   if ( !m_File->good() ) 
   {
      TPZString err;
      err.sprintf(ERR_TPZTRAFI_005, (char*)m_FileName );
      EXIT_PROGRAM(err);
   }   
}

//*************************************************************************
//:
//  f: Boolean TPZTrafficPatternFile: readNextMessage ();
//
//  d:
//:
//*************************************************************************

Boolean TPZTrafficPatternFile::readNextMessage() 
{
   //The format of trace-file could be easily addapted to a different
   //configuration by simply modifying this function. The current format
   //is:
   //GenTime Xsrc Ysrc Zsrc Xdst Ydst Zdst Size
   
   TPZString line;
   do {
      line = TPZString::lineFrom(*m_File);
      if (m_File->eof() ) return false; // EOF 
      line = TPZString::stripBlanks(line);
   } while (line == TPZString("") );

   uTIME time = line.word(1).asInteger();
   unsigned x_org = line.word(2).asInteger();
   unsigned y_org = line.word(3).asInteger();
   unsigned z_org = line.word(4).asInteger();

   unsigned x_dst = line.word(5).asInteger();
   unsigned y_dst = line.word(6).asInteger();
   unsigned z_dst = line.word(7).asInteger();

   unsigned size = line.word(8).asInteger();
   m_Message.setGenerationTime(time);
   m_Message.setSource(TPZPosition(x_org, y_org, z_org));
   m_Message.setDestiny(TPZPosition(x_dst, y_dst, z_dst));
   m_Message.setPacketSize(getSimulation().getPacketLength());
   m_Message.setMessageSize(size);

   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean TPZTrafficPatternFile: injectMessage (const TPZPosition & source);
//
//  d:
//:
//*************************************************************************

Boolean TPZTrafficPatternFile::injectMessage(const TPZPosition& source) 
{
   uTIME currentTime = getSimulation().getNetwork()->getCurrentTime();
   while (currentTime >= m_Message.generationTime() ) 
   {
      if ( !readNextMessage() ) return false;
      getSimulation().getNetwork()->sendMessage(m_Message);      
   }
   return true;
}

//*************************************************************************


// end of file

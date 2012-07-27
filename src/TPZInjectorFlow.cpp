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
//    File: TPZInjectorFlow.cpp
//
//    Class:  TPZInjectorFlow
//
//    Inherited from:  TPZFlow
// :
//*************************************************************************
//end of header


 
#include <TPZInjectorFlow.hpp>

#ifndef __TPZFicheroLog_HPP__
#include <TPZFicheroLog.hpp>
#endif

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

#ifndef __TPZInjector_HPP__
#include <TPZInjector.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

#include <math.h>

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZInjectorFlow,TPZFlow);

//*************************************************************************
//:
//  f: TPZInjectorFlow (TPZComponent & component);
//
//  d:
//:
//*************************************************************************

TPZInjectorFlow :: TPZInjectorFlow( TPZComponent& component)
                 : TPZFlow(component), 
		   m_DataToSend(false),
                   m_PacketInjectionTime(0),
                   m_MissRouting(0),
                   m_flitsSent(0)
{
   m_OutputData=0;
#ifdef PTOPAZ
   pthread_mutex_init(&m_QueueKey, NULL);
#endif
}


//*************************************************************************
//:
//  f: virtual ~ TPZInjectorFlow ();
//
//  d:
//:
//*************************************************************************

TPZInjectorFlow :: ~TPZInjectorFlow()
{
   TPZMessageQueue::Cursor cursor(m_MessageQueue);
   forCursor(cursor)
   {
      delete cursor.element();
   }

   delete m_OutputData;
#ifdef PTOPAZ
   pthread_mutex_unlock(&m_QueueKey);
   pthread_mutex_destroy(&m_QueueKey);
#endif
}


//*************************************************************************
//:
//  f: virtual Boolean inputReading ();
//
//  d:
//:
//*************************************************************************

Boolean TPZInjectorFlow :: inputReading()
{
   return true;      
}


//*************************************************************************
//:
//  f: virtual Boolean stateChange ();
//
//  d:
//:
//*************************************************************************

Boolean TPZInjectorFlow :: stateChange()
{      
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean outputWriting ();
//
//  d:
//:
//*************************************************************************

Boolean TPZInjectorFlow :: outputWriting()
{
   
   outputInterfaz() -> clearData();
   
   if( isAnyMessageToSend() && (!outputInterfaz()->isStopActive()) )
   {
      TPZMessage *msg;
      if(msg = getNextFlitToSend())
      {
#ifdef PTOPAZ      
         TPZNetwork* net = ((TPZSimulation*)(getComponent().getSimulation()))->getNetwork();
         unsigned n = net->getThreadID(pthread_self());
         msg -> setPoolIndex(n);
#endif	 
         outputInterfaz() -> sendData(msg);
         m_flitsSent++;
     if( msg->isHeader() || msg->isHeadTail() )
         {
            TPZNetwork* net = ((TPZSimulation*)(getComponent().getSimulation()))->getNetwork();
        TPZPosition source = msg->source();
        net->incrInjectedFlitsAt(source.valueForCoordinate(TPZPosition::X),source.valueForCoordinate(TPZPosition::Y),source.valueForCoordinate(TPZPosition::Z),msg->packetSize());
         }
      }

      #ifndef NO_TRAZA
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         TPZString texto = getComponent().asString() + " Flit Tx. TIME = ";
         texto += TPZString(delayTime) + " # " + msg->asString();
         if( msg->isHeader() || msg->isHeadTail() )
         {
            texto += TPZString("  CABECERA: ") + 
                     msg->source().asString() +
                     " => " + msg->destiny().asString();
            TPZNetwork* net = ((TPZSimulation*)(getComponent().getSimulation()))->getNetwork();
        TPZPosition source = msg->source();
        net->incrInjectedFlitsAt(source.valueForCoordinate(TPZPosition::X),source.valueForCoordinate(TPZPosition::Y),source.valueForCoordinate(TPZPosition::Z),msg->packetSize());
         }
         TPZWRITE2LOG( texto );
      #endif
      
   }
   
   return true;
}


//*************************************************************************
//:
//  f: void sendMessage (const TPZMessage & amessage)
//
//  d:
//:
//*************************************************************************

void TPZInjectorFlow :: sendMessage(TPZMessage* msg)
{

   TPZNetwork* net = ((TPZSimulation*)(getComponent().getSimulation()))->getNetwork();
   unsigned radioX = net->getSizeX();
   unsigned radioY = net->getSizeY();
   unsigned radioZ = net->getSizeZ();

   //Discarded traffic if -D option activated to avoid memory overload after saturation
   if ( msg->getExternalInfo()!=0 || !(((TPZSimulation*)(getComponent().getSimulation()))->isDiscardTraffic() && m_MessageQueue.numberOfElements() > 10) )
   {
//
//
//  WARNING: Aparently this mutex is not required since each thread inject his own traffic (but with external simulators
//  it is required.
//
#ifdef PTOPAZ
     if (msg->getExternalInfo()!=0) pthread_mutex_lock(&m_QueueKey); 
#endif

     m_MessageQueue.enqueue(msg); 
#ifndef NO_TRAZA
        uTIME delayTime = getOwnerRouter().getCurrentTime() ;
        TPZString texto = getComponent().asString() + " ENQUEUE. TIME = ";
            texto += TPZString(delayTime) + " # " + msg->asString();
            TPZWRITE2LOG( texto );
#endif	
#ifdef PTOPAZ
     if (msg->getExternalInfo()!=0) pthread_mutex_unlock(&m_QueueKey); 
#endif

   }
   else 
   {
     delete msg;
   }
}

//*************************************************************************
//:
//  f: Boolean isAnyMessageToSend () const;
//
//  d:
//:
//*************************************************************************

Boolean TPZInjectorFlow :: isAnyMessageToSend() const
{
   return (m_MessageQueue.numberOfElements() > 0) ? true : false;
}


//*************************************************************************
//:
//  f: getMessagesToSend unsigned () const;
//
//  d:
//:
//*************************************************************************

unsigned TPZInjectorFlow :: getMessagesToSend() const
{
   return m_MessageQueue.numberOfElements(); 
}

//*************************************************************************
//:
//  f: getMessagesToSend unsigned () const;
//
//  d:
//:
//*************************************************************************

unsigned TPZInjectorFlow :: getSentFlits() const
{
   return m_flitsSent; 
}

//*************************************************************************
//:
//  f: getAvaliableMessages unsigned () const;
//
//  d:
//:
//*************************************************************************

unsigned TPZInjectorFlow :: getAvaliableMessages() 
{
#ifdef PTOPAZ
   TPZNetwork* net = ((TPZSimulation*)(getComponent().getSimulation()))->getNetwork();
   unsigned pool_index = net->getThreadID(pthread_self());
   getQueueKey();
   return getMessagePool(pool_index).free();
#else
   return getMessagePool().free();
#endif
}


//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm (Boolean info = false, int delta = 0);
//
//  d:
//:
//*************************************************************************

Boolean TPZInjectorFlow :: controlAlgoritm(Boolean info, int delta)
{
   // To avoid abstraction. Not used in this class.
   return true;
}

//*************************************************************************
//:
//  f: void getQueueKey ();
//
//  d:
//:
//*************************************************************************

void TPZInjectorFlow :: getQueueKey()
{
#ifdef PTOPAZ
   pthread_mutex_lock(&m_QueueKey); 
#endif
}

//*************************************************************************
//:
//  f: void releaseQueueKey ();
//
//  d:
//:
//*************************************************************************

void TPZInjectorFlow :: releaseQueueKey()
{
#ifdef PTOPAZ
   pthread_mutex_unlock(&m_QueueKey); 
#endif
}

//*************************************************************************
//:
//  f: TPZMessage getNextFlitToSend ();
//
//  d:
//:
//*************************************************************************

TPZMessage* TPZInjectorFlow :: getNextFlitToSend()
{
   TPZMessage* topMessage=0;
   TPZMessage* returnMsg=0;
   TPZNetwork* net = ((TPZSimulation*)(getComponent().getSimulation()))->getNetwork();
   unsigned radioX = net->getSizeX();
   unsigned radioY = net->getSizeY();
   unsigned radioZ = net->getSizeZ();
#ifdef PTOPAZ
   unsigned pool_index = net->getThreadID(pthread_self());
   getQueueKey();
#endif
   if( getMessageQueue().elementAt(0,topMessage) )
   {
   
      //If the message does not anticipate that there will be room for the whole flits
#ifdef PTOPAZ
#ifndef NO_POOL
      if(topMessage->flitNumber() == 1 && getMessagePool(pool_index).free() < (topMessage->getMessageLength()*topMessage->getPacketLength()))
      {
         releaseQueueKey();
         return 0;
      } 
#endif
      if((returnMsg = getMessagePool(pool_index).allocate())==0)
      {
         TPZString err;
         err.sprintf(ERR_TPZPOOL_001);
         EXIT_PROGRAM(err);
      }
#else
#ifndef NO_POOL
      if(topMessage->flitNumber() == 1 && getMessagePool().free() < (topMessage->getMessageLength()*topMessage->getPacketLength()))
      {
         return 0;
      }
#endif 
      if((returnMsg = getMessagePool().allocate())==0)
      {
         TPZString err;
         err.sprintf(ERR_TPZPOOL_001);
         EXIT_PROGRAM(err);
      }
#endif
  
      //Reset distance when
      //message back to be reused
      *returnMsg = *topMessage;
             
      if( topMessage->flitNumber() < topMessage->packetSize() )
      {
         // The way to keep track of the flits is:
         // Initially: flitNumber = 1 totalFlit= N. flitNumber isIncrements be-
         // incremented as flits are requested to be sent.
         // When both variables are equal, message is deleted from the queue
         topMessage->setFlitNumber( (topMessage->flitNumber() +1 ) );
      }
      else
      {
         // It has sent the last flit of the packet, which must be
         // check whether the package is the last message
         
         if( topMessage->packetNumber() < topMessage->messageSize() )
         {
            // Resets the count of flits to 1.
            topMessage->setPacketNumber(( topMessage->packetNumber() +1 ));
            topMessage->setFlitNumber(1);
         }
         else
         {
            getMessageQueue().dequeue(topMessage);
             if(topMessage!=0)
             delete topMessage;
         }
      }
   }
#ifdef PTOPAZ   
   releaseQueueKey();
#endif 

   if( returnMsg->flitNumber() == 1 )  
   {   
      int xdelta,ydelta,zdelta;
      net->routingRecord(returnMsg->source(),returnMsg->destiny(),xdelta, ydelta,zdelta,returnMsg->isOrdered());
      returnMsg->setDelta(xdelta,0);
      returnMsg->setDelta(ydelta,1);
      returnMsg->setDelta(zdelta,2);
      
      /// In the first flit leaving the injector 
      // this info must be updated to be copied
      // to the rest of packet flits
      
      setPacketInjectionTime( MAXIMO(getOwnerRouter().getCurrentTime(),
                                     returnMsg->generationTime()) );
      
      if ( returnMsg->packetSize()==1)
      {
         returnMsg->setType(TPZMessage::HEADTAIL);
      }
      else
      {
         returnMsg->setType(TPZMessage::HEADER); 
      }
   }
   else if( returnMsg->flitNumber() == returnMsg->packetSize() )
   {
      returnMsg->setType(TPZMessage::TAIL);
   }
   else
   {
      returnMsg->setType(TPZMessage::INFO);
   }
#ifdef PTOPAZ   
   returnMsg->setPoolIndex(pool_index);
#endif   
   returnMsg->setPacketInjectionTime(getPacketInjectionTime());
   return returnMsg;
}

//*************************************************************************

// end of file

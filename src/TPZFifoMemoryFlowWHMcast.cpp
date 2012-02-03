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
//    File: TPZFifoMemoryFlowWHMcast.cpp
//
//    Class:  TPZFifoMemoryFlowWHMcast
//
//    Inherited from:  TPZFifoMemoryFlowWH
// :
//*************************************************************************
//end of header



#include "TPZFifoMemoryFlowWHMcast.hpp"

#ifndef __TPZSimulation_HPP__
#include "TPZSimulation.hpp"
#endif

#ifndef __TPZNetwork_HPP__
#include "TPZNetwork.hpp"
#endif

#ifndef __TPZInterfaz_HPP__
#include "TPZInterfaz.hpp"
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZFifoMemoryFlowWHMcast, TPZFifoMemoryFlowWH);


//*************************************************************************
//:
//  f: TPZFifoMemoryFlowWHMcast (TPZComponent & component);
//
//  d:
//:
//*************************************************************************

TPZFifoMemoryFlowWHMcast :: TPZFifoMemoryFlowWHMcast(TPZComponent& component)
                     : TPZFifoMemoryFlowWH(component)
{   
}

//*************************************************************************
//:
//  f: virtual Boolean outputWriting ();
//
//  d:
//:
//*************************************************************************

Boolean TPZFifoMemoryFlowWHMcast :: outputWriting()
{
  outputInterfaz()->clearData();
   
   if( ! outputInterfaz()->isStopActive() )
   {
      uTIME timeStamp;
      if( getEventQueue().firstElementTimeStamp(timeStamp) )
      {
         if( timeStamp <= getOwnerRouter().getCurrentTime() )
         {
            TPZEvent event;
            getEventQueue().firstElement(event);
            if(event.message()->isMulticast())
            {
               multiCastTreePropagation();
            }
            else
            {
               getEventQueue().dequeue(event);
               dispatchEvent(event);
            }
         }
      }
   }      
      
   if(m_SendStop) 
   {
        inputInterfaz()->sendStopRightNow();
      #ifndef NO_TRAZA
         uTIME time = getOwnerRouter().getCurrentTime();
         TPZString texto = getComponent().asString() + ": Full Buffer->Stop ";
         texto += TPZString(time);
         TPZWRITE2LOG(texto);
      #endif
   }
   else
   {
      inputInterfaz()->clearStop();   
   }
   return true;   
}
//*************************************************************************
//:
//  f: void broadcastMessageOnHead ();
//
//  d: Helper function.
//:
//*************************************************************************


void TPZFifoMemoryFlowWHMcast :: multiCastTreePropagation()
{
   TPZEvent event;
   getEventQueue().firstElement(event);
   TPZMessage *msg=event.message();
   TPZNetwork* net = ((TPZSimulation*)(getComponent().getSimulation()))->getNetwork();

   if(!msg->isHeadTail())
   {
      TPZString err=getComponent().asString() + "This class has not support for Multi-flit and Multicast simultaneously";
      EXIT_PROGRAM(err);
   }
   
   unsigned long long maskXplus=getOwnerRouter().getMask(_Xplus_);
   unsigned long long maskXminus=getOwnerRouter().getMask(_Xminus_);
   unsigned long long maskYplus=getOwnerRouter().getMask(_Yplus_);
   unsigned long long maskYminus=getOwnerRouter().getMask(_Yminus_);
   unsigned long long maskZplus=getOwnerRouter().getMask(_Zplus_);
   unsigned long long maskZminus=getOwnerRouter().getMask(_Zminus_);
   unsigned long long maskLocalNode=getOwnerRouter().getMask(_LocalNode_);
   unsigned long long msgMask=msg->getMsgmask();
   
   if ((msgMask & maskLocalNode)!=0) msg->setRoutingPort(_LocalNode_); 
   else if ((msgMask & maskXplus)!=0) msg->setRoutingPort(_Xplus_);
   else if ((msgMask & maskXminus)!=0) msg->setRoutingPort(_Xminus_);
   else if ((msgMask & maskYplus)!=0) msg->setRoutingPort(_Yplus_);
   else if ((msgMask & maskYminus)!=0) msg->setRoutingPort(_Yminus_);
   else if ((msgMask & maskZplus)!=0) msg->setRoutingPort(_Zplus_);
   else if ((msgMask & maskZminus)!=0) msg->setRoutingPort(_Zminus_);
   else
   {
      TPZString err = getComponent().asString() +  " Detected a ZERO-value mask"+ " TIME: " + TPZString(getOwnerRouter().getCurrentTime());
      EXIT_PROGRAM (err);
   }
   
         
   if ( (msgMask & (~getOwnerRouter().getMask(msg->getRoutingPort())) )== 0)
   {
      getEventQueue().dequeue(event);
      msg->setMsgmask(msgMask & getOwnerRouter().getMask(msg->getRoutingPort()) );
      event.setMessage(msg);
      dispatchEvent(event);
#ifndef NO_TRAZA
      TPZString texto = getComponent().asString() + ": FINISHED BCAST: ";
      texto += msg->asString();
      TPZWRITE2LOG(texto);
#endif
      return;
   }
   else
   {
      TPZMessage *copyMsg;
      if((copyMsg = getMessagePool().allocate())==0)
      {
          TPZString err;
          err.sprintf(ERR_TPZPOOL_001);
          EXIT_PROGRAM(err);    
      }
      //copy of the message content
      *copyMsg=*msg;
#ifdef PTOPAZ	
      unsigned pool_index=net->getThreadID(pthread_self());
      copyMsg->setPoolIndex(pool_index);
#endif 
      copyMsg->setMulticast();
      copyMsg->setMsgmask(msgMask & getOwnerRouter().getMask(msg->getRoutingPort()) );
      msg->setMsgmask( msgMask & (~getOwnerRouter().getMask(msg->getRoutingPort())) );
      // Statistics functions 
      net->incrementTx(TPZNetwork::Message);
      net->incrementTx(TPZNetwork::Packet,copyMsg->messageSize());
      net->incrementTx(TPZNetwork::Flit,copyMsg->messageSize()*copyMsg->packetSize());
      
      TPZEvent copy=event;
      copy.setMessage(copyMsg);
      dispatchEvent(copy);
 #ifndef NO_TRAZA
      TPZString texto2 = getComponent().asString() + ": BCAST IN PROCESS: ";
      texto2 += copyMsg->asString();
      TPZWRITE2LOG(texto2);
#endif     
      return;
   
   }
   
}

//*************************************************************************


// end of file

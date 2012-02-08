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
//    File: TPZRoutingMuxedFlowBubbleAdap.cpp
//
//    Class:  TPZRoutingMuxedFlowBubbleAdap
//
//    Inherited from:  TPZRoutingFlow
// :
//*************************************************************************
//end of header



#include <TPZRoutingMuxedFlowBubbleAdap.hpp>

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZFicheroLog_HPP__
#include <TPZFicheroLog.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

#ifndef __TPZRoutingMuxed_HPP__
#include <TPZRoutingMuxed.hpp>
#endif

#ifndef __TPZFifoMemory_HPP__
#include <TPZFifoMemory.hpp>
#endif

#ifndef __TPZCrossbar_HPP__
#include <TPZCrossbar.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZRoutingMuxedFlowBubbleAdap,TPZRoutingMuxedFlowBubble);


//*************************************************************************
//:
//  f: TPZRoutingMuxedFlowBubbleAdap (TPZComponent & component);
//
//  d:
//:
//*************************************************************************

TPZRoutingMuxedFlowBubbleAdap :: TPZRoutingMuxedFlowBubbleAdap( TPZComponent& component)
                          : TPZRoutingMuxedFlowBubble(component)
{  
   m_inputs= ((TPZRoutingMuxed&)getComponent()).numberOfInputs();
   m_RequestState=new unsigned[m_inputs+1];
   
   for( int i=0;i<=m_inputs;i++)
   {
      m_RequestState[i] = 1;
   }
      
}

TPZRoutingMuxedFlowBubbleAdap :: ~TPZRoutingMuxedFlowBubbleAdap()
{
   delete[] m_RequestState;
}

//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent (const TPZEvent & event);
//
//  d:
//:
//*************************************************************************

Boolean TPZRoutingMuxedFlowBubbleAdap :: dispatchEvent(const TPZEvent& event)
{
   uTIME delay = getOwnerRouter().getCurrentTime();
   
   if( event.type() == _SendRequest_ )
   {

      unsigned interfaz=m_currentInterfaz;       
#ifndef NO_TRAZA
      TPZString texto1 = getComponent().asString() + TPZString(" Event= SEND-REQUEST ");
      texto1 += TPZString(" TIME:") + TPZString(delay);
      texto1 += TPZString(" iPort:") + TPZString(interfaz);
      TPZWRITE2LOG(texto1);
#endif      
      TPZMessage *msgTx;
      if (createMessageRequest(&msgTx, interfaz))
      {
         outputInterfaz()->sendDataRightNow(msgTx); //TPZ
         
#ifndef NO_TRAZA
         TPZString texto2 = getComponent().asString() + TPZString(" REQ Tx. TIME=");
         texto2 += TPZString(delay) + TPZString(" # ") + msgTx->asString();
         TPZWRITE2LOG( texto2 );
#endif
             
         TPZEvent event(_CheckResponse_, interfaz);
         getEventQueue().enqueue(event,delay+1);
      }
      else
      {
         m_RequestState[interfaz]=1;
	 unsigned vnet=m_syncPorts[interfaz]->getVnet();
         m_arbitrationQueue.enqueue(interfaz, vnet);
	 m_arbitrationQueue.dequeue(m_currentInterfaz);

#ifndef NO_TRAZA
         TPZString texto3 = getComponent().asString() + TPZString(" Bubble condition not accomplished ");
         texto3 += TPZString(" TIME:") + TPZString(delay);
         texto3 += TPZString(" Elemets at queue:") + TPZString(m_arbitrationQueue.numberOfElements());
         TPZWRITE2LOG(texto3);
#endif	 

	 TPZEvent event(_SendRequest_);
         getEventQueue().enqueue(event, delay+1);  
      }
   }
   
   else if( event.type() == _CheckResponse_ )
   {
      
#ifndef NO_TRAZA
      TPZString texto4 = getComponent().asString() + TPZString(" Event= CHECK-RESPONSE ");
      texto4 += TPZString(" TIME:") + TPZString(delay);
      TPZWRITE2LOG(texto4);
#endif
      unsigned interfaz = event.source();
            
      if( outputInterfaz() -> isStopActive() )
      {
         
	 if( m_RequestState[interfaz]==4 )
	 {
	    m_RequestState[interfaz]=1;
	    unsigned vnet=m_syncPorts[interfaz]->getVnet();
            m_arbitrationQueue.enqueue(interfaz, vnet);
	    m_arbitrationQueue.dequeue(m_currentInterfaz);
#ifndef NO_TRAZA
            TPZString texto9 = getComponent().asString() + TPZString(" Same Message. Different Channel (Adaptivity) ");
            TPZWRITE2LOG(texto9);
#endif
	 }

#ifndef NO_TRAZA
      TPZString texto5 = getComponent().asString() + TPZString(" Connection Rejected ");
      texto5 += TPZString(" TIME:") + TPZString(delay);
      texto5 += TPZString(" Elemets at queue:") + TPZString(m_arbitrationQueue.numberOfElements());
      TPZWRITE2LOG(texto5);
#endif	 

	 TPZEvent event(_SendRequest_);
         getEventQueue().enqueue(event, delay);
	 
      }
      else
      {
#ifndef NO_TRAZA
      TPZString texto6 = getComponent().asString() + TPZString(" Connection Accepted ");
      texto6 += TPZString(" TIME:") + TPZString(delay);
      TPZWRITE2LOG(texto6);
#endif
         m_RequestState[interfaz]=1;
	 TPZEvent event(_DataTransmit_, interfaz);
	 getEventQueue().enqueue(event, delay);         
      }
      
   }
   else if( event.type() == _DataTransmit_ )
   {
      unsigned interfaz = event.source();
      if(m_syncPorts[interfaz]==0)
      {
         TPZString err;
         cerr << getComponent().asString() << endl;
         err.sprintf("This sync port should not be empty");
         EXIT_PROGRAM(err);
      }
      if( m_syncPorts[interfaz]->isHeader() )
      {
         updateMessageInfo(m_syncPorts[interfaz]);
	 inputInterfaz(interfaz)->clearStop();
      }      
      outputInterfaz()->sendDataRightNow(m_syncPorts[interfaz]);
#ifndef NO_TRAZA
      TPZString texto7 = getComponent().asString() + TPZString(" Event= DATA-TRANSMIT ");
      texto7 += TPZString(" TIME:") + TPZString(delay) + TPZString(" MSG: ") + m_syncPorts[interfaz]->asString();
      texto7 += TPZString("iPort=") + TPZString(interfaz);
      TPZWRITE2LOG(texto7);
#endif      
      if( m_syncPorts[interfaz]->isTail() )
      {
         if(m_arbitrationQueue.numberOfElements()!=0)
	 {
	    m_arbitrationQueue.dequeue(m_currentInterfaz);	    
#ifndef NO_TRAZA
      TPZString texto8 = getComponent().asString() + TPZString(" Dequeue request from iPort= ") + TPZString(interfaz);
      texto8 += TPZString(" Next iPort= ") + TPZString(m_currentInterfaz);
      TPZWRITE2LOG(texto8);
#endif
 	    TPZEvent event(_SendRequest_);
            getEventQueue().enqueue(event, delay+1); 
	 }
	 else
	 {
	    m_currentInterfaz=0;
	 }
	 

      }
      m_syncPorts[interfaz]=0;
      
      
   }
   
   return true;
}

//*************************************************************************
//:
//  f: Boolean createMessageRequest (TPZMessage ** msg);
//
//  d:
//:
//*************************************************************************

Boolean TPZRoutingMuxedFlowBubbleAdap :: createMessageRequest(TPZMessage** pMsg, unsigned interfaz)
{
   //First of all, error checking
   if (m_syncPorts[interfaz]==0)
   {
      TPZString err;
      err.sprintf("zero sync");
      err.sprintf(getComponent().asString()); 
      EXIT_PROGRAM(err);
   }
   
   TPZMessage *header = m_syncPorts[interfaz];
   
   if (!header->isHeader())
   {
      TPZString err;
      cerr << "Message=" << header->asString() << endl;
      cerr << "Component=" << getComponent().asString() << endl;
      err.sprintf("This message should be a header flit");
      EXIT_PROGRAM(err);
   }
   
   //Create the request message to send to the router
   if((*pMsg = getMessagePool().allocate())==0)
   {
         TPZString err;
         err.sprintf(ERR_TPZPOOL_001);
         EXIT_PROGRAM(err);    
   }
   TPZMessage* msg=*pMsg;
   msg->setType(TPZMessage::REQ);
   msg->setDelta( header->delta(0),0);
   msg->setDelta(header->delta(1), 1);
   msg->setDelta(header->delta(2), 2);
   msg->setVnet(header->getVnet());
#ifdef PTOPAZ	
   TPZNetwork* net = ((TPZSimulation*)(getComponent().getSimulation()))->getNetwork();
   unsigned pool_index=net->getThreadID(pthread_self());
   msg->setPoolIndex(pool_index);
#endif   
   
   if ((header->deltaAbs(0)==1) && (header->deltaAbs(1)==1) &&(header->deltaAbs(2)==1) )
   {
      //Reached destination, set routing and channel
      header->setRoutingPort(_LocalNode_);
      header->setChannel(1);
      msg->setRoutingPort(_LocalNode_);
      msg->setChannel(1);
      m_RequestState[interfaz]=1;
      return true;
   }
   else
   {
      int deltaX = header->delta(0);
      int deltaY = header->delta(1);
      int deltaZ = header->delta(2);
      int vnet= header->getVnet();
      
      if (header->isOrdered())
      {
         m_RequestState[interfaz]=4;
      }
      
      //First we try throug XADAP channel
      if (m_RequestState[interfaz]==1)
      {
         if (header->deltaAbs(0)>1)
	 {
	    header->setRoutingPort((deltaX>0) ? _Xplus_ : _Xminus_);
	    header->setChannel( (vnet-1)*2+1 );
	    msg->setRoutingPort((deltaX>0) ? _Xplus_ : _Xminus_);
	    msg->setChannel(2*vnet);
	    m_RequestState[interfaz]=2;
	    return true;
	 }
	 m_RequestState[interfaz]=2;
      }
      //Second we try YADAP channels
      if (m_RequestState[interfaz]==2)
      {
         if (header->deltaAbs(1)>1)
	 {
	    header->setRoutingPort((deltaY>0) ? _Yplus_ : _Yminus_);
	    header->setChannel( (vnet-1)*2+1 );
	    msg->setRoutingPort((deltaY>0) ? _Yplus_ : _Yminus_);
	    msg->setChannel(2*vnet);
	    m_RequestState[interfaz]=3;
	    return true;
	 }
	 m_RequestState[interfaz]=3;
      }
      //Third we try ZADAP channels
      if (m_RequestState[interfaz]==3)
      {
         if (header->deltaAbs(2)>1)
	 {
	    header->setRoutingPort((deltaZ>0) ? _Zplus_ : _Zminus_);
	    header->setChannel( (vnet-1)*2+1 );
	    msg->setRoutingPort((deltaZ>0) ? _Zplus_ : _Zminus_);
	    msg->setChannel(2*vnet);
	    m_RequestState[interfaz]=4;
	    return true;
	 }
	 m_RequestState[interfaz]=4;
      }
      
      //Finally we try through the DOR channels
      if (m_RequestState[interfaz]==4)
      {
         if (header->deltaAbs(0)>1)
	 {
	    header->setRoutingPort((deltaX>0) ? _Xplus_ : _Xminus_);
	    header->setChannel(2*vnet);
	    msg->setRoutingPort((deltaX>0) ? _Xplus_ : _Xminus_);
	    msg->setChannel((vnet-1)*2+1);	    
	 }
         else if (header->deltaAbs(1)>1)
	 {
	    header->setRoutingPort((deltaY>0) ? _Yplus_ : _Yminus_);
	    header->setChannel(2*vnet);
	    msg->setRoutingPort((deltaY>0) ? _Yplus_ : _Yminus_);
	    msg->setChannel((vnet-1)*2+1);	    
	 }
	 else if (header->deltaAbs(2)>1)
	 {
	    header->setRoutingPort((deltaZ>0) ? _Zplus_ : _Zminus_);
	    header->setChannel(2*vnet);
	    msg->setRoutingPort((deltaZ>0) ? _Zplus_ : _Zminus_);
	    msg->setChannel((vnet-1)*2+1);	    
	 }
	 else
	 {
	    TPZString err;
            cerr << "Message=" << header->asString() << endl;
            cerr << "Component=" << getComponent().asString() << endl;
            err.sprintf("This message should not request LocalNode");
            EXIT_PROGRAM(err);
	 }
	 
	 if(!controlAlgoritm()) 
	 {
	    getMessagePool().release(msg);
	    return false;
	 }
	 return true;
      }
   } 
}

//*************************************************************************


// end of file

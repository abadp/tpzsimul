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
//    File: TPZRoutingMuxedFlowBubble.cpp
//
//    Class:  TPZRoutingMuxedFlowBubble
//
//    Inherited from:  TPZRoutingMuxedFlow
// :
//*************************************************************************
//end of header



#include <TPZRoutingMuxedFlowBubble.hpp>

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZFicheroLog_HPP__
#include <TPZFicheroLog.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

#ifndef __TPZNetwork_HPP__
#include <TPZNetwork.hpp>
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

IMPLEMENT_RTTI_DERIVED(TPZRoutingMuxedFlowBubble,TPZRoutingMuxedFlow);


//*************************************************************************
//:
//  f: TPZRoutingMuxedBubbleFlow (TPZComponent & component);
//
//  d:
//:
//*************************************************************************

TPZRoutingMuxedFlowBubble :: TPZRoutingMuxedFlowBubble( TPZComponent& component)
                          : TPZRoutingMuxedFlow(component),
			    m_BufferArray(0),
			    m_numDimensions(4)
                
{  
         
}
//*************************************************************************
//:
//  f: void postInitialize ();
//
//  d:
//:
//*************************************************************************
void TPZRoutingMuxedFlowBubble :: postInitialize()
{  
   //This is the only solution to check if network is 3D at this point, 
   //the network is not accesible at this point to check sizeZ
   TPZString name3D= TPZString("BUF611");
   TPZComponent* compo3D = getOwnerRouter().componentWithName(name3D);
   if (compo3D!=0) m_numDimensions=6;
   
   //How many virtual networks Have we here?
   unsigned maxVirtualNets= 1;
   while(1)
   {
      TPZString Name=TPZString("BUF11")+TPZString(maxVirtualNets);
      TPZComponent* compoBuffer = getOwnerRouter().componentWithName(Name);  
      if(!compoBuffer) break;
      maxVirtualNets++;
   }
   maxVirtualNets--;
   m_BufferArray=new TPZFifoMemory*[m_numDimensions*maxVirtualNets];
   
   for(int dimension = 1; dimension <= m_numDimensions ; dimension++ )
   {
      //BUF channel nomenclature <dim><class><vnet> (1-4,1-2,1-maxVirtualNets)
      for(int vNet = 1; vNet <= maxVirtualNets ; vNet++ )
      {
         TPZString Name= TPZString("BUF") + TPZString(dimension)+TPZString("1")+TPZString(vNet);
         TPZComponent* compoBuffer = getOwnerRouter().componentWithName(Name); 
         if( compoBuffer==0 )
         {
            EXIT_PROGRAM("Not possible to access input buffers pointers. Check format BUF<dim><class><vn>");
         }
         m_BufferArray[dimension-(m_numDimensions+1)+vNet*m_numDimensions]=POINTERCAST(TPZFifoMemory,compoBuffer);
         if( !m_BufferArray[dimension-(m_numDimensions+1)+vNet*m_numDimensions])
         {
            TPZString err;
            err.sprintf( ERR_TPZRAFLO_002, (char*)getComponent().asString() );
            EXIT_PROGRAM(err);  
         }
     }
  }
   
   return;

}
//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent (const TPZEvent & event);
//
//  d:
//:
//*************************************************************************

Boolean TPZRoutingMuxedFlowBubble :: dispatchEvent(const TPZEvent& event)
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
         unsigned vnet=m_syncPorts[interfaz]->getVnet();
         m_arbitrationQueue.enqueue(interfaz, vnet);
	 m_arbitrationQueue.dequeue(m_currentInterfaz);

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

Boolean TPZRoutingMuxedFlowBubble :: createMessageRequest(TPZMessage** pMsg, unsigned interfaz)
{
   //First we perform message routing
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
   
   int deltaX = header->delta(0);
   int deltaY = header->delta(1);
   int deltaZ = header->delta(2);
   
   if( deltaX > 1 ) header->setRoutingPort(_Xplus_);
   else if( deltaX < -1 ) header->setRoutingPort(_Xminus_);
   else if( deltaY > +1 ) header->setRoutingPort(_Yplus_); 
   else if( deltaY < -1 ) header->setRoutingPort(_Yminus_);
   else if( deltaZ > +1 ) header->setRoutingPort(_Zplus_);
   else if( deltaZ < -1 ) header->setRoutingPort(_Zminus_);
   else
   {
      header->setRoutingPort(_LocalNode_);
      header->setChannel(1);
   }
   
   //If Bubble condition is not accomplished,
   //we do not generate the request
   if(!controlAlgoritm()) return false;
      
   //Now we generate the request
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
   msg->setChannel(header->getVnet());
   msg->setRoutingPort(header->getRoutingPort());
#ifdef PTOPAZ	
   TPZNetwork* net = ((TPZSimulation*)(getComponent().getSimulation()))->getNetwork();
   unsigned pool_index=net->getThreadID(pthread_self());
   msg->setPoolIndex(pool_index);
#endif   
   if (header->getRoutingPort()==_LocalNode_) msg->setChannel(1);
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm (Boolean salidaX, int delta);
//
//  d: 
//:
//*************************************************************************

Boolean TPZRoutingMuxedFlowBubble :: controlAlgoritm()
{
   if (m_syncPorts[m_currentInterfaz]==0)
   {
      TPZString err;
      err.sprintf("zero sync");
      err.sprintf(getComponent().asString()); 
      EXIT_PROGRAM(err);
   }
   
   
   unsigned  fifoID;
   unsigned  vNet=m_syncPorts[m_currentInterfaz]->getVnet();
            
   TPZFifoMemory* fifo = 0;
   TPZROUTINGTYPE myDirection = getComponent().getRoutingDirection();
   TPZROUTINGTYPE messDirection = m_syncPorts[m_currentInterfaz]->getRoutingPort();
   
   //In these cases I do not need to check for bubble availability
   if( (myDirection == messDirection) && (myDirection != _LocalNode_) ) return true;
   if( messDirection==_LocalNode_) return true;
   
   switch( messDirection )
   {
      case _Xplus_  : fifoID = 1; break;
      case _Xminus_ : fifoID = 2; break;
      case _Yplus_  : fifoID = 3; break;
      case _Yminus_ : fifoID = 4; break;
      case _Zplus_  : fifoID = 5; break;
      case _Zminus_ : fifoID = 6; break;         
   }
      
   return m_BufferArray[fifoID-(m_numDimensions+1)+vNet*m_numDimensions]->bubbleReady();   
   
}

//*************************************************************************


// end of file

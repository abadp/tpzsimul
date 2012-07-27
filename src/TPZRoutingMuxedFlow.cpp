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
//    File: TPZRoutingMuxedFlow.cpp
//
//    Class:  TPZRoutingMuxedFlow
//
//    Inherited from:  TPZRoutingFlow
// :
//*************************************************************************
//end of header



#include <TPZRoutingMuxedFlow.hpp>

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

IMPLEMENT_RTTI_DERIVED(TPZRoutingMuxedFlow,TPZRoutingFlow);


//*************************************************************************
//:
//  f: TPZRoutingMuxedFlow (TPZComponent & component);
//
//  d:
//:
//*************************************************************************

TPZRoutingMuxedFlow :: TPZRoutingMuxedFlow( TPZComponent& component)
                          : TPZRoutingFlow(component)
                
{  
   m_inputs= ((TPZRoutingMuxed&)getComponent()).numberOfInputs();
   m_currentInterfaz= 0;
   m_syncPorts= new TPZMessage*[m_inputs+1];
   
   for(int i=0; i<=m_inputs; i++)
       m_syncPorts[i] = 0;
      
}

//*************************************************************************
//:
//  f: ~TPZRoutingMuxedFlow (TPZComponent & component);
//
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: virtual Boolean updateMessageInfo (TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

Boolean TPZRoutingMuxedFlow :: updateMessageInfo(TPZMessage* msg)
{
   int deltaX = msg->delta(0);
   int deltaY = msg->delta(1);
   int deltaZ = msg->delta(2);
   
   if (msg->getRoutingPort()==_Xplus_) msg->setDelta(deltaX-1,0);
   else if(msg->getRoutingPort()==_Xminus_) msg->setDelta(deltaX+1,0);
   else if(msg->getRoutingPort()==_Yplus_) msg->setDelta(deltaY-1,1);         
   else if(msg->getRoutingPort()==_Yminus_)msg->setDelta(deltaY+1,1);
   else if(msg->getRoutingPort()==_Zplus_) msg->setDelta(deltaZ-1,2);         
   else if(msg->getRoutingPort()==_Zminus_)msg->setDelta(deltaZ+1,2);
   else
   {
      if (msg->getRoutingPort()!=_LocalNode_)
      cerr << "This is not a valid port value here" << endl;
   }
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZRoutingMuxedFlow :: onStopUp(unsigned interfaz, unsigned cv)
{   
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZRoutingMuxedFlow :: onStopDown(unsigned interfaz, unsigned cv)
{   
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyUp (unsigned interface, unsigned cv)
//
//  d: I adapted to multiple entries
//        I Have Doubts' referred to the state (exit I assume)
//:
//*************************************************************************

Boolean TPZRoutingMuxedFlow :: onReadyUp(unsigned interfaz, unsigned cv)
{  
   TPZMessage* message;
   inputInterfaz(interfaz)->getData(&message);    
   
   #ifndef NO_TRAZA
      uTIME delay = getOwnerRouter().getCurrentTime();
      TPZString texto = getComponent().asString() + " Flit Rx. TIME = ";
      texto += TPZString(delay) + " # " + message->asString() + " Interfaz =" + TPZString(interfaz);
      TPZWRITE2LOG( texto );
   #endif
   m_syncPorts[interfaz]=message;
   if( message->isHeader())
   {
      inputInterfaz(interfaz)->sendStopRightNow();
#ifndef NO_TRAZA
      uTIME delay = getOwnerRouter().getCurrentTime();
      TPZString texto = getComponent().asString() + " Stop Signal TIME=";
      texto += TPZString(delay) + " input=" + TPZString(interfaz);
      TPZWRITE2LOG( texto );
#endif
      if( m_currentInterfaz==0 )
      {
         m_currentInterfaz=interfaz;
	 uTIME delay = getOwnerRouter().getCurrentTime();
         TPZEvent event(_SendRequest_, interfaz);
         getEventQueue().enqueue(event,delay);
      }
      else
      {
         unsigned vnet=m_syncPorts[interfaz]->getVnet();
         m_arbitrationQueue.enqueue(interfaz, vnet);
      }
 #ifndef NO_TRAZA
      TPZString texto2 = getComponent().asString() + TPZString(" TIME=");
      texto2 += TPZString(delay) + " Elements at req queue=" + TPZString(m_arbitrationQueue.numberOfElements());
      TPZWRITE2LOG( texto2 );
#endif       
   }
   else
   {
      uTIME delay = getOwnerRouter().getCurrentTime();
      TPZEvent event(_DataTransmit_, interfaz);
      getEventQueue().enqueue(event,delay);
   }
      
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyDown (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZRoutingMuxedFlow :: onReadyDown(unsigned interfaz, unsigned cv)
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

Boolean TPZRoutingMuxedFlow :: outputWriting()
{     
   outputInterfaz()->clearData();
   
   uTIME timeStamp;
   uTIME currentTime = getOwnerRouter().getCurrentTime();
   while( getEventQueue().firstElementTimeStamp(timeStamp) )
   {
      if( timeStamp <= currentTime )
      {
         TPZEvent event;
         getEventQueue().dequeue(event);
         dispatchEvent(event);
      }
      else break;
   }

   return true;
 
}

//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent (const TPZEvent & event);
//
//  d:
//:
//*************************************************************************

Boolean TPZRoutingMuxedFlow :: dispatchEvent(const TPZEvent& event)
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
      createMessageRequest(&msgTx, interfaz);
      outputInterfaz()->sendDataRightNow(msgTx); 

#ifndef NO_TRAZA
      TPZString texto8 = getComponent().asString() + TPZString(" REQ Tx. TIME=");
      texto8 += TPZString(delay) + TPZString(" # ") + msgTx->asString();
      TPZWRITE2LOG( texto8 );
#endif

      
      TPZEvent event(_CheckResponse_, interfaz);
      getEventQueue().enqueue(event,delay+1);
   }
   
   else if( event.type() == _CheckResponse_ )
   {
#ifndef NO_TRAZA
      TPZString texto2 = getComponent().asString() + TPZString(" Event= CHECK-RESPONSE ");
      texto2 += TPZString(" TIME:") + TPZString(delay);
      TPZWRITE2LOG(texto2);
#endif
      unsigned interfaz = event.source();
            
      if( outputInterfaz() -> isStopActive() )
      {
         unsigned vnet=m_syncPorts[interfaz]->getVnet();
         m_arbitrationQueue.enqueue(interfaz, vnet);
	 m_arbitrationQueue.dequeue(m_currentInterfaz);

#ifndef NO_TRAZA
      TPZString texto3 = getComponent().asString() + TPZString(" Connection Rejected ");
      texto3 += TPZString(" TIME:") + TPZString(delay);
      texto3 += TPZString(" Elemets at queue:") + TPZString(m_arbitrationQueue.numberOfElements());
      TPZWRITE2LOG(texto3);
#endif	 

	 TPZEvent event(_SendRequest_);
         getEventQueue().enqueue(event, delay);
	 
      }
      else
      {
#ifndef NO_TRAZA
      TPZString texto4 = getComponent().asString() + TPZString(" Connection Accepted ");
      texto4 += TPZString(" TIME:") + TPZString(delay);
      TPZWRITE2LOG(texto4);
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
      TPZString texto5 = getComponent().asString() + TPZString(" Event= DATA-TRANSMIT ");
      texto5 += TPZString(" TIME:") + TPZString(delay) + TPZString(" MSG: ") + m_syncPorts[interfaz]->asString();
      texto5 += TPZString("iPort=") + TPZString(interfaz);
      TPZWRITE2LOG(texto5);
#endif      
      if( m_syncPorts[interfaz]->isTail() )
      {
         if(m_arbitrationQueue.numberOfElements()!=0)
	 {
	    m_arbitrationQueue.dequeue(m_currentInterfaz);	    
#ifndef NO_TRAZA
      TPZString texto5 = getComponent().asString() + TPZString(" Dequeue request from iPort= ") + TPZString(interfaz);
      texto5 += TPZString(" Next iPort= ") + TPZString(m_currentInterfaz);
      TPZWRITE2LOG(texto5);
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

Boolean TPZRoutingMuxedFlow :: createMessageRequest(TPZMessage** pMsg, unsigned interfaz)
{
   if((*pMsg = getMessagePool().allocate())==0)
   {
         TPZString err;
         err.sprintf(ERR_TPZPOOL_001);
         EXIT_PROGRAM(err);    
   }
   if (m_syncPorts[interfaz]==0)
   {
      TPZString err;
      err.sprintf("zero sync");
      err.sprintf(getComponent().asString()); 
      EXIT_PROGRAM(err);
   }
   TPZMessage* msg=*pMsg;
   msg->setType(TPZMessage::REQ);
   
#ifdef PTOPAZ	
   TPZNetwork* net = ((TPZSimulation*)(getComponent().getSimulation()))->getNetwork();
   unsigned pool_index=net->getThreadID(pthread_self());
   msg->setPoolIndex(pool_index);
#endif
   
   TPZMessage *header = m_syncPorts[interfaz];
   msg->setDelta( header->delta(0),0);
   msg->setDelta(header->delta(1), 1);
   msg->setDelta(header->delta(2), 2);
   msg->setVnet(header->getVnet());
   msg->setChannel(header->getVnet());
   
   int deltaX = header->delta(0);
   int deltaY = header->delta(1);
   int deltaZ = header->delta(2);
   
   if (!header->isHeader())
   {
      TPZString err;
      cerr << "Message=" << header->asString() << endl;
      cerr << "Component=" << getComponent().asString() << endl;
      err.sprintf("This message should be a header flit");
      EXIT_PROGRAM(err);
   }
   
   
   if( deltaX > 1 ) 
   {
      msg->setRoutingPort(_Xplus_);
      header->setRoutingPort(_Xplus_);
   }
   else if( deltaX < -1 ) 
   {
      msg->setRoutingPort(_Xminus_);
      header->setRoutingPort(_Xminus_);
   }
   else if( deltaY > +1 ) 
   {
      msg->setRoutingPort(_Yplus_);         
      header->setRoutingPort(_Yplus_); 
   }
   else if(deltaY < -1) 
   {
      msg->setRoutingPort(_Yminus_);
      header->setRoutingPort(_Yminus_);
   }
   else if( deltaZ > +1 ) 
   {
      msg->setRoutingPort(_Zplus_);         
      header->setRoutingPort(_Zplus_); 
   }
   else if(deltaZ < -1) 
   {
      msg->setRoutingPort(_Zminus_);
      header->setRoutingPort(_Zminus_);
   }
   else
   {
      msg->setRoutingPort(_LocalNode_);
      msg->setChannel(1);
      header->setRoutingPort(_LocalNode_);
      header->setChannel(1);
   }   
   
   return true;
}

//*************************************************************************
//:
//  f: TPZString getStatus () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZRoutingMuxedFlow :: getStatus() const
{
   TPZString rs = getComponent().asString();

   if( inputInterfaz()->isStopActive() ) rs += TPZString("\t IN stop\t");
   else rs += TPZString("\t IN clear\t");

   if( inputInterfaz()->isStopActive() ) rs += TPZString("OUT stop");
   else rs += TPZString("OUT clear");
   
   TEventQueue::Cursor cursor(getEventQueue());
   forCursor(cursor)
      rs += TPZString(". Event=") + TPZString(cursor.element().element().type());
   return rs;
}

//*************************************************************************


// end of file

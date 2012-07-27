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
//    File: TPZSimpleRouterFlow.cpp
//
//    Class:  TPZSimpleRouterFlow
//
//    Inherited from:  TPZFlow
// :
//*************************************************************************
//end of header



#include <TPZSimpleRouterFlow.hpp>

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZFicheroLog_HPP__
#include <TPZFicheroLog.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

#ifndef __TPZSimpleRouter_HPP__
#include <TPZSimpleRouter.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZSimpleRouterFlow,TPZFlow);

//*************************************************************************
//:
//  f: TPZSimpleRouterFlow ();
//
//  d:
//:
//*************************************************************************

TPZSimpleRouterFlow :: TPZSimpleRouterFlow( TPZComponent& component)
                 : TPZFlow(component)
{
}


//*************************************************************************
//:
//  f: ~ TPZSimpleRouterFlow ();
//
//  d:
//:
//*************************************************************************

TPZSimpleRouterFlow :: ~TPZSimpleRouterFlow()
{
}


//*************************************************************************
//:
//  f: virtual void initialize ();
//
//  d:
//:
//*************************************************************************

void TPZSimpleRouterFlow :: initialize()
{
   Inhereited :: initialize();
   m_vnets=((TPZSimpleRouter&)getComponent()).getVnets();
   m_ports=((TPZSimpleRouter&)getComponent()).numberOfInputs();
   unsigned ports=m_vnets*m_ports+1;
   m_connections=new unsigned[m_ports+1];
   m_routing=new TPZMessage*[ports];
   m_routingtime=new int[ports];
   m_sync=new TPZMessage*[ports];
   m_fifos=new TPZMessageQueue[ports];
   m_shortcut=new unsigned[ports];

   m_token = 1;
   for(int i=0; i<m_ports+1; i++)
   {
      m_connections[i] = 0;
   }
   for(int i=0; i<ports; i++)
   {
      m_routing[i] = 0;
      m_sync[i] = 0;
      m_routingtime[i] = 0;
      m_shortcut[i]=0;
   }
   TPZSimpleRouter& simpleRouter = (TPZSimpleRouter&)getComponent();
   m_bufferSize = simpleRouter.getBufferSize();
   setCleanInterfaces(true);
}


//*************************************************************************
//:
//  f: virtual void terminate ();
//
//  d:
//:
//*************************************************************************

void TPZSimpleRouterFlow :: terminate()
{
   delete[] m_connections;
   delete[] m_routing;
   delete[] m_routingtime;
   delete[] m_sync;
   delete[] m_fifos;
   delete[] m_shortcut;
} 


//*************************************************************************
//:
//  f: virtual Boolean inputReading ();
//
//  d:
//:
//*************************************************************************
Boolean TPZSimpleRouterFlow :: inputReading()
{  
   unsigned outPort;
   unsigned inPort;
   unsigned virtualChannel;
   cleanOutputInterfaces();
   TPZNetwork* net = ((TPZSimulation*)(getComponent().getSimulation()))->getNetwork();
   
   //**********************************************************************************************************
   // PART 4: Loop through all output ports.
   // We move the data flits for already established connections
   //**********************************************************************************************************
   for( outPort = 1; outPort <= m_ports; outPort++)
   {   
      // Find the input port assigned to outport.
      // If not assigned, go to the next.
      if( ! (inPort = m_connections[outPort] ) ) 
      {
                 continue;
      }
      
      // If no message at routing go to the next
      if( ! m_routing[inPort] )
      {
          TPZString err = getComponent().asString() +  " TIME: " + TPZString(getOwnerRouter().getCurrentTime());
          err+=" connection indicates that input: ";
          err+= TPZString(inPort)+"is connected to output: ";
          err+= TPZString(outPort);
          err+=" but the routing unit is empty (Packet fragmented at the link?)"; 
          //EXIT_PROGRAM (err);
          continue;
      }

      virtualChannel=(inPort-1)/m_ports+1;
      if( inPort % m_ports == 0  ) 
      {
         virtualChannel=m_routing[inPort]->getVnet(); 
	 if(inPort>m_ports)continue;
      }
      if(outPort == m_ports) virtualChannel=1;

      // If there is a message at routing and delay is over
      if( ! ( m_routingtime[inPort]-- ) )
      {
         TPZMessage* mess=m_routing[inPort];
	 
	 if (mess->isMulticast())
	 {
	    unsigned long long messageMask=mess->getMsgmask();
	    unsigned long long portMask=getOwnerRouter().getMask(mess->getRoutingPort());
	    //The last message, no replication is needed
	    if ( (messageMask & (~portMask))== 0)
	    {
	       mess->setMsgmask(messageMask & portMask);
	       m_routing[inPort] = 0;
#ifndef NO_TRAZA
               TPZString texto = getComponent().asString() + " NO-REPLICATION ";
	       texto += TPZString( " Leaving MSG= ") + mess->asString();
               texto += TPZString( " Leaving Mask= ") + TPZString(mess->getMsgmask());
	       texto += TPZString( " OPORT= ") + TPZString(outPort);
               TPZWRITE2LOG(texto);
#endif		       
	    }
	    //need to replicate, destinations remaining
	    else
	    {
	       if((mess = getMessagePool().allocate())==0) 
	       {
	          TPZString err;
                  err.sprintf(ERR_TPZPOOL_001);
                  EXIT_PROGRAM(err);
               }
               *mess=*m_routing[inPort];
	       //New message mask
	       mess->setMulticast();
	       mess->setMsgmask(messageMask & portMask);
#ifdef PTOPAZ	
               unsigned pool_index=net->getThreadID(pthread_self());
               mess->setPoolIndex(pool_index);
#endif 
               // Statistics functions 
               net->incrementTx(TPZNetwork::Message);
               net->incrementTx(TPZNetwork::Packet,mess->messageSize());
               net->incrementTx(TPZNetwork::Flit,mess->messageSize()*mess->packetSize());
	       m_routing[inPort]->setMsgmask(messageMask & (~portMask));
	       updateMessageInfo(m_routing[inPort]);
               m_routingtime[inPort] = getHeaderDelay();
#ifndef NO_TRAZA
               TPZString texto = getComponent().asString() + " REPLICATION ";
	       texto += TPZString( " Leaving Mask= ") + TPZString(mess->getMsgmask());
               texto += TPZString(" Remaining Mask= ") + TPZString(m_routing[inPort]->getMsgmask());
               TPZWRITE2LOG(texto);
#endif	       
	    }
	 }
	 else
	 {
	    m_routing[inPort] = 0;
	 }
	    
	 outputInterfaz(outPort)->sendData(mess,virtualChannel);
         ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::SWTraversal);
         ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::BufferRead);
         if ( ((TPZSimpleRouter&)getComponent()).getTypeForOutput(outPort) != _LocalNode_ )
         {
            getOwnerRouter().incrLinkUtilization();
            ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::LinkTraversal);
         }
#ifndef NO_TRAZA
         TPZString texto = getComponent().asString() + " SW TRAVERSAL ";
         texto +=  " From  ";
         texto += mess->asString() + " TIME: " + TPZString(getOwnerRouter().getCurrentTime());
         texto += TPZString(" inPort: ") + TPZString(inPort) + TPZString(" outPort: ") + TPZString(outPort);
         texto += TPZString(" VChannel: ") + TPZString(virtualChannel);
         TPZWRITE2LOG(texto);
#endif
         // If end of message
         if( mess->isTail() || mess->isHeadTail() )
         {
            m_connections[outPort] = 0;
#ifndef NO_TRAZA
            TPZString texto = getComponent().asString() + " CLEARING ";
            texto +=  " From  ";
            texto += mess->asString() + " TIME: " + TPZString(getOwnerRouter().getCurrentTime());
            texto += TPZString(" inPort: ") + TPZString(inPort) + TPZString(" outPort: ") + TPZString(outPort);
            texto += TPZString(" VChannel: ") + TPZString(virtualChannel);
            TPZWRITE2LOG(texto);
#endif
         }
      }
   }
   
   //******************************************************************************************************************
   // PART 3: Process the connections of the crossbar
   // the token is employed to implement round robin arbitration
   //******************************************************************************************************************
   inPort = m_ports*m_vnets;
   m_token = 0;
   int i;
   for(i = 0; i < m_ports*m_vnets ; i++, inPort = inPort - 1)
   {
      virtualChannel=(inPort-1)/m_ports+1;
     
      // If the routing is empty, go to the next
      if( ! m_routing[inPort] ) 
      {
           continue;
      }
      // If there is a body or tail flit, proceed to the next
      if( (! m_routing[inPort]->isHeader()) && (! m_routing[inPort]->isHeadTail()) )
      {
           continue;
      }  
      // Extract the output port
      outPort = extractOutputPortNumber(m_routing[inPort]); 
      
      if(inPort == m_ports) 
      {
         virtualChannel=m_routing[inPort]->getVnet(); 
	 if(inPort>m_ports)continue;
      }
      if(outPort == m_ports) virtualChannel=1;
      
      if( outputInterfaz(outPort)->isStopActive(virtualChannel) ) 
      {
         if( ! m_token ) m_token = inPort;
     continue;
      }
      
      if( ! m_connections[outPort] )
      {
         // Reserve the port
         m_connections[outPort] = inPort;
#ifndef NO_TRAZA
        TPZString texto = getComponent().asString() + " SW ARB [GRANT] ";
        texto +=  " From  ";
        texto += m_routing[inPort]->asString() + " TIME: " + TPZString(getOwnerRouter().getCurrentTime());
        texto += TPZString(" inPort: ") + TPZString(inPort) + TPZString(" outPort: ") + TPZString(outPort);
        texto += TPZString(" VChannel: ") + TPZString(virtualChannel);
        TPZWRITE2LOG(texto);
#endif
      }
      else
      {
#ifndef NO_TRAZA
           TPZString texto = getComponent().asString() + " SW ARB {REJECT] ";
           texto +=  " From  ";
           texto += m_routing[inPort]->asString() + " TIME: " + TPZString(getOwnerRouter().getCurrentTime());
           texto += TPZString(" inPort: ") + TPZString(inPort) + TPZString(" outPort: ") + TPZString(outPort);
           texto += TPZString(" VChannel: ") + TPZString(virtualChannel);
           TPZWRITE2LOG(texto);
#endif
         // If this is the first port rejected in this round
         // serve it first in the following round
         if( ! m_token )
         {
            m_token = inPort;
         }
      }
      // If not, wait for the next cycle.
   }
   // No ports have been rejected.
   if( ! m_token ) m_token = inPort;
       
   //****************************************************************************************************************
   // PART 2: Loop through all the routing
   // Filling those that are empty with data from the fifo
   //****************************************************************************************************************
   for(inPort = 1; inPort <= m_ports*m_vnets; inPort++) 
   {
      // If the routing is not empty
      if( ! m_routing[inPort] )
      {
        // Take the next message from the corresponding fifo
         if(m_fifos[inPort].numberOfElements()!=0)
         {
                m_fifos[inPort].dequeue(m_routing[inPort]);
#ifndef NO_TRAZA
                  TPZString texto = getComponent().asString() + " BUFFER-READ ";
                  texto +=  m_routing[inPort]->asString()+ " TIME: " + TPZString(getOwnerRouter().getCurrentTime());
                  TPZWRITE2LOG(texto);
#endif
               // If the message is header
               if( m_routing[inPort]->isHeader() || m_routing[inPort]->isHeadTail() )
               {
                  // Calculate the output port
                  updateMessageInfo(m_routing[inPort]);
                  m_routingtime[inPort] = getHeaderDelay();
               }
           else
           {
              m_routingtime[inPort] = 0;
           }

          }          
      }
   }
   
   //**********************************************************************************************************
   // PART 1: Loop through all sync.
   //**********************************************************************************************************
   for( inPort = 1; inPort <= m_ports*m_vnets; inPort++)
   {
      // If there is a message at syncronizer,
      if( m_sync[inPort] )
      {
#ifndef NO_TRAZA
                  TPZString texto = getComponent().asString() + " BUFFER-WRITE ";
                  texto +=  m_sync[inPort]->asString()+ " TIME: " + TPZString(getOwnerRouter().getCurrentTime());
                  TPZWRITE2LOG(texto);
#endif
            // Put it in the corresponding buffer
            m_fifos[inPort].enqueue(m_sync[inPort]);
            // and remove it from syncs
            m_sync[inPort]=0;
      }
   }  
   return true;   
}


//*************************************************************************
//:
//  f: virtual Boolean stateChange ();
//
//  d:
//:
//*************************************************************************

Boolean TPZSimpleRouterFlow :: stateChange()
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

Boolean TPZSimpleRouterFlow :: outputWriting()
{
   unsigned inPort;
   unsigned sizeForCT = ((TPZSimulation*)getComponent().getSimulation())->
                        getPacketLength((TPZNetwork*)getOwnerRouter().getOwner());
   unsigned virtualChannel; 
  

   // Through all the buffers.
   for( inPort = 1; inPort <= m_ports*m_vnets; inPort++)
   {
      // If there is no room for another packet, send stop signal.
      //Only one injection virtual channel

      virtualChannel =(inPort-1)/m_ports+1;
      if( inPort%m_ports == 0 ) {virtualChannel=1; if(inPort>m_ports)continue;} 

      if((m_bufferSize - m_fifos[inPort].numberOfElements()) <= sizeForCT)
      {
         inputInterfaz((inPort-1)%m_ports+1)->sendStopRightNow(virtualChannel);
      }
      else
      {
         inputInterfaz((inPort-1)%m_ports+1)->clearStopRightNow(virtualChannel); 
      }
   }
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean propagateStop ();
//
//  d:
//:
//*************************************************************************

Boolean TPZSimpleRouterFlow :: propagateStop()
{
    return true;
}

//*************************************************************************
//:
//  f: virtual Boolean updateMessageInfo (TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

Boolean TPZSimpleRouterFlow :: updateMessageInfo(TPZMessage* msg)
{  
   if (msg->isMulticast())
   {
      Boolean changeDir;
      if(!msg->isHeadTail())
      {
         TPZString err = getComponent().asString() +  " SimpleRouter does not support Multi-flit packets with multi-destination";
         EXIT_PROGRAM (err);
      }
      unsigned long long msg_mask = msg -> getMsgmask();
     
      unsigned long long maskXminus=getOwnerRouter().getMask(_Xminus_);
      unsigned long long maskXplus=getOwnerRouter().getMask(_Xplus_);
      unsigned long long maskYminus=getOwnerRouter().getMask(_Yminus_);
      unsigned long long maskYplus=getOwnerRouter().getMask(_Yplus_);
      unsigned long long maskZminus=getOwnerRouter().getMask(_Zminus_);
      unsigned long long maskZplus=getOwnerRouter().getMask(_Zplus_);
      unsigned long long maskLocalNode=getOwnerRouter().getMask(_LocalNode_);
 #ifndef NO_TRAZA
      TPZString texto = getComponent().asString() + " MASK-ARB ";
      texto +=  TPZString(msg_mask) + TPZString(" ") + TPZString(maskXplus) + TPZString(" ") + TPZString(maskXminus);
      texto +=  TPZString(" ") + TPZString(maskYplus) + TPZString(" ") + TPZString(maskYminus);
      texto +=  TPZString(" ") + TPZString(maskZplus) + TPZString(" ") + TPZString(maskZminus);
      TPZWRITE2LOG(texto);
#endif        
      if ((msg_mask & maskXplus)!=0) 
      {
         if (msg->getRoutingPort() == _Xplus_) 
	 {
            changeDir = false;
         }
         else
	 {
            msg->setRoutingPort(_Xplus_);
            changeDir = true;
         }	 
      }      
      else if ((msg_mask & maskXminus)!=0) 
      {
         if (msg->getRoutingPort() == _Xminus_) 
	 {
            changeDir = false;
         }
         else
	 {
            msg->setRoutingPort(_Xminus_);
            changeDir = true;
         }
      }
      else if ((msg_mask & maskYplus)!=0) 
      {
         if (msg->getRoutingPort() == _Yplus_) 
	 {
            changeDir = false;
         }
         else 
	 {
            msg->setRoutingPort(_Yplus_);
            changeDir = true;
         }
      }
      else if ((msg_mask & maskYminus)!=0)
      {
         if (msg->getRoutingPort() == _Yminus_) 
	 {
            changeDir = false;
         }
         else 
	 {
            msg->setRoutingPort(_Yminus_);
            changeDir = true;
         }
      }
      else if ((msg_mask & maskZplus)!=0) 
      {
         if (msg->getRoutingPort() == _Zplus_) 
	 {
            changeDir = false;
         }
         else 
	 {
            msg->setRoutingPort(_Zplus_);
            changeDir = true;
         }
      }
      else if ((msg_mask & maskZminus)!=0)
      {
         if (msg->getRoutingPort() == _Zminus_) 
	 {
            changeDir = false;
         }
         else 
	 {
            msg->setRoutingPort(_Zminus_);
            changeDir = true;
         }
      }
      else if ((msg_mask & maskLocalNode)!=0) 
      {
         msg->setRoutingPort(_LocalNode_);
         changeDir = false;
      }
      else
      {
         TPZString err = getComponent().asString() +  " Detected a ZERO-value mask"+ " TIME: " + TPZString(getOwnerRouter().getCurrentTime());
         EXIT_PROGRAM (err);
      }
 
      return changeDir;
        
   }
   else
   {
      int deltaX = msg->delta(0);
      int deltaY = msg->delta(1);
      int deltaZ = msg->delta(2);
      
      if( msg->isHeader() || msg->isHeadTail() )
      {  
         if( deltaX > 1 )
         {
            msg->setDelta(deltaX-1,0);
            if(msg->getRoutingPort() == _Xplus_) return false; // no change of direction
            msg->setRoutingPort(_Xplus_);         
         }
         else if( deltaX < -1 )
         {
            msg->setDelta(deltaX+1,0);
            if(msg->getRoutingPort() == _Xminus_) return false; // no change of direction
            msg->setRoutingPort(_Xminus_);
         }
         else if( deltaY > +1 )
         {
            msg->setDelta(deltaY-1,1);
            if(msg->getRoutingPort() == _Yplus_) return false; // no change of direction
            msg->setRoutingPort(_Yplus_);         
         }
         else if(deltaY < -1)
         {
            msg->setDelta(deltaY+1,1);
            if(msg->getRoutingPort() == _Yminus_) return false; // no change of direction
            msg->setRoutingPort(_Yminus_);
         }
	 else if( deltaZ > +1 )
         {
            msg->setDelta(deltaZ-1,2);
            if(msg->getRoutingPort() == _Zplus_) return false; // no change of direction
            msg->setRoutingPort(_Zplus_);         
         }
         else if(deltaZ < -1)
         {
            msg->setDelta(deltaZ+1,2);
            if(msg->getRoutingPort() == _Zminus_) return false; // no change of direction
            msg->setRoutingPort(_Zminus_);
         }
         else
         {
            msg->setRoutingPort(_LocalNode_);
            return false; // Not needed flow control.
         }
      }
   } 
   if(!((TPZSimulation*)(getComponent().getSimulation()))->getNetwork()->isDORDeadlockable()) return false;
   return true; // No change of direction
}

//*************************************************************************
//:
//  f: TPZROUTING getOurputPort (TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

TPZROUTINGTYPE  TPZSimpleRouterFlow :: getOutputDirection(TPZMessage* msg)
{  
   int deltaX = msg->delta(0);
   int deltaY = msg->delta(1);
   int deltaZ = msg->delta(2);
   if( msg->isHeader() || msg->isHeadTail() )
   {  
      if( deltaX > 1 )
      {
         return _Xplus_;                 
      }
      else if( deltaX < -1 )
      {
         return _Xminus_;                 
      }
      else if( deltaY > +1 )
      {
         return _Yplus_;           
      }
      else if(deltaY < -1)
      {
        return  _Yminus_;                
      }
      else if( deltaZ > +1 )
      {
         return _Zplus_;           
      }
      else if(deltaZ < -1)
      {
        return  _Zminus_;                
      }
      else
      {
         return _LocalNode_;
      }
   }
   return _Unknow_; //Change of direction
}

//*************************************************************************
//:
//  f: unsigned extractOutputPortNumber (const TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

unsigned TPZSimpleRouterFlow :: extractOutputPortNumber(TPZMessage* msg)
{
   unsigned oPort;

   oPort = extractOutputPortNumber( msg->getRoutingPort());
   return oPort;
}


//*************************************************************************
//:
//  f: unsigned extractOutputPortNumber (TPZROUTINGTYPE dir, unsigned channel);
//
//  d:
//:
//*************************************************************************

unsigned TPZSimpleRouterFlow :: extractOutputPortNumber( TPZROUTINGTYPE dir )
{
   unsigned oPort = ((TPZSimpleRouter&)getComponent()).getOutputWith( dir );
   
   if( !oPort )
   {
      TPZString err;
      err.sprintf( ERR_TPZCFLOW_001,
                   (char*)getComponent().asString(),
                   (char*)_routingType2string(dir),
                   0 );
      EXIT_PROGRAM(err);
   }
   
   return oPort;
}

//*************************************************************************
//:
//  f: unsigned extractOutputChannel (const TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

unsigned TPZSimpleRouterFlow :: extractOutputChannel(TPZMessage* msg) const
{
   return msg->getChannel();
}

//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent (const TPZEvent & event);
//
//  d:
//:
//*************************************************************************

Boolean TPZSimpleRouterFlow :: dispatchEvent(const TPZEvent& event)
{
   return true; 
}


//*************************************************************************
//:
//  f: void cleanOutputInterfaces ();
//
//  d:
//:
//*************************************************************************

void TPZSimpleRouterFlow :: cleanOutputInterfaces()
{
   if( cleanInterfaces() )
   {
      unsigned i,j;
      TPZSimpleRouter& simpleRouter = (TPZSimpleRouter&)getComponent();
   
      forInterfaz(i,simpleRouter.numberOfOutputs())
      {
         forInterfaz(j,outputInterfaz(i)->numberOfCV() )
            outputInterfaz(i)->clearData(j);
      }
   }
}

//*************************************************************************
//:
//  f: void cleanInputInterfaces ();
//
//  d:
//:
//*************************************************************************

void TPZSimpleRouterFlow :: cleanInputInterfaces()
{
   if( cleanInterfaces() )
   {
      unsigned i,j;
      TPZSimpleRouter& simpleRouter = (TPZSimpleRouter&)getComponent();
      forInterfaz(i,simpleRouter.numberOfInputs())
      {
          forInterfaz(j,outputInterfaz(i)->numberOfCV() )
            inputInterfaz(i)->clearStop(j);         
      }
   }
}

//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm (Boolean info = false, int delta = 0);
//
//  d:
//:
//*************************************************************************

Boolean TPZSimpleRouterFlow :: controlAlgoritm(Boolean info, int delta)
{
   // To prevent it from being an abstract class.
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZSimpleRouterFlow :: onReadyUp(unsigned interfaz, unsigned cv)
{  
   TPZMessage* msg;
   inputInterfaz(interfaz)->getData(&msg,cv); 
   //Hardwired here the virtual network selection
   //account of events
   msg->incDistance();
   ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::BufferWrite);
   if(interfaz == m_ports)
   {
      cv=1;
   }
   m_sync[m_ports*(cv-1)+interfaz]=msg;
   return true;      
}

      
//*************************************************************************
//:
//  f: virtual Boolean onStopUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZSimpleRouterFlow :: onStopUp(unsigned interfaz, unsigned cv)
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

Boolean TPZSimpleRouterFlow :: onStopDown(unsigned interfaz, unsigned cv)
{
   return true;
}

//*************************************************************************
//:
//  f: TPZString getStatus () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZSimpleRouterFlow :: getStatus() const
{
   TPZSimpleRouter& crb = (TPZSimpleRouter&)getComponent();
   TPZString rs = crb.asString() + TPZString(":\tIn stop= ");

   int i, channel;
   for( i=1; i<=crb.numberOfInputs(); i++ )
   {
      if( inputInterfaz(i)->isStopActive() )
         rs+= TPZString("I") + TPZString(i) + " ";
   }
   rs += TPZString(".\tOut stop= ");
   for( i=1; i<=crb.numberOfOutputs(); i++ )
   {
      if( crb.isLocalNodeOutput(i) )
      {
         if( outputInterfaz(i)->isStopActive() )
            rs+= TPZString("O") + TPZString(i) + " ";
      }

      else 
      {
         channel=1;
         if( outputInterfaz(i)->isStopActive(channel) )
            rs+= TPZString("O") + TPZString(i) + TPZString(channel) + " ";
      } 
   }

   return rs;
}

//*************************************************************************
//:
//  f: virtual void run (utime runtime);
//
//  d:
//:
//*************************************************************************

void TPZSimpleRouterFlow :: run(uTIME runTime)
{
   inputReading();
   stateChange ();
   outputWriting ();
}


//*************************************************************************
//:
//  f: virtual void preRun (utime time);
//
//  d:
//:
//*************************************************************************

void TPZSimpleRouterFlow :: preRun(uTIME runTime)
{
   //preInputRead ();
   // By default, do nothing. Only those components overloaded
   // requiring actions in the second half period of clock.
}


//*************************************************************************
//:
//  f: virtual void postRun (utime time);
//
//  d:
//:
//*************************************************************************

void TPZSimpleRouterFlow :: postRun(uTIME time)
{
   //inputReading ();
}
//*************************************************************************


// end of file

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
//    File: TPZSimpleRouterFlowTorus.cpp
//
//    Class:  TPZSimpleRouterFlowTorus
//
//    Inherited from:  TPZSimpleRouterFlow
// :
//*************************************************************************
//end of header



#include <TPZSimpleRouterFlowTorus.hpp>

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

IMPLEMENT_RTTI_DERIVED(TPZSimpleRouterFlowTorus,TPZSimpleRouterFlow);

//*************************************************************************
//:
//  f: TPZSimpleRouterFlowTorus ();
//
//  d:
//:
//*************************************************************************

TPZSimpleRouterFlowTorus :: TPZSimpleRouterFlowTorus( TPZComponent& component)
                 : TPZSimpleRouterFlow(component)
{
}


//*************************************************************************
//:
//  f: ~ TPZSimpleRouterFlow ();
//
//  d:
//:
//*************************************************************************

TPZSimpleRouterFlowTorus :: ~TPZSimpleRouterFlowTorus()
{
}


//*************************************************************************
//:
//  f: virtual void initialize ();
//
//  d:
//:
//*************************************************************************

void TPZSimpleRouterFlowTorus :: initialize()
{
   Inhereited :: initialize();
   m_vnets=((TPZSimpleRouter&)getComponent()).getVnets();
   m_ports=((TPZSimpleRouter&)getComponent()).numberOfInputs();
   unsigned ports=m_vnets*m_ports+1;
   m_changeDirection=new Boolean[ports];
   
   for(int i=0; i<ports; i++)
   {
      m_changeDirection[i] = false;
   }
   
}


//*************************************************************************
//:
//  f: virtual void terminate ();
//
//  d:
//:
//*************************************************************************

void TPZSimpleRouterFlowTorus :: terminate()
{
   delete[] m_changeDirection;
   
} 


//*************************************************************************
//:
//  f: virtual Boolean inputReading ();
//
//  d:
//:
//*************************************************************************
Boolean TPZSimpleRouterFlowTorus :: inputReading()
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
      
      // If no message at routing, go to the next
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

      // If there is a message routing
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
	       m_changeDirection[inPort] = updateMessageInfo(m_routing[inPort]);
               m_routingtime[inPort] = getHeaderDelay();
#ifndef NO_TRAZA
               TPZString texto = getComponent().asString() + " REPLICATION ";
	       //texto += TPZString( " \nLeaving MSG= ") + mess->asString();
               //texto += TPZString(" \nRemaining MSG= ") + m_routing[inPort]->asString();
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
         // Quito routing the message to come over
         //m_routing[inPort] = 0;    
	 
      }
   }
   
   //******************************************************************************************************************
   // PART 3: Process the connection of the crossbar
   // the token is to be round robin arbitration
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
      // If it is not a header, go to the next
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
      //if we are changing direction or injecting, bubble must be verified
      if( ! m_connections[outPort] && ( ! m_changeDirection[inPort] || bubbleReady(outPort+m_ports*(virtualChannel-1)) ) )
      {
         // Occupy the port
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
           texto += TPZString(" Connection: ") + TPZString(m_connections[outPort]) +  TPZString(" ChDir: ") + TPZString(m_changeDirection[inPort]);
	   texto += TPZString(" Bubble: ") +  TPZString(bubbleReady(outPort+m_ports*(virtualChannel-1)));
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
      
   }
   // No ports were rejected
   if( ! m_token ) m_token = inPort;
       
   //****************************************************************************************************************
   // PART 2: Loop through all the routing
   // Filling those that are empty with data from the fifo
   //****************************************************************************************************************
   for(inPort = 1; inPort <= m_ports*m_vnets; inPort++) //the in ports from the perspective of the output port
   {
      // If the routing is empty
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
               // If the message header
               if( m_routing[inPort]->isHeader() || m_routing[inPort]->isHeadTail() )
               {
                  // Calculate the output port
                  m_changeDirection[inPort] = updateMessageInfo(m_routing[inPort]);
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
      // If a message at syncronizer,
      
      if( m_sync[inPort] )
      {
#ifndef NO_TRAZA
                  TPZString texto = getComponent().asString() + " BUFFER-WRITE ";
                  texto +=  m_sync[inPort]->asString()+ " TIME: " + TPZString(getOwnerRouter().getCurrentTime());
                  TPZWRITE2LOG(texto);
#endif
            // Put it in the corresponding buffer
            m_fifos[inPort].enqueue(m_sync[inPort]);
            // and remove it syncs
            m_sync[inPort]=0;
      }
   }  
   return true;   
}

//*************************************************************************
//:
//  f: virtual Boolean bubbleReady(unsigned inPort);
//
//  d:
//:
//*************************************************************************

Boolean TPZSimpleRouterFlowTorus :: bubbleReady(unsigned inPort) const
{
   unsigned bubble = ((TPZSimulation*)getComponent().getSimulation())->
                        getPacketLength((TPZNetwork*)getOwnerRouter().getOwner()) * 2;   

   // If there is not room for two whole packets, no bubble available.
   if(m_bufferSize - m_fifos[inPort].numberOfElements() < bubble)
   {
      return false;
   }
   else
   {
      return true;
   }
}

//*************************************************************************
// end of file

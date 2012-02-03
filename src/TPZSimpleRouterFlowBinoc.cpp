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
//    File: TPZSimpleRouterFlowBinoc.cpp
//
//    Class:  TPZSimpleRouterFlowBinoc
//
//    Inherited from:  TPZFlow
// :
//*************************************************************************
//end of header



#include <TPZSimpleRouterFlowBinoc.hpp>

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

IMPLEMENT_RTTI_DERIVED(TPZSimpleRouterFlowBinoc,TPZFlow);

//*************************************************************************
//:
//  f: TPZSimpleRouterFlow ();
//
//  d:
//:
//*************************************************************************

TPZSimpleRouterFlowBinoc :: TPZSimpleRouterFlowBinoc( TPZComponent& component)
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

TPZSimpleRouterFlowBinoc :: ~TPZSimpleRouterFlowBinoc()
{
}


//*************************************************************************
//:
//  f: virtual void initialize ();
//
//  d:
//:
//*************************************************************************

void TPZSimpleRouterFlowBinoc :: initialize()
{
   Inhereited :: initialize();
   m_vnets=((TPZSimpleRouter&)getComponent()).getVnets();
   m_ports=((TPZSimpleRouter&)getComponent()).numberOfInputs();
   
   unsigned ports=m_vnets*m_ports+1;
   m_connections=new unsigned[m_ports+1];
   m_routing=new TPZMessage*[ports];
   m_routingtime=new int[ports];
   m_sync=new TPZMessage*[ports];
   m_changeDirection=new Boolean[ports];
   m_fifos=new TPZMessageQueue[ports];
   m_shortcut=new unsigned[ports];
   m_connEstablished=new Boolean[ports];
   
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
      m_changeDirection[i] = false;
      m_shortcut[i]=0;
      m_connEstablished[i] = false;
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

void TPZSimpleRouterFlowBinoc :: terminate()
{
   delete[] m_connections;
   delete[] m_routing;
   delete[] m_routingtime;
   delete[] m_sync;
   delete[] m_changeDirection;
   delete[] m_fifos;
   delete[] m_shortcut;
   delete[] m_connEstablished;
} 

//*************************************************************************
//:
//  f: virtual Boolean inputReading ();
//
//  d:
//:
//*************************************************************************
Boolean TPZSimpleRouterFlowBinoc :: inputReading()
{  
   unsigned outPort;
   unsigned inPort;
   unsigned virtualChannel;
   cleanOutputInterfaces();
   
   //**********************************************************************************************************
   // PART 4: Loop through all output ports.
   // We move the data flits for already established connections
   //**********************************************************************************************************
   for( outPort = 1; outPort <= m_ports; outPort++)
   {   
      // Find the input port assigned to outport.
      // If not assigned, go to the next.
      if( ! (inPort = m_connections[outPort] ) ) continue;
           
      // If no message routing through to the next
      if( ! m_routing[inPort] ) continue;
      
#ifndef NO_TRAZA
      TPZString texto = getComponent().asString() + " CONEXION ESTABLECIDA ";
      texto += TPZString(" inPort: ") + TPZString(inPort) + TPZString(" outPort: ") + TPZString(outPort);
      TPZWRITE2LOG(texto);
#endif     
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
         TPZROUTINGTYPE tipo=((TPZSimpleRouter&)getComponent()).getTypeForHDOutput(outPort);
         TPZROUTINGTYPE tipoReverse=getTipoReverse(tipo); 
         
         // The corresponding output control
         outputInterfaz(outPort)->sendData(m_routing[inPort],virtualChannel);
         ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::SWTraversal);
         ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::BufferRead);
         if ( tipo != _LocalNode_ )
         {
            getOwnerRouter().incrLinkUtilization();
            ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::LinkTraversal);
         }
#ifndef NO_TRAZA
         TPZString texto = getComponent().asString() + " SW TRAVERSAL ";
         texto +=  " From  ";
         texto += m_routing[inPort]->asString() + " TIME: " + TPZString(getOwnerRouter().getCurrentTime());
         texto += TPZString(" inPort: ") + TPZString(inPort) + TPZString(" outPort: ") + TPZString(outPort);
         texto += TPZString(" VChannel: ") + TPZString(virtualChannel);
         TPZWRITE2LOG(texto);
#endif
         if( m_routing[inPort]->isTail() || m_routing[inPort]->isHeadTail() )
         {
            m_connections[outPort] = 0;
            m_connEstablished[inPort] = false;
            if(outPort<5)
            {
               getOwnerRouter().setStateLink(tipo, false);
            }
            else
            {
               if(tipo!=_LocalNode_)
               {
                  ((TPZRouter*)(getOwnerRouter().getNeighbour(tipo)))->setStateLink(tipoReverse, false);
               }
            }
#ifndef NO_TRAZA
            TPZString texto = getComponent().asString() + " CLEARING ";
            texto +=  " From  ";
            texto += m_routing[inPort]->asString() + " TIME: " + TPZString(getOwnerRouter().getCurrentTime());
            texto += TPZString(" inPort: ") + TPZString(inPort) + TPZString(" outPort: ") + TPZString(outPort);
            texto += TPZString(" VChannel: ") + TPZString(virtualChannel);
            TPZWRITE2LOG(texto);
#endif
         }
         // Quito routing the message to come over
         m_routing[inPort] = 0;    
      }
   }
   
   //******************************************************************************************************************
   // PART 3 A: Process the connection of the crossbar, we only arbitrate for NORMAL ports
   // the token is to be round robin arbitration
   //******************************************************************************************************************
   inPort = m_token;
   m_token = 0;
   int i;
   for(i = 0; i < m_ports*m_vnets ; i++, inPort = inPort % (m_ports*m_vnets) + 1)
   {
      //Implementation without CV
      virtualChannel=(inPort-1)/m_ports+1;
      
      if( ! m_routing[inPort] ) continue;
      
      if( (! m_routing[inPort]->isHeader()) && (! m_routing[inPort]->isHeadTail()) ) continue;
      
      if( m_connEstablished[inPort]==true ) continue;
        
      // Remove the output port
      outPort = extractOutputPortNumber(m_routing[inPort], false); 
      
      if(outPort == m_ports) virtualChannel=1;
      if(inPort == m_ports) 
      {
         virtualChannel=m_routing[inPort]->getVnet(); 
	 if(inPort>m_ports)continue;
      }
      //*****************************************************************************************************
      //Here we change the verification of stop signal and link availability.
      //*****************************************************************************************************
      if( outputInterfaz(outPort)->isStopActive(virtualChannel) ) 
      {
         if( ! m_token ) m_token = inPort;
         continue;
      }
      
      TPZROUTINGTYPE tipo=((TPZSimpleRouter&)getComponent()).getTypeForHDOutput(outPort);
      if(tipo!=_LocalNode_)
      {
         if( (m_routing[inPort]->isHeader() || m_routing[inPort]->isHeadTail()) && getOwnerRouter().getStateLink(tipo)==true)
         {
            if( ! m_token ) m_token = inPort;
            continue;
         }
      }   
      //**********************************************************************************************************
      
      if( ! m_connections[outPort] )
      {
         // Take the normal port
         m_connections[outPort] = inPort;
         m_connEstablished[inPort] = true;
         getOwnerRouter().setStateLink(tipo, true);
          
#ifndef NO_TRAZA
         TPZString texto = getComponent().asString() + " SW ARB [GRANT] [NORM]";
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
         TPZString texto = getComponent().asString() + " SW ARB [REJECT] [NORM] ";
         texto +=  " From  ";
         texto += m_routing[inPort]->asString() + " TIME: " + TPZString(getOwnerRouter().getCurrentTime());
         texto += TPZString(" inPort: ") + TPZString(inPort) + TPZString(" outPort: ") + TPZString(outPort);
         texto += TPZString(" VChannel: ") + TPZString(virtualChannel);
         TPZWRITE2LOG(texto);
#endif
         // If the first port is neglected in this round
         // keep it to serve first in the following
         if( ! m_token )
         {
            m_token = inPort;
         }
      }
   }
   // Has not neglected any port, start where possible.
   if( ! m_token )
       m_token = inPort; 
   
   
   return true;   
}


//*************************************************************************
//:
//  f: virtual Boolean stateChange ();
//
//  d:
//:
//*************************************************************************

Boolean TPZSimpleRouterFlowBinoc :: stateChange()
{
   unsigned outPortReverse;
   unsigned inPort;
   unsigned virtualChannel;
   
   //******************************************************************************************************************
   // PART 3 B: We process the connection of the crossbar for REVERSE links
   // the token is to be round robin arbitration
   //******************************************************************************************************************
   inPort = m_token;
   m_token = 0;
   int i;
   for(i = 0; i < m_ports*m_vnets ; i++, inPort = inPort % (m_ports*m_vnets) + 1)
   {
      //Implementation without CV
      virtualChannel=(inPort-1)/m_ports+1;
      
      if( ! m_routing[inPort] ) continue;
      
      if( (! m_routing[inPort]->isHeader()) && (! m_routing[inPort]->isHeadTail()) ) continue;
      
      if( m_connEstablished[inPort]==true ) continue;
        
      if( m_routing[inPort]->isOrdered() ) continue;
            
      // Remove the output port
      outPortReverse = extractOutputPortNumber(m_routing[inPort], true);
      
      if(outPortReverse == m_ports) virtualChannel=1;
      if(inPort == m_ports) 
      {
         virtualChannel=m_routing[inPort]->getVnet(); 
	 if(inPort>m_ports)continue;
      }
      //*****************************************************************************************************
      //Here we change the verification of stop signal and link availability.
      //*****************************************************************************************************
      if( outputInterfaz(outPortReverse)->isStopActive(virtualChannel) ) 
      {
         if( ! m_token ) m_token = inPort;
         continue;
      }
      
      TPZROUTINGTYPE tipo=((TPZSimpleRouter&)getComponent()).getTypeForHDOutput(outPortReverse);
      TPZROUTINGTYPE tipoReverse=getTipoReverse(tipo);
      
      if( (tipo!=_LocalNode_) && (((TPZRouter*)(getOwnerRouter().getNeighbour(tipo)))->getStateLink(tipoReverse)==true) )
      {
         if( ! m_token ) m_token = inPort;
         continue;
      }
      
      //**********************************************************************************************************
      //change order
      //**********************************************************************************************************
           
      if( (! m_connections[outPortReverse]) )
      {
         // Occupy the port reverse
         m_connections[outPortReverse] = inPort;
         m_connEstablished[inPort] = true;
         ((TPZRouter*)(getOwnerRouter().getNeighbour(tipo)))->setStateLink(tipoReverse,true);
     
#ifndef NO_TRAZA
         TPZString texto = getComponent().asString() + " SW ARB [GRANT] [REV]";
         texto +=  " From  ";
         texto += m_routing[inPort]->asString() + " TIME: " + TPZString(getOwnerRouter().getCurrentTime());
         texto += TPZString(" inPort: ") + TPZString(inPort) + TPZString(" outPort: ") + TPZString(outPortReverse);
         texto += TPZString(" VChannel: ") + TPZString(virtualChannel);
         TPZWRITE2LOG(texto);
#endif
      }
      else
      {
#ifndef NO_TRAZA
         TPZString texto = getComponent().asString() + " SW ARB [REJECT] [REV] ";
         texto +=  " From  ";
         texto += m_routing[inPort]->asString() + " TIME: " + TPZString(getOwnerRouter().getCurrentTime());
         texto += TPZString(" inPort: ") + TPZString(inPort) + TPZString(" outPort: ") + TPZString(outPortReverse);
         texto += TPZString(" VChannel: ") + TPZString(virtualChannel);
         TPZWRITE2LOG(texto);
#endif
         // If the first port is neglected in this round
         // keep it to serve first in the following
         if( ! m_token )
         {
            m_token = inPort;
         }
      }
      // If not, wait for the next cycle.
   }
   // Has not neglected any port, start where possible.
   if( ! m_token )
       m_token = inPort; 
   
   //****************************************************************************************************************
   // PART 2: Loop through all the routing
   // Filling those that are empty with data from the fifo
   //****************************************************************************************************************
   for(inPort = 1; inPort <= m_ports*m_vnets; inPort++) //the in ports from the perspective of the output port
   {
      // If the routing is empty
      if( ! m_routing[inPort] )
      {
        // Take the next message in the corresponding fifo
         if(m_fifos[inPort].numberOfElements()!=0)
         {
            m_fifos[inPort].dequeue(m_routing[inPort]);
#ifndef NO_TRAZA
            TPZString texto = getComponent().asString() + " BUFFER-READ ";
            texto +=  m_routing[inPort]->asString()+ " TIME: " + TPZString(getOwnerRouter().getCurrentTime());
            TPZWRITE2LOG(texto);
#endif               
            // If the message header
            if( m_routing[inPort]->isHeader() || m_routing[inPort]->isHeadTail())
            {
               // Calculate the output port with mesh DOR not need to know if it changes direction
               //m_changeDirection [inport] = updateMessageInfo (m_routing [inport]);
               updateMessageInfo(m_routing[inPort]);
               m_routingtime[inPort]=getHeaderDelay();
            }
            else
            {
               m_routingtime[inPort]=0;
            }
         }
      }
   }
   
   //**********************************************************************************************************
   // PART 1: Loop through all sync.
   //**********************************************************************************************************
   for( inPort = 1; inPort <= m_ports*m_vnets; inPort++)
   {
      // If a message sicronizador,
      if(m_sync[inPort])
      {
         // Put it in the corresponding buffer
         m_fifos[inPort].enqueue(m_sync[inPort]);
#ifndef NO_TRAZA
         TPZString texto = getComponent().asString() + " BUFFER-WRITE ";
         texto +=  m_sync[inPort]->asString()+ " TIME: " + TPZString(getOwnerRouter().getCurrentTime());
         TPZWRITE2LOG(texto);
#endif            
         // and remove it syncs
         m_sync[inPort]=0;
      }
   }
   
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean outputWriting ();
//
//  d:
//:
//*************************************************************************

Boolean TPZSimpleRouterFlowBinoc :: outputWriting()
{
   unsigned inPort;
   unsigned sizeForCT = ((TPZSimulation*)getComponent().getSimulation())->
                        getPacketLength((TPZNetwork*)getOwnerRouter().getOwner());
   unsigned virtualChannel; 
  

   // Through all the buffers.
   for( inPort = 1; inPort <= m_ports*m_vnets; inPort++)
   {
      // If there is no room for another package, send stop signal.
      //Only an injection virtual channel
      
      //Implementation without CV
      virtualChannel=(inPort-1)/m_ports+1;
      if( inPort%m_ports == 0 ) {virtualChannel=1; if(inPort>m_ports)continue;} //Injection provided a channel
      if((m_bufferSize - m_fifos[inPort].numberOfElements()) <= sizeForCT)
      {
         inputInterfaz((inPort-1)%m_ports+1)->sendStopRightNow(virtualChannel); //desde1 numbering whore mania
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

Boolean TPZSimpleRouterFlowBinoc :: propagateStop()
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

Boolean TPZSimpleRouterFlowBinoc :: updateMessageInfo(TPZMessage* msg)
{  
   int deltaX = msg->delta(0);
   int deltaY = msg->delta(1);
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
      else
      {
         msg->setRoutingPort(_LocalNode_);
         return false; // Not to do flow control.
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

TPZROUTINGTYPE  TPZSimpleRouterFlowBinoc :: getOutputDirection(TPZMessage* msg)
{  
   int deltaX = msg->delta(0);
   int deltaY = msg->delta(1);

   if( msg->isHeader() || msg->isHeadTail())
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

unsigned TPZSimpleRouterFlowBinoc :: extractOutputPortNumber(TPZMessage* msg, Boolean reverse)
{
   unsigned oPort;

   oPort = extractOutputPortNumber( msg->getRoutingPort(), reverse);
   return oPort;
}


//*************************************************************************
//:
//  f: unsigned extractOutputPortNumber (TPZROUTINGTYPE dir, unsigned channel);
//
//  d:
//:
//*************************************************************************

unsigned TPZSimpleRouterFlowBinoc :: extractOutputPortNumber( TPZROUTINGTYPE dir, Boolean reverse )
{
   unsigned channel=1;
   if ( dir == _LocalNode_ ) reverse=false;
   unsigned oPort = ((TPZSimpleRouter&)getComponent()).getHDOutputWith( dir, channel, reverse );
   
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

unsigned TPZSimpleRouterFlowBinoc :: extractOutputChannel(TPZMessage* msg) const
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

Boolean TPZSimpleRouterFlowBinoc :: dispatchEvent(const TPZEvent& event)
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

void TPZSimpleRouterFlowBinoc :: cleanOutputInterfaces()
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

void TPZSimpleRouterFlowBinoc :: cleanInputInterfaces()
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

Boolean TPZSimpleRouterFlowBinoc :: controlAlgoritm(Boolean info, int delta)
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

Boolean TPZSimpleRouterFlowBinoc :: onReadyUp(unsigned interfaz, unsigned cv)
{  
   TPZMessage* msg;
   inputInterfaz(interfaz)->getData(&msg,cv); 
   //Hardwired here the virtual network selection
   //account of events
   ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::BufferWrite);
   msg->incDistance();
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

Boolean TPZSimpleRouterFlowBinoc :: onStopUp(unsigned interfaz, unsigned cv)
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

Boolean TPZSimpleRouterFlowBinoc :: onStopDown(unsigned interfaz, unsigned cv)
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

TPZString TPZSimpleRouterFlowBinoc :: getStatus() const
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

void TPZSimpleRouterFlowBinoc :: run(uTIME runTime)
{
   inputReading();
   
}


//*************************************************************************
//:
//  f: virtual void preRun (utime time);
//
//  d:
//:
//*************************************************************************

void TPZSimpleRouterFlowBinoc :: preRun(uTIME time)
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

void TPZSimpleRouterFlowBinoc :: postRun(uTIME time)
{
   stateChange();
   outputWriting();
}

//*************************************************************************
//:
//  f: TPZROUTINGTYPE getTipoReverse (TPZROUTINGTYPE type)
//
//  d:
//:
//*************************************************************************
TPZROUTINGTYPE TPZSimpleRouterFlowBinoc :: getTipoReverse(TPZROUTINGTYPE tipo)
{
   switch(tipo)
      {
        case _Xplus_: 
    return _Xminus_;
    break;
    case _Xminus_: 
    return _Xplus_;
    break;
    case _Yplus_: 
    return _Yminus_;
    break;
    case _Yminus_: 
    return _Yplus_;
    case _LocalNode_:
    return _LocalNode_;
    break;
      }
}

//*************************************************************************


// end of file

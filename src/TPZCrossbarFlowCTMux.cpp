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
//    File: TPZCrossbarFlowCTMux.cpp
//
//    Class:  TPZCrossbarFlowCTMux
//
//    Inherited from:  TPZCrossbarFlow
// :
//*************************************************************************
//end of header



#include <TPZCrossbarFlowCTMux.hpp>

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

#ifndef __TPZCrossbar_HPP__
#include <TPZCrossbar.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZCrossbarFlowCTMux,TPZCrossbarFlow);

//*************************************************************************
//:
//  f: TPZCrossbarFlowCTMux ();
//
//  d:
//:
//*************************************************************************

TPZCrossbarFlowCTMux :: TPZCrossbarFlowCTMux( TPZComponent& component)
                   : TPZCrossbarFlow(component){
}


//*************************************************************************
//:
//  f: ~ TPZCrossbarFlowCTMux ();
//
//  d:
//:
//*************************************************************************

TPZCrossbarFlowCTMux :: ~TPZCrossbarFlowCTMux()

{   
   
}


//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent (const TPZEvent & event);
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbarFlowCTMux :: dispatchEvent(const TPZEvent& event)
{        
   if( ( event.type() == _CrossbarRequest_ ) ||
       ( event.type() == _CrossbarPass_ ) )
   {
      TPZCrossbar& crossbar = (TPZCrossbar&)(getComponent());
      unsigned iPort =   event.source();
      unsigned oPort =   getOutputForInput(iPort).left();
      unsigned channel = getOutputForInput(iPort).right();
      
      if(event.type() == _CrossbarRequest_)
      {
        oPort=event.destiny();
        channel=event.channel();  
      } 
      TPZMessage *msg;
      m_MessageReceivedTable->valueAt(iPort,&msg);
      
      if(!msg) return true;
      
      if(!channel)
      {
         if(_CrossbarRequest_)
         {
           EXIT_PROGRAM("Channel set to 0 in request Not allowed");
         }
         else
         {
           EXIT_PROGRAM("Channel set to 0 in data Not allowed");
         }
      }  
      else if( ! iPort )
      {
         TPZString err;
         err.sprintf( ERR_TPZCFLOC_001, 
                      (char*)getComponent().asString(),
                      (char*)msg->asString() );
         EXIT_PROGRAM(err);
      }
      else if(!oPort)
      {
         TPZString err;
         err.sprintf( ERR_TPZCFLOC_002, 
                      (char*)getComponent().asString(),
                      (char*)msg->asString() );
         EXIT_PROGRAM(err);
      }
      
      //Here's the difference with the normal version
      if( event.type() == _CrossbarRequest_)
      {
         for(int i=1; i<=crossbar.numberOfInputs(); i++)
         {
            uPAIR temp;
            m_PortAsignamentTable->valueAt(i,temp);
            if(i!=iPort && (temp.left()==oPort) )
            {
                inputInterfaz(iPort)->sendStop();
                uTIME delayTime = getOwnerRouter().getCurrentTime() + 1;
                getEventQueue().enqueue(event,delayTime); 

   #ifndef NO_TRAZA 
                TPZString time = getOwnerRouter().getCurrentTime();       
                TPZString texto = getComponent().asString() + " CONTENTION  STOP TIME = ";
                texto += time + msg->asString() + " i=" + TPZString(iPort) +
                    ", o=" + TPZString(oPort) + ", OPORT used by i=" + TPZString(i);
                TPZWRITE2LOG( texto );
   #endif
                return true;
            }
         }
      
         if( (outputInterfaz(oPort)->isStopActive(channel)))
         {
            inputInterfaz(iPort)->sendStop();
            uTIME delayTime = getOwnerRouter().getCurrentTime() + 1;
            getEventQueue().enqueue(event,delayTime);

   #ifndef NO_TRAZA 
      TPZString time = getOwnerRouter().getCurrentTime();       
      TPZString texto = getComponent().asString() + " QUEUES  STOP TIME = ";
      texto += time + msg->asString() + " i=" + TPZString(iPort) +
               ", o=" + TPZString(oPort);
             
      TPZWRITE2LOG( texto );
   #endif
               return true;
         }
               establishConnection(iPort,oPort,channel);

      setStateForInput(iPort,Connect);   
      }

   
    
      inputInterfaz(iPort)->clearStopRightNow();
     
      outputInterfaz(oPort)->sendData(msg,channel);
      ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::SWTraversal);
      if (msg->getRoutingPort()!=_LocalNode_) 
      ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::LinkTraversal);
           
   #ifndef NO_TRAZA 
      TPZString time = getOwnerRouter().getCurrentTime();       
      TPZString texto = getComponent().asString() + " Flit Tx. TIME = ";
      texto += time + msg->asString() + " i=" + TPZString(iPort) +
               ", o=" + TPZString(oPort);
            
      TPZWRITE2LOG( texto );
   #endif
   
      if( msg->isTail() )
      {
         clearConnection(iPort);
         uPAIR temp;
         temp = getConnectionRequestForWith(oPort);
         iPort=temp.left();
         channel=temp.right();
         if( iPort != 0 )
         {
            establishConnection(iPort,oPort,channel);
            TPZEvent requestEvent(_CrossbarRequest_,iPort,oPort,channel);
            uTIME delayTime = getOwnerRouter().getCurrentTime() +
                              MAXIMO(1,getHeaderDelay());
            
            if( ((TPZCrossbar&)getComponent()).isLocalNodeOutput(oPort) )
            {
               if( getHeaderDelay() > 1 )
                  delayTime -= 1;
            }
            
            getEventQueue().enqueue(requestEvent,delayTime);
         }
      }
   }  
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean onReadyUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbarFlowCTMux :: onReadyUp(unsigned interfaz, unsigned cv)
{
  unsigned i = interfaz;
   TPZMessage* msg;
   TPZMessage* lastMessage;
   
   inputInterfaz(i)->getData(&msg);
   m_MessageReceivedTable->valueAt(i,&lastMessage);

   if(lastMessage)
      if( *msg == *lastMessage )
         return true;

   m_MessageReceivedTable->setValueAt(i,msg);
   
   uTIME delayTime = getOwnerRouter().getCurrentTime() ;

    #ifndef NO_TRAZA
            TPZString texto = getComponent().asString() + " Flit Rx. TIME = ";
            texto += TPZString(delayTime) + " # " + msg->asString();
            TPZWRITE2LOG( texto );
   #endif
   if( getOutputForInput(i).left() == 0 )
   {
      // This is to run the TPZCrossbarFlowCTMux
      if(!msg->isHeader())
      {
         return true;
      } 
      
      // This from the head.
      unsigned outPort = extractOutputPortNumber(msg);
      unsigned outChannel = extractOutputChannel(msg);
      if( outChannel==0)
      {
         EXIT_PROGRAM("Message without output VC");
      }          
      TPZEvent requestEvent(_CrossbarRequest_,i,outPort,outChannel);
            
      setStateForInput(i,Waiting);

      if( isOutputPortFree(outPort,outChannel) )
      {
         // You must assign an output
         
         delayTime += getHeaderDelay();
         
         if( ((TPZCrossbar&)getComponent()).isLocalNodeOutput(outPort) )
         {
            if( getHeaderDelay() > 1 )
               delayTime -= 1;
         }
         
         getEventQueue().enqueue(requestEvent,delayTime);
      }
      else
      {
         // The output port is busy. A request is queued
             
          #ifndef NO_TRAZA
               TPZString texto = getComponent().asString() + " Esperando una SALIDA !";
               texto += TPZString(delayTime) + " iport= " + TPZString(i);
               TPZWRITE2LOG( texto );
         #endif
                        
         addConnectionRequest(i,uPAIR(outPort,outChannel));
      }

       #ifndef NO_TRAZA
          TPZString time = getOwnerRouter().getCurrentTime();       
         TPZString texto = getComponent().getName() + " Stop Tx. TIME = ";
         texto += time + " iport= " + TPZString(i);
         TPZWRITE2LOG( texto );
      #endif
            
      inputInterfaz(i)->sendStopRightNow();
   }
   else
   {
      delayTime += getDataDelay();
      TPZEvent passEvent(_CrossbarPass_,i);
      getEventQueue().enqueue(passEvent,delayTime);
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

Boolean TPZCrossbarFlowCTMux :: onStopUp(unsigned interfaz, unsigned cv)
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

Boolean TPZCrossbarFlowCTMux :: onStopDown(unsigned interfaz, unsigned cv)
{   
      return Inhereited::onStopDown(interfaz,cv);
}

//*************************************************************************
//:
//  f: virtual unsigned extractOutputPortNumber (TPZMessage * msg);
//
//  d:
//:
//*************************************************************************

unsigned TPZCrossbarFlowCTMux :: extractOutputPortNumber(TPZMessage* msg)
{
   unsigned oPort = ((TPZCrossbar&)getComponent()).getOutputWith( msg->getRoutingPort(),1 );
   if( !oPort )
   {
      TPZString err;
      err.sprintf( ERR_TPZCMFLO_003,
                   (char*)getComponent().asString(),
                   (char*)msg->asString(),
                   (char*)_routingType2string(msg->getRoutingPort()) );
      EXIT_PROGRAM(err);
   }
   
   if( ((TPZCrossbar&)getComponent()).isLocalNodeOutput(oPort) )
      oPort += ( msg->getChannel() - 1 );
   return oPort;
}

//*************************************************************************
//:
//  f: unsigned extractOutputPortNumber (TPZROUTINGTYPE dir, unsigned channel);
//
//  d:
//:
//*************************************************************************

unsigned TPZCrossbarFlowCTMux :: extractOutputPortNumber( TPZROUTINGTYPE dir,
                                                     unsigned channel )
{
   unsigned oPort = ((TPZCrossbar&)getComponent()).getOutputWith( dir,1 );
   if( !oPort )
   {
      TPZString err;
      err.sprintf( ERR_TPZCMFLO_003,
                   (char*)getComponent().asString(),
                   (char*)_routingType2string(dir) );
      EXIT_PROGRAM(err);
   }
   
   if( ((TPZCrossbar&)getComponent()).isLocalNodeOutput(oPort) )
      oPort += ( channel - 1 );
   return oPort;
}

//*************************************************************************


// end of file

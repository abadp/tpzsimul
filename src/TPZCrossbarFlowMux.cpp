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
//    File: TPZCrossbarFlowMux.cpp
//
//    Class:  TPZCrossbarFlowMux
//
//    Inherited from:  TPZCrossbarFlow
// :
//*************************************************************************
//end of header



#include <TPZCrossbarFlowMux.hpp>

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
 
IMPLEMENT_RTTI_DERIVED(TPZCrossbarFlowMux,TPZCrossbarFlow);

//*************************************************************************
//:
//  f: TPZCrossbarFlowMux ();
//
//  d:
//:
//*************************************************************************

TPZCrossbarFlowMux :: TPZCrossbarFlowMux( TPZComponent& component)
                    : TPZCrossbarFlow(component)
{

}
 

//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent (const TPZEvent & event);
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbarFlowMux :: dispatchEvent(const TPZEvent& event)
{
   if( event.type() == _CrossbarRequest_ )
   {
      ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::SWArbitration);
      unsigned oPort = event.destiny();

      if( ! isOutputPortFree(oPort) )
      {
         Inhereited::cleanConnectionRequestFor(oPort);
         return true;
      }
       
      uPAIR inp_channel = checkMultipleRequestFor(oPort);
      
      if( outputInterfaz(oPort)->isStopActive(inp_channel.right()) )
      {
         cleanConnectionRequestFor(oPort,inp_channel);
         return true;
      }
      
      unsigned iPort = inp_channel.left();
      
      establishConnection(iPort,oPort,inp_channel.right());

      cleanConnectionRequestFor(oPort,inp_channel);
      setLastInputForOutput(iPort,oPort);
      setStateForInput(iPort,Connect);
      
      removeAllRequestEventsForInput(iPort);
      removeAllRequestEventsForOutput(oPort);

      inputInterfaz(iPort)->clearStopRightNow();

   }

   else if( event.type() == _CrossbarPass_ ) 
   {
      unsigned iPort  = event.source();
      unsigned oPort  = getOutputForInput(iPort).left();
      unsigned channel= getOutputForInput(iPort).right();
 
      if( ! iPort )
      {
         TPZString err;
         err.sprintf( ERR_TPZCMFLO_001,
                      (char*)getComponent().asString() );
         EXIT_PROGRAM(err);
      }
      TPZMessage* msg;
      m_MessageReceivedTable->valueAt(iPort,&msg);
      
      if(!oPort)
      {
         TPZString time = getOwnerRouter().getCurrentTime();       
         TPZString err=getComponent().asString()+" not asigned oPort for iPort="+TPZString(iPort)+msg->asString()+" Time ="+TPZString(time);
         EXIT_PROGRAM(err);
      }
 
      setStateForInput(iPort,Connect);      
      inputInterfaz(iPort)->clearStopRightNow();
      updateMessageInfo(msg,oPort,channel);    
      outputInterfaz(oPort)->sendData(msg,channel);
      ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::SWTraversal);
      if ( ((TPZCrossbar&)getComponent()).getTypeForOutput(oPort, channel) != _LocalNode_ )
      {
         getOwnerRouter().incrLinkUtilization();
         ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::LinkTraversal);
      }

       #ifndef NO_TRAZA
            TPZString time = getOwnerRouter().getCurrentTime();       
            TPZString texto = getComponent().asString() + " Flit Tx. TIME = ";
            texto += time + msg->asString() + " i=" + TPZString(iPort) +
                     ", o=" + TPZString(oPort);
            
            TPZWRITE2LOG( texto );
      #endif

      if( msg->isTail() || msg->isHeadTail() )
      {
         clearConnection(iPort); 
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

Boolean TPZCrossbarFlowMux :: onReadyUp(unsigned interfaz, unsigned cv)
{
   unsigned i = interfaz;
   TPZMessage* msg;
   TPZMessage* lastMessage;

   inputInterfaz(i)->getData(&msg);
   m_MessageReceivedTable->valueAt(i,&lastMessage);
      
   if(lastMessage)
      if( (*msg == *lastMessage) && (msg->type()!=TPZMessage::REQ) ) return true;

   m_MessageReceivedTable->setValueAt(i,msg);
   uTIME delayTime = getOwnerRouter().getCurrentTime() ;

   #ifndef NO_TRAZA
        TPZString texto = getComponent().asString() + " Flit Rx. TIME = ";
        texto += TPZString(delayTime) + " # " + msg->asString();
        TPZWRITE2LOG( texto );
  #endif
            
   if( msg->type()==TPZMessage::REQ )
   {      
      unsigned outPort    = extractOutputPortNumber(msg);            
      unsigned outChannel = (((TPZCrossbar&)getComponent()).isLocalNodeOutput(outPort)) ?
                            1 : extractOutputChannel(msg);
      
      if(  outPort ==0 )
      {
        TPZString err=getComponent().asString()+ TPZString("null port request")+msg->asString();
        EXIT_PROGRAM(err);
      }
#ifdef PTOPAZ
      int n=msg->getPoolIndex();
      getMessagePool(n).release(msg);
#else      
      getMessagePool().release(msg);
#endif      
      m_MessageReceivedTable->setValueAt(i,0);

      if( isOutputPortFree(outPort,outChannel) && 
          ( ! outputInterfaz(outPort)->isStopActive(outChannel)) )
      {
         delayTime += getHeaderDelay();
         if( ! isAnyRequestForOutput(outPort) )
         {
            TPZEvent requestEvent(_CrossbarRequest_,i,outPort,outChannel);
            getEventQueue().enqueue(requestEvent,delayTime);
         }
         addConnectionRequest(i,uPAIR(outPort,outChannel));
         #ifndef NO_TRAZA
            TPZString time = getOwnerRouter().getCurrentTime();       
            TPZString texto = getComponent().asString() + " REQ accepted." + msg->asString() + time + 
            " iport= " + TPZString(i)+"  o="+TPZString(outPort) + " cvo="+ TPZString(outChannel);
            TPZWRITE2LOG( texto );
         #endif
          
      }
      else
      {
         // The output port is busy. 
         #ifndef NO_TRAZA
            TPZString time = getOwnerRouter().getCurrentTime();       
            TPZString texto = getComponent().asString() + " REQ rejected." +msg->asString()+time+" iport=" +TPZString(i)
            + "  o="+TPZString(outPort) + " cvo="+ TPZString(outChannel);
            if(!isOutputPortFree(outPort,outChannel))
            {
               texto+=" REASON 1 : port is occupied by other packet in transit";
            }
            if( outputInterfaz(outPort)->isStopActive(outChannel))
            {
               texto+=" REASON 2 : remote buffer is full";
            }
            TPZWRITE2LOG( texto );
         #endif
      }     
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

Boolean TPZCrossbarFlowMux :: onStopUp(unsigned interfaz, unsigned cv)
{
   TPZWRITE2LOG(getComponent().asString()+ ": StopUp "+TPZString(interfaz)+" "+TPZString(cv));
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbarFlowMux :: onStopDown(unsigned interfaz, unsigned cv)
{
   TPZWRITE2LOG(getComponent().asString()+ ": StopDown "+TPZString(interfaz)+" "+TPZString(cv));
   return true;

}


//*************************************************************************
//:
//  f: virtual unsigned extractOutputPortNumber (const TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

unsigned TPZCrossbarFlowMux :: extractOutputPortNumber(TPZMessage* msg)
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
//  f: virtual unsigned extractOutputPortNumber (TPZROUTINGTYPE dir,
//                                               unsigned channel);
//
//  d:
//:
//*************************************************************************

unsigned TPZCrossbarFlowMux :: extractOutputPortNumber( TPZROUTINGTYPE dir,
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
//:
//  f: virtual Boolean isOutputPortFree (unsigned outputPort,
//                                       unsigned channel = 0) const;
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbarFlowMux :: isOutputPortFree( unsigned outputPort, 
                                                unsigned channel) const
{
   // If this any output channel is occupied,
   // then it returns false.
   
   unsigned state=0;
   unsigned mux = ((TPZCrossbar&)(getComponent())).getOutputMux();
   for( int i=1; i<=mux; i++ )
   {
      m_OutputStateTable->valueAt(i+(outputPort-1)*mux,state);
      if(state) return false;
   }
   
   return true;
   
}


//*************************************************************************
//:
//  f: void cleanInputInterfaces ();
//
//  d:
//:
//*************************************************************************

void TPZCrossbarFlowMux :: cleanInputInterfaces()
{
   // Is overwritten to do nothing.
}


//*************************************************************************
//:
//  f: virtual void postRun (utime time);
//
//  d:
//:
//*************************************************************************

void TPZCrossbarFlowMux :: postRun(uTIME time)
{
   setCleanInterfaces(false);
   run(time);
   setCleanInterfaces(true);
}


//*************************************************************************
//:
//  f: Boolean updateMessageInfo (TPZmessage ** msg,
//                                unsigned port,
//                                unsigned channel);
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbarFlowMux :: updateMessageInfo( TPZMessage* msg, 
                                              unsigned oPort,
                                              unsigned channel )
{
   // It does nothing in headers.
   return true;
}

//*************************************************************************

// end of file

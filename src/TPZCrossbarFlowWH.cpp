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
//    File: TPZCrossbarFlowWH.cpp
//
//    Class:  TPZCrossbarFlowWH
//
//    Inherited from:  TPZCrossbarFlow
// :
//*************************************************************************
//end of header



#include <TPZCrossbarFlowWH.hpp>

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
 
IMPLEMENT_RTTI_DERIVED(TPZCrossbarFlowWH,TPZCrossbarFlow);

//*************************************************************************
//:
//  f: TPZCrossbarFlowMux ();
//
//  d:
//:
//*************************************************************************

TPZCrossbarFlowWH :: TPZCrossbarFlowWH( TPZComponent& component)
                    : TPZCrossbarFlow(component),
                      m_OutputPortStateTable(0),
                      m_InOutPortTable(0),
	              m_routingDelay(1),
	              m_VCArbitrationDelay(1),
	              m_SWArbitrationDelay(1),
	              m_SWTraversalDelay(1)
{
   
}

//*************************************************************************
//:
//  f: virtual void initialize ();
//
//  d:
//:
//*************************************************************************
void TPZCrossbarFlowWH :: initialize()
{
   Inhereited :: initialize();
   
   TPZCrossbar& crossbar = (TPZCrossbar&)getComponent(); 
   
   m_OutputPortStateTable = new TPZStateTable(crossbar.numberOfOutputs());
   m_InOutPortTable       = new TPZInToOutConnectTable(crossbar.numberOfInputs());
   
   m_routingDelay       = crossbar.getRoutingDelay();
   m_VCArbitrationDelay = crossbar.getVCArbitrationDelay();
   m_SWArbitrationDelay = crossbar.getSWArbitrationDelay();
   m_SWTraversalDelay   = crossbar.getSWTraversalDelay();
   
   m_OutputPortStateTable->initialize(FREE);
}

//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent (const TPZEvent & event);
//
//  d:
//:
//*************************************************************************
Boolean TPZCrossbarFlowWH :: dispatchEvent(const TPZEvent& event)
{
   uTIME delayTime = getOwnerRouter().getCurrentTime();
   
   //**********************************************************************
   // EVENT= ROUTING
   //**********************************************************************
   if( event.type() == _RoutingVC_ )
   {
      
      //update the header and select output port
      TPZMessage *msg;
      unsigned iPort = event.source();
      m_MessageReceivedTable->valueAt(iPort,&msg);
      
      if( (!(msg->isHeader())) && (!(msg->isHeadTail())))
      {
         TPZString err;
         err.sprintf( "%s :Data flits should not pass through this event",(char*)getComponent().asString() );
         EXIT_PROGRAM(err); 
      }
      
#ifndef NO_TRAZA
         TPZString texto = getComponent().asString() + " Event ROUTING. TIME = ";
         texto += TPZString(getOwnerRouter().getCurrentTime()) + " # " + "iPort=" + TPZString(iPort) + " # " + msg->asString() ;
         TPZWRITE2LOG( texto );
#endif
            
      if (!msg->isMulticast())//multicast messages are proceesed at FIFO buffer, avoid re-routing
      {
         int deltaX = msg->delta(0);
         int deltaY = msg->delta(1);
         int deltaZ = msg->delta(2);
  
         if( deltaX > 1 ) msg->setRoutingPort(_Xplus_);         
         else if( deltaX < -1 ) msg->setRoutingPort(_Xminus_);
         else if( deltaY > +1 ) msg->setRoutingPort(_Yplus_);         
         else if( deltaY < -1 ) msg->setRoutingPort(_Yminus_);
         else if( deltaZ > +1 ) msg->setRoutingPort(_Zplus_);         
         else if( deltaZ < -1 ) msg->setRoutingPort(_Zminus_);
         else msg->setRoutingPort(_LocalNode_);
      }      
      unsigned portout = extractOutputPortNumber(msg);
      m_InOutPortTable->setValueAt(iPort, portout);
      
      TPZEvent SWAllocEvent(_SwitchAllocator_, iPort, portout, 1, msg);
      getEventQueue().enqueue(SWAllocEvent, delayTime+m_routingDelay); 
      
   }
   
   //**********************************************************************
   // EVENT= SWITCH ALLOCATION
   //**********************************************************************
   else if( event.type() == _SwitchAllocator_ )
   {
      
      unsigned iPort  = event.source();
      unsigned oPort = event.destiny();
      TPZMessage* msg;
      m_MessageReceivedTable->valueAt(iPort,&msg);
      
#ifndef NO_TRAZA
      TPZString texto = getComponent().asString() + " Event SW ALLOCATION. TIME = "
      + TPZString(getOwnerRouter().getCurrentTime()) + " # " + "iPort=" + TPZString(iPort) 
      + " # oPort=" + TPZString(oPort) + msg->asString() ;
      TPZWRITE2LOG( texto );
#endif
            
      if (getStateForOutputPort(oPort)==FREE)
      {
#ifndef NO_TRAZA
         TPZString texto2 = getComponent().asString() + " Free port=" + TPZString(oPort);
         TPZWRITE2LOG( texto2);
#endif
         setStateForOutputPort(oPort, ASIGNED);
              
         TPZEvent SWTravEvent(_SwitchTraversal_, iPort, oPort, 1, msg);
         getEventQueue().enqueue(SWTravEvent, delayTime+1);
      }
      
      else
      {
#ifndef NO_TRAZA
         TPZString texto3 = getComponent().asString() + "Occupied port";
         TPZWRITE2LOG( texto3 );
#endif
     
         TPZEvent SWAllocEvent(_SwitchAllocator_,iPort, oPort, 1, msg);
         getEventQueue().enqueue(SWAllocEvent, delayTime+m_SWArbitrationDelay);
      }
      ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::SWArbitration);
   }
   
   //**********************************************************************
   // EVENT= SWITCH TRAVERSAL
   //**********************************************************************
   else if( event.type() == _SwitchTraversal_ )
   {
      
      unsigned iPort  = event.source();
      unsigned oPort = event.destiny();
      TPZMessage* msg;
      m_MessageReceivedTable->valueAt(iPort,&msg);
      
#ifndef NO_TRAZA
      TPZString texto = getComponent().asString() + " Event SW TRAVERSAL. TIME = "
      + TPZString(getOwnerRouter().getCurrentTime()) + " # " + "iPort=" + TPZString(iPort) 
      + " # oPort=" + TPZString(oPort) + msg->asString() ;
      TPZWRITE2LOG( texto );
#endif
      
      if ( (!outputInterfaz(oPort)->isStopActive()))
      {
         inputInterfaz(iPort)->clearStopRightNow();
         if (!msg->isMulticast()) updateMessageInfo(msg); //again, MC messages already processed    
         outputInterfaz(oPort)->sendData(msg);
              
         if ( msg->isTail() || msg->isHeadTail() )
         {
            setStateForOutputPort(oPort, FREE);
         }
	 
	 ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::SWTraversal);
	 if ( ((TPZCrossbar&)getComponent()).getTypeForOutput(oPort,1) != _LocalNode_)
	 {
	    getOwnerRouter().incrLinkUtilization();
	    ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::LinkTraversal);
	 }
      }
      else
      {
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         TPZEvent SWTravEvent(_SwitchTraversal_,iPort, oPort, 1, msg);
         getEventQueue().enqueue(SWTravEvent, delayTime+m_SWTraversalDelay);
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
Boolean TPZCrossbarFlowWH :: onReadyUp(unsigned interfaz, unsigned cv)
{
   unsigned i = interfaz;
   TPZMessage* msg;
   TPZMessage* lastMessage;

   inputInterfaz(i)->getData(&msg);
   
   m_MessageReceivedTable->setValueAt(i,msg);
   uTIME delayTime = getOwnerRouter().getCurrentTime() ;

#ifndef NO_TRAZA
   TPZString texto = getComponent().asString() + " Flit Rx. TIME = ";
   texto += TPZString(delayTime) + " # " + msg->asString();
   TPZWRITE2LOG( texto );
#endif
            
   if( msg->isHeader() || msg->isHeadTail() )
   {      
      // It is a header flit, routing must be performed
      TPZEvent routingEvent(_RoutingVC_, i);
      getEventQueue().enqueue(routingEvent, delayTime);      
   }
   
   else
   {
      //data or tail flits move directly to SWarbitration stage
      unsigned outPort;
      m_InOutPortTable->valueAt(i, outPort);
      TPZEvent SWTravEvent(_SwitchTraversal_, i, outPort, 1, msg);
      getEventQueue().enqueue(SWTravEvent, delayTime+1);
      
   }
   
   inputInterfaz(i)->sendStopRightNow();
   return true;    
}
      
//*************************************************************************
//:
//  f: virtual Boolean onStopUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************
Boolean TPZCrossbarFlowWH :: onStopUp(unsigned interfaz, unsigned cv)
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
Boolean TPZCrossbarFlowWH :: onStopDown(unsigned interfaz, unsigned cv)
{
   TPZWRITE2LOG(getComponent().asString()+ ": StopDown "+TPZString(interfaz)+" "+TPZString(cv));
   return true;

}

//*************************************************************************
//:
//  f: void cleanInputInterfaces ();
//
//  d:
//:
//*************************************************************************
void TPZCrossbarFlowWH :: cleanInputInterfaces()
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
void TPZCrossbarFlowWH :: postRun(uTIME time)
{
   setCleanInterfaces(false);
   run(time);
   setCleanInterfaces(true);
      
}

//*************************************************************************
//:
//  f: Boolean updateMessageInfo (TPZmessage * msg);
//
//  d:
//:
//*************************************************************************
Boolean TPZCrossbarFlowWH :: updateMessageInfo( TPZMessage* msg )
{
   //update hop-count for current direction
   int deltaX = msg->delta(0);
   int deltaY = msg->delta(1);
   int deltaZ = msg->delta(2);
  
   TPZROUTINGTYPE tipo=msg->getRoutingPort();
   
   switch(tipo)
   {
      case _Xplus_:
         msg->setDelta(deltaX-1,0);
	 return true;
      case _Xminus_:
         msg->setDelta(deltaX+1,0);
	 return true;
      case _Yplus_:
         msg->setDelta(deltaY-1,1);
	 return true;
      case _Yminus_:
         msg->setDelta(deltaY+1,1);
	 return true;
      case _Zplus_:
         msg->setDelta(deltaZ-1,2);
	 return true;
      case _Zminus_:
         msg->setDelta(deltaZ+1,2);
	 return true;
      case _LocalNode_:
         return true;
   }
   
}

//*************************************************************************
//:
//  f: virtual void setStateForOutputPort (unsigned i, TPZVCState state);
//                                
//  d:
//:
//*************************************************************************
void TPZCrossbarFlowWH ::  setStateForOutputPort(unsigned i, TPZState state)
{
   m_OutputPortStateTable->setValueAt(i, state);
}

//*************************************************************************
//:
//  f: virtual TPZVCState getStateForOutputPort (unsigned i);
//                                
//  d:
//:
//*************************************************************************
TPZState TPZCrossbarFlowWH ::  getStateForOutputPort(unsigned i)
{
   TPZState state;
   m_OutputPortStateTable->valueAt(i, state);
   return state;
}

//*************************************************************************

// end of file

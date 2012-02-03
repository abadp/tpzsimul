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
//    File: TPZCrossbarFlowVCTorusMux.cpp
//
//    Class:  TPZCrossbarFlowVCTorusMux
//
//    Inherited from:  TPZCrossbarFlowVCTorus
// :
//*************************************************************************
//end of header



#include <TPZCrossbarFlowVCTorusMux.hpp>

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
 
IMPLEMENT_RTTI_DERIVED(TPZCrossbarFlowVCTorusMux,TPZCrossbarFlowVCTorus);

//*************************************************************************
//:
//  f: TPZCrossbarFlowVCTorusMux ();
//
//  d:
//:
//*************************************************************************

TPZCrossbarFlowVCTorusMux :: TPZCrossbarFlowVCTorusMux( TPZComponent& component)
                    : TPZCrossbarFlowVCTorus(component),
            m_InputPortStateTable(0)
            
{
   
}

//*************************************************************************
//:
//  f: virtual void initialize ();
//
//  d:
//:
//*************************************************************************

void TPZCrossbarFlowVCTorusMux :: initialize()
{
   Inhereited :: initialize();
   m_InputPortStateTable   = new TPZState[m_inputs];
   
   for(int i=0; i<m_inputs; i++)
   {
      m_InputPortStateTable[i] = FREE;
   }
}
//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent (const TPZEvent & event);
//
//  d:
//:
//*************************************************************************
Boolean TPZCrossbarFlowVCTorusMux :: dispatchEvent(const TPZEvent& event)
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
      if( (!(msg->isHeader())) && (!(msg->isHeadTail())) )
      {
         TPZString err;
         err.sprintf( "%s :Body flits should not go through this state",(char*)getComponent().asString() );
         EXIT_PROGRAM(err); 
      }
#ifndef NO_TRAZA
        TPZString texto = getComponent().asString() + " Event ROUTING. TIME = ";
        texto += TPZString(getOwnerRouter().getCurrentTime()) + " # " + "iPort=" + TPZString(iPort) + " # " + msg->asString() ;
        TPZWRITE2LOG( texto );
#endif
      
      if (!msg->isMulticast()) //MC messages have been already processed
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
      unsigned offset=getOffsetFor(msg);
      setOffsetForInputPort(iPort, offset);
      
      //*****************************************************************
      //Ideal assignation of the first VC to request
      //*****************************************************************
      unsigned msgtype=msg->getVnet();
      unsigned VCFirstReq;
      //Oredered messages must request the same VC to avoid message forwarding
      if( msg->isOrdered()) VCFirstReq=1+offset;
      else VCFirstReq= getFirstVCReqFor(iPort, portout, msgtype);
      m_InOutVCTable->setValueAt(iPort, VCFirstReq);
      //*****************************************************************
      
      TPZEvent VCAllocEvent(_VCAllocator_, iPort, portout, VCFirstReq, msg);
      getEventQueue().enqueue(VCAllocEvent, delayTime+m_routingDelay); 
      
   }
   //**********************************************************************
   // EVENT= VIRTUAL CHANNEL ARBITRATION
   //**********************************************************************
   else if( event.type() == _VCAllocator_ ) 
   {
      unsigned iPort  = event.source();
      unsigned oPort = event.destiny();
      unsigned VChannel = event.channel();
      TPZMessage* msg;
      m_MessageReceivedTable->valueAt(iPort,&msg);
      if ( ( (!(msg->isHeader())) && (!(msg->isHeadTail())) ) || (!iPort) || (!oPort) ) 
      {
          TPZString err;
          err.sprintf("%s :Some kind of error at this point", (char*)getComponent().asString() );
          EXIT_PROGRAM(err);
      }
      
#ifndef NO_TRAZA
       TPZString texto = getComponent().asString() + " Event VC ALLOCATION. TIME = ";
       texto += TPZString(getOwnerRouter().getCurrentTime()) + " # " + "iPort=" + TPZString(iPort) 
       + " # oPort=" + TPZString(oPort) + " # VChannel =" + TPZString(VChannel) + msg->asString() ;
       TPZWRITE2LOG( texto );
#endif
      
      unsigned messtype=msg->getVnet();
      unsigned oVirtualChannel= getAbsolutValueForOutputVC( oPort, VChannel, messtype);
                  
      TPZState state=getStateForOutputVC(oVirtualChannel);
      
      if (state==FREE)
      {
#ifndef NO_TRAZA
         TPZString texto2 = getComponent().asString() + " Found Free VC" + " # OutputVC = " + TPZString(oVirtualChannel);
         TPZWRITE2LOG( texto2 );
#endif
         setStateForOutputVC(oVirtualChannel, ASIGNED);
         TPZEvent SWAllocEvent(_SwitchAllocator_, iPort, oPort, VChannel, msg);
         getEventQueue().enqueue(SWAllocEvent, delayTime+m_VCArbitrationDelay); 
      }
      else
      {
         //On the next VC arbitration event we request a different Virtual Channel
         unsigned mux = ((TPZCrossbar&)(getComponent())).getOutputMux();
         unsigned msgtypes = ((TPZCrossbar&)(getComponent())).getNumberMT();
         unsigned offset = getOffsetForInputPort(iPort);
	 unsigned VCNextRequest;
         if (msg->isOrdered())VCNextRequest=VChannel;
         else VCNextRequest=((VChannel)%(mux/(2*msgtypes)))+offset+1;
         m_InOutVCTable->setValueAt(iPort, VCNextRequest);
#ifndef NO_TRAZA
         TPZString texto3 = getComponent().asString() + " VC Occupied" + " # New VC Requested = " + TPZString(VCNextRequest);
         TPZWRITE2LOG( texto3 );
#endif
         TPZEvent VCAllocEvent(_VCAllocator_,iPort, oPort, VCNextRequest);
         getEventQueue().enqueue(VCAllocEvent, delayTime+m_VCArbitrationDelay);
      }
      ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::VCArbitration);
   }
   //**********************************************************************
   // EVENT= SWITCH ARBITRATION
   //**********************************************************************
   else if( event.type() == _SwitchAllocator_ )
   {
      
      unsigned iPort  = event.source();
      unsigned oPort = event.destiny();
      unsigned VChannel = event.channel();
      TPZMessage* msg;
      m_MessageReceivedTable->valueAt(iPort,&msg);
      
#ifndef NO_TRAZA
      TPZString texto = getComponent().asString() + " Event SW ALLOCATION. TIME = "
      + TPZString(getOwnerRouter().getCurrentTime()) + " # " + "iPort=" + TPZString(iPort) 
      + " # oPort=" + TPZString(oPort) + " # VChannel =" + TPZString(VChannel) + msg->asString() ;
      TPZWRITE2LOG( texto );
#endif
      
      unsigned mux = ((TPZCrossbar&)(getComponent())).getOutputMux();
      unsigned msgtypes = ((TPZCrossbar&)(getComponent())).getNumberMT();
      unsigned messtype=msg->getVnet(); 
      unsigned outputVChannel= (messtype-1)*(mux/msgtypes) + VChannel;
      if (oPort == m_outputs) outputVChannel=1;
      
      TPZState stateOut=getStateForOutputPort(oPort);
      unsigned iFisicalPort=getFisicalIportFor(iPort);
      TPZState stateIn=getStateForInputPort(iFisicalPort);
      
      if ( (!outputInterfaz(oPort)->isStopActive(outputVChannel)) && stateOut==FREE && stateIn==FREE)
      {
#ifndef NO_TRAZA
         TPZString texto2 = getComponent().asString() + " Free Port=" + TPZString(oPort) + "turns into ASIGNED";
         TPZWRITE2LOG( texto2);
#endif
         setStateForOutputPort(oPort, ASIGNED);
	 m_InOutPortTable->setValueAt(iPort, oPort);
         setStateForInputPort(iFisicalPort, ASIGNED);
         TPZEvent SWTravEvent(_SwitchTraversal_, iPort, oPort, VChannel, msg);
         getEventQueue().enqueue(SWTravEvent, delayTime+m_SWArbitrationDelay);
      }
      
      else
      {
#ifndef NO_TRAZA
         TPZString texto3 = getComponent().asString() + "Port already occupied";
         TPZWRITE2LOG( texto3 );
#endif
         TPZEvent SWAllocEvent(_SwitchAllocator_,iPort, oPort, VChannel, msg);
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
      unsigned VChannel = event.channel();
      TPZMessage* msg;
      m_MessageReceivedTable->valueAt(iPort,&msg);
      
#ifndef NO_TRAZA
      TPZString texto = getComponent().asString() + " Event SW TRAVERSAL. TIME = "
      + TPZString(getOwnerRouter().getCurrentTime()) + " # " + "iPort=" + TPZString(iPort) 
      + " # oPort=" + TPZString(oPort) + " # VChannel =" + TPZString(VChannel) + msg->asString() ;
      TPZWRITE2LOG( texto );
#endif
      unsigned mux = ((TPZCrossbar&)(getComponent())).getOutputMux();
      unsigned msgtypes = ((TPZCrossbar&)(getComponent())).getNumberMT();
      unsigned messtype=msg->getVnet(); 
      unsigned outputVChannel= (messtype-1)*(mux/msgtypes) + VChannel;
      if (oPort == m_outputs) outputVChannel=1;
      
      inputInterfaz(iPort)->clearStopRightNow();
      if (!msg->isMulticast() )updateMessageInfo(msg);//only unicast message need to update their contents.
      outputInterfaz(oPort)->sendData(msg,outputVChannel);
#ifndef NO_TRAZA
      TPZString texto2 = getComponent().asString() + "Port liberation =" + TPZString(oPort);
      TPZWRITE2LOG( texto2 );
#endif
      setStateForOutputPort(oPort, FREE);
      unsigned iFisicalPort=getFisicalIportFor(iPort);
      setStateForInputPort(iFisicalPort, FREE);
      
      if ( msg->isTail() || msg->isHeadTail() )
      {
         unsigned oVirtualChannel= getAbsolutValueForOutputVC( oPort, VChannel, messtype);
#ifndef NO_TRAZA
         TPZString texto3 = getComponent().asString() + "Channel liberation =" + TPZString(oVirtualChannel);
         TPZWRITE2LOG( texto3 );
#endif
         setStateForOutputVC(oVirtualChannel, FREE);
      }
      ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::SWTraversal);
      if ( ((TPZCrossbar&)getComponent()).getTypeForOutput(oPort,1) != _LocalNode_)
      {
         getOwnerRouter().incrLinkUtilization();
	 ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::LinkTraversal);
      }
   }
   return true;
}

//*************************************************************************
//:
//  f: virtual void setStateForInputPort (unsigned i, TPZState state);
//                                
//  d:
//:
//*************************************************************************

void TPZCrossbarFlowVCTorusMux ::  setStateForInputPort(unsigned i, TPZState state)
{
   m_InputPortStateTable[i]=state;
}

//*************************************************************************
//:
//  f: virtual TPZState getStateForInputPort (unsigned i);
//                                
//  d:
//:
//*************************************************************************

TPZState TPZCrossbarFlowVCTorusMux ::  getStateForInputPort(unsigned i)
{
   return m_InputPortStateTable[i];
}
//*************************************************************************
//:
//  f: virtual unsigned getFisicalIportFor (unsigned IPORT)
//
//  d:
//:
//*************************************************************************
unsigned TPZCrossbarFlowVCTorusMux :: getFisicalIportFor( unsigned iPort )
{
   unsigned inputs=((TPZCrossbar&)(getComponent())).numberOfInputs();
   unsigned FisicalIport;
   unsigned mux = ((TPZCrossbar&)(getComponent())).getOutputMux();
   unsigned num_inj=inputs-(mux*4);
   
   if ((iPort>0) && (iPort<=mux))
   {
      FisicalIport=1;
   }
   else if ((iPort>mux) && (iPort<=2*mux))
   {
      FisicalIport=2;
   }
   else if ((iPort>2*mux) && (iPort<=3*mux))
   {
      FisicalIport=3;
   }
   else if ((iPort>3*mux) && (iPort<=4*mux))
   {
      FisicalIport=4;
   }
   else if ((iPort>4*mux) && (iPort<=(4*mux+num_inj)))
   {
      FisicalIport=5;
   }
   else
   {
      TPZString err;
      err.sprintf( ERR_TPZCMFLO_001,(char*)getComponent().asString() );
      EXIT_PROGRAM(err);
   }
   
   return FisicalIport;
}

//*************************************************************************

// end of file

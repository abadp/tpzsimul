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
//    File: TPZCrossbarFlowVC.cpp
//
//    Class:  TPZCrossbarFlowVC
//
//    Inherited from:  TPZCrossbarFlow
// :
//*************************************************************************
//end of header

#include <TPZCrossbarFlowVCTorus.hpp>

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

IMPLEMENT_RTTI_DERIVED(TPZCrossbarFlowVCTorus, TPZCrossbarFlowVC);

//*************************************************************************
//:
//  f: TPZCrossbarFlowMux ();
//
//  d:
//:
//*************************************************************************

TPZCrossbarFlowVCTorus::TPZCrossbarFlowVCTorus(TPZComponent& component) :
                   TPZCrossbarFlowVC(component),
		   m_InputPortOffsetTable(0)
{
}

//*************************************************************************
//:
//  f: virtual void initialize ();
//
//  d:
//:
//*************************************************************************
void TPZCrossbarFlowVCTorus :: initialize()
{
   Inhereited :: initialize();
   
   unsigned mux = ((TPZCrossbar&)(getComponent())).getOutputMux();
   if (mux%2 !=0)
   {
      TPZString err;
      err.sprintf("%s :Torus topologies only supported for a pair number of ports", (char*)getComponent().asString() );
      EXIT_PROGRAM(err);
   }
   unsigned msgtypes = ((TPZCrossbar&)(getComponent())).getNumberMT();
   if ( (mux/msgtypes)<2 )
   {
      TPZString err2;
      err2.sprintf("%s :At least two virtual channels for each message type", (char*)getComponent().asString() );
      EXIT_PROGRAM(err2);
   }
   m_InputPortOffsetTable   = new unsigned[m_inputs];
   
   for(int i=0; i<m_inputs; i++)
   {
      m_InputPortOffsetTable[i] = 0;
   }
}
//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent (const TPZEvent & event);
//
//  d:
//:
//*************************************************************************
Boolean TPZCrossbarFlowVCTorus::dispatchEvent(const TPZEvent& event) 
{
   uTIME delayTime = getOwnerRouter().getCurrentTime();
   //**********************************************************************
   // EVENT= ROUTING
   //**********************************************************************
   if (event.type() == _RoutingVC_) 
   {
      //update the header and select output port
      TPZMessage *msg;
      unsigned iPort = event.source();
      m_MessageReceivedTable->valueAt(iPort, &msg);
      if ( (!(msg->isHeader())) && (!(msg->isHeadTail())) ) 
      {
         TPZString err;
         err.sprintf("%s :Body flits should not go through this state", (char*)getComponent().asString() );
         EXIT_PROGRAM(err);
      }
#ifndef NO_TRAZA
      TPZString texto = getComponent().asString() + " Event ROUTING. TIME = ";
      texto += TPZString(getOwnerRouter().getCurrentTime()) + " # " + "iPort=" + TPZString(iPort) + " # " + msg->asString() ;
      TPZWRITE2LOG(texto);
#endif

      if (!msg->isMulticast()) //MC messages have been already processed
      {
         int deltaX = msg->delta(0);
         int deltaY = msg->delta(1);
         int deltaZ = msg->delta(2);
      
         if (deltaX > 1) msg->setRoutingPort(_Xplus_);
         else if (deltaX < -1) msg->setRoutingPort(_Xminus_);
         else if (deltaY > +1) msg->setRoutingPort(_Yplus_);
         else if (deltaY < -1) msg->setRoutingPort(_Yminus_);
         else if (deltaZ > +1) msg->setRoutingPort(_Zplus_);
         else if (deltaZ < -1) msg->setRoutingPort(_Zminus_);
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
      if (msg->isOrdered()) VCFirstReq=1+offset;
      else VCFirstReq= getFirstVCReqFor(iPort, portout, msgtype);
      
      m_InOutVCTable->setValueAt(iPort, VCFirstReq);
      //*****************************************************************

      TPZEvent VCAllocEvent(_VCAllocator_, iPort, portout, VCFirstReq, msg);
      getEventQueue().enqueue(VCAllocEvent, delayTime+m_routingDelay);

    }
   //**********************************************************************
   // EVENT= VIRTUAL CHANNEL ARBITRATION
   //**********************************************************************
    else if (event.type() == _VCAllocator_) 
    {
       unsigned iPort = event.source();
       unsigned oPort = event.destiny();
       unsigned VChannel = event.channel();
       TPZMessage* msg;
       m_MessageReceivedTable->valueAt(iPort, &msg);
       
       if ( ( (!(msg->isHeader())) && (!(msg->isHeadTail())) ) || (!iPort) || (!oPort) ) 
       {
          TPZString err;
          err.sprintf("%s :Some kind of error at this point", (char*)getComponent().asString() );
          EXIT_PROGRAM(err);
       }
       
#ifndef NO_TRAZA
       TPZString texto = getComponent().asString() + " Event VC ALLOCATION. TIME = ";
       texto += TPZString(getOwnerRouter().getCurrentTime()) + " # " + "iPort=" + TPZString(iPort) + " # oPort="
       + TPZString(oPort) + " # VChannel =" + TPZString(VChannel) + msg->asString() ;
       TPZWRITE2LOG(texto);
#endif
       
       //The absolute port value corresponding to the message must be calculated
       unsigned messtype=msg->getVnet();
       unsigned oVirtualChannel=getAbsolutValueForOutputVC(oPort, VChannel, messtype);
       
       TPZState state=getStateForOutputVC(oVirtualChannel);

       if (state!=FREE) 
       {
          //On the next VC arbitration event we request a different Virtual Channel
          unsigned mux = ((TPZCrossbar&)(getComponent())).getOutputMux();
          unsigned msgtypes = ((TPZCrossbar&)(getComponent())).getNumberMT();
          unsigned offset = getOffsetForInputPort(iPort);
	  unsigned VCNextRequest;
          if (msg->isOrdered()) VCNextRequest=VChannel;
	  else VCNextRequest=((VChannel)%(mux/(2*msgtypes)))+offset+1;
          m_InOutVCTable->setValueAt(iPort, VCNextRequest);
#ifndef NO_TRAZA
          TPZString texto3 = getComponent().asString() + " VC Occupied" + " # New VC Requested = " + TPZString(VCNextRequest);
          TPZWRITE2LOG(texto3);
#endif
          TPZEvent VCAllocEvent(_VCAllocator_, iPort, oPort, VCNextRequest);
          getEventQueue().enqueue(VCAllocEvent, delayTime+m_VCArbitrationDelay);
       }
       else 
       {
#ifndef NO_TRAZA
          TPZString texto2 = getComponent().asString() + " Found Free VC" + " # OutputVC = " + TPZString(oVirtualChannel);
          TPZWRITE2LOG(texto2);
#endif
          setStateForOutputVC(oVirtualChannel, ASIGNED);
          TPZEvent SWAllocEvent(_SwitchAllocator_, iPort, oPort, VChannel, msg);
          getEventQueue().enqueue(SWAllocEvent, delayTime+m_VCArbitrationDelay);
       }
       ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::VCArbitration);
    }
   
   //**********************************************************************
   // EVENT= SWITCH ARBITRATION
   //**********************************************************************
    else if (event.type() == _SwitchAllocator_) 
    {
       unsigned iPort = event.source();
       unsigned oPort = event.destiny();
       unsigned VChannel = event.channel();
       TPZMessage* msg;
       m_MessageReceivedTable->valueAt(iPort, &msg);

#ifndef NO_TRAZA
       TPZString texto = getComponent().asString() + " Event SW ALLOCATION. TIME = "
       + TPZString(getOwnerRouter().getCurrentTime()) + " # " + "iPort=" + TPZString(iPort)
       + " # oPort=" + TPZString(oPort) + " # VChannel =" + TPZString(VChannel) + msg->asString() ;
       TPZWRITE2LOG(texto);
#endif
       unsigned mux = ((TPZCrossbar&)(getComponent())).getOutputMux();
       unsigned msgtypes = ((TPZCrossbar&)(getComponent())).getNumberMT();
       unsigned messtype=msg->getVnet();
       unsigned outputVChannel= (messtype-1)*(mux/msgtypes) + VChannel;
       if (oPort == m_outputs) outputVChannel=1;
       
       TPZState state=getStateForOutputPort(oPort);
       
       if ( (!outputInterfaz(oPort)->isStopActive(outputVChannel)) && state==FREE) 
       {
#ifndef NO_TRAZA
          TPZString texto2 = getComponent().asString() + " Free Port=" + TPZString(oPort) + "turns into ASIGNED";
          TPZWRITE2LOG(texto2);
#endif
          setStateForOutputPort(oPort, ASIGNED);
	  m_InOutPortTable->setValueAt(iPort, oPort);
          TPZEvent SWTravEvent(_SwitchTraversal_, iPort, oPort, VChannel, msg);
          getEventQueue().enqueue(SWTravEvent, delayTime+m_SWArbitrationDelay);
       }
       else 
       {
#ifndef NO_TRAZA
          TPZString texto3 = getComponent().asString() + "already occupied port";
          TPZWRITE2LOG(texto3);
#endif
          TPZEvent SWAllocEvent(_SwitchAllocator_, iPort, oPort, VChannel, msg);
          getEventQueue().enqueue(SWAllocEvent, delayTime+m_SWArbitrationDelay);
       }
       ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::SWArbitration);
    }
   //**********************************************************************
   // EVENT= SWITCH TRAVERSAL
   //**********************************************************************
    else if (event.type() == _SwitchTraversal_) 
    {
       unsigned iPort = event.source();
       unsigned oPort = event.destiny();
       unsigned VChannel = event.channel();
       TPZMessage* msg;
       m_MessageReceivedTable->valueAt(iPort, &msg);
       
#ifndef NO_TRAZA
       TPZString texto = getComponent().asString() + " Event SW TRAVERSAL. TIME = "
       + TPZString(getOwnerRouter().getCurrentTime()) + " # " + "iPort=" + TPZString(iPort)
       + " # oPort=" + TPZString(oPort) + " # VChannel =" + TPZString(VChannel) + msg->asString() ;
        TPZWRITE2LOG(texto);
#endif
       unsigned mux = ((TPZCrossbar&)(getComponent())).getOutputMux();
       unsigned msgtypes = ((TPZCrossbar&)(getComponent())).getNumberMT();
       unsigned messtype=msg->getVnet();
       unsigned outputVChannel= (messtype-1)*(mux/msgtypes) + VChannel;
       if (oPort == m_outputs) outputVChannel=1;
       
       inputInterfaz(iPort)->clearStopRightNow();
       if (!msg->isMulticast() )updateMessageInfo(msg);//only unicast message need to update their contents.
       outputInterfaz(oPort)->sendData(msg, outputVChannel);
#ifndef NO_TRAZA
       TPZString texto2 = getComponent().asString() + "Port liberation =" + TPZString(oPort);
       TPZWRITE2LOG(texto2);
#endif
       setStateForOutputPort(oPort, FREE);

       if (msg->isTail() || msg->isHeadTail() ) 
       {
          unsigned oVirtualChannel= getAbsolutValueForOutputVC(oPort, VChannel, messtype);
          
#ifndef NO_TRAZA
            TPZString texto3 = getComponent().asString() + "Channel liberation =" + TPZString(oVirtualChannel);
            TPZWRITE2LOG(texto3);
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
//  f: virtual unsigned getFirstVCReqFor (unsigned IPORT)
//                                
//  d:
//:
//*************************************************************************
unsigned TPZCrossbarFlowVCTorus::getFirstVCReqFor(unsigned iPort, unsigned oPort, unsigned msgtype) 
{
   TPZMessage *msg;
   m_MessageReceivedTable->valueAt(iPort, &msg);
   
   unsigned offset=getOffsetFor(msg);
   unsigned VCout;
   unsigned mux = ((TPZCrossbar&)(getComponent())).getOutputMux();
   unsigned msgtypes = ((TPZCrossbar&)(getComponent())).getNumberMT();
   Boolean ideal_asign = false;
   for (int i=1; i<=(mux/(2*msgtypes)); i++) 
   {
      unsigned outVC=getAbsolutValueForOutputVC(oPort, i+offset, msgtype);
      TPZState state=getStateForOutputVC(outVC);
      if (state==FREE) 
      {
         ideal_asign = true;
         VCout = i+offset;
         break;
      }
   }
   
   if (ideal_asign == false) VCout = (rand()%(mux/(2*msgtypes)))+offset+1;
   return VCout;
}

//*************************************************************************
//:
//  f: virtual unsigned getOffsetFor (TPZMessage* msg)
//                                
//  d:
//:
//*************************************************************************
unsigned TPZCrossbarFlowVCTorus::getOffsetFor(TPZMessage* msg) 
{
   unsigned offset;
   
   unsigned mux = ((TPZCrossbar&)(getComponent())).getOutputMux();
   unsigned msgtypes = ((TPZCrossbar&)(getComponent())).getNumberMT();
   unsigned factor=mux/(2*msgtypes);
   
   TPZROUTINGTYPE direction=msg->getRoutingPort();
   TPZPosition source=msg->source();
   TPZPosition destiny= msg->destiny();
   unsigned sourceComponent, destinyComponent;
   
   switch(direction)
   {
      case _Xplus_: 
         sourceComponent= source.valueForCoordinate(TPZPosition::X);
	 destinyComponent= destiny.valueForCoordinate(TPZPosition::X);
	 if (destinyComponent > sourceComponent) offset=0;
	 else offset= factor;
         break;
      case _Xminus_:
         sourceComponent= source.valueForCoordinate(TPZPosition::X);
	 destinyComponent= destiny.valueForCoordinate(TPZPosition::X);
	 if (destinyComponent > sourceComponent) offset=factor;
	 else offset= 0;
         break;
      case _Yplus_: 
         sourceComponent= source.valueForCoordinate(TPZPosition::Y);
	 destinyComponent= destiny.valueForCoordinate(TPZPosition::Y);
	 if (destinyComponent > sourceComponent) offset=0;
	 else offset= factor;
         break;
      case _Yminus_:
         sourceComponent= source.valueForCoordinate(TPZPosition::Y);
	 destinyComponent= destiny.valueForCoordinate(TPZPosition::Y);
	 if (destinyComponent > sourceComponent) offset=factor;
	 else offset= 0;
         break;
      case _Zplus_: 
         sourceComponent= source.valueForCoordinate(TPZPosition::Z);
	 destinyComponent= destiny.valueForCoordinate(TPZPosition::Z);
	 if (destinyComponent > sourceComponent) offset=0;
	 else offset= factor;
         break;
      case _Zminus_:
         sourceComponent= source.valueForCoordinate(TPZPosition::Z);
	 destinyComponent= destiny.valueForCoordinate(TPZPosition::Z);
	 if (destinyComponent > sourceComponent) offset=factor;
	 else offset= 0;
         break;
      case _LocalNode_:
         offset= 0;
	 break;      
   }
   return offset;
   
}

//*************************************************************************
//:
//  f: virtual void setStateForInputPort (unsigned i, TPZState state);
//                                
//  d:
//:
//*************************************************************************
void TPZCrossbarFlowVCTorus ::  setOffsetForInputPort(unsigned i, unsigned offset)
{
   m_InputPortOffsetTable[i]=offset;
}

//*************************************************************************
//:
//  f: virtual TPZState getStateForInputPort (unsigned i);
//                                
//  d:
//:
//*************************************************************************

unsigned TPZCrossbarFlowVCTorus ::  getOffsetForInputPort(unsigned i)
{
   return m_InputPortOffsetTable[i];
}
//*************************************************************************

// end of file

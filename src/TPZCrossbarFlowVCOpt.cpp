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
//    File: TPZCrossbarFlowVCOpt.cpp
//
//    Class:  TPZCrossbarFlowVCOpt
//
//    Inherited from:  TPZCrossbarFlowVC
// :
//*************************************************************************
//end of header

#include <TPZCrossbarFlowVCOpt.hpp>

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

IMPLEMENT_RTTI_DERIVED(TPZCrossbarFlowVCOpt, TPZCrossbarFlowVC);

//*************************************************************************
//:
//  f: TPZCrossbarFlowMux ();
//
//  d:
//:
//*************************************************************************

TPZCrossbarFlowVCOpt::TPZCrossbarFlowVCOpt(TPZComponent& component) :
                   TPZCrossbarFlowVC(component) 
		   
{
}

//*************************************************************************
//:
//  f: virtual Boolean onReadyUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************
Boolean TPZCrossbarFlowVCOpt::onReadyUp(unsigned interfaz, unsigned cv) 
{
    unsigned i = interfaz;
    TPZMessage* msg;
    TPZMessage* lastMessage;

    inputInterfaz(i)->getData(&msg);

    m_MessageReceivedTable->setValueAt(i, msg);
    uTIME delayTime = getOwnerRouter().getCurrentTime() ;

#ifndef NO_TRAZA
    TPZString texto = getComponent().asString() + " Flit Rx. TIME = ";
    texto += TPZString(delayTime) + " # " + msg->asString();
    TPZWRITE2LOG(texto);
#endif

    if (msg->isHeader() || msg->isHeadTail()) 
    {
       // It is a header flit, routing must be performed
       // Here is the point where we optimize pipeline.
       // If certain conditions are met, the header can move
       // directly to Switch Traversal Stage.
       if ( checkPipelineForMessageAt(i)==true)
       {
          unsigned outPort;
          unsigned outVirtualChannel;
          m_InOutPortTable->valueAt(i, outPort);
          m_InOutVCTable->valueAt(i, outVirtualChannel);
#ifndef NO_TRAZA
          TPZString texto2 = getComponent().asString() + " Flit BYPASS. TIME = ";
          texto2 += TPZString(delayTime) + " # " ;
	  texto2 += TPZString(" InPort=") + TPZString(i) + " OutPort=" + TPZString(outPort);
	  texto2 += TPZString(" OutVC=") + TPZString(outVirtualChannel);
          TPZWRITE2LOG(texto2);
#endif
	  ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::RouterBypass);
	  TPZEvent SWTravEvent(_SwitchTraversal_, i, outPort, outVirtualChannel, msg);
          getEventQueue().enqueue(SWTravEvent, delayTime+1);          
       }
       else
       {
          TPZEvent routingEvent(_RoutingVC_, i);
          getEventQueue().enqueue(routingEvent, delayTime);        
       }
    }
    
    else 
    {
       //data or tail going directly to arbitration Switch
       unsigned outPort;
       unsigned outVirtualChannel;
       m_InOutPortTable->valueAt(i, outPort);
       m_InOutVCTable->valueAt(i, outVirtualChannel);
       TPZEvent xbarAllocEvent(_SwitchAllocator_, i, outPort, outVirtualChannel, msg);
       getEventQueue().enqueue(xbarAllocEvent, delayTime);
    }

    inputInterfaz(i)->sendStopRightNow();

    return true;
}

//*************************************************************************
//:
//  f: virtual Boolean checkPipelineFor ( TPZMessage* msg)
//
//  d:
//:
//*************************************************************************
Boolean TPZCrossbarFlowVCOpt::checkPipelineForMessageAt(unsigned iPort) 
{
   TPZMessage* msg;
   m_MessageReceivedTable->valueAt(iPort, &msg);
   
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
   
   //first we check if the port is free
   unsigned portout = extractOutputPortNumber(msg);
   if (getStateForOutputPort(portout)!=FREE) return false;
   
   //The we check if no packet is in transit through any VC of the port
   unsigned mux = ((TPZCrossbar&)(getComponent())).getOutputMux();
   unsigned msgtypes = ((TPZCrossbar&)(getComponent())).getNumberMT();
   for (int i=1; i<=(mux/msgtypes); i++) 
   {
      for (int j=1; j<=msgtypes; j++)
      {
         unsigned outVC=getAbsolutValueForOutputVC(portout, i, j);
         if ( getStateForOutputVC(outVC)!=FREE) return false;
      }
   }
   
   //Finally, we check that the stop signal at next router is not active
   unsigned msgtype=msg->getVnet();
   unsigned VCFirstReq;
   if (msg->isOrdered()) VCFirstReq=1;
   else VCFirstReq= getFirstVCReqFor(iPort, portout, msgtype);
   
   unsigned outputVChannel= (msgtype-1)*(mux/msgtypes) + VCFirstReq;
   if (portout == m_outputs) outputVChannel=1;
   if (outputInterfaz(portout)->isStopActive(outputVChannel)) return false; 
       
   //if the three conditions are met, bypass is possible
   //we update necessary control information
   m_InOutVCTable->setValueAt(iPort, VCFirstReq);
   setStateForOutputVC(getAbsolutValueForOutputVC(portout, VCFirstReq, msgtype), ASIGNED);
   setStateForOutputPort(portout, ASIGNED);
   m_InOutPortTable->setValueAt(iPort, portout);
   return true;
}
   
//*************************************************************************

// end of file

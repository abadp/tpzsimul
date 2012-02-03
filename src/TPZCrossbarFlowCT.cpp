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
//    File: TPZCrossbarFlowCT.cpp
//
//    Class:  TPZCrossbarFlowCT
//
//    Inherited from:  TPZCrossbarFlow
// :
//*************************************************************************
//end of header



#include <TPZCrossbarFlowCT.hpp>

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

IMPLEMENT_RTTI_DERIVED(TPZCrossbarFlowCT,TPZCrossbarFlow);

//*************************************************************************
//:
//  f: TPZCrossbarFlowCT ();
//
//  d:
//:
//*************************************************************************

TPZCrossbarFlowCT :: TPZCrossbarFlowCT( TPZComponent& component)
                   : TPZCrossbarFlow(component){
}


//*************************************************************************
//:
//  f: ~ TPZCrossbarFlowCT ();
//
//  d:
//:
//*************************************************************************

TPZCrossbarFlowCT :: ~TPZCrossbarFlowCT()

{   
   
}


//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent (const TPZEvent & event);
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbarFlowCT :: dispatchEvent(const TPZEvent& event)
{
   if( ( event.type() == _CrossbarRequest_ ) ||
       ( event.type() == _CrossbarPass_ ) )
   {
      if( event.type() == _CrossbarRequest_)
      {
         ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::SWArbitration);
      }
      
      unsigned iPort = event.source();
      unsigned oPort = getOutputForInput(iPort).left();
      TPZMessage* msg;
      m_MessageReceivedTable->valueAt(iPort,&msg);
      // If I've sent the message, step 
      if(!msg) return true;
      
      if( ! iPort )
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

      if( (outputInterfaz(oPort)->isStopActive())&&  (msg->isHeader() || msg->isHeadTail()) )
      {
         inputInterfaz(iPort)->sendStop();
         uTIME delayTime = getOwnerRouter().getCurrentTime() + 1;
         getEventQueue().enqueue(event,delayTime);

   #ifndef NO_TRAZA 
      TPZString time = getOwnerRouter().getCurrentTime();       
      TPZString texto = getComponent().asString() + " STOP TIME = ";
      texto += time + msg->asString() + " i=" + TPZString(iPort) +
               ", o=" + TPZString(oPort);
            
      TPZWRITE2LOG( texto );
   #endif
               return true;
      }

      setStateForInput(iPort,Connect);      
    
      inputInterfaz(iPort)->clearStopRightNow();
     
      outputInterfaz(oPort)->sendData(msg);
      ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::SWTraversal);
      if ( ((TPZCrossbar&)getComponent()).getTypeForOutput(oPort,1) != _LocalNode_ )
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

      // I have sent the message so I can remove from the table of messages.
      m_MessageReceivedTable->setValueAt(iPort,(TPZMessage *)0);
   
      if( msg->isTail() || msg->isHeadTail() )
      {
         clearConnection(iPort);
         iPort = getConnectionRequestFor(oPort);
         if( iPort != 0 )
         {
            establishConnection(iPort,oPort);
            TPZEvent requestEvent(_CrossbarRequest_,iPort,oPort);
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
//  f: virtual Boolean onStopUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbarFlowCT :: onStopUp(unsigned interfaz, unsigned cv)
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

Boolean TPZCrossbarFlowCT :: onStopDown(unsigned interfaz, unsigned cv)
{   
      return Inhereited::onStopDown(interfaz,cv);
}
//*************************************************************************


// end of file

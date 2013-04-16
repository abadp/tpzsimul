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
//    File: TPZConsumerFlowReactive.cpp
//
//    Class:  TPZConsumerFlowReactive
//
//    Inherited from:  TPZConsumerFlow
// :
//*************************************************************************
//end of header



#include <TPZConsumerFlowReactive.hpp>

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZConsumer_HPP__
#include <TPZConsumer.hpp>
#endif

#ifndef __TPZFicheroLog_HPP__
#include <TPZFicheroLog.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

#ifndef __TPZConst_H__
#include <TPZConst.hpp>
#endif

#ifndef __TPZGlobalData_HPP__
#include <TPZGlobalData.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

#ifndef __TPZNetwork_HPP__
#include <TPZNetwork.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZConsumerFlowReactive,TPZConsumerFlow);


//*************************************************************************
//:
//  f: TPZConsumerFlow ();
//
//  d:
//:
//*************************************************************************

TPZConsumerFlowReactive :: TPZConsumerFlowReactive( TPZComponent& component)
                 : TPZConsumerFlow(component)
{
   
}


//*************************************************************************
//:
//  f: virtual Boolean stateChange ();
//
//  d:
//:
//*************************************************************************

Boolean TPZConsumerFlowReactive :: stateChange()
{
   if( (!inputInterfaz()->isStopActive()) && m_DataReceived )
   {
#ifndef NO_TRAZA
         TPZString time = getOwnerRouter().getCurrentTime();
         TPZString texto= getComponent().asString() +
                          " Flit Rx. TIME = " +
                          time + ".  " +
                          m_InputData->asString();
#endif
      if(m_State  == Information && m_InputData->isHeader())
      {
         TPZString err;
         err.sprintf( "%s : I detect a Header flit before previous message Tail flit: %s",
                      (char*)getComponent().asString(),
                      (char*)m_InputData->asString() );
         EXIT_PROGRAM(err);
      } 
   

      if( m_State == Header )
      {
         if (m_InputData->isHeadTail()) m_State = Header;
          else m_State = Information;

         if( m_InputData->destiny() != getOwnerRouter().getPosition() && !m_InputData->isMulticast() )
         {
            TPZString err;
            err.sprintf( ERR_TPZCOFLO_001,(char*)getComponent().asString(),(char*)m_InputData->asString() );
            EXIT_PROGRAM(err);
         }
         if(m_InputData->isHeadTail())
         {
#ifndef NO_TRAZA
            texto += "  PACKET RX";
#endif
            m_InputData->setDestiny(getOwnerRouter().getPosition());
            getOwnerRouter().onPacketReceived(m_InputData);
            uTIME lastTime = getOwnerRouter().getCurrentTime();
            ((TPZSimulation*)(getComponent().getSimulation()))->setLastMessage ( lastTime );
         }
      }
          
      if( m_InputData->isTail() )
      {
#ifndef NO_TRAZA
         texto += "  PACKET RX";
#endif
        
         getOwnerRouter().onPacketReceived(m_InputData);
         m_State = Header;
         uTIME lastTime = getOwnerRouter().getCurrentTime();
         ((TPZSimulation*)(getComponent().getSimulation()))->setLastMessage ( lastTime );
        //reinjection of the packet, in order to emulate reactive nature
        //injectProtocolMessage emulates traffic where messages travel back to their source node
        //injectProtocolRandomMessage function can also be employed. In this case, destination of
        //re-injected packet is chosen randomly.
     
        int order = m_InputData-> getVnet ();
        unsigned mess_types= ((TPZSimulation*)(getComponent().getSimulation()))->getProtocolMessTypes();
        if (order < mess_types)
        ((TPZSimulation *)(getComponent().getSimulation()))->injectProtocolMessage(m_InputData->destiny(), m_InputData->source(), order+1);
      
     }
       
#ifndef NO_TRAZA
   TPZWRITE2LOG(texto);
#endif
   }

   return true;
}

//*************************************************************************
// end of file

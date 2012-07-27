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
//    File: TPZRoutingFlowBubble.cpp
//
//    Class:  TPZRoutingFlowBubble
//
//    Inherited from:  TPZRoutingFlow
// :
//*************************************************************************
//end of header



#include <TPZRoutingFlowBubble.hpp>

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZFicheroLog_HPP__
#include <TPZFicheroLog.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

#ifndef __TPZFifoMemory_HPP__
#include <TPZFifoMemory.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZRoutingFlowBubble,TPZRoutingFlow);


//*************************************************************************
//:
//  f: TPZRoutingFlowBubble (TPZComponent & component);
//
//  d:
//:
//*************************************************************************

TPZRoutingFlowBubble :: TPZRoutingFlowBubble( TPZComponent& component)
                      : TPZRoutingFlow(component)
{

}

//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm (Boolean info = false, int delta = 0);
//
//  d:
//:
//*************************************************************************

Boolean TPZRoutingFlowBubble :: controlAlgoritm(Boolean info,int delta)
{
   if( ! changeDirection(m_Message) ) return true;

   TPZROUTINGTYPE newDirection = m_Message->getRoutingPort();
         
   TPZComponentId id( TPZComponentId::FifoMemory, newDirection, "*" );
   TPZComponent* component = getOwnerRouter().componentWithIdLike(id); 
   TPZFifoMemory* fifo = POINTERCAST(TPZFifoMemory,component);
   if( ! fifo )
   {
      TPZString err;
     
      err.sprintf( ERR_TPZRBFLO_002,
                   (char*)getComponent().asString(),
                   (char*)id.asString() );
      EXIT_PROGRAM(err);  
   }
   
   unsigned packet = ((TPZSimulation*)(getComponent().getSimulation()))->
                     getPacketLength((TPZNetwork*)getOwnerRouter().getOwner());

   #ifndef NO_TRAZA
      TPZString texto = TPZString("holes=") + TPZString(fifo->bufferHoles());
      texto += TPZString(" direction=") + TPZString(newDirection) + " yo=" +
               TPZString(getComponent().componentId().componentType());
      TPZWRITE2LOG(texto);
   #endif
   
  return fifo->bubbleReady();
}

//*************************************************************************


// end of file

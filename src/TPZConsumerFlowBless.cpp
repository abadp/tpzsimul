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
//    File: TPZConsumerFlowBless.cpp
//
//    Class: TPZConsumerFlowBless
//
//    Inherited from: TPZConsumerFlow
// :
//*************************************************************************
//end of header


#include <TPZConsumerFlowBless.hpp>

#ifndef __TPZGlobalData_HPP__
#include <TPZGlobalData.hpp>
#endif

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

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

#ifndef __TPZNetwork_HPP__
#include <TPZNetwork.hpp>
#endif
//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZConsumerFlowBless,TPZConsumerFlow);


//*************************************************************************
//:
//  f: TPZConsumerMSIFlow();
//
//  d:
//:
//*************************************************************************

TPZConsumerFlowBless :: TPZConsumerFlowBless( TPZComponent& component)
                 : TPZConsumerFlow(component)
                   
{
   
}


//*************************************************************************
//:
//  f: virtual ~TPZConsumerMSIFlow();
//
//  d:
//:
//*************************************************************************

TPZConsumerFlowBless :: ~TPZConsumerFlowBless()
{
   
}



//*************************************************************************
//:
//  f: virtual Boolean stateChange();
//
//  d:
//:
//*************************************************************************

Boolean TPZConsumerFlowBless :: stateChange()
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
      
      if ( m_InputData->destiny() != getOwnerRouter().getPosition() )
      {
         TPZString err;
         err.sprintf( ERR_TPZCOFLO_001,(char*)getComponent().asString(),(char*)m_InputData->asString() );
         EXIT_PROGRAM(err);
      }
      
      TPZAssemblyQueue::Cursor cursor(m_AssemblyQueue);
      Boolean found=false;
      unsigned AssemblySize=0;
      
      //***********************************************************************
      //Check if a new message is arriving to the assembling queue
      //***********************************************************************
      #ifndef NO_TRAZA
	 TPZString texto2= getComponent().asString() + " Moving through the assembly queue: [ ";
      #endif
      
      forCursor(cursor)
      {
         
	 assemblyPair temp= cursor.element();
	 unsigned long msgTemp= temp.left();
	 #ifndef NO_TRAZA
	 texto2+= TPZString(msgTemp) + ", ";
	 #endif
	 if ( msgTemp == (m_InputData->getIdentifier()) )
	 {
	    //part of an existing message
#ifndef NO_TRAZA
            texto += "  FRAGMENT";
#endif
	    assemblyPair tempNew;
	    tempNew.setLeft(temp.left());
	    tempNew.setRight(temp.right()-1);
	    cursor.remove();
	    m_AssemblyQueue.add(tempNew);
	    found=true;
	    break;
	 }
      }
#ifndef NO_TRAZA
      texto2+= " ]";
      TPZWRITE2LOG(texto2);
#endif
      //**********************************************************************
      // new message
      //**********************************************************************
      if (found==false)
      {
         assemblyPair temp2;
	 temp2.setLeft(m_InputData->getIdentifier());
	 temp2.setRight(m_InputData->packetSize()-1);
	 m_AssemblyQueue.add(temp2);
      }
      //***********************************************************************
      // Any message completed??
      //***********************************************************************
      forCursor(cursor)
      {
         assemblyPair temp3= cursor.element();
	 unsigned remaining=temp3.right();
	 if (remaining==0)
	 {
	    getOwnerRouter().onPacketReceived(m_InputData);
	    uTIME lastTime = getOwnerRouter().getCurrentTime();
            ((TPZSimulation*)(getComponent().getSimulation()))->setLastMessage ( lastTime );
#ifndef NO_TRAZA
            texto += "  PACKET RX";
#endif
	    cursor.remove();
	    break;
	 }
      }
            
   #ifndef NO_TRAZA
         TPZWRITE2LOG(texto);
   #endif
   }
   return true;
}


// end of file

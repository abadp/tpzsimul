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
//    File: TPZConsumerFlow.cpp
//
//    Class:  TPZConsumerFlow
//
//    Inherited from:  TPZFlow
// :
//*************************************************************************
//end of header



#include <TPZConsumerFlow.hpp>

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

IMPLEMENT_RTTI_DERIVED(TPZConsumerFlow,TPZFlow);


//*************************************************************************
//:
//  f: TPZConsumerFlow ();
//
//  d:
//:
//*************************************************************************

TPZConsumerFlow :: TPZConsumerFlow( TPZComponent& component)
                 : TPZFlow(component),
                   m_ReceivingMessage(false),
                   m_DataReceived(false),
                   m_State(Header)
{
   m_InputData=0;
}


//*************************************************************************
//:
//  f: virtual ~ TPZConsumerFlow ();
//
//  d:
//:
//*************************************************************************

TPZConsumerFlow :: ~TPZConsumerFlow()
{
   if(m_InputData)
   {
      m_InputData->setExternalInfo((void*)0);
      
#ifdef PTOPAZ	    
            unsigned n=m_InputData->getPoolIndex();
            getMessagePool(n).release(m_InputData);
#else
            getMessagePool().release(m_InputData);
#endif
   }
}


//*************************************************************************
//:
//  f: virtual void terminate ();
//
//  d:
//:
//*************************************************************************

void TPZConsumerFlow :: terminate()
{

}


//*************************************************************************
//:
//  f: virtual void run (utime Runtime)
//
//  d:
//:
//*************************************************************************

void TPZConsumerFlow :: run(uTIME runTime)
{
   TPZConsumer& consumer = (TPZConsumer&) getComponent();
   if( consumer.getDataDelay() != 0 )
   {
      Inhereited::run(runTime);
   }
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZConsumerFlow :: onReadyUp(unsigned interfaz, unsigned cv)
{
   TPZConsumer& consumer = (TPZConsumer&) getComponent();
   if( consumer.getDataDelay() == 0 )
   {
      Inhereited::run(getOwnerRouter().getCurrentTime());
   }

   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean inputReading ();
//
//  d:
//:
//*************************************************************************

Boolean TPZConsumerFlow :: inputReading()
{

   if( ! inputInterfaz()->isStopActive() )
   {
      if( inputInterfaz()->isReadyActive() )
      {
         if(m_InputData) 
         {

            m_InputData->setExternalInfo((void*)0);

#ifdef PTOPAZ	    
           
#ifndef NO_POOL
           (((TPZSimulation*)(getComponent().getSimulation()))->getNetwork())->messageRelease(m_InputData);
#else
           getMessagePool(0).release(m_InputData);
#endif

#else
           getMessagePool().release(m_InputData);
#endif	    
         }

         m_DataReceived = inputInterfaz()->getData(&m_InputData);
      }
      else
      {
         m_DataReceived = false;
      }
   }

   return true;   
}


//*************************************************************************
//:
//  f: virtual Boolean stateChange ();
//
//  d:
//:
//*************************************************************************

Boolean TPZConsumerFlow :: stateChange()
{
   if( (!inputInterfaz()->isStopActive()) && m_DataReceived )
   {
#ifndef NO_TRAZA
      TPZString time = getOwnerRouter().getCurrentTime();
      TPZString texto= getComponent().asString() + " Flit Rx. TIME = " + time + ".  " + m_InputData->asString();
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
      }
        
#ifndef NO_TRAZA
      TPZWRITE2LOG(texto);
#endif
   }

   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean outputWriting ();
//
//  d:
//:
//*************************************************************************

Boolean TPZConsumerFlow :: outputWriting()
{
   // For the moment, we assume that  arrived flits are allways consumed.
   // If in the future you want to simulate some kind of contention at
   // consumption queues, this is the appropiate place.
   
   return true;   
}


//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm (Boolean info = false, int delta = 0);
//
//  d:
//:
//*************************************************************************

Boolean TPZConsumerFlow :: controlAlgoritm(Boolean info, int delta)
{
   // To avoid abstraction. Not used in this class.
   return true;
}



//*************************************************************************


// end of file

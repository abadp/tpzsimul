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
//    File: TPZOutputStageFlow.cpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header

#include <TPZOutputStageFlow.hpp>

#ifndef __TPZOutputStage_HPP__
#include <TPZOutputStage.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZFicheroLog_HPP__
#include <TPZFicheroLog.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

#ifndef __TPZMultiportIOFifo_HPP__
#include <TPZMultiportIOFifo.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZOutputStageFlow,TPZFlow);


//*************************************************************************
//:
//  f: TPZOutputStageFlow(TPZComponent& component);
//
//  d:
//:
//*************************************************************************

TPZOutputStageFlow :: TPZOutputStageFlow(TPZComponent& component)
                   : TPZFlow(component),
                     m_Size(0),
		     m_PortOccupation(0),
                     m_inputs(1),
                     m_memory(0),
                     m_outLast(0),
                     m_payload(0)
                     
{
   
}


//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************

void TPZOutputStageFlow :: initialize()
{

   m_Size = ((TPZOutputStage&)getComponent()).getBufferSize(); 
   m_inputs= ((TPZOutputStage&)getComponent()).numberOfInputs();
   m_memory=new QueueFlits[m_inputs];
   m_PortOccupation=new unsigned[m_inputs];
   
   for (int i=0; i<m_inputs; i++)
   {
      m_PortOccupation[i]=0;
   }   
   Inhereited::initialize();
}

//*************************************************************************
//:
//  f: ~TPZOutputStageFlow();
//
//  d:
//:
//*************************************************************************

TPZOutputStageFlow :: ~TPZOutputStageFlow()
{
   delete[] m_memory;
}

//*************************************************************************
//:
//  f: virtual Boolean inputReading();
//
//  d: Lectura desde varios puertos de entrada
//:
//*************************************************************************

Boolean TPZOutputStageFlow :: inputReading()
{ 
   return true;   
}

//*************************************************************************
//:
//  f: virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
//
//  d: Lectura desde varios puertos de entrada
//:
//*************************************************************************

Boolean TPZOutputStageFlow :: onReadyUp( unsigned interfaz, unsigned cv)
{
   if(! bufferHoles() )
   {
      TPZString err;
      err.sprintf(ERR_TPZFFLOW_001,(char*)getComponent().asString());
      err+="\n Buffer Overflow";
      EXIT_PROGRAM(err);
   }
   
   TPZMessage* msg;
   inputInterfaz(interfaz)->getData(&msg);
   if (msg->isHeader() || msg->isHeadTail())
   {
      m_PortOccupation[interfaz-1]++;
      if ( (msg->isOrdered()) )
      {
	getOwnerRouter().setInOrderFree(msg->getLastDirMiss());
      }
   }
   m_memory[interfaz-1].enqueue(msg);
#ifndef NO_TRAZA
   uTIME delayTime = getOwnerRouter().getCurrentTime() ;
   TPZString texto = getComponent().asString() + " Flit Rx (" + TPZString(delayTime) 
                     + " # " + msg->asString() + ") holes= " + TPZString(bufferHoles());
   TPZWRITE2LOG( texto );
#endif
   
   return true;
}
   
//*************************************************************************
//:
//  f: virtual Boolean stateChange();
//
//  d:
//:
//*************************************************************************

Boolean TPZOutputStageFlow :: stateChange()
{
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean outputWriting();
//
//  d:
//:
//*************************************************************************

Boolean TPZOutputStageFlow :: outputWriting()
{
   TPZMessage*  flitnow;
   outputInterfaz()->clearData();
            
   if(m_payload==1)
   {
      if(m_memory[m_outLast].numberOfElements()!=0 )
      {
         m_memory[m_outLast].dequeue(flitnow);
         if(flitnow->isTail()) m_payload=0;
         sendFlit(flitnow);
      }
   }
   
   else
   {
      if ((! outputInterfaz()->isStopActive()))
      {
         for(int i=0; i<m_inputs; i++)
         {
            unsigned index=(i+m_outLast+1)%(m_inputs);
            if(m_memory[index].numberOfElements()!=0)
            {
               m_outLast=index;
               m_payload=1;
               m_memory[index].dequeue(flitnow);
               if (flitnow->isHeader()) m_PortOccupation[index]--;
	       else if (flitnow->isHeadTail())
	       {
	          m_PortOccupation[index]--;
	          m_payload=0;
	       }
	       sendFlit(flitnow);
               break;
            }         
         }
      }
   } 
   
   //********************************************************************
   // Flow control for each input sepparatedly
   //********************************************************************
   unsigned lpack= ((TPZSimulation*)getComponent().getSimulation())->getPacketLength((TPZNetwork*)getOwnerRouter().getOwner());
   for (int index=1; index<m_inputs+1; index++)
   {
      if ( ((m_Size/m_inputs)-m_memory[index-1].numberOfElements()) <= lpack+1 ) 
      {
         inputInterfaz(index)->sendStopRightNow();
#ifndef NO_TRAZA
         uTIME time = getOwnerRouter().getCurrentTime();
         TPZString texto = getComponent().asString() + ": Full Buffer->Stop Interfaz " + TPZString(index);
         texto += TPZString(time);
         TPZWRITE2LOG(texto);
#endif
      }
      else 
      {
         inputInterfaz(index)->clearStopRightNow();
      }
   }
      
   return true;   
}


//*************************************************************************
//:
//  f: virtual Boolean propagateStop();
//
//  d:
//:
//*************************************************************************

Boolean TPZOutputStageFlow :: propagateStop()
{
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean TPZOutputStageFlow :: onStopUp(unsigned interfaz, unsigned cv)
{
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean TPZOutputStageFlow :: onStopDown(unsigned interfaz, unsigned cv)
{
   return true;
}


//*************************************************************************
//:
//  f: virtual void sendFlit(const TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

void TPZOutputStageFlow :: sendFlit(TPZMessage* msg)
{
   TPZROUTINGTYPE myDirection = getComponent().getRoutingDirection();
   ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::OStageTraversal);
   
   if (myDirection != _LocalNode_)
   {
      ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::LinkTraversal);
   }
   
   outputInterfaz()->sendData(msg);
#ifndef NO_TRAZA
   uTIME delayTime = getOwnerRouter().getCurrentTime() ;
   TPZString texto = getComponent().asString() + " Flit Tx (" + TPZString(delayTime) 
                     + " # " + msg->asString() + ") holes= " + TPZString(bufferHoles());
   TPZWRITE2LOG( texto );
#endif
}

//*************************************************************************
unsigned TPZOutputStageFlow::bufferElements() const
{ 
   unsigned count=0;
   for(int index=0; index<m_inputs;index++)
   {
      count=m_memory[index].numberOfElements()+count;
   }
   return count; 
}

//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
//
//  d:
//:
//*************************************************************************

Boolean TPZOutputStageFlow :: controlAlgoritm(Boolean info, int delta)
{
   return true;
}

//*************************************************************************
//:
//  f: Boolean controlReplicationAlgoritm(unsigned index);
//
//  d:
//:
//*************************************************************************

Boolean TPZOutputStageFlow :: controlReplicationAlgorithm( unsigned index)
{
   TPZROUTINGTYPE Direction=getComponent().getRoutingDirection();
   if ( Direction ==_LocalNode_ ) return true;
   else
   {
      if ( m_PortOccupation[index-1] == 0 ) return true;
      else return false;
   }
}

// end of file

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
//    File: TPZMultiportFifoFlow.cpp
//
//    Class:  TPZMultiportFifoFlow
//
//    Inherited from:  TPZFlow
// :
//*************************************************************************
//end of header



#include <TPZMultiportFifoFlow.hpp>

#ifndef __TPZMultiportFifo_HPP__
#include <TPZMultiportFifo.hpp>
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

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZMultiportFifoFlow,TPZFlow);

//*************************************************************************
//:
//  f: inline unsigned bufferSize () const;
//  d:
//
//
//  f: inline unsigned bufferHoles () const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: TPZMultiportFifoFlow (TPZComponent & component);
//
//  d:
//:
//*************************************************************************

TPZMultiportFifoFlow :: TPZMultiportFifoFlow(TPZComponent& component)
                   : TPZFlow(component),
                     m_Size(0),
                     m_inputs(1),
                     m_SendStop(false),
                     m_memory(0),
                     m_payload(0),
		     m_outLast(0)
                     
                     
{
   
}


//*************************************************************************
//:
//  f: virtual void initialize ();
//
//  d:
//:
//*************************************************************************

void TPZMultiportFifoFlow :: initialize()
{

   m_Size = ((TPZMultiportFifo&)getComponent()).getBufferSize(); 
   m_inputs= ((TPZMultiportFifo&)getComponent()).numberOfInputs();
   m_memory=new QueueFlits[m_inputs];   
   Inhereited::initialize();
}

//*************************************************************************
//:
//  f: ~TPZMultiportFifoFlow();
//
//  d:
//:
//*************************************************************************

TPZMultiportFifoFlow :: ~TPZMultiportFifoFlow()
{
   delete[] m_memory;
}

//*************************************************************************
//:
//  f: virtual Boolean inputReading ();
//
//  d: Reading from multiple input ports
//:
//*************************************************************************

Boolean TPZMultiportFifoFlow :: inputReading()
{ 
   for(int i=1;i<m_inputs+1;i++)
   {  
      if( inputInterfaz(i)->isReadyActive() )
      {
         if( ! bufferHoles() )
         {
            TPZString err;
            err.sprintf(ERR_TPZFFLOW_001,(char*)getComponent().asString());
            err+="\n More flits than those allowed by memory capacity";
            EXIT_PROGRAM(err);
         }

         TPZMessage* msg;
         inputInterfaz(i)->getData(&msg);    
         m_memory[i-1].enqueue(msg);
      
#ifndef NO_TRAZA
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         TPZString texto = getComponent().asString() + " Flit Rx (" + TPZString(delayTime); 
         texto += TPZString(" # ") + msg->asString() + ") holes= " + TPZString(bufferHoles());
         TPZWRITE2LOG( texto );
#endif
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

Boolean TPZMultiportFifoFlow :: stateChange()
{
   if( controlAlgoritm(false,m_inputs)==false ) m_SendStop=true;
   else m_SendStop=false;

   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean outputWriting ();
//
//  d:
//:
//*************************************************************************

Boolean TPZMultiportFifoFlow :: outputWriting()
{
   TPZMessage* flitnow;
   outputInterfaz()->clearData();
   //First we check if there is any message in transit         
   if(m_payload==1)
   {
      if(m_memory[m_outLast].numberOfElements()!=0 )
      {
         m_memory[m_outLast].dequeue(flitnow);
         if(flitnow->isTail() || flitnow->isHeadTail() )
         {
            //Last flit of the packet. The next to look elsewhere
            m_payload=0;
         }
         sendFlit(flitnow);
      }
   }
   //Now we can look for a new candidate to leave    
   //In this case, the selection allways starts with index=0, but it could
   //be changed to implement Round Robin or priority easily.
   else
   {
      for(int index=0; index<m_inputs; index++)
      {
         if(m_memory[index].numberOfElements()!=0)
         {
            m_payload=1;
            m_memory[index].dequeue(flitnow);
            sendFlit(flitnow);
            break;
         }         
      }
   } 
   //Check buffer occupation to generate the appropriate
   //control signals (STOP).     
   if(!controlAlgoritm(false,m_inputs+1))
   {
      for(int index=1;index<m_inputs+1;index++)
         {
            inputInterfaz(index)->sendStop();
         }
#ifndef NO_TRAZA
         uTIME time = getOwnerRouter().getCurrentTime();
         TPZString texto = getComponent().asString() + ": Full Buffer->Stop ";
         texto += TPZString(time);
         TPZWRITE2LOG(texto);
#endif
   }
   else
   {
      for(int index=1;index<m_inputs+1;index++)
         {
            inputInterfaz(index)->clearStop(); 
         }
        
   }
   return true;   
}



//*************************************************************************
//:
//  f: virtual Boolean propagateStop ();
//
//  d:
//:
//*************************************************************************

Boolean TPZMultiportFifoFlow :: propagateStop()
{
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZMultiportFifoFlow :: onStopUp(unsigned interfaz, unsigned cv)
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

Boolean TPZMultiportFifoFlow :: onStopDown(unsigned interfaz, unsigned cv)
{
   return true;
}


//*************************************************************************
//:
//  f: virtual void sendFlit (const TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

void TPZMultiportFifoFlow :: sendFlit(TPZMessage* msg)
{
   outputInterfaz()->sendData(msg);
#ifndef NO_TRAZA
   uTIME delayTime = getOwnerRouter().getCurrentTime() ;
   TPZString texto = getComponent().asString() + " Flit Tx (";
   texto += TPZString(delayTime) + " # " + msg->asString();
   texto += TPZString(") holes= ") + TPZString(bufferHoles());
   TPZWRITE2LOG( texto );
#endif
}

//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm (Boolean info = false, int delta = 0);
//
//  d:
//:
//*************************************************************************

Boolean TPZMultiportFifoFlow :: controlAlgoritm(Boolean info, int delta)
{
   unsigned sizeForCT = ((TPZSimulation*)getComponent().getSimulation())->
                               getPacketLength((TPZNetwork*)getOwnerRouter().getOwner());   
   //Utlize the delta to pass the number of entries in buffer
   return ( bufferHoles() > delta*sizeForCT  ) ? true : false;
}
//*************************************************************************
//:
//  f: virtual Boolean bubbleReady () const;
//  d: semifull control (used in bubble's fifo)
//:
//*************************************************************************

Boolean TPZMultiportFifoFlow ::bubbleReady()const
{     
   unsigned sizeForCT = ((TPZSimulation*)getComponent().getSimulation())->
                        getPacketLength((TPZNetwork*)getOwnerRouter().getOwner());    
   return ( bufferHoles() >= (m_inputs+1)*sizeForCT ) ? true : false;
}   

//*************************************************************************
//:
//  f: unsigned bufferElements() const;
//  d: semifull control (used in bubble's fifo)
//:
//*************************************************************************

unsigned TPZMultiportFifoFlow::bufferElements() const
{ 
   unsigned count=0;
   for(int index=0; index<m_inputs;index++)
   {
      count=m_memory[index].numberOfElements()+count;
   }
   return count; 
}
//*************************************************************************
// end of file

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
//    File: TPZMultiportIOFifoFlow.cpp
//
//    Class:  TPZMultiportIOFifoFlow
//
//    Inherited from:  TPZFlow
// :
//*************************************************************************
//end of header

#include <TPZMultiportIOFifoFlow.hpp>

#ifndef __TPZMultiportIOFifo_HPP__
#include <TPZMultiportIOFifo.hpp>
#endif

#ifndef __TPZMultiportFifo_HPP__
#include <TPZMultiportFifo.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

#ifndef __TPZNetwork_HPP__
#include <TPZNetwork.hpp>
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

IMPLEMENT_RTTI_DERIVED(TPZMultiportIOFifoFlow, TPZFlow);

//*************************************************************************
//:
//  f: TPZMultiportIOFifoFlow(TPZComponent& component);
//
//  d:
//:
//*************************************************************************

TPZMultiportIOFifoFlow :: TPZMultiportIOFifoFlow(TPZComponent& component)
	:TPZFlow(component),
	m_Size(0),
	m_inputs(1),
	m_outputs(1),
	m_ocupado(false),
	m_inOcupado(false),
	m_lowocupation(true),
	m_memory(0),
	m_NumOfPackets(0),
	m_inMemSelected(0),
	m_outPortSelected(0),
	m_sizeInPackets(0),
	m_missroutingLimit(0)
	
{
  
}

//*************************************************************************
//:
//  f: ~TPZMultiportIOFifoFlow();
//
//  d:
//:
//*************************************************************************
TPZMultiportIOFifoFlow :: ~TPZMultiportIOFifoFlow()
{
   delete[] m_memory;
}
//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************
   
void TPZMultiportIOFifoFlow :: initialize()
{
   m_Size = ((TPZMultiportIOFifo&)getComponent()).getBufferSize();
   m_inputs = ((TPZMultiportIOFifo&)getComponent()).numberOfInputs();
   m_outputs = ((TPZMultiportIOFifo&)getComponent()).numberOfOutputs();
   m_missroutingLimit = ((TPZMultiportIOFifo&)getComponent()).getMissLimit();
   m_memory = new QueueFlits[m_inputs];
   m_ocupado = new Boolean[m_outputs];
   m_inOcupado = new Boolean[m_inputs];
   m_inMemSelected = new unsigned[m_inputs];
   m_outPortSelected = new unsigned[m_outputs];
   for (int i=0; i<m_outputs; i++)
      {
      m_ocupado[i]=false;
      m_outPortSelected[i]=0;
      }
   for (int j=0; j<m_inputs; j++)
      {
      m_inOcupado[j]=false;
      m_inMemSelected[j]=0;
      }
      
   Inhereited::initialize();
}
   


//*************************************************************************
//:
//   f: virtual void postInitialize();
//
//   d:
//:
//*************************************************************************

void TPZMultiportIOFifoFlow :: postInitialize()
{
   
   m_inputDirection=getComponent().getRoutingDirection();
   TPZString texto=getComponent().getName();
   TPZString Name_next_mp, Name_out_stage;
   
   if (texto==TPZString("MPR1"))
   {
      m_outputDirection= _LocalNode_;
      Name_next_mp= TPZString("MPR5");
      Name_out_stage= TPZString("OS5");
   }
   else if (texto==TPZString("MPR2"))
   {
      m_outputDirection= _Xplus_;
      Name_next_mp= TPZString("MPR3");
      Name_out_stage= TPZString("OS1");
   } 
   else if (texto==TPZString("MPR3"))
   {
      m_outputDirection= _Yplus_;
      Name_next_mp= TPZString("MPR4");
      Name_out_stage= TPZString("OS2");
   }
   else if (texto==TPZString("MPR4"))
   {
      m_outputDirection= _Xminus_;
      Name_next_mp= TPZString("MPR1");
      Name_out_stage= TPZString("OS3");
   }
   else if (texto==TPZString("MPR5"))
   {
      m_outputDirection= _Yminus_;
      Name_next_mp= TPZString("MPR2");
      Name_out_stage= TPZString("OS4");
   }
   else if (texto==TPZString("MPL1"))
   {
      m_outputDirection= _Xplus_;
      Name_next_mp= TPZString("MPL2");
      Name_out_stage= TPZString("OS1");
   }
   else if (texto==TPZString("MPL2"))
   {
      m_outputDirection= _Yminus_;
      Name_next_mp= TPZString("MPL3");
      Name_out_stage= TPZString("OS4");
   }
   else if (texto==TPZString("MPL3"))
   {
      m_outputDirection= _LocalNode_;
      Name_next_mp= TPZString("MPL5");
      Name_out_stage= TPZString("OS5");
   }
   else if (texto==TPZString("MPL4"))
   {
      m_outputDirection= _Yplus_;
      Name_next_mp= TPZString("MPL1");
      Name_out_stage= TPZString("OS2");
   }
   else if (texto==TPZString("MPL5"))
   {
      m_outputDirection= _Xminus_;
      Name_next_mp= TPZString("MPL4");
      Name_out_stage= TPZString("OS3");
   }
   else
   {
      TPZString err;
      err.sprintf("Bad naming for Rotary multiports");
      EXIT_PROGRAM(err);
   }
   
   TPZComponent* compoSalida = getOwnerRouter().componentWithName(Name_out_stage);
   TPZComponent* compoMulti = getOwnerRouter().componentWithName(Name_next_mp);
   m_NextMulti=POINTERCAST(TPZMultiportIOFifo, compoMulti);
   m_OutMux=POINTERCAST(TPZMultiportFifo, compoSalida);
}

//*************************************************************************
//:
//  f: virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
//
//  d: Lectura desde varios puertos de entrada
//:
//*************************************************************************

Boolean TPZMultiportIOFifoFlow :: onReadyUp( unsigned interfaz, unsigned cv)
{
   /*uTIME delayTime= getOwnerRouter().getCurrentTime();
   TPZMessage* msg;
   unsigned lpack= ((TPZSimulation*)getComponent().getSimulation())->getPacketLength((TPZNetwork*)getOwnerRouter().getOwner());
   unsigned sizeInPackets=m_Size/lpack;
   if( !bufferHoles() || (m_NumOfPackets > sizeInPackets) )
   {
      TPZString err;
      cerr << "Buffer Holes=" << bufferHoles() << endl;
      cerr << "Num Packs=" << m_NumOfPackets << endl;
      err.sprintf("Multiport Buffer Overflow");
      EXIT_PROGRAM(err);
   }
      
   inputInterfaz(interfaz)->getData(&msg);
#ifndef NO_TRAZA
   TPZString texto=getComponent().asString() + "Flit Rx ( Time=" + TPZString(delayTime) + " # " + msg->asString() 
                   + ") holes=" + TPZString(bufferHoles());
   TPZWRITE2LOG( texto );
#endif
   
   if (msg->isHeader() || msg->isHeadTail())
   {
      msg->setDelayMultiport(delayTime+getDataDelay());
      msg->incrHopCount();
      m_NumOfPackets++;
      if (interfaz==1) getOwnerRouter().incrContador(m_inputDirection);
      if ( (msg->getHopCount()> m_missroutingLimit) && (!(msg->isOrdered())) )
      {
         msg->setLastMissRouted(true);
      }
      unsigned memIndex=getInputMem();
      m_memory[memIndex].enqueue(msg);
      if (msg->isHeader()) m_inOcupado[memIndex]=true;
      m_inMemSelected[interfaz-1]=memIndex;      
   }
   else
   {
      m_memory[m_inMemSelected[interfaz-1]].enqueue(msg);
      if (msg->isTail()) m_inOcupado[m_inMemSelected[interfaz-1]]=false;
   }*/
   return true;
}
    
//*************************************************************************
//:
//  f: void getInputMem();
//
//  d: 
//:
//*************************************************************************
unsigned TPZMultiportIOFifoFlow :: getInputMem()
{
   if ( m_inOcupado[0]==true && m_inOcupado[1]==false ) return 1;
   else if ( m_inOcupado[0]==false && m_inOcupado[1]==true ) return 0;
   else if ( m_inOcupado[0]==false && m_inOcupado[1]==false )
   {
      if ( (m_memory[0].numberOfElements()) > (m_memory[1].numberOfElements()) ) return 1;
      else return 0;
   }
   else if ( m_inOcupado[0]==true && m_inOcupado[1]==true )
   {
      TPZString err;
      err.sprintf("Two messages in transit and a third one arrives, not allowed");
      EXIT_PROGRAM(err);
   }
}   

//*************************************************************************
//:
//  f: virtual Boolean inputReading();
//
//  d: Lectura desde varios puertos de entrada
//:
//*************************************************************************
   
Boolean TPZMultiportIOFifoFlow :: inputReading()
{
   for(int interfaz=1;interfaz<m_inputs+1;interfaz++)
   {
      if( inputInterfaz(interfaz)->isReadyActive() )
      {
         uTIME delayTime= getOwnerRouter().getCurrentTime();
         TPZMessage* msg;
         unsigned lpack= ((TPZSimulation*)getComponent().getSimulation())->getPacketLength((TPZNetwork*)getOwnerRouter().getOwner());
         unsigned sizeInPackets=m_Size/lpack;
         if( !bufferHoles() || (m_NumOfPackets > sizeInPackets) )
         {
            TPZString err;
            cerr << "Buffer Holes=" << bufferHoles() << endl;
            cerr << "Num Packs=" << m_NumOfPackets << endl;
            err.sprintf("Multiport Buffer Overflow");
            EXIT_PROGRAM(err);
         }
      
         inputInterfaz(interfaz)->getData(&msg);
#ifndef NO_TRAZA
         TPZString texto=getComponent().asString() + "Flit Rx ( Time=" + TPZString(delayTime) + " # " + msg->asString() 
                   + ") holes=" + TPZString(bufferHoles());
         TPZWRITE2LOG( texto );
#endif
   
         if (msg->isHeader() || msg->isHeadTail())
         {
            msg->setDelayMultiport(delayTime+getDataDelay());
            msg->incrHopCount();
            m_NumOfPackets++;
            if (interfaz==1) getOwnerRouter().incrContador(m_inputDirection);
            if ( (msg->getHopCount()> m_missroutingLimit) && (!(msg->isOrdered())) )
            {
               msg->setLastMissRouted(true);
            }
            unsigned memIndex=getInputMem();
            m_memory[memIndex].enqueue(msg);
            if (msg->isHeader()) m_inOcupado[memIndex]=true;
            m_inMemSelected[interfaz-1]=memIndex;      
         }
         else
         {
            m_memory[m_inMemSelected[interfaz-1]].enqueue(msg);
            if (msg->isTail()) m_inOcupado[m_inMemSelected[interfaz-1]]=false;
         }
      }
   }
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean stateChange();
//
//  d:
//:
//*************************************************************************
   
Boolean TPZMultiportIOFifoFlow :: stateChange()
{
   unsigned lpack= ((TPZSimulation*)getComponent().getSimulation())->getPacketLength((TPZNetwork*)getOwnerRouter().getOwner());
   unsigned sizeInPackets=m_Size/lpack;
   if (getOwnerRouter().getContador(m_inputDirection)>=6) m_lowocupation=true;
   else if (getOwnerRouter().getContador(m_inputDirection) < 4) m_lowocupation=false;
      
   if (m_NumOfPackets >= sizeInPackets-1) inputInterfaz(2)->sendStopRightNow();
   else inputInterfaz(2)->clearStop();
      
   if (m_inputDirection==_LocalNode_)
   {
      if (m_NumOfPackets >= sizeInPackets-3) inputInterfaz(1)->sendStopRightNow();
      else inputInterfaz(1)->clearStop();
   }
   else
   {
      if (m_lowocupation==true)
      {
         if (m_NumOfPackets >= sizeInPackets-2) inputInterfaz(1)->sendStopRightNow();
	 else inputInterfaz(1)->clearStop();
      }
      else
      {
         if (m_NumOfPackets >= sizeInPackets-3) inputInterfaz(1)->sendStopRightNow();
	 else inputInterfaz(1)->clearStop();
      }
   }
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean outputWriting();
//
//  d:
//:
//*************************************************************************
   
Boolean TPZMultiportIOFifoFlow :: outputWriting()
{
   unsigned lpack= ((TPZSimulation*)getComponent().getSimulation())->getPacketLength((TPZNetwork*)getOwnerRouter().getOwner());
   unsigned sizeInPackets=m_Size/lpack;
   
   //**********************************************************************
   //--Arbitration of both output ports
   //**********************************************************************
   Boolean Limpiar_ocupado_uno=false;
   Boolean Limpiar_ocupado_dos=false;
   for(int j=1; j<=m_outputs; j++)
   {
      outputInterfaz(j)->clearData();
   }
   
   unsigned offset=rand()%2;
   for(int j=1; j<=m_inputs; j++)
   {
      int i=((j+offset)%2)+1;
      TPZMessage* flitnow;
            
      if (m_memory[i-1].numberOfElements() == 0)
      {
         continue;
      }
      
      m_memory[i-1].firstElement(flitnow);
      
      //**********************************************************************
      //--Found a header flit
      //**********************************************************************
      if (flitnow->isHeader() || flitnow->isHeadTail())
      {
         if( flitnow->getDelayMultiport()>getOwnerRouter().getCurrentTime() )
	 {
	    continue;
	 }
	 //First we try to request access to the Output Stage	 
	 Boolean isValidOut=checkHeader(flitnow, 1);
	 if ( (!outputInterfaz(1)->isStopActive()) && (m_ocupado[0]==false) && (isValidOut==true) )
	 {
	    m_ocupado[0]=true;
            if (flitnow->isHeadTail()) Limpiar_ocupado_uno=true;
	    m_outPortSelected[i-1]=1;
	    m_memory[i-1].dequeue(flitnow);
	    sendFlit(flitnow, 1);
#ifndef NO_TRAZA
            uTIME delayTime=getOwnerRouter().getCurrentTime();
            TPZString texto=getComponent().asString() + "Flit Tx ( Time=" + TPZString(delayTime) 
	                    + " # " + flitnow->asString() + ") holes=" + TPZString(bufferHoles());
            TPZWRITE2LOG( texto );
#endif
            getOwnerRouter().decrContador(flitnow->getLastDirMiss());
            m_NumOfPackets--;
         }
	 //Second we try to obtain access to next multiport
	 else if ( (!outputInterfaz(2)->isStopActive()) && (m_ocupado[1]==false) )
	 {
	    Boolean newValidOut=checkHeader(flitnow, 2);
	    if (newValidOut==true)
	    {
	       m_ocupado[1]=true;
	       if (flitnow->isHeadTail()) Limpiar_ocupado_dos=true;
	       m_outPortSelected[i-1]=2;
	       m_memory[i-1].dequeue(flitnow);
               sendFlit(flitnow, 2);
#ifndef NO_TRAZA
               uTIME delayTime=getOwnerRouter().getCurrentTime();
               TPZString texto2=getComponent().asString() + "Flit Tx ( Time=" + TPZString(delayTime) 
	                        + " # " + flitnow->asString() + ") holes=" + TPZString(bufferHoles());
               TPZWRITE2LOG( texto2 );
#endif
	       m_NumOfPackets--;
	    }
	 }
      }
      //**********************************************************************
      //--Found a body or tail flit
      //**********************************************************************
      else
      {
         unsigned interfaz=m_outPortSelected[i-1];
	 m_memory[i-1].dequeue(flitnow);
	 sendFlit(flitnow, interfaz);
#ifndef NO_TRAZA
         uTIME delayTime=getOwnerRouter().getCurrentTime();
         TPZString texto3=getComponent().asString() + "Flit Tx ( Time=" + TPZString(delayTime) 
	                  + " # " + flitnow->asString() + ") holes=" + TPZString(bufferHoles());
         TPZWRITE2LOG( texto3 );
#endif
         if (flitnow->isTail())
	 {
	    if (interfaz==1) Limpiar_ocupado_uno=true;
	    if (interfaz==2) Limpiar_ocupado_dos=true;
	 }
      }
   }
   if (Limpiar_ocupado_uno==true) m_ocupado[0]=false;
   if (Limpiar_ocupado_dos==true) m_ocupado[1]=false;

   //*************************************************************************
   //--Flow Control (only valid for Rotary)
   //*************************************************************************
   /*if (getOwnerRouter().getContador(m_inputDirection)>=6) m_lowocupation=true;
   else if (getOwnerRouter().getContador(m_inputDirection) < 4) m_lowocupation=false;
      
   if (m_NumOfPackets > sizeInPackets-1) inputInterfaz(2)->sendStopRightNow();
   else inputInterfaz(2)->clearStop();
      
   if (m_inputDirection==_LocalNode_)
   {
      if (m_NumOfPackets >= sizeInPackets-3) inputInterfaz(1)->sendStopRightNow();
      else inputInterfaz(1)->clearStop();
   }
   else
   {
      if (m_lowocupation==true)
      {
         if (m_NumOfPackets > sizeInPackets-2) inputInterfaz(1)->sendStopRightNow();
	 else inputInterfaz(1)->clearStop();
      }
      else
      {
         if (m_NumOfPackets > sizeInPackets-3) inputInterfaz(1)->sendStopRightNow();
	 else inputInterfaz(1)->clearStop();
      }
   }*/

return true;
}

//*************************************************************************
//:
//  f: virtual Boolean checkHeader(TPZMessage* flitnow, unsigned interfaz);
//
//  d:
//:
//*************************************************************************
Boolean TPZMultiportIOFifoFlow :: checkHeader(TPZMessage* flitnow, unsigned interfaz)
{
   Boolean isValidOut;
   
   if (interfaz==1)
   {
      int deltaX = flitnow->delta(0);
      int deltaY = flitnow->delta(1);
      
      if (flitnow->isOrdered())
      {
         switch( m_outputDirection )
	 {
	    case _Xplus_ :
	       isValidOut = (deltaX > 1)? true : false; break;
	    case _Xminus_ :
	       isValidOut = (deltaX < -1)? true : false; break;
	    case _Yplus_ :
	       isValidOut = (((deltaX==1)||(deltaX==-1))&&(deltaY > 1))? true : false; break;
	    case _Yminus_ :
	       isValidOut = (((deltaX==1)||(deltaX==-1))&&(deltaY < -1))? true : false; break;
	    case _LocalNode_ :
	       isValidOut = ( abs(deltaX)==1 && abs(deltaY)==1 )? true : false; break;
         }	
      }
      else
      {
         switch( m_outputDirection )
         {
            case _Xplus_  : 
            isValidOut = (deltaX > 1)? true : false; break;
            case _Xminus_ : 
            isValidOut = (deltaX < -1)? true : false; break;
            case _Yplus_  : 
            isValidOut = (deltaY > 1)? true : false; break;
            case _Yminus_ : 
            isValidOut = (deltaY < -1)? true : false; break;         
            case _LocalNode_ :
            isValidOut = ( abs(deltaX)==1 && abs(deltaY)==1 )? true : false; break;
         }
      
         if (flitnow->getLastMissRouted()==true && m_outputDirection != _LocalNode_ )
         {
            isValidOut = true;
         }         
      }
      
   }
   else if (interfaz==2)
   {
      if ((m_NextMulti->bufferHoles()) >= bufferHoles())
      {
         isValidOut = true;
      }
      else
      {
         isValidOut = false;
      }
   }
   else
   {
      TPZString err;
      err.sprintf("Out of range interfaz");
      EXIT_PROGRAM(err);
   }
   
   return isValidOut;
}

//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm();
//
//  d:
//:
//*************************************************************************
   
Boolean TPZMultiportIOFifoFlow :: controlAlgoritm(Boolean info, int delta)
{
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean propagateStop();
//
//  d:
//:
//*************************************************************************
   
Boolean TPZMultiportIOFifoFlow :: propagateStop()
{
   return true;
}
   
//*************************************************************************
//:
//  f: virtual Boolean onStopUp();
//
//  d:
//:
//*************************************************************************

Boolean TPZMultiportIOFifoFlow :: onStopUp(unsigned interfaz, unsigned cv)
{
   return true;
}
   
//*************************************************************************
//:
//  f: virtual Boolean onStopDown();
//
//  d:
//:
//*************************************************************************

Boolean TPZMultiportIOFifoFlow :: onStopDown(unsigned interfaz, unsigned cv)
{
   return true;
}
   
//*************************************************************************
//:
//  f: virtual void sendFlit();
//
//  d:
//:
//*************************************************************************

void TPZMultiportIOFifoFlow :: sendFlit(TPZMessage* msg, unsigned interfaz)
{
   ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::MPTraversal);
   outputInterfaz(interfaz)->sendData(msg);
}

//*************************************************************************
//:
//  f: unsigned bufferElements();
//
//  d:
//:
//*************************************************************************
   
unsigned TPZMultiportIOFifoFlow :: bufferElements() const
{
   unsigned acum=0;
   for (int i=0; i<m_inputs; i++)
      {
      acum=m_memory[i].numberOfElements()+acum;
      }
   return acum;
}

// fin del fichero

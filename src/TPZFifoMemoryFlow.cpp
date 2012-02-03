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
//    File: TPZFifoMemoryFlow.cpp
//
//    Class:  TPZFifoMemoryFlow
//
//    Inherited from:  TPZFlow
// :
//*************************************************************************
//end of header



#include <TPZFifoMemoryFlow.hpp>

#ifndef __TPZFifoMemory_HPP__
#include <TPZFifoMemory.hpp>
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

#ifndef __TPZNetwork_HPP__
#include <TPZNetwork.hpp>
#endif
//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZFifoMemoryFlow,TPZFlow);

//*************************************************************************
//:
//  f: inline unsigned bufferSize () const;
//  d:
//
//  f: inline unsigned bufferElements () const;
//  d:
//
//  f: inline unsigned bufferHoles () const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: TPZFifoMemoryFlow (TPZComponent & component);
//
//  d:
//:
//*************************************************************************

TPZFifoMemoryFlow :: TPZFifoMemoryFlow(TPZComponent& component)
                   : TPZFlow(component),
                     m_Size(0),
                     m_SendStop(false),
                     m_tail(true),
             m_ControlTail(false)

{

}


//*************************************************************************
//:
//  f: virtual void initialize ();
//
//  d:
//: 
//*************************************************************************

void TPZFifoMemoryFlow :: initialize()
{
   m_Size = ((TPZFifoMemory&)getComponent()).getBufferSize();   
   Inhereited::initialize();
}


//*************************************************************************
//:
//  f: virtual Boolean inputReading ();
//
//  d:
//:
//*************************************************************************
 
Boolean TPZFifoMemoryFlow :: inputReading()
{  
   TPZMessage* msg;
   inputInterfaz()->getData(&msg); 

   if(!inputInterfaz()->isReadyActive() && m_tail==false)
   {
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         TPZString err="A message queue was expected but transmission has been interrupted";
         err=err+msg->asString()+ "Time crash=" + TPZString(delayTime);
         EXIT_PROGRAM(err);
   }
    
   if( inputInterfaz()->isReadyActive() )
   {
      ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::BufferWrite);
      msg->incDistance();
      if(msg->isTail() || msg->isHeadTail()) 
      {
         m_tail=true;
     setControlTail(true);
      }
      
      if(msg->isHeader() || msg->isHeadTail())
      {
         getOwnerRouter().incrContadorTotal();
      }
            
      if(msg->isHeader() && m_tail==false)
      {
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         TPZString err="A header flit arrives before previous tail flit: crossbar blocked";
         err=err+msg->asString()+ "Time crash=" + TPZString(delayTime);
         EXIT_PROGRAM(err);
      }
   
      if( ! bufferHoles())
      {
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         TPZString err;
         err.sprintf(ERR_TPZFFLOW_001,(char*)getComponent().asString());
         err=err+msg->asString()+ "Time crash=" + TPZString(delayTime);
         EXIT_PROGRAM(err);
      }
      
      
       uTIME delay = getOwnerRouter().getCurrentTime() + 
                       getDataDelay();
       TPZEvent event(_BufferData_,msg);
       getEventQueue().enqueue(event,delay);

#ifndef NO_TRAZA
       uTIME delayTime = getOwnerRouter().getCurrentTime() ;
       TPZString texto = getComponent().asString() + " Flit Rx on Time" +
                         TPZString(delayTime) + " # " + msg->asString() +
                        ") holes= " + TPZString(bufferHoles());
       TPZWRITE2LOG( texto );
#endif

   
      stateChange();
      if(m_SendStop)
      {
          inputInterfaz()->sendStopRightNow();
#ifndef NO_TRAZA
           uTIME time = getOwnerRouter().getCurrentTime();
           TPZString texto = getComponent().asString() + ": Full Buffer->Stop ";
           texto += TPZString(time);
           TPZWRITE2LOG(texto);
#endif
     }
     else
     {
        inputInterfaz()->clearStop();   
     }
      return true;   
   }
   return false;
}


//*************************************************************************
//:
//  f: virtual Boolean stateChange ();
//
//  d:
//:
//*************************************************************************

Boolean TPZFifoMemoryFlow :: stateChange()
{
   if( controlAlgoritm()==false )
   {
      m_SendStop=true;
      inputInterfaz()->sendStopRightNow();
   }
       
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

Boolean TPZFifoMemoryFlow :: outputWriting()
{
   outputInterfaz()->clearData();
   
   if( ! outputInterfaz()->isStopActive() )
   {
      uTIME timeStamp;
      if( getEventQueue().firstElementTimeStamp(timeStamp) )
      {
         if( timeStamp <= getOwnerRouter().getCurrentTime() )
         {
            TPZEvent event;
            getEventQueue().dequeue(event);
            dispatchEvent(event);
         }
      }
   }      
      
   if(m_SendStop)
   {
        inputInterfaz()->sendStopRightNow();
      #ifndef NO_TRAZA
         uTIME time = getOwnerRouter().getCurrentTime();
         TPZString texto = getComponent().asString() + ": Full Buffer->Stop ";
         texto += TPZString(time);
         TPZWRITE2LOG(texto);
      #endif
   }
   else
   {
      inputInterfaz()->clearStop();   
   }
   return true;   
}


//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent (const TPZEvent & event);
//
//  d:
//:
//*************************************************************************

Boolean TPZFifoMemoryFlow :: dispatchEvent(const TPZEvent& event)
{
   if( event.type() == _BufferData_ )
   {
      TPZMessage* msg ;
      msg = event.message();
      sendFlit(msg);

      #ifndef NO_TRAZA
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         TPZString texto = getComponent().asString() + " Flit Tx (";
         texto += TPZString(delayTime) + " # " + msg->asString() +
                  ") holes= " + TPZString(bufferHoles());
         TPZWRITE2LOG( texto );
      #endif
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

Boolean TPZFifoMemoryFlow :: propagateStop()
{
   if( controlAlgoritm() )
   {
      // In this case, the buffer has room for more flits,
      // whatever the flow control policy is, since method 
      // "controlAlgoritm ()" has been overwritten accordingly.
      setStopped(false);
      return false;
   }
   setStopped(true);   
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZFifoMemoryFlow :: onStopUp(unsigned interfaz, unsigned cv)
{
     #ifndef NO_TRAZA
         uTIME time = getOwnerRouter().getCurrentTime();
         TPZString texto = getComponent().asString() + ": STOP UP!! ";
         texto += TPZString(time);
         TPZWRITE2LOG(texto);
      #endif
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZFifoMemoryFlow :: onStopDown(unsigned interfaz, unsigned cv)
{      
   #ifndef NO_TRAZA
         uTIME time = getOwnerRouter().getCurrentTime();
         TPZString texto = getComponent().asString() + ": STOP DOWN ";
         texto += TPZString(time);
         TPZWRITE2LOG(texto);
    #endif
   if( controlAlgoritm() ) 
   {
     // There is room at the buffer, so you can deactivate
     // stop signal without any danger of overflowing the buffer.
      
     return Inhereited::onStopDown(interfaz,cv);
   }
   
   return true;
}


//*************************************************************************
//:
//  f: virtual void sendFlit (const TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

void TPZFifoMemoryFlow :: sendFlit(TPZMessage* msg)
{
   ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::BufferRead);
   outputInterfaz()->sendData(msg);
}

//*************************************************************************
//:
//  f: virtual Boolean bubbleReady () const;
//  d: semifull control (used in bubble's fifo)
//:
//*************************************************************************

 Boolean TPZFifoMemoryFlow ::bubbleReady()const
 {     
    return true;
 }   
//*************************************************************************

// end of file

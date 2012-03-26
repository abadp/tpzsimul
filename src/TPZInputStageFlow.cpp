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
//    File: TPZInputStageFlow.cpp
//
//    Class:  TPZInputStageFlow
//
//    Inherited from:  TPZRunnableComponent
// :
//*************************************************************************
//end of header

#include <TPZInputStageFlow.hpp>

#ifndef __TPZInputStage_HPP__
#include <TPZInputStage.hpp>
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

IMPLEMENT_RTTI_DERIVED(TPZInputStageFlow,TPZFlow);

//*************************************************************************
//:
//  f: TPZInputStageFlow(TPZComponent& component);
//
//  d:
//:
//*************************************************************************

TPZInputStageFlow :: TPZInputStageFlow(TPZComponent& component)
                   : TPZFlow(component),
                     m_Size(0),
                     m_outputs(1),
		     m_OptimalOutput(0)
{

}


//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//: 
//*************************************************************************

void TPZInputStageFlow :: initialize()
{
   m_Size = ((TPZInputStage&)getComponent()).getBufferSize();  
   m_outputs =  ((TPZInputStage&)getComponent()).numberOfOutputs();
   Inhereited::initialize();
}

//*************************************************************************
//:
//  f: virtual void postInitialize();
//
//  d:
//: 
//*************************************************************************

void TPZInputStageFlow :: postInitialize()
{
   TPZROUTINGTYPE rType=getComponent().getRoutingDirection();
   TPZString NameLeft, NameRight;
   
   switch (rType)
   {
      case _Xplus_:
         NameLeft= TPZString("MPL4");
         NameRight= TPZString("MPR1");
         break;
      case _Yplus_:
         NameLeft= TPZString("MPL3");
         NameRight= TPZString("MPR2");
         break;
      case _Xminus_:
         NameLeft= TPZString("MPL2");
         NameRight= TPZString("MPR3");
         break;
      case _Yminus_:
         NameLeft= TPZString("MPL1");
         NameRight= TPZString("MPR4");
         break;
      case _LocalNode_:
         NameLeft= TPZString("MPL5");
         NameRight= TPZString("MPR5");
         break;
   }
   TPZComponent* compoBuffer1 = getOwnerRouter().componentWithName(NameLeft);
   TPZComponent* compoBuffer2 = getOwnerRouter().componentWithName(NameRight);
   
   m_BufferLeft=POINTERCAST(TPZMultiportIOFifo, compoBuffer1);
   m_BufferRight=POINTERCAST(TPZMultiportIOFifo, compoBuffer2);
}

//*************************************************************************
//:
//  f: virtual Boolean onReadyUp();
//
//  d:
//:
//*************************************************************************
Boolean TPZInputStageFlow :: onReadyUp(unsigned interfaz, unsigned cv)
{
   TPZMessage* msg;
   inputInterfaz()->getData(&msg);

#ifndef NO_TRAZA
   uTIME delayTime = getOwnerRouter().getCurrentTime() ;
   TPZString texto = getComponent().asString() + " Flit Rx on Time" +  TPZString(delayTime) 
                     + " # " + msg->asString() + ") holes= " + TPZString(bufferHoles());
   TPZWRITE2LOG( texto );
#endif

   msg->incDistance();
   if (msg->isHeader() || msg->isHeadTail() )
   {
      updateMessageInfo(msg);
      getOwnerRouter().incrProtocolNumber(msg->getVnet());
   }
   
   if( ! bufferHoles())
   {
      TPZString err;
      err.sprintf("Buffer Exhausted, revision needed");
      EXIT_PROGRAM(err);
   }
   
   uTIME delay = getOwnerRouter().getCurrentTime() + getDataDelay(); 
   TPZEvent event(_BufferData_,msg);
   getEventQueue().enqueue(event,delay);
   
   return true;
}
   
//*************************************************************************
//:
//  f: virtual Boolean inputReading();
//
//  d:
//:
//*************************************************************************
 
Boolean TPZInputStageFlow :: inputReading()
{  
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean stateChange();
//
//  d:
//:
//*************************************************************************

Boolean TPZInputStageFlow :: stateChange()
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

Boolean TPZInputStageFlow :: outputWriting()
{
   uTIME ciclo=getOwnerRouter().getCurrentTime();
   
   for (int i=1; i<=m_outputs; i++)
   {
      outputInterfaz(i)->clearData();
   }
   
   uTIME timeStamp;
   if( getEventQueue().firstElementTimeStamp(timeStamp) )
   {
      if( timeStamp <= getOwnerRouter().getCurrentTime() )
      {
         static TPZEvent event;
         getEventQueue().firstElement(event);
         dispatchEvent(event);
      }
   }
         
   if(!controlAlgoritm())
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
//  f: virtual Boolean dispatchEvent(const TPZEvent& event);
//
//  d: 
//:
//*************************************************************************

Boolean TPZInputStageFlow :: dispatchEvent(const TPZEvent& event)
{
   if( event.type() == _BufferData_ )
   {
      TPZMessage* msg;
      msg = event.message();
      unsigned oPort;
      
      if (msg->isHeader() || msg->isHeadTail())
      {
         oPort=calculateOptimalOutput(msg);
	 if ( oPort!=0)
	 {
	    if ( outputInterfaz(oPort)->isStopActive())
	    {
	       TPZString err;
               err.sprintf("This is not possible");
               EXIT_PROGRAM(err);
	    }
	    if ( (msg->isOrdered()) )
	    {
	       TPZROUTINGTYPE dir=getComponent().getRoutingDirection();
	       getOwnerRouter().setInOrderOccupied(dir);
	    }
	    sendFlit(msg, oPort);
	    
	    static TPZEvent event;
            getEventQueue().dequeue(event);
	    setOptimalOutput(oPort);
#ifndef NO_TRAZA
            uTIME delayTime = getOwnerRouter().getCurrentTime() ;
            TPZString texto = getComponent().asString() + " Flit Tx (";
            texto += TPZString(delayTime) + " # " + msg->asString() + ") holes= " + TPZString(bufferHoles());
            TPZWRITE2LOG( texto );
#endif
	 }
      }
      
      else
      {
         oPort=getOptimalOutput();
	 sendFlit(msg, oPort);
	 static TPZEvent event;
         getEventQueue().dequeue(event);
#ifndef NO_TRAZA
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         TPZString texto = getComponent().asString() + " Flit Tx (";
         texto += TPZString(delayTime) + " # " + msg->asString() + ") holes= " + TPZString(bufferHoles());
         TPZWRITE2LOG( texto );
#endif
      } 
   }
   return true;
}

//*************************************************************************
//:
//  f: virtual unsigned calculateOptimalOutput();
//
//  d:
//:
//*************************************************************************

unsigned TPZInputStageFlow :: calculateOptimalOutput(TPZMessage *msg)
{
   TPZROUTINGTYPE Direction = getComponent().getRoutingDirection();
   unsigned optimalOut;
   
   if (msg->isOrdered())
   {
      if (getOwnerRouter().getFreeInOrder(Direction)==false) return 0;
      if ((outputInterfaz(1)->isStopActive()) && (outputInterfaz(2)->isStopActive())) return 0;
      
      else if ((outputInterfaz(1)->isStopActive()) && (!(outputInterfaz(2)->isStopActive())))
      {
         if ( Direction==_Xplus_ )return 0;
	 else if ( Direction==_Xminus_ )return 2;
	 else if ( Direction==_Yplus_ )return 0;
	 else if ( Direction==_Yminus_ ) return 2;
	 else if ( Direction==_LocalNode_ ) return 0;	 	 
      }
      else if ((!(outputInterfaz(1)->isStopActive())) && (outputInterfaz(2)->isStopActive()))
      {
         if ( Direction==_Xplus_ )return 1;
	 else if ( Direction==_Xminus_ )return 0;
	 else if ( Direction==_Yplus_ )return 1;
	 else if ( Direction==_Yminus_ ) return 0;
	 else if ( Direction==_LocalNode_ ) return 0;	 
      }
      else
      {
         if ( Direction==_Xplus_ )return 1;
	 else if ( Direction==_Xminus_ )return 2;
	 else if ( Direction==_Yplus_ )return 1;
	 else if ( Direction==_Yminus_ ) return 2;
	 else if ( Direction==_LocalNode_ ) return 1;	 
      }
   }
   
   else
   {
      if ((outputInterfaz(1)->isStopActive()) && (outputInterfaz(2)->isStopActive())) return 0;
      else if ((outputInterfaz(1)->isStopActive()) && (!(outputInterfaz(2)->isStopActive()))) return 2;
      else if ((!(outputInterfaz(1)->isStopActive())) && (outputInterfaz(2)->isStopActive())) return 1;
      else
      {
         if (m_BufferRight->bufferHoles() < m_BufferLeft->bufferHoles()) return 2;
         else if (m_BufferLeft->bufferHoles() < m_BufferRight->bufferHoles())return 1;
         else 
	 { 
	    int deltaX = msg->delta(0);
            int deltaY = msg->delta(1);
	    
	    switch( Direction )
            {
               case _Xplus_ :
               
               if ((deltaX>1) && (deltaY>1)) optimalOut=2;
	       else if ((deltaX>1) && (deltaY==1 || deltaY==-1)) optimalOut=2;
               else if ((deltaX==-1 || deltaX==1) &&  (deltaY>1)) optimalOut=2;
               else if ((deltaX==-1 || deltaX==1) && (deltaY<-1)) optimalOut=1;
               else if ((deltaX==-1 || deltaX==1) && (deltaY==1 || deltaY==-1))	optimalOut=1;
	       else optimalOut=(rand()%2)+1;
               break;
     
               case _Xminus_ :
               
               if ( (deltaX<-1) && (deltaY>1) )optimalOut=1;
               else if ( (deltaX<-1) && (deltaY<-1)) optimalOut=2;
               else if ( (deltaX<-1) && (deltaY==1 || deltaY==-1)) optimalOut=1;
               else if ((deltaX==1 || deltaX==-1) && (deltaY>1)) optimalOut=1;
               else if ((deltaX==1 || deltaX==-1) && (deltaY==1 || deltaY==-1)) optimalOut=2;
               else if ((deltaX==1 || deltaX==-1) &&  deltaY<-1) optimalOut=2;
	       else optimalOut=(rand()%2)+1;
               break;
     
               case _Yplus_ :
	       
               if ( (deltaX>1) && (deltaY>1)) optimalOut=1;
               else if ((deltaX>1) && (deltaY==-1 || deltaY==1)) optimalOut=1;
               else if (deltaX<-1 && (deltaY==1 || deltaY==-1)) optimalOut=2;
               else if ((deltaX==1 || deltaX==-1) &&  deltaY>1) optimalOut=1;
	       else if ((deltaX==1 || deltaX==-1) && (deltaY==1 || deltaY==-1)) optimalOut=2;
               else optimalOut=(rand()%2)+1;
               break;
    
               case _Yminus_ :
               
               if ( deltaX>1 && (deltaY<-1)) optimalOut=2;
	       if ( deltaX>1 && (deltaY==1 || deltaY==-1)) optimalOut=2;
               else if (deltaX<-1 && deltaY<-1) optimalOut=1;
               else if (deltaX<-1 && (deltaY==1 || deltaY==-1)) optimalOut=1; 
	       else if ((deltaX==1 || deltaX==-1) && deltaY<-1) optimalOut=2;
               else if ((deltaX==1 || deltaX==-1) && (deltaY==1 || deltaY==-1)) optimalOut=1;
               else optimalOut=(rand()%2)+1;
               break;
    
               case _LocalNode_ :
               if (deltaX>1 && deltaY<-1) optimalOut=1;
               else if (deltaX>1 && (deltaY==1 || deltaY==-1)) optimalOut=1;
               else if (deltaX<-1 && deltaY>1) optimalOut=2;
	       else if (deltaX<-1 && (deltaY==1 || deltaY==-1)) optimalOut=2;
               else if ((deltaX==1 || deltaX==-1) && deltaY>1) optimalOut=2;
               else if ((deltaX==1 || deltaX==-1) && deltaY<-1) optimalOut=1;
               else optimalOut=(rand()%2)+1;
               break;
            }
	 }
	    
      }
      return optimalOut;
   }
}
//*************************************************************************
//:
//  f: virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean TPZInputStageFlow :: onStopUp(unsigned interfaz, unsigned cv)
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

Boolean TPZInputStageFlow :: onStopDown(unsigned interfaz, unsigned cv)
{      
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
//
//  d:
//:
//*************************************************************************
Boolean TPZInputStageFlow :: controlAlgoritm(Boolean info, int delta)
{
   unsigned sizeForCT;
   sizeForCT = ((TPZSimulation*)getComponent().getSimulation())->getPacketLength((TPZNetwork*)getOwnerRouter().getOwner());
   return ( bufferHoles() >= sizeForCT+1 ) ? true : false;
}
//*************************************************************************
//:
//  f: virtual void sendFlit(const TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

void TPZInputStageFlow :: sendFlit(TPZMessage* msg, unsigned interfaz)
{
   ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::IStageTraversal);
   outputInterfaz(interfaz)->sendData(msg);
}

//*************************************************************************
//:
//  f: virtual void updateMessageInfo(TPZMessage* msg);
//
//  d:
//:
//*************************************************************************
Boolean TPZInputStageFlow :: updateMessageInfo(TPZMessage* msg)
{
   int deltaX = msg->delta(0);
   int deltaY =msg->delta(1);
   TPZROUTINGTYPE Direction = getComponent().getRoutingDirection();
   
   msg->flushHopCount();
   msg->setLastDirMiss(getComponent().getRoutingDirection());
   
   if ( msg->getLastMissRouted()==true)
   {
      if (msg->isOrdered() )
      {
         TPZString err;
         err.sprintf("An ordered message cannot be missrouted");
         EXIT_PROGRAM(err);
      }
      
      //RECALCULATE ROUTING RECORD
      int deltaX;
      int deltaY;
      int deltaZ;
      ((TPZNetwork*)getOwnerRouter().getOwner())->routingRecord(getOwnerRouter().getPosition(),msg->destiny(),deltaX,deltaY,deltaZ);
      msg->setDelta(deltaX,0);
      msg->setDelta(deltaY,1);
      msg->setDelta(deltaZ,2);
      msg->setLastMissRouted(false);
      return true;
   }
   
   if ( Direction==_Xplus_ ) msg->setDelta(deltaX-1,0);
   else if ( Direction==_Xminus_ ) msg->setDelta(deltaX+1,0);
   else if ( Direction==_Yplus_ ) msg->setDelta(deltaY-1,1);
   else if ( Direction==_Yminus_ ) msg->setDelta(deltaY+1,1);
   
   if ( msg->isOrdered() )
   {
      msg->setInOrderInputDirection(Direction);
      //Ordered messages do not use wraparound links
      // They are routed in the mesh embedded in the torus
      if ( Direction == _LocalNode_)
      {
         TPZPosition src=msg->source();
	 TPZPosition dst=msg->destiny();
	 TPZPosition deltaPos = dst - src;
	 
         int deltaX = deltaPos.valueForCoordinate(TPZPosition::X);
         int deltaY = deltaPos.valueForCoordinate(TPZPosition::Y);
         deltaX = (deltaX>=0) ? deltaX+1 : deltaX-1;
         deltaY = (deltaY>=0) ? deltaY+1 : deltaY-1;
         
	 msg->setDelta(deltaX,0);
         msg->setDelta(deltaY,1);
      }
   }	
   
   return true;
}

//*************************************************************************

// fin del fichero

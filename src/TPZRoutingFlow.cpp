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
//    File: TPZRoutingFlow.cpp
//
//    Class:  TPZRoutingFlow
//
//    Inherited from:  TPZFlow
// :
//*************************************************************************
//end of header



#include <TPZRoutingFlow.hpp>

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
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

#ifndef __TPZConst_H__
#include <TPZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZRoutingFlow,TPZFlow);

//*************************************************************************
//:
//  f: inline void setHeader (Boolean val);
//  d:
//
//  f: inline isHeader Boolean () const;
//  d:
//
//  f: inline void setWaiting (Boolean val);
//  d:
//
//  f: inline isWaiting Boolean () const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: TPZRoutingFlow (TPZComponent & component);
//
//  d:
//:
//*************************************************************************

TPZRoutingFlow :: TPZRoutingFlow( TPZComponent& component)
                : TPZFlow(component),
                  m_NextHeader(true),
                  m_Waiting(false)
{
   m_Message=0;
}



//*************************************************************************
//:
//  f: virtual Boolean inputReading ();
//
//  d:
//:
//*************************************************************************

Boolean TPZRoutingFlow :: inputReading()
{
   return true;   
}


//*************************************************************************
//:
//  f: virtual Boolean stateChange ();
//
//  d:
//:
//*************************************************************************

Boolean TPZRoutingFlow :: stateChange()
{ 
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean outputWriting ();
//
//  d:
//:
//*************************************************************************

Boolean TPZRoutingFlow :: outputWriting()
{   
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
         else
         {
            outputInterfaz()->clearData();
         }
      }
   }      

   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean updateMessageInfo (TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

Boolean TPZRoutingFlow :: updateMessageInfo(TPZMessage* msg)
{
  
   if (!msg->isMulticast())
   {
      int deltaX = msg->delta(0);
      int deltaY = msg->delta(1);
      int deltaZ = msg->delta(2);
  
      if( msg->isHeader() || msg->isHeadTail() )
      {  
         if( deltaX > 1 )
         {
            msg->setDelta(deltaX-1,0);
            msg->setRoutingPort(_Xplus_);         
         }
         else if( deltaX < -1 )
         {
            msg->setDelta(deltaX+1,0);
            msg->setRoutingPort(_Xminus_);
         }
         else if( deltaY > +1 )
         {
            msg->setDelta(deltaY-1,1);
            msg->setRoutingPort(_Yplus_);         
         }
         else if(deltaY < -1)
         {
            msg->setDelta(deltaY+1,1);
            msg->setRoutingPort(_Yminus_);
         }
	 else if( deltaZ > +1 )
         {
            msg->setDelta(deltaZ-1,2);
            msg->setRoutingPort(_Zplus_);         
         }
         else if(deltaZ < -1)
         {
            msg->setDelta(deltaZ+1,2);
            msg->setRoutingPort(_Zminus_);
         }
         else
         {
            msg->setRoutingPort(_LocalNode_);
         }
      }
   } 
   return changeDirection(msg);
}

//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent (const TPZEvent & event);
//
//  d:
//:
//*************************************************************************

Boolean TPZRoutingFlow :: dispatchEvent(const TPZEvent& event)
{
   m_Message = event.message();
   if( ! controlAlgoritm() )
   {
      #ifndef NO_TRAZA
         TPZString texto = getComponent().asString();
         texto += " Waiting for flow control ";
         TPZWRITE2LOG(texto);
      #endif
      
      uTIME delay = getOwnerRouter().getCurrentTime() + 1;         
      TPZEvent event(_BufferData_,m_Message);
      getEventQueue().enqueue(event,delay);
      return true;   
   }
   
   setWaiting(false);
   if (!m_Message->isHeadTail()) setHeader(false);
   outputInterfaz()->sendData(m_Message);
      
   #ifndef NO_TRAZA
      TPZString delayTime = TPZString(getOwnerRouter().getCurrentTime());
      TPZString texto = getComponent().asString() + " Flit Tx. TIME = ";
      texto += delayTime + " # " + m_Message->asString();
      TPZWRITE2LOG( texto );
   #endif
      
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean onReadyUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZRoutingFlow :: onReadyUp(unsigned interfaz, unsigned cv)
{      
   inputInterfaz()->getData(&m_Message);


   #ifndef NO_TRAZA
      TPZString delayTime1 = TPZString(getOwnerRouter().getCurrentTime());
      TPZString texto1 = getComponent().asString() + " Flit Rx. TIME = ";
      texto1 += delayTime1 + " # " + m_Message->asString();
      TPZWRITE2LOG( texto1 );
   #endif

         
   if( isHeader() )
   {
      outputInterfaz()->clearDataRightNow();
      setWaiting(true);
      updateMessageInfo(m_Message);   
      uTIME delay = getOwnerRouter().getCurrentTime() + 
                    getHeaderDelay();         
      TPZEvent event(_BufferData_,m_Message);
      getEventQueue().enqueue(event,delay);
        
      inputInterfaz()->sendStopRightNow();
      if (m_Message->isHeadTail()) 
      {
         setHeader(true);
      }
      return true;
   }
         
   if( m_Message->isTail() || m_Message->isHeadTail() ) setHeader(true);
   outputInterfaz()->sendDataRightNow(m_Message);
      
   #ifndef NO_TRAZA
      TPZString delayTime = getOwnerRouter().getCurrentTime();
      TPZString texto = getComponent().asString() + " Flit Tx. TIME = ";
      texto += delayTime + " # " + m_Message->asString();
      TPZWRITE2LOG( texto );
   #endif
   
   return true;
}
//*************************************************************************
//:
//  f: virtual Boolean onReadyDown (unsigned interface, unsigned cv)
//
//  d:
//
//*************************************************************************

Boolean TPZRoutingFlow :: onReadyDown(unsigned interfaz, unsigned cv)
{
   #ifndef NO_TRAZA
      TPZString texto = getComponent().asString() + " READY DOWN";
      TPZWRITE2LOG( texto );
   #endif
   
   if( !isWaiting() )
     outputInterfaz()->clearDataRightNow();

   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZRoutingFlow :: onStopDown(unsigned interfaz, unsigned cv)
{
   #ifndef NO_TRAZA
      TPZString texto = getComponent().asString() + " STOP DOWN";
      TPZWRITE2LOG( texto );
   #endif
   
   if(  !isWaiting() )
     inputInterfaz()->clearStopRightNow();

   return true;
}

//*************************************************************************
//:
//  f: Boolean changeDirection (const TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

Boolean TPZRoutingFlow :: changeDirection(const TPZMessage* msg)
{
   if( msg->getRoutingPort() ==_LocalNode_) return false;

   return ( msg->getRoutingPort()!=getComponent().getRoutingDirection() );
}


//*************************************************************************
//:
//  f: getDirection unsigned () const;
//
//  d:
//:
//*************************************************************************

unsigned TPZRoutingFlow :: getDirection() const
{
   unsigned rc = 0;
   TPZROUTINGTYPE dir = getComponent().getRoutingDirection();
   switch(dir)
   {
      case _Xplus_ : case _Xminus_ : rc = CHANNEL_X; break;
      case _Yplus_ : case _Yminus_ : rc = CHANNEL_Y; break;
      case _Zplus_ : case _Zminus_ : rc = CHANNEL_Z; break;
      default : rc = CHANNEL_N;
   }
   return rc;
}


//*************************************************************************


// end of file

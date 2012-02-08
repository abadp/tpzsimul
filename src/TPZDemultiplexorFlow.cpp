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
//    File: TPZDemultiplexorFlow.cpp
//
//    Class:  TPZDemultiplexorFlow
//
//    Inherited from:  TPZFlow
// :
//*************************************************************************
//end of header



#include <TPZDemultiplexorFlow.hpp>

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZDemultiplexor_HPP__
#include <TPZDemultiplexor.hpp>
#endif

#ifndef __TPZFicheroLog_HPP__
#include <TPZFicheroLog.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZDemultiplexorFlow,TPZFlow);

//*************************************************************************
//:
//  f: TPZDemultiplexorFlow (TPZComponent & component);
//
//  d:
//:
//*************************************************************************

TPZDemultiplexorFlow :: TPZDemultiplexorFlow( TPZComponent& component)
                      : TPZFlow(component),
                        m_CurrentOutput(1),
			m_Message(0)
{
   
}


//*************************************************************************
//:
//  f: ~ TPZDemultiplexorFlow ();
//
//  d:
//:
//*************************************************************************

TPZDemultiplexorFlow :: ~TPZDemultiplexorFlow()
{

}

//*************************************************************************
//:
//  f: virtual Boolean inputReading ();
//
//  d:
//:
//*************************************************************************

Boolean TPZDemultiplexorFlow :: inputReading()
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

Boolean TPZDemultiplexorFlow :: stateChange()
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

Boolean TPZDemultiplexorFlow :: outputWriting()
{ 
   unsigned ports=((TPZDemultiplexor&)getComponent()).numberOfOutputs();
   for( unsigned i=1; i<=ports; i++)
   {
      outputInterfaz(i)->clearData();
   }
   if( ! outputInterfaz(m_CurrentOutput)->isStopActive() )
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
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean updateMessageInfo (TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

Boolean TPZDemultiplexorFlow :: updateMessageInfo(TPZMessage* msg)
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
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent (const TPZEvent & event);
//
//  d:
//:
//*************************************************************************

Boolean TPZDemultiplexorFlow :: dispatchEvent(const TPZEvent& event)
{
   uTIME delayTime = getOwnerRouter().getCurrentTime();
   
   if( event.type() == _RoutingVC_ )
   {
      if( (!(m_Message->isHeader())) && (!(m_Message->isHeadTail())))
      {
         TPZString err;
         err.sprintf( "%s :Data flits should not pass through this event",(char*)getComponent().asString() );
         EXIT_PROGRAM(err); 
      }
#ifndef NO_TRAZA
      TPZString texto = getComponent().asString() + " Event ROUTING + TRAVERSAL. TIME = ";
      texto += TPZString(getOwnerRouter().getCurrentTime()) + " # " + m_Message->asString() ;
      TPZWRITE2LOG( texto );
#endif
      updateMessageInfo(m_Message);
      unsigned portout = extractOutputPortNumber(m_Message);
      setCurrentOutput(portout);
      outputInterfaz(getCurrentOutput())->sendData(m_Message);
      inputInterfaz()->clearStop();      
   }
   else if( event.type() == _SwitchTraversal_ )
   {
#ifndef NO_TRAZA
      TPZString texto3 = getComponent().asString() + " Event DEMUX TRAVERSAL. TIME = ";
      texto3 += TPZString(getOwnerRouter().getCurrentTime()) + " # " + m_Message->asString() ;
      TPZWRITE2LOG( texto3 );
#endif
      outputInterfaz(getCurrentOutput())->sendData(m_Message);
      inputInterfaz()->clearStop();
#ifndef NO_TRAZA
      TPZString texto2 = getComponent().asString() + " Flit Tx. TIME = ";
      texto2 += TPZString(delayTime) + " # " + m_Message->asString();
      TPZWRITE2LOG( texto2 );
#endif
   
   }
   return true;   
}

//*************************************************************************
//:
//  f: unsigned extractOutputPortNumber (const TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

unsigned TPZDemultiplexorFlow :: extractOutputPortNumber(TPZMessage* msg)
{
   TPZROUTINGTYPE dir=msg->getRoutingPort();
   unsigned ports=((TPZDemultiplexor&)getComponent()).numberOfOutputs();
   if (dir==_LocalNode_)
   {
      return ports;
   }
   else
   {
      switch(dir)
      {
         case _Xplus_:
            return 1;
         case _Xminus_:
            return 2;
         case _Yplus_:
            return 3;
         case _Yminus_:
            return 4;
         case _Zplus_:
            return 5;
	 case _Zminus_:
	    return 6;
      }
   }
   TPZString err;
   err.sprintf( "This point should not be reached");
   EXIT_PROGRAM(err);
}
//*************************************************************************
//:
//  f: virtual Boolean onStopDown (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZDemultiplexorFlow :: onStopDown(unsigned interfaz, unsigned cv)
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

Boolean TPZDemultiplexorFlow :: onStopUp(unsigned interfaz, unsigned cv)
{
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZDemultiplexorFlow :: onReadyUp(unsigned interfaz, unsigned cv)
{
   inputInterfaz()->getData(&m_Message);
   
   #ifndef NO_TRAZA
      TPZString delayTime1 = TPZString(getOwnerRouter().getCurrentTime());
      TPZString texto1 = getComponent().asString() + " Flit Rx. TIME = ";
      texto1 += delayTime1 + " # " + m_Message->asString();
      TPZWRITE2LOG( texto1 );
   #endif
   
   if( m_Message->isHeader() || m_Message->isHeadTail() )
   {
      uTIME delay = getOwnerRouter().getCurrentTime() + getHeaderDelay();         
      TPZEvent event(_RoutingVC_,m_Message);
      getEventQueue().enqueue(event,delay);
      inputInterfaz()->sendStopRightNow();
      
   }
   else
   {
      uTIME delay = getOwnerRouter().getCurrentTime() + getDataDelay();         
      TPZEvent event(_SwitchTraversal_,m_Message);
      getEventQueue().enqueue(event,delay);
      inputInterfaz()->sendStopRightNow();
   }
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm (Boolean info = false, int delta = 0);
//
//  d:
//:
//*************************************************************************

Boolean TPZDemultiplexorFlow :: controlAlgoritm(Boolean info, int delta)
{
   // To prevent it from being an abstract class.
   return true;
}


//*************************************************************************


// end of file

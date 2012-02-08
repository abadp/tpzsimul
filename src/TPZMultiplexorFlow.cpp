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
//    File: TPZMultiplexorFlow.cpp
//
//    Class:  TPZMultiplexorFlow
//
//    Inherited from:  TPZFlow
// :
//*************************************************************************
//end of header



#include <TPZMultiplexorFlow.hpp>

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZFicheroLog_HPP__
#include <TPZFicheroLog.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

#ifndef __TPZMultiplexor_HPP__
#include <TPZMultiplexor.hpp>
#endif

#ifndef __TPZConst_H__
#include <TPZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZMultiplexorFlow,TPZFlow);

//*************************************************************************
//:
//  f: TPZMultiplexorFlow (TPZComponent & component);
//
//  d:
//:
//*************************************************************************

TPZMultiplexorFlow :: TPZMultiplexorFlow( TPZComponent& component)
                      : TPZFlow(component),
                        m_CurrentInput(0),
                        m_sync(0)
{

}


//*************************************************************************
//:
//  f: ~ TPZMultiplexorFlow ();
//
//  d:
//:
//*************************************************************************

TPZMultiplexorFlow :: ~TPZMultiplexorFlow()
{

}

//*************************************************************************
//:
//  f: initialize ();
//
//  d:
//:
//*************************************************************************

void TPZMultiplexorFlow :: initialize()
{
   Inhereited :: initialize();
   unsigned ports=((TPZMultiplexor&)getComponent()).numberOfInputs();
   m_sync=new TPZMessage*[ports+1];
   
   for(int i=0; i<ports+1; i++)
   {
      m_sync[i]=0;
   }
}

//*************************************************************************
//:
//  f: virtual Boolean inputReading ();
//
//  d:
//:
//*************************************************************************

Boolean TPZMultiplexorFlow :: inputReading()
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

Boolean TPZMultiplexorFlow :: stateChange()
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

Boolean TPZMultiplexorFlow :: outputWriting()
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
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZMultiplexorFlow :: onStopDown(unsigned interfaz, unsigned cv)
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

Boolean TPZMultiplexorFlow :: onStopUp(unsigned interfaz, unsigned cv)
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

Boolean TPZMultiplexorFlow :: onReadyUp(unsigned interfaz, unsigned cv)
{
   TPZMessage* msg;
   inputInterfaz(interfaz)->getData(&msg); 
   m_sync[interfaz]=msg;
   uTIME delayTime = getOwnerRouter().getCurrentTime() ;

#ifndef NO_TRAZA
   TPZString texto = getComponent().asString() + " Flit Rx. TIME = ";
   texto += TPZString(delayTime) + " # " + msg->asString();
   TPZWRITE2LOG( texto );
#endif
            
   if( msg->isHeader() || msg->isHeadTail() )
   {      
      delayTime += getHeaderDelay();
      TPZEvent event(_SwitchAllocator_, interfaz, msg);
      getEventQueue().enqueue(event, delayTime);
      inputInterfaz(interfaz)->sendStopRightNow();      
   }
   
   else
   {
      delayTime += getDataDelay();
      TPZEvent event(_SwitchTraversal_, interfaz, msg);
      getEventQueue().enqueue(event, delayTime);
      inputInterfaz(interfaz)->sendStopRightNow();
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

Boolean TPZMultiplexorFlow :: dispatchEvent(const TPZEvent& event)
{
   uTIME delayTime = getOwnerRouter().getCurrentTime();
   
   if( event.type() == _SwitchAllocator_ )
   {
      unsigned interfaz=event.source();
      TPZMessage* msg=m_sync[interfaz];
      
      if( (!(msg->isHeader())) && (!(msg->isHeadTail())))
      {
         TPZString err;
         err.sprintf( "%s :Data flits should not pass through this event",(char*)getComponent().asString() );
         EXIT_PROGRAM(err); 
      }
#ifndef NO_TRAZA
      TPZString texto = getComponent().asString() + " Event MUX ALLOCATION. TIME = ";
      texto += TPZString(getOwnerRouter().getCurrentTime()) + " # " + msg->asString() ;
      TPZWRITE2LOG( texto );
#endif
      
      if( m_CurrentInput==0 )
      {
         m_CurrentInput=interfaz;
	 outputInterfaz()->sendData(msg);
         inputInterfaz(interfaz)->clearStop();	 
      }
      else
      {
         TPZEvent event(_SwitchAllocator_, interfaz, msg);
	 getEventQueue().enqueue(event, delayTime+1);
      }
      
   }
   else if( event.type() == _SwitchTraversal_ )
   {
      unsigned interfaz=event.source();
      TPZMessage* msg=m_sync[interfaz];
      
      if ( interfaz!=m_CurrentInput)
      {
         TPZString err;
         err.sprintf( "%s :This should not be happening",(char*)getComponent().asString() );
         EXIT_PROGRAM(err); 
      }
      
#ifndef NO_TRAZA
      TPZString texto2 = getComponent().asString() + " Event MUX TRAVERSAL. TIME = ";
      texto2 += TPZString(getOwnerRouter().getCurrentTime()) + " # " + msg->asString() ;
      TPZWRITE2LOG( texto2 );
#endif
      outputInterfaz()->sendData(msg);
      inputInterfaz(interfaz)->clearStop();

#ifndef NO_TRAZA
      TPZString texto3 = getComponent().asString() + " Flit Tx. TIME = ";
      texto3 += TPZString(delayTime) + " # " + msg->asString();
      TPZWRITE2LOG( texto3 );
#endif
      if (msg->isTail() || msg->isHeadTail() )
      {
         m_CurrentInput=0;
      }
      m_sync[interfaz]=0;
   
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

Boolean TPZMultiplexorFlow :: controlAlgoritm(Boolean info, int delta)
{
   // To prevent it from being an abstract class.
   return true;
}

//*************************************************************************


// end of file

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
//    File: TPZFlow.cpp
//
//    Class:  TPZFlow
//
//    Inherited from: 
// :
//*************************************************************************
//end of header



#include <TPZFlow.hpp>

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

#ifndef __TPZFicheroLog_HPP__
#include <TPZFicheroLog.hpp>
#endif

#ifndef __TPZEvent_HPP__
#include <TPZEvent.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_BASE(TPZFlow);

//*************************************************************************
//:
//  f: virtual Boolean inputReading () = 0;
//  d:
//
//  f: virtual Boolean stateChange () = 0;
//  d:
//
//  f: virtual Boolean outputWriting () = 0;
//  d:
//
//  f: virtual Boolean controlAlgoritm (Boolean data = false) = 0;
//  d:
//
//  f: inline TPZComponent & getComponent () const;
//  d:
//
//  f: inline isStopped Boolean () const;
//  d:
//
//  f: inline TPZEventQueue & getEventQueue () const;
//  d:
//

//:
//*************************************************************************

//TPZMessagePool*  TPZFlow::m_MessagePool;

//*************************************************************************
//:
//  f: TPZFlow (TPZComponent & component);
//
//  d:
//:
//*************************************************************************

TPZFlow :: TPZFlow( TPZComponent& component)
         : m_Component(component),
           m_Stopped(false),
           m_State(0),
           m_RegisterTime(0),
           m_InputInterfaz(0),
           m_OutputInterfaz(0)
{
  
}


//*************************************************************************
//:
//  f: virtual ~ TPZFlow ();
//
//  d:
//:
//*************************************************************************

TPZFlow :: ~TPZFlow()
{
   if( m_InputInterfaz )  delete[] m_InputInterfaz;
   if( m_OutputInterfaz ) delete[] m_OutputInterfaz;
}

//*************************************************************************
//:
//  f: void TPZFlow: initializePool (unsigned size)
//
//  d: I go from above the storage capacity of the network
//:
//*************************************************************************
void TPZFlow :: initializePool(unsigned size)
{
   TPZSIMULATION->initializePool(size);   
}
//*************************************************************************
//:
//  f: virtual void initialize ();
//
//  d:
//:
//*************************************************************************

void TPZFlow :: initialize()
{
   // NOTE: The component is the owner of the interface, so it
   //       should not be deleted here when destroyed. The mission 
   //       of this initialize is to reference the interfaces to
   //       access them quickly. It is desirable that
   //       child classes overwritting this method, call
   //       this one first, and then perform what they have
   //       expected.
   
   TPZRunnableComponent& rc = (TPZRunnableComponent&) getComponent();
   unsigned inputs  = rc.numberOfInputs();
   unsigned outputs = rc.numberOfOutputs();
   unsigned i;
   
   m_InputInterfaz = (inputs)  ? new TPZInterfaz*[inputs]  : 0;
   m_OutputInterfaz= (outputs) ? new TPZInterfaz*[outputs] : 0;
    
   for( i=0; i<inputs; i++ )
   {
      *(m_InputInterfaz+i) = getComponent().getInputInterfaz(i+1);
      if( ! (*(m_InputInterfaz+i)) )
      {
         TPZString err;
         err.sprintf( ERR_TPZFLOW_001, (char*)rc.asString() );
         EXIT_PROGRAM(err);
      }
   }

   for( i=0; i<outputs; i++ )
   {
      *(m_OutputInterfaz+i)= getComponent().getOutputInterfaz(i+1);
      if( ! (*(m_OutputInterfaz+i)) )
      {
         TPZString err;
         err.sprintf( ERR_TPZFLOW_002, (char*)rc.asString() );
         EXIT_PROGRAM(err);
      }      
   }


}


//*************************************************************************
//:
//  f: virtual void postInitialize ();
//
//  d:
//:
//*************************************************************************

void TPZFlow :: postInitialize()
{
   // By default, do nothing.
}

//*************************************************************************
//:
//  f: virtual void terminate ();
//
//  d:
//:
//*************************************************************************

void TPZFlow :: terminate()
{

}


//*************************************************************************
//:
//  f: void setStopped (Boolean stop);
//
//  d:
//:
//*************************************************************************

void TPZFlow :: setStopped(Boolean stop)
{
   m_Stopped = stop;
}


//*************************************************************************
//:
//  f: virtual void run (utime runtime);
//
//  d:
//:
//*************************************************************************

void TPZFlow :: run(uTIME runTime)
{
   inputReading();
   stateChange();
   outputWriting();
}


//*************************************************************************
//:
//  f: virtual void preRun (utime time);
//
//  d:
//:
//*************************************************************************

void TPZFlow :: preRun(uTIME time)
{
   // By default, do nothing. Only those components overwritten
   // requiring actions in the second half period of clock.
}


//*************************************************************************
//:
//  f: virtual void postRun (utime time);
//
//  d:
//:
//*************************************************************************

void TPZFlow :: postRun(uTIME time)
{
   // By default, do nothing. Only those components overwritten
   // requiring actions in the second half period of clock.
}


//*************************************************************************
//:
//  f: TPZRouter & getOwnerRouter () const;
//
//  d:
//:
//*************************************************************************

TPZRouter& TPZFlow :: getOwnerRouter() const
{
   return *((TPZRouter*)(getComponent().getRouter()));
}

//*************************************************************************
//:
//  f: TPZMessagePool& getMessagePool() const;
//
//  d:
//:
//*************************************************************************

TPZMessagePool& TPZFlow :: getMessagePool() const
{
   return TPZSIMULATION->getMessagePool();
}

#ifdef PTOPAZ
//*************************************************************************
//:
//  f: TPZMessagePool& getMessagePool(unsigned n) const;
//
//  d:
//:
//*************************************************************************

TPZMessagePool& TPZFlow :: getMessagePool(unsigned n) const
{
   return TPZSIMULATION->getMessagePool(n);
}
#endif

//*************************************************************************
//:
//  f: virtual Boolean onStopUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZFlow :: onStopUp(unsigned interfaz, unsigned cv)
{

   if( propagateStop() && inputInterfaz(interfaz) )     
   {
      inputInterfaz(interfaz)->sendStopRightNow(cv);
   }
   setStopped(true);
   
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZFlow :: onStopDown(unsigned interfaz, unsigned cv)
{
   if( inputInterfaz(interfaz) )
   {  
      inputInterfaz(interfaz)->clearStopRightNow(cv);
   }     
   setStopped(false);
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZFlow :: onReadyUp(unsigned interfaz, unsigned cv)
{   
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyDown (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZFlow :: onReadyDown(unsigned interfaz, unsigned cv)
{
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean propagateStop ();
//
//  d:
//:
//*************************************************************************

Boolean TPZFlow :: propagateStop()
{
   // This method is overwritten in those classes that have to do
   // additional testing to determine whether to retransmit a
   // immediate stop sign at the input, as is the case of the FIFO.
   
   return true;
}


//*************************************************************************
//:
//  f: inputInterfaz TPZInterfaz * (unsigned number = 1) const;
//
//  d:
//:
//*************************************************************************

TPZInterfaz* TPZFlow :: inputInterfaz(unsigned number) const
{
   return m_InputInterfaz ? *(m_InputInterfaz+number-1) : 0;
}


//*************************************************************************
//:
//  f: outputInterfaz TPZInterfaz * (unsigned number = 1) const;
//
//  d:
//:
//*************************************************************************
      
TPZInterfaz* TPZFlow :: outputInterfaz(unsigned number) const
{ 
   return m_OutputInterfaz ? *(m_OutputInterfaz+number-1) : 0;
}


//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent (const TPZEvent & event);
//
//  d:
//:
//*************************************************************************

Boolean TPZFlow :: dispatchEvent(const TPZEvent& event)
{
   TPZString err;
   err.sprintf( ERR_TPZFLOW_003,
                (char*)getComponent().asString(),
                (char*)event.asString() );
   EXIT_PROGRAM(err);
   return true;
}


//*************************************************************************
//:
//  f: getHeaderDelay utime () const;
//
//  d:
//:
//*************************************************************************

uTIME TPZFlow :: getHeaderDelay() const
{
   return ((TPZRunnableComponent&)getComponent()).getHeaderDelay();
}


//*************************************************************************
//:
//  f: getDataDelay utime () const;
//
//  d:
//:
//*************************************************************************

uTIME TPZFlow :: getDataDelay() const
{
   return ((TPZRunnableComponent&)getComponent()).getDataDelay();
}


//*************************************************************************
//:
//  f: virtual void setRegisterTime ();
//
//  d:
//:
//*************************************************************************

void TPZFlow :: setRegisterTime()
{
   m_RegisterTime = getOwnerRouter().getCurrentTime();
}


//*************************************************************************
//:
//  f: virtual TPZString getStatus () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZFlow :: getStatus() const
{
  return "";  
}


//*************************************************************************

// end of file

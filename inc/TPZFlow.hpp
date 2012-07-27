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
//    File: TPZFlow.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZFlow_HPP__
#define __TPZFlow_HPP__

//*************************************************************************

   #include <TPZBase.hpp>

   #ifndef __TPZPriorityQueue_HPP__
   #include <TPZPriorityQueue.hpp>
   #endif

   #ifndef __TPZEvent_HPP__
   #include <TPZEvent.hpp>
   #endif

   #ifndef __TPZPool_HPP__
   #include <TPZPool.hpp>
   #endif
   
//*************************************************************************

   typedef TPZPriorityQueue<TPZEvent> TEventQueue;
   typedef TPZPool<TPZMessage> TPZMessagePool;

//*************************************************************************

   class TPZComponent;
   class TPZInterfaz;
   class TPZRouter;

//*************************************************************************

   class TPZFlow : public TPZBase
   {
   public:
      TPZFlow( TPZComponent& component);
      virtual ~TPZFlow();

      virtual void run(uTIME runTime);      
      virtual void preRun(uTIME time);
      virtual void postRun(uTIME time);
      virtual void initialize();
      virtual void postInitialize();
      virtual void terminate();

      virtual Boolean inputReading()    = 0;
      virtual Boolean stateChange()     = 0;
      virtual Boolean outputWriting()   = 0;

      virtual Boolean controlAlgoritm(Boolean data=false, int delta=0) = 0;
      virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
      virtual Boolean onReadyDown(unsigned interfaz, unsigned cv);
      virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
      virtual TPZString getStatus() const;

      TPZInterfaz* inputInterfaz(unsigned number =1 )  const;
      TPZInterfaz* outputInterfaz(unsigned number =1 ) const;
      
      TPZRouter& getOwnerRouter() const;
      TPZComponent& getComponent() const 
      { return (TPZComponent&)m_Component; }
      
      // Run time information
      DEFINE_RTTI(TPZFlow);
      
      TPZMessagePool& getMessagePool() const;
      
#ifdef PTOPAZ
      TPZMessagePool& getMessagePool(unsigned n) const;
#endif
      void initializePool(unsigned size);

   protected:
      virtual Boolean dispatchEvent(const TPZEvent& event);
      virtual Boolean propagateStop();

      virtual void    setState(int state)
      { m_State = state; }

      virtual int     getState() const
      { return m_State; }
                  
      void setStopped(Boolean stop=true);

      Boolean isStopped() const
      { return m_Stopped; }
      
      TEventQueue& getEventQueue() const
      { return (TEventQueue&)m_EventQueue; }


   protected:
      uTIME getHeaderDelay() const;
      uTIME getDataDelay() const;
      
      uTIME getRegisterTime() const
      { return m_RegisterTime; }

      void setRegisterTime(); 

      TPZInterfaz**  m_InputInterfaz;
      TPZInterfaz**  m_OutputInterfaz;
      
   private:
      TPZComponent&   m_Component;
      Boolean         m_Stopped;
      TEventQueue     m_EventQueue;
      int             m_State;
      uTIME           m_RegisterTime;     
      //static TPZMessagePool*  m_MessagePool;
      //unsigned m_poolSize;

};


//*************************************************************************

#endif


// end of file

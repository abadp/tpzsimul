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
//    File: TPZCrossbarFlow.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZCrossbarFlow_HPP__
#define __TPZCrossbarFlow_HPP__

//*************************************************************************

   #include <TPZFlow.hpp>

   #ifndef __TPZArray_HPP__
   #include <TPZArray.hpp>
   #endif

   #ifndef __TPZQueue_HPP__
   #include <TPZQueue.hpp>
   #endif

   #ifndef __TPZPair_HPP__
   #include <TPZPair.hpp>
   #endif
      
//*************************************************************************

   class TPZMessage;
   
//*************************************************************************
   
   typedef TPZPair<unsigned,unsigned> uPAIR;
   typedef TPZArray<uPAIR>            TPZCrossbarTable;
   typedef TPZArray<TPZMessage*>      TPZMessageTable;

//*************************************************************************

   class TPZCrossbarFlow : public TPZFlow
   {
      typedef TPZFlow Inhereited;
   
   protected:
      typedef TPZQueue<uPAIR> TPZRequestQueue;   
            
   public:
      typedef enum { Free, Connect, PassMessage, Waiting } TPZCrossbarState;
      
      TPZCrossbarFlow( TPZComponent& component);
      virtual ~TPZCrossbarFlow();

      virtual void initialize();
      virtual void terminate();

      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
      virtual Boolean inputReading();
      virtual Boolean stateChange();
      virtual Boolean outputWriting();

      virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
      virtual TPZString getStatus() const;
      virtual Boolean  addConnectionRequest(unsigned source, uPAIR destiny, uTIME time=(uTIME)0.0);
      
      // Run time information
      DEFINE_RTTI(TPZCrossbarFlow);

   protected:
      virtual  void    cleanOutputInterfaces();
      virtual  void    cleanInputInterfaces();
      virtual  Boolean propagateStop();
      virtual  void    cleanConnectionRequestFor( unsigned port, 
                                                  const uPAIR& pair=uPAIR(0,0));
      virtual  Boolean dispatchEvent(const TPZEvent& event);
      unsigned getConnectionRequestFor(unsigned port);
      uPAIR    getConnectionRequestForWith(unsigned port);
      Boolean  isAnyRequestForOutput(unsigned output);
      
      uPAIR    getOutputForInput(unsigned inputPort) const;
      unsigned getInputForOutput(unsigned outputPort, unsigned channel ) const;
      virtual  Boolean  isOutputPortFree(unsigned outputPort, unsigned channel=0) const;   
      void     establishConnection(unsigned input, unsigned output, unsigned channel=0);
      virtual  void     clearConnection(unsigned input);
      
      virtual  unsigned extractOutputPortNumber(TPZROUTINGTYPE dir, unsigned channel);
      virtual  unsigned extractOutputPortNumber(TPZMessage* msg);
      virtual  Boolean  updateMessageInfo(TPZMessage* msg, unsigned oPort, unsigned channel);
      
      unsigned extractOutputChannel(TPZMessage* msg) const;

      unsigned getLastInputForOutput(unsigned output) const;
      void     setLastInputForOutput(unsigned input,unsigned output);
      void     setStateForInput(unsigned i, TPZCrossbarState state);
      virtual  uPAIR    checkMultipleRequestFor(unsigned oPort);
      void     removeAllRequestEventsForInput(unsigned iPort);
      void     removeAllRequestEventsForOutput(unsigned oPort);
      void     enableEvent(int event);
      void     disableEvent(int event);
      Boolean  isEventEnable(int event);
      TPZCrossbarState getStateForInput(unsigned i) const;

      Boolean cleanInterfaces() const
      { return m_CleanInterfaces; }
      
      void setCleanInterfaces(Boolean value)
      { m_CleanInterfaces = value; }
            
   protected:
      
      typedef TPZArray<TPZCrossbarState> TPZStateTable;

      TPZCrossbarTable*   m_PortAsignamentTable;
      TPZArray<unsigned>* m_OutputStateTable;
      TPZArray<unsigned>* m_OutputLastUseTable;
      TPZMessageTable*    m_MessageReceivedTable;
      TPZStateTable*      m_InputStateTable;
      TPZRequestQueue*    m_RequestQueue;
      Boolean             m_CleanInterfaces;
      Boolean             m_EventsActive[2];
};

//*************************************************************************


#endif


// end of file

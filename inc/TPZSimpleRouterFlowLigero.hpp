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
//    File: TPZSimpleRouterFlowLigero.hpp
//
//    Class: TPZSimpleRouterFlowLigero
//
//    Inherited from: TPZSimpleRouterFlow
// :
//*************************************************************************
//end of header


#ifndef __TPZSimpleRouterFlowLigero_HPP__
#define __TPZSimpleRouterFlowLigero_HPP__

//*************************************************************************

   #include <TPZSimpleRouterFlow.hpp>

//*************************************************************************

class TPZSimpleRouterFlowLigero : public TPZSimpleRouterFlow
{
      typedef TPZSimpleRouterFlow Inhereited;
   
   public:
      typedef enum { Bypass, Turn, Consume, Escape } TPZIStageState;
      typedef enum { Leave, Advance } TPZMPStageState;
      TPZSimpleRouterFlowLigero( TPZComponent& component);
      virtual ~TPZSimpleRouterFlowLigero();
      
      virtual void initialize();
      virtual void terminate();
      virtual Boolean inputReading();
      virtual Boolean outputWriting();
      virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
      // Run time information
      DEFINE_RTTI(TPZSimpleRouterFlowLigero);

   protected:
       Boolean checkFlowControl(unsigned Port, TPZMessageQueue* queue, unsigned numBubble, unsigned BufferSize) const;
       Boolean checkEscapeFlowControl() const;
       Boolean checkEscapePath( TPZROUTINGTYPE direction);
       
       Boolean checkHeader(unsigned inPort, TPZMessage* msg);
       
       Boolean updateMessageInfo(TPZMessage* msg, unsigned oPort);
       
       TPZROUTINGTYPE  getOutputDirection(TPZMessage* msg);
       TPZROUTINGTYPE  getInjectionDirection(TPZMessage* msg);
              
   protected:
      
      unsigned m_ISSize;
      unsigned m_OSSize;
      unsigned m_MPSize;
      unsigned m_missLoops;
      unsigned m_missLimit;
      
      Boolean* m_linkAvailable;
      Boolean* m_clearConnection;
      
      Boolean m_consAvailable;
      Boolean m_clearConsConnection;
      
      TPZIStageState* m_istage_ctrl;
      unsigned m_injection_ctrl;
      
      TPZMPStageState* m_mp_one_ctrl;
      TPZMPStageState* m_mp_two_ctrl;
      
      Boolean* m_ringExitAvailable;
      Boolean* m_clearRingExit;
      
      Boolean* m_ringAdvanceAvailable;
      Boolean* m_clearRingAdvance;
      
      TPZMessage** m_latch_cons;
      
      TPZMessage** m_latch_out_turn;
      TPZMessage** m_latch_out_byp;
      TPZMessage** m_latch_out_inj;
      TPZMessage** m_latch_mp_one;
      TPZMessage** m_latch_mp_two;
      
      TPZMessageQueue* m_fifos_cons;
      TPZMessageQueue* m_fifos_out_byp;
      TPZMessageQueue* m_fifos_mp_one;
      TPZMessageQueue* m_fifos_mp_two;
      TPZMessageQueue* m_fifos_out_inj;
      TPZMessageQueue* m_fifos_out_turn;
      
      TPZMessageQueue  m_fifo_escape;
      TPZMessage*      m_latch_escape;     
      TPZROUTINGTYPE   m_escape_ctrl;
      Boolean          m_clearEscConnection;
      Boolean*         m_interfazOnEscape;
      
      Boolean*         m_interfazInOrder;   
};

//*************************************************************************


#endif


// end of file

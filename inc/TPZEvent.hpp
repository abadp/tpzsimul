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
//    File: TPZEvent.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZEvent_HPP__
#define __TPZEvent_HPP__

//*************************************************************************

   #ifndef __TPZBase_HPP__
   #include <TPZBase.hpp>
   #endif

   #ifndef __TPZMessage_HPP__
   #include <TPZMessage.hpp>
   #endif

//*************************************************************************

   typedef enum {
                  _NullEvent_,
                  _FirstElement_,
                  _CrossbarRequest_,
                  _CrossbarPass_,
                  _RoutingHeader_,
                  _Multiplexor_,
                  _BufferData_,
		  _RoutingVC_,
		  _VCAllocator_,
		  _SwitchAllocator_,
		  _SwitchTraversal_,
		  _SendRequest_,
		  _CheckResponse_,
		  _DataTransmit_,
                  Routing_CheckCrossbarResponse,
                  Routing_HeaderTx,
                  Routing_InputStopped,
                  Routing_SendNextRequest,
                  Free_routing,
                  Again_createMessageRequest,
	     _antiS_bubbles_
                } 
                TPZEVENTTYPE;

//*************************************************************************

   class TPZEvent 
   {
   public:
      TPZEvent();
      TPZEvent( TPZEVENTTYPE type, 
                unsigned src=0, 
                unsigned dst=0,
                unsigned chn=0,
                TPZMessage* msg=0 );
      TPZEvent(TPZEVENTTYPE type, TPZMessage* msg);
      TPZEvent(TPZEVENTTYPE type, unsigned src, TPZMessage* msg);
      TPZEvent(const TPZEvent& event);

      TPZEvent& operator=(const TPZEvent& event);
      Boolean   operator==(const TPZEvent& event) const;
      TPZString asString() const;
            
      unsigned  source() const
      { return m_Source; }
      
      unsigned destiny() const
      { return m_Destiny; }
      
      unsigned channel() const
      { return m_Channel; }
      
      TPZMessage * message() const
      { return m_Message; }
      
      void setMessage(TPZMessage * a_message)
      { m_Message=a_message; }
            
      TPZEVENTTYPE type() const
      { return m_Type; }
      
      DEFINE_RTTI(TPZEvent);

   private:
      TPZEVENTTYPE m_Type;
      unsigned     m_Source;
      unsigned     m_Destiny;
      unsigned     m_Channel;
      TPZMessage*  m_Message;
   };
   
//*************************************************************************   


#endif


//*************************************************************************

// end of file

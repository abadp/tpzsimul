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
//    File: TPZPort.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZPort_HPP__
#define __TPZPort_HPP__

//************************************************************************

   #ifndef __TPZComponent_HPP__
   #include <TPZComponent.hpp>
   #endif
   
   #ifndef __TPZConnection_HPP__
   #include <TPZConnection.hpp>
   #endif

   #ifndef __TPZMessage_HPP__
   #include <TPZMessage.hpp>
   #endif
   
//************************************************************************

   class TPZPort : public TPZComponent
   {
      typedef TPZComponent Inhereited;

   public:
      TPZPort( const TPZComponentId& id);
      
      virtual Boolean sendDataRightNow(TPZMessage* msg) =0;
      virtual Boolean clearDataRightNow() =0;
     
      virtual Boolean sendStopRightNow() =0;
      virtual Boolean clearStopRightNow() =0;
      
      Boolean establishConnection(TPZConnection* connection);
      
      virtual void run(uTIME runTime);
      virtual TPZString asString() const;

      void setDataChanged(Boolean change);
      
      Boolean isDataChanged() const
      { return m_DataChanged; }

      Boolean isStopChanged() const
      { return m_StopChanged; }

      void setStopChanged(Boolean change)
      { m_StopChanged = change; }
           
      virtual Boolean sendData(TPZMessage* msg);
      
      virtual Boolean sendStop()
      {
         if( m_Stop ) return false;
         setStopChanged( ((!m_Stop)?true:false) );
         m_Stop = true;
         return true;      
      }

      virtual Boolean clearData()
      { 
         if( ! m_Ready ) return false;
         setDataChanged( ((m_Ready)?true:false) );
         m_Ready = false;
         return true; 
      }
      
      virtual Boolean clearStop()
      {
         if( ! m_Stop ) return false;
         setStopChanged( ((m_Stop)?true:false) );
         m_Stop = false;
         return true;
      }
      
      void setData(TPZMessage* msg)
      { m_MessageStored=msg; }
      
      TPZMessage* getData() const
      { return m_MessageStored; }

      Boolean isStopActive() const
      { return m_Stop; }

      Boolean isReadyActive() const
      { return m_Ready; }
      
      // Run time information
      DEFINE_RTTI(TPZPort);

   protected:
      TPZMessage*       m_MessageStored;
      Boolean           m_Ready;
      Boolean           m_Stop;
      Boolean           m_DataChanged;
      Boolean           m_StopChanged;
      unsigned          m_CurrentConnection;      

      unsigned getOwnerInterfazId() const;      
};

//************************************************************************


#endif

// end of file

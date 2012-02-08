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
//    File: TPZGlobalData.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZGlobalData_HPP__
#define __TPZGlobalData_HPP__

//************************************************************************

   #ifndef __TPZArray_HPP__
   #include <TPZArray.hpp>
   #endif
   
   #ifndef __TPZString_HPP__
   #include <TPZString.hpp>
   #endif
      

//*************************************************************************

   class TPZTypeChannel
   {
   public:
      TPZTypeChannel() : m_RoutingType(_Unknow_),
                         m_Channel(0)
      { }
      TPZTypeChannel(TPZROUTINGTYPE type, unsigned channel) :
                     m_RoutingType(type),
                     m_Channel(channel)
      { }
      TPZTypeChannel(const TPZTypeChannel& type) :
                     m_RoutingType(type.m_RoutingType),
                     m_Channel(type.m_Channel)
      { }
      TPZTypeChannel& operator=(const TPZTypeChannel& type)
      { 
         m_RoutingType = type.m_RoutingType;
         m_Channel = type.m_Channel; 
         return *this;
      }
      Boolean operator==(const TPZTypeChannel& type)
      { 
         return ((m_RoutingType==type.m_RoutingType) &&
                 (m_Channel==type.m_Channel)) ? true : false; 
      }
      TPZROUTINGTYPE type() const
      { return m_RoutingType; }
   private:
      TPZROUTINGTYPE m_RoutingType;
      unsigned       m_Channel;
   };
   
//*************************************************************************
//** Added for simulation on SimpleRouter Half duplex
//*************************************************************************

   class TPZTypeChannelHalfDuplex
   {
   public:
      TPZTypeChannelHalfDuplex() : m_RoutingType(_Unknow_),
                         m_Channel(0),
			 m_ReversePort(false)
      { }
      TPZTypeChannelHalfDuplex(TPZROUTINGTYPE type, unsigned channel, Boolean reversePort) :
                     m_RoutingType(type),
                     m_Channel(channel),
		     m_ReversePort(reversePort)
      { }
      TPZTypeChannelHalfDuplex(const TPZTypeChannelHalfDuplex& type) :
                     m_RoutingType(type.m_RoutingType),
                     m_Channel(type.m_Channel),
		     m_ReversePort(type.m_ReversePort)
      { }
      TPZTypeChannelHalfDuplex& operator=(const TPZTypeChannelHalfDuplex& type)
      { 
         m_RoutingType = type.m_RoutingType;
         m_Channel = type.m_Channel; 
	 m_ReversePort = type.m_ReversePort;
         return *this;
      }
      Boolean operator==(const TPZTypeChannelHalfDuplex& type)
      { 
         return ((m_RoutingType==type.m_RoutingType) &&
                 (m_Channel==type.m_Channel) && (m_ReversePort==type.m_ReversePort)) ? true : false; 
      }
      TPZROUTINGTYPE type() const
      { return m_RoutingType; }
      Boolean HalfDuplex() const
      { return m_ReversePort; }
   private:
      TPZROUTINGTYPE m_RoutingType;
      unsigned       m_Channel;
      Boolean        m_ReversePort;
   };
   
//*************************************************************************
   
   typedef TPZArray<TPZROUTINGTYPE> TPZTypeArray;   
   typedef TPZArray<TPZTypeChannel> TPZRoutingTypeArray;
   typedef TPZArray<TPZTypeChannelHalfDuplex> TPZRoutingHalfDuplexTypeArray;
   
//*************************************************************************

   class TPZGlobalData
   {      
   public:   
      TPZGlobalData();
      virtual ~TPZGlobalData();

      //Initialization ************************************************* **
      
      Boolean routerIsInitializated() const
      { return m_rInitializated; }
      
      void routerSetInitializated()
      { m_rInitializated=true; }
      
      Boolean simpleRouterIsInitializated() const
      { return m_sInitializated; }

      void simpleRouterSetInitializated()
      { m_sInitializated=true; }

      Boolean crossbarIsInitializated() const
      { return m_cInitializated; }

      void crossbarSetInitializated()
      { m_cInitializated=true; }
      
      //************************************************** TPZRouter **** **

      TPZTypeArray* routerOutputsType() const
      { return m_rOutputsType; }
      
      void routerSetOutputsType(TPZTypeArray* type)
      { m_rOutputsType=type; } 
      
      TPZTypeArray* routerInputsType() const
      { return m_rInputsType; }

      void routerSetInputsType(TPZTypeArray* type)
      { m_rInputsType=type; } 

      unsigned routerBufferSize() const
      { return m_rBufferSize; }
      
      void routerSetBufferSize(unsigned size)
      { m_rBufferSize=size; }

      TPZString routerBufferControl() const
      { return m_rBufferControl; }
      
      void routerSetBufferControl(const TPZString& control)
      { m_rBufferControl=control; }

      TPZString routerRoutingControl() const
      { return m_rRoutingControl; }
      
      void routerSetRoutingControl(const TPZString& control)
      { m_rRoutingControl=control; }
      
      
      
      //************************************************** ** ** TPZSimpleRouter
      
      TPZRoutingTypeArray* simpleRouterOutputsType() const
      { return m_sOutputsType; }
      
      TPZRoutingHalfDuplexTypeArray* simpleRouterHDOutputsType() const
      { return m_sHDOutputsType; }
      
      void simpleRouterSetOutputsType(TPZRoutingTypeArray* type)
      { m_sOutputsType=type; }
      
      void simpleRouterSetHDOutputsType(TPZRoutingHalfDuplexTypeArray* type)
      { m_sHDOutputsType=type; } 
      
      TPZRoutingTypeArray* simpleRouterInputsType() const
      { return m_sInputsType; }
      
      TPZRoutingHalfDuplexTypeArray* simpleRouterHDInputsType() const
      { return m_sHDInputsType; }

      void simpleRouterSetInputsType(TPZRoutingTypeArray* type)
      { m_sInputsType=type; } 
      
      void simpleRouterSetHDInputsType(TPZRoutingHalfDuplexTypeArray* type)
      { m_sHDInputsType=type; }

      unsigned simpleRouterNumberOfLocalPorts() const
      { return m_sLocalPorts; }

      void simpleRouterSetNumberOfLocalPorts(unsigned number)
      { m_sLocalPorts = number; }

      //************************************************** ** ** TPZCrossbar
      
      TPZRoutingTypeArray* crossbarOutputsType() const
      { return m_cOutputsType; }
      
      void crossbarSetOutputsType(TPZRoutingTypeArray* type)
      { m_cOutputsType=type; } 
      
      TPZRoutingTypeArray* crossbarInputsType() const
      { return m_cInputsType; }

      void crossbarSetInputsType(TPZRoutingTypeArray* type)
      { m_cInputsType=type; } 

      unsigned crossbarNumberOfLocalPorts() const
      { return m_cLocalPorts; }

      void crossbarSetNumberOfLocalPorts(unsigned number)
      { m_cLocalPorts = number; }

      unsigned crossbarOutputMux() const
      { return m_cOutputMux; }

      void crossbarSetOutputMux(unsigned number)
      { m_cOutputMux = number; }
      
      unsigned crossbarNumberVC() const
      { return m_cNumVC; }

      void crossbarSetNumberVC(unsigned number)
      { m_cNumVC = number; }
      
      unsigned crossbarNumberMT() const
      { return m_cNumMT; }

      void crossbarSetNumberMT(unsigned number)
      { m_cNumMT = number; }
      
      // Run time information
      DEFINE_RTTI(TPZGlobalData);

   private:
                                     

      TPZRoutingTypeArray* m_sInputsType;        // TPZSimpleRouter
      TPZRoutingTypeArray* m_sOutputsType;
      TPZRoutingHalfDuplexTypeArray* m_sHDInputsType;        
      TPZRoutingHalfDuplexTypeArray* m_sHDOutputsType;
      unsigned             m_sLocalPorts;
      unsigned             m_sOutputMux;
      Boolean              m_sInitializated;

      TPZRoutingTypeArray* m_cInputsType;        // TPZCrossbar
      TPZRoutingTypeArray* m_cOutputsType;
      unsigned             m_cLocalPorts;
      unsigned             m_cOutputMux;
      unsigned             m_cNumVC;
      unsigned             m_cNumMT;
      Boolean              m_cInitializated;
      
      TPZTypeArray*        m_rInputsType;        // TPZRouter
      TPZTypeArray*        m_rOutputsType;      
      unsigned             m_rBufferSize;
      TPZString            m_rBufferControl;
      TPZString            m_rRoutingControl;
      Boolean              m_rInitializated;

};


//*************************************************************************

#endif


// end of file

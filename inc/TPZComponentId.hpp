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
//    File: TPZComponentId.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZComponentId_HPP__
#define __TPZComponentId_HPP__

//*************************************************************************

   #ifndef __TPZBase_HPP__
   #include <TPZBase.hpp>
   #endif

   #ifndef __TPZTag_HPP__
   #include <TPZTag.hpp>
   #endif

//*************************************************************************

   class TPZComponentId
   {
   public:
      typedef enum { UnknowComponent,
                     Port,
                     Crossbar,
                     FifoMemory,
                     MultiportFifo,
                     Connection,
                     RunComponent,
                     InputInterfaz,
                     OutputInterfaz,
                     Injector,
                     Consumer,
                     Router,
                     Routing,
                     RoutingMuxed,
                     Test,
                     Network,
                     Multiplexor,
		     Demultiplexor,
                     ViewInterfaz,
                     Simulation
                   } ComponentClass;

      TPZComponentId(const TPZComponentId& id);
      TPZComponentId( ComponentClass clase, 
                      TPZROUTINGTYPE tipo=_Unknow_,
                      const TPZString& name = "",
                      unsigned channel =1);
      TPZComponentId(const TPZTag& tag);
      
      virtual ~TPZComponentId();
      
      TPZComponentId& operator=(const TPZComponentId& id);
      Boolean operator==(const TPZComponentId& id) const;
      Boolean isLike(const TPZComponentId& id) const;
      TPZString asString() const;

      ComponentClass componentClass() const
      { return m_Class; }

      void setComponentType(TPZROUTINGTYPE type)
      { m_Type = type; }

      TPZROUTINGTYPE componentType() const
      { return m_Type; }

      void setName(const TPZString& aName)
      { m_Name = aName; }

      TPZString name() const
      { return m_Name; }
      
      void setChannel(unsigned channel)
      { m_Channel = channel; }

      unsigned channel() const
      { return m_Channel; }

      // Run time information
      DEFINE_RTTI(TPZComponentId);
      TPZString getPortName() const
      {
     return m_TypeAsIs;
      }

      friend ostream& operator<<(ostream& os, const TPZComponentId& id);

   private:
      ComponentClass m_Class;
      TPZROUTINGTYPE m_Type;
      TPZString      m_Name;
      TPZString      m_TypeAsIs;
      unsigned       m_Channel;
   };

//*************************************************************************

   #define INPUT_PORT   TPZComponentId( TPZComponentId::Port,   \
                                        _Unknow_,               \
                                        "IP", 4 )

   #define OUTPUT_PORT  TPZComponentId( TPZComponentId::Port,  \
                                        _Unknow_,              \
                                        "OP", 4 )

   #define INJECTOR_ID   TPZComponentId( TPZComponentId::Injector, \
                                         _Unknow_,                 \
                                         "INJ" )

   #define CONSUMER_ID   TPZComponentId( TPZComponentId::Consumer,  \
                                         _Unknow_,                  \
                                         "CSM" )

   #define ROUTER_ID   TPZComponentId( TPZComponentId::Router,  \
                                       _Unknow_,                \
                                       "ROU" )

   #define BUFFER_ID   TPZComponentId( TPZComponentId::FifoMemory, \
                                       _Unknow_,                   \
                                       "BUF" )
   #define MPBUFFER_ID   TPZComponentId( TPZComponentId::MultiportFifo, \
                                       _Unknow_,                   \
                                       "MPBUF" )

   #define CROSSBAR_ID TPZComponentId( TPZComponentId::Crossbar,  \
                                       _Unknow_,                  \
                                       "CRB" )

   #define ROUTING_ID TPZComponentId( TPZComponentId::Routing,  \
                                      _Unknow_,                 \
                                      "RTNG" )

   #define TEST_ID TPZComponentId( TPZComponentId::Test,  \
                                   _Unknow_,              \
                                   "TEST" )

   #define NETWORK_ID TPZComponentId( TPZComponentId::Network,  \
                                      _Unknow_,              \
                                      "NETWORK" )
                                         
//*************************************************************************

#endif


// end of file

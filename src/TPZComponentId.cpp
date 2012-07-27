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
//    File: TPZComponentId.cpp
//
//    Class:  TPZComponentId
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#include <TPZComponentId.hpp>

#ifndef __TPZConst_H__
#include <TPZConst.hpp>
#endif

#ifndef __TPZBase_HPP__
#include <TPZBase.hpp>
#endif

IMPLEMENT_RTTI_BASE(TPZComponentId);

//*************************************************************************
//:
//  f: inline ComponentClass componentClass () const;
//  d:
//
//  f: inline void setComponentType (TPZROUTINGTYPE type);
//  d:
//
//  f: inline TPZROUTINGTYPE componentType () const;
//  d:
//
//  f: inline void setName (const TPZString & aName);
//  d:
//
//  f: inline TPZString name () const;
//  d:
//
//  f: inline unsigned channel () const;
//  d:
//:
//*************************************************************************

//*************************************************************************
//:
//  f: TPZComponentId (ComponentClass class
//                     ComponentType type
//                     const TPZString & name,
//                     unsigned channel);
//
//  d:
//:
//*************************************************************************

TPZComponentId :: TPZComponentId( ComponentClass clase, 
                                  TPZROUTINGTYPE tipo,
                                  const TPZString& name,
                                  unsigned channel )
                : m_Class(clase),
                  m_Type(tipo),
                  m_Name(name),
                  m_Channel(channel),
              m_TypeAsIs("_Unknown_")
{

}


//*************************************************************************
//:
//  f: TPZComponentId (const TPZComponentId & id);
//
//  d:
//:
//*************************************************************************

TPZComponentId :: TPZComponentId(const TPZComponentId& id)
                : m_Class(id.componentClass()),
                  m_Type(id.componentType()),
                  m_Name(id.name()),
                  m_Channel(id.channel()),
                  m_TypeAsIs(id.getPortName())
{

}


//*************************************************************************
//:
//  f: TPZComponentId (const TPZTag & tag);
//
//  d:
//:
//*************************************************************************

TPZComponentId :: TPZComponentId(const TPZTag& tag)
                : m_Class(UnknowComponent),
                  m_Type(_Unknow_),
                  m_Channel(1)
{
   // Identifier is constructed from Sgml tag.
   m_Name = tag.tagId();
   
   TPZString channel, type;
   
   if( tag.getAttributeValueWithName(TPZ_TAG_CHANNEL, channel) )
   {
      m_Channel = channel.asInteger();
   }
   
   if( tag.getAttributeValueWithName(TPZ_TAG_TYPE,type) )
   {
      m_Type = _string2routingType(type);
      m_TypeAsIs=type;
      
   }

   TPZString component = tag.tagName();
   
   if( component == TPZ_TAG_ROUTER )
      m_Class = TPZComponentId::Router;

   else if( component == TPZ_TAG_BUFFER )
      m_Class = TPZComponentId::FifoMemory;
   
   else if( component == TPZ_TAG_MPBUFFER )
      m_Class = TPZComponentId::MultiportFifo;
   
   else if( component == TPZ_TAG_ROUTING )
      m_Class = TPZComponentId::Routing;
   
   else if( component == TPZ_TAG_ROUTING_MUXED )
      m_Class = TPZComponentId::RoutingMuxed;
   
   else if( component == TPZ_TAG_CROSSBAR )
      m_Class = TPZComponentId::Crossbar;

   else if( component == TPZ_TAG_MUX )
      m_Class = TPZComponentId::Multiplexor;
   
   else if( component == TPZ_TAG_DEMUX )
      m_Class = TPZComponentId::Demultiplexor;
   
   else if( component == TPZ_TAG_CONNECTION )
      m_Class = TPZComponentId::Connection;

   else if( component == TPZ_TAG_INJECTOR )
      m_Class = TPZComponentId::Injector;

   else if( component == TPZ_TAG_CONSUMER )
      m_Class = TPZComponentId::Consumer;

   else if( component == TPZ_TAG_TORUSNET || component == TPZ_TAG_MESHNET)
      m_Class = TPZComponentId::Network;

   else if( component == TPZ_TAG_SIMULATION )
      m_Class = TPZComponentId::Simulation;

}

//*************************************************************************
//:
//  f: void TPZComponentId:: ~ TPZComponentId ()
//
//  d:
//:
//*************************************************************************

TPZComponentId :: ~TPZComponentId()
{
}

//*************************************************************************
//:
//  f: TPZComponentId & operator = (const TPZComponentId & id);
//
//  d:
//:
//*************************************************************************

TPZComponentId& TPZComponentId :: operator=(const TPZComponentId& id)
{
   m_Class  = id.componentClass();
   m_Type   = id.componentType();
   m_Name   = id.name();
   m_Channel= id.channel();
   m_TypeAsIs= id.getPortName();

   
   return *this;
}


//*************************************************************************
//:
//  f: Boolean operator == (const TPZComponentId & id) const;
//
//  d:
//:
//*************************************************************************

Boolean TPZComponentId :: operator==(const TPZComponentId& id) const
{
   if( (m_Class==id.componentClass()) &&
       (m_Type==id.componentType())   &&
       (m_Name==id.name())            &&
       (m_Channel==id.channel())      &&
       (m_TypeAsIs==id.getPortName())
    )
   {
      return true;
   }
   return false;
}


//*************************************************************************
//:
//  f: Boolean isLike (const TPZComponentId & id) const;
//
//  d:
//:
//*************************************************************************

Boolean TPZComponentId::isLike(const TPZComponentId& id) const
{
   if( (m_Class==id.componentClass()) &&
       (id.componentType()==_Unknow_ || m_Type==id.componentType() ) &&
       (m_Name.isLike(id.name())) )
   {
      return true;
   }
   return false;
}


//*************************************************************************
//:
//  f: TPZString AsString () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZComponentId::asString() const
{
   return name();
}

//*************************************************************************
//:
//  f: friend ostream & operator <<(ostream & os, const TPZComponentId & id);
//
//  d:
//:
//*************************************************************************

ostream& operator<<(ostream& os, const TPZComponentId& id)
{
   os << "Component id = " << id.name() << endl;
   return os;
}


// end of file

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
//    File: TPZInjector.cpp
//
//    Class:  TPZInjector
//
//    Inherited from:  TPZRunnableComponent
// :
//*************************************************************************
//end of header




#include <TPZInjector.hpp>

#ifndef __TPZInjectorFlow_HPP__
#include <TPZInjectorFlow.hpp>
#endif

#ifndef __TPZInjectorFlowUC_HPP__
#include <TPZInjectorFlowUC.hpp>
#endif

#ifndef __TPZInjectorFlowCT_HPP__
#include <TPZInjectorFlowCT.hpp>
#endif

#ifndef __TPZInjectorFlowCTUC_HPP__
#include <TPZInjectorFlowCTUC.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

#ifndef __TPZConst_H__
#include <TPZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZInjector,TPZRunnableComponent);

//*************************************************************************
//:
//  f: TPZInjector (const TPZComponentId & id, TPZInjectorType type);
//
//  d:
//:
//*************************************************************************

TPZInjector :: TPZInjector(const TPZComponentId& id, TPZInjectorType type)
             : TPZRunnableComponent(id),
               m_InjectorType(type),
               m_numberOfMessageTypes(0)
{

}


//*************************************************************************
//:
//  f: virtual ~ TPZInjector ();
//
//  d:
//:
//*************************************************************************

TPZInjector :: ~TPZInjector()
{

}


//*************************************************************************
//:
//  f: virtual void initialize ();
//
//  d:
//:
//*************************************************************************

void TPZInjector :: initialize()
{
   if( isInitializated() ) return;
   setNumberOfInputs(0);
   Inhereited::initialize();
}


//*************************************************************************
//:
//  f: virtual void buildFlowControl ();
//
//  d:
//:
//*************************************************************************

void TPZInjector :: buildFlowControl()
{
   if( m_InjectorType == TPZInjector::WH )
      {
      setFlowControl( new TPZInjectorFlow(*this) );
      }
   else if( m_InjectorType == TPZInjector::WH_UC )
      {
      setFlowControl( new TPZInjectorFlowUC(*this) );
      }   
   else if( m_InjectorType == TPZInjector::CT )
      {
      setFlowControl( new TPZInjectorFlowCT(*this) );
      }
   else if( m_InjectorType == TPZInjector::CT_UC )
      {
      setFlowControl( new TPZInjectorFlowCTUC(*this) );
      }
   getFlowControl()->initialize();
}


//*************************************************************************
//:
//  f: void sendMessage (const TPZMessage & amessage)
//
//  d:
//:
//*************************************************************************

void TPZInjector :: sendMessage(TPZMessage* aMessage)
{
   ((TPZInjectorFlow*)getFlowControl())->sendMessage(aMessage);
}

//*************************************************************************
//:
//  f: getAvaliableMessages unsigned () const;
//
//  d:
//:
//*************************************************************************

unsigned TPZInjector :: getAvaliableMessages() const
{
   return (((TPZInjectorFlow*)getFlowControl())->getAvaliableMessages());
}

//*************************************************************************
//:
//  f: getMessagesToSend unsigned () const;
//
//  d:
//:
//*************************************************************************

unsigned TPZInjector :: getMessagesToSend() const
{
   return (((TPZInjectorFlow*)getFlowControl())->getMessagesToSend());
}

//*************************************************************************
//:
//  f: getSentFlits unsigned () const;
//
//  d:
//:
//*************************************************************************

unsigned TPZInjector :: getSentFlits() const
{
   return (((TPZInjectorFlow*)getFlowControl())->getSentFlits());
}

//*************************************************************************
//:
//  f: getSentFlits unsigned () const;
//
//  d:
//:
//*************************************************************************

void TPZInjector :: clearSentFlits() 
{
   (((TPZInjectorFlow*)getFlowControl())->clearSentFlits());
}


//*************************************************************************
//:
//  f: virtual TPZString AsString () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZInjector :: asString() const
{
   TPZString string = getName();
   if( getRouter() )
      string+=((TPZRouter*)getRouter())->getPosition().asString();
   return string;
}


//*************************************************************************
//:
//  f: static TPZInjector * newFrom (TPZTag const * tag, TPZComponent * owner);
//
//  d:
//:
//*************************************************************************

TPZInjector* TPZInjector :: newFrom(const TPZTag* tag, TPZComponent* owner)
{
   TPZComponentId idInjector(*tag);
   TPZString type;
   TPZString numberOfMessageTypes;
      
   tag->getAttributeValueWithName(TPZ_TAG_TYPE_INJECTOR, type);
   tag->getAttributeValueWithName(TPZ_TAG_NUM_MESS_TYPES, numberOfMessageTypes);
      
   TPZInjector* injector = 0;
   type = (type==TPZString("")) ? ((TPZRouter*)owner)->valueForBufferControl() : type;
   
   if( type == TPZ_TAG_WORMHOLE )
   {
      injector = new TPZInjector(idInjector,TPZInjector::WH);
   }
   else if( type == TPZ_TAG_WORMHOLE_UNICAST )
   {
      injector = new TPZInjector(idInjector,TPZInjector::WH_UC);
   }
   else if( type == TPZ_TAG_CUTHROUGH )
   {
      injector = new TPZInjector(idInjector,TPZInjector::CT);
   }
   else if( type == TPZ_TAG_CUTHROUGH_UNICAST )
   {
      injector = new TPZInjector(idInjector,TPZInjector::CT_UC);
   }
   else
   {
      TPZString err;
      err.sprintf( ERR_TPZINJEC_001, (char*)type );
      EXIT_PROGRAM(err);
   }

   if ( numberOfMessageTypes == TPZString(""))
   {
      injector->setNumberOfMessageTypes(1);
   }
   else
   {
      int numero = numberOfMessageTypes.asInteger();
      injector->setNumberOfMessageTypes(numero);
   }
   
   
   ((TPZRouter*)owner)->addInjector(injector);
   return injector;
}


//*************************************************************************

// end of file

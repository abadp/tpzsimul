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
//    File: TPZConsumer.cpp
//
//    Class:  TPZConsumer
//
//    Inherited from:  TPZRunnableComponent
// :
//*************************************************************************
//end of header




#include <TPZConsumer.hpp>

#ifndef __TPZConsumerFlow_HPP__
#include <TPZConsumerFlow.hpp>
#endif

#ifndef __TPZConsumerFlowReactive_HPP__
#include <TPZConsumerFlowReactive.hpp>
#endif

#ifndef __TPZConsumerFlowBless_HPP__
#include <TPZConsumerFlowBless.hpp>
#endif

#ifndef __TPZConsumerFlowRespTimeSim_HPP__
#include <TPZConsumerFlowRespTimeSim.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZConst_H__
#include <TPZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZConsumer,TPZRunnableComponent);


//*************************************************************************
//:
//  f: TPZCosumer (const TPZComponentId & id);
//
//  d:
//:
//*************************************************************************

TPZConsumer :: TPZConsumer(const TPZComponentId& id, TPZConsumerType m_Type)
             : TPZRunnableComponent(id),
           m_ConsumerType(m_Type)
{

}


//*************************************************************************
//:
//  f: virtual ~ TPZConsumer ();
//
//  d:
//:
//*************************************************************************

TPZConsumer :: ~TPZConsumer()
{

}


//*************************************************************************
//:
//  f: virtual void initialize ();
//
//  d:
//:
//*************************************************************************

void TPZConsumer :: initialize()
{
   setNumberOfOutputs(0);
   Inhereited::initialize();
}

//*************************************************************************
//:
//  f: virtual void buildFlowControl ();
//
//  d:
//:
//*************************************************************************

void TPZConsumer :: buildFlowControl()
{
   if( m_ConsumerType == TPZConsumer::CT )
      setFlowControl(new TPZConsumerFlow(*this));
   else if (m_ConsumerType == TPZConsumer::REACT )
      setFlowControl(new TPZConsumerFlowReactive(*this));
   else if (m_ConsumerType == TPZConsumer::BLESS )
      setFlowControl(new TPZConsumerFlowBless(*this));
   else if (m_ConsumerType == TPZConsumer::RSPTSIM)
      setFlowControl(new TPZConsumerFlowRespTimeSim(*this));   
   else
      setFlowControl(new TPZConsumerFlow(*this));
   
   getFlowControl() -> initialize();
   
}


//*************************************************************************
//:
//  f: virtual void buildInputInterfaz (unsigned number = 1);
//
//  d:
//:
//*************************************************************************

void TPZConsumer :: buildInputInterfaz(unsigned number)
{
   // The method must be overwritten, because the output interface
   // from the multiplexor must carry N control signals, the number of VCs.

   for( int i=1; i<=number; i++ )   
   {
      TPZInterfaz* interfaz = 
                   TPZInterfaz::createInputInterfaz(i,numberOfInputs());
      interfaz->setOwner(this);
      addComponent(interfaz);
   }
}


//*************************************************************************
//:
//  f: virtual TPZString AsString () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZConsumer :: asString() const
{
   TPZString string = getName();
   if( getRouter() )
      string += ((TPZRouter*)getRouter())->getPosition().asString();
   return string;
}


//*************************************************************************
//:
//  f: static TPZConsumer * newFrom (TPZTag const * tag, TPZComponent * owner);
//
//  d:
//:
//*************************************************************************

TPZConsumer* TPZConsumer :: newFrom(const TPZTag* tag, TPZComponent* owner)
{
   TPZComponentId idConsumer(*tag);
   TPZString type;
   TPZString dataDelay, consumerInputs, readLatency, writeLatency, bufferSize;;
   uTIME delay = 1;
   int cycles_read=0;
   int cycles_write=0;
   int buffer_size=1;
   
   TPZConsumer* consumer = 0;

   tag->getAttributeValueWithName(TPZ_TAG_TYPE_CONSUMER, type);
   
   if( type == TPZ_TAG_REACTIVE )
   {
      consumer = new TPZConsumer(idConsumer,TPZConsumer::REACT);
   }
   else if( type == TPZ_TAG_BLESS )
   {
      consumer = new TPZConsumer(idConsumer,TPZConsumer::BLESS);
   }
   else if (type == TPZ_TAG_RSPTSIM) 
   {
      consumer = new TPZConsumer(idConsumer, TPZConsumer::RSPTSIM);
   }
   else
   {
      consumer = new TPZConsumer(idConsumer,TPZConsumer::CT);
   }
   
   if( tag->getAttributeValueWithName(TPZ_TAG_DATA_DELAY, dataDelay) )
   {
      delay = dataDelay.asInteger();
   }
   consumer->setDataDelay(delay);
   
   if (tag->getAttributeValueWithName(TPZ_TAG_CONSUMER_READLAT, readLatency)) 
   {
      cycles_read = readLatency.asInteger();
   }
   consumer->setReadLatency(cycles_read);
   
   if (tag->getAttributeValueWithName(TPZ_TAG_CONSUMER_WRITELAT, writeLatency)) 
   {
      cycles_write = writeLatency.asInteger();
   }
   consumer->setWriteLatency(cycles_write);
    
   if (tag->getAttributeValueWithName(TPZ_TAG_CONSUMER_BUFFERSIZE, bufferSize)) 
   {
      buffer_size = bufferSize.asInteger();
   }
   consumer->setBufferSize(buffer_size);
   
   if( tag->getAttributeValueWithName(TPZ_TAG_INPUTS, consumerInputs) )
   {
      unsigned size = consumerInputs.asInteger();
      consumer->setNumberOfInputs(size);
   } 
      
   ((TPZRouter*)owner)->addConsumer(consumer);
   return consumer;
}


//*************************************************************************



// end of file

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
//    File: TPZOutputStage.cpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header

#include <TPZOutputStage.hpp>

#ifndef __TPZOutputStageFlow_HPP__
#include <TPZOutputStageFlow.hpp>
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

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZOutputStage,TPZRunnableComponent);

//*************************************************************************
//:
//  f: TPZOutputStage(const TPZComponentId& id);
//
//  d:
//:
//*************************************************************************

TPZOutputStage :: TPZOutputStage( const TPZComponentId& id,
                                unsigned bufferSize,
				unsigned inputs,
				TPZOutputStageControl control )
               : TPZRunnableComponent(id),
                 m_BufferSize(bufferSize),
		 m_inputs(inputs),
		 m_StageControl(control)
{

}


//*************************************************************************
//:
//  f: virtual ~TPZOutputStage();
//
//  d:
//:
//*************************************************************************

TPZOutputStage :: ~TPZOutputStage()
{

}


//*************************************************************************
//:
//  f: virtual void buildFlowControl();
//
//  d:
//:
//*************************************************************************

void TPZOutputStage :: buildFlowControl()
{
   if( getStageControl() == TPZOutputStage::CT )
   {
      setFlowControl( new TPZOutputStageFlow(*this) );
   }
   if( getFlowControl() )
   {
      getFlowControl()->initialize();
   }
   else
   {
      TPZString err;
      err.sprintf(ERR_TPZFIFO_001,getStageControl());
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: unsigned bufferHoles() const;
//
//  d:
//:
//*************************************************************************

unsigned TPZOutputStage :: bufferHoles() const
{
   return ((TPZOutputStageFlow*)getFlowControl())->bufferHoles();
}

//*************************************************************************
//:
//  f: unsigned bufferHoles() const;
//
//  d:
//:
//*************************************************************************

Boolean TPZOutputStage :: controlReplicationAlgorithm( unsigned index) const
{
   return ((TPZOutputStageFlow*)getFlowControl())->controlReplicationAlgorithm(index);
}

//*************************************************************************
//:
//  f: unsigned bufferOccupation() const;
//
//  d:
//:
//*************************************************************************

unsigned TPZOutputStage :: bufferOccupation() const
{
   return (getBufferSize()-bufferHoles());
}


//*************************************************************************
//:
//  f: void setBufferSize(unsigned size);
//
//  d:
//:
//*************************************************************************

void TPZOutputStage :: setBufferSize(unsigned size)
{
   m_BufferSize = size;
   if( getFlowControl() )
   {
      ((TPZOutputStageFlow*)getFlowControl()) -> setBufferSize(size);
   }
}

//*************************************************************************
//:
//  f: virtual TPZString asString() const;
//
//  d:
//:
//*************************************************************************

TPZString TPZOutputStage :: asString() const
{
   TPZString string = getName();
   if( getRouter() )
      string += ((TPZRouter*)getRouter())->getPosition().asString();
   return string;
}


//*************************************************************************
//:
//  f: static TPZOutputStage* newFrom(const TPZTag* tag, TPZComponent* owner);
//
//  d:
//:
//*************************************************************************

TPZOutputStage* TPZOutputStage :: newFrom(const TPZTag* tag, TPZComponent* owner)
{
   TPZComponentId idBuffer(*tag);
   TPZString bufferSize, stageControl, dataDelay, numInputs;
   
   unsigned size, delay=1, inputs=2;
   
   if( tag->getAttributeValueWithName(TPZ_TAG_SIZE, bufferSize) )
   {
      size = bufferSize.asInteger();
   }
   else
   {
      size = ((TPZRouter*)owner)->valueForBufferSize();
   }

   if( tag->getAttributeValueWithName(TPZ_TAG_INPUTS, numInputs) )
   {
      inputs = numInputs.asInteger();
   }
   
   if( tag->getAttributeValueWithName(TPZ_TAG_DATA_DELAY, dataDelay) )
   {
      delay = dataDelay.asInteger();
   }

   if( ! (tag->getAttributeValueWithName(TPZ_TAG_CONTROL, stageControl)))
   {
      stageControl = ((TPZRouter*)owner)->valueForBufferControl();
   }
   
   TPZOutputStageControl control = TPZOutputStage::NIL;
   if( stageControl == TPZ_TAG_CUTHROUGH )
   {
      control = TPZOutputStage::CT;
   }
      
   TPZOutputStage* buffer = new TPZOutputStage(idBuffer,size,inputs,control);
   buffer->setNumberOfInputs(inputs);
   buffer->setDataDelay(delay);
   
    ((TPZRouter*)owner)->addOutputStageBuffer(buffer);
   
   return buffer;
}

//*************************************************************************

// fin del fichero

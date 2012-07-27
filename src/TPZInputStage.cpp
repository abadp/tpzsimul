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
//    File: TPZFifoMemory.cpp
//
//    Class:  TPZFifoMemory
//
//    Inherited from:  TPZRunnableComponent
// :
//*************************************************************************
//end of header


#include <TPZInputStage.hpp>

#ifndef __TPZInputStageFlow_HPP__
#include <TPZInputStageFlow.hpp>
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

IMPLEMENT_RTTI_DERIVED(TPZInputStage,TPZRunnableComponent);


//*************************************************************************
//:
//  f: TPZInputStage(const TPZComponentId& id);
//
//  d:
//:
//*************************************************************************

TPZInputStage :: TPZInputStage( const TPZComponentId& id,
                                unsigned bufferSize, 
				unsigned outputs, TPZInputStageControl control )
               : TPZRunnableComponent(id),
                 m_BufferSize(bufferSize),
		 m_outputs(outputs),
		 m_StageControl(control)
{

}


//*************************************************************************
//:
//  f: virtual ~TPZInputStage();
//
//  d:
//:
//*************************************************************************

TPZInputStage :: ~TPZInputStage()
{

}


//*************************************************************************
//:
//  f: virtual void buildFlowControl();
//
//  d:
//:
//*************************************************************************

void TPZInputStage :: buildFlowControl()
{
   if( getStageControl() == TPZInputStage::CT )
   {
      setFlowControl( new TPZInputStageFlow(*this) );
   }
   else
   {
      TPZString err;
      err.sprintf(ERR_TPZFIFO_001,getStageControl());
      EXIT_PROGRAM(err);
   }
   getFlowControl()->initialize();   
}


//*************************************************************************
//:
//  f: unsigned bufferHoles() const;
//
//  d:
//:
//*************************************************************************

unsigned TPZInputStage :: bufferHoles() const
{
   return ((TPZInputStageFlow*)getFlowControl())->bufferHoles();
}

//*************************************************************************
//:
//  f: unsigned bufferOccupation() const;
//
//  d:
//:
//*************************************************************************

unsigned TPZInputStage :: bufferOccupation() const
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

void TPZInputStage :: setBufferSize(unsigned size)
{
   m_BufferSize = size;
   if( getFlowControl() )
   {
      ((TPZInputStageFlow*)getFlowControl()) -> setBufferSize(size);
   }
}

//*************************************************************************
//:
//  f: virtual TPZString asString() const;
//
//  d:
//:
//*************************************************************************

TPZString TPZInputStage :: asString() const
{
   TPZString string = getName();
   if( getRouter() )
      string += ((TPZRouter*)getRouter())->getPosition().asString();
   return string;
}


//*************************************************************************
//:
//  f: static TPZFifoMemory* newFrom(const TPZTag* tag, TPZComponent* owner);
//
//  d:
//:
//*************************************************************************

TPZInputStage* TPZInputStage :: newFrom(const TPZTag* tag, TPZComponent* owner)
{
   TPZComponentId idBuffer(*tag);
   TPZString bufferSize, stageControl, PowerModel, dataDelay, numOutputs;
   
   unsigned size, delay=1, outputs=2;
   
   if( tag->getAttributeValueWithName(TPZ_TAG_SIZE, bufferSize) )
   {
      size = bufferSize.asInteger();
   }
   else
   {
      size = ((TPZRouter*)owner)->valueForBufferSize();
   }

   if( tag->getAttributeValueWithName(TPZ_TAG_OUTPUTS, numOutputs) )
   {
      outputs = numOutputs.asInteger();
   }
   
   if( tag->getAttributeValueWithName(TPZ_TAG_DATA_DELAY, dataDelay) )
   {
      delay = dataDelay.asInteger();
   }

   if( ! (tag->getAttributeValueWithName(TPZ_TAG_CONTROL, stageControl)))
   {
      stageControl = ((TPZRouter*)owner)->valueForBufferControl();
   }
  
   TPZInputStageControl control = TPZInputStage::NIL;
   if( stageControl == TPZ_TAG_CUTHROUGH )
   {
      control = TPZInputStage::CT;
   }
      
   TPZInputStage* buffer = new TPZInputStage(idBuffer,size,outputs, control);
   buffer->setNumberOfOutputs(outputs);
   buffer->setDataDelay(delay);
     
   ((TPZRouter*)owner)->addInputStageBuffer(buffer);
   
   return buffer;
}


//*************************************************************************

// fin del fichero

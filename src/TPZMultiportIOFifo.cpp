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
//    File: TPZMultiportIOFifo.cpp
//
//    Class:  TPZMultiportIOFifo
//
//    Inherited from:  TPZRunnableComponent
// :
//*************************************************************************
//end of header




#include <TPZMultiportIOFifo.hpp>

#ifndef __TPZMultiportIOFifoFlow_HPP__
#include <TPZMultiportIOFifoFlow.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

#ifndef __TPZConst_H__
#include <TPZConst.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZMultiportIOFifo,TPZRunnableComponent);

//*************************************************************************
//:
//  f: inline TPZBufferControl getBufferControl () const;
//  d:
//
//  f: inline unsigned getBufferSize () const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: TPZMultiportIOFifo (const TPZComponentId & id);
//
//  d:
//:
//*************************************************************************

TPZMultiportIOFifo :: TPZMultiportIOFifo( const TPZComponentId& id,
                           unsigned bufferSize, unsigned numberInputPorts,
                                  unsigned numberOutputPorts, unsigned wideOfMem,
                           TPZBufferControl control )
               : TPZRunnableComponent(id),
                 m_BufferSize(bufferSize),
                 m_BufferControl(control),
                 m_inputs(numberInputPorts),
                   m_outputs(numberOutputPorts),
                 m_BufferSizeShort(0),
                 m_wideOfMemory(wideOfMem)
{
     setNumberOfInputs(numberInputPorts);
    setNumberOfOutputs(numberOutputPorts);
}


//*************************************************************************
//:
//  f: virtual ~ TPZMultiportIOFifo ();
//
//  d:
//:
//*************************************************************************

TPZMultiportIOFifo :: ~TPZMultiportIOFifo()
{

}


//*************************************************************************
//:
//  f: virtual void buildFlowControl ();
//
//  d:
//:
//*************************************************************************

void TPZMultiportIOFifo :: buildFlowControl()
{
   TPZString err;
   err.sprintf("TPZMultiportIOFifo :: %d is UNIMPLEMENTED", getBufferControl());
   if( getBufferControl() == TPZMultiportIOFifo::CT )
   {
      //EXIT_PROGRAM(err);  
      setFlowControl (new TPZMultiportIOFifoFlow (* this));
   }
   else
   {
      TPZString err;
      err.sprintf("Solo implemetado control de flujo CT y WH");
      EXIT_PROGRAM(err);  
   }
   
   if( getFlowControl() )
   {
      getFlowControl()->initialize();
   }
   else
   {
      TPZString err;
      err.sprintf(ERR_TPZFIFO_001,getBufferControl());
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: bufferHoles unsigned () const;
//
//  d:
//:
//*************************************************************************

unsigned TPZMultiportIOFifo :: bufferHoles() const
{
   return ((TPZMultiportIOFifoFlow*)getFlowControl())->bufferHoles();
}

//*************************************************************************
//:
//  f: bufferOccupation unsigned () const;
//
//  d:
//:
//*************************************************************************

unsigned TPZMultiportIOFifo :: bufferOccupation() const
{
   return (getBufferSize()-bufferHoles());
}


//*************************************************************************
//:
//  f: void setBufferSize (unsigned size);
//
//  d:
//:
//*************************************************************************

void TPZMultiportIOFifo :: setBufferSize(unsigned size)
{
   m_BufferSize = size;
   if( getFlowControl() )
   {
      ((TPZMultiportIOFifoFlow*)getFlowControl()) -> setBufferSize(size);
   }
}
//*************************************************************************
//:
//  f: void setBufferSizeShort (unsigned size);
//
//  d:
//:
//*************************************************************************

void TPZMultiportIOFifo :: setBufferSizeShort(unsigned size)
{
   m_BufferSizeShort = size;

}
//*************************************************************************
//:
//  f: virtual Boolean bubbleReady () const;
//
//  d: bubble Return information for routing decision unit.
//:
//*************************************************************************

Boolean TPZMultiportIOFifo ::bubbleReady() const
{

      return ((TPZMultiportIOFifoFlow*)getFlowControl()) -> bubbleReady();

}

//*************************************************************************
//:
//  f: virtual TPZString AsString () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZMultiportIOFifo :: asString() const
{
   TPZString string = getName();
   if( getRouter() )
      string += ((TPZRouter*)getRouter())->getPosition().asString();
   return string;
}


//*************************************************************************
//:
//  f: static TPZMultiportIOFifo * newFrom (TPZTag const * tag, TPZComponent * owner);
//
//  d:
//:
//*************************************************************************

TPZMultiportIOFifo* TPZMultiportIOFifo :: newFrom(const TPZTag* tag, TPZComponent* owner)
{
   TPZComponentId idBuffer(*tag);
   TPZString multiportSize, bufferControl, dataDelay, inputs, outputs, wideMem;
   TPZString bufferShort;
   unsigned size,sizeShort, delay=1;
      
   if( tag->getAttributeValueWithName(TPZ_TAG_SIZE, multiportSize) )
   {
      size = multiportSize.asInteger();
   }
   else
   {
      size = ((TPZRouter*)owner)->valueForBufferSize();
   }
   
   if( tag->getAttributeValueWithName(TPZ_TAG_SIZE_SHORT, bufferShort) )
   {
      sizeShort = bufferShort.asInteger();
   }
   else
   {
      sizeShort = ((TPZRouter*)owner)->valueForBufferSize();
   }
   
   if( tag->getAttributeValueWithName(TPZ_TAG_DATA_DELAY, dataDelay) )
   {
      delay = dataDelay.asInteger();
   }

   if( ! (tag->getAttributeValueWithName(TPZ_TAG_CONTROL, bufferControl)) )
   {
      bufferControl = ((TPZRouter*)owner)->valueForBufferControl();
   }
   //Added VPC (number of input ports)
   if( ! (tag->getAttributeValueWithName(TPZ_TAG_INPUTS, inputs)) )
   {
      TPZString err;
      err.sprintf(ERR_TPZCROSS_002, (char*)TPZ_TAG_INPUTS );
      EXIT_PROGRAM(err);
   }
   //Added ICL (number of output ports)
   if( ! (tag->getAttributeValueWithName(TPZ_TAG_OUTPUTS, outputs)) )
   {
      TPZString err;
      err.sprintf(ERR_TPZCROSS_002, (char*)TPZ_TAG_OUTPUTS );
      EXIT_PROGRAM(err);
   }

   //Added ICL (width = length packet memory)
   if( ! (tag->getAttributeValueWithName(TPZ_TAG_WIDE_MEM, wideMem)) )
   {
      TPZString err;
      err.sprintf(ERR_TPZCROSS_002, (char*)TPZ_TAG_WIDE_MEM );
      EXIT_PROGRAM(err);
   }
   
   TPZBufferControl control = TPZMultiportIOFifo::NIL;
   if( bufferControl == TPZ_TAG_CUTHROUGH )
   {
      control = TPZMultiportIOFifo::CT;
   }
   else if ( bufferControl == TPZ_TAG_WORMHOLE )
   {
      control = TPZMultiportIOFifo::WH;
   }
   else
   {
      TPZString err;
      err.sprintf( ERR_TPZFIFO_002, (char*)bufferControl );
      EXIT_PROGRAM(err);
   }
   
   //VPVAdded
   unsigned inp = inputs.asInteger();   
   //ICLAdded
   unsigned outp = outputs.asInteger();   
   unsigned wide = wideMem.asInteger(); 
   
   TPZMultiportIOFifo* buffer = new TPZMultiportIOFifo(idBuffer,size,inp,outp,wide,control);
   if(sizeShort!=((TPZRouter*)owner)->valueForBufferSize())
      buffer->setBufferSizeShort(sizeShort);
   buffer->setOwner(owner);
   buffer->setDataDelay(delay-1);   //What is this?
   
   //((TPZRouter *) owner) -> addBuffer (buffer);
   
   ((TPZRouter*)owner)->addMultiportIOBuffer(buffer);
   
   return buffer;
}

//*************************************************************************

// end of file

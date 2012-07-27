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
                           unsigned numberOutputPorts, 
                           unsigned missLimit, TPZBufferControl control )
               : TPZRunnableComponent(id),
                 m_BufferSize(bufferSize),
                 m_BufferControl(control),
                 m_inputs(numberInputPorts),
                 m_outputs(numberOutputPorts),
                 m_missLimit(missLimit)
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
   if( getBufferControl() == TPZMultiportIOFifo::CT )
   {
      setFlowControl (new TPZMultiportIOFifoFlow (* this));
   }
   else
   {
      TPZString err;
      err.sprintf("Only CT Flow Control implemented");
      EXIT_PROGRAM(err);  
   }
   
   if( getFlowControl() )
   {
      getFlowControl()->initialize();
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
   TPZString multiportSize, bufferControl, dataDelay, inputs, outputs, wideMem, missrouting;
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
   
   if( tag->getAttributeValueWithName(TPZ_TAG_DATA_DELAY, dataDelay) )
   {
      delay = dataDelay.asInteger();
   }

   if( ! (tag->getAttributeValueWithName(TPZ_TAG_CONTROL, bufferControl)) )
   {
      bufferControl = ((TPZRouter*)owner)->valueForBufferControl();
   }
   
   if( ! (tag->getAttributeValueWithName(TPZ_TAG_INPUTS, inputs)) )
   {
      TPZString err;
      err.sprintf(ERR_TPZCROSS_002, (char*)TPZ_TAG_INPUTS );
      EXIT_PROGRAM(err);
   }
   
   if( ! (tag->getAttributeValueWithName(TPZ_TAG_OUTPUTS, outputs)) )
   {
      TPZString err;
      err.sprintf(ERR_TPZCROSS_002, (char*)TPZ_TAG_OUTPUTS );
      EXIT_PROGRAM(err);
   }

   if( ! (tag->getAttributeValueWithName(TPZ_TAG_MISSR_LIMIT, missrouting)) )
   {
      TPZString err;
      err.sprintf(ERR_TPZCROSS_002, (char*)TPZ_TAG_MISSR_LIMIT );
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
   
   unsigned inp = inputs.asInteger();   
   unsigned outp = outputs.asInteger();   
   unsigned missl= missrouting.asInteger();
   
   TPZMultiportIOFifo* buffer = new TPZMultiportIOFifo(idBuffer,size,inp,outp,missl,control);
   buffer->setDataDelay(delay);  
   ((TPZRouter*)owner)->addMultiportIOBuffer(buffer);
   
   return buffer;
}

//*************************************************************************

// end of file

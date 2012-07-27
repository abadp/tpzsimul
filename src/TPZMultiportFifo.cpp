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
//    File: TPZMultiportFifo.cpp
//
//    Class:  TPZMultiportFifo
//
//    Inherited from:  TPZRunnableComponent
// :
//*************************************************************************
//end of header




#include <TPZMultiportFifo.hpp>

#ifndef __TPZMultiportFifoFlow_HPP__
#include <TPZMultiportFifoFlow.hpp>
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

IMPLEMENT_RTTI_DERIVED(TPZMultiportFifo,TPZRunnableComponent);

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
//  f: TPZMultiportFifo (const TPZComponentId & id);
//
//  d:
//:
//*************************************************************************

TPZMultiportFifo :: TPZMultiportFifo( const TPZComponentId& id,
                                unsigned bufferSize, unsigned numberPorts,
                                TPZBufferControl control )
               : TPZRunnableComponent(id),
                 m_BufferSize(bufferSize),
                 m_BufferControl(control),
                 m_inputs(numberPorts)
{
     setNumberOfInputs(numberPorts);
}


//*************************************************************************
//:
//  f: virtual ~ TPZMultiportFifo ();
//
//  d:
//:
//*************************************************************************

TPZMultiportFifo :: ~TPZMultiportFifo()
{

}


//*************************************************************************
//:
//  f: virtual void buildFlowControl ();
//
//  d:
//:
//*************************************************************************

void TPZMultiportFifo :: buildFlowControl()
{
   if( getBufferControl() == TPZMultiportFifo::CT )
   {
      setFlowControl( new TPZMultiportFifoFlow(*this) );
   }
   else
   {
      TPZString err;
      err.sprintf("Only CT flow control implemented");
      EXIT_PROGRAM(err);  
   }
   
   getFlowControl()->initialize();
}


//*************************************************************************
//:
//  f: virtual TPZString AsString () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZMultiportFifo :: asString() const
{
   TPZString string = getName();
   if( getRouter() )
      string += ((TPZRouter*)getRouter())->getPosition().asString();
   return string;
}


//*************************************************************************
//:
//  f: static TPZMultiportFifo * newFrom (TPZTag const * tag, TPZComponent * owner);
//
//  d:
//:
//*************************************************************************

TPZMultiportFifo* TPZMultiportFifo :: newFrom(const TPZTag* tag, TPZComponent* owner)
{
   TPZComponentId idBuffer(*tag);
   TPZString bufferSize, bufferControl, dataDelay, inputs;
   TPZString bufferShort;

   unsigned size,sizeShort, delay=1;
   
   if( tag->getAttributeValueWithName(TPZ_TAG_SIZE, bufferSize) )
   {
      size = bufferSize.asInteger();
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
   //Added VPC (number of input ports)
   if( ! (tag->getAttributeValueWithName(TPZ_TAG_INPUTS, inputs)) )
   {
      TPZString err;
      err.sprintf(ERR_TPZCROSS_002, (char*)TPZ_TAG_INPUTS );
      EXIT_PROGRAM(err);
   }
   
   TPZBufferControl control = TPZMultiportFifo::NIL;
   if( bufferControl == TPZ_TAG_CUTHROUGH )
   {
      control = TPZMultiportFifo::CT;
   }
   else
   {
      TPZString err;
      err.sprintf( ERR_TPZFIFO_002, (char*)bufferControl );
      EXIT_PROGRAM(err);
   }
   
   //VPVAdded
   unsigned inp = inputs.asInteger();   
   TPZMultiportFifo* buffer = new TPZMultiportFifo(idBuffer,size,inp,control);
   buffer->setOwner(owner);
   buffer->setDataDelay(delay-1);
   
   ((TPZRouter*)owner)->addMultiportBuffer(buffer);
   
   return buffer;
}
   
   
//*************************************************************************

// end of file

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




#include <TPZFifoMemory.hpp>

#ifndef __TPZFifoMemoryFlowWH_HPP__
#include <TPZFifoMemoryFlowWH.hpp>
#endif

#ifndef __TPZFifoMemoryFlowDamq_HPP__
#include <TPZFifoMemoryFlowDamq.hpp>
#endif

#ifndef __TPZFifoMemoryFlowCT_HPP__
#include <TPZFifoMemoryFlowCT.hpp>
#endif

#ifndef __TPZFifoMemoryFlowCTMcast_HPP__
#include <TPZFifoMemoryFlowCTMcast.hpp>
#endif

#ifndef __TPZFifoMemoryFlowWHMcast_HPP__
#include <TPZFifoMemoryFlowWHMcast.hpp>
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

IMPLEMENT_RTTI_DERIVED(TPZFifoMemory,TPZRunnableComponent);

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
//  f: TPZFifoMemory (const TPZComponentId & id);
//
//  d:
//:
//*************************************************************************

TPZFifoMemory :: TPZFifoMemory( const TPZComponentId& id,
                                unsigned bufferSize,
                                TPZBufferControl control )
               : TPZRunnableComponent(id),
                 m_BufferSize(bufferSize),
                 m_BufferControl(control)
{

}


//*************************************************************************
//:
//  f: virtual ~ TPZFifoMemory ();
//
//  d:
//:
//*************************************************************************

TPZFifoMemory :: ~TPZFifoMemory()
{

}


//*************************************************************************
//:
//  f: virtual void buildFlowControl ();
//
//  d:
//:
//*************************************************************************

void TPZFifoMemory :: buildFlowControl()
{
   if( getBufferControl() == TPZFifoMemory::WH )
   {
      setFlowControl( new TPZFifoMemoryFlowWH(*this) );
   }
   else if( getBufferControl() == TPZFifoMemory::DAMQ )
   {
      setFlowControl( new TPZFifoMemoryFlowDamq(*this) );
   }
   else if( getBufferControl() == TPZFifoMemory::CT )
   {
      setFlowControl( new TPZFifoMemoryFlowCT(*this) );
   }
   else if( getBufferControl() == TPZFifoMemory::CT_MCAST )
   {
      setFlowControl( new TPZFifoMemoryFlowCTMcast(*this) );
   }
   else if( getBufferControl() == TPZFifoMemory::WH_MCAST )
   {
      setFlowControl( new TPZFifoMemoryFlowWHMcast(*this) );
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

unsigned TPZFifoMemory :: bufferHoles() const
{
   return ((TPZFifoMemoryFlow*)getFlowControl())->bufferHoles();
}

//*************************************************************************
//:
//  f: bufferOccupation unsigned () const;
//
//  d:
//:
//*************************************************************************

unsigned TPZFifoMemory :: bufferOccupation() const
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

void TPZFifoMemory :: setBufferSize(unsigned size)
{
   m_BufferSize = size;
   if( getFlowControl() )
   {
      ((TPZFifoMemoryFlow*)getFlowControl()) -> setBufferSize(size);
   }
}

//*************************************************************************
//:
//  f: virtual Boolean bubbleReady () const;
//
//  d: bubble Return information for routing decision unit.
//:
//*************************************************************************

Boolean TPZFifoMemory ::bubbleReady() const
{

      return ((TPZFifoMemoryFlow*)getFlowControl()) -> bubbleReady();

}

//*************************************************************************
//:
//  f: virtual TPZString AsString () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZFifoMemory :: asString() const
{
   TPZString string = getName();
   if( getRouter() )
      string += ((TPZRouter*)getRouter())->getPosition().asString();
   return string;
}


//*************************************************************************
//:
//  f: static TPZFifoMemory * newFrom (TPZTag const * tag, TPZComponent * owner);
//
//  d:
//:
//*************************************************************************

TPZFifoMemory* TPZFifoMemory :: newFrom(const TPZTag* tag, TPZComponent* owner)
{
   TPZComponentId idBuffer(*tag);
   TPZString bufferSize, bufferControl, dataDelay;
   
   unsigned size, granularity=0, delay=1;
   
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

  
   TPZBufferControl control = TPZFifoMemory::NIL;
   if( bufferControl == TPZ_TAG_WORMHOLE )
   {
      control = TPZFifoMemory::WH;
   }
   else if( bufferControl == TPZ_TAG_DAMQ )
   {
      control = TPZFifoMemory::DAMQ;
   }
   else if( bufferControl == TPZ_TAG_CUTHROUGH )
   {
      control = TPZFifoMemory::CT;
   }
   else if( bufferControl == TPZ_TAG_CUTHROUGH_MULTICAST )
   {
      control = TPZFifoMemory::CT_MCAST;
   }
   else if( bufferControl == TPZ_TAG_WORMHOLE_MULTICAST )
   {
      control = TPZFifoMemory::WH_MCAST;
   }
   else
   {
      TPZString err;
      err.sprintf( ERR_TPZFIFO_002, (char*)bufferControl );
      EXIT_PROGRAM(err);
   }
   
   TPZFifoMemory* buffer = new TPZFifoMemory(idBuffer,size,control);
   buffer->setDataDelay(delay-1);
      
   ((TPZRouter*)owner)->addBuffer(buffer);
      
   return buffer;
}

//*************************************************************************

// end of file

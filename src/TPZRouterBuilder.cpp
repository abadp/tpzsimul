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
//    File: TPZRouterBuilder.cpp
//
//    Class:  TPZRouterBuilder
//
//    Inherited from:  TPZBuilder
// :
//*************************************************************************
//end of header


#include <TPZRouterBuilder.hpp>

#ifndef __TPZTag_HPP__
#include <TPZTag.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

#ifndef __TPZFifoMemory_HPP__
#include <TPZFifoMemory.hpp>
#endif

#ifndef __TPZMultiportFifo_HPP__
#include <TPZMultiportFifo.hpp>
#endif

#ifndef __TPZInputStage_HPP__
#include <TPZInputStage.hpp>
#endif

#ifndef __TPZOutputStage_HPP__
#include <TPZOutputStage.hpp>
#endif

#ifndef __TPZMultiportIOFifo_HPP__
#include <TPZMultiportIOFifo.hpp>
#endif

#ifndef __TPZRouting_HPP__
#include <TPZRouting.hpp>
#endif

#ifndef __TPZRoutingMuxed_HPP__
#include <TPZRoutingMuxed.hpp>
#endif

#ifndef __TPZCrossbar_HPP__
#include <TPZCrossbar.hpp>
#endif

#ifndef __TPZMultiplexor_HPP__
#include <TPZMultiplexor.hpp>
#endif

#ifndef __TPZDemultiplexor_HPP__
#include <TPZDemultiplexor.hpp>
#endif

#ifndef __TPZConnection_HPP__
#include <TPZConnection.hpp>
#endif

#ifndef __TPZInjector_HPP__
#include <TPZInjector.hpp>
#endif

#ifndef __TPZConsumer_HPP__
#include <TPZConsumer.hpp>
#endif

#ifndef __TPZSimpleRouter_HPP__
#include <TPZSimpleRouter.hpp>
#endif

#ifndef __TPZConst_H__
#include <TPZConst.hpp>
#endif


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZRouterBuilder,TPZBuilder);

//*************************************************************************
//:
//  f: TPZRouterBuilder (const TPZString & sgmlFile)
//
//  d:
//:
//*************************************************************************

TPZRouterBuilder :: TPZRouterBuilder(const TPZString& sgmlFile)
                  : TPZBuilder(sgmlFile)
{

}


//*************************************************************************
//:
//  f: TPZComponent * parseComponentDefinition (TPZTag const * tag,
//                                             TPZComponent * owner,
//                                             INDEX & index);
//
//  d:
//:
//*************************************************************************

TPZComponent* TPZRouterBuilder::parseComponentDefinition( const TPZTag* tag,
                                                          TPZComponent* owner,
                                                          INDEX& index )
{
   TPZString tagName = tag->tagName();
   TPZComponent* rComponent = 0;

   TPZString err("TPZRouterBuilder :: ");
   err += tagName + " is unimplemented\n";
   
   if( tagName == TPZ_TAG_ROUTER )
   {
      rComponent = TPZRouter::newFrom(tag,owner,index);
   }
   else if( tagName == TPZ_TAG_SIMPLE_ROUTER )
   {
      rComponent = TPZSimpleRouter::newFrom(tag,owner,index);
   }
   else if( tagName == TPZ_TAG_BUFFER )
   {
      rComponent = TPZFifoMemory::newFrom(tag,owner);
   }
   else if( tagName == TPZ_TAG_MPBUFFER )
   {
      rComponent = TPZMultiportFifo ::newFrom(tag,owner);
   }
   else if( tagName == TPZ_TAG_ISTAGE )
   {
      rComponent = TPZInputStage ::newFrom(tag,owner);
   }
   else if( tagName == TPZ_TAG_OSTAGE )
   {
      rComponent = TPZOutputStage ::newFrom(tag,owner);
   }
   else if( tagName == TPZ_TAG_MPIOBUFFER )
   {
      rComponent = TPZMultiportIOFifo ::newFrom(tag,owner);
   }
   else if( tagName == TPZ_TAG_ROUTING )
   {
      rComponent = TPZRouting::newFrom(tag,owner);
   }
   else if( tagName == TPZ_TAG_ROUTING_MUXED )
   {
      rComponent = TPZRoutingMuxed::newFrom(tag,owner);
   }
   else if( tagName == TPZ_TAG_CROSSBAR )
   {
      rComponent = TPZCrossbar::newFrom(tag,owner,index);
   }
   else if( tagName == TPZ_TAG_MUX )
   {
      rComponent = TPZMultiplexor::newFrom(tag,owner);
   }
   else if( tagName == TPZ_TAG_DEMUX )
   {
      rComponent = TPZDemultiplexor::newFrom(tag,owner);
   }
   else if( tagName == TPZ_TAG_CONNECTION )
   {
      TPZConnection::newFrom(tag,owner);
   }
   else if( tagName == TPZ_TAG_INJECTOR )
   {
      rComponent = TPZInjector::newFrom(tag,owner);
   }
   else if( tagName == TPZ_TAG_CONSUMER )
   {
      rComponent = TPZConsumer::newFrom(tag,owner);
   }
   else
   {
      TPZString err;
      err.sprintf( ERR_TPZRBLDR_001, (char*)tagName );
      EXIT_PROGRAM(err);
   }

   return rComponent;
}


//*************************************************************************


// end of file

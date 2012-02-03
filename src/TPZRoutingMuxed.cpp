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
//    File: TPZRoutingMuxed.cpp
//
//    Class:  TPZRoutingMuxed
//
//    Inherited from:  TPZRouting
// :
//*************************************************************************
//end of header




#include <TPZRoutingMuxed.hpp>

#ifndef __TPZRoutingMuxedFlow_HPP__
#include <TPZRoutingMuxedFlow.hpp>
#endif

#ifndef __TPZRoutingMuxedFlowBubble_HPP__
#include <TPZRoutingMuxedFlowBubble.hpp>
#endif

#ifndef __TPZRoutingMuxedFlowBubbleAdap_HPP__
#include <TPZRoutingMuxedFlowBubbleAdap.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

#ifndef __TPZRunnableComponent_HPP__
#include <TPZRunnableComponent.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

#ifndef __TPZConst_H__
#include <TPZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZRoutingMuxed,TPZRunnableComponent);

//*************************************************************************
//:
//  f: inline TPZRoutingControl getRoutingControl () const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: TPZRoutingMuxed (const TPZComponentId & id, TPZRoutingControl control);
//
//  d:
//:
//*************************************************************************

TPZRoutingMuxed :: TPZRoutingMuxed( 
                             const TPZComponentId& id,
                        TPZRoutingMuxedControl control,
                             unsigned inputs 
                             )
            : TPZRunnableComponent(id),
              m_RoutingControl(control)
{
       setNumberOfInputs(inputs);
}


//*************************************************************************
//:
//  f: virtual ~ TPZRoutingMuxed ();
//
//  d:
//:
//*************************************************************************

TPZRoutingMuxed :: ~TPZRoutingMuxed()
{

}


//*************************************************************************
//:
//  f: virtual void buildFlowControl ();
//
//  d:
//:
//*************************************************************************

void TPZRoutingMuxed :: buildFlowControl()
{
   TPZString err;
   err.sprintf("TPZRoutingMuxed :: %d is UNIMPLEMENTED", getRoutingMuxedControl() );
   
   if( getRoutingMuxedControl() == TPZRoutingMuxed::MUXED_ADAP_BUBBLE )
   {
      setFlowControl( new TPZRoutingMuxedFlowBubbleAdap(*this) );
   }
   else if( getRoutingMuxedControl() == TPZRoutingMuxed::MUXED_DOR )
   {
      setFlowControl( new TPZRoutingMuxedFlow(*this) );
   }
   else if( getRoutingMuxedControl() == TPZRoutingMuxed::MUXED_DOR_BUBBLE )
   {
      setFlowControl( new TPZRoutingMuxedFlowBubble(*this) );
   }
   else
   {
      TPZString err;
      err.sprintf(ERR_TPZRTING_001, getRoutingMuxedControl() );
      EXIT_PROGRAM(err);
   }
   
   if( getFlowControl() )
   {
      getFlowControl()->initialize();
   }
   else
   {
      TPZString err;
      err.sprintf(ERR_TPZRTING_001, getRoutingMuxedControl() );
      EXIT_PROGRAM(err);
   }
}

//*************************************************************************
//:
//  f: virtual TPZString AsString () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZRoutingMuxed :: asString() const
{
   TPZString string = getName();
   if( getRouter() )
      string += ((TPZRouter*)getRouter())->getPosition().asString();
   return string;
}

//*************************************************************************
//:
//  f: static TPZRoutingMuxed * newFrom (TPZTag const * tag, TPZComponent * owner);
//
//  d:
//:
//*************************************************************************

TPZRoutingMuxed* TPZRoutingMuxed :: newFrom(const TPZTag* tag, TPZComponent* owner)
{
   TPZComponentId idRouting(*tag);
   TPZString routingControl, headerDelay, dataDelay;
   TPZString inputs;
   
   
   if( ! (tag->getAttributeValueWithName(TPZ_TAG_INPUTS, inputs)) )
   {
      TPZString err;
      err.sprintf(ERR_TPZCROSS_002, (char*)TPZ_TAG_INPUTS );
      EXIT_PROGRAM(err);
   }
   //
   
   if( ! (tag->getAttributeValueWithName(TPZ_TAG_CONTROL, routingControl)) )
   {
      routingControl = ((TPZRouter*)owner)->valueForRoutingControl();
   }
   
   if( ! (tag->getAttributeValueWithName(TPZ_TAG_HEADER_DELAY, headerDelay)) )
   {
      TPZString err;
      err.sprintf( ERR_TPZRTING_002, (char*)TPZ_TAG_HEADER_DELAY );
      EXIT_PROGRAM(err);
   }
   
   if( ! (tag->getAttributeValueWithName(TPZ_TAG_DATA_DELAY, dataDelay)) )
   {
      TPZString err;
      err.sprintf( ERR_TPZRTING_002, (char*)TPZ_TAG_DATA_DELAY );
      EXIT_PROGRAM(err);
   }
   
   TPZRoutingMuxedControl control = TPZRoutingMuxed::NIL;
   
   if( routingControl == TPZ_TAG_MUXED_DOR )
   {
      control = TPZRoutingMuxed::MUXED_DOR;
   }
   else if( routingControl == TPZ_TAG_MUXED_DOR_BUBBLE )
   {
      control = TPZRoutingMuxed::MUXED_DOR_BUBBLE;
   }
   else if( routingControl == TPZ_TAG_MUXED_ADAP_BUBBLE )
   {
      control = TPZRoutingMuxed::MUXED_ADAP_BUBBLE;
   }
   else
   {
      TPZString err;
      err.sprintf(ERR_TPZRTING_003, (char*)routingControl );
      EXIT_PROGRAM(err);
   }
   
   unsigned inp = inputs.asInteger();
   TPZRoutingMuxed* routing = new TPZRoutingMuxed(idRouting,control,inp);
   
   routing->setHeaderDelay(headerDelay.asInteger());
   routing->setDataDelay(dataDelay.asInteger());
   
   return routing;
}
      
 
//*************************************************************************

// end of file

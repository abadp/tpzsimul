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
//    File: TPZRouting.cpp
//
//    Class:  TPZRouting
//
//    Inherited from:  TPZRunnableComponent
// :
//*************************************************************************
//end of header




 #include <TPZRouting.hpp>
 
 #ifndef __TPZRoutingFlow_HPP__
 #include <TPZRoutingFlow.hpp>
 #endif

 #ifndef __TPZRoutingFlowBubble_HPP__
 #include <TPZRoutingFlowBubble.hpp>
 #endif

 #ifndef __TPZRouter_HPP__
 #include <TPZRouter.hpp>
 #endif

 #ifndef __TPZConst_H__
 #include <TPZConst.hpp>
 #endif

 //*************************************************************************

 IMPLEMENT_RTTI_DERIVED(TPZRouting,TPZRunnableComponent);

 //*************************************************************************
 //:
 //  f: inline TPZRoutingControl getRoutingControl () const;
 //  d:
 //:
 //*************************************************************************


 //*************************************************************************
 //:
 //  f: TPZRouting (const TPZComponentId & id, TPZRoutingControl control);
 //
 //  d:
 //:
 //*************************************************************************

 TPZRouting :: TPZRouting( const TPZComponentId& id,
                           TPZRoutingControl control )
             : TPZRunnableComponent(id),
               m_RoutingControl(control)               
 {

 }


 //*************************************************************************
 //:
 //  f: virtual ~ TPZRouting ();
 //
 //  d:
 //:
 //*************************************************************************

 TPZRouting :: ~TPZRouting()
 {

 }


 //*************************************************************************
 //:
 //  f: virtual void buildFlowControl ();
 //
 //  d:
 //:
 //*************************************************************************

 void TPZRouting :: buildFlowControl()
 {
    
    TPZString err;
    err.sprintf("TPZRouting :: %d is unimplemented in this version: please use previous versions\n", getRoutingControl() );
     
    if( getRoutingControl() == TPZRouting::DOR )
    {
       setFlowControl( new TPZRoutingFlow(*this) );
    }
    else if( getRoutingControl() == TPZRouting::DOR_BURBLE )
    {
       setFlowControl( new TPZRoutingFlowBubble(*this) );
    }
    else 
    {
         EXIT_PROGRAM(err);
    }
    
    if( getFlowControl() )
    {
       getFlowControl()->initialize();
    }
    else
    {
       TPZString err;
       err.sprintf(ERR_TPZRTING_001, getRoutingControl() );
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

 TPZString TPZRouting :: asString() const
 {
    TPZString string = getName();
    if( getRouter() )
       string += ((TPZRouter*)getRouter())->getPosition().asString();
    return string;
 }


 //*************************************************************************
 //:
 //  f: static TPZRouting * newFrom (TPZTag const * tag, TPZComponent * owner);
 //
 //  d:
 //:
 //*************************************************************************

 TPZRouting* TPZRouting :: newFrom(const TPZTag* tag, TPZComponent* owner)
 {
    TPZComponentId idRouting(*tag);
    TPZString routingControl, headerDelay, dataDelay;

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

    
    TPZRoutingControl control = TPZRouting::NIL;
    if( routingControl == TPZ_TAG_DOR )
    {
       control = TPZRouting::DOR;
    }
    else if( routingControl == TPZ_TAG_DORBURBLE )
    {
       control = TPZRouting::DOR_BURBLE;
    }
    else
    {
       TPZString err;
       err.sprintf(ERR_TPZRTING_003, (char*)routingControl );
       EXIT_PROGRAM(err);
    }

    TPZRouting* routing = new TPZRouting(idRouting,control);

    routing->setHeaderDelay(headerDelay.asInteger());
    routing->setDataDelay(dataDelay.asInteger());
    
    return routing;
 }

 //*************************************************************************

 // end of file

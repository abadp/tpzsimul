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
//    File: TPZMultiplexor.cpp
//
//    Class:  TPZMultiplexor
//
//    Inherited from:  TPZRunnableComponent
// :
//*************************************************************************
//end of header




#include <TPZMultiplexor.hpp>

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZMultiplexorFlow_HPP__
#include <TPZMultiplexorFlow.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

#ifndef __TPZConst_H__
#include <TPZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZMultiplexor,TPZRunnableComponent);

//*************************************************************************
//:
//  f: TPZMultiplexor (const TPZComponentId & id);
//
//  d:
//:
//*************************************************************************

TPZMultiplexor :: TPZMultiplexor(const TPZComponentId& id, TPZMuxControl control)
                  : TPZRunnableComponent(id),
                    m_muxControl(control)
		    
                  
{

}

//*************************************************************************
//:
//  f: ~ TPZMultiplexor ();
//
//  d:
//:
//*************************************************************************

TPZMultiplexor :: ~TPZMultiplexor()
{

}
//*************************************************************************
//:
//  f: virtual void buildFlowControl ();
//
//  d:
//:
//*************************************************************************

void TPZMultiplexor :: buildFlowControl()
{
   if (getMuxControl()==TPZMultiplexor::CT)
   {
      setFlowControl(new TPZMultiplexorFlow(*this));
   }
   
   if (getFlowControl())
   {
      getFlowControl()->initialize();
   }
   else
   {
      TPZString err;
      err.sprintf(ERR_TPZFIFO_001, getMuxControl());
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

TPZString TPZMultiplexor :: asString() const
{
   TPZString string = getName();
   if( getRouter() )
      string += ((TPZRouter*)getRouter())->getPosition().asString();
   return string;
}


//*************************************************************************
//:
//  f: static TPZMultiplexor * newFrom (TPZTag const * tag, TPZComponent * owner);
//
//  d:
//:
//*************************************************************************

TPZMultiplexor* TPZMultiplexor :: newFrom( const TPZTag* tag, TPZComponent* owner )
{
   TPZComponentId idMultiplexor(*tag);
   TPZString muxInputs, muxControl;
   
   if( ! (tag->getAttributeValueWithName(TPZ_TAG_INPUTS, muxInputs)) )
   {
      TPZString err;
      err.sprintf( ERR_TPZRTING_002, (char*)TPZ_TAG_INPUTS );
      EXIT_PROGRAM(err);
   }
   
   if( ! (tag->getAttributeValueWithName(TPZ_TAG_MUX_CONTROL, muxControl)) )
   {
      TPZString err;
      err.sprintf( ERR_TPZRTING_002, (char*)TPZ_TAG_MUX_CONTROL );
      EXIT_PROGRAM(err);
   }
   
   TPZMuxControl control=TPZMultiplexor::NIL;
   
   if ( muxControl==TPZ_TAG_CUTHROUGH)
   {
      control=TPZMultiplexor::CT;
   }
   
   TPZMultiplexor* mux = new TPZMultiplexor(idMultiplexor, control);
   mux->setNumberOfInputs(muxInputs.asInteger());
   
   return mux;
}


//*************************************************************************


// end of file

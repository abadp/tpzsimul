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
//    File: TPZNetworkBuilder.cpp
//
//    Class:  TPZNetworkBuilder
//
//    Inherited from:  TPZBuilder
// :
//*************************************************************************
//end of header



#include <TPZNetworkBuilder.hpp>

#ifndef __TPZTag_HPP__
#include <TPZTag.hpp>
#endif

#ifndef __TPZNetworkTorus_HPP__
#include <TPZNetworkTorus.hpp>
#endif

#ifndef __TPZNetworkMesh_HPP__
#include <TPZNetworkMesh.hpp>
#endif

#ifndef __TPZNetworkMidimew_HPP__
#include <TPZNetworkMidimew.hpp>
#endif

#ifndef __TPZNetworkSquareMidimew_HPP__
#include <TPZNetworkSquareMidimew.hpp>
#endif

#ifndef __TPZConst_H__
#include <TPZConst.hpp>
#endif


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZNetworkBuilder,TPZBuilder);

//*************************************************************************
//:
//  f: TPZNetworkBuilder (const TPZString & sgmlFile)
//
//  d:
//:
//*************************************************************************

TPZNetworkBuilder :: TPZNetworkBuilder(const TPZString& sgmlFile)
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

TPZComponent* TPZNetworkBuilder::parseComponentDefinition( const TPZTag* tag,
                                                           TPZComponent* owner,
                                                           INDEX& index )
{
   TPZString tagName = tag->tagName();
   TPZComponent* rComponent = 0;

   if( tagName == TPZ_TAG_TORUSNET )
   {
      rComponent = TPZNetworkTorus::newFrom(tag,owner);
   }
   else if( tagName == TPZ_TAG_MESHNET )
   {
      rComponent = TPZNetworkMesh::newFrom(tag,owner);
   }
   else if (tagName == TPZ_TAG_MIDIMEWNET )
   {
      rComponent = TPZNetworkMidimew::newFrom(tag,owner);
   }
   else if (tagName == TPZ_TAG_SQUAREMIDIMEWNET )
   {
      rComponent = TPZNetworkSquareMidimew::newFrom(tag,owner);
   }
   else
   {
      TPZString err;
      err.sprintf( ERR_TPZNBLDR_001, (char*)tagName );
      EXIT_PROGRAM(err);
   }

   return rComponent;
}


//*************************************************************************


// end of file

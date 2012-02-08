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
//    File: TPZNetworkMesh.cpp
//
//    Class:  TPZNetworkMesh
//
//    Inherited from:  TPZNetwork
// :
//*************************************************************************
//end of header




#include <TPZNetworkMesh.hpp>

#ifndef __TPZConnection_HPP__
#include <TPZConnection.hpp>
#endif

#ifndef __TPZBuilder_HPP__
#include <TPZBuilder.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

#ifndef __TPZConst_H__
#include <TPZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZNetworkMesh,TPZNetworkTorus);

//*************************************************************************
//:
//  f: TPZNetworkMesh (const TPZComponentId & id,
//                      const TPZString & RouterOS,
//                      unsigned x,
//                      unsigned and
//                      unsigned z);
//
//  d:
//:
//*************************************************************************

TPZNetworkMesh :: TPZNetworkMesh( const TPZComponentId& id,
                                    const TPZString& routerId,
                                    unsigned x, 
                                    unsigned y, 
                                    unsigned z )
                 : TPZNetworkTorus(id,routerId,x,y,z)
{
   unsigned diametro=getDiameter();   
   initializeHistogram(diametro);
}


//*************************************************************************
//:
//  f: virtual ~ TPZNetworkMesh ();
//
//  d:
//:
//*************************************************************************

TPZNetworkMesh :: ~TPZNetworkMesh()
{

}
//*************************************************************************
//:
//  f: void routingRecord (const TPZPosition & org
//                          TPZPosition const & dst,
//                          int & deltaX,
//                          int & deltaY,
//                          int & Deltana,
//                          Boolean ordered = false);
//
//  d: Construct the routing record from source and destination positions
//     of nodes in the Grid. This is the only difference with tori
//:
//*************************************************************************

void TPZNetworkMesh :: routingRecord( const TPZPosition& org, 
                                       const TPZPosition& dst,
                                       int&  deltaX,
                                       int&  deltaY,
                                       int&  deltaZ,
                                       Boolean ordered )
{
   TPZPosition deltaPos = dst - org;
   deltaX = deltaPos.valueForCoordinate(TPZPosition::X);
   deltaY = deltaPos.valueForCoordinate(TPZPosition::Y);
   deltaZ = deltaPos.valueForCoordinate(TPZPosition::Z);
   deltaX = (deltaX>=0) ? deltaX+1 : deltaX-1;
   deltaY = (deltaY>=0) ? deltaY+1 : deltaY-1;
   deltaZ = (deltaZ>=0) ? deltaZ+1 : deltaZ-1;}

//*************************************************************************
//:
//  f: getDiameter unsigned long () const;
//
//  d:
//:
//*************************************************************************
unsigned TPZNetworkMesh::getDiameter() const
{
   unsigned p=getSizeX();
   unsigned q=getSizeY();
   unsigned r=getSizeZ();
   return p+q+r-3;
}
//*************************************************************************
//:
//  f: void generateDORMasks()
//
//  d:
//:
//************************************************************************* 
void TPZNetworkMesh::generateDORMasks(TPZRouter* router) 
{
    TPZPosition pos=router->getPosition();
    
    unsigned sx=getSizeX();
    unsigned sy=getSizeY();
    unsigned sz=getSizeZ();
    
    unsigned posx=pos.valueForCoordinate(TPZPosition::X);
    unsigned posy=pos.valueForCoordinate(TPZPosition::Y);
    unsigned posz=pos.valueForCoordinate(TPZPosition::Z);
    
    unsigned numNodes=sx*sy*sz;

    unsigned nodeNumber= posx+posy*sx+posz*sx*sy;
    unsigned long long maskXplus=0;
    unsigned long long maskXminus=0;
    unsigned long long maskYplus=0;
    unsigned long long maskYminus=0;
    unsigned long long maskZplus=0;
    unsigned long long maskZminus=0;
    unsigned long long maskLocalNode=0;
    
    unsigned long long maskprimamesh=1;
    
    maskLocalNode=(maskprimamesh << nodeNumber);
    
    for (unsigned deep=posz+1; deep<sz; deep++) 
    {
	unsigned long long maskaux=1;
	unsigned aux= (nodeNumber + sx*sy*(deep-posz));
	maskZplus= maskZplus + (maskaux << aux);
    }
    
    for (unsigned deep=0; deep<posz; deep++)
    {
        unsigned long long maskaux=1;
	unsigned aux= (nodeNumber - sx*sy*(deep+1));
	maskZminus= maskZminus + (maskaux << aux);
    }
    
    for (unsigned fil=posy+1; fil<sy; fil++)
    {
        unsigned long long Mascara=0;
	unsigned long long Mascara_aux2=0;
	unsigned long long Mascara_aux=1;
	for (unsigned deep=0; deep<sz; deep++)
	{
	   unsigned Position=deep*sx*sy+fil*sx+posx;
	   Mascara_aux2=Mascara_aux<<Position;
           maskYplus=maskYplus|Mascara_aux2;
	}
    }
    
    for (unsigned fil=0; fil<posy; fil++)
    {
        unsigned long long Mascara=0;
	unsigned long long Mascara_aux2=0;
	unsigned long long Mascara_aux=1;
	for (unsigned deep=0; deep<sz; deep++)
	{
	   unsigned Position=deep*sx*sy+fil*sx+posx;
	   Mascara_aux2=Mascara_aux<<Position;
           maskYminus=maskYminus|Mascara_aux2;
	}
    }
    
    for (unsigned col=posx+1; col<sx; col++) 
    {
	unsigned long long Mascara=0;
	unsigned long long Mascara_aux2=0;
	unsigned long long Mascara_aux=1;
	for (unsigned fil=0; fil<sy; fil++) 
	{
	   for (unsigned deep=0; deep<sz; deep++)
	   {
	      unsigned Position=deep*sx*sy+fil*sx+(col%sx);
	      Mascara_aux2=Mascara_aux<<Position;
              maskXplus=maskXplus|Mascara_aux2;
	   }
	}
    }

    for (unsigned col2=0; col2<posx; col2++) 
    {
	unsigned long long Mascara=0;
	unsigned long long Mascara_aux2=0;
	unsigned long long Mascara_aux=1;
	for (unsigned fil2=0; fil2<sy; fil2++) 
	{
	   for (unsigned deep2=0; deep2<sz; deep2++)
	   {
	      unsigned Position=deep2*sx*sy+fil2*sx+(col2%sx);
	      Mascara_aux2=Mascara_aux<<Position;
	      maskXminus=maskXminus|Mascara_aux2;
	   }
	}
    }
    
    router->setMask(_Xplus_ , maskXplus);
    router->setMask(_Xminus_ , maskXminus);
    router->setMask(_Yplus_ , maskYplus);
    router->setMask(_Yminus_ , maskYminus);
    router->setMask(_Zplus_ , maskZplus);
    router->setMask(_Zminus_ , maskZminus);
    router->setMask(_LocalNode_ , maskLocalNode);
        
}

//*************************************************************************
//:
//  f: virtual TPZString AsString () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZNetworkMesh :: asString() const
{
   TPZGlobalData& data = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)this);
   TPZString rs= "Mesh(";
   rs +=  TPZString(getSizeX()) + "," +
          TPZString(getSizeY()) + "," +
          TPZString(getSizeZ()) + ")" +
          "\n Buffer control : " + data.routerBufferControl() +
          "\n Routing control: " + data.routerRoutingControl();
   return rs;
}
//*************************************************************************
// end of file

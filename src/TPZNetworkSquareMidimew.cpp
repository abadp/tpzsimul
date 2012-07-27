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
//    File: TPZNetworkSquareMidimew.cpp
//
//    Class:  TPZNetworkSquareMidimew
//
//    Inherited from:  TPZNetwork
// :
//*************************************************************************
//end of header




#include <TPZNetworkSquareMidimew.hpp>

#ifndef __TPZConnection_HPP__
#include <TPZConnection.hpp>
#endif

#ifndef __TPZBuilder_HPP__
#include <TPZBuilder.hpp>
#endif

#include <math.h>

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

#ifndef __TPZConst_H__
#include <TPZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZNetworkSquareMidimew,TPZNetwork);

//*************************************************************************
//:
//  f: TPZNetworkSquareMidimew (const TPZComponentId & id,
//                      const TPZString & RouterOS,
//                      unsigned x,
//                      unsigned and
//                      unsigned z);
//
//  d:
//:
//*************************************************************************

TPZNetworkSquareMidimew :: TPZNetworkSquareMidimew( const TPZComponentId& id,
                                    const TPZString& routerId,
                                    unsigned x, 
                                    unsigned y, 
                                    unsigned z )
                 : TPZNetwork(id,routerId,x,y,z)
{
   unsigned diametro=getDiameter();   
   initializeHistogram(diametro);
}


//*************************************************************************
//:
//  f: virtual ~ TPZNetworkSquareMidimew ();
//
//  d:
//:
//*************************************************************************

TPZNetworkSquareMidimew :: ~TPZNetworkSquareMidimew()
{

}


//*************************************************************************
//:
//  f: virtual void initialize ();
//
//  d:
//:
//*************************************************************************

void TPZNetworkSquareMidimew :: initialize()
{
   if( isInitializated() ) return;
   
   /* Check consistency of parameters */
   if(getSizeY()!=1 ||getSizeZ()!=1)
   {
     TPZString err;
     err.sprintf( ERR_TPZTONET_002, (char*)getRouterId() );
     err.sprintf("\nSquare Midimew Network Ysize and Zsize must be 1. Al nodes in Xsize!");
     EXIT_PROGRAM(err); 
   }

   if((int)ceil(sqrt((float)getSizeX()))%2 || getSizeX() < 4 )
   {
     TPZString err;
     err.sprintf( ERR_TPZTONET_002, (char*)getRouterId() );
     err.sprintf("\n Square Midimew Network Xsize must be an even 2's power(4,16,64,256,1024,4096...)");
     EXIT_PROGRAM(err); 
   }

   if( !getSizeX() || !getSizeY() || !getSizeZ() )
   {
      TPZString err;
      err.sprintf(ERR_TPZTONET_001, getSizeX(), getSizeY(), getSizeZ() );
      EXIT_PROGRAM(err);
   }

   /* Create the routers for each node */
   int i;     
   for( i=0; i<getSizeX(); i++ )
   {
      TPZPosition pos(i,0,0);
      TPZRouter* newRouter = (TPZRouter*)
                             (TPZRouter::routerBuilder->createComponentWithId(getRouterId(),this));
      if( ! newRouter )
      {
          TPZString err;
          err.sprintf( ERR_TPZTONET_002, (char*)getRouterId() );
          EXIT_PROGRAM(err);
      }
      newRouter->setPosition(pos);                       
      addRouter(newRouter);
   }

   /* Connect the routers together */
   for( i=0; i<getSizeX(); i++ )
   {
      initializeConnectionsFor(TPZPosition(i,0,0));
   }
  
   /* Other initialization tasks */
   generateRoutingTable();

   setInitializated(true);
   setSimulation(getSimulation());
}


//*************************************************************************
//:
//  f: unsigned distance (const TPZPosition & org, const TPZPosition & dst);
//
//  d:
//:
//*************************************************************************

unsigned TPZNetworkSquareMidimew :: distance(const TPZPosition& org, const TPZPosition& dst)
{
   /* Calculate topological distance between two nodes */
   int deltaX, deltaY, deltaZ;
   routingRecord(org,dst,deltaX,deltaY,deltaZ);
   return int( abs(deltaX) + abs(deltaY) + abs(deltaZ) - 3 );
}


//*************************************************************************
//:
//  f: void routingRecord (const TPZPosition & org
//                          TPZPosition const & dst,
//                          int & deltaX,
//                          int & deltaY,
//                          Deltana int &);
//
//  d:
//:
//*************************************************************************

void TPZNetworkSquareMidimew :: routingRecord( const TPZPosition& org, 
                                       const TPZPosition& dst,
                                       int&  deltaX,
                                       int&  deltaY,
                                       int&  deltaZ,
                                       Boolean ordered  )
{
   /* Calculate routing record */
   int origen=org.valueForCoordinate(TPZPosition::X);
   int destino=dst.valueForCoordinate(TPZPosition::X);
   int m = (destino-origen+getSizeX()) % getSizeX();
   
   /* Use lookup table */
   deltaX = m_routingTable[2*(m-1)];
   deltaY = m_routingTable[2*(m-1)+1];
   deltaZ = 0;
   
   /* Routing record values must be incremented by 1 or -1 
    * depending on their sign */
   deltaX = (deltaX>=0) ? deltaX+1 : deltaX-1;
   deltaY = (deltaY>=0) ? deltaY+1 : deltaY-1;
   deltaZ = 1;//(deltaZ>=0) ? deltaZ+1 : deltaZ-1;
}
//*************************************************************************
//:
//  f: TPZPosition CreatePosition (int num);
//
//  d:
//:
//*************************************************************************
TPZPosition TPZNetworkSquareMidimew::CreatePosition(int num)     
{  
   return TPZPosition(num,0,0);
}

//*************************************************************************
//:
//  f: static TPZNetworkSquareMidimew * newFrom (TPZTag const * tag, TPZComponent * owner);
//
//  d:
//:
//*************************************************************************

TPZNetworkSquareMidimew* TPZNetworkSquareMidimew :: newFrom( const TPZTag* tag,
                                             TPZComponent* owner )
{
   TPZComponentId idNetwork(*tag);
   TPZString sizeX, sizeY, sizeZ;
   unsigned x, y=1, z=1;
   /* Read network size from tag attributes */
   if( !(tag->getAttributeValueWithName(TPZ_TAG_SIZEX, sizeX)) )
   {
      TPZString err;
      err.sprintf(ERR_TPZTONET_003, (char*)TPZ_TAG_SIZEX );
      EXIT_PROGRAM(err);
   }
   x = sizeX.asInteger();
   
/*   if( tag->getAttributeValueWithName(TPZ_TAG_SIZEY, sizeY) )
   {
      y = sizeY.asInteger();
   }
   
   if( tag->getAttributeValueWithName(TPZ_TAG_SIZEZ, sizeZ) )
   {
      z = sizeZ.asInteger();
   } */
   
   /* Check size consistency */ 
   if( !x /*|| !y || !z*/ )
   {
      TPZString err;
      err.sprintf(ERR_TPZTONET_004, x, 0, 0 );
      EXIT_PROGRAM(err);
   }
   /* Read the router identifier */
   TPZString routerId;
   if( ! (tag->getAttributeValueWithName(TPZ_TAG_REFROUTER, routerId)) )
   {
      TPZString err;
      err.sprintf(ERR_TPZTONET_003, (char*)TPZ_TAG_REFROUTER );
      EXIT_PROGRAM(err);
   }
   /* Read the connection delay */
   TPZString connectionDelay;
   unsigned delay=0;

   if( tag->getAttributeValueWithName(TPZ_TAG_DELAY, connectionDelay) )
   {
      delay = connectionDelay.asInteger();
   }
   /* Instantiation of the network class */ 
   TPZNetworkSquareMidimew* net = new TPZNetworkSquareMidimew( idNetwork, routerId, x, y, z );
   net->setConnectionDelay(delay);
   net -> setSimulation(owner);
   net -> initialize();
   
   return net;
}
//*************************************************************************
//:
//  f: getDiameter unsigned long () const;
//
//  d:
//:
//*************************************************************************
unsigned TPZNetworkSquareMidimew::getDiameter() const
{
   /* Calculate the diameter of the network */
   return (unsigned)(3*sqrt((float)getSizeX())/4);
}

//*************************************************************************
//:
//  f: virtual TPZString AsString () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZNetworkSquareMidimew :: asString() const
{
   TPZGlobalData& data = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)this);
   TPZString rs= "SquareMidimew(";
   rs +=  TPZString(getSizeX()) + "," +
          TPZString(getSizeY()) + "," +
          TPZString(getSizeZ()) + ")" +
          "\n Buffer control : " + data.routerBufferControl() +
          "\n Routing control: " + data.routerRoutingControl();
   return rs;
}

//*************************************************************************
//:
//  f: void generateRoutingTable ()
//
//  d:
//:
//*************************************************************************

void TPZNetworkSquareMidimew :: generateRoutingTable()
{
   cerr << "Generating Routing Table...\n";
   int n = getSizeX();
   m_routingTable = new int[2*(n-1)];
   int sy = (int)(sqrt((float)n));
   int sx = sy/2 - 1;
   for(int m=1; m<n; m++)
   {
      int min=n;
      for(int x=-3*sy/4; x < 3*sy/4; x++ )
      {
         for(int y=-3*sy/4; y < 3*sy/4; y++)
         {
            if(m == ((x*sx + y*sy + n) % n) && abs(x)+abs(y) <= min)
             {
               min = abs(x)+abs(y);
               m_routingTable[2*(m-1)] = x; m_routingTable[2*(m-1)+1] = y;
             }
         }
      }
   }
}


//*************************************************************************
//:
//  f: void initializeConnectionsFor (const TPZPosition & pos);
//
//  d:
//:
//*************************************************************************

void TPZNetworkSquareMidimew :: initializeConnectionsFor(const TPZPosition& pos)
{
   /* Make connections to router at position 'pos' */
   TPZRouter* router = getRouterAt(pos);
   int myNum=pos.valueForCoordinate(TPZPosition::X);
   int sy = (int)sqrt((float)getSizeX());
   int sx = sy/2 - 1;
   int n = getSizeX();


   /* Get neighbouring routers */
   TPZRouter* routerXp = getRouterAt(CreatePosition( (myNum+sx)%n ));
   TPZRouter* routerXm = getRouterAt(CreatePosition( (myNum-sx+n)%n ));
   TPZRouter* routerYp = getRouterAt(CreatePosition( (myNum+sy)%n ));
   TPZRouter* routerYm = getRouterAt(CreatePosition( (myNum-sy+n)%n ));

   /* Get input port numbers */ 
   unsigned iXp = router->getInputWithType(_Xplus_);
   unsigned iXm = router->getInputWithType(_Xminus_);
   unsigned iYp = router->getInputWithType(_Yplus_);
   unsigned iYm = router->getInputWithType(_Yminus_);
   
   /* Get output port numbers */ 
   unsigned oXp = router->getOutputWithType(_Xplus_);
   unsigned oXm = router->getOutputWithType(_Xminus_);
   unsigned oYp = router->getOutputWithType(_Yplus_);
   unsigned oYm = router->getOutputWithType(_Yminus_);
   
   /* Make the connections */
   if( iXp && oXp )
      TPZConnection::connectInterfaces( this,
                                        router->getOutputInterfaz(oXp),
                                        routerXp->getInputInterfaz(iXp) );

   if( iXm && oXm )
      TPZConnection::connectInterfaces( this,
                                        router->getOutputInterfaz(oXm),
                                        routerXm->getInputInterfaz(iXm),
                                        getConnectionDelay() );

   if( iYp && oYp )
      TPZConnection::connectInterfaces( this,
                                        router->getOutputInterfaz(oYp),
                                        routerYp->getInputInterfaz(iYp),
                                        getConnectionDelay() );

   if( iYm && oYm )
      TPZConnection::connectInterfaces( this,
                                        router->getOutputInterfaz(oYm),
                                        routerYm->getInputInterfaz(iYm),
                                        getConnectionDelay() );
}

//*************************************************************************

// end of file

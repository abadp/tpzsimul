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
//    File: TPZNetworkMidimew.cpp
//
//    Class:  TPZNetworkMidimew
//
//    Inherited from:  TPZNetwork
// :
//*************************************************************************
//end of header




#include <TPZNetworkMidimew.hpp>

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

IMPLEMENT_RTTI_DERIVED(TPZNetworkMidimew,TPZNetwork);

//*************************************************************************
//:
//  f: TPZNetworkMidimew (const TPZComponentId & id,
//                      const TPZString & RouterOS,
//                      unsigned x,
//                      unsigned and
//                      unsigned z);
//
//  d: Midimew Network. Concerning network labeling, it works as a network
//    with only one dimension.
//:
//*************************************************************************

TPZNetworkMidimew :: TPZNetworkMidimew( const TPZComponentId& id,
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
//  f: virtual ~ TPZNetworkMidimew ();
//
//  d:
//:
//*************************************************************************

TPZNetworkMidimew :: ~TPZNetworkMidimew()
{

}


//*************************************************************************
//:
//  f: virtual void initialize ();
//
//  d:
//:
//*************************************************************************

void TPZNetworkMidimew :: initialize()
{
   if( isInitializated() ) return;
   
   if(getSizeY()!=1 ||getSizeZ()!=1)
   {
     TPZString err;
     err.sprintf( ERR_TPZTONET_002, (char*)getRouterId() );
     err.sprintf("\n Midimew Network Ysize and Zsize must be =1. All nodes in Xsize!");
     EXIT_PROGRAM(err); 
   }

   if( !getSizeX() || !getSizeY() || !getSizeZ() )
   {
      TPZString err;
      err.sprintf(ERR_TPZTONET_001, getSizeX(), getSizeY(), getSizeZ() );
      EXIT_PROGRAM(err);
   }
   int i, j, k;     
   for( i=0; i<getSizeX(); i++ )
   {
      for( j=0; j<getSizeY(); j++ )
      {
         for( k=0; k<getSizeZ(); k++ )
         {
            TPZPosition pos(i,j,k);
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
      }
   }
   for( i=0; i<getSizeX(); i++ )
      for( j=0; j<getSizeY(); j++ )
         for( k=0; k<getSizeZ(); k++ )
         {
            initializeConnectionsFor(TPZPosition(i,j,k));
         }

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

unsigned TPZNetworkMidimew :: distance(const TPZPosition& org, const TPZPosition& dst)
{
   int deltaX, deltaY, deltaZ;
   routingRecord(org,dst,deltaX,deltaY,deltaZ,false);
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
//  d: Builds routing record from nodes position
//     It employs the formal solution of the algorithm
//:
//*************************************************************************

void TPZNetworkMidimew :: routingRecord( const TPZPosition& org, 
                                       const TPZPosition& dst,
                                       int&  deltaX,
                                       int&  deltaY,
                                       int&  deltaZ,
                                       Boolean ordered )
{
   
   int m,signo;
   //optimal routing record solutions
   int x0;
   int y0;
   int x1;
   int y1;
   //
   int b=(int)ceill(sqrt(getSizeX()/2.0));
   int origen=org.valueForCoordinate(TPZPosition::X);
   int destino=dst.valueForCoordinate(TPZPosition::X);
   helperRR(origen,destino,deltaX,deltaY,deltaZ,ordered);
    
   deltaX = (deltaX>=0) ? deltaX+1 : deltaX-1;
   deltaY = (deltaY>=0) ? deltaY+1 : deltaY-1;
   deltaZ = 1; //third dimension not allowed
}
//*************************************************************************
//:
//  f: helperRR(int origen, int destino, int& deltaX, int& deltaY, int& deltaZ,Boolean ordered ) const
//
//  d: 
//:
//*************************************************************************

void  TPZNetworkMidimew ::helperRR( int  origen, int destino, int&  deltaX, int&  deltaY, int&  deltaZ,Boolean ordered ) const
{
    int m,signo;
   //optimal routing record solutions
   int x0;
   int y0;
   int x1;
   int y1;
   //
   int b=(int)ceill(sqrt(getSizeX()/2.0));
   //I need to decide when the message is generated according to the torus
   
    m = abs(destino-origen);
   
    if(destino<=origen)
       signo=-1;
    else
       signo=1;
    
    if (m>getSizeX()/2){
       signo=-signo;
       m=getSizeX()-m;
    }
    //Valid solutions for Diophantine ecuation
    //of circulating graphs
    y0=-m%b;
    x0=-y0+m/b;
    y1=b+y0;
    x1=x0-(b-1);   
    //
    if(abs(x0)+abs(y0)<abs(x1)+abs(y1))
    {
       deltaX=signo*x0;
       deltaY=signo*y0;
    }
    else
    {
       deltaX=signo*x1;
       deltaY=signo*y1;
    }
   if(abs(x1)+abs(y1)==abs(x0)+abs(y0))
   {
       //Balanced utilization when point-to-point order is required
       //function of source node
       if(drand48()<0.5 && !ordered || ordered && origen%2 == 0)
       {
         deltaX=signo*x1;
         deltaY=signo*y1;  
       }
       else
       {
         deltaX=signo*x0;
         deltaY=signo*y0;
       }
    }

    if(abs(deltaX)+abs(deltaY)>getDiameter())
    {
      TPZString err;
      err.sprintf("Superado diametro de la red\n");
      EXIT_PROGRAM(err);
    }  
   
}
      
//*************************************************************************
//:
//  f: TPZPosition CreatePosition (int num);
//
//  d:
//:
//*************************************************************************
TPZPosition TPZNetworkMidimew::CreatePosition(int num)     
   {  
         unsigned numi=unsigned(num);        
         unsigned x = numi%unsigned(getSizeX());
         return TPZPosition(x,0);

 }

//*************************************************************************
//:
//  f: static TPZNetworkMidimew * newFrom (TPZTag const * tag, TPZComponent * owner);
//
//  d:
//:
//*************************************************************************

TPZNetworkMidimew* TPZNetworkMidimew :: newFrom( const TPZTag* tag,
                                             TPZComponent* owner )
{
   TPZComponentId idNetwork(*tag);
   TPZString sizeX, sizeY, sizeZ;
   unsigned x, y=1, z=1;
   
   if( !(tag->getAttributeValueWithName(TPZ_TAG_SIZEX, sizeX)) )
   {
      TPZString err;
      err.sprintf(ERR_TPZTONET_003, (char*)TPZ_TAG_SIZEX );
      EXIT_PROGRAM(err);
   }
   
   if( tag->getAttributeValueWithName(TPZ_TAG_SIZEY, sizeY) )
   {
      y = sizeY.asInteger();
   }
   
   if( tag->getAttributeValueWithName(TPZ_TAG_SIZEZ, sizeZ) )
   {
      z = sizeZ.asInteger();
   }
   
   x = sizeX.asInteger();
   
   if( !x || !y || !z )
   {
      TPZString err;
      err.sprintf(ERR_TPZTONET_004, x, y, z );
      EXIT_PROGRAM(err);
   }
   
   TPZString routerId;
   
   if( ! (tag->getAttributeValueWithName(TPZ_TAG_REFROUTER, routerId)) )
   {
      TPZString err;
      err.sprintf(ERR_TPZTONET_003, (char*)TPZ_TAG_REFROUTER );
      EXIT_PROGRAM(err);
   }

   TPZString connectionDelay;
   unsigned delay=0;

   if( tag->getAttributeValueWithName(TPZ_TAG_DELAY, connectionDelay) )
   {
      delay = connectionDelay.asInteger();
   }
   
   TPZNetworkMidimew* net = new TPZNetworkMidimew( idNetwork, routerId, x, y, z );
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
unsigned TPZNetworkMidimew::getDiameter() const
{
   unsigned b=(unsigned)ceil(sqrt(getSizeX()/2.0));
   if (getSizeX()<=2*b*b-2*b+1)
         return b-1;
   else
         return b;

}

//*************************************************************************
//:
//  f: virtual TPZString AsString () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZNetworkMidimew :: asString() const
{
   TPZGlobalData& data = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)this);
   TPZString rs= "Midimew(";
   rs +=  TPZString(getSizeX()) + "," +
          TPZString(getSizeY()) + "," +
          TPZString(getSizeZ()) + ")" +
          "\n Buffer control : " + data.routerBufferControl() +
          "\n Routing control: " + data.routerRoutingControl();;
   return rs;
}

//*************************************************************************
//:
//  f: void initializeConnectionsFor (const TPZPosition & pos);
//
//  d:
//:
//*************************************************************************

void TPZNetworkMidimew :: initializeConnectionsFor(const TPZPosition& pos)
{

   TPZRouter* router = getRouterAt(pos);
   int myNum=pos.valueForCoordinate(TPZPosition::X);
   int b=(int)ceill(sqrt(getSizeX()/2.0));
   
   TPZRouter* routerXp = getRouterAt(CreatePosition((myNum+b)%getSizeX()));
   TPZRouter* routerXm = getRouterAt(CreatePosition((myNum-b+getSizeX())%getSizeX()));
   TPZRouter* routerYp = getRouterAt(CreatePosition((myNum+(b-1))%getSizeX()));
   TPZRouter* routerYm = getRouterAt(CreatePosition((myNum-(b-1)+getSizeX())%getSizeX()));
   TPZRouter* routerZp = getRouterAt( positionOf(_Zplus_,pos)  );  
   TPZRouter* routerZm = getRouterAt( positionOf(_Zminus_,pos) );


   unsigned iXp = router->getInputWithType(_Xplus_);
   unsigned iXm = router->getInputWithType(_Xminus_);
   unsigned iYp = router->getInputWithType(_Yplus_);
   unsigned iYm = router->getInputWithType(_Yminus_);
   unsigned iZp = router->getInputWithType(_Zplus_);
   unsigned iZm = router->getInputWithType(_Zminus_);
   
   unsigned oXp = router->getInputWithType(_Xplus_);
   unsigned oXm = router->getInputWithType(_Xminus_);
   unsigned oYp = router->getInputWithType(_Yplus_);
   unsigned oYm = router->getInputWithType(_Yminus_);
   unsigned oZp = router->getInputWithType(_Zplus_);
   unsigned oZm = router->getInputWithType(_Zminus_);

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

   if( iZp && oZp )
      TPZConnection::connectInterfaces( this,
                                        router->getOutputInterfaz(oZp),
                                        routerZp->getInputInterfaz(iZp),
                                        getConnectionDelay() );

   if( iZm && oZm )
      TPZConnection::connectInterfaces( this,
                                        router->getOutputInterfaz(oZm),
                                        routerZm->getInputInterfaz(iZm),
                                        getConnectionDelay() );

}

//*************************************************************************

// end of file

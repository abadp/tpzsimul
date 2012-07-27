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
//    File: TPZNetworkTorus.cpp
//
//    Class:  TPZNetworkTorus
//
//    Inherited from:  TPZNetwork
// :
//*************************************************************************
//end of header




#include <TPZNetworkTorus.hpp>

#ifndef __TPZNetworkMesh_HPP__
#include <TPZNetworkMesh.hpp>
#endif

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

IMPLEMENT_RTTI_DERIVED(TPZNetworkTorus,TPZNetwork);

//*************************************************************************
//:
//  f: TPZNetworkTorus (const TPZComponentId & id,
//                      const TPZString & RouterOS,
//                      unsigned x,
//                      unsigned and
//                      unsigned z);
//
//  d:
//:
//*************************************************************************

TPZNetworkTorus :: TPZNetworkTorus( const TPZComponentId& id,
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
//  f: virtual ~ TPZNetworkTorus ();
//
//  d:
//:
//*************************************************************************

TPZNetworkTorus :: ~TPZNetworkTorus()
{

}


//*************************************************************************
//:
//  f: virtual void initialize ();
//
//  d:
//:
//*************************************************************************

void TPZNetworkTorus :: initialize()
{
   if( isInitializated() ) return;

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
      {
         for( k=0; k<getSizeZ(); k++ )
         {
            initializeConnectionsFor(TPZPosition(i,j,k));
         }
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

unsigned TPZNetworkTorus :: distance(const TPZPosition& org, const TPZPosition& dst)
{
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
//  d: Build routing record from source and destination positions of nodes.
//     Check that distance does not exceed radius/2 of the network. 
//     If you have 2 directions equivalent, we choose randomly one of them;
//:
//*************************************************************************

void TPZNetworkTorus :: routingRecord( const TPZPosition& org, 
                                       const TPZPosition& dst,
                                       int&  deltaX,
                                       int&  deltaY,
                                       int&  deltaZ,
                                       Boolean ordered)
{
   Boolean balancing=false; 
   TPZPosition deltaPos = dst - org;
   deltaX = deltaPos.valueForCoordinate(TPZPosition::X);
   deltaY = deltaPos.valueForCoordinate(TPZPosition::Y);
   deltaZ = deltaPos.valueForCoordinate(TPZPosition::Z);
   
   int total=deltaX+deltaY+deltaZ;
   
   int absDeltaX = (deltaX>0) ? deltaX : -deltaX;
   int absDeltaY = (deltaY>0) ? deltaY : -deltaY;
   int absDeltaZ = (deltaZ>0) ? deltaZ : -deltaZ;
   
   int R2x = int(getSizeX()/2);
   int R2y = int(getSizeY()/2);
   int R2z = int(getSizeZ()/2);
    
   
   if( (absDeltaX > R2x) ||
       ((absDeltaX==R2x) && (getSizeX()%2==0) &&  (drand48()<0.5 && !ordered)) )
   {
      deltaX = (deltaX>0) ? (deltaX-getSizeX()) : (deltaX+getSizeX());
   }

   if( (absDeltaY > R2y) ||
       ((absDeltaY==R2y) && (getSizeY()%2==0) && (drand48()<0.5 && !ordered)) )
   {
      deltaY = (deltaY>0) ? (deltaY-getSizeY()) : (deltaY+getSizeY());
   }

   if( (absDeltaZ > R2z) || 
       ((absDeltaZ==R2z) && (getSizeZ()%2==0) && (drand48()<0.5 && !ordered)) )
   {
      deltaZ = (deltaZ>0) ? (deltaZ-getSizeZ()) : (deltaZ+getSizeZ());
   }

   
   if( abs(deltaX)+abs(deltaY)+abs(deltaZ)>(getSizeX()/2+getSizeY()/2+getSizeZ()/2))
   {
      TPZString err;
      err.sprintf(ERR_TPZTONET_003, "diameter Overflow\n");
      EXIT_PROGRAM(err);
   }
   
   deltaX = (deltaX>=0) ? deltaX+1 : deltaX-1;
   deltaY = (deltaY>=0) ? deltaY+1 : deltaY-1;
   deltaZ = (deltaZ>=0) ? deltaZ+1 : deltaZ-1;
   

}

//*************************************************************************
//:
//  f: static TPZNetworkTorus * newFrom (TPZTag const * tag, TPZComponent * owner);
//
//  d:
//:
//*************************************************************************

TPZNetworkTorus* TPZNetworkTorus :: newFrom( const TPZTag* tag,
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
   TPZString operationMode;
   unsigned delay=0;

   TPZNetworkTorus* net;
   if( tag->getAttributeValueWithName(TPZ_TAG_CHANNEL_MODE, operationMode) )
   {
      cout << operationMode << " mode building...\n";
   }
   if( tag->getAttributeValueWithName(TPZ_TAG_DELAY, connectionDelay) )
   {
      delay = connectionDelay.asInteger();
   }
   if(tag->tagName()==TPZ_TAG_TORUSNET)
   {
      net = new TPZNetworkTorus( idNetwork, routerId, x, y, z );
   }
   else if(tag->tagName()==TPZ_TAG_MESHNET)
   {
      net = new TPZNetworkMesh( idNetwork, routerId, x, y, z );
   }
   else
   {
      TPZString err;
      err.sprintf( ERR_TPZNBLDR_001, (char*)tag->tagName() );
      EXIT_PROGRAM(err);
   }
      
   net->setConnectionDelay(delay);
   net->setChannelOperationMode(operationMode);
   
   net->setOwner(owner);
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
 unsigned TPZNetworkTorus::getDiameter() const
{
   unsigned maximo;
   unsigned dimension;
   if(getSizeX()>=getSizeY())
      maximo=getSizeX();
   else
      maximo=getSizeY();
   
   if(getSizeZ()>=maximo)
      maximo=getSizeZ();
   
   if(getSizeZ()!=1)
      dimension=3;
   else
      dimension=2;

   return dimension*(int)(maximo/2)+1;
}
//*************************************************************************
//:
//  f: virtual TPZString AsString () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZNetworkTorus :: asString() const
{
   TPZGlobalData& data = ((TPZSimulation*)getSimulation())->globalData();
   TPZString rs= "Torus(";
   rs +=  TPZString(getSizeX()) + "," +
          TPZString(getSizeY()) + "," +
          TPZString(getSizeZ()) + ")" +
          "\n Buffer control : " + data.routerBufferControl() +
          "\n Routing control: " + data.routerRoutingControl();
   return rs;
}

//*************************************************************************
//:
//  f: void generateDORMasks()
//
//  d:
//:
//************************************************************************* 
void TPZNetworkTorus::generateDORMasks(TPZRouter* router) 
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
    
    unsigned long long mask_aux=1;
    unsigned long long mask_aux_2=0;
    
    maskLocalNode=(mask_aux << nodeNumber);
        
    for (unsigned  deep=1; deep<=sz/2; deep++)
    {
       maskZplus+= (mask_aux << ((nodeNumber + sx*sy*deep)%numNodes) );
    }
    
    for (unsigned deep=posz+1+sz/2; deep<posz+sz; deep++)
    {
       maskZminus+= (mask_aux << ((deep%sz)*sx*sy+posx+posy*sx) );
    }
    
    for (unsigned row=posy+1; row<=posy+sy/2; row++)
    {
       for (unsigned deep=0; deep<sz; deep++)
       {
          maskYplus+= ( mask_aux << (deep*sx*sy+sx*(row%sy)+posx) );
       }
    }
     
    for (unsigned row=posy+1+sy/2; row<posy+sy; row++)
    {
       for (unsigned deep=0; deep<sz; deep++)
       {
          maskYminus+= ( mask_aux << (deep*sx*sy+sx*(row%sy)+posx) );
       }
    }
    
    for (unsigned col=posx+1; col<=posx+sx/2; col++)
    {
       for (unsigned row=0; row<sy; row++)
       {
          for (unsigned deep=0; deep<sz; deep++)
	  {
	     maskXplus+= ( mask_aux << (deep*sx*sy+row*sx+(col%sx)) );
	  }
       }
    }
     
    for (unsigned col=posx+1+sx/2; col<posx+sx; col++)
    {
       for (unsigned row=0; row<sy; row++)
       {
          for (unsigned deep=0; deep<sz; deep++)
	  {
	     maskXminus+= ( mask_aux << (deep*sx*sy+row*sx+(col%sx)) );
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
//  f: unsigned long int setMulticastMask (const TPZPosition & current, const TPZROUTINGTYPE & direction);
//
//  d: Generates an integer that corresponds to a binary mask that indicates the nodes
//     reached from the port of departure from that node. So, a bit to 1 on the X position
//     X indicates that the node is accessible from the port of departure.
//:
//*************************************************************************
unsigned long long TPZNetworkTorus :: setMulticastMask(const TPZPosition& current,
                                        const TPZROUTINGTYPE& direction)
{

unsigned int N=getSizeX()*getSizeY();
unsigned int Radio=getSizeX();
unsigned long long Mascara=0;

unsigned deltaX = current.valueForCoordinate(TPZPosition::X);
unsigned deltaY = current.valueForCoordinate(TPZPosition::Y);

int col, fil;
unsigned int Position;
unsigned long long Mascara_aux=1;
unsigned long long Mascara_aux2=0;

switch (direction)
   {
   case _Yplus_:
      for (col=0; col<Radio; col++)
         {
     for ( fil=deltaY+1; fil<=deltaY+Radio/2; fil++)
        {
        Position=(fil%Radio)*Radio+col;
        Mascara_aux2=Mascara_aux<<Position;
        Mascara=Mascara|Mascara_aux2;
        }
     }
      break;
    case _Xplus_:
      for (col=deltaX+1; col<=deltaX+Radio/2; col++)
         {
     for (fil=0; fil<Radio; fil++)
        {
        Position=fil*Radio+(col%Radio);
        Mascara_aux2=Mascara_aux<<Position;
        Mascara=Mascara|Mascara_aux2;
        }
     }
      break;
     case _Xminus_:
        for (col=deltaX+Radio/2; col<deltaX+Radio; col++)
       {
       for (fil=0; fil<Radio; fil++)
          {
          Position=fil*Radio+(col%Radio);
          Mascara_aux2=Mascara_aux<<Position;
          Mascara=Mascara|Mascara_aux2;
          }
       }
    break;
      case _Yminus_:
         for (col=0; col<Radio; col++)
        {
        for (fil=deltaY+Radio/2; fil<deltaY+Radio; fil++)
           {
           Position=(fil%Radio)*Radio+col;
               Mascara_aux2=Mascara_aux<<Position;
           Mascara=Mascara|Mascara_aux2;
           }
        }
     break;
      case _LocalNode_:
         Position=deltaY*Radio+deltaX;
     Mascara_aux2=Mascara_aux<<Position;
     Mascara=Mascara|Mascara_aux2;
     break;
         
    
     default:
        break;
     }
 return Mascara;
}
   

//*************************************************************************

// end of file

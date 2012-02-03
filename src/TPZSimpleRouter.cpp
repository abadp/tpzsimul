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
//    File: TPZSimpleRouter.cpp
//
//    Class:  TPZCrossbar
//
//    Inherited from:  TPZRunnableComponent
// :
//*************************************************************************
//end of header




#include <TPZSimpleRouter.hpp>

#ifndef __TPZSimpleRouterFlow_HPP__
#include <TPZSimpleRouterFlow.hpp>
#endif

#ifndef __TPZSimpleRouterFlowWH_HPP__
#include <TPZSimpleRouterFlowWH.hpp>
#endif

#ifndef __TPZSimpleRouterFlowDAMQ_HPP__
#include <TPZSimpleRouterFlowDAMQ.hpp>
#endif

#ifndef __TPZSimpleRouterFlowTorus_HPP__
#include <TPZSimpleRouterFlowTorus.hpp>
#endif

#ifndef __TPZSimpleRouterFlowBless_HPP__
#include <TPZSimpleRouterFlowBless.hpp>
#endif

#ifndef __TPZSimpleRouterFlowFast_HPP__
#include <TPZSimpleRouterFlowFast.hpp>
#endif

#ifndef __TPZSimpleRouterFlowBinoc_HPP__
#include <TPZSimpleRouterFlowBinoc.hpp>
#endif

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

#ifndef __TPZRouterBuilder_HPP__
#include <TPZRouterBuilder.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

#ifndef __TPZConst_H__
#include <TPZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZSimpleRouter,TPZRunnableComponent);

//*************************************************************************

//*************************************************************************
//:
//  f: inline CrossbarType getCrossbarType () const;
//  d:
//
//  f: inline void setCrossbarType (CrossbarType type);
//  d:
//:
//*************************************************************************



//*************************************************************************
//:
//  f: TPZSimpleRouter (const TPZComponentId & id,
//                  unsigned inputs,
//                  unsigned output);
//
//  d:
//:
//*************************************************************************

TPZSimpleRouter :: TPZSimpleRouter( const TPZComponentId& id,
                            unsigned inputs,
                            unsigned outputs,
                              TPZRoutingControl routingControl,
                                TPZBufferControl bufferControl, unsigned vnets )
             : TPZRunnableComponent(id),
             m_RoutingControl(routingControl),
             m_BufferControl(bufferControl),
             m_vnets(vnets)
{
   setNumberOfInputs(inputs);
   setNumberOfOutputs(outputs);   
}


//*************************************************************************
//:
//  f: virtual ~ TPZSimpleRouter ();
//
//  d:
//:
//*************************************************************************

TPZSimpleRouter :: ~TPZSimpleRouter()
{
 
}

void TPZSimpleRouter :: terminate()
{
   getFlowControl()->terminate(); 
}


//*************************************************************************
//:
//  f: numberOfLocalPorts unsigned () const;
//
//  d:
//:
//*************************************************************************

unsigned TPZSimpleRouter :: numberOfLocalPorts() const
{
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));
   return gData.simpleRouterNumberOfLocalPorts();
}


//*************************************************************************
//:
//  f: void setNumberOfLocalPorts (unsigned number);
//
//  d:
//:
//*************************************************************************

void TPZSimpleRouter :: setNumberOfLocalPorts(unsigned number)
{
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));
   gData.simpleRouterSetNumberOfLocalPorts(number);
}


//*************************************************************************
//:
//  f: virtual void buildFlowControl ();
//
//  d:
//:
//*************************************************************************

void TPZSimpleRouter :: buildFlowControl()
{
   if( getRoutingControl() == TPZSimpleRouter::MESH_DOR && 
       getBufferControl() == TPZSimpleRouter::BC_CT)
   {
      setFlowControl( new TPZSimpleRouterFlow(*this) ); 
   }
   else if (getRoutingControl() == TPZSimpleRouter::MESH_DOR &&
            getBufferControl() ==  TPZSimpleRouter::BC_WH)
   {  
      setFlowControl(new TPZSimpleRouterFlowWH(*this));
   }
   else if (getRoutingControl() == TPZSimpleRouter::MESH_DOR &&
            getBufferControl() ==  TPZSimpleRouter::BC_DAMQ)
   {  
      setFlowControl(new TPZSimpleRouterFlowDAMQ(*this));
   }
   else if (getRoutingControl() == TPZSimpleRouter::MESH_DOR_FAST &&
            getBufferControl() ==  TPZSimpleRouter::BC_CT)
   {  
      setFlowControl(new TPZSimpleRouterFlowFast(*this));
   }
   else if (getRoutingControl() == TPZSimpleRouter::TORUS_DOR &&
            getBufferControl() == TPZSimpleRouter::BC_CT)
   {  
      setFlowControl(new TPZSimpleRouterFlowTorus(*this));
   }
   else if (getRoutingControl() == TPZSimpleRouter :: MESH_DOR_BINOC)
   {  
      setFlowControl(new TPZSimpleRouterFlowBinoc(*this));
   }
   else if (getRoutingControl() == TPZSimpleRouter :: TORUS_BLESS)
   {  
      setFlowControl(new TPZSimpleRouterFlowBless(*this));
   }
   if(getFlowControl())
   {
      getFlowControl() -> initialize();
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

TPZString TPZSimpleRouter :: asString() const
{
   TPZString string = getName();
   if( getRouter() )
      string += ((TPZRouter*)getRouter())->getPosition().asString();
   return string;
}


//*************************************************************************
//:
//  f: static unsigned getInputWith (TPZROUTINGTYPE type, unsigned channel);
//
//  d:
//:
//*************************************************************************

unsigned TPZSimpleRouter :: getInputWith(TPZROUTINGTYPE type, unsigned channel)
{
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));
   if( ! gData.simpleRouterInputsType() ) return 0;
   
   TPZTypeChannel elementToFind(type,channel);
   TPZTypeChannel tempType;

   for( int i=1; i<=gData.simpleRouterInputsType()->numberOfElements(); i++ )
   {
      gData.simpleRouterInputsType()->valueAt(i,tempType);
      if( elementToFind == tempType ) return i;
   }

   return 0;
}

//*************************************************************************
//:
//  f: static unsigned getHDInputWith (TPZROUTINGTYPE type, unsigned channel, Boolean reverse);
//
//  d:
//:
//*************************************************************************

unsigned TPZSimpleRouter :: getHDInputWith(TPZROUTINGTYPE type, unsigned channel, Boolean reverse)
{
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));
   if( ! gData.simpleRouterHDInputsType() ) return 0;
   
   TPZTypeChannelHalfDuplex elementToFind(type,channel,reverse);
   TPZTypeChannelHalfDuplex tempType;

   for( int i=1; i<=gData.simpleRouterHDInputsType()->numberOfElements(); i++ )
   {
      gData.simpleRouterHDInputsType()->valueAt(i,tempType);
      if( elementToFind == tempType ) return i;
   }

   return 0;
}

//*************************************************************************
//:
//  f: unsigned getOutputWith (TPZROUTINGTYPE type, unsigned channel);
//
//  d:
//:
//*************************************************************************

unsigned TPZSimpleRouter :: getOutputWith(TPZROUTINGTYPE type, unsigned channel)
{
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));

   if( ! gData.simpleRouterOutputsType() ) return 0;
   
   TPZTypeChannel elementToFind(type,channel);
   TPZTypeChannel tempType;

   for( int i=1; i<=gData.simpleRouterOutputsType()->numberOfElements(); i++ )
   {
      gData.simpleRouterOutputsType()->valueAt(i,tempType);
      if( elementToFind == tempType ) return i;
   }

   return 0;
}

//*************************************************************************
//:
//  f: unsigned getHDOutputWith (TPZROUTINGTYPE type, unsigned channel, Boolean reverse);
//
//  d:
//:
//*************************************************************************

unsigned TPZSimpleRouter :: getHDOutputWith(TPZROUTINGTYPE type, unsigned channel, Boolean reverse)
{
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));

   if( ! gData.simpleRouterHDOutputsType() ) return 0;
   
   TPZTypeChannelHalfDuplex elementToFind(type,channel,reverse);
   TPZTypeChannelHalfDuplex tempType;

   for( int i=1; i<=gData.simpleRouterHDOutputsType()->numberOfElements(); i++ )
   {
      gData.simpleRouterHDOutputsType()->valueAt(i,tempType);
      if( elementToFind == tempType ) return i;
   }

   return 0;
}


//*************************************************************************
//:
//  f: TPZROUTINGTYPE getTypeForOutput (unsigned output, unsigned channel) 
//  d:
//:
//*************************************************************************

TPZROUTINGTYPE TPZSimpleRouter :: getTypeForOutput(unsigned output)
{
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));
   if( ! gData.simpleRouterOutputsType() )
   {
      TPZString err;
      err.sprintf(ERR_TPZCROSS_001,(char*)asString());
      EXIT_PROGRAM(err);
   }
   TPZTypeChannel type;
   gData.simpleRouterOutputsType()->valueAt(output,type);
   return type.type();
}

//*************************************************************************
//:
//  f: TPZROUTINGTYPE getTypeForOutput (unsigned output, unsigned channel)
//  d:
//:
//*************************************************************************

TPZROUTINGTYPE TPZSimpleRouter :: getTypeForHDOutput(unsigned output)
{
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));
   if( ! gData.simpleRouterHDOutputsType() )
   {
      TPZString err;
      err.sprintf(ERR_TPZCROSS_001,(char*)asString());
      EXIT_PROGRAM(err);
   }
   TPZTypeChannelHalfDuplex type;
   gData.simpleRouterHDOutputsType()->valueAt(output,type);

   return type.type();
}


//*************************************************************************
//:
//  f: Boolean isLocalNodeOutput (unsigned port);
//
//  d:
//:
//*************************************************************************

Boolean TPZSimpleRouter :: isLocalNodeOutput(unsigned oPort)
{
   TPZTypeChannel tempType;
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));
   gData.simpleRouterOutputsType()->valueAt(oPort,tempType);
   return (tempType.type()==_LocalNode_) ? true : false;
}

//*************************************************************************
//:
//  f: Boolean isLocalNodeOutput (unsigned port);
//
//  d:
//:
//*************************************************************************

Boolean TPZSimpleRouter :: isLocalNodeHDOutput(unsigned oPort)
{
   TPZTypeChannelHalfDuplex tempType;
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));
   gData.simpleRouterHDOutputsType()->valueAt(oPort,tempType);
   return (tempType.type()==_LocalNode_) ? true : false;
}

//*************************************************************************
//:
//  f: Boolean isLocalNodeOutput (unsigned port);
//
//  d:
//:
//*************************************************************************

unsigned TPZSimpleRouter :: getBufferSize() const
{
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));
   return gData.routerBufferSize();
}

//*************************************************************************
//:
//  f: static TPZSimpleRouter * newFrom (TPZTag const * tag,
//                                  TPZComponent * owner,
//                                  INDEX & index);
//  d:
//:
//*************************************************************************

TPZSimpleRouter* TPZSimpleRouter :: newFrom( const TPZTag* tag, 
                                     TPZComponent* owner,
                                     INDEX& index)
{
   
   TPZComponentId idSimpleRouter(*tag);
   TPZString inputs, outputs; 
   TPZString dataDelay, headerDelay,virtualNets;
    
   if( ! (tag->getAttributeValueWithName(TPZ_TAG_INPUTS, inputs)) )
   {
      TPZString err;
      err.sprintf(ERR_TPZCROSS_002, (char*)TPZ_TAG_INPUTS );
      EXIT_PROGRAM(err);
   }
   if( ! (tag->getAttributeValueWithName(TPZ_TAG_OUTPUTS, outputs)) )
   {
      TPZString err;
      err.sprintf(ERR_TPZCROSS_003, (char*)TPZ_TAG_OUTPUTS );
      EXIT_PROGRAM(err);
   }
   if( ! (tag->getAttributeValueWithName(TPZ_TAG_HEADER_DELAY, headerDelay)) )
   {
      TPZString err;
      err.sprintf(ERR_TPZCROSS_002, (char*)TPZ_TAG_HEADER_DELAY);
      EXIT_PROGRAM(err);
   }
   if( ! (tag->getAttributeValueWithName(TPZ_TAG_DATA_DELAY, dataDelay)) )
   {
      TPZString err;
      err.sprintf(ERR_TPZCROSS_002, (char*)TPZ_TAG_DATA_DELAY);
      EXIT_PROGRAM(err);
   }
   if( ! (tag->getAttributeValueWithName(TPZ_TAG_VIRTUALNETWORKS , virtualNets )))
   {
      TPZString err;
      err.sprintf(ERR_TPZCROSS_002, (char*)TPZ_TAG_VIRTUALNETWORKS );
      EXIT_PROGRAM(err);
   }
      
   unsigned inp = inputs.asInteger();
   unsigned out = outputs.asInteger();
   unsigned vnets = virtualNets.asInteger();

   TPZString routingControlString = ((TPZRouter*)owner)->valueForRoutingControl();
   TPZRoutingControl routingControl = TPZSimpleRouter::NIL;
   if( routingControlString == TPZ_TAG_MESH_DOR )
   {
      routingControl = TPZSimpleRouter::MESH_DOR;
   }
   else if( routingControlString == TPZ_TAG_MESH_DOR_FAST )
   {
      routingControl = TPZSimpleRouter::MESH_DOR_FAST;
   }
   else if( routingControlString == TPZ_TAG_TORUS_DOR )
   {
      routingControl = TPZSimpleRouter::TORUS_DOR;
   }
   else if( routingControlString == TPZ_TAG_MESH_DOR_BINOC )
   {
      routingControl = TPZSimpleRouter::MESH_DOR_BINOC;
   }
   else if( routingControlString == TPZ_TAG_TORUS_BLESS )
   {
      routingControl = TPZSimpleRouter::TORUS_BLESS;
   }
   else
   {
      TPZString err= TPZString(routingControlString) + "is Unknown";
      EXIT_PROGRAM(err);
   }
   
   TPZString bufferControlString = ((TPZRouter*)owner)->valueForBufferControl();
   TPZBufferControl bufferControl = TPZSimpleRouter::BC_NIL;
   if( bufferControlString == TPZ_TAG_CUTHROUGH )
   {
      bufferControl = TPZSimpleRouter::BC_CT;
   }
   else if ( bufferControlString == TPZ_TAG_WORMHOLE )
   {
      bufferControl = TPZSimpleRouter::BC_WH;
   }
   else if ( bufferControlString == TPZ_TAG_DAMQ )
   {
      bufferControl = TPZSimpleRouter::BC_DAMQ;
   }
   else
   {
      if ( routingControlString != TPZ_TAG_TORUS_BLESS)
      {
      TPZString err= TPZString(bufferControlString) + "is Unknown";
      EXIT_PROGRAM(err);
      }
   }
   
   TPZSimpleRouter* simpleRouter = new TPZSimpleRouter( idSimpleRouter, inp, out, routingControl, bufferControl,vnets );
   simpleRouter->setOwner(owner);
   simpleRouter->setSimulation(owner->getSimulation());
   simpleRouter->setDataDelay(dataDelay.asInteger());
   simpleRouter->setHeaderDelay(headerDelay.asInteger());
   TPZGlobalData& gData = ((TPZSimulation*)(owner->getSimulation()))->globalData((TPZNetwork*)(owner->getOwner()));
      
   TPZString endTag = TPZString("/") + tag->tagName();
   TPZTag* nextTag = TPZComponent::routerBuilder->getTagWithIndex(++index);
   
   if( ! gData.simpleRouterIsInitializated() )
   {
      if ( routingControl == TPZSimpleRouter::MESH_DOR_BINOC)
      {
         gData.simpleRouterSetHDInputsType( new TPZRoutingHalfDuplexTypeArray(inp) );
         gData.simpleRouterSetHDOutputsType( new TPZRoutingHalfDuplexTypeArray(out) );
      }
      else
      {
         gData.simpleRouterSetInputsType( new TPZRoutingTypeArray(inp) );
         gData.simpleRouterSetOutputsType( new TPZRoutingTypeArray(out) );
      }
   }  
         
   while( nextTag && nextTag->tagName()!=endTag )
   {
      if( nextTag->tagName() == TPZ_TAG_INPUT )
      {
         // This is an entry simpleRouter
         unsigned number = nextTag->tagId().asInteger();
         TPZString type;
         TPZString channel;
         nextTag->getAttributeValueWithName(TPZ_TAG_TYPE, type);
         nextTag->getAttributeValueWithName(TPZ_TAG_CHANNEL, channel);
         if(type == TPZString(""))
         {
            TPZString err;
            err.sprintf(ERR_TPZCROSS_003, (char*)type );
            EXIT_PROGRAM(err);
         }
         if(channel == TPZString(""))
         {
            channel = "1";
         }
     
     if ( routingControl == TPZSimpleRouter::MESH_DOR_BINOC)
     {
        TPZString HDType;
        Boolean reverse;
        nextTag->getAttributeValueWithName(TPZ_TAG_HD_TYPE, HDType);
        if(HDType == TPZString(""))
            {
               TPZString err;
               err.sprintf(ERR_TPZCROSS_003, (char*)HDType );
               EXIT_PROGRAM(err);
            }
        if (HDType == TPZString("1")) reverse=true;
        else reverse=false;
        if( ! gData.simpleRouterIsInitializated() )
            {         
               gData.simpleRouterHDInputsType()->setValueAt(number,TPZTypeChannelHalfDuplex(_string2routingType(type),channel.asInteger(), reverse));
            } 
     
     }
     
     else
         {
            if( ! gData.simpleRouterIsInitializated() )
            {         
               gData.simpleRouterInputsType()->setValueAt(number,TPZTypeChannel(_string2routingType(type),channel.asInteger()));

            }
     }
      }
      else if( nextTag->tagName() == TPZ_TAG_OUTPUT )
      {
         // This is a departure simpleRouter
         unsigned number = nextTag->tagId().asInteger();
         TPZString type;
         TPZString channel;
         nextTag->getAttributeValueWithName(TPZ_TAG_TYPE, type);
         nextTag->getAttributeValueWithName(TPZ_TAG_CHANNEL, channel);
         if(type == TPZString(""))
         {
            TPZString err;
            err.sprintf(ERR_TPZCROSS_003, (char*)type );
            EXIT_PROGRAM(err);
         }
         if(channel == TPZString(""))
         {
            channel = "1";
         }
     
     if ( routingControl == TPZSimpleRouter::MESH_DOR_BINOC)
     {
        TPZString HDType;
        Boolean reverse;
        nextTag->getAttributeValueWithName(TPZ_TAG_HD_TYPE, HDType);
        if(HDType == TPZString(""))
            {
               TPZString err;
               err.sprintf(ERR_TPZCROSS_003, (char*)HDType );
               EXIT_PROGRAM(err);
            }
        if (HDType == TPZString("1")) reverse=true;
        else reverse=false;
        if( ! gData.simpleRouterIsInitializated() )
            {         
               gData.simpleRouterHDOutputsType()->setValueAt(number,TPZTypeChannelHalfDuplex(_string2routingType(type),channel.asInteger(), reverse));
            } 
     
     }
     
     else
     {
            if( ! gData.simpleRouterIsInitializated() )
            {         
               gData.simpleRouterOutputsType()->setValueAt(number,TPZTypeChannel(_string2routingType(type),channel.asInteger()));

            }
     }
      }
      else
      {
         TPZString err;
         err.sprintf(ERR_TPZCROSS_004, (char*)(nextTag->tagName()) );
         EXIT_PROGRAM(err);
      }
      
      nextTag = TPZComponent::routerBuilder->getTagWithIndex(++index);
   
   } // while



   if( ! gData.simpleRouterIsInitializated() )
   {
      for( int i=1; i<=simpleRouter->numberOfOutputs(); i++ )
      {
         if ( routingControl == TPZSimpleRouter::MESH_DOR_BINOC)
     {
     if( simpleRouter->isLocalNodeHDOutput(i) )
            {
               simpleRouter->setNumberOfLocalPorts( simpleRouter->numberOfLocalPorts() + 1 );
           
            }
     }
     else
     {
        if( simpleRouter->isLocalNodeOutput(i) )
            {
               simpleRouter->setNumberOfLocalPorts( simpleRouter->numberOfLocalPorts() + 1 );
            }
     }
      }
   }
   
   gData.simpleRouterSetInitializated();
   owner->addPostRunComponent(simpleRouter);
   owner->addPreRunComponent(simpleRouter);

   // 5 channels with a buffer and a routing
   ((TPZRouter*)owner)->addToPoolSize(simpleRouter->getVnets()*(inp*(gData.routerBufferSize()+1)));
    
   return simpleRouter;
}
//*************************************************************************
//:
//  f: virtual void buildOutputInterfaz (unsigned number = 1);
//
//  d:
//:
//*************************************************************************

void TPZSimpleRouter  :: buildOutputInterfaz(unsigned number)
{
   if( m_vnets  == 1 ) 
   {
      Inhereited::buildOutputInterfaz(number);
      return;
   }

   // In this case, the outputs are multiplexed by a crossbar
   // factor set  by"getOutputMux ()", 
   // The outputs that go to LocalNode are not multiplexed
   
   TPZInterfaz* interfaz;
   for( int i=1; i<=number-1; i++ )   
   {
      interfaz =TPZInterfaz::createOutputInterfaz(i,m_vnets);
      interfaz->setOwner(this);
      addComponent(interfaz);
   }
   //Consumption has no virtual channels
   interfaz =TPZInterfaz::createOutputInterfaz(number,1);
   interfaz->setOwner(this);
   addComponent(interfaz);
}

//*************************************************************************
//:
//  f: virtual void buildInputInterfaz (unsigned number = 1);
//
//  d:
//:
//*************************************************************************

void TPZSimpleRouter :: buildInputInterfaz(unsigned number)
{
   if( m_vnets  == 1 ) 
   {
      Inhereited::buildInputInterfaz(number);
      return;
   }
   TPZInterfaz* interfaz;
   for( int i=1; i<=number-1; i++ )
   {
      interfaz =TPZInterfaz::createInputInterfaz(i,m_vnets);
      interfaz->setOwner(this);
      addComponent(interfaz);
   }
   //the injection has no virtual channels
   interfaz = TPZInterfaz::createInputInterfaz(number,1);
   interfaz->setOwner(this);
   addComponent(interfaz);
}

//*************************************************************************


// end of file

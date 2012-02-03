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
//    File: TPZCrossbar.cpp
//
//    Class:  TPZCrossbar
//
//    Inherited from:  TPZRunnableComponent
// :
//*************************************************************************
//end of header




#include <TPZCrossbar.hpp>

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZCrossbarFlow_HPP__
#include <TPZCrossbarFlow.hpp>
#endif

#ifndef __TPZCrossbarFlowCT_HPP__
#include <TPZCrossbarFlowCT.hpp>
#endif

#ifndef __TPZCrossbarFlowCTMux_HPP__
#include <TPZCrossbarFlowCTMux.hpp>
#endif

#ifndef __TPZCrossbarFlowMux_HPP__
#include <TPZCrossbarFlowMux.hpp>
#endif

#ifndef __TPZCrossbarFlowWH_HPP__
#include <TPZCrossbarFlowWH.hpp>
#endif

#ifndef __TPZCrossbarFlowVC_HPP__
#include <TPZCrossbarFlowVC.hpp>
#endif

#ifndef __TPZCrossbarFlowVCOpt_HPP__
#include <TPZCrossbarFlowVCOpt.hpp>
#endif

#ifndef __TPZCrossbarFlowVCMux_HPP__
#include <TPZCrossbarFlowVCMux.hpp>
#endif

#ifndef __TPZCrossbarFlowVCMuxOpt_HPP__
#include <TPZCrossbarFlowVCMuxOpt.hpp>
#endif

#ifndef __TPZCrossbarFlowVCTorus_HPP__
#include <TPZCrossbarFlowVCTorus.hpp>
#endif

#ifndef __TPZCrossbarFlowVCTorusMux_HPP__
#include <TPZCrossbarFlowVCTorusMux.hpp>
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

IMPLEMENT_RTTI_DERIVED(TPZCrossbar,TPZRunnableComponent);

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
//  f: TPZCrossbar (const TPZComponentId & id,
//                  unsigned inputs,
//                  unsigned output);
//
//  d:
//:
//*************************************************************************

TPZCrossbar :: TPZCrossbar( const TPZComponentId& id,
                            unsigned inputs,
                            unsigned outputs )
             : TPZRunnableComponent(id)
{
   setNumberOfInputs(inputs);
   setNumberOfOutputs(outputs);   
}


//*************************************************************************
//:
//  f: virtual ~ TPZCrossbar ();
//
//  d:
//:
//*************************************************************************

TPZCrossbar :: ~TPZCrossbar()
{
 
}

//*************************************************************************
//:
//  f: getOutputMux unsigned () const;
//
//  d:
//:
//*************************************************************************

unsigned TPZCrossbar :: getOutputMux() const
{
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));
   return gData.crossbarOutputMux();
}


//*************************************************************************
//:
//  f: void setOutputMux (unsigned number);
//
//  d:
//:
//*************************************************************************

void TPZCrossbar :: setOutputMux(unsigned number)
{
      
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));
   gData.crossbarSetOutputMux(number);
}

//*************************************************************************
//:
//  f: getNumberVC unsigned () const;
//
//  d:
//:
//*************************************************************************

unsigned TPZCrossbar :: getNumberVC() const
{
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));
   return gData.crossbarNumberVC();
}


//*************************************************************************
//:
//  f: void setNumberVC (unsigned number);
//
//  d:
//:
//*************************************************************************

void TPZCrossbar :: setNumberVC(unsigned number)
{
      
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));
   gData.crossbarSetNumberVC(number);
}

//*************************************************************************
//:
//  f: getNumberMT unsigned () const;
//
//  d:
//:
//*************************************************************************

unsigned TPZCrossbar :: getNumberMT() const
{
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));
   return gData.crossbarNumberMT();
}


//*************************************************************************
//:
//  f: void setNumberMT (unsigned number);
//
//  d:
//:
//*************************************************************************

void TPZCrossbar :: setNumberMT(unsigned number)
{
      
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));
   gData.crossbarSetNumberMT(number);
}

//*************************************************************************
//:
//  f: numberOfLocalPorts unsigned () const;
//
//  d:
//:
//*************************************************************************

unsigned TPZCrossbar :: numberOfLocalPorts() const
{
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));
   return gData.crossbarNumberOfLocalPorts();
}


//*************************************************************************
//:
//  f: void setNumberOfLocalPorts (unsigned number);
//
//  d:
//:
//*************************************************************************

void TPZCrossbar :: setNumberOfLocalPorts(unsigned number)
{
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));
   gData.crossbarSetNumberOfLocalPorts(number);
}


//*************************************************************************
//:
//  f: virtual void buildFlowControl ();
//
//  d:
//:
//*************************************************************************

void TPZCrossbar :: buildFlowControl()
{
   TPZString err;
   err.sprintf("TPZCrossbar :: %d component has been deprecated\n", getCrossbarType() );

   if( getCrossbarType() == TPZCrossbar::WH )
      setFlowControl( new TPZCrossbarFlow(*this) );
        
   else if( getCrossbarType() == TPZCrossbar::CT )
      setFlowControl( new TPZCrossbarFlowCT(*this) );
   
   else if( getCrossbarType() == TPZCrossbar::VC )
      setFlowControl( new TPZCrossbarFlowVC(*this) );
   
   else if( getCrossbarType() == TPZCrossbar::VC_OPT )
      setFlowControl( new TPZCrossbarFlowVCOpt(*this) );
   
   else if( getCrossbarType() == TPZCrossbar::VC_MUX )
      setFlowControl( new TPZCrossbarFlowVCMux(*this) );
   
   else if( getCrossbarType() == TPZCrossbar::VC_MUX_OPT )
      setFlowControl( new TPZCrossbarFlowVCMuxOpt(*this) );
   
   else if( getCrossbarType() == TPZCrossbar::VC_TORUS )
      setFlowControl( new TPZCrossbarFlowVCTorus(*this) );
   
   else if( getCrossbarType() == TPZCrossbar::VC_TORUS_MUX )
      setFlowControl( new TPZCrossbarFlowVCTorusMux(*this) );
   
   else if( getCrossbarType() == TPZCrossbar::CT_MUX )
      setFlowControl( new TPZCrossbarFlowCTMux(*this) );
   
   else if( getCrossbarType() == TPZCrossbar::MUX_ADAPT )
      setFlowControl( new TPZCrossbarFlowMux(*this) );
   
   else if( getCrossbarType() == TPZCrossbar::WH_RTG )
      setFlowControl( new TPZCrossbarFlowWH(*this) );    
   
   else
      EXIT_PROGRAM(err)
   
   getFlowControl() -> initialize();
}


//*************************************************************************
//:
//  f: virtual void buildOutputInterfaz (unsigned number = 1);
//
//  d:
//:
//*************************************************************************

void TPZCrossbar :: buildOutputInterfaz(unsigned number)
{
   if( getOutputMux() == 0 ) 
   {
      Inhereited::buildOutputInterfaz(number);
      return;
   }

   // In this case, crossbar outputs are multiplexed by a 
   // factor set by "getOutputMux()". The outputs advancing
   // to LocalNode are not multiplexed.

   for( int i=1; i<=number; i++ )   
   {
      TPZInterfaz* interfaz = ( TPZCrossbar::isLocalNodeOutput(i) ) ?
                   TPZInterfaz::createOutputInterfaz(i,1) :
                   TPZInterfaz::createOutputInterfaz(i,getOutputMux());
      interfaz->setOwner(this);
      addComponent(interfaz);
   }

}


//*************************************************************************
//:
//  f: virtual TPZString AsString () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZCrossbar :: asString() const
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

unsigned TPZCrossbar :: getInputWith(TPZROUTINGTYPE type, unsigned channel)
{
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));
   if( ! gData.crossbarInputsType() ) return 0;
   
   TPZTypeChannel elementToFind(type,channel);
   TPZTypeChannel tempType;

   for( int i=1; i<=gData.crossbarInputsType()->numberOfElements(); i++ )
   {
      gData.crossbarInputsType()->valueAt(i,tempType);
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

unsigned TPZCrossbar :: getOutputWith(TPZROUTINGTYPE type, unsigned channel)
{
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));

   if( ! gData.crossbarOutputsType() ) return 0;
   
   TPZTypeChannel elementToFind(type,channel);
   TPZTypeChannel tempType;

   for( int i=1; i<=gData.crossbarOutputsType()->numberOfElements(); i++ )
   {
      gData.crossbarOutputsType()->valueAt(i,tempType);
      if( elementToFind == tempType ) return i;
   }

   return 0;
}


//*************************************************************************
//:
//  f: TPZROUTINGTYPE getTypeForOutput (unsigned output, unsigned channel) / /
//  d:
//:
//*************************************************************************

TPZROUTINGTYPE TPZCrossbar :: getTypeForOutput(unsigned output, unsigned channel)
{
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));
   if( ! gData.crossbarOutputsType() )
   {
      TPZString err;
      err.sprintf(ERR_TPZCROSS_001,(char*)asString());
      EXIT_PROGRAM(err);
   }
   TPZTypeChannel tempType;
   gData.crossbarOutputsType()->valueAt(output,tempType);

   return tempType.type();
}

//*************************************************************************
//:
//  f: TPZROUTINGTYPE getTypeForInput (unsigned output, unsigned channel) / /
//  d:
//:
//*************************************************************************
TPZROUTINGTYPE TPZCrossbar :: getTypeForInput(unsigned input, unsigned channel)
{
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));
   if( ! gData.crossbarOutputsType() )
   {
      TPZString err;
      err.sprintf(ERR_TPZCROSS_001,(char*)asString());
      EXIT_PROGRAM(err);
   }
   TPZTypeChannel tempType;
   gData.crossbarInputsType()->valueAt(input,tempType);

   return tempType.type();
}

//*************************************************************************
//:
//  f: Boolean isLocalNodeOutput (unsigned port);
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbar :: isLocalNodeOutput(unsigned oPort)
{
   TPZTypeChannel tempType;
   TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)(TPZROUTER->getOwner()));
   gData.crossbarOutputsType()->valueAt(oPort,tempType);
   return (tempType.type()==_LocalNode_) ? true : false;
}


//*************************************************************************
//:
//  f: static TPZCrossbar * newFrom (TPZTag const * tag,
//                                  TPZComponent * owner,
//                                  INDEX & index);
//  d:
//:
//*************************************************************************

TPZCrossbar* TPZCrossbar :: newFrom( const TPZTag* tag, 
                                     TPZComponent* owner,
                                     INDEX& index)
{
   TPZComponentId idCrossbar(*tag);
   TPZString inputs, outputs, type, outputMux, threshold; 
   TPZString numOfVC, numOfMT; 
   TPZString dataDelay, headerDelay;
   TPZString routingDelay, vcarbDelay, swarbDelay, swtravDelay;
    
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
   if( ! (tag->getAttributeValueWithName(TPZ_TAG_TYPE, type)) )
   {
      TPZString err;
      err.sprintf(ERR_TPZCROSS_002, (char*)TPZ_TAG_TYPE );
      EXIT_PROGRAM(err);
   }
   
   unsigned hDelay = 1;
   if( tag->getAttributeValueWithName(TPZ_TAG_HEADER_DELAY, headerDelay) )
   {
      hDelay = headerDelay.asInteger();
   }
   
   unsigned dDelay = 0;
   if(tag->getAttributeValueWithName(TPZ_TAG_DATA_DELAY, dataDelay) )
   {
      dDelay = dataDelay.asInteger();
   }
   
   unsigned outMux = 1;
   if( tag->getAttributeValueWithName(TPZ_TAG_OUTPUTMUX, outputMux) )
   {
      outMux = outputMux.asInteger();
   }   
   
   unsigned NumVirtualChannels = 1;
   if( tag->getAttributeValueWithName(TPZ_TAG_NUM_VC, numOfVC) )
   {
      NumVirtualChannels = numOfVC.asInteger();
   }
   
   unsigned NumMessageTypes = 1;
   if( tag->getAttributeValueWithName(TPZ_TAG_NUM_MT, numOfMT) )
   {
      NumMessageTypes = numOfMT.asInteger();
   }   
   
   unsigned inp = inputs.asInteger();
   unsigned out = outputs.asInteger();
   
   unsigned RTGDelay = 1;
   if( tag->getAttributeValueWithName(TPZ_TAG_RTG_DELAY, routingDelay) )
   {
      RTGDelay = routingDelay.asInteger();
   }
   
   unsigned VADelay = 1;
   if( tag->getAttributeValueWithName(TPZ_TAG_VA_DELAY, vcarbDelay) )
   {
      VADelay = vcarbDelay.asInteger();
   }
   
   unsigned SADelay = 1;
   if( tag->getAttributeValueWithName(TPZ_TAG_SA_DELAY, swarbDelay) )
   {
      SADelay = swarbDelay.asInteger();
   }
   
   unsigned STDelay = 1;
   if( tag->getAttributeValueWithName(TPZ_TAG_ST_DELAY, swtravDelay) )
   {
      STDelay = swtravDelay.asInteger();
   }
   
   
   //********************************************************************
   
   TPZCrossbar* crossbar = new TPZCrossbar( idCrossbar, inp, out );
   crossbar->setOwner(owner);
   crossbar->setSimulation(owner->getSimulation());
   crossbar->setDataDelay(dDelay);
   crossbar->setHeaderDelay(hDelay);
   crossbar->setOutputMux(outMux);
   crossbar->setNumberVC(NumVirtualChannels);
   crossbar->setNumberMT(NumMessageTypes);
   
   crossbar->setRoutingDelay(RTGDelay);
   crossbar->setVCArbitrationDelay(VADelay);
   crossbar->setSWArbitrationDelay(SADelay);
   crossbar->setSWTraversalDelay(STDelay);
   
   TPZGlobalData& gData = ((TPZSimulation*)(owner->getSimulation()))->globalData((TPZNetwork*)(owner->getOwner()));
   
   TPZString endTag = TPZString("/") + tag->tagName();
   TPZTag* nextTag = TPZComponent::routerBuilder->getTagWithIndex(++index);
   
   if( ! gData.crossbarIsInitializated() )
   {
      gData.crossbarSetInputsType( new TPZRoutingTypeArray(inp) );
      gData.crossbarSetOutputsType( new TPZRoutingTypeArray(out) );
   }  
         
   while( nextTag && nextTag->tagName()!=endTag )
   {
      if( nextTag->tagName() == TPZ_TAG_INPUT )
      {
         // This is a crossbar input
         unsigned number = nextTag->tagId().asInteger();
         TPZString type, channel;
         nextTag->getAttributeValueWithName(TPZ_TAG_TYPE, type);
         nextTag->getAttributeValueWithName(TPZ_TAG_CHANNEL, type);
         if(type == TPZString(""))
         {
            TPZString err;
            err.sprintf(ERR_TPZCROSS_003, (char*)type );
            EXIT_PROGRAM(err);
         }
         if(channel ==TPZString("")) channel = "1";
         
         TPZTypeChannel typeChannel( _string2routingType(type),
                                     channel.asInteger() );
         
         if( ! gData.crossbarIsInitializated() )
         {         
            gData.crossbarInputsType()->setValueAt(number,typeChannel);
         }
      }
      else if( nextTag->tagName() == TPZ_TAG_OUTPUT )
      {
         // This is a crossbar output
         unsigned number = nextTag->tagId().asInteger();
         TPZString type, channel;
         nextTag->getAttributeValueWithName(TPZ_TAG_TYPE, type);
         nextTag->getAttributeValueWithName(TPZ_TAG_CHANNEL, channel);
         if(type == TPZString(""))
         {
            TPZString err;
            err.sprintf(ERR_TPZCROSS_003, (char*)type );
            EXIT_PROGRAM(err);
         }
         if(channel ==TPZString("")) 
         {
            channel = "1";
         } 
         TPZTypeChannel typeChannel( _string2routingType(type),
                                     channel.asInteger() );
         if( ! gData.crossbarIsInitializated() )
         {         
            gData.crossbarOutputsType()->setValueAt(number,typeChannel);      
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


   // Flow rates

   if( type == TPZ_TAG_WORMHOLE )
   {
      crossbar->setCrossbarType(TPZCrossbar::WH);
   }
   else if( type == TPZ_TAG_CUTHROUGH )
   {
      crossbar->setCrossbarType(TPZCrossbar::CT);
   }
   else if( type == TPZ_TAG_VIRTUAL_CHANNEL )
   {
      crossbar->setCrossbarType(TPZCrossbar::VC);
   }
   else if( type == TPZ_TAG_VIRTUAL_CHANNEL_OPT )
   {
      crossbar->setCrossbarType(TPZCrossbar::VC_OPT);
   }
   else if( type == TPZ_TAG_VIRTUAL_CHANNEL_MUXED )
   {
      crossbar->setCrossbarType(TPZCrossbar::VC_MUX);
   }
   else if( type == TPZ_TAG_VIRTUAL_CHANNEL_MUXED_OPT )
   {
      crossbar->setCrossbarType(TPZCrossbar::VC_MUX_OPT);
   }
   else if( type == TPZ_TAG_VIRTUAL_CHANNEL_TORUS )
   {
      crossbar->setCrossbarType(TPZCrossbar::VC_TORUS);
   }
   else if( type == TPZ_TAG_VIRTUAL_CHANNEL_TORUS_MUX )
   {
      crossbar->setCrossbarType(TPZCrossbar::VC_TORUS_MUX);
   }
   else if( type == TPZ_TAG_CUTHROUGH_MUX )
   {
      crossbar->setCrossbarType(TPZCrossbar::CT_MUX);
   }
   else if( type == TPZ_TAG_MUX_ADAPT )
   {
      crossbar->setCrossbarType(TPZCrossbar::MUX_ADAPT);
   }
   else if( type == TPZ_TAG_WH_RTG )
   {
      crossbar->setCrossbarType(TPZCrossbar::WH_RTG);
   }
   else
   {
      TPZString err;
      err.sprintf(ERR_TPZCROSS_003, (char*)type );
      EXIT_PROGRAM(err);
   }

   if( ! gData.crossbarIsInitializated() )
   {
      for( int i=1; i<=crossbar->numberOfOutputs(); i++ )
      {
         if( crossbar->isLocalNodeOutput(i) )
         {
            crossbar->setNumberOfLocalPorts( crossbar->numberOfLocalPorts() + 1 );
         }
      }
   }
   
   gData.crossbarSetInitializated();
   owner->addPostRunComponent(crossbar);
   owner->addPreRunComponent(crossbar);
      
   return crossbar;
}

//*************************************************************************


// end of file

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
//    File: TPZError.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZError_HPP__
#define __TPZError_HPP__


   #ifdef __TPZERROR_IMPLEMENT__
      #define _EXTERN
      #define _CONST
      #define _IS(value)  =value
   #else
      #define _EXTERN     extern
      #define _CONST      const
      #define _IS(value) 
   #endif
   

//*************************************************************************

   _EXTERN _CONST char ERR_TPZSIMUL_001[]
                  _IS("TPZSimulator: Can not run more than% d simulations while");


   _EXTERN _CONST char ERR_TPZBLDR_001[]
                  _IS("TPZBuilder: Can not open file '% s'");
   _EXTERN _CONST char ERR_TPZBLDR_002[]
                  _IS("TPZBuilder: Does not exist any SGML component with id=\"% S \" ");

   _EXTERN _CONST char ERR_TPZCFLOC_001[]
                  _IS("TPZCrossbarFlowCT:% s input port = 0, msg =% s");
   _EXTERN _CONST char ERR_TPZCFLOC_002[]
                  _IS("TPZCrossbarFlowCT:% s port output = 0, msg =% s");

   _EXTERN _CONST char ERR_TPZCFLOW_001[]
                  _IS("TPZCrossbarFlow:% s Invalid output port: (% s,% d)");
   _EXTERN _CONST char ERR_TPZCFLOW_002[]
                  _IS("TPZCrossbarFlow:% s input port = 0");
   _EXTERN _CONST char ERR_TPZCFLOW_003[]
                  _IS("TPZCrossbarFlow:% s port output = 0");
   _EXTERN _CONST char ERR_TPZCFLOW_004[]
                  _IS("TPZCrossbarFlow:% s Request No event");
   _EXTERN _CONST char ERR_TPZCFLOW_005[]
                  _IS("TPZCrossbarFlow:% s unknown event:% d");

   _EXTERN _CONST char ERR_TPZCMFLO_001[]
                  _IS("TPZCrossbarFlowMux:% s input port = 0");
   _EXTERN _CONST char ERR_TPZCMFLO_002[]
                  _IS("TPZCrossbarFlowMux:% s port output = 0");
   _EXTERN _CONST char ERR_TPZCMFLO_003[]
                  _IS("TPZCrossbarFlowMux:% s Invalid output port: (% s)");

   _EXTERN _CONST char ERR_TPZCMRFL_001[]
                  _IS("TPZCrossbarMRequestFlow:% s input port = 0");
   _EXTERN _CONST char ERR_TPZCMRFL_002[]
                  _IS("TPZCrossbarMRequestFlow:% s port output = 0");
   _EXTERN _CONST char ERR_TPZCMRFL_003[]
                  _IS("TPZCrossbarMRequestFlow:% s There is a list of requests");

   _EXTERN _CONST char ERR_TPZCOFLO_001[]
                  _IS("TPZConsumerFlow:% s error routing; msg =% s");

   _EXTERN _CONST char ERR_TPZCONEC_001[]
                  _IS("TPZConnection: invalid parameter; own src =% p =% p dst =% p");
   _EXTERN _CONST char ERR_TPZCONEC_002[]
                  _IS("TPZConnection: Interfaces with different num. CV:% d,% d");
   _EXTERN _CONST char ERR_TPZCONEC_003[]
                  _IS("TPZConnection: Destination '% s' is not a port of entry");
   _EXTERN _CONST char ERR_TPZCONEC_004[]
                  _IS("TPZConnection: Source '% s' is not an output port");
   _EXTERN _CONST char ERR_TPZCONEC_005[]
                  _IS("TPZConnection: Missing 'source' in connection '% s'");
   _EXTERN _CONST char ERR_TPZCONEC_006[]
                  _IS("TPZConnection: Missing 'destiny' in connection '% s'");
   _EXTERN _CONST char ERR_TPZCONEC_007[]
                  _IS("TPZConnection: The 'source =% s' does not exist for connection '% s'");
   _EXTERN _CONST char ERR_TPZCONEC_008[]
                  _IS("TPZConnection: The 'destiny =% s' does not exist for connection '% s'");
                  
   _EXTERN _CONST char ERR_TPZCROSS_001[]
                  _IS("TPZCrossbar:% s has not been initialized");
   _EXTERN _CONST char ERR_TPZCROSS_002[]
                  _IS("TPZCrossbar: Missing tag '% s'");
   _EXTERN _CONST char ERR_TPZCROSS_003[]
                  _IS("TPZCrossbar: 'type =% s' not valid");
   _EXTERN _CONST char ERR_TPZCROSS_004[]
                  _IS("TPZCrossbar: Tag not valid:% s");

   _EXTERN _CONST char ERR_TPZDECON_001[]
                  _IS("TPZConnectionDelay: invalid parameter; own src =% p =% p dst =% p");
   _EXTERN _CONST char ERR_TPZDECON_002[]
                  _IS("TPZConnectionDelay: Interfaces with different num. CV:% d,% d");
   _EXTERN _CONST char ERR_TPZDECON_003[]
                  _IS("TPZConnectionDelay: Destination '% s' is not a port of entry");
   _EXTERN _CONST char ERR_TPZDECON_004[]
                  _IS("TPZConnectionDelay: Source '% s' is not an output port");

   _EXTERN _CONST char ERR_TPZFFLOW_001[]
                  _IS("TPZFifoMemoryFlow:% s No room at the input Buffer");

   _EXTERN _CONST char ERR_TPZFIFO_001[]
                  _IS("TPZFifoMemory: Can not create the flow '% d'");
   _EXTERN _CONST char ERR_TPZFIFO_002[]
                  _IS("TPZFifoMemory 'bufferControl =% s' not valid");

   _EXTERN _CONST char ERR_TPZFILOG_001[]
                  _IS("TPZFicheroLog: Can not open file '% s'");

   _EXTERN _CONST char ERR_TPZFLOW_001[]
                  _IS("TPZFlow:% s not initialized input interface");
   _EXTERN _CONST char ERR_TPZFLOW_002[]
                  _IS("TPZFlow:% s not initialized the output interface");
   _EXTERN _CONST char ERR_TPZFLOW_003[]
                  _IS("TPZFlow:% s unknown event:% s");

   _EXTERN _CONST char ERR_TPZINJEC_001[]
                  _IS("TPZInjector: 'type =% s' not valid");

   _EXTERN _CONST char ERR_TPZINTER_001[]
                  _IS("TPZInterfaz: Channel '% d' not valid");

   _EXTERN _CONST char ERR_TPZNBLDR_001[]
                  _IS("TPZNetworkBuilder: Tag '% s' not valid");

   _EXTERN _CONST char ERR_TPZNET_001[]
                  _IS("TPZNetwork There is already a router at% s");
   _EXTERN _CONST char ERR_TPZNET_002[]
                  _IS("TPZNetwork: Unable to inject the message in% s");

   _EXTERN _CONST char ERR_TPZNET_003[]
                  _IS("TPZNetwork: There is no router in (% d,% d)");

   _EXTERN _CONST char ERR_TPZRAFLO_001[]
                  _IS("TPZRoutingAdaptativeBurbleFlow:% s Buffer not found");
    _EXTERN _CONST char ERR_TPZRAFLO_002[]
                  _IS("TPZRoutingFlowHamilton:% s Buffer not found (id =% s)");
    
    
   _EXTERN _CONST char ERR_TPZRBAF3_001[]
                  _IS("TPZRoutingAdaptativeBurble3CvFlow:% s Buffer not found");

   _EXTERN _CONST char ERR_TPZRBFLO_001[]
                  _IS("TPZRoutingFlowBubble:% s Delta = 0");
   _EXTERN _CONST char ERR_TPZRBFLO_002[]
                  _IS("TPZRoutingFlowBubble:% s Buffer not found:% s");

   _EXTERN _CONST char ERR_TPZRBLDR_001[]
                  _IS("TPZRouterBuilder: Tag '% s' not valid");

   _EXTERN _CONST char ERR_TPZRC2FL_001[]
                  _IS("TPZRoutingCV2Flow:% s Delta = 0");

   _EXTERN _CONST char ERR_TPZRCAFL_001[]
                  _IS("TPZRoutingAdaptativeCVFlow:% s can not store more flits");

   _EXTERN _CONST char ERR_TPZRCFLO_001[]
                  _IS("TPZRoutingCVFlow:% s Delta = 0");

   _EXTERN _CONST char ERR_TPZRFLOW_001[]
                  _IS("TPZRoutingFlow:% s Delta = 0");

   _EXTERN _CONST char ERR_TPZROUTE_001[]
                  _IS("TPZRouter: Sgml descriptors missing on Router '% s'");
   _EXTERN _CONST char ERR_TPZROUTE_002[]
                  _IS("TPZRouter: Component '% s' unknown");
   _EXTERN _CONST char ERR_TPZROUTE_003[]
                  _IS("TPZRouter: Excessive Number of CV's (% d)");

   _EXTERN _CONST char ERR_TPZRTING_001[]
                  _IS("TPZRouting: Can not create the flow '% d'");
   _EXTERN _CONST char ERR_TPZRTING_002[]
                  _IS("TPZRouting: Missing tag '% s'");
   _EXTERN _CONST char ERR_TPZRTING_003[]
                  _IS("TPZRouting 'routingControl =% s' not valid");
   _EXTERN _CONST char ERR_TPZRTING_004[]
                  _IS("TPZRouting: 'routingControl=%s' require una tabla de rutas (tableUpdow=\"file \")");
   _EXTERN _CONST char ERR_TPZRTING_005[]
                  _IS("TPZRouting: 'routingControl=%s' require dos tablas de rutas (tableUpdow=\"file\" tableUpdowAdap=\"file \"");
   _EXTERN _CONST char ERR_TPZRTING_006[]
                  _IS("TPZRouting: 'routingControl=%s' require una tablas de rutas (tableHamilton=\"file\") y el ciclo PH(hamiltonCycle=\"file \"");
      
   _EXTERN _CONST char ERR_TPZRUNCO_001[]
                  _IS("TPZRunnableComponent:% s has no associated flow");

   _EXTERN _CONST char ERR_TPZSIMUL_0012[]
                  _IS("TPZSimulation: The bimodal traffic needs more parameters");
                  
   _EXTERN _CONST char ERR_TPZSIMUL_0022[]
                  _IS("TPZSimulation: The broadcast traffic generation needs the probability");
                  
   _EXTERN _CONST char ERR_TPZTONET_001[]
                  _IS("TPZNetworkTorus: The network is not valid dimensions: (% d,% d,% d)");
   _EXTERN _CONST char ERR_TPZTONET_002[]
                  _IS("TPZNetworkTorus: Can not create the router with id = '% s'");
   _EXTERN _CONST char ERR_TPZTONET_003[]
                  _IS("TPZNetworkTorus: Missing tag '% s'");
   _EXTERN _CONST char ERR_TPZTONET_004[]
                  _IS("TPZNetworkTorus: No dimension may be 0: (x =% d, y =% d, z =% d)");
   _EXTERN _CONST char ERR_TPZTONET_005[]
                  _IS("TPZIrregular: Can not open file% s");
   _EXTERN _CONST char ERR_TPZTONET_006[]
                  _IS("TPZIrregular: Format error in file% s route table");
   _EXTERN _CONST char ERR_TPZTONET_007[]
                  _IS("TPZIrregular: Format error in file% s Hamiltonian cycle");

   _EXTERN _CONST char ERR_TPZTRAFI_001[]
                  _IS("TPZTrafficPattern: Missing tag '% s'");
   _EXTERN _CONST char ERR_TPZTRAFI_002[]
                  _IS("TPZTrafficPattern: 'type =% s' not valid");
   _EXTERN _CONST char ERR_TPZTRAFI_003[]
                  _IS("TPZTrafficPattern traffic patterns '% s' not valid");
   _EXTERN _CONST char ERR_TPZTRAFI_004[]
                  _IS("TPZTrafficPattern: We have hot-spots demasido (% d)");
   _EXTERN _CONST char ERR_TPZTRAFI_005[]
                  _IS("TPZTrafficPattern: The file '% s' can not be opened");
   _EXTERN _CONST char ERR_TPZTRAFI_006[]
                  _IS("TPZTrafficPattern dist =% d 'is wrong. Must be <= the radio network controller (% d)");
   _EXTERN _CONST char ERR_TPZTRAFI_007[]
                  _IS("TPZTrafficPattern: The 'dist' can only be specified for 2 hot-spots");
   _EXTERN _CONST char ERR_TPZPOOL_001[]
                  _IS("TPZPool: Not enough messages in pool");                  
//*************************************************************************


#endif


// end of file

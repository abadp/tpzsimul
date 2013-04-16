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
//    File: TPZConst.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


 #ifndef __TPZConst_HPP__
 #define __TPZConst_HPP__


    #ifdef __TPZCONST_IMPLEMENT__
       #define _EXTERN
       #define _CONST
       #define _IS(value)  =value
    #else
       #define _EXTERN     extern
       #define _CONST      const
       #define _IS(value)
    #endif

    #ifndef __TPZString_HPP__
    #include "TPZString.hpp"
    #endif

//*************************************************************************
// Log filename
//*************************************************************************
_EXTERN _CONST TPZString TPZ_LOG_FILENAME          _IS("TPZSimul.log");


//*************************************************************************
// Maximum number of Virtual channels supported
// Maximum number of Phisical networks supported
//*************************************************************************
_EXTERN _CONST unsigned TPZ_MAX_NUMBER_OF_CV       _IS(25);
_EXTERN _CONST unsigned TPZ_MAX_NUMBER_SIMULATIONS _IS(10);

//*************************************************************************
// Tags for the .ini file
//*************************************************************************
_EXTERN _CONST TPZString TPZ_TAG_INI_ROUTER      _IS("RouterFile");
_EXTERN _CONST TPZString TPZ_TAG_INI_NETWORK     _IS("NetworkFile");
_EXTERN _CONST TPZString TPZ_TAG_INI_SIMULATION  _IS("SimulationFile");

//*************************************************************************
//SGML Tags for network description 
//*************************************************************************

   //**********************************************************************
   // Router Description
   //**********************************************************************
   _EXTERN _CONST TPZString TPZ_TAG_INPUT            _IS("Input");         //Router ports, connect each router with
   _EXTERN _CONST TPZString TPZ_TAG_OUTPUT           _IS("Output");        //its neighbours
   
   _EXTERN _CONST TPZString TPZ_TAG_CONNECTION       _IS("Connection");    //Link between routers, but also connections
                                                                           //between router internal components
									   
   _EXTERN _CONST TPZString TPZ_TAG_INJECTOR         _IS("Injector");      //Component generating messages
         _EXTERN _CONST TPZString TPZ_TAG_TYPE_INJECTOR   _IS("typeInj");
         _EXTERN _CONST TPZString TPZ_TAG_INJECT_CONTROL  _IS("injControl");
	 
   _EXTERN _CONST TPZString TPZ_TAG_CONSUMER         _IS("Consumer");      //Component consuming messages
         _EXTERN _CONST TPZString TPZ_TAG_TYPE_CONSUMER     _IS("typeCons");
	 _EXTERN _CONST TPZString TPZ_TAG_RSPTSIM	        _IS("RSPTSIM");
	 _EXTERN _CONST TPZString TPZ_TAG_CONSUMER_READLAT      _IS("readLatency");
	 _EXTERN _CONST TPZString TPZ_TAG_CONSUMER_WRITELAT     _IS("writeLatency");
	 _EXTERN _CONST TPZString TPZ_TAG_CONSUMER_BUFFERSIZE   _IS("bufferSize");	 
	 
   _EXTERN _CONST TPZString TPZ_TAG_SIMPLE_ROUTER    _IS("SimpleRouter");  //Monoblock router, no internal components
         _EXTERN _CONST TPZString TPZ_TAG_MESH_DOR                               _IS("MESH-DOR");
         _EXTERN _CONST TPZString TPZ_TAG_MESH_DOR_FAST                          _IS("MESH-DOR-FAST");
         _EXTERN _CONST TPZString TPZ_TAG_TORUS_DOR                              _IS("TORUS-DOR");
         _EXTERN _CONST TPZString TPZ_TAG_MESH_DOR_BINOC                         _IS("MESH-DOR-BINOC");
         _EXTERN _CONST TPZString TPZ_TAG_TORUS_BLESS                            _IS("TORUS-BLESS");
	 _EXTERN _CONST TPZString TPZ_TAG_LIGERO                                 _IS("LIGERO");
	 _EXTERN _CONST TPZString TPZ_TAG_LIGERO_MCAST                           _IS("LIGERO-MCAST");
	 	_EXTERN _CONST TPZString TPZ_TAG_ISSIZE                          _IS("IsSize");
		_EXTERN _CONST TPZString TPZ_TAG_OSSIZE                          _IS("OsSize");
		_EXTERN _CONST TPZString TPZ_TAG_MPSIZE                          _IS("MpSize");
		_EXTERN _CONST TPZString TPZ_TAG_MISS_LOOPS                      _IS("MissLoops");
		_EXTERN _CONST TPZString TPZ_TAG_MISS_LIMIT                      _IS("MissLimit");
	 
   _EXTERN _CONST TPZString TPZ_TAG_ROUTER           _IS("Router");        //Router with multiple internal components
         _EXTERN _CONST TPZString TPZ_TAG_BUFFER           _IS("Buffer");
         _EXTERN _CONST TPZString TPZ_TAG_MPBUFFER         _IS("MPBuffer");
         _EXTERN _CONST TPZString TPZ_TAG_MPIOBUFFER       _IS("MPIOBuffer");
         _EXTERN _CONST TPZString TPZ_TAG_ROUTING          _IS("Routing");
         _EXTERN _CONST TPZString TPZ_TAG_ROUTING_MUXED    _IS("RoutingMuxed");
         _EXTERN _CONST TPZString TPZ_TAG_CROSSBAR         _IS("Crossbar");
         _EXTERN _CONST TPZString TPZ_TAG_MUX              _IS("Multiplexor");
         _EXTERN _CONST TPZString TPZ_TAG_DEMUX            _IS("Demultiplexor");
	 _EXTERN _CONST TPZString TPZ_TAG_ISTAGE           _IS("InputStage");
         _EXTERN _CONST TPZString TPZ_TAG_OSTAGE           _IS("OutputStage");
    
    
    _EXTERN _CONST TPZString TPZ_TAG_MESHNET          _IS("MeshNetwork");
    _EXTERN _CONST TPZString TPZ_TAG_TORUSNET         _IS("TorusNetwork");
    _EXTERN _CONST TPZString TPZ_TAG_MIDIMEWNET       _IS("MidimewNetwork");
    _EXTERN _CONST TPZString TPZ_TAG_SQUAREMIDIMEWNET _IS("SquareMidimewNetwork");
    _EXTERN _CONST TPZString TPZ_TAG_SIMULATION       _IS("Simulation");
    
    _EXTERN _CONST TPZString TPZ_TAG_XPLUS           _IS("X+");
    _EXTERN _CONST TPZString TPZ_TAG_XMINUS          _IS("X-");
    _EXTERN _CONST TPZString TPZ_TAG_YPLUS           _IS("Y+");
    _EXTERN _CONST TPZString TPZ_TAG_YMINUS          _IS("Y-");
    _EXTERN _CONST TPZString TPZ_TAG_ZPLUS           _IS("Z+");
    _EXTERN _CONST TPZString TPZ_TAG_ZMINUS          _IS("Z-");
    _EXTERN _CONST TPZString TPZ_TAG_LOCAL           _IS("Node");

    _EXTERN _CONST TPZString ICL_TAG_BUBBLE_SIZE     _IS("bubbleSize");
    _EXTERN _CONST TPZString TPZ_TAG_WIDE_MEM        _IS("wideMemory");
    _EXTERN _CONST TPZString TPZ_TAG_MISSR_LIMIT     _IS("missrtgLimit");
    _EXTERN _CONST TPZString TPZ_TAG_SIZE            _IS("size");
    _EXTERN _CONST TPZString TPZ_TAG_SIZE_SHORT      _IS("sizeShort");
    _EXTERN _CONST TPZString TPZ_TAG_CONTROL         _IS("control");
    _EXTERN _CONST TPZString TPZ_TAG_INPUTS          _IS("inputs");
    _EXTERN _CONST TPZString TPZ_TAG_OUTPUTS         _IS("outputs");
    _EXTERN _CONST TPZString TPZ_TAG_BUFFER_SIZE     _IS("bufferSize");
    _EXTERN _CONST TPZString TPZ_TAG_MULTIPORT_SIZE  _IS("multiportSize");    
    _EXTERN _CONST TPZString TPZ_TAG_BUFFER_CONTROL  _IS("bufferControl");
    _EXTERN _CONST TPZString TPZ_TAG_MUX_CONTROL     _IS("MuxControl");
    _EXTERN _CONST TPZString TPZ_TAG_DEMUX_CONTROL   _IS("DemuxControl");
    _EXTERN _CONST TPZString TPZ_TAG_ROUTING_CONTROL _IS("routingControl");
    _EXTERN _CONST TPZString TPZ_TAG_WRAPPER         _IS("wrapper");
    _EXTERN _CONST TPZString TPZ_TAG_TYPE            _IS("type");
    _EXTERN _CONST TPZString TPZ_TAG_OUTPUTMUX       _IS("mux");
    _EXTERN _CONST TPZString TPZ_TAG_VIRTUALNETWORKS _IS("vnets");
    
    //Crossbar Types
    _EXTERN _CONST TPZString TPZ_TAG_WORMHOLE                   _IS("WH");
    _EXTERN _CONST TPZString TPZ_TAG_WORMHOLE_UNICAST           _IS("WH-UC");
    _EXTERN _CONST TPZString TPZ_TAG_DAMQ                       _IS("DAMQ");
    _EXTERN _CONST TPZString TPZ_TAG_WH_RTG                     _IS("WH-RTG");    
    _EXTERN _CONST TPZString TPZ_TAG_VIRTUAL_CHANNEL            _IS("VC");
    _EXTERN _CONST TPZString TPZ_TAG_VIRTUAL_CHANNEL_OPT        _IS("VC-OPT");
    _EXTERN _CONST TPZString TPZ_TAG_VIRTUAL_CHANNEL_MUXED      _IS("VC-MUX");
    _EXTERN _CONST TPZString TPZ_TAG_VIRTUAL_CHANNEL_MUXED_OPT  _IS("VC-MUX-OPT");
    _EXTERN _CONST TPZString TPZ_TAG_VIRTUAL_CHANNEL_TORUS      _IS("VC-TORUS");
    _EXTERN _CONST TPZString TPZ_TAG_VIRTUAL_CHANNEL_TORUS_MUX  _IS("VC-TORUS-MUX");
    _EXTERN _CONST TPZString TPZ_TAG_CUTHROUGH                  _IS("CT");
    _EXTERN _CONST TPZString TPZ_TAG_CUTHROUGH_UNICAST          _IS("CT-UC");
    _EXTERN _CONST TPZString TPZ_TAG_CUTHROUGH_MULTICAST        _IS("CT-MCAST");
    _EXTERN _CONST TPZString TPZ_TAG_WORMHOLE_MULTICAST         _IS("WH-MCAST");
    _EXTERN _CONST TPZString TPZ_TAG_CUTHROUGH_MUX              _IS("CTM");
    _EXTERN _CONST TPZString TPZ_TAG_MUX_ADAPT                  _IS("MUX-CT");  
      
    _EXTERN _CONST TPZString TPZ_TAG_BLESS              _IS("BLESS");
    _EXTERN _CONST TPZString TPZ_TAG_INJ_RANDOM         _IS("RANDOM");
    _EXTERN _CONST TPZString TPZ_TAG_INJ_RROBIN         _IS("RROBIN");
    _EXTERN _CONST TPZString TPZ_TAG_INJ_FREE           _IS("FREE");
    _EXTERN _CONST TPZString TPZ_TAG_INJ_VNETS          _IS("VNET");
    _EXTERN _CONST TPZString TPZ_TAG_INJ_VC             _IS("INJVC");
    
    //Routing Algorithms types
    _EXTERN _CONST TPZString TPZ_TAG_DOR                                   _IS("DOR");
    _EXTERN _CONST TPZString TPZ_TAG_DORBURBLE                             _IS("DOR-BU");
    
    //Routing Decision units multiplexed (aka muxed Crossbar)
    _EXTERN _CONST TPZString TPZ_TAG_MUXED_DOR                             _IS("DOR-MUX");
    _EXTERN _CONST TPZString TPZ_TAG_MUXED_DOR_BUBBLE                      _IS("DOR-MUX-BUBBLE");
    _EXTERN _CONST TPZString TPZ_TAG_MUXED_ADAP_BUBBLE                     _IS("ADAP-MUX-BUBBLE");
    
    _EXTERN _CONST TPZString TPZ_TAG_SOURCE          _IS("source");
    _EXTERN _CONST TPZString TPZ_TAG_DESTINY         _IS("destiny");

    _EXTERN _CONST TPZString TPZ_TAG_CHANNEL         _IS("channel");

    _EXTERN _CONST TPZString TPZ_TAG_SIZEX           _IS("sizeX");
    _EXTERN _CONST TPZString TPZ_TAG_SIZEY           _IS("sizeY");
    _EXTERN _CONST TPZString TPZ_TAG_SIZEZ           _IS("sizeZ");
    _EXTERN _CONST TPZString TPZ_TAG_REFROUTER       _IS("router");

    _EXTERN _CONST TPZString TPZ_TAG_NETWORK         _IS("Network");
    
   
    _EXTERN _CONST TPZString TPZ_TAG_SIMULCYCLES     _IS("SimulationCycles");
    _EXTERN _CONST TPZString TPZ_TAG_STOPSIMUL       _IS("StopInjectionAfter");
    _EXTERN _CONST TPZString TPZ_TAG_STOPSIMUL_MSG   _IS("StopInjectionAfterMsg");
    _EXTERN _CONST TPZString TPZ_TAG_RESETSTATS      _IS("ResetStats");
    _EXTERN _CONST TPZString TPZ_TAG_PATTERN         _IS("TrafficPattern");
    _EXTERN _CONST TPZString TPZ_TAG_MESSAGELENGTH   _IS("MessageLength");
    _EXTERN _CONST TPZString TPZ_TAG_PACKETLENGTH    _IS("PacketLength");
    _EXTERN _CONST TPZString TPZ_TAG_SEED            _IS("Seed");
    _EXTERN _CONST TPZString TPZ_TAG_LOAD            _IS("Load");
    _EXTERN _CONST TPZString TPZ_TAG_PROB            _IS("Probability");
    
    //Generate repeating patterns of consumption
    _EXTERN _CONST TPZString TPZ_TAG_RAFAGA          _IS("BurstMode");
    _EXTERN _CONST TPZString TPZ_TAG_RAFAGA_WITDTH   _IS("width");
    _EXTERN _CONST TPZString TPZ_TAG_RAFAGA_NUMBER   _IS("number");
    
    
    //Specific Ruby tags
    _EXTERN _CONST TPZString TPZ_TAG_FLITSIZE        _IS("LinkWidth");
    _EXTERN _CONST TPZString TPZ_TAG_NETWORKCLOCK    _IS("NetworkClockRatio");
    _EXTERN _CONST TPZString TPZ_TAG_UNIFYNETWORK    _IS("UnifyNetworks");
    
    //Traffic Pattern tags
    _EXTERN _CONST TPZString TPZ_TAG_EMPTY             _IS("EMPTY");
    _EXTERN _CONST TPZString TPZ_TAG_RANDOM            _IS("RANDOM");
    _EXTERN _CONST TPZString TPZ_TAG_PERMUTATION       _IS("PERMUTATION");
    _EXTERN _CONST TPZString TPZ_TAG_PSUFFLE           _IS("PERFECT-SUFFLE");
    _EXTERN _CONST TPZString TPZ_TAG_BREVERSAL         _IS("BIT-REVERSAL");
    _EXTERN _CONST TPZString TPZ_TAG_TORNADO           _IS("TORNADO");
    _EXTERN _CONST TPZString TPZ_TAG_LOCALTRAFFIC      _IS("LOCAL");
    _EXTERN _CONST TPZString TPZ_TAG_SIGMA             _IS("sigma");
    _EXTERN _CONST TPZString TPZ_TAG_HOTSPOT           _IS("HOT-SPOT");
    _EXTERN _CONST TPZString TPZ_TAG_NODOSHOT          _IS("nodesHot");
    _EXTERN _CONST TPZString TPZ_TAG_PROBHOT           _IS("probHot");
    _EXTERN _CONST TPZString TPZ_TAG_DISTHOT           _IS("dist");
    _EXTERN _CONST TPZString TPZ_TAG_TRACE             _IS("TRACE");
    _EXTERN _CONST TPZString TPZ_TAG_TRACE_FILE        _IS("traceFile");
    
    _EXTERN _CONST TPZString TPZ_TAG_PARAM1            _IS("p1");
    _EXTERN _CONST TPZString TPZ_TAG_PARAM2            _IS("p2");
    _EXTERN _CONST TPZString TPZ_TAG_PARAM3            _IS("p3");
    
    
    _EXTERN _CONST TPZString TPZ_TAG_MODAL             _IS("MODAL");
    _EXTERN _CONST TPZString TPZ_TAG_MULTICAST         _IS("MULTICAST");
    _EXTERN _CONST TPZString TPZ_TAG_PROB_MULTICAST    _IS("probMulticast");
    _EXTERN _CONST TPZString TPZ_TAG_LENGTH_MULTICAST  _IS("lengthMulticast");
    _EXTERN _CONST TPZString TPZ_TAG_REACTIVE          _IS("REACTIVE");
    _EXTERN _CONST TPZString TPZ_TAG_BIMODAL           _IS("BIMODAL");
    _EXTERN _CONST TPZString TPZ_TAG_PROBBIMODAL       _IS("prob");
    _EXTERN _CONST TPZString TPZ_TAG_MSGBIMODAL        _IS("numMsg");
    
    _EXTERN _CONST TPZString TPZ_TAG_NUM_MESS_TYPES    _IS("numMessTypes");
    
    
    _EXTERN _CONST TPZString TPZ_TAG_NUM_VC          _IS("NumVC");
    _EXTERN _CONST TPZString TPZ_TAG_NUM_MT          _IS("NumMT");
    
                            
    
    _EXTERN _CONST TPZString TPZ_TAG_HEADER_DELAY    _IS("headerDelay");
    _EXTERN _CONST TPZString TPZ_TAG_DATA_DELAY      _IS("dataDelay");
    _EXTERN _CONST TPZString TPZ_TAG_RTG_DELAY       _IS("routingDelay");
    _EXTERN _CONST TPZString TPZ_TAG_VA_DELAY        _IS("vcarbDelay");
    _EXTERN _CONST TPZString TPZ_TAG_SA_DELAY        _IS("swarbDelay");
    _EXTERN _CONST TPZString TPZ_TAG_ST_DELAY        _IS("swtravDelay");
    _EXTERN _CONST TPZString TPZ_TAG_DELAY           _IS("delay");
    _EXTERN _CONST TPZString TPZ_TAG_CHANNEL_MODE    _IS("operation");
    _EXTERN _CONST TPZString TPZ_TAG_HD_TYPE         _IS("tipoReverse");

 //*************************************************************************


 #endif


 // end of file

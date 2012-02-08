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
//    File: TPZSimulator.cpp
//
//    Class:  TPZSimulator
//
//    Inherited from:  TPZNotifier
// :
//*************************************************************************
//end of header



#include "TPZSimulator.hpp"

#include "TPZRouterBuilder.hpp"       // TPZRouterBuilder
#include "TPZNetworkBuilder.hpp"       // TPZNetworkBuilder
#include "TPZSimulationBuilder.hpp"       // TPZSimulationBuilder
#include "TPZSimulation.hpp"       // TPZSimulation
#include "TPZIniFile.hpp"         // TPZIniFile
#include "TPZOptions.hpp"         // TPZOptions
#include "TPZConst.hpp"         // Consistent TPZ
#include "TPZBase.hpp"

#include <signal.h>


//*************************************************************************

#define  OPT_SIMUL_NAME         's'
#define  OPT_SIMUL_LOAD         'l'
#define  OPT_SIMUL_PROB         'p' //VPV
#define  OPT_RSIM_EVOL          'd' // Dynamic evolution of net
#define  OPT_RSIM_EVOL_CYCLE    'm' // Dynamic evolution of net cycle
#define  OPT_SIMUL_STATE        'q'
#define  OPT_SIMUL_CYCLES       'c'
#define  OPT_SIMUL_BUFFERFILE   'b'
#define  OPT_SIMUL_PATTERNFILE  'f'
#define  OPT_SIMUL_TRAFFIC      't'
#define  OPT_SIMUL_BIMODALTRAFF 'B'
#define  OPT_SIMUL_PACKETLENGTH 'L'
#define  OPT_SIMUL_BUFFERSIZE   'u'
#define  OPT_CLOCK_TRACE        'C'
#define  OPT_NO_SEED            'r' //fixed seed
#define  OPT_RES_VERBOSITY      'v'
#define  OPT_SIMUL_BCAST_PROB   'A' //Probability of generation a CAST messages
#define  OPT_SPECIFY_NETWORK    'n' //Specifies network. Non-operating
#define  OPT_BEGIN_TRACE_DUMP   'T' //Starts to roll after that the trace clock. Not operational
#define  OPT_RANDOM_SEED        'R' //Variability simulations
#define  OPT_INJECT_MAP         'I' //Show map periodically or at the end injeccion
#define  OPT_NUM_THREADS        'N' //Create N threads
#define  OPT_DROP_TRAFFIC       'D' //Drop traffic if injectionqueues are full
#define  OPT_SGML_NAME          'F'

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZSimulator,TPZNotifier);

//*************************************************************************

TPZSimulator* TPZSimulator :: m_Instance = 0;


//*************************************************************************
//:
//  f: static void handlerSignalUSR1 (int val);
//
//  d:
//:
//*************************************************************************

static void handlerSignalUSR1(int val)
{
    TPZBase::setErrorText("asda");
   for(int i=1; i<=TPZ_MAX_NUMBER_SIMULATIONS; i++)
   {
      TPZSimulation* simulation = TPZSIMULATOR()->getSimulation(i);
      if(simulation)
      {
         ofstream ofs;
         ofs.open(TPZ_LOG_FILENAME,ios::app);
         simulation->getNetwork()->writeComponentStatus(ofs);
         ofs.close();
      }
   }

   signal(SIGUSR1,handlerSignalUSR1);
}


//*************************************************************************
//:
//  f: static void handlerSignalUSR2 (int val);
//
//  d:
//:
//*************************************************************************

static void handlerSignalUSR2(int val)
{
   for(int i=1; i<=TPZ_MAX_NUMBER_SIMULATIONS; i++)
   {
      TPZSimulation* simulation = TPZSIMULATOR()->getSimulation(i);
      if(simulation)
      {
         ofstream ofs;
         ofs.open(TPZ_LOG_FILENAME,ios::app);
         simulation->writeSimulationStatus(ofs);
         ofs.close();
      }
   }

   signal(SIGUSR2,handlerSignalUSR2);
}

//*************************************************************************
//:
//  f: TPZSimulator ();
//
//  d:
//:
//*************************************************************************

TPZSimulator :: TPZSimulator()
              : m_SimulationArray(0),
                m_OldHandler(0)
{
   disableNotification();
   m_SimulationArray = new TSimulationArray(TPZ_MAX_NUMBER_SIMULATIONS);
   m_SimulationArray->initialize(0);   
   m_OldHandler = signal(SIGUSR2,handlerSignalUSR2);
   signal(SIGUSR1,handlerSignalUSR1);
   
}


//*************************************************************************
//:
//  f: ~ TPZSimulator ();
//
//  d:
//:
//*************************************************************************

TPZSimulator :: ~TPZSimulator()
{
   deleteAllSimulations();
   delete m_SimulationArray;
   signal(SIGUSR2,m_OldHandler);
}


//*************************************************************************
//:
//  f: TPZSimulator * instance ();
//
//  d:
//:
//*************************************************************************

TPZSimulator* TPZSimulator :: instance()
{
   if( ! m_Instance )
      m_Instance = new TPZSimulator();

   return m_Instance;
}


//*************************************************************************
//:
//  f: void destroyInstance ();
//
//  d:
//:
//*************************************************************************

void TPZSimulator :: destroyInstance()
{
   delete m_Instance;
   m_Instance = 0L;
}



//*************************************************************************
//:
//  f: unsigned createSimulation (int argc, char * argv []);
//
//  d:
//:
//*************************************************************************

unsigned TPZSimulator :: createSimulation(int argc, char* argv[])
{
   TPZString param;
   for( int i=0; i<argc; i++ )
   {
      param += TPZString(argv[i]) + " ";
   }
   return createSimulation(param);
}


//*************************************************************************
//:
//  f: unsigned createSimulation (const TPZString & param);
//
//  d:
//:
//*************************************************************************

unsigned TPZSimulator :: createSimulation(const TPZString& param)
{
   unsigned simulations=0;
   
   for(int i=1; i<=TPZ_MAX_NUMBER_SIMULATIONS; i++)
   {
      TPZSimulation* temp = 0;
      m_SimulationArray->valueAt(i,temp);
      if(temp) simulations++;
   }   
   
   if( simulations == TPZ_MAX_NUMBER_SIMULATIONS )
   {
      TPZString err;
      err.sprintf(ERR_TPZSIMUL_001, TPZ_MAX_NUMBER_SIMULATIONS);
      EXIT_PROGRAM(err);
   }

   TPZSimulation* simulation = initialize(param);
   if( simulation )
   {
      for(int i=1; i<=TPZ_MAX_NUMBER_SIMULATIONS; i++)
      {
         TPZSimulation* temp = 0;
         m_SimulationArray->valueAt(i,temp);
         if( ! temp )
         {
            m_SimulationArray->setValueAt(i,simulation);
            return i;
         }  
      }
   }
   return 0;
}


//*************************************************************************
//:
//  f: getSimulation TPZSimulation * (unsigned index) const;
//
//  d:
//:
//*************************************************************************

TPZSimulation* TPZSimulator :: getSimulation(unsigned index) const
{
   TPZSimulation* simulation = 0;
   m_SimulationArray->valueAt(index,simulation);
   return simulation;
}


//*************************************************************************
//:
//  f: void deleteSimulation (unsigned index);
//
//  d:
//:
//*************************************************************************

void TPZSimulator :: deleteSimulation(unsigned index)
{
   TPZSimulation* simulation = getSimulation(index);
   if( simulation )
   {
      delete simulation;
      m_SimulationArray->setValueAt(index,(TPZSimulation*)0);
   }
}


//*************************************************************************
//:
//  f: void deleteAllSimulations ();
//
//  d:
//:
//*************************************************************************

void TPZSimulator :: deleteAllSimulations()
{
   for(int i=1; i<=TPZ_MAX_NUMBER_SIMULATIONS; i++)
   {
      deleteSimulation(i);
   }
}


//*************************************************************************
//:
//  f: TPZSimulation * initialize (const TPZString & param);
//
//  d:
//:
//*************************************************************************

TPZSimulation* TPZSimulator :: initialize(const TPZString& param)
{
   TPZOptions opts(param);
   
   TPZString sgmlName = opts.getValueForOption(OPT_SGML_NAME);
   if(sgmlName == TPZString(""))
   {
      TPZString iniFileName= param.word(1) + TPZString(".ini");
      installBuilders(iniFileName); 
   }
   else
   {
      installBuilders(sgmlName);
   }
   
   TPZSimulationBuilder* sBuilder = (TPZSimulationBuilder*)
                                    TPZComponent::simulationBuilder;
   
   TPZString simulationName = opts.getValueForOption(OPT_SIMUL_NAME);
   TPZString bufferFile     = opts.getValueForOption(OPT_SIMUL_BUFFERFILE); 
   TPZString loadString     = opts.getValueForOption(OPT_SIMUL_LOAD);
   TPZString probString     = opts.getValueForOption(OPT_SIMUL_PROB);
   TPZString rsimtrace      = opts.getValueForOption(OPT_RSIM_EVOL);
   TPZString traceByCycle   = opts.getValueForOption(OPT_RSIM_EVOL_CYCLE);
   TPZString cyclesString   = opts.getValueForOption(OPT_SIMUL_CYCLES);
   TPZString patternFile    = opts.getValueForOption(OPT_SIMUL_PATTERNFILE);
   TPZString traffic        = opts.getValueForOption(OPT_SIMUL_TRAFFIC);
   TPZString bimodal        = opts.getValueForOption(OPT_SIMUL_BIMODALTRAFF);
   TPZString packetLength   = opts.getValueForOption(OPT_SIMUL_PACKETLENGTH);
   TPZString bufferSize     = opts.getValueForOption(OPT_SIMUL_BUFFERSIZE);
   TPZString clockTrace     = opts.getValueForOption(OPT_CLOCK_TRACE);
   TPZString network        = opts.getValueForOption(OPT_SPECIFY_NETWORK);
   TPZString dumptraza      = opts.getValueForOption(OPT_BEGIN_TRACE_DUMP);
   TPZString seed           = opts.getValueForOption(OPT_RANDOM_SEED);
   TPZString injectMap      = opts.getValueForOption(OPT_INJECT_MAP);
   TPZString numHilos       = opts.getValueForOption(OPT_NUM_THREADS);
   TPZString verbosity      = opts.getValueForOption(OPT_RES_VERBOSITY);
   
   Boolean   dropTraffic    = opts.isOptionActive(OPT_DROP_TRAFFIC);
   Boolean   noSeed         = opts.isOptionActive(OPT_NO_SEED);
   Boolean   noDisplay      = opts.isOptionActive(OPT_SIMUL_STATE);
   

   TPZSimulation* simulation = (TPZSimulation*)
                  (sBuilder->createComponentWithId(simulationName));
   
   // After Creating a simulation run, the builders Are No Longer
   // to re-use. To Avoid to keep hold of the
   // During The simulation memory, plow uninstalled.
   
   if( network != TPZString("") )
   {
      simulation -> overrideNetwork(network);
   }
#ifndef NO_TRAZA
   if( dumptraza != TPZString("") )
   {
      simulation ->setDumpTraza(dumptraza.asInteger());
   }
#else
   if( dumptraza != TPZString("") )
   {
      TPZString err=" Ignored option -T <clock> only maskes sense when simulator is compuled without -DNO_TRAZA";
      EXIT(err);
   }
#endif
   
   uninstallBuilders();

   if( ! simulation )
   {
      TPZString texto = " Simulation don't found: '";
      texto += simulationName + "'";
      EXIT(texto);
   }
   
   if( probString != TPZString("") && loadString != TPZString("") )
   {
      TPZString texto = "No puedes usar  -l<carga norm> y -p<proba inyec> a la vez";
      EXIT(texto);
   }
   
   if( opts.getValueForOption(OPT_INJECT_MAP) || injectMap!=TPZString(""))
   {
      if(injectMap==TPZString(""))
      {
         simulation -> getNetwork()->setDumpMapInjectors(1);
      }
      else
      {
         simulation -> getNetwork()->setDumpMapInjectors(injectMap.asInteger());
      }
   }
#ifdef PTOPAZ
   if( numHilos != TPZString("") )
   {
      simulation -> setNumberThreads(numHilos.asInteger());
   } else
   {
      simulation -> setNumberThreads(1);
   }
#else
   if( numHilos != TPZString("") )
      cerr << "WARNING: Number of threads defined, and PTOPAZ is disabled!!!" << endl;
#endif

   if( clockTrace != TPZString("") )
   {
      simulation -> setClock(clockTrace.asDouble());
   }

   if( packetLength != TPZString("") )
   {
      simulation -> setPacketLength(packetLength.asInteger());
   }

   if( cyclesString != TPZString("") )
   {
      simulation->setSimulationCycles(cyclesString.asInteger());
   }

   if( patternFile != TPZString("") )
   {
      simulation->setTrafficPatternFile(patternFile);
   }

   if( traffic != TPZString("") )
   {
      simulation->setTrafficPattern(traffic);
   }

   if( bimodal != TPZString("") )
   {
      simulation->setBimodalTraffic(bimodal);
   }
   if( verbosity != TPZString("") )
   {
      simulation->setVerbosity(verbosity.asInteger());
   }
   if( probString == TPZString("") && loadString == TPZString("") )
   {
      if(simulation->defaultLoadProb() != -1 && simulation->defaultLoadSupply() != -1)
      {
         TPZString texto = "No puedes usar  -l<carga norm> y -p<proba inyec> a la vez";
         EXIT(texto);
      }
      else if(simulation->defaultLoadProb() != -1)
      {
         simulation->setLoadProb(simulation->defaultLoadProb());  
      }
      else if(simulation->defaultLoadSupply() != -1)
      {
         simulation->setLoadSupply(simulation->defaultLoadSupply());  
      }
      else
      {
         TPZString texto = "Must specify load or probability";
         EXIT(texto);
      }
   }
   else if( probString != TPZString("") )
   {
      simulation->setLoadProb((probString.asDouble())/*/50*/);
   }
   else if( loadString != TPZString("") )
   {
      simulation->setLoadSupply(loadString.asDouble());
   }
   
   if( noDisplay ) simulation->setShowState(false);

   if( bufferFile != TPZString("") )
   {
      simulation -> setShowBufferState(true);
      simulation -> setBuffersFileName(bufferFile);
   }

   if( bufferSize != TPZString("") )
   {
      simulation -> setBufferSize(bufferSize.asInteger());
   }
   else
   {
      //I have to start the message pool indirectly
      simulation ->  setBufferSize(0);
   }
   
   if( rsimtrace != TPZString("") )
   {
      simulation -> getNetwork()->setReportMsg(rsimtrace.asInteger());
   }
   
   if( traceByCycle != TPZString(""))
   {
      simulation -> getNetwork()->setReportCycles(traceByCycle.asInteger());
   }
   
   if(seed != TPZString(""))
   {
      simulation -> setSeed ( seed.asInteger() );
   }
   
   if(!noSeed)
   {
      simulation -> enableSeed();
   }
   
   if( dropTraffic  )
   {
      simulation -> discardTraffic();
   }

   simulation->getNetwork()->postInitialize();
   
   return simulation;
}


//*************************************************************************
//:
//  f: void installBuilers (TPZString const & name);
//
//  d:
//:
//*************************************************************************

void TPZSimulator :: installBuilders(const TPZString& name)
{
   TPZIniFile iniFile(name);
   
   TPZString rFile = iniFile.getValueFor("RouterFile");
   TPZString nFile = iniFile.getValueFor("NetworkFile");
   TPZString sFile = iniFile.getValueFor("SimulationFile");
   TPZRouterBuilder*     rBuilder = new TPZRouterBuilder(rFile);
   TPZNetworkBuilder*    nBuilder = new TPZNetworkBuilder(nFile);
   TPZSimulationBuilder* sBuilder = new TPZSimulationBuilder(sFile);
   
   rBuilder->loadSgmlInformation();
   nBuilder->loadSgmlInformation();
   sBuilder->loadSgmlInformation();
   
   TPZComponent::routerBuilder    = rBuilder;
   TPZComponent::networkBuilder   = nBuilder;
   TPZComponent::simulationBuilder= sBuilder;
}

//*************************************************************************
//:
//  f: void uninstallBuilers ();
//
//  d:
//:
//*************************************************************************

void TPZSimulator :: uninstallBuilders()
{
   delete TPZComponent::routerBuilder;
   delete TPZComponent::networkBuilder;
   delete TPZComponent::simulationBuilder;

   TPZComponent::routerBuilder    = 0;
   TPZComponent::networkBuilder   = 0;
   TPZComponent::simulationBuilder= 0;
}

//*************************************************************************

// end of file

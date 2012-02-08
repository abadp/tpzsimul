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
//    File: TPZSimulation.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZSIMUL_HPP__
#define __TPZSIMUL_HPP__

//************************************************************************

   #ifndef __TPZCOMPO_HPP__
   #include "TPZComponent.hpp"       // TPZComponent
   #endif

   #ifndef __TPZNET_HPP__
   #include "TPZNetwork.hpp"         // TPZNetwork
   #endif

   #ifndef __TPZGLOBA_HPP__
   #include "TPZGlobalData.hpp"       // TPZGlobalData
   #endif

   #ifndef __TPZPool_HPP__
   #include <TPZPool.hpp>
   #endif

   #ifndef __TPZVector_HPP__
   #include <TPZVector.hpp>
   #endif


   #include <time.h>
   #include <unistd.h>
   #include <signal.h>

//************************************************************************

   class TPZSimulationBuilder;
   class TPZTrafficPattern;   
#ifdef PTOPAZ
   typedef TPZVector<TPZMessagePool*> TPZVectorPool;
#endif
         
//************************************************************************


   class TPZSimulation : public TPZComponent
   {
      typedef TPZComponent Inhereited;
      friend class TPZSimulationBuilder;
      
      
   public:
      TPZSimulation(const TPZComponentId& id);
      virtual ~TPZSimulation();

      virtual TPZString asString() const;
      virtual void run(uTIME time=0);
      virtual void initialize();
      virtual void writeResults();
      void writeSimulationStatus(ostream& os);
      TPZString writeSimulationStatus();
      void resetStats();
      void setTrafficPattern(const TPZString& name);
      void setBimodalTraffic(const TPZString& params);
      void setTrafficPatternFile(const TPZString& name);
      
      void overrideNetwork(const TPZString& name );
      
      Boolean injectProtocolMessage(const TPZPosition& source, const TPZPosition& destiny, unsigned index); 
      
      Boolean injectProtocolRandomMessage(const TPZPosition& source, unsigned type);
      
      //*******************************************************************
      //ADDED FOR MULTIPLE TRAFFIC SOURCES SHARING THE ONLY VIRTUAL CHANNEL
      //*******************************************************************
      
      void setProtocolMessTypes ( unsigned types )
      { m_ProtocolTypes = types; }
      
      unsigned getProtocolMessTypes ()
      { return m_ProtocolTypes; }
      
      //*******************************************************************
      //END OF ADDITION
      //*******************************************************************
      
      //*******************************************************************
      //ADDED FOR BYPASS STATISTICS CALCULATION
      //*******************************************************************
      void incrPipelineNormal()
      { m_pipelineNormal++; }
      
      void incrPipelineBypass()
      { m_pipelineBypass++; }
      
      //*******************************************************************
      //END OF ADDITION
      //*******************************************************************
      
      void* getExternalInfoAt(unsigned x, unsigned y, unsigned z=0)
      { return getNetwork()->getExternalInfoAt(x,y,z); }
            
      virtual void* getExternalInfoAt(TPZPosition pos,int id=0)
      { return getNetwork()->getExternalInfoAt(pos); }
            
      uTIME getCurrentTime() const
      { return m_Clock; }
      
      void setCurrentTime(uTIME time) 
      { m_Clock = time; }
      
      void setNetwork(TPZNetwork* net)
      { m_Network = net; }
      
      virtual TPZNetwork* getNetwork(int a=0) const
      { return m_Network; }
      
      void setSimulationCycles(unsigned long cycles)
      { m_SimulationCycles = cycles; }

      unsigned long getSimulationCycles() const
      { return m_SimulationCycles; }

      void setStopSimulation(unsigned long cycles)
      { m_StopSimulation = cycles; }
      
      void setStopSimulMessages(unsigned long cycles)
      { m_StopSimulMessages = cycles; }
      
      unsigned long getStopSimulMessages()
      { return m_StopSimulMessages; }
      
      void decrStopSimulMessages()
      { m_StopSimulMessages--; }

      void setBurstWidth (unsigned cycles )
      { 
        setStopSimulation(cycles);
        m_burstWidth = cycles; 
      }
      
      unsigned getBurstWidth()const
      { return m_burstWidth;}
      
      void setBurstRepet (unsigned times)
      { m_burstNumber = times;}
      
      unsigned getBurstRepet() const
      { return m_burstNumber;}
      
      void setResetStats(unsigned long cycles)
      { m_resetStats = cycles; }
      
      void setSeed(unsigned seed)
      { m_Seed = seed; }
      
      void setMessageLength(unsigned length)
      { m_MessageLength = length; }

      unsigned getMessageLength() const
      { return m_MessageLength; }

      virtual void setPacketLength(unsigned length,int net=0)
      { m_PacketLength = length; }

      virtual unsigned getPacketLength(TPZNetwork *net=0) const
      { return m_PacketLength; }

      void setRadius(unsigned radius)
      { m_Radius = radius; }

      unsigned getRadius() const
      { return m_Radius; }

      double defaultLoadSupply() const
      { return m_defaultLoad; }

      void setDefaultLoadSupply(double value)
      { m_defaultLoad = value; }
      
      void setLoadSupply(double value);
      
      double defaultLoadProb() const
      { return m_defaultProb; }

      void setDefaultLoadProb(double value)
      { m_defaultProb = value; }
      
      void setLoadProb(double value);
      
      void setVerbosity(unsigned value)
      { m_verbosity = value; }
      
      void setShowState(Boolean value)
      { m_ShowState = value; }

      void setTrafficPattern(TPZTrafficPattern* pattern)
      { m_TrafficPattern = pattern; }

      void setShowBufferState(Boolean value)
      { m_ShowBufferState = value; }
      
      unsigned getFlitSize( )const
      { return m_flitSize; }
                  
      void setClock(float value)
      { m_clockTR = (unsigned long)value; }
      
      float getClock( )const
      { return m_clockTR; }
      
      void setFlitSize(unsigned value)
      { m_flitSize = value; }
                  
      void setBuffersFileName(const TPZString& name);
      
      void setAsciiOutputFormat()
      {
         m_binaryOutputFormat=false;
      }
      
      Boolean getAsciiOutputFormat()
      {
         return m_binaryOutputFormat;
      }
      
      void enableSeed()
      {
         srand(m_Seed);
         srand48(m_Seed);
      }

      void setBufferSize(unsigned size)
      { 
        if(m_Network)
        { 
           m_Network->setBufferSize(size); 
           m_bufferSize=size;
        }
      }
     
     
     unsigned getBufferSize()const
     {return m_bufferSize;}
     
     Boolean needToUnify() const
     {return m_unifyNetworks;}
     
     double getNetworkClockRatioSGML() const
     { return m_RouterProcesorRatio; }
     

     TPZGlobalData& globalData(TPZNetwork* net=0) const
     { return (TPZGlobalData&)m_GlobalData; }
      
     void setNumberOfMsgs(unsigned long a)
     {
        m_mensajestraza=a;
     }
     
     void setDumpTraza(int a)
     {
        m_startDump=a;
     }
     
     int getDumpTraza() const
     {
        return m_startDump;
     }
     
     void discardTraffic ()
     {
        m_discardTraffic = true;
     }

     Boolean isDiscardTraffic () const
     {
        return m_discardTraffic;
     }
      
      // Run time information
      DEFINE_RTTI(TPZSimulation);
      
      void setLastMessage (uTIME time)
      { m_LastMessage = time; }
      
      uTIME getLastMessage ()
      { return m_LastMessage; }
      
      void incrTerminatingMsgNumber ()
      { m_terminatingMsgNumber++; }
      
      unsigned getTerminatingMsgNumber ()
      { return m_terminatingMsgNumber; }
      
      Boolean getisRafagaMode ()
      { return m_isRafagaMode; }
      
      TPZMessagePool& getMessagePool() const;
      
      void initializePool(unsigned size);

      double getLoad() const
      {
         return m_q;
      }
      
#ifdef PTOPAZ

      unsigned getNumberThreads ()
      {
         return m_numberThreads;
      }
      
      void setNumberThreads (unsigned n)
      {
         m_numberThreads = n;
      }
      
      TPZMessagePool& getMessagePool(unsigned n) const;
      
#endif
                  
      TPZTrafficPattern* getTrafficPattern() const
      { return m_TrafficPattern; }
   
   protected:


      TPZString buffersFileName() const
      { return m_BuffersFileName; }

      Boolean showBufferState() const
      { return m_ShowBufferState; }
      
      unsigned showResolutionBS() const
      { return m_BufferStateResolution;}
      
      void      writeBufferStatus();
      TPZString convertSeconds(double secs);
      void      writeCurrentData();
            
      static TPZSimulation* newFrom( const TPZTag* tag, 
                                     INDEX& i);
      void parseTags(TPZSimulation *simulation,const TPZTag* tag, 
                                     INDEX& i);

      TPZNetwork*        m_Network;
      TPZGlobalData      m_GlobalData;

   protected:
      TPZTrafficPattern* m_TrafficPattern;
      
      Boolean            m_isRafagaMode;
      unsigned short     m_ProtocolTypes;
      unsigned long      m_SimulationCycles;
      unsigned long      m_StopSimulation;
      unsigned long      m_StopSimulMessages;
      unsigned long      m_resetStats;
      unsigned           m_Seed;
      unsigned           m_MessageLength;
      unsigned           m_PacketLength;
      unsigned           m_verbosity;
      unsigned           m_Radius;
      unsigned           m_flitSize;
      double             m_q;
      double             m_defaultLoad;
      double             m_defaultProb;
      Boolean            m_ShowState;
      time_t             m_StartHour;
      uTIME              m_Clock;
      uTIME              m_simulatedClock;
      ofstream*          m_BufferOutput;
      Boolean            m_ShowBufferState;
      unsigned           m_BufferStateResolution;
      Boolean            m_binaryOutputFormat;
      TPZString          m_BuffersFileName;
      unsigned long      m_mensajestraza;
      unsigned long      m_clockTR;
      unsigned           m_burstWidth;
      unsigned           m_burstNumber;
      unsigned           m_bufferSize;
      Boolean            m_discardTraffic;
            
      unsigned           m_terminatingMsgNumber;
      uTIME              m_LastMessage;
      
      unsigned long      m_pipelineNormal;
      unsigned long      m_pipelineBypass;
      
      //Ruby Member
      double             m_RouterProcesorRatio;
      Boolean            m_unifyNetworks;
      TPZString          m_overrideNetwork;
      int                m_startDump;
#ifdef PTOPAZ
      TPZVectorPool      m_MessagePool;
      unsigned           m_numberThreads;
#else
      TPZMessagePool*    m_MessagePool;
#endif
      unsigned           m_poolSize;
   private:
      //profiling functions taken from GEMS
         static double process_memory_total()
         {
           const double MULTIPLIER = 4096.0/(1024.0*1024.0); // 4kB page size, 1024*1024 bytes per MB, 
           ifstream proc_file;
           proc_file.open("/proc/self/statm");
           int total_size_in_pages = 0;
           int res_size_in_pages = 0;
           proc_file >> total_size_in_pages;
           proc_file >> res_size_in_pages;
           return double(total_size_in_pages)*MULTIPLIER; // size in megabytes
         }

         static double process_memory_resident()
         {
           const double MULTIPLIER = 4096.0/(1024.0*1024.0); // 4kB page size, 1024*1024 bytes per MB, 
           ifstream proc_file;
           proc_file.open("/proc/self/statm");
           int total_size_in_pages = 0;
           int res_size_in_pages = 0;
           proc_file >> total_size_in_pages;
           proc_file >> res_size_in_pages;
           return double(res_size_in_pages)*MULTIPLIER; // size in megabytes
         }
   };

//*************************************************************************

#endif


// end of file

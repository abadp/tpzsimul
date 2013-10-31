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
//    File: TPZTrafficPattern.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZTrafficPattern_HPP__
#define __TPZTrafficPattern_HPP__

//************************************************************************

   //# Include <stdlib.h>

   #ifndef __TPZRunTimeInformation_HPP__
   #include <TPZRunTimeInformation.hpp>
   #endif

   #ifndef __TPZString_HPP__
   #include <TPZString.hpp>
   #endif

   #ifndef __TPZMessage_HPP__
   #include <TPZMessage.hpp>
   #endif

   #ifndef __TPZSimulation_HPP__
   #include <TPZSimulation.hpp>
   #endif

   #ifndef __TPZArray_HPP__
   #include <TPZArray.hpp>
   #endif

//************************************************************************

   class TPZTag;
   class TPZPosition;
   class TPZNetwork;
   //class ifstream;
      
//************************************************************************

   class TPZTrafficPattern
   {      
   public:   
      TPZTrafficPattern(TPZSimulation& simul);
      virtual ~TPZTrafficPattern() { }
      void generateMessages(double q);
      void generateMessagesXsegment(double q,int from, int to);
      virtual Boolean injectMessage(const TPZPosition& src=TPZPosition(0,0,0)) = 0;
      
      Boolean injectProtocolMessage(const TPZPosition& source, const TPZPosition& destiny, unsigned type);
      
      Boolean injectProtocolRandomMessage(const TPZPosition& source, unsigned type);
      
      virtual double  getInjectFactor(const TPZPosition& pos)
      { return 1.0; }
      
      virtual double getClock()
      {return 0;}
            
      virtual TPZString asString() const 
      { return (TPZString(isBimodal() ? TPZString("Bimodal ") : TPZString("")) + (getProbMulticast()>0? TPZString("Multicast enabled PROB:")+TPZString(getProbMulticast())+TPZString(" "):TPZString(" ")));}
      
      void setParameter1(double param)
      { m_Parameter1 = param; }

      void setParameter2(double param)
      { m_Parameter2 = param; }

      void setParameter3(double param)
      { m_Parameter3 = param; }

      double getParameter1() const
      { return m_Parameter1; }

      double getParameter2() const
      { return m_Parameter2; }

      double getParameter3() const
      { return m_Parameter3; }

      Boolean isFromFile() const
      { return m_IsFile; }

      Boolean isBimodal() const
      { return m_Bimodal; }

      virtual Boolean isEmpty() const
      { return false; }
      
      void setProbBimodal(double prob)
      { m_ProbBimodal = prob; }

      void setMessagesBimodal(unsigned nMsg)
      { m_NumMsgBimodal = nMsg; }

      double getProbBimodal() const
      { return m_ProbBimodal; }
      
      double getMessagesBimodal() const
      { return m_NumMsgBimodal; }
      
      void setBimodal()
      { m_Bimodal = true; }
      
      //*****************************
      //MULTICAST TRAFFIC
      //*****************************
      
      void setMulticast()
      { m_Multicast = true; }
      
      void setProbMulticast (double prob)
      { m_ProbMulticast = prob; }
      
      double getProbMulticast () const
      { return m_ProbMulticast; }
      
      Boolean isMulticast() const
      { return m_Multicast; }
      
      void setLengthMulticast (unsigned length)
      { m_LengthMulticast = length; }
      
      unsigned getLengthMulticast () const
      { return m_LengthMulticast; }
      //********************************
      
      //*******************************************************************
      //ADDED FOR MULTIPLE TRAFFIC SOURCES SHARING THE ONLY VIRTUAL CHANNEL
      //*******************************************************************
      
      Boolean isReactive() const
      { return m_reactive; }
      
      void setNumMessTypes(unsigned types)
      { 
        m_NumMessTypes = types; }
      
      unsigned getNumMessTypes() const
      { 
        return m_NumMessTypes; }
      
      void setReactive()
      { m_reactive = true; }
      
      //******************************************************************
      //END OF ADDITION
      //******************************************************************
      
      void updateSizes()
      {   
         m_SizeX = getSimulation().getNetwork()->getSizeX();
         m_SizeY = getSimulation().getNetwork()->getSizeY();
         m_SizeZ = getSimulation().getNetwork()->getSizeZ();
         m_TotalNodes = m_SizeX*m_SizeY*m_SizeZ;
      }

      static TPZTrafficPattern* createTrafficPattern(const TPZTag& tag,
                                                     TPZSimulation& simul);

      static TPZTrafficPattern* createTrafficPattern(const TPZString& fileName,
                                                     TPZSimulation& simul);

      // Run time information
      DEFINE_RTTI(TPZTrafficPattern);

   protected:
      TPZMessage* generateBasicMessage(const TPZPosition& source);
   
      virtual void initialize()
      { }
      
      TPZSimulation& getSimulation() const
      { return m_Simulation; }
            
      void setFromFile(Boolean val=true)
      { m_IsFile = val; }
      
      unsigned getX() const
      { return m_SizeX; }

      unsigned getY() const
      { return m_SizeY; }

      unsigned getZ() const
      { return m_SizeZ; }

      unsigned numberOfNodes() const
      { return m_TotalNodes; }
      
   private:
      double   m_Parameter1, m_Parameter2, m_Parameter3;
      double   m_ProbBimodal;
      unsigned m_NumMsgBimodal;  
      double   m_ProbMulticast;
      unsigned m_LengthMulticast;
      Boolean  m_Multicast;
      unsigned m_SizeX;
      unsigned m_SizeY;
      unsigned m_SizeZ;
      unsigned m_TotalNodes;
      Boolean  m_Bimodal;
      Boolean  m_IsFile;
      TPZSimulation& m_Simulation;
      
      Boolean  m_reactive;
      unsigned m_NumMessTypes;
   };

//************************************************************************

   class TPZTrafficPatternEmpty : public TPZTrafficPattern
   {
      typedef TPZTrafficPattern Inhereited;
   public:
      TPZTrafficPatternEmpty(TPZSimulation& s) : TPZTrafficPattern(s) { }
      virtual Boolean injectMessage(const TPZPosition& src=TPZPosition(0,0,0));
      virtual Boolean isEmpty() const
      { return true; }
      virtual TPZString asString() const 
      { return Inhereited::asString() + "Empty"; }

      DEFINE_RTTI(TPZTrafficPatternEmpty);
   
   };

//************************************************************************

   class TPZTrafficPatternRandom : public TPZTrafficPattern
   {
      typedef TPZTrafficPattern Inhereited;
   public:
      TPZTrafficPatternRandom(TPZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const TPZPosition& src=TPZPosition(0,0,0));
      virtual TPZString asString() const 
      { return Inhereited::asString() + "Random"; }
      DEFINE_RTTI(TPZTrafficPatternRandom);
   
   };

//************************************************************************

   class TPZTrafficPatternRandom3D : public TPZTrafficPatternRandom
   {
      typedef TPZTrafficPatternRandom Inhereited;
   public:
      TPZTrafficPatternRandom3D(TPZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const TPZPosition& src=TPZPosition(0,0,0));
      DEFINE_RTTI(TPZTrafficPatternRandom3D);   
   };

//************************************************************************

   class TPZTrafficPatternPermutation : public TPZTrafficPattern
   {
      typedef TPZTrafficPattern Inhereited;
   public:
      TPZTrafficPatternPermutation(TPZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const TPZPosition& src=TPZPosition(0,0,0));
      virtual TPZString asString() const 
      { return Inhereited::asString() + "Transpose matrix"; }
      DEFINE_RTTI(TPZTrafficPatternPermutation);
   
   };
//************************************************************************

   class TPZTrafficPatternPermutation3D : public TPZTrafficPatternPermutation
   {
      typedef TPZTrafficPatternPermutation Inhereited;
   public:
      TPZTrafficPatternPermutation3D(TPZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const TPZPosition& src=TPZPosition(0,0,0));
      DEFINE_RTTI(TPZTrafficPatternPermutation3D);
   };

//************************************************************************

   class TPZTrafficPatternPerfectShuffle : public TPZTrafficPattern
   {
      typedef TPZTrafficPattern Inhereited;
   public:
      TPZTrafficPatternPerfectShuffle(TPZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const TPZPosition& src=TPZPosition(0,0,0));
      virtual TPZString asString() const
      { return Inhereited::asString() + "Perfect Shuffle"; }
      DEFINE_RTTI(TPZTrafficPatternPerfectShuffle);
   
   };

//************************************************************************

   class TPZTrafficPatternPerfectShuffle3D : public TPZTrafficPatternPerfectShuffle
   {
      typedef TPZTrafficPatternPerfectShuffle Inhereited;
   public:
      TPZTrafficPatternPerfectShuffle3D(TPZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const TPZPosition& src=TPZPosition(0,0,0));
      DEFINE_RTTI(TPZTrafficPatternPerfectShuffle3D);
   
   };

//************************************************************************

   class TPZTrafficPatternBitReversal : public TPZTrafficPattern
   {
      typedef TPZTrafficPattern Inhereited;
   public:
      TPZTrafficPatternBitReversal(TPZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const TPZPosition& src=TPZPosition(0,0,0));
      virtual TPZString asString() const
      { return Inhereited::asString() + "Bit reversal"; }
      DEFINE_RTTI(TPZTrafficPatternBitReversal);
   
   };
//************************************************************************

   class TPZTrafficPatternBitReversal3D : public TPZTrafficPatternBitReversal
   {
      typedef TPZTrafficPatternBitReversal Inhereited;
   public:
      TPZTrafficPatternBitReversal3D(TPZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const TPZPosition& src=TPZPosition(0,0,0));
      DEFINE_RTTI(TPZTrafficPatternBitReversal3D);   
   };
//************************************************************************

   class TPZTrafficPatternTornado : public TPZTrafficPattern
   {
      typedef TPZTrafficPattern Inhereited;
   public:
      TPZTrafficPatternTornado(TPZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const TPZPosition& src=TPZPosition(0,0,0));
      virtual TPZString asString() const
      { return Inhereited::asString() + "Tornado traffic for k-ary n-cubes"; }
      DEFINE_RTTI(TPZTrafficPatternTornado);
   };
//************************************************************************

   class TPZTrafficPatternTornado3D : public TPZTrafficPatternTornado
   {
      typedef TPZTrafficPatternTornado Inhereited;
   public:
      TPZTrafficPatternTornado3D(TPZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const TPZPosition& src=TPZPosition(0,0,0));
      DEFINE_RTTI(TPZTrafficPatternTornado3D);
   };    
//************************************************************************

   class TPZTrafficPatternLocal : public TPZTrafficPattern
   {
      typedef TPZTrafficPattern Inhereited;   
   public:
      TPZTrafficPatternLocal(TPZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const TPZPosition& src=TPZPosition(0,0,0));
      virtual TPZString asString() const 
      { return Inhereited::asString() + "Local"; }
      DEFINE_RTTI(TPZTrafficPatternLocal);
   
   };

//************************************************************************

   class TPZTrafficPatternLocal3D : public TPZTrafficPatternLocal
   {
      typedef TPZTrafficPatternLocal Inhereited;   
   public:
      TPZTrafficPatternLocal3D(TPZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const TPZPosition& src=TPZPosition(0,0,0));
      DEFINE_RTTI(TPZTrafficPatternLocal);
   
   };

//************************************************************************

   class TPZTrafficPatternHotSpot : public TPZTrafficPattern
   {
      typedef TPZTrafficPattern Inhereited;
      typedef TPZArray<Boolean> THotSpotList;
   public:
      TPZTrafficPatternHotSpot(TPZSimulation& s) : TPZTrafficPattern(s), 
                                                   m_HotSpotList(0)
      { }
      virtual ~TPZTrafficPatternHotSpot() 
      { delete m_HotSpotList; }
      
      virtual Boolean injectMessage(const TPZPosition& src=TPZPosition(0,0,0));
      virtual double  getInjectFactor(const TPZPosition& pos);

      virtual TPZString asString() const;
      DEFINE_RTTI(TPZTrafficPatternHotSpot);
   
   protected:
      virtual void initialize();

   private:
      THotSpotList* m_HotSpotList;
      
   };

//************************************************************************

   class TPZTrafficPatternFile : public TPZTrafficPattern
   {
      typedef TPZTrafficPattern Inhereited;   
   public:
      TPZTrafficPatternFile(const TPZString& fileName, TPZSimulation& s);
      virtual Boolean injectMessage(const TPZPosition& src=TPZPosition(0,0,0));
      virtual TPZString asString() const 
      { return TPZString("File=") + m_FileName; }
      DEFINE_RTTI(TPZTrafficPatternFile);
   private:
      Boolean readNextMessage();
      TPZMessage m_Message;
      TPZString  m_FileName;
      ifstream*  m_File;
      Boolean    m_eof;
      Boolean    m_first;
   };

      
//************************************************************************


#endif


// end of file

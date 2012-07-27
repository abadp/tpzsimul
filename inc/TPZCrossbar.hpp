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
//    File: TPZCrossbar.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZCrossbar_HPP__
#define __TPZCrossbar_HPP__

//************************************************************************

   #include <TPZRunnableComponent.hpp>

   #ifndef __TPZFlow_HPP__
   #include <TPZFlow.hpp>
   #endif

   #ifndef __TPZArray_HPP__
   #include <TPZArray.hpp>
   #endif

//************************************************************************

   class TPZRouterBuilder;

//************************************************************************

   class TPZCrossbar : public TPZRunnableComponent
   {
      friend class TPZRouterBuilder;
      
      //Begin *********************************************** nested class
      //End ************************************************* nested class

      typedef TPZRunnableComponent Inhereited;
      
      typedef enum { WH,
                     WH_RTG,
		     VC,
		     VC_OPT,
		     VC_MUX,
		     VC_MUX_OPT,
		     VC_TORUS,
		     VC_TORUS_MUX,
		     CT, 
                     CT_MUX,
                     MUX_ADAPT} CrossbarType;

   public:
      TPZCrossbar( const TPZComponentId& id, 
                   unsigned inputs,
                   unsigned outputs );
                   
      virtual ~TPZCrossbar();

      virtual TPZString asString() const;      
      
      CrossbarType getCrossbarType()
      { return m_CrossbarType; }
      
      unsigned getInputWith(TPZROUTINGTYPE type, unsigned channel);
      unsigned getOutputWith(TPZROUTINGTYPE type, unsigned channel);
      TPZROUTINGTYPE getTypeForOutput(unsigned output, unsigned channel);
      TPZROUTINGTYPE getTypeForInput(unsigned input, unsigned channel);
      Boolean  isLocalNodeOutput(unsigned oPort);
      
      unsigned getOutputMux() const;      
      void setOutputMux(unsigned mux);
      
      unsigned getNumberVC() const;      
      void setNumberVC(unsigned mux);
      
      unsigned getNumberMT() const;      
      void setNumberMT(unsigned mux);
      
      unsigned getRoutingDelay()
      { return m_routingDelay; }
      void setRoutingDelay(unsigned delay)
      { m_routingDelay=delay; }
      
      unsigned getVCArbitrationDelay()
      { return m_VCArbitrationDelay; }
      void setVCArbitrationDelay(unsigned delay)
      { m_VCArbitrationDelay=delay; }
      
      unsigned getSWArbitrationDelay()
      { return m_SWArbitrationDelay; }
      void setSWArbitrationDelay(unsigned delay)
      { m_SWArbitrationDelay=delay; }
      
      unsigned getSWTraversalDelay()
      { return m_SWTraversalDelay; }
      void setSWTraversalDelay(unsigned delay)
      { m_SWTraversalDelay=delay; }
      
      unsigned numberOfLocalPorts() const;
      void setNumberOfLocalPorts(unsigned number);

                  
      // Run time information
      DEFINE_RTTI(TPZCrossbar);
   
   protected:      
      virtual void buildFlowControl();
      virtual void buildOutputInterfaz(unsigned number);
      void setCrossbarType(CrossbarType type)
      { m_CrossbarType = type; }

   private:
      static TPZCrossbar* newFrom( const TPZTag* tag, 
                                   TPZComponent* owner,
                                   INDEX& index);
      CrossbarType m_CrossbarType;
      unsigned     m_routingDelay;
      unsigned     m_VCArbitrationDelay;
      unsigned     m_SWArbitrationDelay;
      unsigned     m_SWTraversalDelay;
   };

//*************************************************************************

#endif


// end of file

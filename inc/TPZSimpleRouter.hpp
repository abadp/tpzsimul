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
//    File: TPZSimpleRouter.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZSimpleRouter_HPP__
#define __TPZSimpleRouter_HPP__

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

   class TPZSimpleRouter : public TPZRunnableComponent
   {
      friend class TPZRouterBuilder;
      
      //Begin *********************************************** nested class
      //End ************************************************* nested class

      typedef TPZRunnableComponent Inhereited;
      typedef enum { NIL, 
                     MESH_DOR,
                     MESH_DOR_FAST,
		     TORUS_DOR,
		     TORUS_BLESS,
                     MESH_DOR_BINOC		     
		   } 
                     TPZRoutingControl;
      typedef enum { BC_NIL, BC_WH, BC_CT, BC_DAMQ } TPZBufferControl;


   public:
      TPZSimpleRouter( const TPZComponentId& id, 
                   unsigned inputs,
                   unsigned outputs,
	                TPZRoutingControl routingControl,
		             TPZBufferControl bufferControl, unsigned vnets);
                   
      virtual ~TPZSimpleRouter();
      virtual void terminate();

      virtual TPZString asString() const;      
      
      unsigned getBufferSize() const;
      unsigned getInputWith(TPZROUTINGTYPE type, unsigned channel=1);
      unsigned getOutputWith(TPZROUTINGTYPE type, unsigned channel=1);
      TPZROUTINGTYPE getTypeForOutput(unsigned output);
      unsigned getHDInputWith(TPZROUTINGTYPE type, unsigned channel=1, Boolean reverse=false);
      unsigned getHDOutputWith(TPZROUTINGTYPE type, unsigned channel=1, Boolean reverse=false);
      TPZROUTINGTYPE getTypeForHDOutput(unsigned output);
      
      Boolean  isLocalNodeOutput(unsigned oPort);
      Boolean  isLocalNodeHDOutput(unsigned oPort);
      
      TPZRoutingControl getRoutingControl() const
      { return m_RoutingControl; }
      TPZBufferControl getBufferControl() const
      { return m_BufferControl; }
      unsigned getVnets() const {return m_vnets;}
      
      unsigned numberOfLocalPorts() const;
      void setNumberOfLocalPorts(unsigned number);

      // Run time information
      DEFINE_RTTI(TPZSimpleRouter);
   
   protected:      
      virtual void buildFlowControl();
      virtual void buildOutputInterfaz(unsigned number);
      virtual void buildInputInterfaz(unsigned number);



   private:
      static TPZSimpleRouter* newFrom( const TPZTag* tag, 
                                   TPZComponent* owner,
                                   INDEX& index);

        TPZRoutingControl m_RoutingControl;
        TPZBufferControl m_BufferControl;
        unsigned m_vnets;
	
   };

//*************************************************************************

#endif


// end of file

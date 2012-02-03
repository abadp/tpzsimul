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
//    File: TPZRoutingMuxed.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZRoutingMuxed_HPP__
#define __TPZRoutingMuxed_HPP__

//************************************************************************

   #include <TPZRunnableComponent.hpp>

//************************************************************************

   class TPZRouterBuilder;

//************************************************************************

   class TPZRoutingMuxed : public TPZRunnableComponent
   {
      typedef TPZRunnableComponent Inhereited;
      friend class TPZRouterBuilder;

   public:
      
      typedef enum { NIL,
      		   MUXED_DOR,
		   MUXED_DOR_BUBBLE,
		   MUXED_ADAP_BUBBLE               
                } TPZRoutingMuxedControl;
                     
      TPZRoutingMuxed(const TPZComponentId& id,
            		    TPZRoutingMuxedControl control, 
                      unsigned inputs);
      virtual ~TPZRoutingMuxed();
      
      
      TPZRoutingMuxedControl getRoutingMuxedControl() const
      { return m_RoutingControl; }

      // Run time information
      DEFINE_RTTI(TPZRoutingMuxed);
      
      TPZString asString() const;
      
   protected:      
      virtual void buildFlowControl();

   private:
      static TPZRoutingMuxed* newFrom(const TPZTag* tag, TPZComponent* owner);
      TPZRoutingMuxedControl m_RoutingControl;
      
   };

//*************************************************************************

#endif


// end of file

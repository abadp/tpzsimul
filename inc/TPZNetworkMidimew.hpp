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
//    File: TPZNetworkMidimew.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZNetworkMidimew_HPP__
#define __TPZNetworkMidimew_HPP__

//*************************************************************************

   #include <TPZNetwork.hpp>

//*************************************************************************

   class TPZNetworkBuilder;

//*************************************************************************

   class TPZNetworkMidimew : public TPZNetwork
   {
      friend class TPZNetworkBuilder;
      
   public:
      TPZNetworkMidimew( const TPZComponentId& id,
                       const TPZString& routerId,
                       unsigned x, 
                       unsigned y, 
                       unsigned z=1 );
      ~TPZNetworkMidimew();

      virtual void initialize();
      unsigned getDiameter() const;
      virtual TPZString asString() const;
      virtual TPZPosition CreatePosition(int num);
      virtual unsigned long long setMulticastMask(const TPZPosition&, const TPZROUTINGTYPE&)
      {return 0;}
      virtual unsigned distance(const TPZPosition& src, const TPZPosition& dst);
      virtual void     routingRecord( const TPZPosition& src,
                                      const TPZPosition& dst,
                                      int&  deltaX,
                                      int&  deltaY,
                                      int&  deltaZ,
                                      Boolean ordered=false  );
          
      // Run time information
      DEFINE_RTTI(TPZNetworkMidimew);
      
   protected:
      virtual void initializeConnectionsFor(const TPZPosition& pos);
   private:
      static TPZNetworkMidimew* newFrom(const TPZTag* tag, TPZComponent* owner);
      void   helperRR( int  origen, int destino, int&  deltaX, int&  deltaY, int&  deltaZ,Boolean ordered ) const;

      
   };

//*************************************************************************


#endif


// end of file

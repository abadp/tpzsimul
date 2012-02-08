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
//    File: TPZPair.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZPair_HPP__
#define __TPZPair_HPP__

//*************************************************************************

   template <class Q, class T>
   class TPZPair
   {
   public:
      TPZPair( )
      { }

      TPZPair( const Q& name, const T& value ) :
             m_Left(name), m_Right(value) 
      { }

      TPZPair( const TPZPair<Q,T>& pair ) :
             m_Left(pair.left()), m_Right(pair.right())
      {  }

      TPZPair& operator=(const TPZPair<Q,T>& pair) { 
         m_Left = pair.left(); 
         m_Right= pair.right();
         return *this; 
      }

      Boolean operator==(const TPZPair<Q,T>& pair) const {
         return (m_Left==pair.left()) && (m_Right==pair.right());
      }
            
      Q left() const
      { return m_Left; }
      
      T right() const
      { return m_Right; }
      
      void setLeft(const Q& data)
      { m_Left = data; }

      void setRight(const T& data)
      { m_Right = data; }
   
   private:
      Q m_Left;
      T m_Right;
};

//*************************************************************************

#endif


// end of file

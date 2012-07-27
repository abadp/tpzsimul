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
//    File: TPZBuilder.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header



#ifndef __TPZBuilder_HPP__
#define __TPZBuilder_HPP__

//*************************************************************************

   #ifndef __TPZBase_HPP__
   #include <TPZBase.hpp>
   #endif
   
   #ifndef __TPZArray_HPP__
   #include <TPZArray.hpp>
   #endif
   

//*************************************************************************

   class TPZTag;
   class TPZComponent;

//*************************************************************************

   class TPZBuilder : public TPZBase
   {
      typedef TPZArray<TPZTag*> TPZTagArray;
      
   public:
      TPZBuilder(const TPZString& sgmlFile);
      virtual ~TPZBuilder();

      Boolean loadSgmlInformation();
      TPZTag* getTagWithIndex(INDEX index);
      INDEX   getTagIndex(const TPZString& tagId);
      INDEX   getTagIndexWithName(const TPZString& tagName);
      
      TPZComponent* createComponentWithId( const TPZString& name,
                                           TPZComponent* owner=0);
                                           
      virtual TPZComponent* parseComponentDefinition( const TPZTag* tag, 
                                                      TPZComponent* owner,
                                                      INDEX& index ) = 0;
      // Define RTTI
      DEFINE_RTTI(TPZBuilder);
      
      TPZString getName() const
      { return m_FileName; }
      TPZString showTagArray(const TPZString& name) const;

   private:
      void removeTagArray();
      
      ifstream*    m_SgmlFile;
      TPZTagArray* m_TagArray;
      TPZString    m_FileName;
   };

//****************************************************


#endif

// end of file

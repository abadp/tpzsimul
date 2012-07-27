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
//    File: TPZTag.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZTag_HPP__
#define __TPZTag_HPP__

//*************************************************************************

   #ifndef __TPZSequence_HPP__
   #include <TPZSequence.hpp>
   #endif

   #ifndef __TPZAttribute_HPP__
   #include <TPZAttribute.hpp>
   #endif

//*************************************************************************

   class TPZAttribute;

//*************************************************************************

class TPZTag  : public TPZBase
{
      typedef TPZSequence<TPZAttribute*> TPZAttributeList;

   public :
      TPZTag();
      TPZTag(const TPZString& name, const TPZString& id);
      TPZTag(const TPZTag& aTag);
      virtual ~TPZTag();

      unsigned numberOfAttributes() const;
      Boolean getAttributeValueWithName(const TPZString& name, TPZString& value) const;
      Boolean addAttribute(TPZAttribute* attr);
      TPZTag& operator=(const TPZTag& aTag);

      friend Boolean  operator>>(istream &aStream, TPZTag& aTag);
      friend ostream& operator<<(ostream &aStream, const TPZTag& aTag);

      TPZString tagName() const
      { return m_TagName; }

      TPZString tagId() const
      { return m_TagId; }

      Boolean isComment() const
      { return m_Comment; }

      static TPZTag* createTag(const TPZString& tagString);

      // Run time information
      DEFINE_RTTI(TPZTag);

   private :
      static TPZString      extractTagName(TPZString& tagString);
      static TPZString      extractTagId(TPZString& tagString);
      static TPZAttribute*  extractAttribute(TPZString& tagString);
      static Boolean        removeDecoration(TPZString& tagString);
      static Boolean        isStringValid(const TPZString& aTag);

      TPZString        m_TagName;
      TPZString        m_TagId;
      TPZAttributeList m_AttributeList;
      Boolean          m_Comment;
   };



#endif

//*************************************************************************

// end of file

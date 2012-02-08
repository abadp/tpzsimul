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
//    File: TPZTag.cpp
//
//    Class:  TPZTag
//
//    Inherited from:  TPZBase
// :
//*************************************************************************
//end of header



#include <TPZTag.hpp>

#ifndef __TPZAttribute_HPP__
#include <TPZAttribute.hpp>
#endif


//************************************************************************

   static const char  LEFT_DELIMITER        = '<';
   static const char  RIGHT_DELIMITER       = '>';
   static const char* TAG_COMMENT_DELIMITER = "!--";


//************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZTag,TPZBase);

//*************************************************************************
//:
//  f: inline TPZString tagName () const;
//  d:
//
//  f: inline TPZString Tágide () const;
//  d:
//
//  f: inline isComment Boolean () const;
//  d:
//: 
//*************************************************************************


//*************************************************************************
//:
//  f: TPZTag ();
// 
//  d:
//: 
//*************************************************************************

TPZTag :: TPZTag()
        : m_TagName(TPZString("")),
          m_TagId(TPZString("")),
          m_Comment(false)
{

}

//*************************************************************************
//:
//  f: TPZTag (TPZString const & name, const TPZString & id);
// 
//  d:
//: 
//*************************************************************************

TPZTag :: TPZTag(const TPZString& name, const TPZString& id)
        : m_TagName(name),
          m_TagId(id),
          m_Comment(false)
{

}

//*************************************************************************
//:
//  f: TPZTag (const TPZTag & ATAG);
// 
//  d:
//: 
//*************************************************************************

TPZTag :: TPZTag(const TPZTag& aTag)
        : m_TagName(aTag.tagName()),
          m_TagId(aTag.tagId()),
          m_Comment(aTag.isComment())
{

   TPZAttributeList* list = (TPZAttributeList*)&(aTag.m_AttributeList);
   TPZAttribute* attr;
   TPZAttributeList::Cursor cursor(*list);
   forCursor(cursor)
   {
      attr = new TPZAttribute(*(cursor.element()) );
      if(attr) m_AttributeList.addAsLast(attr);
   }

}


//*************************************************************************
//:
//  f: ~ TPZTag ();
// 
//  d:
//: 
//*************************************************************************

TPZTag :: ~TPZTag()
{
   TPZAttributeList::Cursor cursor(m_AttributeList);
   forCursor(cursor)
   {
      delete cursor.element();
   }

   m_AttributeList.removeAllElements();
}

//*************************************************************************
//:
//  f: numberOfAttributes unsigned () const;
// 
//  d: Default Constructor. Initialize the buffer to 0;
//: 
//*************************************************************************

unsigned TPZTag :: numberOfAttributes() const
{
   return m_AttributeList.numberOfElements();
}


//*************************************************************************
//:
//  f: Boolean getAttributeValueWithName (TPZString const & name,
//                                        TPZString & value) const;
// 
//  d:
//: 
//*************************************************************************

Boolean TPZTag :: getAttributeValueWithName(const TPZString& name, TPZString& value) const
{
   TPZAttributeList::Cursor cursor(m_AttributeList);
   TPZAttribute* attr;
   forCursor(cursor)
   {
       attr = cursor.element();
       if( attr->name() == name )
       {
          value = attr->value();
          return true;
       }
   }
   return false;
}

//*************************************************************************
//:
//  f: Boolean addAttribute (TPZAttribute * attr);
// 
//  d:
//: 
//*************************************************************************

Boolean TPZTag :: addAttribute(TPZAttribute* attr)
{
   if( (!attr) || m_AttributeList.containsElement(attr) )
      return false;
   m_AttributeList.addAsLast(attr);
   return true;
}


//*************************************************************************
//:
//  f: TPZTag * createTag (const TPZString & tagStr)
// 
//  d:
//: 
//*************************************************************************

TPZTag* TPZTag :: createTag(const TPZString& tagStr)
{
   TPZTag* tag = 0L;
   TPZString tagString = tagStr;
   if( isStringValid(tagString) )
   {
      Boolean comment = removeDecoration(tagString);
      if( comment )
      {
         tag = new TPZTag(TAG_COMMENT_DELIMITER,tagString);
         tag->m_Comment = true;
         return tag;
      }

      TPZString name = TPZTag::extractTagName(tagString);
      if( name == TPZString("") )
         return 0L;

      TPZString tagId = TPZTag::extractTagId(tagString);
      tag = new TPZTag(name,tagId);

      Boolean anadirAttr = true;
      do
      {
         anadirAttr = tag->addAttribute( extractAttribute(tagString) );
      }
      while(anadirAttr);
   }
   return tag;
}

//*************************************************************************
//:
//  f: TPZString extractTagName (TPZString & tagString)
// 
//  d:
//: 
//*************************************************************************

TPZString TPZTag :: extractTagName(TPZString& tagString)
{
   TPZString name = tagString.firstWord();
   tagString.removeFirstWord();
   return name;
}


//*************************************************************************
//:
//  f: TPZString extractTagId (TPZString & tagString)
// 
//  d:
//: 
//*************************************************************************

TPZString TPZTag :: extractTagId(TPZString& tagString)
{
   TPZString id = tagString.firstWord();
   TPZString value = TPZString("");
   if( id == TPZString("id") )
   {
      tagString.removeFirstWord();
      tagString.removeFirstWord();     // "="
      
      if( tagString[(unsigned)1] != '"' )
      {
         value = tagString.firstWord();
         tagString.removeFirstWord();
      }
      else
      {
         unsigned index = tagString.indexOf('"',2);
         value = tagString.subString(2,index-2);
         tagString = TPZString::stripBlanks(tagString.subString(index+1));
      }
   }
   return value;
}


//*************************************************************************
//:
//  f: TPZAttribute * extractAttribute (TPZString & tagString)
// 
//  d:
//: 
//*************************************************************************

TPZAttribute* TPZTag :: extractAttribute(TPZString& tagString)
{
   TPZAttribute* attr = 0L;
   TPZString id, value;

   id = tagString.firstWord();
   tagString.removeFirstWord();
   tagString.removeFirstWord();     // "="
   if( tagString[(unsigned)1] != '"' )
   {
      value = tagString.firstWord();
      tagString.removeFirstWord();
   }
   else
   {
      unsigned index = tagString.indexOf('"',2);
      value = tagString.subString(2,index-2);
      tagString = TPZString::stripBlanks(tagString.subString(index+1));
   }

   if( (id==TPZString("")) || (value==TPZString("")) )
      return 0L;

   return new TPZAttribute(id,value);
}


//*************************************************************************
//:
//  f: Boolean isStringValid (const TPZString & ATAG);
// 
//  d:
//: 
//*************************************************************************

Boolean TPZTag :: isStringValid(const TPZString& aTag)
{
   TPZString tempString = TPZString::stripBlanks(aTag);
   if( (tempString[(unsigned)1] == LEFT_DELIMITER) &&
       (tempString[tempString.size()] == RIGHT_DELIMITER) )
      return true;
   return false;
}


//*************************************************************************
//:
//  f: Boolean removeDecoration (TPZString & tagString)
// 
//  d:
//: 
//*************************************************************************

Boolean TPZTag :: removeDecoration(TPZString& tagString)
{
   tagString = TPZString::stripBlanks(tagString);
   if( tagString[(unsigned)1] == LEFT_DELIMITER )
      tagString.removeFirst();
   if( tagString[tagString.size()] == RIGHT_DELIMITER )
      tagString.removeLast();

   if( tagString.subString(1,3) == TAG_COMMENT_DELIMITER )
   {
      tagString = tagString.subString(4);
      return true;   // Is a comment
   }

   TPZString line;
   char currentCharacter, lastCharacter = 0x0;

   Boolean comillasAbiertas = false;

   for( int i=1; i<=tagString.size(); i++ )
   {
      currentCharacter = tagString[(unsigned)i];
      if( (currentCharacter == '=') && !comillasAbiertas )
      {
         if( lastCharacter != ' ')
            line += " = ";
         else
            line += "= ";
         lastCharacter = ' ';
      }

      else if( (lastCharacter != ' ') || (currentCharacter != ' ') || comillasAbiertas )
      {
         if( currentCharacter == '"' )
            comillasAbiertas = (comillasAbiertas) ? false : true;
         
         line += currentCharacter;
         lastCharacter = currentCharacter;
      }
   } // for

   // if there is an even number of quotes, add a few at the end.
   if( comillasAbiertas )
      line += '"';

   tagString = line;
   return false;   // The string is not a comment.
}


//*************************************************************************
//:
//  f: TPZTag & operator = (const TPZTag & ATAG);
// 
//  d:
//: 
//*************************************************************************

TPZTag& TPZTag :: operator=(const TPZTag& aTag)
{
   m_AttributeList.removeAllElements();

   TPZAttributeList* list = (TPZAttributeList*)&(aTag.m_AttributeList);
   TPZAttribute* attr;
   TPZAttributeList::Cursor cursor(*list);
   forCursor(cursor)
   {
      attr = new TPZAttribute(*(cursor.element()) );
      if(attr) m_AttributeList.addAsLast(attr);
   }
   m_TagName = aTag.tagName();
   m_TagId   = aTag.tagId();
   m_Comment = aTag.isComment();

   return *this;
}


//*************************************************************************
//:
//  f: friend Boolean operator>> (istream & astream, TPZTag & ATAG);
// 
//  d:
//: 
//*************************************************************************

Boolean operator >> (istream &aStream, TPZTag& aTag)
{
   TPZString line = TPZString::lineFrom(aStream);
   TPZTag* tempTag = TPZTag::createTag(line);

   if(!tempTag) return false;

   aTag = *tempTag;
   delete tempTag;

   return true;
}


//*************************************************************************
//:
//  f: friend ostream & operator <<(ostream & os, const TPZTag & ATAG);
// 
//  d:
//: 
//*************************************************************************

ostream& operator << (ostream &os, const TPZTag& aTag)
{
   os << "[tagName= " << aTag.tagName();

   if( aTag.tagId() != TPZString("") )
      os << ", tagId= " << aTag.tagId() << "]" << endl;
   else
      os << " ]" << endl;


   TPZTag::TPZAttributeList::Cursor cursor((TPZTag::TPZAttributeList&)aTag.m_AttributeList);
   forCursor(cursor)
   {
      os << "   " << *( cursor.element() ) << endl;
   }


   return os;
}

//************************************************************************


// end of file

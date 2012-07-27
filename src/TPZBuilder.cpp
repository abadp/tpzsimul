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
//    File: TPZBuilder.cpp
//
//    Class:  TPZBuilder
//
//    Inherited from:  TPZBase
// :
//*************************************************************************
//end of header


#include <TPZBuilder.hpp>

#ifndef __TPZTag_HPP__
#include <TPZTag.hpp>
#endif

#ifndef __TPZConst_H__
#include <TPZConst.hpp>
#endif

//*************************************************************************


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZBuilder,TPZBase);

//*************************************************************************
//:
//  f: TPZString getName () const;
//  d:
//
//  f: virtual TPZComponent * parseComponentDefinition (TPZTag const * tag,
//                                                     TPZComponent * owner,
//                                                     INDEX & index) = 0;
//  d:
//:
//*************************************************************************

//*************************************************************************
//:
//  f: TPZBuilder (const TPZString & sgmlFile)
//
//  d:
//:
//*************************************************************************

TPZBuilder :: TPZBuilder(const TPZString& sgmlFile)
            : m_FileName(sgmlFile),
              m_TagArray(0)
{
   m_SgmlFile = new ifstream(sgmlFile);
   if( ! m_SgmlFile->good() )
   {
      TPZString err;
      err.sprintf(ERR_TPZBLDR_001, (char*)sgmlFile);
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: ~ TPZBuilder ();
//
//  d:
//:
//*************************************************************************

TPZBuilder :: ~TPZBuilder()
{
   m_SgmlFile -> close();
   delete m_SgmlFile;
   removeTagArray();
}


//*************************************************************************
//:
//  f: Boolean loadSgmlInformation ();
//
//  d:
//:
//*************************************************************************

Boolean TPZBuilder :: loadSgmlInformation()
{
   TPZString linea;
   unsigned numeroLinea = 0;
   TPZSequence<TPZTag*> tagList;

   while(1)
   {
      linea = TPZString::lineFrom(*m_SgmlFile);
      linea = TPZString::stripBlanks(linea);

      numeroLinea++;
      if( m_SgmlFile->eof() ) break;
      if( linea != TPZString("") )
      {
         TPZTag* tag = TPZTag::createTag(linea);
         if(!tag)
         {
            TPZString text = TPZString("Error in line ") + 
                             TPZString(numeroLinea);
            text += TPZString(". File ") + getName();
            text += TPZString("wrong definition of Tag :\n") + linea;
            THROW_BERROR(text);
         }

         if( ! tag->isComment() )   // Comments are not saved
         {
            tagList.addAsLast(tag);
         }
      }
   } // while

   removeTagArray();

   m_TagArray = new TPZTagArray(tagList.numberOfElements());
   m_TagArray->initialize(0);
   TPZCursor<TPZTag*> cursor(tagList);

   INDEX j = 1;
   forCursor(cursor)
   {
      m_TagArray->setValueAt(j++,cursor.element());
   }

   return true;
}


//*************************************************************************
//:
//  f: TPZTag * getTagWithIndex (index INDEX)
//
//  d:
//:
//*************************************************************************

TPZTag* TPZBuilder :: getTagWithIndex(INDEX index)
{
   if( index && m_TagArray  )
   {
      TPZTag* tempTag;
      if( m_TagArray->valueAt(index,tempTag) )
      {
         return tempTag;
      }
   }
   return 0;
}


//*************************************************************************
//:
//  f: INDEX getTagIndex (const TPZString & Tágide)
//
//  d:
//:
//*************************************************************************

INDEX TPZBuilder :: getTagIndex(const TPZString& tagId)
{
   if( m_TagArray )
   {
      TPZTag* tag;
      for(INDEX i=1; i<=m_TagArray->numberOfElements(); i++ )
      {
         m_TagArray->valueAt(i,tag);
         if( tag->tagId() == tagId )
            return i;
      }
   }
   // If the tag with that name is not found, 
   // returns a 0. Remember that the index starts at 1.
   return 0;
}


//*************************************************************************
//:
//  f: INDEX getTagIndexWithName (const TPZString & tagName);
//
//  d:
//:
//*************************************************************************

INDEX TPZBuilder :: getTagIndexWithName(const TPZString& tagName)
{
   if( m_TagArray )
   {
      TPZTag* tag;
      for(INDEX i=1; i<=m_TagArray->numberOfElements(); i++ )
      {
         m_TagArray->valueAt(i,tag);
         if( tag->tagName() == tagName )
            return i;
      }
   }
   // If the tag with that name is not found, 
   // returns a 0. Remember that the index starts at 1.
   return 0;
}

//*************************************************************************
//:
//  f: TPZComponent * createComponentWithId (const TPZString & id,
//                                         TPZComponent * owner);
//
//  d:
//:
//*************************************************************************

TPZComponent* TPZBuilder :: createComponentWithId( const TPZString& id,
                                                   TPZComponent* owner )
{
   INDEX indice = getTagIndex(id);
   TPZTag* tag = getTagWithIndex(indice);
   if( ! tag )
   {
      TPZString err;
      tag = getTagWithIndex(1);
      err.sprintf(ERR_TPZBLDR_002, (char*)id );
      err=err+TPZString("(cont...)\n Help=     Avaliable ")+tag->tagName()+TPZString(" (")+showTagArray(tag->tagName())+TPZString(" )");
      EXIT_PROGRAM(err);
   }
   TPZComponent* rComponent = parseComponentDefinition(tag,owner,indice);
   if( ! rComponent )
   {
      tag = getTagWithIndex(1);
      TPZString err;
      err.sprintf(ERR_TPZBLDR_002, (char*)id );
      err=err+TPZString("(cont...)\n Help=     Avaliable ")+tag->tagName()+TPZString(" (")+showTagArray(tag->tagName())+TPZString(" )");
      EXIT_PROGRAM(err);
      
   }
   return rComponent;
}


//*************************************************************************
//:
//  f: void removeTagArray ();
//
//  d:
//:
//*************************************************************************

void TPZBuilder :: removeTagArray()
{
   if( m_TagArray )
   {
      TPZTag* tag;
      for(INDEX i=1; i<=m_TagArray->numberOfElements(); i++ )
      {
         if( m_TagArray->valueAt(i,tag) )
            delete tag;
      }
      delete m_TagArray;
      m_TagArray = 0;
   }
}
//*************************************************************************
//:
//  f: void removeTagArray ();
//
//  d:
//:
//*************************************************************************

TPZString TPZBuilder :: showTagArray(const TPZString& name) const
{
   TPZString out = "";
   if( m_TagArray )
   {
      TPZTag* tag;
      for(INDEX i=1; i<=m_TagArray->numberOfElements(); i++ )
      {
         if( m_TagArray->valueAt(i,tag) && tag->tagName() == name)
            out=out+tag->tagId()+TPZString(" , ");
      }
   }
   return out;
}

//*************************************************************************


// end of file

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
//    File: TPZBaseCollection.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header



#ifndef __TPZBaseCollection_HPP__
#define __TPZBaseCollection_HPP__

//*************************************************************************

   #ifndef __TPZBase_HPP__
   #include <TPZBase.hpp>
   #endif

//*************************************************************************

   template <class T> class TPZCursor;

//*************************************************************************

   template<class T>
   class TPZNode
   {
   public:

      TPZNode(const T& data, TPZNode<T>* next = 0L, TPZNode<T>* back = 0L) :
              m_Element(data),
              m_pNext(next),
              m_pBack(back)
      {  }

      TPZNode<T>* getNext() const
      { return m_pNext; }

      TPZNode<T>* getBack() const
      { return m_pBack; }

      void setNext(TPZNode<T>* next)
      { m_pNext=next; }

      void setBack(TPZNode<T>* back)
      { m_pBack=back; }

      T element() const
      { return m_Element; }

   private:
      T    m_Element;
      TPZNode<T>* m_pNext;
      TPZNode<T>* m_pBack;
   };

//*************************************************************************


   template <class T>
   class TPZBaseCollection : public TPZBase
   {
      friend class TPZCursor<T>;

   public:

      typedef TPZCursor<T> Cursor;

      virtual  ~TPZBaseCollection();
      void     removeAllElements();
      Boolean  allElementsDo(Boolean (*function)(T& data) );
      Boolean  containsElement(const T& element);
      Boolean  elementAt(unsigned index, T& element) const;
      Boolean  removeAt(unsigned index) ;
      Boolean  insertAt(const T& data, unsigned index);

      unsigned numberOfElements() const
      { return m_NumberElements; }

      Boolean isValid() const
      { return (m_NumberElements ? true : false); }

      unsigned getUserInformation() const { return m_UserInformation; }
      void setUserInformation(unsigned info) { m_UserInformation=info; }
      
   protected:
      TPZBaseCollection();
      Boolean insertBefore(const T& data, TPZNode<T>* node);
      Boolean insertAfter(const T& data, TPZNode<T>* node);
      Boolean remove(TPZNode<T>* node);

      TPZNode<T>* getRoot() const
      { return m_pRoot; }

      TPZNode<T>* getTail() const
      { return m_pTail; }

      void setRoot(TPZNode<T>* newRoot)
      { m_pRoot = newRoot; }

   private:
      TPZNode<T>*       m_pRoot;
      TPZNode<T>*       m_pTail;
      unsigned          m_NumberElements;
      unsigned          m_UserInformation;
   };

//*************************************************************************

   template <class T>
   class TPZCursor
   {
   public:
      TPZCursor(const TPZBaseCollection<T>& coll);
      void setToFirst();
      void setToNext();
      void setToLast();
      Boolean isValid() const;
      T element() const;
      Boolean remove();
      Boolean addAfter(const T& data);
      Boolean addBefore(const T& data);

   private:
      void invalidate();

      TPZBaseCollection<T>& m_Collection;
      TPZNode<T>*           m_pCurrent;
      Boolean               m_Valid;
   };

//*************************************************************************

   #define forCursor(x) for(x.setToFirst();x.isValid();x.setToNext())

//*************************************************************************

   #include <TPZBaseCollection.inl>

//*************************************************************************

#endif


// end of file

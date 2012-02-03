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
//    File: TPZBaseCollection.inl
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


//*************************************************************************
//
//   TPZBaseCollection ()
//
//*************************************************************************

template <class T>
TPZBaseCollection<T> :: TPZBaseCollection()
                      : m_pRoot(0L),
	                     m_pTail(0L),
                        m_NumberElements(0)
{

}


//*************************************************************************
//
//   ~ TPZBaseCollection ()
//
//*************************************************************************

template <class T>
TPZBaseCollection<T> :: ~TPZBaseCollection()
{
//   removeAllElements ();
}


//*************************************************************************
//
//   removeAllElements void ()
//
//*************************************************************************

template <class T>
void TPZBaseCollection<T> :: removeAllElements()
{
   if( isValid() )
   {
      TPZNode<T>* tempRoot = m_pRoot;
      while(tempRoot)
      {
         m_pRoot = m_pRoot->getNext();
         delete tempRoot;
         tempRoot = m_pRoot;
      }
      m_NumberElements=0;
      m_pTail = 0L;
   }
}


//*************************************************************************
//
//   Boolean elementAt (unsigned index, T & element) const;
//
//*************************************************************************

template <class T>
Boolean TPZBaseCollection<T> :: elementAt(unsigned index, T& element) const
{
   // The possible values ​​of "index" are between 0 and NumberOfElements () -1
   if( index<numberOfElements() )
   {
      TPZNode<T>* tempNode = m_pRoot;
      unsigned tempIndex = index;
      while(tempIndex)
      {
         tempNode = tempNode->getNext();
         if(!tempNode) return false;
         tempIndex--;
      }
      element = tempNode->element();
      return true;
   }
   return false;
}
//*************************************************************************
//
//      Boolean RemoveAt (unsigned index);
//
//*************************************************************************

template <class T>
Boolean TPZBaseCollection<T> :: removeAt(unsigned index)
{
   // The possible values ​​of "index" are between 0 and NumberOfElements () -1
   if( index<numberOfElements() )
   {
      TPZNode<T>* tempNode = m_pRoot;
      unsigned tempIndex = index;
      while(tempIndex)
      {
         tempNode = tempNode->getNext();
         if(!tempNode) return false;
         tempIndex--;
      }
      remove(tempNode);
      return true;
   }
   return false;
}
//*************************************************************************
//
//      Boolean RemoveAt (unsigned index);
//
//*************************************************************************

template <class T>
Boolean TPZBaseCollection<T> :: insertAt(const T& data, unsigned index)
{
   // The possible values ​​of "index" are between 0 and NumberOfElements () -1
   if( index<numberOfElements() )
   {
      TPZNode<T>* tempNode = m_pRoot;
      unsigned tempIndex = index;
      while(tempIndex)
      {
         tempNode = tempNode->getNext();
         if(!tempNode) return false;
         tempIndex--;
      }
      insertAfter(data,tempNode);
      return true;
   }
   return false;
}

//*************************************************************************
//
//   AllElementsDo Boolean (Boolean (* function) (T & data))
//
//*************************************************************************

template <class T>
Boolean TPZBaseCollection<T> :: allElementsDo(Boolean (*function)(T& data) )
{
   typename TPZBaseCollection<T>::Cursor cursor(*this);
   forCursor(cursor)
   {
      if( (*function)((T&)cursor.element()) == 0)
         return false;
   }
   return true;
}


//*************************************************************************
//
//   Boolean containsElement (const T & data element)
//
//*************************************************************************

template <class T>
Boolean TPZBaseCollection<T> :: containsElement(const T& dataElement)
{
   typename TPZBaseCollection<T>::Cursor cursor(*this);
   forCursor(cursor)
   {
      if( cursor.element() == dataElement )
         return true;
   }
   return false;
}


//*************************************************************************
//
//   Boolean insertBefore (const T & data, TPZNode <T> * Node)
//
//*************************************************************************

template <class T>
Boolean TPZBaseCollection<T> :: insertBefore(const T& data, TPZNode<T>* node)
{
   TPZNode<T>* newNode = new TPZNode<T>(data);
   if(!newNode) return false;
   if(!node)
   {
      // In this case, added to the beginning of the collection
      if(m_pRoot)
      {
         m_pRoot->setBack(newNode);
         newNode->setNext(m_pRoot);
         newNode->setBack(0);
      }
      else
      {
         // As there was no root, pTail also points to the last item.
         m_pTail = newNode;
      }
      m_pRoot = newNode;
   }
   else
   {
      TPZNode<T>* left = node->getBack();
      if(!left) // node == m_pRoot
         m_pRoot = newNode;
      newNode->setNext(node);
      newNode->setBack(left);
      if(left)
         left->setNext(newNode);
      node->setBack(newNode);
   }
   m_NumberElements++;
   return true;
}


//*************************************************************************
//
//   Boolean InsertAfter (const T & data, TPZNode <T> * Node)
//
//*************************************************************************

template <class T>
Boolean TPZBaseCollection<T> :: insertAfter(const T& data, TPZNode<T>* node)
{
   TPZNode<T>* newNode = new TPZNode<T>(data);
   if(!newNode) return false;
   if(!node)
   {
      // In this case, added to the beginning of the collection
      if(m_pRoot)
      {
         m_pRoot->setBack(newNode);
         newNode->setNext(m_pRoot);
         newNode->setBack(0);
      }
      else
      {
         // As there was no root, pTail also points to the last item.
         m_pTail = newNode;
      }
      m_pRoot = newNode;
   }
   else
   {
      TPZNode<T>* right = node->getNext();
      newNode->setNext(right);
      newNode->setBack(node);
      if(right)
         right->setBack(newNode);
      else
         m_pTail = newNode;
      node->setNext(newNode);
      
   }
   m_NumberElements++;
   return true;
}


//*************************************************************************
//
//   Boolean remove (TPZNode <T> * Node)
//
//*************************************************************************

template <class T>
Boolean TPZBaseCollection<T> :: remove(TPZNode<T>* node)
{
   if(node)
   {
   
      if( node == m_pRoot )
      {
         m_pRoot = node->getNext();
         if(m_pRoot)
            m_pRoot->setBack(0);
      }
      else
      {
         TPZNode<T>* left  = node->getBack();
         TPZNode<T>* right = node->getNext();
         if(left)  left->setNext(right);
         if(right) right->setBack(left);
      }
      if( node == m_pTail )
      {
         m_pTail = node->getBack();
      }
      
      
      delete node;
      m_NumberElements--;
      if( m_NumberElements && !getRoot() )
      {
         EXIT_PROGRAM("Error referencial en la Collection");
      }
      return true;
   }
   return false;
}


//*************************************************************************
//*************************************************************************
//
//   TPZCursor (const TPZBaseCollection <T> & Coll)
//
//*************************************************************************

template <class T>
TPZCursor<T> :: TPZCursor(const TPZBaseCollection<T>& coll)
              : m_Collection((TPZBaseCollection<T>&)coll),
                m_pCurrent(0L),
                m_Valid(false)
{

}


//*************************************************************************
//
//   setToFirst void ()
//
//*************************************************************************

template <class T>
void TPZCursor<T> :: setToFirst()
{
   m_pCurrent = m_Collection.getRoot();
   if(m_pCurrent) m_Valid = true;
   else           m_Valid = false;
}


//*************************************************************************
//
//   setToNext void ()
//
//*************************************************************************

template <class T>
void TPZCursor<T> :: setToNext()
{
   if( isValid() ) m_pCurrent = m_pCurrent->getNext();
   if(m_pCurrent) m_Valid = true;
   else           m_Valid = false;
}


//*************************************************************************
//
//   setToLast void ()
//
//*************************************************************************

template <class T>
void TPZCursor<T> :: setToLast()
{
   if( !isValid() )
   {
      m_pCurrent = m_Collection.getRoot();
      if(m_pCurrent) m_Valid = true;
   }

   if( isValid() )
   {
      while( m_pCurrent->getNext() )
         m_pCurrent = m_pCurrent->getNext();
   }
}


//*************************************************************************
//
//   Boolean isValid () const
//
//*************************************************************************

template <class T>
Boolean TPZCursor<T> :: isValid() const
{
   return m_Valid ;
}


//*************************************************************************
//
//   T element () const
//
//*************************************************************************

template <class T>
T TPZCursor<T> :: element() const
{
   return m_pCurrent->element();
}


//*************************************************************************
//
//   Boolean remove ()
//
//*************************************************************************

template <class T>
Boolean TPZCursor<T> :: remove()
{
   Boolean result = m_Collection.remove(m_pCurrent);
   invalidate();
   return result;
}


//*************************************************************************
//
//   Boolean addAfter (const T & data)
//
//*************************************************************************

template <class T>
Boolean TPZCursor<T> :: addAfter(const T& data)
{
   Boolean result = m_Collection.insertAfter(data,m_pCurrent);
   invalidate();
   return result;
}


//*************************************************************************
//
//   Boolean addBefore (const T & data)
//
//*************************************************************************

template <class T>
Boolean TPZCursor<T> :: addBefore(const T& data)
{
   Boolean result = m_Collection.insertBefore(data,m_pCurrent);
   invalidate();
   return result;
}


//*************************************************************************
//
//   void invalidate ()
//
//*************************************************************************

template <class T>
void TPZCursor<T> :: invalidate()
{
   m_pCurrent = 0L;
   m_Valid = false;
}

//*************************************************************************


// end of file

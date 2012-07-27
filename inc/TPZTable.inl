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
//    File: TPZTable.inl
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


template <class T>
TPZQueueTable<T> :: TPZQueueTable(unsigned size)
                  : m_Size(size),
                    m_Elements(0),
                    m_Array(0)
{
   m_Array = new T*[size];
   for( register int i=0; i<getSize(); i++ )
      m_Array[i] = 0;
}

//*************************************************************************

template <class T>
TPZQueueTable<T> :: TPZQueueTable()
                  : m_Size(1),
                    m_Elements(0),
                    m_Array(0)
{
   m_Array = new T*[1];
   for( register int i=0; i<getSize(); i++ )
      m_Array[i] = 0;
}


//*************************************************************************

template <class T>
TPZQueueTable<T> :: ~TPZQueueTable()
{
   for( register int i=0; i<getSize(); i++ )
      delete m_Array[i];
      
   delete[] m_Array;
}

//*************************************************************************

template <class T>
T& TPZQueueTable<T> :: element(unsigned index)
{
   if( ! indexValid(index) ) 
   {
      TPZString texto = "element() : Index out of range: ";
      texto += TPZString(index);
      EXIT_PROGRAM(texto);   
   }

   return (T&)(*(m_Array[index-1]));

}

//*************************************************************************

template <class T>
void TPZQueueTable<T> :: remove(unsigned index)
{
   if( ! indexValid(index) ) return;

   T* temp = m_Array[index-1];
   delete temp;
   *(m_Array+index-1) = 0;
   
   for( register int i=0, j=0; i<getSize(); i++ )
   {
      if( m_Array[i] )
      {
         m_Array[j] = m_Array[i];
         j++;
      }
   }
   
   m_Elements--;

}

//*************************************************************************

template <class T>
void TPZQueueTable<T> :: swap(unsigned index1, unsigned index2)
{
   if( index1 == index2 ) return;
   if( ! indexValid(index1) ) return;
   if( ! indexValid(index2) ) return;

   T* temp = m_Array[index1-1];
   m_Array[index1-1] = m_Array[index2-1];
   m_Array[index2-1] = temp;
}

//*************************************************************************

template <class T>
void TPZQueueTable<T> :: add(const T& data)
{
   if( m_Elements >= getSize() )
   {
      EXIT_PROGRAM("Queue table full");   
   }
   
   m_Array[m_Elements] = new T(data);
   
   m_Elements++;
}

//*************************************************************************

template <class T>
Boolean TPZQueueTable<T> :: indexValid(unsigned index)
{
   if( (!index) || (index > m_Elements) )
   {
      return false;
   }
   return true;
}

//*************************************************************************


// end of file

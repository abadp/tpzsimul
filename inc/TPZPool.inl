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
//    File: TPZPool.inl
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZFicheroLog_HPP__
#include <TPZFicheroLog.hpp>
#endif

template <class T>
TPZPool<T>::TPZPool(int size)
{
#ifndef NO_POOL
   assert(size>0);
   m_size=size;
   if((m_owned = new int[size]) == 0)
   {
      cerr << "Allocation error 1\n";
      exit(1);
   }
   if((m_pool = new T[size]) == 0)
   {
      cerr << "Allocation error 2\n";
      exit(1);
   }
   for(int i=0; i<m_size; i++)
      *(m_owned+i)=0;
   m_current=0;
   m_ocupation=0;
#endif
}

template <class T>
TPZPool<T>::~TPZPool()
{
#ifndef NO_POOL
   #ifndef NO_TRAZA
      TPZWRITE2LOG( TPZString("TPZPool: Removing ") + TPZString(m_ocupation) + " unreleased items\n" );
   #endif
   cerr << "TPZPool: Removing " << m_ocupation << " unreleased items\n";
   delete [] m_pool;
   delete [] m_owned;
#endif
}

template <class T>
inline T* TPZPool<T>::allocate()
{
#ifndef NO_POOL

   if(m_size-1 == m_ocupation) 
   { 
     return 0; //If Pool is full allocate a new element
   }
   do
   { 
      ++m_current%=m_size;
   }
   while(*(m_owned+m_current));
   (*(m_owned+m_current))=1;
   m_ocupation++;
   #ifndef NO_TRAZA
      TPZWRITE2LOG (TPZString ("TPZPool: Allocating ") + TPZString ((long int) (m_pool + m_current)));
    #endif
   int temp = m_current;
   return m_pool+temp;
#else
   return new T;
#endif
}

template <class T>
inline void TPZPool<T>::release(T* item)
{
#ifndef NO_POOL
   if(!item)
   { 
      #ifndef NO_TRAZA
         TPZWRITE2LOG( TPZString("TPZPool: Releasing null item"));
      #endif
      cerr << "TPZPool: Releasing null item\n";
   }
   if((*(m_owned+(item - m_pool)))==0) 
   {
      #ifndef NO_TRAZA
         TPZWRITE2LOG( TPZString("TPZPool: Releasing unallocated item ") + TPZString((long int)item));
      #endif
      cerr << "TPZPool: Releasing unallocated item\n";
   }
   //Used Clear element
   T clearTipe;
   *item=clearTipe;
   
   m_ocupation--;
   (*(m_owned+(item - m_pool)))=0;
   #ifndef NO_TRAZA
      TPZWRITE2LOG (TPZString ("TPZPool: Releasing ") + TPZString ((long int) item));
   #endif
#else
    delete  item;
#endif
}
//************************************************************************


// end of file

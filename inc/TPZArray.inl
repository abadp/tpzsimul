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
//    File: TPZArray.inl
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header



//*************************************************************************
//
//   TPZArray (unsigned size);
//
//*************************************************************************

template <class T>
TPZArray<T> :: TPZArray(unsigned size)
             : m_Size(size),
               m_Root(0)
{
   m_Root = new T[m_Size];
}


//*************************************************************************
//
//   ~ TPZArray (unsigned size);
//
//*************************************************************************

template <class T>
TPZArray<T> :: ~TPZArray()
{
   delete[] m_Root;
}


//*************************************************************************
//
//   Boolean valueAt (unsigned index, T & data) const;
//
//*************************************************************************

template <class T>
Boolean TPZArray<T> :: valueAt(unsigned index, T& data) const
{
   if( isLimitsOk(index) )
   {
      data = *(m_Root+index-1);
      return true;
   }
   return false;
}


//*************************************************************************
//
//   Boolean valueAt (unsigned index, T * data) const;
//
//*************************************************************************

template <class T>
Boolean TPZArray<T> :: valueAt(unsigned index, T* data) const
{
   if( isLimitsOk(index) )
   {
      *data = *(m_Root+index-1);
      return true;
   }
   return false;
}


//*************************************************************************
//
//   Boolean setValueAt (unsigned index, const T & data);
//
//*************************************************************************

template <class T>
Boolean TPZArray<T> :: setValueAt(unsigned index, const T& data)
{
   if( isLimitsOk(index) )
   {
      *(m_Root+index-1) = data;
      return true;
   }
   
   return false;
}


//*************************************************************************
//
//   Boolean initialize (const T & data);
//
//*************************************************************************

template <class T>
void TPZArray<T> :: initialize(const T& data)
{
   for(int i=0; i<m_Size; i++)
   {
      *(m_Root+i) = data;
   }
}

//*************************************************************************
//
//   Boolean isLimitsOk (unsigned index) const;
//
//*************************************************************************

template <class T>
Boolean TPZArray<T> :: isLimitsOk(unsigned index) const
{
   return (index>0 && index<=m_Size) ? true : false;
}

            
// end of file

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
//    File: TPZVector.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header

#ifndef __TPZVector_HPP__
#define __TPZVector_HPP__

template <class TYPE>
class TPZVector
{
public:
  TPZVector();
  explicit TPZVector(int initial_size);  // Constructor
  ~TPZVector();
  const TYPE& ref(int index) const;   // Get an element of the vector
  TYPE& ref(int index);               // Get an element of the vector
  void clear();                       // remove all elements of the vector
  int size() const { return m_size; } 
  void setSize(int new_size);         // Increase size, reallocates memory as needed
  void print(ostream& out) const;

  // Array Reference operator overloading
  const TYPE& operator[](int index) const { return ref(index); }  
  TYPE& operator[](int index) { return ref(index); }

  // Public copy constructor and assignment operator
  TPZVector(const TPZVector& vec);  
  TPZVector<TYPE>& operator=(const TPZVector& vec);
private:

  void grow(int new_max_size);  // Expands vector to new_max_size
  
  // Data members
  TYPE* m_vec;           // Array to hold the elements
  int m_size;            // Number of elements in use
  int m_max_size;        // Size of allocated array
};

template <class TYPE> 
ostream& operator<<(ostream& out, const TPZVector<TYPE>& vec);

// *********************

template <class TYPE>
TPZVector<TYPE>::TPZVector()
{ 
  m_size = 0; 
  m_max_size = 0;
  m_vec = NULL;
}

template <class TYPE>
TPZVector<TYPE>::TPZVector(int initial_size)
{ 
  m_size = 0; 
  m_max_size = initial_size;
  m_vec = NULL;
  grow(initial_size);
}

template <class TYPE>
TPZVector<TYPE>::~TPZVector()
{ 
  delete [] m_vec;  
}

template <class TYPE> 
const TYPE& TPZVector<TYPE>::ref(int index) const
{
  return m_vec[index];
}

template <class TYPE> 
TYPE& TPZVector<TYPE>::ref(int index)
{
  return m_vec[index];
}


template <class TYPE> 
void TPZVector<TYPE>::setSize(int new_size)
{
  if (new_size > m_max_size) {
    grow(max((m_max_size+1)*2, new_size));
  }
  m_size = new_size;
}

template <class TYPE> 
inline
void TPZVector<TYPE>::clear()
{
  m_size = 0;
  m_max_size = 0;
  delete [] m_vec;
  m_vec = NULL;
}

// Copy constructor
template <class TYPE> 
TPZVector<TYPE>::TPZVector(const TPZVector& vec)
{ 
  // Setup the new memory
  m_size = vec.m_size;
  m_max_size = vec.m_max_size;
  if (m_max_size != 0) {
    m_vec = new TYPE[m_max_size];
    assert(m_vec != (int)NULL);
  } else { 
    m_vec = (int)NULL;
  }
  
  // Copy the elements of the array
  for(int i=0; i<m_size; i++) {
    m_vec[i] = vec.m_vec[i];  // Element copy
  }
}

template <class TYPE> 
TPZVector<TYPE>& TPZVector<TYPE>::operator=(const TPZVector& vec)
{
  if (this == &vec) {
    //    assert(0);
  } else {
    // Free the old memory
    delete [] m_vec;

    // Setup the new memory
    m_size = vec.m_size;
    m_max_size = vec.m_max_size;

    if (m_max_size != 0) {
      m_vec = new TYPE[m_max_size];
      assert(m_vec != (int)NULL);
    } else {
      m_vec = (int)NULL;
    }

    // Copy the elements of the array
    for(int i=0; i<m_size; i++) {
      m_vec[i] = vec.m_vec[i];  // Element copy
    }
  }
  return *this;
}

template <class TYPE> 
void TPZVector<TYPE>::grow(int new_max_size)
{
  TYPE* temp_vec;
  m_max_size = new_max_size;
  if (new_max_size != 0) {
    temp_vec = new TYPE[new_max_size];
    assert(temp_vec != (int)NULL);
  } else {
    temp_vec = (int)NULL;
  }

  // Copy the elements of the array
  for(int i=0; i<m_size; i++) {
    temp_vec[i] = m_vec[i];  // Element copy
  }
  delete [] m_vec;
  m_vec = temp_vec;
}

template <class TYPE> 
ostream& operator<<(ostream& out, const TPZVector<TYPE>& vec)
{
  vec.print(out);
  return out;
}

#endif //VECTOR_H

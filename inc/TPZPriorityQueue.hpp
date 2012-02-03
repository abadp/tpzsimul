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
//    File: TPZPriorityQueue.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZPriorityQueue_HPP__
#define __TPZPriorityQueue_HPP__

//************************************************************************

   #ifndef __TPZBaseCollection_HPP__
   #include <TPZBaseCollection.hpp>
   #endif

   #ifndef __TPZNotifier_HPP__
   #include <TPZNotifier.hpp>
   #endif

//************************************************************************

template<class T>
class TPZTimeStamp
{
public:
   TPZTimeStamp() : m_Time((uTIME)0.0)
   {  }

   TPZTimeStamp(const T& data, uTIME time) :
               m_Element(data),
               m_Time(time)
   {  }

   TPZTimeStamp(const TPZTimeStamp<T>& aTimeStamp) :
               m_Element(aTimeStamp.m_Element),
               m_Time(aTimeStamp.m_Time)
   {  }

   uTIME timeStamp() const
   { return m_Time; }

   T element() const
   { return m_Element; }

   TPZString asString() const
   {  return TPZString(timeStamp()) + " " + element().asString(); }
   
   Boolean operator==(const TPZTimeStamp<T>& aTimeS) const
   {
      if( (m_Element == aTimeS.element()) &&
          (m_Time == aTimeS.timeStamp()) )
         return true;
      return false;
   }
private:
   T       m_Element;
   uTIME m_Time;
};

//************************************************************************

template <class T>
class TPZPriorityQueue : public TPZBaseCollection<TPZTimeStamp<T> >
{
   typedef TPZBaseCollection<TPZTimeStamp<T> > Inhereited;

public:

   TPZPriorityQueue();
   ~TPZPriorityQueue();

   Boolean enqueue(const T& data, uTIME timeStamp);
   Boolean enqueue(const T& data);

   Boolean dequeue(T& data);
   
   Boolean firstElement(T& data);
   Boolean firstElementTimeStamp(uTIME& time);

   TPZNotifier* getNotifier() const
   { return m_Notifier; }

private:

   void onFirstElement();
   TPZNotifier* m_Notifier;
};

//************************************************************************

#include "TPZPriorityQueue.inl"

#endif


// end of file

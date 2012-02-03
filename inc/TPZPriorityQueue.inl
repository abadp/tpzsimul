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
//    File: TPZPriorityQueue.inl
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header



//*************************************************************************
//
//   TPZPriorityQueue ()
//
//*************************************************************************

template <class T>
TPZPriorityQueue<T> :: TPZPriorityQueue()
                     : m_Notifier(0L)
{
   m_Notifier = new TPZNotifier();
}


//*************************************************************************
//
//   ~ TPZPriorityQueue ()
//
//*************************************************************************

template <class T>
TPZPriorityQueue<T> :: ~TPZPriorityQueue()
{
   delete m_Notifier;
}


//*************************************************************************
//
//   Boolean enqueue (const T & data, TPZTIME timeStamp)
//
//*************************************************************************

template <class T>
Boolean TPZPriorityQueue<T> :: enqueue(const T& data, uTIME timeStamp)
{
   TPZTimeStamp<T> newElement(data,timeStamp);
   if( ! this->isValid() )
   {
      // not inserted anything yet. Establishing m_pRoot
      Boolean retorno = Inhereited::insertAfter(newElement,0L);
      if(retorno) onFirstElement();
      return retorno;
   }
   TPZNode<TPZTimeStamp<T> >* tempNode = this->getRoot();
   while(tempNode)
   {
      if( tempNode->element().timeStamp() > timeStamp )
      {
         Boolean retorno = Inhereited::insertBefore(newElement,tempNode);
         return retorno;
      }
      tempNode = tempNode->getNext();
   }
   // If you come here to be inserted at the end of the list.
   return Inhereited::insertAfter(newElement,this->getTail());
}


//*************************************************************************
//
//   Boolean enqueue (const T & data)
//
//*************************************************************************

template <class T>
Boolean TPZPriorityQueue<T> :: enqueue(const T& data)
{
   return TPZPriorityQueue<T>::enqueue(data,0);
}


//*************************************************************************
//
//
//
//*************************************************************************

template <class T>
Boolean TPZPriorityQueue<T> :: dequeue(T& data)
{
   Boolean retorno = false;
   if( this->isValid() )
   {
      TPZTimeStamp<T> temp = this->getRoot()->element();
      data = temp.element();
      retorno = Inhereited::remove(this->getRoot());
      // If after clearing the top of the queue, there are more data,
      // notify the observers.
      
      // if (isValid ()) onFirstElement ();
   
   }
   return retorno;
}


//*************************************************************************
//
//
//
//*************************************************************************

template <class T>
Boolean TPZPriorityQueue<T> :: firstElement(T& data)
{
   if( this->isValid() )
   {
      TPZTimeStamp<T> temp = this->getRoot()->element();
      data = temp.element();
      return true;
   }
   return false;
}


//*************************************************************************
//
//
//
//*************************************************************************

template <class T>
Boolean TPZPriorityQueue<T> :: firstElementTimeStamp(uTIME& time)
{ 
   if( this->isValid() )
   {
      TPZTimeStamp<T> temp = this->getRoot()->element();
      time = temp.timeStamp();      
      return true;
   }
   
   return false;
}


//*************************************************************************
//
//
//
//*************************************************************************

template <class T>
void TPZPriorityQueue<T> :: onFirstElement()
{
//   if (m_Notifier)
//      m_Notifier-> notifyObservers (_FirstElement_)
}
 
//*************************************************************************


// end of file

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
//    File: TPZString.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZString_HPP__
#define __TPZString_HPP__

//*****************************************************************
   
   #include <iostream>
   #include <fstream>
   #include <ostream>
   #include <stdlib.h>
   
   //class ostream;
   //class istream;
   //class ofstream;
   //class ifstream;
   using namespace std;
//*****************************************************************

   class TPZString
   {
   public:

      TPZString();
      TPZString(const TPZString& aTPZString);
      TPZString(const char* aString);
      TPZString(char aChar);
      TPZString(int aInt);
      TPZString(long aLong);
      TPZString(unsigned uint);
      TPZString(unsigned long ulong);
      TPZString(unsigned long long aLongLong);
      TPZString(double aDouble);

      ~TPZString();

      unsigned size() const { return m_Tamano; }
      
      int    isLike(const TPZString& aString) const;
      int    asInteger() const;
      double asDouble() const;

      unsigned indexOf(const TPZString& aTPZString, unsigned startPos=1) const;
      unsigned indexOf(char aCharacter, unsigned startPos=1) const;
      unsigned lastIndexOf(char aCharacter) const;

      TPZString  subString(unsigned first, unsigned increment=0) const;
      TPZString  word(unsigned index) const;
      TPZString  firstWord() const;
      TPZString  lastWord() const;
      void       removeFirstWord();

      void removeFirst();
      void removeLast();
      TPZString& remove(const TPZString& string);
      void replace(char org, char dst);
                 
      TPZString& upperCase();
      TPZString& lowerCase();

      TPZString& sprintf(const char *format, ...);
      
      void setChar(int index, char car);

      char getChar(int index);//function added by pablo
      
      static TPZString  stripBlanks(const TPZString& aTPZString);
      static TPZString  invert(const TPZString& aTPZString);
      static TPZString  lineFrom(istream& aStream, char delim='\n');

      static TPZString  lowerCase(const TPZString& aString);
      static TPZString  upperCase(const TPZString& aString);
      static TPZString  getStringLeftTo(const TPZString& text, char delim);
      static TPZString  getStringRightTo(const TPZString& text, char delim);
	   static TPZString  withoutBlanks(const TPZString& string);
           
      int        operator == (const TPZString& aTPZString) const;
      int        operator == (const char* pString) const;
      int        operator != (const TPZString& aTPZString) const;
      int        operator != (const char* pString) const;
      char       operator [] (unsigned index) const;
      TPZString  operator +  (const TPZString& aTPZString) const;
      TPZString  operator +  (const char* aString) const;
      TPZString  operator +  (char aCharacter) const;
      TPZString& operator += (const TPZString& aTPZString);
      TPZString& operator += (char car);
      TPZString& operator =  (const TPZString& aTPZString);
      TPZString& operator =  (const char* aString);

      operator char*() const;

      friend ostream&  operator << (ostream& os, const TPZString& aTPZString);
      friend ofstream& operator << (ofstream& os, const TPZString& aTPZString);

   private:
      char*    m_pBuffer;
      unsigned m_Tamano;

   };


#endif


// end of file

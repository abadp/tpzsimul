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
//    File: TPZString.cpp
//
//    Class:  TPZString
//
//    Inherited from: 
// :
//*************************************************************************
//end of header



#include <TPZString.hpp>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
/*#include <zlib.h>*/

//*************************************************************************
//:
//  f: inline unsigned size () const;
//  d: Returns the number of characters in the string;
//: 
//*************************************************************************

//*************************************************************************
//:
//  f: TPZString ();
// 
//  d: Default Constructor. Initialize the buffer to 0;
//: 
//*************************************************************************

TPZString :: TPZString() 
           : m_pBuffer(0), 
             m_Tamano(0)
{

}


//*************************************************************************
//:
//  f: TPZString (const TPZString & aTPZString)
//  
//  d: Copy Constructor. Build a buffer and copy all characters
//:
//*************************************************************************

TPZString :: TPZString(const TPZString& aString)
           : m_Tamano(aString.m_Tamano),
             m_pBuffer(0)
{
   if(m_Tamano)
   {
      m_pBuffer = new char[m_Tamano+1];
      memcpy(m_pBuffer,aString.m_pBuffer,(m_Tamano+1)*sizeof(char));
   }
}


//*************************************************************************
//:
//  f: TPZString (const char * aString);
//
//  d: Build a string from a string;
//:
//*************************************************************************

TPZString :: TPZString(const char* aString)
           : m_Tamano( strlen(aString)  ),
             m_pBuffer(0)
{
   if( m_Tamano )
   {
      m_pBuffer = new char[ m_Tamano+1 ];
      strcpy(m_pBuffer,aString);
   }
}


//*************************************************************************
//:
//  f: TPZString (char achar);
//
//  d: Build a string containing a single character;
//:
//*************************************************************************

TPZString :: TPZString(char aChar)
           : m_Tamano( 1 )
{
   m_pBuffer = new char[2];
   m_pBuffer[0] = aChar;
   m_pBuffer[1] = '\0';
}


//*************************************************************************
//:
//  f: TPZString (int NOT);
//
//  d: It constructs a string consisting of digits representing
//     the integer passed as parameter;
//:
//*************************************************************************

TPZString :: TPZString(int aInt)
{
   char temp[22];
   for(int clear=0;clear<22;clear++) temp[clear]=0; 
   ::sprintf(temp,"%d",aInt);
   m_Tamano = strlen(temp) ;
   m_pBuffer = new char[ m_Tamano+1 ];
   strcpy(m_pBuffer,temp);
}


//*************************************************************************
//:
//  f: TPZString (long along);
//
//  d: It constructs a string consisting of digits representing
//     long integer number passed as parameter;
//:
//*************************************************************************

TPZString :: TPZString(long aLong)
{
   char temp[22];
   for(int clear=0;clear<22;clear++) temp[clear]=0; 
   ::sprintf(temp,"%d",aLong);
   m_Tamano = strlen(temp) ;
   m_pBuffer = new char[ m_Tamano+1 ];
   strcpy(m_pBuffer,temp);
}

//*************************************************************************
//:
//  f: TPZString (long long aLongLong)
//
//  d: It constructs a string consisting of digits representing
//     whole number length of 64 bits that is passed as a parameter;
//:
//*************************************************************************

TPZString :: TPZString(unsigned long long aLongLong)
{
   char temp[22];
   for(int clear=0;clear<22;clear++) temp[clear]=0; 
   ::sprintf(temp,"%d",aLongLong);
   m_Tamano = strlen(temp) ;
   m_pBuffer = new char[ m_Tamano+1 ];
   strcpy(m_pBuffer,temp);
}

//*************************************************************************
//:
//  f: TPZString (unsigned uint);
//
//  d: It constructs a string consisting of digits representing
//     The unsigned integer is passed as a parameter;
//:
//*************************************************************************

TPZString :: TPZString(unsigned uint)
{
   char temp[22];
   for(int clear=0;clear<22;clear++) temp[clear]=0; 
   ::sprintf(temp,"%d",uint);
   m_Tamano = strlen(temp) ;
   m_pBuffer = new char[ m_Tamano+1 ];
   strcpy(m_pBuffer,temp);
}


//*************************************************************************
//:
//  f: TPZString (unsigned long ulong);
//
//  d: It constructs a string consisting of digits representing
//     the unsigned long integer that is passed as a parameter;
//:
//*************************************************************************

TPZString :: TPZString(unsigned long ulong)
{
   char temp[22];
   for(int clear=0;clear<22;clear++) temp[clear]=0; 
   ::sprintf(temp,"%d",ulong);
   m_Tamano = strlen(temp) ;
   m_pBuffer = new char[ m_Tamano+1 ];
   strcpy(m_pBuffer,temp);
}


//*************************************************************************
//:
//  f: TPZString (double adouble)
//
//  d: It constructs a string consisting of digits representing
//     real number that is passed as a parameter;
//:
//*************************************************************************

TPZString :: TPZString(double aDouble)
{
   char temp[30];
   for(int clear=0;clear<30;clear++) temp[clear]=0; 
   ::sprintf(temp,"%g",aDouble);
   m_Tamano = strlen(temp) ;
   m_pBuffer = new char[ m_Tamano+1 ];
   strcpy(m_pBuffer,temp);
}


//*************************************************************************
//:
//  f: ~ TPZString ();
//
//  d: Destroyer. Deletes the character buffer if it was created;
//:
//*************************************************************************

TPZString :: ~TPZString()
{
   delete[] m_pBuffer;
}


//*************************************************************************
//:
//  f: int isLike (const TPZString & aString) const;
//
//  d: Returns 1 if the pattern that is passed as parameter mTPZhes
//     the string object. Otherwise, it returns 0;
//:
//*************************************************************************

int TPZString :: isLike(const TPZString& aString) const
{
   if( aString == TPZString("*") )
   {
      return 1;
   }
   else if( aString == *this )
   {
      return 1;
   }
   
   return 0;
}

//*************************************************************************
//:
//  f: int AsInteger () const;
//
//  d:
//:
//*************************************************************************

int TPZString :: asInteger() const
{
   if( m_pBuffer )
      return atoi(m_pBuffer);
      
   return -1;
}


//*************************************************************************
//:
//  f: double asDouble () const;
//
//  d:
//:
//*************************************************************************

double TPZString :: asDouble() const
{
   if( m_pBuffer )
      return (double)(atof(m_pBuffer));
      
   return -1.0;
}


//*************************************************************************
//:
//  f: TPZString & sprintf (const char * format, ...);
//
//  d:
//:
//*************************************************************************

TPZString& TPZString :: sprintf(const char *format, ...)
{
    va_list ap;
    va_start( ap, format );
    char buffer[1024];
    vsprintf( buffer, format, ap );
    va_end( ap );
    return operator=(buffer);
}


//*************************************************************************
//:
//  f: unsigned indexOf (const TPZString & aTPZString, unsigned startPos) const;
//
//  d:
//:
//*************************************************************************

unsigned TPZString :: indexOf(const TPZString& aTPZString, unsigned startPos) const
{
   if( startPos>size() ) return 0;

   char* pSource = (char *)aTPZString;
   char* pExiste = strstr( m_pBuffer, pSource );
   if( !pExiste ) return 0;

   unsigned offset = 0;
   for( unsigned i=startPos-1; i<size(); i++)
      if( pExiste == (m_pBuffer+i) )
      {
          offset = i+1;
          break;
      }
   return offset;
}


//*************************************************************************
//:
//  f: unsigned indexOf (char aCharacter, unsigned startPos) const;
//
//  d:
//:
//*************************************************************************

unsigned TPZString :: indexOf(char aCharacter, unsigned startPos) const
{
   if( startPos>size() ) return 0;

   unsigned offset = 0;
   for( unsigned i=startPos-1; i < size(); i++ )
      if( aCharacter==*(m_pBuffer+i) )
      {
         offset = i + 1;
         break;
      }

   return offset;
}


//*************************************************************************
//:
//  f: unsigned lastIndexOf (char aCharacter) const;
//
//  d:
//:
//*************************************************************************

unsigned TPZString :: lastIndexOf(char aCharacter) const
{
   unsigned offset = 0;
   for( int i=size()-1; i>=0; i-- )
      if( aCharacter==*(m_pBuffer+i) )
      {
         offset = i + 1;
         break;
      }
   return offset;
}


//*************************************************************************
//:
//  f: TPZString substring (unsigned first, unsigned increment) const;
//
//  d:
//:
//*************************************************************************

TPZString TPZString :: subString(unsigned first, unsigned increment) const
{
   TPZString rs;
   if( (first+increment-1) > size() )
       return rs;

   unsigned length = (increment) ? increment : m_Tamano-first+1;
   char* pTemp = new char[length+1];
   for(register int i=0; i<length; i++)
       pTemp[i] = m_pBuffer[first+i-1];

   pTemp[length] = '\0';
   
   rs = pTemp;
   delete[] pTemp;
   return rs;
}


//*************************************************************************
//:
//  f: TPZString word (unsigned index) const;
//
//  d:
//:
//*************************************************************************

TPZString TPZString :: word(unsigned index) const
{
   TPZString sinBlancos = TPZString::stripBlanks(*this);

   if( sinBlancos.size()==0 )
      return "";

   sinBlancos += TPZString(" ");
   unsigned firstIndex=1;
   unsigned numeroPalabra=0;

   TPZString palabraBuscada;
   int lastCharacterIsSpace = 0;

   for( unsigned lastIndex=1; lastIndex<=sinBlancos.size(); lastIndex++ )
   {
      char caracter = sinBlancos[lastIndex];
      if( isspace(caracter) )
      {
         if( !lastCharacterIsSpace )
         {
            numeroPalabra++;
            if( numeroPalabra==index )
            {
               unsigned delta = lastIndex-firstIndex;
               palabraBuscada = sinBlancos.subString( firstIndex, delta);
               break;
            }
            else
            {
               firstIndex = lastIndex + 1;
            }
         }
         lastCharacterIsSpace = 1;
      }
      else
         lastCharacterIsSpace = 0;

   }
   return TPZString::stripBlanks(palabraBuscada);
}


//*************************************************************************
//:
//  f: TPZString FirstWord () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZString :: firstWord() const
{
   return word(1);
}


//*************************************************************************
//:
//  f: TPZString lastWord () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZString :: lastWord() const
{
   TPZString reflejo = TPZString::invert(*this);
   return TPZString::invert(reflejo.word(1)) ;
}


//*************************************************************************
//:
//  f: void removeFirstWord ();
//
//  d:
//:
//*************************************************************************

void TPZString :: removeFirstWord()
{
   TPZString temp = firstWord();
   int index = temp.indexOf(temp);
   (*this) = subString(index+temp.size()+1);
}


//*************************************************************************
//:
//  f: void removeFirst ();
//
//  d:
//:
//*************************************************************************

void TPZString :: removeFirst()
{
   (*this) = subString(2);
}


//*************************************************************************
//:
//  f: void removeLast ();
//
//  d:
//:
//*************************************************************************

void TPZString :: removeLast()
{
   (*this) = subString(1,size()-1);
}


//*************************************************************************
//:
//  f: TPZString & remove (const TPZString & string);
//
//  d:
//:
//*************************************************************************

TPZString& TPZString :: remove(const TPZString& string)
{
   int index  = indexOf(string);
   if( index )
   {
      (*this) = ( (index==1) ? TPZString("") : subString(1,index-1) ) + 
                subString(index+string.size());
   }
     
   return *this;
}


//*************************************************************************
//:
//  f: void replace (char org, char dst);
//
//  d:
//:
//*************************************************************************

void TPZString :: replace(char org, char dst)
{
   for(register int i=0; i<m_Tamano; i++)
   {
      if( *(m_pBuffer+i) == org ) 
         *(m_pBuffer+i) = dst;
   }
}

//*************************************************************************
//:
//  f: TPZString & UpperCase ();
//
//  d:
//:
//*************************************************************************

TPZString& TPZString :: upperCase()
{
   char current;
   for(int i=0; i<m_Tamano; i++)
   {
      current = *(m_pBuffer+i);
      if( islower(current) )
         *(m_pBuffer+i) = toupper(current);
   }
   return *this;
}


//*************************************************************************
//:
//  f: TPZString & lowercase ();
//
//  d:
//:
//*************************************************************************

TPZString& TPZString :: lowerCase()
{
   char current;
   for(int i=0; i<m_Tamano; i++)
   {
      current = *(m_pBuffer+i);
      if( isupper(current) )
         *(m_pBuffer+i) = tolower(current);
   }
   return *this;
}


//*************************************************************************
//:
//  f: static TPZString lowercase (const TPZString & aString);
//
//  d:
//:
//*************************************************************************

TPZString TPZString :: lowerCase(const TPZString& aString)
{
   TPZString returnSt = aString;
   return returnSt.lowerCase();
}


//*************************************************************************
//:
//  f: static TPZString lowercase (const TPZString & aString);
//
//  d:
//:
//*************************************************************************

TPZString TPZString :: upperCase(const TPZString& aString)
{
   TPZString returnSt = aString;
   return returnSt.upperCase();
}


//*************************************************************************
//:
//  f: int operator == (const aTPZString TPZString &) const;
//
//  d:
//:
//*************************************************************************

int TPZString :: operator == (const TPZString& aTPZString) const
{
   if( size() != aTPZString.size() )
       return 0;

   for( int i=0; i < size(); i++)
      if( *(m_pBuffer+i) != aTPZString[(unsigned)(i+1)] )
         return 0;
   return 1;
}


//*************************************************************************
//:
//  f: int operator == (const char * PString) const;
//
//  d:
//:
//*************************************************************************

int TPZString :: operator == (const char* pString) const
{
   TPZString tempString(pString);
   return  TPZString::operator==(tempString);
}


//*************************************************************************
//:
//  f: int operator! = (const aTPZString TPZString &) const;
//
//  d:
//:
//*************************************************************************

int TPZString :: operator != (const TPZString& aTPZString) const
{
   if( size() != aTPZString.size() )
       return 1;

   for( int i=0; i < size(); i++)
      if( *(m_pBuffer+i) != aTPZString[(unsigned)(i+1)] )
         return 1;
   return 0;
}


//*************************************************************************
//:
//  f: int operator! = (const char * PString) const;
//
//  d:
//:
//*************************************************************************

int TPZString :: operator != (const char* pString) const
{
   return operator!=(TPZString(pString));
}


//*************************************************************************
//:
//  f: operator char * () const;
//
//  d:
//:
//*************************************************************************

TPZString :: operator char* () const
{
   return m_pBuffer;
}


//*************************************************************************
//:
//  f: char operator [] (unsigned index) const;
//
//  d:
//:
//*************************************************************************

char TPZString :: operator [] (unsigned index) const
{
   return ((index<=m_Tamano)&&index) ? (m_pBuffer[index-1]) : 0;
}


//*************************************************************************
//:
//  f: TPZString operator + (const aTPZString TPZString &) const;
//
//  d:
//:
//*************************************************************************

TPZString TPZString :: operator + (const TPZString& aString) const
{
   TPZString rs = *this;
   
   if( aString.m_Tamano )
   {
      unsigned sizeTotal = m_Tamano + aString.m_Tamano + 1;
      char* pTemp = new char[sizeTotal];
      memset(pTemp,0,sizeTotal*sizeof(char));
      if(m_pBuffer) strcpy( pTemp, m_pBuffer );
      strcat( pTemp,aString.m_pBuffer );
      rs = pTemp;
      delete[] pTemp;
   }
   return rs;
}


//*************************************************************************
//:
//  f: TPZString operator + (const char * aString) const;
//
//  d:
//:
//*************************************************************************

TPZString TPZString :: operator + (const char* aString) const
{
   return operator+(TPZString(aString));
}


//*************************************************************************
//:
//  f: TPZString operator + (char aCharacter) const;
//
//  d:
//:
//*************************************************************************

TPZString TPZString :: operator + (char aCharacter) const
{
   return operator+(TPZString(aCharacter));
}


//*************************************************************************
//:
//  f: TPZString & operator + = (const TPZString & aTPZString)
//
//  d:
//:
//*************************************************************************

TPZString& TPZString :: operator += (const TPZString& aString)
{
   if( aString.m_Tamano  )
   {
      TPZString temp = operator+(aString);
      operator=(temp);
   }
   return *this;
}


//*************************************************************************
//:
//  f: TPZString & operator + = (char car);
//
//  d:
//:
//*************************************************************************

TPZString& TPZString :: operator += (char car)
{
   return operator+=(TPZString(car));
}


//*************************************************************************
//:
//  f: TPZString & operator = (const TPZString & aTPZString)
//
//  d:
//:
//*************************************************************************

TPZString& TPZString :: operator = (const TPZString& aString)
{
   delete[] m_pBuffer;
   m_pBuffer = 0;
   
   m_Tamano = aString.m_Tamano;
   
   if( m_Tamano )
   {
      m_pBuffer = new char[m_Tamano+1];
      memcpy(m_pBuffer,aString.m_pBuffer,(m_Tamano+1)*sizeof(char));
   }
   
   return *this;
}


//*************************************************************************
//:
//  f: TPZString & operator = (const char * aString);
//
//  d:
//:
//*************************************************************************

TPZString& TPZString :: operator = (const char* aString)
{
   return operator=(TPZString(aString));
}



//*************************************************************************
//:
//  f: TPZString lineFrom (istream & astream, char delim);
//
//  d:
//:
//*************************************************************************

TPZString TPZString :: lineFrom ( istream& aStream, char delim)
{
   TPZString result;
   // unsigned bytes = 0;
   char c=0;

   for( aStream.get(c); c!=delim && !aStream.fail(); aStream.get(c) )
       result += c;

   return result;
}

//*************************************************************************
//:
//  f: TPZString stripBlanks ();
//
//  d:
//:
//*************************************************************************

TPZString TPZString :: stripBlanks(const TPZString& aTPZString)
{
   if( aTPZString.size() == 0 ) return "";

   unsigned first, last;
   for( first=1; isspace(aTPZString[first]); first++ );
   for( last=aTPZString.size(); isspace(aTPZString[last]); last-- );

   if( first > last ) return "";

   TPZString palabraSinBlancos = aTPZString;
   return palabraSinBlancos.subString(first,last-first+1);
}


//*************************************************************************
//:
//  f: static TPZString invert ();
//
//  d:
//:
//*************************************************************************

TPZString TPZString :: invert(const TPZString& aTPZString)
{
   TPZString reflejo;
   for( int i=aTPZString.size(); i>=1; i-- )
      reflejo += aTPZString[(unsigned)i];
   return reflejo;
}


//*************************************************************************
//:
//  f: static TPZString getStringLeftTo (const TPZString & text, char delim);
//
//  d: g
//:
//*************************************************************************

TPZString TPZString :: getStringLeftTo(const TPZString& text, char delim)
{
   unsigned index = text.indexOf(delim);
   if( index )
      return text.subString(1,index-1);
      
   // In the absence of the delimiter string is returned unwillingly
   // passed by reference.
   
   return text;
}


//*************************************************************************
//:
//  f: static TPZString getStringRightTo (const TPZString & text, char delim);
//
//  d:
//:
//*************************************************************************

TPZString TPZString :: getStringRightTo(const TPZString& text, char delim)
{
   unsigned index = text.indexOf(delim);
   if( index )
      return text.subString(index+1);
   
   // If no success the delimiter returns an empty string.
   return "";
}


//*************************************************************************
//:
//  f: static TPZString withoutBlanks (const TPZString & text);
//
//  d:
//:
//*************************************************************************

TPZString TPZString :: withoutBlanks(const TPZString& text)
{
   TPZString rs;
   for( int i=1; i<=text.size(); i++ )
   {
      if( text[(unsigned)i] != ' ' )
      {
         rs += text[(unsigned)i];
      }
   }
   return rs;
}


//*************************************************************************
//:
//  f: void setChar (int index, char car);
//
//  d:
//:
//*************************************************************************

void TPZString :: setChar( int index, char car )
{
   if( index && index<=m_Tamano ) m_pBuffer[index-1] = car;
}


//*************************************************************************
//:
//  f: char getch (int index);
//
//  d: Paul pa function added by the roundabout
//:
//*************************************************************************

char TPZString :: getChar( int index)
{
   char aux;
   if ( index && index<=m_Tamano ) aux = m_pBuffer[index-1];
   return aux;
}

//*************************************************************************
//:
//  f: friend ostream & operator <<(ostream & os, const TPZString & aTPZString)
//
//  d:
//:
//*************************************************************************

ostream& operator << ( ostream& os, const TPZString& aTPZString )
{
   char* buffer= (char *)aTPZString;
   if(buffer)
      os << buffer;
   return os;
}


//*************************************************************************
//:
//  f: friend ofstream & operator <<(ofstream & ofs, const TPZString & aTPZString)
//
//  d:
//:
//*************************************************************************

ofstream& operator << ( ofstream& ofs, const TPZString& aTPZString )
{
   char* buffer= (char *)aTPZString;
   if(buffer)
      ofs.operator<<(buffer);
   return ofs;
}


//*************************************************************************

// end of file

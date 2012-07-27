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
//    File: TPZFicheroLog.cpp
//
//    Class:  TPZFicheroLog
//
//    Inherited from:  TPZBase
// :
//*************************************************************************
//end of header



#include <TPZFicheroLog.hpp>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

static TPZString _logName = "TraceLog.txt";
static TPZString _bakName = "TraceLog.bak";
static unsigned  _linesSaved = 250000;

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZFicheroLog,TPZBase);

//*************************************************************************

TPZFicheroLog* TPZFicheroLog :: m_Instance = 0;


//*************************************************************************
//:
//  f: TPZFicheroLog ();
//
//  d:
//:
//*************************************************************************

TPZFicheroLog :: TPZFicheroLog()
               : m_LinesWrited(0),
                 m_print(true)
{
   unlink(_logName);
   m_FileLog = new ofstream(_logName);
   if( ! m_FileLog->good() )
   {
      TPZString err;
      err.sprintf( ERR_TPZFILOG_001, (char*)_logName );
      EXIT_PROGRAM(err);
   }
   
   // File header
   
   time_t theTime = time(0);
   TPZString fileHeader = TPZString("//***************************************") +
                        TPZString("\n//   File : ") +   _logName +
                        TPZString("\n//   Date :    ") +
                        TPZString(ctime(&theTime)) +
                        TPZString("//*************************************** \\ ");
                                             
   m_FileLog->write((char*)fileHeader, fileHeader.size());
}


//*************************************************************************
//:
//  f: ~ TPZFicheroLog ();
//
//  d:
//:
//*************************************************************************

TPZFicheroLog :: ~TPZFicheroLog()
{
   m_FileLog->close();
   delete m_FileLog;
}


//*************************************************************************
//:
//  f: TPZFicheroLog * instance ();
//
//  d:
//:
//*************************************************************************

TPZFicheroLog* TPZFicheroLog :: instance()
{
   if( ! m_Instance )
      m_Instance = new TPZFicheroLog();

   return m_Instance;
}


//*************************************************************************
//:
//  f: void destroyInstance ();
//
//  d:
//:
//*************************************************************************

void TPZFicheroLog :: destroyInstance()
{
   delete m_Instance;
   m_Instance = 0L;
}

//*************************************************************************
//:
//  f: void write (const TPZString & text);
//
//  d:
//:
//*************************************************************************

void TPZFicheroLog :: write(const TPZString& text)
{   
#ifdef PTOPAZ
   TPZString final=TPZString(" thread ID: ")+TPZString(pthread_self())+TPZString("\n");
#else
   TPZString final=TPZString("\n");
#endif
   m_FileLog->write((char*)text,text.size());
   m_FileLog->write((char*)final,final.size());
   if( (++m_LinesWrited) >= _linesSaved )
   {
      m_FileLog->close();
      rename(_logName,_bakName);
      m_FileLog->open(_logName,ios::out|ios::trunc);
      m_LinesWrited = 0;
   }   
}


//*************************************************************************

// end of file

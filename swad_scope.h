// swad_scope.h: scope (platform, centre, degree, course...)

#ifndef _SWAD_SCO
#define _SWAD_SCO
/*
    SWAD (Shared Workspace At a Distance in Spanish),
    is a web platform developed at the University of Granada (Spain),
    and used to support university teaching.

    This file is part of SWAD core.
    Copyright (C) 1999-2018 Antonio Ca�as Vargas

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*****************************************************************************/
/********************************* Headers ***********************************/
/*****************************************************************************/

#include <stdbool.h>		// For boolean type

/*****************************************************************************/
/***************************** Public constants ******************************/
/*****************************************************************************/

/*****************************************************************************/
/******************************* Public types ********************************/
/*****************************************************************************/

// Related with scope of courses-degrees-centres
#define Sco_NUM_SCOPES	7
typedef enum
  {
   Sco_SCOPE_UNK = 0,	// Unknown
   Sco_SCOPE_SYS = 1,	// System
   Sco_SCOPE_CTY = 2,	// Country
   Sco_SCOPE_INS = 3,	// Institution
   Sco_SCOPE_CTR = 4,	// Centre
   Sco_SCOPE_DEG = 5,	// Degree
   Sco_SCOPE_CRS = 6,	// Course
  } Sco_Scope_t;

/*****************************************************************************/
/***************************** Public prototypes *****************************/
/*****************************************************************************/

void Sco_PutSelectorScope (const char *ParamName,bool SendOnChange);
void Sco_PutParamScope (const char *ParamName,Sco_Scope_t Scope);
void Sco_GetScope (const char *ParamName);
void Sco_AdjustScope (void);

void Sco_SetScopesForListingGuests (void);
void Sco_SetScopesForListingStudents (void);

Sco_Scope_t Sco_GetScopeFromUnsignedStr (const char *UnsignedStr);
Sco_Scope_t Sco_GetScopeFromDBStr (const char *ScopeDBStr);

#endif

// swad_project.h: projects (final degree projects, thesis)

#ifndef _SWAD_PRJ
#define _SWAD_PRJ
/*
    SWAD (Shared Workspace At a Distance),
    is a web platform developed at the University of Granada (Spain),
    and used to support university teaching.

    This file is part of SWAD core.
    Copyright (C) 1999-2017 Antonio Ca�as Vargas

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

#include "swad_date.h"
#include "swad_file_browser.h"
#include "swad_notification.h"
#include "swad_user.h"

/*****************************************************************************/
/************************** Public types and constants ***********************/
/*****************************************************************************/

#define Prj_MAX_CHARS_PROJECT_TITLE		(128 - 1)	// 127
#define Prj_MAX_BYTES_PROJECT_TITLE		((Prj_MAX_CHARS_PROJECT_TITLE       + 1) * Str_MAX_BYTES_PER_CHAR - 1)	// 2047

#define Prj_NUM_TYPES_PREASSIGNED 2
typedef enum
  {
   Prj_NOT_PREASSIGNED = 0,
   Prj_PREASSIGNED     = 1,
  } Prj_Preassigned_t;
#define Prj_PREASSIGNED_DEFAULT Prj_NOT_PREASSIGNED

#define Prj_NUM_PROPOSAL_TYPES 3
typedef enum
  {
   Prj_PROPOSAL_NEW,
   Prj_PROPOSAL_MODIFIED,
   Prj_PROPOSAL_UNMODIFIED,
  } Prj_Proposal_t;
#define Prj_PROPOSAL_DEFAULT Prj_PROPOSAL_NEW

// Related with user's roles in a project
// Don't change these numbers! They are used in database
#define Prj_NUM_ROLES_IN_PROJECT 4
typedef enum
  {
   Prj_ROLE_UNK	= 0,	// Unknown
   Prj_ROLE_STD	= 1,	// Student
   Prj_ROLE_TUT	= 2,	// Tutor
   Prj_ROLE_EVA	= 3,	// Evaluator
  } Prj_RoleInProject_t;

struct Project
  {
   long PrjCod;
   bool Hidden;
   Prj_Preassigned_t Preassigned;
   unsigned NumStds;
   Prj_Proposal_t Proposal;
   time_t TimeUTC[Dat_NUM_START_END_TIME];
   bool Open;
   char Title[Prj_MAX_BYTES_PROJECT_TITLE + 1];
   long DptCod;
   char *Description;
   char *Knowledge;
   char *Materials;
   char URL[Cns_MAX_BYTES_WWW + 1];
  };

#define Prj_NUM_ORDERS 4
typedef enum
  {
   Prj_ORDER_START_TIME = 0,
   Prj_ORDER_END_TIME   = 1,
   Prj_ORDER_TITLE      = 2,
   Prj_ORDER_DEPARTMENT = 3,
  } Prj_Order_t;
#define Prj_ORDER_DEFAULT Prj_ORDER_START_TIME

#define Prj_NUM_WHICH_PROJECTS 2
typedef enum
  {
   Prj_ONLY_MY_PROJECTS,
   Prj_ALL_PROJECTS,
  } Prj_WhichProjects_t;
#define Prj_WHICH_PROJECTS_DEFAULT Prj_ONLY_MY_PROJECTS

/*****************************************************************************/
/***************************** Public prototypes *****************************/
/*****************************************************************************/

void Prj_SeeProjects (void);
void Prj_ShowTableAllProjects (void);
void Prj_PrintOneProject (void);

void Prj_ReqAddStd (void);
void Prj_ReqAddTut (void);
void Prj_ReqAddEva (void);

void Prj_AddStd (void);
void Prj_AddTut (void);
void Prj_AddEva (void);

void Prj_ReqRemStd (void);
void Prj_ReqRemTut (void);
void Prj_ReqRemEva (void);

void Prj_RemStd (void);
void Prj_RemTut (void);
void Prj_RemEva (void);

void Prj_PutHiddenParamPrjOrder (void);

void Prj_RequestCreatePrj (void);
void Prj_RequestEditPrj (void);

void Prj_GetListProjects (void);
void Prj_GetDataOfProjectByCod (struct Project *Prj);
void Prj_FreeListProjects (void);

long Prj_GetParamPrjCod (void);
void Prj_ReqRemProject (void);
void Prj_RemoveProject (void);
void Prj_HideProject (void);
void Prj_ShowProject (void);
void Prj_RecFormProject (void);

void Prj_RemoveCrsProjects (long CrsCod);
void Prj_RemoveUsrFromProjects (long UsrCod);

unsigned Prj_GetNumCoursesWithProjects (Sco_Scope_t Scope);
unsigned Prj_GetNumProjects (Sco_Scope_t Scope);

#endif

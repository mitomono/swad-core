// swad_constant.h: common constants used in several modules

#ifndef _SWAD_CNS
#define _SWAD_CNS
/*
    SWAD (Shared Workspace At a Distance),
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

#include "swad_string.h"

/*****************************************************************************/
/***************************** Public constants ******************************/
/*****************************************************************************/

#define Cns_MAX_CHARS_WWW	(256 - 1)		// 255: max. number of chars of a URL
#define Cns_MAX_BYTES_WWW	Cns_MAX_CHARS_WWW	// 255

#define Cns_MAX_CHARS_IP	(3 + 1 + 3 + 1 + 3 + 1 + 3)	// 15: max. number of chars of an IP address
				// Example: 255.255.255.255
				//           3+1+3+1+3+1+3
				//          123456789012345
#define Cns_MAX_BYTES_IP	Cns_MAX_CHARS_IP	// 15

#define Cns_MAX_CHARS_DATE	(16 - 1)	// 15
#define Cns_MAX_BYTES_DATE	((Cns_MAX_CHARS_DATE + 1) * Str_MAX_BYTES_PER_CHAR - 1)	// 255

#define Cns_MAX_CHARS_EMAIL_ADDRESS	(256 - 1)			// 255
#define Cns_MAX_BYTES_EMAIL_ADDRESS	Cns_MAX_CHARS_EMAIL_ADDRESS	// 255

#define Cns_MAX_CHARS_SUBJECT	(256 - 1)	// 255
#define Cns_MAX_BYTES_SUBJECT	((Cns_MAX_CHARS_SUBJECT + 1) * Str_MAX_BYTES_PER_CHAR - 1)	// 4095

#define Cns_MAX_BYTES_TEXT	( 64 * 1024 - 1)	// Used for medium texts
#define Cns_MAX_BYTES_LONG_TEXT	(256 * 1024 - 1)	// Used for big contents

/*****************************************************************************/
/******************************* Public types ********************************/
/*****************************************************************************/

typedef enum
  {
   Cns_QUIET,
   Cns_VERBOSE,
  } Cns_QuietOrVerbose_t;

// Related with names of centres, degrees, courses,...
typedef enum
  {
   Cns_SHRT_NAME,
   Cns_FULL_NAME,
  } Cns_ShrtOrFullName_t;

/*****************************************************************************/
/***************************** Public prototypes *****************************/
/*****************************************************************************/

#endif

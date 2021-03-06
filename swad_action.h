// swad_action.h: actions

#ifndef _SWAD_ACT
#define _SWAD_ACT
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
/********************************** Headers **********************************/
/*****************************************************************************/

#include <stdbool.h>		// For boolean type

#include "swad_cryptography.h"
#include "swad_string.h"
#include "swad_tab.h"
#include "swad_text.h"

/*****************************************************************************/
/****************************** Public constants *****************************/
/*****************************************************************************/

#define Act_MAX_CHARS_ACTION_TXT	(256 - 1)			// 255
#define Act_MAX_BYTES_ACTION_TXT	Act_MAX_CHARS_ACTION_TXT	// 255

typedef enum
  {
   Act_CONT_NORM,
   Act_CONT_DATA,
  } Act_Content_t;

typedef enum
  {
   Act_BRW_UNK_TAB,	// Unknown tab
   Act_BRW_1ST_TAB,	// The main (original, first) tab in the browser
   Act_BRW_NEW_TAB,	// A new (second) blank tab in the browser
   Act_BRW_2ND_TAB,	// The second tab in the browser
   Act_UPLOAD_FILE,	// Upload a file. Do not write HTML content. Write Status code instead for Dropzone.js
   Act_DOWNLD_FILE,	// Download a file in a new tab. Do not write HTML content.
   Act_204_NO_CONT,	// Do not write HTML content. HTTP will return Status 204 No Content
  } Act_BrowserTab_t;

typedef signed int Act_Action_t;	// Must be a signed type, because -1 is used to indicate obsolete action

#define Act_NUM_ACTIONS	(1 + 9 + 43 + 17 + 47 + 33 + 24 + 115 + 157 + 437 + 165 + 172 + 42 + 14 + 76)

#define Act_MAX_ACTION_COD 1734

#define Act_MAX_OPTIONS_IN_MENU_PER_TAB 13

#define Act_MAX_BYTES_ID (32 + Cry_BYTES_ENCRYPTED_STR_SHA256_BASE64 + 10 + 1)

/*****************************************************************************/
/************************* Not asociates with tabs ***************************/
/*****************************************************************************/
#define ActAll			 0

#define ActUnk			 1
#define ActHom			 2
#define ActMnu			 3

// The following 5 actions use AJAX to refresh only a part of the page
#define ActRefCon		 4
#define ActRefLstClk		 5
#define ActRefNewSocPubGbl	 6
#define ActRefOldSocPubGbl	 7
#define ActRefOldSocPubUsr	 8

#define ActWebSvc		 9

/*****************************************************************************/
/******************************** System tab *********************************/
/*****************************************************************************/
// Actions in menu
#define ActSysReqSch		(ActWebSvc +  1)
#define ActSeeCty		(ActWebSvc +  2)
#define ActSeePen		(ActWebSvc +  3)
#define ActSeeLnk		(ActWebSvc +  4)
#define ActLstPlg		(ActWebSvc +  5)
#define ActSetUp		(ActWebSvc +  6)
#define ActSeeCalSys		(ActWebSvc +  7)

// Secondary actions
#define ActSysSch		(ActWebSvc +  8)
#define ActEdiCty		(ActWebSvc +  9)
#define ActNewCty		(ActWebSvc + 10)
#define ActRemCty		(ActWebSvc + 11)
#define ActRenCty		(ActWebSvc + 12)
#define ActChgCtyWWW		(ActWebSvc + 13)

#define ActReqRemOldCrs		(ActWebSvc + 14)
#define ActRemOldCrs		(ActWebSvc + 15)

#define ActSeeBan		(ActWebSvc + 16)
#define ActEdiBan		(ActWebSvc + 17)
#define ActNewBan		(ActWebSvc + 18)
#define ActRemBan		(ActWebSvc + 19)
#define ActShoBan		(ActWebSvc + 20)
#define ActHidBan		(ActWebSvc + 21)
#define ActRenBanSho		(ActWebSvc + 22)
#define ActRenBanFul		(ActWebSvc + 23)
#define ActChgBanImg		(ActWebSvc + 24)
#define ActChgBanWWW		(ActWebSvc + 25)
#define ActClkBan		(ActWebSvc + 26)

#define ActEdiLnk		(ActWebSvc + 27)
#define ActNewLnk		(ActWebSvc + 28)
#define ActRemLnk		(ActWebSvc + 29)
#define ActRenLnkSho		(ActWebSvc + 30)
#define ActRenLnkFul		(ActWebSvc + 31)
#define ActChgLnkWWW		(ActWebSvc + 32)

#define ActEdiPlg		(ActWebSvc + 33)
#define ActNewPlg		(ActWebSvc + 34)
#define ActRemPlg		(ActWebSvc + 35)
#define ActRenPlg		(ActWebSvc + 36)
#define ActChgPlgDes		(ActWebSvc + 37)
#define ActChgPlgLog		(ActWebSvc + 38)
#define ActChgPlgAppKey		(ActWebSvc + 39)
#define ActChgPlgURL		(ActWebSvc + 40)
#define ActChgPlgIP		(ActWebSvc + 41)

#define ActPrnCalSys		(ActWebSvc + 42)
#define ActChgCalSys1stDay	(ActWebSvc + 43)

/*****************************************************************************/
/******************************** Country tab ********************************/
/*****************************************************************************/
// Actions in menu
#define ActCtyReqSch		(ActChgCalSys1stDay +  1)
#define ActSeeCtyInf		(ActChgCalSys1stDay +  2)
#define ActSeeIns		(ActChgCalSys1stDay +  3)
#define ActSeeCalCty		(ActChgCalSys1stDay +  4)

// Secondary actions
#define ActCtySch		(ActChgCalSys1stDay +  5)
#define ActPrnCtyInf		(ActChgCalSys1stDay +  6)
#define ActChgCtyMapAtt		(ActChgCalSys1stDay +  7)

#define ActEdiIns		(ActChgCalSys1stDay +  8)
#define ActReqIns		(ActChgCalSys1stDay +  9)
#define ActNewIns		(ActChgCalSys1stDay + 10)
#define ActRemIns		(ActChgCalSys1stDay + 11)
#define ActRenInsSho		(ActChgCalSys1stDay + 12)
#define ActRenInsFul		(ActChgCalSys1stDay + 13)
#define ActChgInsWWW		(ActChgCalSys1stDay + 14)
#define ActChgInsSta		(ActChgCalSys1stDay + 15)

#define ActPrnCalCty		(ActChgCalSys1stDay + 16)
#define ActChgCalCty1stDay	(ActChgCalSys1stDay + 17)

/*****************************************************************************/
/****************************** Institution tab ******************************/
/*****************************************************************************/
// Actions in menu
#define ActInsReqSch		(ActChgCalCty1stDay +  1)
#define ActSeeInsInf		(ActChgCalCty1stDay +  2)
#define ActSeeCtr		(ActChgCalCty1stDay +  3)
#define ActSeeDpt		(ActChgCalCty1stDay +  4)
#define ActSeeCalIns		(ActChgCalCty1stDay +  5)

// Secondary actions
#define ActInsSch		(ActChgCalCty1stDay +  6)
#define ActPrnInsInf		(ActChgCalCty1stDay +  7)
#define ActChgInsCtyCfg		(ActChgCalCty1stDay +  8)
#define ActRenInsShoCfg		(ActChgCalCty1stDay +  9)
#define ActRenInsFulCfg		(ActChgCalCty1stDay + 10)
#define ActChgInsWWWCfg		(ActChgCalCty1stDay + 11)
#define ActReqInsLog		(ActChgCalCty1stDay + 12)
#define ActRecInsLog		(ActChgCalCty1stDay + 13)
#define ActRemInsLog		(ActChgCalCty1stDay + 14)

#define ActEdiCtr		(ActChgCalCty1stDay + 15)
#define ActReqCtr		(ActChgCalCty1stDay + 16)
#define ActNewCtr		(ActChgCalCty1stDay + 17)
#define ActRemCtr		(ActChgCalCty1stDay + 18)
#define ActChgCtrPlc		(ActChgCalCty1stDay + 19)
#define ActRenCtrSho		(ActChgCalCty1stDay + 20)
#define ActRenCtrFul		(ActChgCalCty1stDay + 21)
#define ActChgCtrWWW		(ActChgCalCty1stDay + 22)
#define ActChgCtrSta		(ActChgCalCty1stDay + 23)

#define ActSeePlc		(ActChgCalCty1stDay + 24)
#define ActEdiPlc		(ActChgCalCty1stDay + 25)
#define ActNewPlc		(ActChgCalCty1stDay + 26)
#define ActRemPlc		(ActChgCalCty1stDay + 27)
#define ActRenPlcSho		(ActChgCalCty1stDay + 28)
#define ActRenPlcFul		(ActChgCalCty1stDay + 29)

#define ActEdiDpt		(ActChgCalCty1stDay + 30)
#define ActNewDpt		(ActChgCalCty1stDay + 31)
#define ActRemDpt		(ActChgCalCty1stDay + 32)
#define ActChgDptIns		(ActChgCalCty1stDay + 33)
#define ActRenDptSho		(ActChgCalCty1stDay + 34)
#define ActRenDptFul		(ActChgCalCty1stDay + 35)
#define ActChgDptWWW		(ActChgCalCty1stDay + 36)

#define ActSeeHld		(ActChgCalCty1stDay + 37)
#define ActEdiHld		(ActChgCalCty1stDay + 38)
#define ActNewHld		(ActChgCalCty1stDay + 39)
#define ActRemHld		(ActChgCalCty1stDay + 40)
#define ActChgHldPlc		(ActChgCalCty1stDay + 41)
#define ActChgHldTyp		(ActChgCalCty1stDay + 42)
#define ActChgHldStrDat		(ActChgCalCty1stDay + 43)
#define ActChgHldEndDat		(ActChgCalCty1stDay + 44)
#define ActRenHld		(ActChgCalCty1stDay + 45)

#define ActPrnCalIns		(ActChgCalCty1stDay + 46)
#define ActChgCalIns1stDay	(ActChgCalCty1stDay + 47)

/*****************************************************************************/
/********************************* Centre tab ********************************/
/*****************************************************************************/
// Actions in menu
#define ActCtrReqSch		(ActChgCalIns1stDay +  1)
#define ActSeeCtrInf		(ActChgCalIns1stDay +  2)
#define ActSeeDeg		(ActChgCalIns1stDay +  3)
#define ActSeeCalCtr		(ActChgCalIns1stDay +  4)

// Secondary actions
#define ActCtrSch		(ActChgCalIns1stDay +  5)
#define ActPrnCtrInf		(ActChgCalIns1stDay +  6)
#define ActChgCtrInsCfg		(ActChgCalIns1stDay +  7)
#define ActRenCtrShoCfg		(ActChgCalIns1stDay +  8)
#define ActRenCtrFulCfg		(ActChgCalIns1stDay +  9)
#define ActChgCtrPlcCfg		(ActChgCalIns1stDay + 10)
#define ActChgCtrWWWCfg		(ActChgCalIns1stDay + 11)
#define ActReqCtrLog		(ActChgCalIns1stDay + 12)
#define ActRecCtrLog		(ActChgCalIns1stDay + 13)
#define ActRemCtrLog		(ActChgCalIns1stDay + 14)
#define ActReqCtrPho		(ActChgCalIns1stDay + 15)
#define ActRecCtrPho		(ActChgCalIns1stDay + 16)
#define ActChgCtrPhoAtt		(ActChgCalIns1stDay + 17)

#define ActSeeDegTyp		(ActChgCalIns1stDay + 18)
#define ActEdiDegTyp		(ActChgCalIns1stDay + 19)
#define ActNewDegTyp		(ActChgCalIns1stDay + 20)
#define ActRemDegTyp		(ActChgCalIns1stDay + 21)
#define ActRenDegTyp		(ActChgCalIns1stDay + 22)

#define ActEdiDeg		(ActChgCalIns1stDay + 23)
#define ActReqDeg		(ActChgCalIns1stDay + 24)
#define ActNewDeg		(ActChgCalIns1stDay + 25)
#define ActRemDeg		(ActChgCalIns1stDay + 26)
#define ActRenDegSho		(ActChgCalIns1stDay + 27)
#define ActRenDegFul		(ActChgCalIns1stDay + 28)
#define ActChgDegTyp		(ActChgCalIns1stDay + 29)
#define ActChgDegWWW		(ActChgCalIns1stDay + 30)
#define ActChgDegSta		(ActChgCalIns1stDay + 31)

#define ActPrnCalCtr		(ActChgCalIns1stDay + 32)
#define ActChgCalCtr1stDay	(ActChgCalIns1stDay + 33)

/*****************************************************************************/
/********************************* Degree tab ********************************/
/*****************************************************************************/
// Actions in menu
#define ActDegReqSch		(ActChgCalCtr1stDay +  1)
#define ActSeeDegInf		(ActChgCalCtr1stDay +  2)
#define ActSeeCrs		(ActChgCalCtr1stDay +  3)
#define ActSeeCalDeg		(ActChgCalCtr1stDay +  4)

// Secondary actions
#define ActDegSch		(ActChgCalCtr1stDay +  5)
#define ActPrnDegInf		(ActChgCalCtr1stDay +  6)
#define ActChgDegCtrCfg		(ActChgCalCtr1stDay +  7)
#define ActRenDegShoCfg		(ActChgCalCtr1stDay +  8)
#define ActRenDegFulCfg		(ActChgCalCtr1stDay +  9)
#define ActChgDegWWWCfg		(ActChgCalCtr1stDay + 10)
#define ActReqDegLog		(ActChgCalCtr1stDay + 11)
#define ActRecDegLog		(ActChgCalCtr1stDay + 12)
#define ActRemDegLog		(ActChgCalCtr1stDay + 13)

#define ActEdiCrs		(ActChgCalCtr1stDay + 14)
#define ActReqCrs		(ActChgCalCtr1stDay + 15)
#define ActNewCrs		(ActChgCalCtr1stDay + 16)
#define ActRemCrs		(ActChgCalCtr1stDay + 17)
#define ActChgInsCrsCod		(ActChgCalCtr1stDay + 18)
#define ActChgCrsYea		(ActChgCalCtr1stDay + 19)
#define ActRenCrsSho		(ActChgCalCtr1stDay + 20)
#define ActRenCrsFul		(ActChgCalCtr1stDay + 21)
#define ActChgCrsSta		(ActChgCalCtr1stDay + 22)

#define ActPrnCalDeg		(ActChgCalCtr1stDay + 23)
#define ActChgCalDeg1stDay	(ActChgCalCtr1stDay + 24)

/*****************************************************************************/
/******************************** Course tab *********************************/
/*****************************************************************************/
// Actions in menu
#define ActCrsReqSch		(ActChgCalDeg1stDay +   1)
#define ActSeeCrsInf		(ActChgCalDeg1stDay +   2)
#define ActSeeTchGui		(ActChgCalDeg1stDay +   3)
#define ActSeeSyl		(ActChgCalDeg1stDay +   4)
#define ActSeeBib		(ActChgCalDeg1stDay +   5)
#define ActSeeFAQ		(ActChgCalDeg1stDay +   6)
#define ActSeeCrsLnk		(ActChgCalDeg1stDay +   7)
#define ActSeeCrsTT		(ActChgCalDeg1stDay +   8)
#define ActSeeCalCrs		(ActChgCalDeg1stDay +   9)

// Secondary actions
#define ActCrsSch		(ActChgCalDeg1stDay +  10)
#define ActPrnCrsInf		(ActChgCalDeg1stDay +  11)
#define ActChgCrsDegCfg		(ActChgCalDeg1stDay +  12)
#define ActRenCrsShoCfg		(ActChgCalDeg1stDay +  13)
#define ActRenCrsFulCfg		(ActChgCalDeg1stDay +  14)
#define ActChgInsCrsCodCfg	(ActChgCalDeg1stDay +  15)
#define ActChgCrsYeaCfg		(ActChgCalDeg1stDay +  16)
#define ActEdiCrsInf		(ActChgCalDeg1stDay +  17)
#define ActEdiTchGui		(ActChgCalDeg1stDay +  18)
#define ActSeeSylLec		(ActChgCalDeg1stDay +  19)
#define ActSeeSylPra		(ActChgCalDeg1stDay +  20)
#define ActEdiSylLec		(ActChgCalDeg1stDay +  21)
#define ActEdiSylPra		(ActChgCalDeg1stDay +  22)
#define ActDelItmSylLec		(ActChgCalDeg1stDay +  23)
#define ActDelItmSylPra		(ActChgCalDeg1stDay +  24)
#define ActUp_IteSylLec		(ActChgCalDeg1stDay +  25)
#define ActUp_IteSylPra		(ActChgCalDeg1stDay +  26)
#define ActDwnIteSylLec		(ActChgCalDeg1stDay +  27)
#define ActDwnIteSylPra		(ActChgCalDeg1stDay +  28)
#define ActRgtIteSylLec		(ActChgCalDeg1stDay +  29)
#define ActRgtIteSylPra		(ActChgCalDeg1stDay +  30)
#define ActLftIteSylLec		(ActChgCalDeg1stDay +  31)
#define ActLftIteSylPra		(ActChgCalDeg1stDay +  32)
#define ActInsIteSylLec		(ActChgCalDeg1stDay +  33)
#define ActInsIteSylPra		(ActChgCalDeg1stDay +  34)
#define ActModIteSylLec		(ActChgCalDeg1stDay +  35)
#define ActModIteSylPra		(ActChgCalDeg1stDay +  36)

#define ActEdiBib		(ActChgCalDeg1stDay +  37)
#define ActEdiFAQ		(ActChgCalDeg1stDay +  38)
#define ActEdiCrsLnk		(ActChgCalDeg1stDay +  39)

#define ActChgFrcReaCrsInf	(ActChgCalDeg1stDay +  40)
#define ActChgFrcReaTchGui	(ActChgCalDeg1stDay +  41)
#define ActChgFrcReaSylLec	(ActChgCalDeg1stDay +  42)
#define ActChgFrcReaSylPra	(ActChgCalDeg1stDay +  43)
#define ActChgFrcReaBib		(ActChgCalDeg1stDay +  44)
#define ActChgFrcReaFAQ		(ActChgCalDeg1stDay +  45)
#define ActChgFrcReaCrsLnk	(ActChgCalDeg1stDay +  46)

#define ActChgHavReaCrsInf	(ActChgCalDeg1stDay +  47)
#define ActChgHavReaTchGui	(ActChgCalDeg1stDay +  48)
#define ActChgHavReaSylLec	(ActChgCalDeg1stDay +  49)
#define ActChgHavReaSylPra	(ActChgCalDeg1stDay +  50)
#define ActChgHavReaBib		(ActChgCalDeg1stDay +  51)
#define ActChgHavReaFAQ		(ActChgCalDeg1stDay +  52)
#define ActChgHavReaCrsLnk	(ActChgCalDeg1stDay +  53)

#define ActSelInfSrcCrsInf	(ActChgCalDeg1stDay +  54)
#define ActSelInfSrcTchGui	(ActChgCalDeg1stDay +  55)
#define ActSelInfSrcSylLec	(ActChgCalDeg1stDay +  56)
#define ActSelInfSrcSylPra	(ActChgCalDeg1stDay +  57)
#define ActSelInfSrcBib		(ActChgCalDeg1stDay +  58)
#define ActSelInfSrcFAQ		(ActChgCalDeg1stDay +  59)
#define ActSelInfSrcCrsLnk	(ActChgCalDeg1stDay +  60)
#define ActRcvURLCrsInf		(ActChgCalDeg1stDay +  61)
#define ActRcvURLTchGui		(ActChgCalDeg1stDay +  62)
#define ActRcvURLSylLec		(ActChgCalDeg1stDay +  63)
#define ActRcvURLSylPra		(ActChgCalDeg1stDay +  64)
#define ActRcvURLBib		(ActChgCalDeg1stDay +  65)
#define ActRcvURLFAQ		(ActChgCalDeg1stDay +  66)
#define ActRcvURLCrsLnk		(ActChgCalDeg1stDay +  67)
#define ActRcvPagCrsInf		(ActChgCalDeg1stDay +  68)
#define ActRcvPagTchGui		(ActChgCalDeg1stDay +  69)
#define ActRcvPagSylLec		(ActChgCalDeg1stDay +  70)
#define ActRcvPagSylPra		(ActChgCalDeg1stDay +  71)
#define ActRcvPagBib		(ActChgCalDeg1stDay +  72)
#define ActRcvPagFAQ		(ActChgCalDeg1stDay +  73)
#define ActRcvPagCrsLnk		(ActChgCalDeg1stDay +  74)
#define ActEditorCrsInf		(ActChgCalDeg1stDay +  75)
#define ActEditorTchGui		(ActChgCalDeg1stDay +  76)
#define ActEditorSylLec		(ActChgCalDeg1stDay +  77)
#define ActEditorSylPra		(ActChgCalDeg1stDay +  78)
#define ActEditorBib		(ActChgCalDeg1stDay +  79)
#define ActEditorFAQ		(ActChgCalDeg1stDay +  80)
#define ActEditorCrsLnk		(ActChgCalDeg1stDay +  81)
#define ActPlaTxtEdiCrsInf	(ActChgCalDeg1stDay +  82)
#define ActPlaTxtEdiTchGui	(ActChgCalDeg1stDay +  83)
#define ActPlaTxtEdiSylLec	(ActChgCalDeg1stDay +  84)
#define ActPlaTxtEdiSylPra	(ActChgCalDeg1stDay +  85)
#define ActPlaTxtEdiBib		(ActChgCalDeg1stDay +  86)
#define ActPlaTxtEdiFAQ		(ActChgCalDeg1stDay +  87)
#define ActPlaTxtEdiCrsLnk	(ActChgCalDeg1stDay +  88)
#define ActRchTxtEdiCrsInf	(ActChgCalDeg1stDay +  89)
#define ActRchTxtEdiTchGui	(ActChgCalDeg1stDay +  90)
#define ActRchTxtEdiSylLec	(ActChgCalDeg1stDay +  91)
#define ActRchTxtEdiSylPra	(ActChgCalDeg1stDay +  92)
#define ActRchTxtEdiBib		(ActChgCalDeg1stDay +  93)
#define ActRchTxtEdiFAQ		(ActChgCalDeg1stDay +  94)
#define ActRchTxtEdiCrsLnk	(ActChgCalDeg1stDay +  95)
#define ActRcvPlaTxtCrsInf	(ActChgCalDeg1stDay +  96)
#define ActRcvPlaTxtTchGui	(ActChgCalDeg1stDay +  97)
#define ActRcvPlaTxtSylLec	(ActChgCalDeg1stDay +  98)
#define ActRcvPlaTxtSylPra	(ActChgCalDeg1stDay +  99)
#define ActRcvPlaTxtBib		(ActChgCalDeg1stDay + 100)
#define ActRcvPlaTxtFAQ		(ActChgCalDeg1stDay + 101)
#define ActRcvPlaTxtCrsLnk	(ActChgCalDeg1stDay + 102)
#define ActRcvRchTxtCrsInf	(ActChgCalDeg1stDay + 103)
#define ActRcvRchTxtTchGui	(ActChgCalDeg1stDay + 104)
#define ActRcvRchTxtSylLec	(ActChgCalDeg1stDay + 105)
#define ActRcvRchTxtSylPra	(ActChgCalDeg1stDay + 106)
#define ActRcvRchTxtBib		(ActChgCalDeg1stDay + 107)
#define ActRcvRchTxtFAQ		(ActChgCalDeg1stDay + 108)
#define ActRcvRchTxtCrsLnk	(ActChgCalDeg1stDay + 109)

#define ActPrnCrsTT		(ActChgCalDeg1stDay + 110)
#define ActEdiCrsTT		(ActChgCalDeg1stDay + 111)
#define ActChgCrsTT		(ActChgCalDeg1stDay + 112)
#define ActChgCrsTT1stDay	(ActChgCalDeg1stDay + 113)

#define ActPrnCalCrs		(ActChgCalDeg1stDay + 114)
#define ActChgCalCrs1stDay	(ActChgCalDeg1stDay + 115)

/*****************************************************************************/
/***************************** Assessment tab ********************************/
/*****************************************************************************/
// Actions in menu
#define ActSeeAss		(ActChgCalCrs1stDay +   1)
#define ActSeeAsg		(ActChgCalCrs1stDay +   2)
#define ActSeePrj		(ActChgCalCrs1stDay +   3)
#define ActReqTst		(ActChgCalCrs1stDay +   4)
#define ActSeeAllGam		(ActChgCalCrs1stDay +   5)
#define ActSeeAllSvy		(ActChgCalCrs1stDay +   6)
#define ActSeeAllExaAnn		(ActChgCalCrs1stDay +   7)

// Secondary actions
#define ActEdiAss		(ActChgCalCrs1stDay +   8)
#define ActChgFrcReaAss		(ActChgCalCrs1stDay +   9)
#define ActChgHavReaAss		(ActChgCalCrs1stDay +  10)
#define ActSelInfSrcAss		(ActChgCalCrs1stDay +  11)
#define ActRcvURLAss		(ActChgCalCrs1stDay +  12)
#define ActRcvPagAss		(ActChgCalCrs1stDay +  13)
#define ActEditorAss		(ActChgCalCrs1stDay +  14)
#define ActPlaTxtEdiAss		(ActChgCalCrs1stDay +  15)
#define ActRchTxtEdiAss		(ActChgCalCrs1stDay +  16)
#define ActRcvPlaTxtAss		(ActChgCalCrs1stDay +  17)
#define ActRcvRchTxtAss		(ActChgCalCrs1stDay +  18)

#define ActFrmNewAsg		(ActChgCalCrs1stDay +  19)
#define ActEdiOneAsg		(ActChgCalCrs1stDay +  20)
#define ActPrnOneAsg		(ActChgCalCrs1stDay +  21)
#define ActNewAsg		(ActChgCalCrs1stDay +  22)
#define ActChgAsg		(ActChgCalCrs1stDay +  23)
#define ActReqRemAsg		(ActChgCalCrs1stDay +  24)
#define ActRemAsg		(ActChgCalCrs1stDay +  25)
#define ActHidAsg		(ActChgCalCrs1stDay +  26)
#define ActShoAsg		(ActChgCalCrs1stDay +  27)

#define ActSeeTblAllPrj		(ActChgCalCrs1stDay +  28)
#define ActFrmNewPrj		(ActChgCalCrs1stDay +  29)
#define ActEdiOnePrj		(ActChgCalCrs1stDay +  30)
#define ActPrnOnePrj		(ActChgCalCrs1stDay +  31)
#define ActNewPrj		(ActChgCalCrs1stDay +  32)
#define ActChgPrj		(ActChgCalCrs1stDay +  33)
#define ActReqRemPrj		(ActChgCalCrs1stDay +  34)
#define ActRemPrj		(ActChgCalCrs1stDay +  35)
#define ActHidPrj		(ActChgCalCrs1stDay +  36)
#define ActShoPrj		(ActChgCalCrs1stDay +  37)
#define ActReqAddStdPrj		(ActChgCalCrs1stDay +  38)
#define ActReqAddTutPrj		(ActChgCalCrs1stDay +  39)
#define ActReqAddEvaPrj		(ActChgCalCrs1stDay +  40)
#define ActAddStdPrj		(ActChgCalCrs1stDay +  41)
#define ActAddTutPrj		(ActChgCalCrs1stDay +  42)
#define ActAddEvaPrj		(ActChgCalCrs1stDay +  43)
#define ActReqRemStdPrj		(ActChgCalCrs1stDay +  44)
#define ActReqRemTutPrj		(ActChgCalCrs1stDay +  45)
#define ActReqRemEvaPrj		(ActChgCalCrs1stDay +  46)
#define ActRemStdPrj		(ActChgCalCrs1stDay +  47)
#define ActRemTutPrj		(ActChgCalCrs1stDay +  48)
#define ActRemEvaPrj		(ActChgCalCrs1stDay +  49)

#define ActAdmDocPrj		(ActChgCalCrs1stDay +  50)
#define ActReqRemFilDocPrj	(ActChgCalCrs1stDay +  51)
#define ActRemFilDocPrj		(ActChgCalCrs1stDay +  52)
#define ActRemFolDocPrj		(ActChgCalCrs1stDay +  53)
#define ActCopDocPrj		(ActChgCalCrs1stDay +  54)
#define ActPasDocPrj		(ActChgCalCrs1stDay +  55)
#define ActRemTreDocPrj		(ActChgCalCrs1stDay +  56)
#define ActFrmCreDocPrj		(ActChgCalCrs1stDay +  57)
#define ActCreFolDocPrj		(ActChgCalCrs1stDay +  58)
#define ActCreLnkDocPrj		(ActChgCalCrs1stDay +  59)
#define ActRenFolDocPrj		(ActChgCalCrs1stDay +  60)
#define ActRcvFilDocPrjDZ	(ActChgCalCrs1stDay +  61)
#define ActRcvFilDocPrjCla	(ActChgCalCrs1stDay +  62)
#define ActExpDocPrj		(ActChgCalCrs1stDay +  63)
#define ActConDocPrj		(ActChgCalCrs1stDay +  64)
#define ActZIPDocPrj		(ActChgCalCrs1stDay +  65)
#define ActReqDatDocPrj		(ActChgCalCrs1stDay +  66)
#define ActChgDatDocPrj		(ActChgCalCrs1stDay +  67)
#define ActDowDocPrj		(ActChgCalCrs1stDay +  68)

#define ActAdmAssPrj		(ActChgCalCrs1stDay +  69)
#define ActReqRemFilAssPrj	(ActChgCalCrs1stDay +  70)
#define ActRemFilAssPrj		(ActChgCalCrs1stDay +  71)
#define ActRemFolAssPrj		(ActChgCalCrs1stDay +  72)
#define ActCopAssPrj		(ActChgCalCrs1stDay +  73)
#define ActPasAssPrj		(ActChgCalCrs1stDay +  74)
#define ActRemTreAssPrj		(ActChgCalCrs1stDay +  75)
#define ActFrmCreAssPrj		(ActChgCalCrs1stDay +  76)
#define ActCreFolAssPrj		(ActChgCalCrs1stDay +  77)
#define ActCreLnkAssPrj		(ActChgCalCrs1stDay +  78)
#define ActRenFolAssPrj		(ActChgCalCrs1stDay +  79)
#define ActRcvFilAssPrjDZ	(ActChgCalCrs1stDay +  80)
#define ActRcvFilAssPrjCla	(ActChgCalCrs1stDay +  81)
#define ActExpAssPrj		(ActChgCalCrs1stDay +  82)
#define ActConAssPrj		(ActChgCalCrs1stDay +  83)
#define ActZIPAssPrj		(ActChgCalCrs1stDay +  84)
#define ActReqDatAssPrj		(ActChgCalCrs1stDay +  85)
#define ActChgDatAssPrj		(ActChgCalCrs1stDay +  86)
#define ActDowAssPrj		(ActChgCalCrs1stDay +  87)

#define ActSeeTst		(ActChgCalCrs1stDay +  88)
#define ActAssTst		(ActChgCalCrs1stDay +  89)
#define ActEdiTstQst		(ActChgCalCrs1stDay +  90)
#define ActEdiOneTstQst		(ActChgCalCrs1stDay +  91)
#define ActReqImpTstQst		(ActChgCalCrs1stDay +  92)
#define ActImpTstQst		(ActChgCalCrs1stDay +  93)
#define ActLstTstQst		(ActChgCalCrs1stDay +  94)
#define ActRcvTstQst		(ActChgCalCrs1stDay +  95)
#define ActReqRemTstQst		(ActChgCalCrs1stDay +  96)
#define ActRemTstQst		(ActChgCalCrs1stDay +  97)
#define ActShfTstQst		(ActChgCalCrs1stDay +  98)
#define ActCfgTst		(ActChgCalCrs1stDay +  99)
#define ActEnableTag		(ActChgCalCrs1stDay + 100)
#define ActDisableTag		(ActChgCalCrs1stDay + 101)
#define ActRenTag		(ActChgCalCrs1stDay + 102)
#define ActRcvCfgTst		(ActChgCalCrs1stDay + 103)

#define ActReqSeeMyTstRes	(ActChgCalCrs1stDay + 104)
#define ActSeeMyTstRes		(ActChgCalCrs1stDay + 105)
#define ActSeeOneTstResMe	(ActChgCalCrs1stDay + 106)
#define ActReqSeeUsrTstRes	(ActChgCalCrs1stDay + 107)
#define ActSeeUsrTstRes		(ActChgCalCrs1stDay + 108)
#define ActSeeOneTstResOth	(ActChgCalCrs1stDay + 109)

#define ActSeeOneGam		(ActChgCalCrs1stDay + 110)
#define ActPlyGam		(ActChgCalCrs1stDay + 111)
#define ActPlyGam1stQst		(ActChgCalCrs1stDay + 112)
#define ActPlyGamNxtQst		(ActChgCalCrs1stDay + 113)
#define ActPlyGamAns		(ActChgCalCrs1stDay + 114)
#define ActAnsGam		(ActChgCalCrs1stDay + 115)
#define ActFrmNewGam		(ActChgCalCrs1stDay + 116)
#define ActEdiOneGam		(ActChgCalCrs1stDay + 117)
#define ActNewGam		(ActChgCalCrs1stDay + 118)
#define ActChgGam		(ActChgCalCrs1stDay + 119)
#define ActReqRemGam		(ActChgCalCrs1stDay + 120)
#define ActRemGam		(ActChgCalCrs1stDay + 121)
#define ActReqRstGam		(ActChgCalCrs1stDay + 122)
#define ActRstGam		(ActChgCalCrs1stDay + 123)
#define ActHidGam		(ActChgCalCrs1stDay + 124)
#define ActShoGam		(ActChgCalCrs1stDay + 125)
#define ActAddOneGamQst		(ActChgCalCrs1stDay + 126)
#define ActGamLstTstQst		(ActChgCalCrs1stDay + 127)
#define ActAddTstQstToGam	(ActChgCalCrs1stDay + 128)
#define ActReqRemGamQst		(ActChgCalCrs1stDay + 129)
#define ActRemGamQst		(ActChgCalCrs1stDay + 130)
#define ActUp_GamQst		(ActChgCalCrs1stDay + 131)
#define ActDwnGamQst		(ActChgCalCrs1stDay + 132)

#define ActSeeOneSvy		(ActChgCalCrs1stDay + 133)
#define ActAnsSvy		(ActChgCalCrs1stDay + 134)
#define ActFrmNewSvy		(ActChgCalCrs1stDay + 135)
#define ActEdiOneSvy		(ActChgCalCrs1stDay + 136)
#define ActNewSvy		(ActChgCalCrs1stDay + 137)
#define ActChgSvy		(ActChgCalCrs1stDay + 138)
#define ActReqRemSvy		(ActChgCalCrs1stDay + 139)
#define ActRemSvy		(ActChgCalCrs1stDay + 140)
#define ActReqRstSvy		(ActChgCalCrs1stDay + 141)
#define ActRstSvy		(ActChgCalCrs1stDay + 142)
#define ActHidSvy		(ActChgCalCrs1stDay + 143)
#define ActShoSvy		(ActChgCalCrs1stDay + 144)
#define ActEdiOneSvyQst		(ActChgCalCrs1stDay + 145)
#define ActRcvSvyQst		(ActChgCalCrs1stDay + 146)
#define ActReqRemSvyQst		(ActChgCalCrs1stDay + 147)
#define ActRemSvyQst		(ActChgCalCrs1stDay + 148)

#define ActSeeOneExaAnn		(ActChgCalCrs1stDay + 149)
#define ActSeeDatExaAnn		(ActChgCalCrs1stDay + 150)
#define ActEdiExaAnn		(ActChgCalCrs1stDay + 151)
#define ActRcvExaAnn		(ActChgCalCrs1stDay + 152)
#define ActPrnExaAnn		(ActChgCalCrs1stDay + 153)
#define ActReqRemExaAnn		(ActChgCalCrs1stDay + 154)
#define ActRemExaAnn		(ActChgCalCrs1stDay + 155)
#define ActHidExaAnn		(ActChgCalCrs1stDay + 156)
#define ActShoExaAnn		(ActChgCalCrs1stDay + 157)

/*****************************************************************************/
/******************************** Files tab **********************************/
/*****************************************************************************/
// Actions in menu
#define ActSeeAdmDocIns		(ActShoExaAnn +   1)
#define ActAdmShaIns		(ActShoExaAnn +   2)
#define ActSeeAdmDocCtr		(ActShoExaAnn +   3)
#define ActAdmShaCtr		(ActShoExaAnn +   4)
#define ActSeeAdmDocDeg		(ActShoExaAnn +   5)
#define ActAdmShaDeg		(ActShoExaAnn +   6)
#define ActSeeAdmDocCrsGrp	(ActShoExaAnn +   7)
#define ActAdmTchCrsGrp		(ActShoExaAnn +   8)
#define ActAdmShaCrsGrp		(ActShoExaAnn +   9)
#define ActAdmAsgWrkUsr		(ActShoExaAnn +  10)
#define ActReqAsgWrkCrs		(ActShoExaAnn +  11)
#define ActSeeAdmMrk		(ActShoExaAnn +  12)
#define ActAdmBrf		(ActShoExaAnn +  13)
// Secondary actions
#define ActChgToSeeDocIns	(ActShoExaAnn +  14)
#define ActSeeDocIns		(ActShoExaAnn +  15)
#define ActExpSeeDocIns		(ActShoExaAnn +  16)
#define ActConSeeDocIns		(ActShoExaAnn +  17)
#define ActZIPSeeDocIns		(ActShoExaAnn +  18)
#define ActReqDatSeeDocIns	(ActShoExaAnn +  19)
#define ActDowSeeDocIns		(ActShoExaAnn +  20)
#define ActChgToAdmDocIns	(ActShoExaAnn +  21)
#define ActAdmDocIns		(ActShoExaAnn +  22)
#define ActReqRemFilDocIns	(ActShoExaAnn +  23)
#define ActRemFilDocIns		(ActShoExaAnn +  24)
#define ActRemFolDocIns		(ActShoExaAnn +  25)
#define ActCopDocIns		(ActShoExaAnn +  26)
#define ActPasDocIns		(ActShoExaAnn +  27)
#define ActRemTreDocIns		(ActShoExaAnn +  28)
#define ActFrmCreDocIns		(ActShoExaAnn +  29)
#define ActCreFolDocIns		(ActShoExaAnn +  30)
#define ActCreLnkDocIns		(ActShoExaAnn +  31)
#define ActRenFolDocIns		(ActShoExaAnn +  32)
#define ActRcvFilDocInsDZ	(ActShoExaAnn +  33)
#define ActRcvFilDocInsCla	(ActShoExaAnn +  34)
#define ActExpAdmDocIns		(ActShoExaAnn +  35)
#define ActConAdmDocIns		(ActShoExaAnn +  36)
#define ActZIPAdmDocIns		(ActShoExaAnn +  37)
#define ActShoDocIns		(ActShoExaAnn +  38)
#define ActHidDocIns		(ActShoExaAnn +  39)
#define ActReqDatAdmDocIns	(ActShoExaAnn +  40)
#define ActChgDatAdmDocIns	(ActShoExaAnn +  41)
#define ActDowAdmDocIns		(ActShoExaAnn +  42)

#define ActReqRemFilShaIns	(ActShoExaAnn +  43)
#define ActRemFilShaIns		(ActShoExaAnn +  44)
#define ActRemFolShaIns		(ActShoExaAnn +  45)
#define ActCopShaIns		(ActShoExaAnn +  46)
#define ActPasShaIns		(ActShoExaAnn +  47)
#define ActRemTreShaIns		(ActShoExaAnn +  48)
#define ActFrmCreShaIns		(ActShoExaAnn +  49)
#define ActCreFolShaIns		(ActShoExaAnn +  50)
#define ActCreLnkShaIns		(ActShoExaAnn +  51)
#define ActRenFolShaIns		(ActShoExaAnn +  52)
#define ActRcvFilShaInsDZ	(ActShoExaAnn +  53)
#define ActRcvFilShaInsCla	(ActShoExaAnn +  54)
#define ActExpShaIns		(ActShoExaAnn +  55)
#define ActConShaIns		(ActShoExaAnn +  56)
#define ActZIPShaIns		(ActShoExaAnn +  57)
#define ActReqDatShaIns		(ActShoExaAnn +  58)
#define ActChgDatShaIns		(ActShoExaAnn +  59)
#define ActDowShaIns		(ActShoExaAnn +  60)

#define ActChgToSeeDocCtr	(ActShoExaAnn +  61)
#define ActSeeDocCtr		(ActShoExaAnn +  62)
#define ActExpSeeDocCtr		(ActShoExaAnn +  63)
#define ActConSeeDocCtr		(ActShoExaAnn +  64)
#define ActZIPSeeDocCtr		(ActShoExaAnn +  65)
#define ActReqDatSeeDocCtr	(ActShoExaAnn +  66)
#define ActDowSeeDocCtr		(ActShoExaAnn +  67)
#define ActChgToAdmDocCtr	(ActShoExaAnn +  68)
#define ActAdmDocCtr		(ActShoExaAnn +  69)
#define ActReqRemFilDocCtr	(ActShoExaAnn +  70)
#define ActRemFilDocCtr		(ActShoExaAnn +  71)
#define ActRemFolDocCtr		(ActShoExaAnn +  72)
#define ActCopDocCtr		(ActShoExaAnn +  73)
#define ActPasDocCtr		(ActShoExaAnn +  74)
#define ActRemTreDocCtr		(ActShoExaAnn +  75)
#define ActFrmCreDocCtr		(ActShoExaAnn +  76)
#define ActCreFolDocCtr		(ActShoExaAnn +  77)
#define ActCreLnkDocCtr		(ActShoExaAnn +  78)
#define ActRenFolDocCtr		(ActShoExaAnn +  79)
#define ActRcvFilDocCtrDZ	(ActShoExaAnn +  80)
#define ActRcvFilDocCtrCla	(ActShoExaAnn +  81)
#define ActExpAdmDocCtr		(ActShoExaAnn +  82)
#define ActConAdmDocCtr		(ActShoExaAnn +  83)
#define ActZIPAdmDocCtr		(ActShoExaAnn +  84)
#define ActShoDocCtr		(ActShoExaAnn +  85)
#define ActHidDocCtr		(ActShoExaAnn +  86)
#define ActReqDatAdmDocCtr	(ActShoExaAnn +  87)
#define ActChgDatAdmDocCtr	(ActShoExaAnn +  88)
#define ActDowAdmDocCtr		(ActShoExaAnn +  89)

#define ActReqRemFilShaCtr	(ActShoExaAnn +  90)
#define ActRemFilShaCtr		(ActShoExaAnn +  91)
#define ActRemFolShaCtr		(ActShoExaAnn +  92)
#define ActCopShaCtr		(ActShoExaAnn +  93)
#define ActPasShaCtr		(ActShoExaAnn +  94)
#define ActRemTreShaCtr		(ActShoExaAnn +  95)
#define ActFrmCreShaCtr		(ActShoExaAnn +  96)
#define ActCreFolShaCtr		(ActShoExaAnn +  97)
#define ActCreLnkShaCtr		(ActShoExaAnn +  98)
#define ActRenFolShaCtr		(ActShoExaAnn +  99)
#define ActRcvFilShaCtrDZ	(ActShoExaAnn + 100)
#define ActRcvFilShaCtrCla	(ActShoExaAnn + 101)
#define ActExpShaCtr		(ActShoExaAnn + 102)
#define ActConShaCtr		(ActShoExaAnn + 103)
#define ActZIPShaCtr		(ActShoExaAnn + 104)
#define ActReqDatShaCtr		(ActShoExaAnn + 105)
#define ActChgDatShaCtr		(ActShoExaAnn + 106)
#define ActDowShaCtr		(ActShoExaAnn + 107)

#define ActChgToSeeDocDeg	(ActShoExaAnn + 108)
#define ActSeeDocDeg		(ActShoExaAnn + 109)
#define ActExpSeeDocDeg		(ActShoExaAnn + 110)
#define ActConSeeDocDeg		(ActShoExaAnn + 111)
#define ActZIPSeeDocDeg		(ActShoExaAnn + 112)
#define ActReqDatSeeDocDeg	(ActShoExaAnn + 113)
#define ActDowSeeDocDeg		(ActShoExaAnn + 114)
#define ActChgToAdmDocDeg	(ActShoExaAnn + 115)
#define ActAdmDocDeg		(ActShoExaAnn + 116)
#define ActReqRemFilDocDeg	(ActShoExaAnn + 117)
#define ActRemFilDocDeg		(ActShoExaAnn + 118)
#define ActRemFolDocDeg		(ActShoExaAnn + 119)
#define ActCopDocDeg		(ActShoExaAnn + 120)
#define ActPasDocDeg		(ActShoExaAnn + 121)
#define ActRemTreDocDeg		(ActShoExaAnn + 122)
#define ActFrmCreDocDeg		(ActShoExaAnn + 123)
#define ActCreFolDocDeg		(ActShoExaAnn + 124)
#define ActCreLnkDocDeg		(ActShoExaAnn + 125)
#define ActRenFolDocDeg		(ActShoExaAnn + 126)
#define ActRcvFilDocDegDZ	(ActShoExaAnn + 127)
#define ActRcvFilDocDegCla	(ActShoExaAnn + 128)
#define ActExpAdmDocDeg		(ActShoExaAnn + 129)
#define ActConAdmDocDeg		(ActShoExaAnn + 130)
#define ActZIPAdmDocDeg		(ActShoExaAnn + 131)
#define ActShoDocDeg		(ActShoExaAnn + 132)
#define ActHidDocDeg		(ActShoExaAnn + 133)
#define ActReqDatAdmDocDeg	(ActShoExaAnn + 134)
#define ActChgDatAdmDocDeg	(ActShoExaAnn + 135)
#define ActDowAdmDocDeg		(ActShoExaAnn + 136)

#define ActReqRemFilShaDeg	(ActShoExaAnn + 137)
#define ActRemFilShaDeg		(ActShoExaAnn + 138)
#define ActRemFolShaDeg		(ActShoExaAnn + 139)
#define ActCopShaDeg		(ActShoExaAnn + 140)
#define ActPasShaDeg		(ActShoExaAnn + 141)
#define ActRemTreShaDeg		(ActShoExaAnn + 142)
#define ActFrmCreShaDeg		(ActShoExaAnn + 143)
#define ActCreFolShaDeg		(ActShoExaAnn + 144)
#define ActCreLnkShaDeg		(ActShoExaAnn + 145)
#define ActRenFolShaDeg		(ActShoExaAnn + 146)
#define ActRcvFilShaDegDZ	(ActShoExaAnn + 147)
#define ActRcvFilShaDegCla	(ActShoExaAnn + 148)
#define ActExpShaDeg		(ActShoExaAnn + 149)
#define ActConShaDeg		(ActShoExaAnn + 150)
#define ActZIPShaDeg		(ActShoExaAnn + 151)
#define ActReqDatShaDeg		(ActShoExaAnn + 152)
#define ActChgDatShaDeg		(ActShoExaAnn + 153)
#define ActDowShaDeg		(ActShoExaAnn + 154)

#define ActChgToSeeDocCrs	(ActShoExaAnn + 155)
#define ActSeeDocCrs		(ActShoExaAnn + 156)
#define ActExpSeeDocCrs		(ActShoExaAnn + 157)
#define ActConSeeDocCrs		(ActShoExaAnn + 158)
#define ActZIPSeeDocCrs		(ActShoExaAnn + 159)
#define ActReqDatSeeDocCrs	(ActShoExaAnn + 160)
#define ActDowSeeDocCrs		(ActShoExaAnn + 161)

#define ActSeeDocGrp		(ActShoExaAnn + 162)
#define ActExpSeeDocGrp		(ActShoExaAnn + 163)
#define ActConSeeDocGrp		(ActShoExaAnn + 164)
#define ActZIPSeeDocGrp		(ActShoExaAnn + 165)
#define ActReqDatSeeDocGrp	(ActShoExaAnn + 166)
#define ActDowSeeDocGrp		(ActShoExaAnn + 167)

#define ActChgToAdmDocCrs	(ActShoExaAnn + 168)
#define ActAdmDocCrs		(ActShoExaAnn + 169)
#define ActReqRemFilDocCrs	(ActShoExaAnn + 170)
#define ActRemFilDocCrs		(ActShoExaAnn + 171)
#define ActRemFolDocCrs		(ActShoExaAnn + 172)
#define ActCopDocCrs		(ActShoExaAnn + 173)
#define ActPasDocCrs		(ActShoExaAnn + 174)
#define ActRemTreDocCrs		(ActShoExaAnn + 175)
#define ActFrmCreDocCrs		(ActShoExaAnn + 176)
#define ActCreFolDocCrs		(ActShoExaAnn + 177)
#define ActCreLnkDocCrs		(ActShoExaAnn + 178)
#define ActRenFolDocCrs		(ActShoExaAnn + 179)
#define ActRcvFilDocCrsDZ	(ActShoExaAnn + 180)
#define ActRcvFilDocCrsCla	(ActShoExaAnn + 181)
#define ActExpAdmDocCrs		(ActShoExaAnn + 182)
#define ActConAdmDocCrs		(ActShoExaAnn + 183)
#define ActZIPAdmDocCrs		(ActShoExaAnn + 184)
#define ActShoDocCrs		(ActShoExaAnn + 185)
#define ActHidDocCrs		(ActShoExaAnn + 186)
#define ActReqDatAdmDocCrs	(ActShoExaAnn + 187)
#define ActChgDatAdmDocCrs	(ActShoExaAnn + 188)
#define ActDowAdmDocCrs		(ActShoExaAnn + 189)

#define ActAdmDocGrp		(ActShoExaAnn + 190)
#define ActReqRemFilDocGrp	(ActShoExaAnn + 191)
#define ActRemFilDocGrp		(ActShoExaAnn + 192)
#define ActRemFolDocGrp		(ActShoExaAnn + 193)
#define ActCopDocGrp		(ActShoExaAnn + 194)
#define ActPasDocGrp		(ActShoExaAnn + 195)
#define ActRemTreDocGrp		(ActShoExaAnn + 196)
#define ActFrmCreDocGrp		(ActShoExaAnn + 197)
#define ActCreFolDocGrp		(ActShoExaAnn + 198)
#define ActCreLnkDocGrp		(ActShoExaAnn + 199)
#define ActRenFolDocGrp		(ActShoExaAnn + 200)
#define ActRcvFilDocGrpDZ	(ActShoExaAnn + 201)
#define ActRcvFilDocGrpCla	(ActShoExaAnn + 202)
#define ActExpAdmDocGrp		(ActShoExaAnn + 203)
#define ActConAdmDocGrp		(ActShoExaAnn + 204)
#define ActZIPAdmDocGrp		(ActShoExaAnn + 205)
#define ActShoDocGrp		(ActShoExaAnn + 206)
#define ActHidDocGrp		(ActShoExaAnn + 207)
#define ActReqDatAdmDocGrp	(ActShoExaAnn + 208)
#define ActChgDatAdmDocGrp	(ActShoExaAnn + 209)
#define ActDowAdmDocGrp		(ActShoExaAnn + 210)

#define ActChgToAdmTch		(ActShoExaAnn + 211)

#define ActAdmTchCrs		(ActShoExaAnn + 212)
#define ActReqRemFilTchCrs	(ActShoExaAnn + 213)
#define ActRemFilTchCrs		(ActShoExaAnn + 214)
#define ActRemFolTchCrs		(ActShoExaAnn + 215)
#define ActCopTchCrs		(ActShoExaAnn + 216)
#define ActPasTchCrs		(ActShoExaAnn + 217)
#define ActRemTreTchCrs		(ActShoExaAnn + 218)
#define ActFrmCreTchCrs		(ActShoExaAnn + 219)
#define ActCreFolTchCrs		(ActShoExaAnn + 220)
#define ActCreLnkTchCrs		(ActShoExaAnn + 221)
#define ActRenFolTchCrs		(ActShoExaAnn + 222)
#define ActRcvFilTchCrsDZ	(ActShoExaAnn + 223)
#define ActRcvFilTchCrsCla	(ActShoExaAnn + 224)
#define ActExpTchCrs		(ActShoExaAnn + 225)
#define ActConTchCrs		(ActShoExaAnn + 226)
#define ActZIPTchCrs		(ActShoExaAnn + 227)
#define ActReqDatTchCrs		(ActShoExaAnn + 228)
#define ActChgDatTchCrs		(ActShoExaAnn + 229)
#define ActDowTchCrs		(ActShoExaAnn + 230)

#define ActAdmTchGrp		(ActShoExaAnn + 231)
#define ActReqRemFilTchGrp	(ActShoExaAnn + 232)
#define ActRemFilTchGrp		(ActShoExaAnn + 233)
#define ActRemFolTchGrp		(ActShoExaAnn + 234)
#define ActCopTchGrp		(ActShoExaAnn + 235)
#define ActPasTchGrp		(ActShoExaAnn + 236)
#define ActRemTreTchGrp		(ActShoExaAnn + 237)
#define ActFrmCreTchGrp		(ActShoExaAnn + 238)
#define ActCreFolTchGrp		(ActShoExaAnn + 239)
#define ActCreLnkTchGrp		(ActShoExaAnn + 240)
#define ActRenFolTchGrp		(ActShoExaAnn + 241)
#define ActRcvFilTchGrpDZ	(ActShoExaAnn + 242)
#define ActRcvFilTchGrpCla	(ActShoExaAnn + 243)
#define ActExpTchGrp		(ActShoExaAnn + 244)
#define ActConTchGrp		(ActShoExaAnn + 245)
#define ActZIPTchGrp		(ActShoExaAnn + 246)
#define ActReqDatTchGrp		(ActShoExaAnn + 247)
#define ActChgDatTchGrp		(ActShoExaAnn + 248)
#define ActDowTchGrp		(ActShoExaAnn + 249)

#define ActChgToAdmSha		(ActShoExaAnn + 250)

#define ActAdmShaCrs		(ActShoExaAnn + 251)
#define ActReqRemFilShaCrs	(ActShoExaAnn + 252)
#define ActRemFilShaCrs		(ActShoExaAnn + 253)
#define ActRemFolShaCrs		(ActShoExaAnn + 254)
#define ActCopShaCrs		(ActShoExaAnn + 255)
#define ActPasShaCrs		(ActShoExaAnn + 256)
#define ActRemTreShaCrs		(ActShoExaAnn + 257)
#define ActFrmCreShaCrs		(ActShoExaAnn + 258)
#define ActCreFolShaCrs		(ActShoExaAnn + 259)
#define ActCreLnkShaCrs		(ActShoExaAnn + 260)
#define ActRenFolShaCrs		(ActShoExaAnn + 261)
#define ActRcvFilShaCrsDZ	(ActShoExaAnn + 262)
#define ActRcvFilShaCrsCla	(ActShoExaAnn + 263)
#define ActExpShaCrs		(ActShoExaAnn + 264)
#define ActConShaCrs		(ActShoExaAnn + 265)
#define ActZIPShaCrs		(ActShoExaAnn + 266)
#define ActReqDatShaCrs		(ActShoExaAnn + 267)
#define ActChgDatShaCrs		(ActShoExaAnn + 268)
#define ActDowShaCrs		(ActShoExaAnn + 269)

#define ActAdmShaGrp		(ActShoExaAnn + 270)
#define ActReqRemFilShaGrp	(ActShoExaAnn + 271)
#define ActRemFilShaGrp		(ActShoExaAnn + 272)
#define ActRemFolShaGrp		(ActShoExaAnn + 273)
#define ActCopShaGrp		(ActShoExaAnn + 274)
#define ActPasShaGrp		(ActShoExaAnn + 275)
#define ActRemTreShaGrp		(ActShoExaAnn + 276)
#define ActFrmCreShaGrp		(ActShoExaAnn + 277)
#define ActCreFolShaGrp		(ActShoExaAnn + 278)
#define ActCreLnkShaGrp		(ActShoExaAnn + 279)
#define ActRenFolShaGrp		(ActShoExaAnn + 280)
#define ActRcvFilShaGrpDZ	(ActShoExaAnn + 281)
#define ActRcvFilShaGrpCla	(ActShoExaAnn + 282)
#define ActExpShaGrp		(ActShoExaAnn + 283)
#define ActConShaGrp		(ActShoExaAnn + 284)
#define ActZIPShaGrp		(ActShoExaAnn + 285)
#define ActReqDatShaGrp		(ActShoExaAnn + 286)
#define ActChgDatShaGrp		(ActShoExaAnn + 287)
#define ActDowShaGrp		(ActShoExaAnn + 288)

#define ActAdmAsgWrkCrs		(ActShoExaAnn + 289)

#define ActReqRemFilAsgUsr	(ActShoExaAnn + 290)
#define ActRemFilAsgUsr		(ActShoExaAnn + 291)
#define ActRemFolAsgUsr		(ActShoExaAnn + 292)
#define ActCopAsgUsr		(ActShoExaAnn + 293)
#define ActPasAsgUsr		(ActShoExaAnn + 294)
#define ActRemTreAsgUsr		(ActShoExaAnn + 295)
#define ActFrmCreAsgUsr		(ActShoExaAnn + 296)
#define ActCreFolAsgUsr		(ActShoExaAnn + 297)
#define ActCreLnkAsgUsr		(ActShoExaAnn + 298)
#define ActRenFolAsgUsr		(ActShoExaAnn + 299)
#define ActRcvFilAsgUsrDZ	(ActShoExaAnn + 300)
#define ActRcvFilAsgUsrCla	(ActShoExaAnn + 301)
#define ActExpAsgUsr		(ActShoExaAnn + 302)
#define ActConAsgUsr		(ActShoExaAnn + 303)
#define ActZIPAsgUsr		(ActShoExaAnn + 304)
#define ActReqDatAsgUsr		(ActShoExaAnn + 305)
#define ActChgDatAsgUsr		(ActShoExaAnn + 306)
#define ActDowAsgUsr		(ActShoExaAnn + 307)

#define ActReqRemFilWrkUsr	(ActShoExaAnn + 308)
#define ActRemFilWrkUsr		(ActShoExaAnn + 309)
#define ActRemFolWrkUsr		(ActShoExaAnn + 310)
#define ActCopWrkUsr		(ActShoExaAnn + 311)
#define ActPasWrkUsr		(ActShoExaAnn + 312)
#define ActRemTreWrkUsr		(ActShoExaAnn + 313)
#define ActFrmCreWrkUsr		(ActShoExaAnn + 314)
#define ActCreFolWrkUsr		(ActShoExaAnn + 315)
#define ActCreLnkWrkUsr		(ActShoExaAnn + 316)
#define ActRenFolWrkUsr		(ActShoExaAnn + 317)
#define ActRcvFilWrkUsrDZ	(ActShoExaAnn + 318)
#define ActRcvFilWrkUsrCla	(ActShoExaAnn + 319)
#define ActExpWrkUsr		(ActShoExaAnn + 320)
#define ActConWrkUsr		(ActShoExaAnn + 321)
#define ActZIPWrkUsr		(ActShoExaAnn + 322)
#define ActReqDatWrkUsr		(ActShoExaAnn + 323)
#define ActChgDatWrkUsr		(ActShoExaAnn + 324)
#define ActDowWrkUsr		(ActShoExaAnn + 325)

#define ActReqRemFilAsgCrs	(ActShoExaAnn + 326)
#define ActRemFilAsgCrs		(ActShoExaAnn + 327)
#define ActRemFolAsgCrs		(ActShoExaAnn + 328)
#define ActCopAsgCrs		(ActShoExaAnn + 329)
#define ActPasAsgCrs		(ActShoExaAnn + 330)
#define ActRemTreAsgCrs		(ActShoExaAnn + 331)
#define ActFrmCreAsgCrs		(ActShoExaAnn + 332)
#define ActCreFolAsgCrs		(ActShoExaAnn + 333)
#define ActCreLnkAsgCrs		(ActShoExaAnn + 334)
#define ActRenFolAsgCrs		(ActShoExaAnn + 335)
#define ActRcvFilAsgCrsDZ	(ActShoExaAnn + 336)
#define ActRcvFilAsgCrsCla	(ActShoExaAnn + 337)
#define ActExpAsgCrs		(ActShoExaAnn + 338)
#define ActConAsgCrs		(ActShoExaAnn + 339)
#define ActZIPAsgCrs		(ActShoExaAnn + 340)
#define ActReqDatAsgCrs		(ActShoExaAnn + 341)
#define ActChgDatAsgCrs		(ActShoExaAnn + 342)
#define ActDowAsgCrs		(ActShoExaAnn + 343)

#define ActReqRemFilWrkCrs	(ActShoExaAnn + 344)
#define ActRemFilWrkCrs		(ActShoExaAnn + 345)
#define ActRemFolWrkCrs		(ActShoExaAnn + 346)
#define ActCopWrkCrs		(ActShoExaAnn + 347)
#define ActPasWrkCrs		(ActShoExaAnn + 348)
#define ActRemTreWrkCrs		(ActShoExaAnn + 349)
#define ActFrmCreWrkCrs		(ActShoExaAnn + 350)
#define ActCreFolWrkCrs		(ActShoExaAnn + 351)
#define ActCreLnkWrkCrs		(ActShoExaAnn + 352)
#define ActRenFolWrkCrs		(ActShoExaAnn + 353)
#define ActRcvFilWrkCrsDZ	(ActShoExaAnn + 354)
#define ActRcvFilWrkCrsCla	(ActShoExaAnn + 355)
#define ActExpWrkCrs		(ActShoExaAnn + 356)
#define ActConWrkCrs		(ActShoExaAnn + 357)
#define ActZIPWrkCrs		(ActShoExaAnn + 358)
#define ActReqDatWrkCrs		(ActShoExaAnn + 359)
#define ActChgDatWrkCrs		(ActShoExaAnn + 360)
#define ActDowWrkCrs		(ActShoExaAnn + 361)

#define ActChgToSeeMrk		(ActShoExaAnn + 362)

#define ActSeeMrkCrs		(ActShoExaAnn + 363)
#define ActExpSeeMrkCrs		(ActShoExaAnn + 364)
#define ActConSeeMrkCrs		(ActShoExaAnn + 365)
#define ActReqDatSeeMrkCrs	(ActShoExaAnn + 366)
#define ActSeeMyMrkCrs		(ActShoExaAnn + 367)

#define ActSeeMrkGrp		(ActShoExaAnn + 368)
#define ActExpSeeMrkGrp		(ActShoExaAnn + 369)
#define ActConSeeMrkGrp		(ActShoExaAnn + 370)
#define ActReqDatSeeMrkGrp	(ActShoExaAnn + 371)
#define ActSeeMyMrkGrp		(ActShoExaAnn + 372)

#define ActChgToAdmMrk		(ActShoExaAnn + 373)

#define ActAdmMrkCrs		(ActShoExaAnn + 374)
#define ActReqRemFilMrkCrs	(ActShoExaAnn + 375)
#define ActRemFilMrkCrs		(ActShoExaAnn + 376)
#define ActRemFolMrkCrs		(ActShoExaAnn + 377)
#define ActCopMrkCrs		(ActShoExaAnn + 378)
#define ActPasMrkCrs		(ActShoExaAnn + 379)
#define ActRemTreMrkCrs		(ActShoExaAnn + 380)
#define ActFrmCreMrkCrs		(ActShoExaAnn + 381)
#define ActCreFolMrkCrs		(ActShoExaAnn + 382)
#define ActRenFolMrkCrs		(ActShoExaAnn + 383)
#define ActRcvFilMrkCrsDZ	(ActShoExaAnn + 384)
#define ActRcvFilMrkCrsCla	(ActShoExaAnn + 385)
#define ActExpAdmMrkCrs		(ActShoExaAnn + 386)
#define ActConAdmMrkCrs		(ActShoExaAnn + 387)
#define ActZIPAdmMrkCrs		(ActShoExaAnn + 388)
#define ActShoMrkCrs		(ActShoExaAnn + 389)
#define ActHidMrkCrs		(ActShoExaAnn + 390)
#define ActReqDatAdmMrkCrs	(ActShoExaAnn + 391)
#define ActChgDatAdmMrkCrs	(ActShoExaAnn + 392)
#define ActDowAdmMrkCrs		(ActShoExaAnn + 393)
#define ActChgNumRowHeaCrs	(ActShoExaAnn + 394)
#define ActChgNumRowFooCrs	(ActShoExaAnn + 395)

#define ActAdmMrkGrp		(ActShoExaAnn + 396)
#define ActReqRemFilMrkGrp	(ActShoExaAnn + 397)
#define ActRemFilMrkGrp		(ActShoExaAnn + 398)
#define ActRemFolMrkGrp		(ActShoExaAnn + 399)
#define ActCopMrkGrp		(ActShoExaAnn + 400)
#define ActPasMrkGrp		(ActShoExaAnn + 401)
#define ActRemTreMrkGrp		(ActShoExaAnn + 402)
#define ActFrmCreMrkGrp		(ActShoExaAnn + 403)
#define ActCreFolMrkGrp		(ActShoExaAnn + 404)
#define ActRenFolMrkGrp		(ActShoExaAnn + 405)
#define ActRcvFilMrkGrpDZ	(ActShoExaAnn + 406)
#define ActRcvFilMrkGrpCla	(ActShoExaAnn + 407)
#define ActExpAdmMrkGrp		(ActShoExaAnn + 408)
#define ActConAdmMrkGrp		(ActShoExaAnn + 409)
#define ActZIPAdmMrkGrp		(ActShoExaAnn + 410)
#define ActShoMrkGrp		(ActShoExaAnn + 411)
#define ActHidMrkGrp		(ActShoExaAnn + 412)
#define ActReqDatAdmMrkGrp	(ActShoExaAnn + 413)
#define ActChgDatAdmMrkGrp	(ActShoExaAnn + 414)
#define ActDowAdmMrkGrp		(ActShoExaAnn + 415)
#define ActChgNumRowHeaGrp	(ActShoExaAnn + 416)
#define ActChgNumRowFooGrp	(ActShoExaAnn + 417)

#define ActReqRemFilBrf		(ActShoExaAnn + 418)
#define ActRemFilBrf		(ActShoExaAnn + 419)
#define ActRemFolBrf		(ActShoExaAnn + 420)
#define ActCopBrf		(ActShoExaAnn + 421)
#define ActPasBrf		(ActShoExaAnn + 422)
#define ActRemTreBrf		(ActShoExaAnn + 423)
#define ActFrmCreBrf		(ActShoExaAnn + 424)
#define ActCreFolBrf		(ActShoExaAnn + 425)
#define ActCreLnkBrf		(ActShoExaAnn + 426)
#define ActRenFolBrf		(ActShoExaAnn + 427)
#define ActRcvFilBrfDZ		(ActShoExaAnn + 428)
#define ActRcvFilBrfCla		(ActShoExaAnn + 429)
#define ActExpBrf		(ActShoExaAnn + 430)
#define ActConBrf		(ActShoExaAnn + 431)
#define ActZIPBrf		(ActShoExaAnn + 432)
#define ActReqDatBrf		(ActShoExaAnn + 433)
#define ActChgDatBrf		(ActShoExaAnn + 434)
#define ActDowBrf		(ActShoExaAnn + 435)
#define ActReqRemOldBrf		(ActShoExaAnn + 436)
#define ActRemOldBrf		(ActShoExaAnn + 437)

/*****************************************************************************/
/******************************* Users tab ***********************************/
/*****************************************************************************/
// Actions in menu
#define ActReqSelGrp		(ActRemOldBrf +   1)
#define ActLstStd		(ActRemOldBrf +   2)
#define ActLstTch		(ActRemOldBrf +   3)
#define ActLstOth		(ActRemOldBrf +   4)
#define ActSeeAtt		(ActRemOldBrf +   5)
#define ActReqSignUp		(ActRemOldBrf +   6)
#define ActSeeSignUpReq		(ActRemOldBrf +   7)
#define ActLstCon		(ActRemOldBrf +   8)

// Secondary actions
#define ActChgGrp		(ActRemOldBrf +   9)
#define ActReqEdiGrp		(ActRemOldBrf +  10)
#define ActNewGrpTyp		(ActRemOldBrf +  11)
#define ActReqRemGrpTyp		(ActRemOldBrf +  12)
#define ActRemGrpTyp		(ActRemOldBrf +  13)
#define ActRenGrpTyp		(ActRemOldBrf +  14)
#define ActChgMdtGrpTyp		(ActRemOldBrf +  15)
#define ActChgMulGrpTyp		(ActRemOldBrf +  16)
#define ActChgTimGrpTyp		(ActRemOldBrf +  17)
#define ActNewGrp		(ActRemOldBrf +  18)
#define ActReqRemGrp		(ActRemOldBrf +  19)
#define ActRemGrp		(ActRemOldBrf +  20)
#define ActOpeGrp		(ActRemOldBrf +  21)
#define ActCloGrp		(ActRemOldBrf +  22)
#define ActEnaFilZonGrp		(ActRemOldBrf +  23)
#define ActDisFilZonGrp		(ActRemOldBrf +  24)
#define ActChgGrpTyp		(ActRemOldBrf +  25)
#define ActRenGrp		(ActRemOldBrf +  26)
#define ActChgMaxStdGrp		(ActRemOldBrf +  27)

#define ActLstGst		(ActRemOldBrf +  28)

#define ActPrnGstPho		(ActRemOldBrf +  29)
#define ActPrnStdPho		(ActRemOldBrf +  30)
#define ActPrnTchPho		(ActRemOldBrf +  31)
#define ActLstGstAll		(ActRemOldBrf +  32)
#define ActLstStdAll		(ActRemOldBrf +  33)
#define ActLstTchAll		(ActRemOldBrf +  34)

#define ActSeeRecOneStd		(ActRemOldBrf +  35)
#define ActSeeRecOneTch		(ActRemOldBrf +  36)
#define ActSeeRecSevGst		(ActRemOldBrf +  37)
#define ActSeeRecSevStd		(ActRemOldBrf +  38)
#define ActSeeRecSevTch		(ActRemOldBrf +  39)
#define ActPrnRecSevGst		(ActRemOldBrf +  40)
#define ActPrnRecSevStd		(ActRemOldBrf +  41)
#define ActPrnRecSevTch		(ActRemOldBrf +  42)
#define ActRcvRecOthUsr		(ActRemOldBrf +  43)
#define ActEdiRecFie		(ActRemOldBrf +  44)
#define ActNewFie		(ActRemOldBrf +  45)
#define ActReqRemFie		(ActRemOldBrf +  46)
#define ActRemFie		(ActRemOldBrf +  47)
#define ActRenFie		(ActRemOldBrf +  48)
#define ActChgRowFie		(ActRemOldBrf +  49)
#define ActChgVisFie		(ActRemOldBrf +  50)
#define ActRcvRecCrs		(ActRemOldBrf +  51)

#define ActFrmLogInUsrAgd	(ActRemOldBrf +  52)
#define ActLogInUsrAgd		(ActRemOldBrf +  53)
#define ActLogInUsrAgdLan	(ActRemOldBrf +  54)
#define ActSeeUsrAgd		(ActRemOldBrf +  55)

#define ActReqEnrSevStd		(ActRemOldBrf +  56)
#define ActReqEnrSevNET		(ActRemOldBrf +  57)
#define ActReqEnrSevTch		(ActRemOldBrf +  58)

#define ActReqLstStdAtt		(ActRemOldBrf +  59)
#define ActSeeLstMyAtt		(ActRemOldBrf +  60)
#define ActPrnLstMyAtt		(ActRemOldBrf +  61)
#define ActSeeLstStdAtt		(ActRemOldBrf +  62)
#define ActPrnLstStdAtt		(ActRemOldBrf +  63)
#define ActFrmNewAtt		(ActRemOldBrf +  64)
#define ActEdiOneAtt		(ActRemOldBrf +  65)
#define ActNewAtt		(ActRemOldBrf +  66)
#define ActChgAtt		(ActRemOldBrf +  67)
#define ActReqRemAtt		(ActRemOldBrf +  68)
#define ActRemAtt		(ActRemOldBrf +  69)
#define ActHidAtt		(ActRemOldBrf +  70)
#define ActShoAtt		(ActRemOldBrf +  71)
#define ActSeeOneAtt		(ActRemOldBrf +  72)
#define ActRecAttStd		(ActRemOldBrf +  73)
#define ActRecAttMe		(ActRemOldBrf +  74)

#define ActSignUp		(ActRemOldBrf +  75)
#define ActUpdSignUpReq		(ActRemOldBrf +  76)
#define ActReqRejSignUp		(ActRemOldBrf +  77)
#define ActRejSignUp		(ActRemOldBrf +  78)

#define ActReqMdfOneOth		(ActRemOldBrf +  79)
#define ActReqMdfOneStd		(ActRemOldBrf +  80)
#define ActReqMdfOneTch		(ActRemOldBrf +  81)

#define ActReqMdfOth		(ActRemOldBrf +  82)
#define ActReqMdfStd		(ActRemOldBrf +  83)
#define ActReqMdfNET		(ActRemOldBrf +  84)
#define ActReqMdfTch		(ActRemOldBrf +  85)

#define ActReqOthPho		(ActRemOldBrf +  86)
#define ActReqStdPho		(ActRemOldBrf +  87)
#define ActReqTchPho		(ActRemOldBrf +  88)
#define ActDetOthPho		(ActRemOldBrf +  89)
#define ActDetStdPho		(ActRemOldBrf +  90)
#define ActDetTchPho		(ActRemOldBrf +  91)
#define ActUpdOthPho		(ActRemOldBrf +  92)
#define ActUpdStdPho		(ActRemOldBrf +  93)
#define ActUpdTchPho		(ActRemOldBrf +  94)
#define ActReqRemOthPho		(ActRemOldBrf +  95)
#define ActReqRemStdPho		(ActRemOldBrf +  96)
#define ActReqRemTchPho		(ActRemOldBrf +  97)
#define ActRemOthPho		(ActRemOldBrf +  98)
#define ActRemStdPho		(ActRemOldBrf +  99)
#define ActRemTchPho		(ActRemOldBrf + 100)
#define ActCreOth		(ActRemOldBrf + 101)
#define ActCreStd		(ActRemOldBrf + 102)
#define ActCreNET		(ActRemOldBrf + 103)
#define ActCreTch		(ActRemOldBrf + 104)
#define ActUpdOth		(ActRemOldBrf + 105)
#define ActUpdStd		(ActRemOldBrf + 106)
#define ActUpdNET		(ActRemOldBrf + 107)
#define ActUpdTch		(ActRemOldBrf + 108)

#define ActReqAccEnrStd		(ActRemOldBrf + 109)
#define ActReqAccEnrNET		(ActRemOldBrf + 110)
#define ActReqAccEnrTch		(ActRemOldBrf + 111)
#define ActAccEnrStd		(ActRemOldBrf + 112)
#define ActAccEnrNET		(ActRemOldBrf + 113)
#define ActAccEnrTch		(ActRemOldBrf + 114)
#define ActRemMe_Std		(ActRemOldBrf + 115)
#define ActRemMe_NET		(ActRemOldBrf + 116)
#define ActRemMe_Tch		(ActRemOldBrf + 117)

#define ActNewAdmIns		(ActRemOldBrf + 118)
#define ActRemAdmIns		(ActRemOldBrf + 119)
#define ActNewAdmCtr		(ActRemOldBrf + 120)
#define ActRemAdmCtr		(ActRemOldBrf + 121)
#define ActNewAdmDeg		(ActRemOldBrf + 122)
#define ActRemAdmDeg		(ActRemOldBrf + 123)

#define ActRcvFrmEnrSevStd	(ActRemOldBrf + 124)
#define ActRcvFrmEnrSevNET	(ActRemOldBrf + 125)
#define ActRcvFrmEnrSevTch	(ActRemOldBrf + 126)

#define ActCnfID_Oth		(ActRemOldBrf + 127)
#define ActCnfID_Std		(ActRemOldBrf + 128)
#define ActCnfID_Tch		(ActRemOldBrf + 129)

#define ActFrmIDsOth		(ActRemOldBrf + 130)
#define ActFrmIDsStd		(ActRemOldBrf + 131)
#define ActFrmIDsTch		(ActRemOldBrf + 132)
#define ActRemID_Oth		(ActRemOldBrf + 133)
#define ActRemID_Std		(ActRemOldBrf + 134)
#define ActRemID_Tch		(ActRemOldBrf + 135)
#define ActNewID_Oth		(ActRemOldBrf + 136)
#define ActNewID_Std		(ActRemOldBrf + 137)
#define ActNewID_Tch		(ActRemOldBrf + 138)
#define ActFrmPwdOth		(ActRemOldBrf + 139)
#define ActFrmPwdStd		(ActRemOldBrf + 140)
#define ActFrmPwdTch		(ActRemOldBrf + 141)
#define ActChgPwdOth		(ActRemOldBrf + 142)
#define ActChgPwdStd		(ActRemOldBrf + 143)
#define ActChgPwdTch		(ActRemOldBrf + 144)
#define ActFrmMaiOth		(ActRemOldBrf + 145)
#define ActFrmMaiStd		(ActRemOldBrf + 146)
#define ActFrmMaiTch		(ActRemOldBrf + 147)
#define ActRemMaiOth		(ActRemOldBrf + 148)
#define ActRemMaiStd		(ActRemOldBrf + 149)
#define ActRemMaiTch		(ActRemOldBrf + 150)
#define ActNewMaiOth		(ActRemOldBrf + 151)
#define ActNewMaiStd		(ActRemOldBrf + 152)
#define ActNewMaiTch		(ActRemOldBrf + 153)

#define ActRemStdCrs		(ActRemOldBrf + 154)
#define ActRemNETCrs		(ActRemOldBrf + 155)
#define ActRemTchCrs		(ActRemOldBrf + 156)
#define ActRemUsrGbl		(ActRemOldBrf + 157)

#define ActReqRemAllStdCrs	(ActRemOldBrf + 158)
#define ActRemAllStdCrs		(ActRemOldBrf + 159)

#define ActReqRemOldUsr		(ActRemOldBrf + 160)
#define ActRemOldUsr		(ActRemOldBrf + 161)

#define ActLstDupUsr		(ActRemOldBrf + 162)
#define ActLstSimUsr		(ActRemOldBrf + 163)
#define ActRemDupUsr		(ActRemOldBrf + 164)

#define ActLstClk		(ActRemOldBrf + 165)

/*****************************************************************************/
/******************************** Social tab *********************************/
/*****************************************************************************/
// Actions in menu
#define ActSeeSocTmlGbl		(ActLstClk +   1)
#define ActSeeSocPrf		(ActLstClk +   2)
#define ActSeeFor		(ActLstClk +   3)
#define ActSeeChtRms		(ActLstClk +   4)
// Secondary actions
#define ActRcvSocPstGbl		(ActLstClk +   5)
#define ActRcvSocComGbl		(ActLstClk +   6)
#define ActShaSocNotGbl		(ActLstClk +   7)
#define ActUnsSocNotGbl		(ActLstClk +   8)
#define ActFavSocNotGbl		(ActLstClk +   9)
#define ActUnfSocNotGbl		(ActLstClk +  10)
#define ActFavSocComGbl		(ActLstClk +  11)
#define ActUnfSocComGbl		(ActLstClk +  12)
#define ActReqRemSocPubGbl	(ActLstClk +  13)
#define ActRemSocPubGbl		(ActLstClk +  14)
#define ActReqRemSocComGbl	(ActLstClk +  15)
#define ActRemSocComGbl		(ActLstClk +  16)

#define ActReqOthPubPrf		(ActLstClk +  17)

#define ActRcvSocPstUsr		(ActLstClk +  18)
#define ActRcvSocComUsr		(ActLstClk +  19)
#define ActShaSocNotUsr		(ActLstClk +  20)
#define ActUnsSocNotUsr		(ActLstClk +  21)
#define ActFavSocNotUsr		(ActLstClk +  22)
#define ActUnfSocNotUsr		(ActLstClk +  23)
#define ActFavSocComUsr		(ActLstClk +  24)
#define ActUnfSocComUsr		(ActLstClk +  25)
#define ActReqRemSocPubUsr	(ActLstClk +  26)
#define ActRemSocPubUsr		(ActLstClk +  27)
#define ActReqRemSocComUsr	(ActLstClk +  28)
#define ActRemSocComUsr		(ActLstClk +  29)

#define ActSeeOthPubPrf		(ActLstClk +  30)
#define ActCal1stClkTim		(ActLstClk +  31)
#define ActCalNumClk		(ActLstClk +  32)
#define ActCalNumFilVie		(ActLstClk +  33)
#define ActCalNumForPst		(ActLstClk +  34)
#define ActCalNumMsgSnt		(ActLstClk +  35)

#define ActFolUsr		(ActLstClk +  36)
#define ActUnfUsr		(ActLstClk +  37)
#define ActSeeFlg		(ActLstClk +  38)
#define ActSeeFlr		(ActLstClk +  39)

#define ActSeeForCrsUsr		(ActLstClk +  40)
#define ActSeeForCrsTch		(ActLstClk +  41)
#define ActSeeForDegUsr		(ActLstClk +  42)
#define ActSeeForDegTch		(ActLstClk +  43)
#define ActSeeForCtrUsr		(ActLstClk +  44)
#define ActSeeForCtrTch		(ActLstClk +  45)
#define ActSeeForInsUsr		(ActLstClk +  46)
#define ActSeeForInsTch		(ActLstClk +  47)
#define ActSeeForGenUsr		(ActLstClk +  48)
#define ActSeeForGenTch		(ActLstClk +  49)
#define ActSeeForSWAUsr		(ActLstClk +  50)
#define ActSeeForSWATch		(ActLstClk +  51)
#define ActSeePstForCrsUsr	(ActLstClk +  52)
#define ActSeePstForCrsTch	(ActLstClk +  53)
#define ActSeePstForDegUsr	(ActLstClk +  54)
#define ActSeePstForDegTch	(ActLstClk +  55)
#define ActSeePstForCtrUsr	(ActLstClk +  56)
#define ActSeePstForCtrTch	(ActLstClk +  57)
#define ActSeePstForInsUsr	(ActLstClk +  58)
#define ActSeePstForInsTch	(ActLstClk +  59)
#define ActSeePstForGenUsr	(ActLstClk +  60)
#define ActSeePstForGenTch	(ActLstClk +  61)
#define ActSeePstForSWAUsr	(ActLstClk +  62)
#define ActSeePstForSWATch	(ActLstClk +  63)
#define ActRcvThrForCrsUsr	(ActLstClk +  64)
#define ActRcvThrForCrsTch	(ActLstClk +  65)
#define ActRcvThrForDegUsr	(ActLstClk +  66)
#define ActRcvThrForDegTch	(ActLstClk +  67)
#define ActRcvThrForCtrUsr	(ActLstClk +  68)
#define ActRcvThrForCtrTch	(ActLstClk +  69)
#define ActRcvThrForInsUsr	(ActLstClk +  70)
#define ActRcvThrForInsTch	(ActLstClk +  71)
#define ActRcvThrForGenUsr	(ActLstClk +  72)
#define ActRcvThrForGenTch	(ActLstClk +  73)
#define ActRcvThrForSWAUsr	(ActLstClk +  74)
#define ActRcvThrForSWATch	(ActLstClk +  75)
#define ActRcvRepForCrsUsr	(ActLstClk +  76)
#define ActRcvRepForCrsTch	(ActLstClk +  77)
#define ActRcvRepForDegUsr	(ActLstClk +  78)
#define ActRcvRepForDegTch	(ActLstClk +  79)
#define ActRcvRepForCtrUsr	(ActLstClk +  80)
#define ActRcvRepForCtrTch	(ActLstClk +  81)
#define ActRcvRepForInsUsr	(ActLstClk +  82)
#define ActRcvRepForInsTch	(ActLstClk +  83)
#define ActRcvRepForGenUsr	(ActLstClk +  84)
#define ActRcvRepForGenTch	(ActLstClk +  85)
#define ActRcvRepForSWAUsr	(ActLstClk +  86)
#define ActRcvRepForSWATch	(ActLstClk +  87)
#define ActReqDelThrCrsUsr	(ActLstClk +  88)
#define ActReqDelThrCrsTch	(ActLstClk +  89)
#define ActReqDelThrDegUsr	(ActLstClk +  90)
#define ActReqDelThrDegTch	(ActLstClk +  91)
#define ActReqDelThrCtrUsr	(ActLstClk +  92)
#define ActReqDelThrCtrTch	(ActLstClk +  93)
#define ActReqDelThrInsUsr	(ActLstClk +  94)
#define ActReqDelThrInsTch	(ActLstClk +  95)
#define ActReqDelThrGenUsr	(ActLstClk +  96)
#define ActReqDelThrGenTch	(ActLstClk +  97)
#define ActReqDelThrSWAUsr	(ActLstClk +  98)
#define ActReqDelThrSWATch	(ActLstClk +  99)
#define ActDelThrForCrsUsr	(ActLstClk + 100)
#define ActDelThrForCrsTch	(ActLstClk + 101)
#define ActDelThrForDegUsr	(ActLstClk + 102)
#define ActDelThrForDegTch	(ActLstClk + 103)
#define ActDelThrForCtrUsr	(ActLstClk + 104)
#define ActDelThrForCtrTch	(ActLstClk + 105)
#define ActDelThrForInsUsr	(ActLstClk + 106)
#define ActDelThrForInsTch	(ActLstClk + 107)
#define ActDelThrForGenUsr	(ActLstClk + 108)
#define ActDelThrForGenTch	(ActLstClk + 109)
#define ActDelThrForSWAUsr	(ActLstClk + 110)
#define ActDelThrForSWATch	(ActLstClk + 111)
#define ActCutThrForCrsUsr	(ActLstClk + 112)
#define ActCutThrForCrsTch	(ActLstClk + 113)
#define ActCutThrForDegUsr	(ActLstClk + 114)
#define ActCutThrForDegTch	(ActLstClk + 115)
#define ActCutThrForCtrUsr	(ActLstClk + 116)
#define ActCutThrForCtrTch	(ActLstClk + 117)
#define ActCutThrForInsUsr	(ActLstClk + 118)
#define ActCutThrForInsTch	(ActLstClk + 119)
#define ActCutThrForGenUsr	(ActLstClk + 120)
#define ActCutThrForGenTch	(ActLstClk + 121)
#define ActCutThrForSWAUsr	(ActLstClk + 122)
#define ActCutThrForSWATch	(ActLstClk + 123)
#define ActPasThrForCrsUsr	(ActLstClk + 124)
#define ActPasThrForCrsTch	(ActLstClk + 125)
#define ActPasThrForDegUsr	(ActLstClk + 126)
#define ActPasThrForDegTch	(ActLstClk + 127)
#define ActPasThrForCtrUsr	(ActLstClk + 128)
#define ActPasThrForCtrTch	(ActLstClk + 129)
#define ActPasThrForInsUsr	(ActLstClk + 130)
#define ActPasThrForInsTch	(ActLstClk + 131)
#define ActPasThrForGenUsr	(ActLstClk + 132)
#define ActPasThrForGenTch	(ActLstClk + 133)
#define ActPasThrForSWAUsr	(ActLstClk + 134)
#define ActPasThrForSWATch	(ActLstClk + 135)
#define ActDelPstForCrsUsr	(ActLstClk + 136)
#define ActDelPstForCrsTch	(ActLstClk + 137)
#define ActDelPstForDegUsr	(ActLstClk + 138)
#define ActDelPstForDegTch	(ActLstClk + 139)
#define ActDelPstForCtrUsr	(ActLstClk + 140)
#define ActDelPstForCtrTch	(ActLstClk + 141)
#define ActDelPstForInsUsr	(ActLstClk + 142)
#define ActDelPstForInsTch	(ActLstClk + 143)
#define ActDelPstForGenUsr	(ActLstClk + 144)
#define ActDelPstForGenTch	(ActLstClk + 145)
#define ActDelPstForSWAUsr	(ActLstClk + 146)
#define ActDelPstForSWATch	(ActLstClk + 147)
#define ActEnbPstForCrsUsr	(ActLstClk + 148)
#define ActEnbPstForCrsTch	(ActLstClk + 149)
#define ActEnbPstForDegUsr	(ActLstClk + 150)
#define ActEnbPstForDegTch	(ActLstClk + 151)
#define ActEnbPstForCtrUsr	(ActLstClk + 152)
#define ActEnbPstForCtrTch	(ActLstClk + 153)
#define ActEnbPstForInsUsr	(ActLstClk + 154)
#define ActEnbPstForInsTch	(ActLstClk + 155)
#define ActEnbPstForGenUsr	(ActLstClk + 156)
#define ActEnbPstForGenTch	(ActLstClk + 157)
#define ActEnbPstForSWAUsr	(ActLstClk + 158)
#define ActEnbPstForSWATch	(ActLstClk + 159)
#define ActDisPstForCrsUsr	(ActLstClk + 160)
#define ActDisPstForCrsTch	(ActLstClk + 161)
#define ActDisPstForDegUsr	(ActLstClk + 162)
#define ActDisPstForDegTch	(ActLstClk + 163)
#define ActDisPstForCtrUsr	(ActLstClk + 164)
#define ActDisPstForCtrTch	(ActLstClk + 165)
#define ActDisPstForInsUsr	(ActLstClk + 166)
#define ActDisPstForInsTch	(ActLstClk + 167)
#define ActDisPstForGenUsr	(ActLstClk + 168)
#define ActDisPstForGenTch	(ActLstClk + 169)
#define ActDisPstForSWAUsr	(ActLstClk + 170)
#define ActDisPstForSWATch	(ActLstClk + 171)

#define ActCht			(ActLstClk + 172)

/*****************************************************************************/
/******************************* Messages tab ********************************/
/*****************************************************************************/
// Actions in menu
#define ActSeeNtf		(ActCht +   1)
#define ActSeeAnn		(ActCht +   2)
#define ActSeeAllNot		(ActCht +   3)
#define ActReqMsgUsr		(ActCht +   4)
#define ActSeeRcvMsg		(ActCht +   5)
#define ActSeeSntMsg		(ActCht +   6)
#define ActMaiStd		(ActCht +   7)
// Secondary actions
#define ActWriAnn		(ActCht +   8)
#define ActRcvAnn		(ActCht +   9)
#define ActHidAnn		(ActCht +  10)
#define ActRevAnn		(ActCht +  11)
#define ActRemAnn		(ActCht +  12)
#define ActSeeOneNot		(ActCht +  13)
#define ActWriNot		(ActCht +  14)
#define ActRcvNot		(ActCht +  15)
#define ActHidNot		(ActCht +  16)
#define ActRevNot		(ActCht +  17)
#define ActReqRemNot		(ActCht +  18)
#define ActRemNot		(ActCht +  19)

#define ActSeeNewNtf		(ActCht +  20)
#define ActMrkNtfSee		(ActCht +  21)
#define ActSeeMai		(ActCht +  22)
#define ActEdiMai		(ActCht +  23)
#define ActNewMai		(ActCht +  24)
#define ActRemMai		(ActCht +  25)
#define ActRenMaiSho		(ActCht +  26)
#define ActRenMaiFul		(ActCht +  27)

#define ActRcvMsgUsr		(ActCht +  28)
#define ActReqDelAllSntMsg	(ActCht +  29)
#define ActReqDelAllRcvMsg	(ActCht +  30)
#define ActDelAllSntMsg		(ActCht +  31)
#define ActDelAllRcvMsg		(ActCht +  32)
#define ActDelSntMsg		(ActCht +  33)
#define ActDelRcvMsg		(ActCht +  34)
#define ActExpSntMsg		(ActCht +  35)
#define ActExpRcvMsg		(ActCht +  36)
#define ActConSntMsg		(ActCht +  37)
#define ActConRcvMsg		(ActCht +  38)
#define ActLstBanUsr		(ActCht +  39)
#define ActBanUsrMsg		(ActCht +  40)
#define ActUnbUsrMsg		(ActCht +  41)
#define ActUnbUsrLst		(ActCht +  42)

/*****************************************************************************/
/****************************** Statistics tab *******************************/
/*****************************************************************************/
// Actions in menu
#define ActReqUseGbl		(ActUnbUsrLst +  1)
#define ActSeePhoDeg		(ActUnbUsrLst +  2)
#define ActReqStaCrs		(ActUnbUsrLst +  3)
#define ActReqAccGbl		(ActUnbUsrLst +  4)
#define ActReqMyUsgRep		(ActUnbUsrLst +  5)
#define ActMFUAct		(ActUnbUsrLst +  6)

// Secondary actions
#define ActSeeUseGbl		(ActUnbUsrLst +  7)
#define ActPrnPhoDeg		(ActUnbUsrLst +  8)
#define ActCalPhoDeg		(ActUnbUsrLst +  9)
#define ActSeeAccGbl		(ActUnbUsrLst + 10)
#define ActReqAccCrs		(ActUnbUsrLst + 11)
#define ActSeeAccCrs		(ActUnbUsrLst + 12)
#define ActSeeAllStaCrs		(ActUnbUsrLst + 13)

#define ActSeeMyUsgRep		(ActUnbUsrLst + 14)

/*****************************************************************************/
/******************************** Profile tab ********************************/
/*****************************************************************************/
// Actions in menu
#define ActFrmLogIn		(ActSeeMyUsgRep +  1)
#define ActFrmRolSes		(ActSeeMyUsgRep +  2)
#define ActMyCrs		(ActSeeMyUsgRep +  3)
#define ActSeeMyTT		(ActSeeMyUsgRep +  4)
#define ActSeeMyAgd		(ActSeeMyUsgRep +  5)
#define ActFrmMyAcc		(ActSeeMyUsgRep +  6)
#define ActReqEdiRecCom		(ActSeeMyUsgRep +  7)
#define ActEdiPrf		(ActSeeMyUsgRep +  8)
// Secondary actions
#define ActReqSndNewPwd		(ActSeeMyUsgRep +  9)
#define ActSndNewPwd		(ActSeeMyUsgRep + 10)
#define ActLogOut		(ActSeeMyUsgRep + 11)
#define ActLogIn		(ActSeeMyUsgRep + 12)
#define ActLogInNew		(ActSeeMyUsgRep + 13)
#define ActLogInLan		(ActSeeMyUsgRep + 14)
#define ActAnnSee		(ActSeeMyUsgRep + 15)
#define ActChgMyRol		(ActSeeMyUsgRep + 16)
#define ActFrmNewEvtMyAgd	(ActSeeMyUsgRep + 17)
#define ActEdiOneEvtMyAgd	(ActSeeMyUsgRep + 18)
#define ActNewEvtMyAgd		(ActSeeMyUsgRep + 19)
#define ActChgEvtMyAgd		(ActSeeMyUsgRep + 20)
#define ActReqRemEvtMyAgd	(ActSeeMyUsgRep + 21)
#define ActRemEvtMyAgd		(ActSeeMyUsgRep + 22)
#define ActHidEvtMyAgd		(ActSeeMyUsgRep + 23)
#define ActShoEvtMyAgd		(ActSeeMyUsgRep + 24)
#define ActPrvEvtMyAgd		(ActSeeMyUsgRep + 25)
#define ActPubEvtMyAgd		(ActSeeMyUsgRep + 26)
#define ActPrnAgdQR		(ActSeeMyUsgRep + 27)

#define ActChkUsrAcc		(ActSeeMyUsgRep + 28)
#define ActCreUsrAcc		(ActSeeMyUsgRep + 29)
#define ActRemID_Me		(ActSeeMyUsgRep + 30)
#define ActNewIDMe		(ActSeeMyUsgRep + 31)
#define ActRemOldNic		(ActSeeMyUsgRep + 32)
#define ActChgNic		(ActSeeMyUsgRep + 33)
#define ActRemMaiMe		(ActSeeMyUsgRep + 34)
#define ActNewMaiMe		(ActSeeMyUsgRep + 35)
#define ActCnfMai		(ActSeeMyUsgRep + 36)
#define ActFrmChgMyPwd		(ActSeeMyUsgRep + 37)
#define ActChgPwd		(ActSeeMyUsgRep + 38)
#define ActReqRemMyAcc		(ActSeeMyUsgRep + 39)
#define ActRemMyAcc		(ActSeeMyUsgRep + 40)

#define ActChgMyData		(ActSeeMyUsgRep + 41)

#define ActReqMyPho		(ActSeeMyUsgRep + 42)
#define ActDetMyPho		(ActSeeMyUsgRep + 43)
#define ActUpdMyPho		(ActSeeMyUsgRep + 44)
#define ActReqRemMyPho		(ActSeeMyUsgRep + 45)
#define ActRemMyPho		(ActSeeMyUsgRep + 46)

#define ActEdiPri		(ActSeeMyUsgRep + 47)
#define ActChgPriPho		(ActSeeMyUsgRep + 48)
#define ActChgPriPrf		(ActSeeMyUsgRep + 49)

#define ActReqEdiMyIns		(ActSeeMyUsgRep + 50)
#define ActChgCtyMyIns		(ActSeeMyUsgRep + 51)
#define ActChgMyIns		(ActSeeMyUsgRep + 52)
#define ActChgMyCtr		(ActSeeMyUsgRep + 53)
#define ActChgMyDpt		(ActSeeMyUsgRep + 54)
#define ActChgMyOff		(ActSeeMyUsgRep + 55)
#define ActChgMyOffPho		(ActSeeMyUsgRep + 56)

#define ActReqEdiMyNet		(ActSeeMyUsgRep + 57)
#define ActChgMyNet		(ActSeeMyUsgRep + 58)

#define ActChgThe		(ActSeeMyUsgRep + 59)
#define ActReqChgLan		(ActSeeMyUsgRep + 60)
#define ActChgLan		(ActSeeMyUsgRep + 61)
#define ActChg1stDay		(ActSeeMyUsgRep + 62)
#define ActChgDatFmt		(ActSeeMyUsgRep + 63)
#define ActChgCol		(ActSeeMyUsgRep + 64)
#define ActHidLftCol		(ActSeeMyUsgRep + 65)
#define ActHidRgtCol		(ActSeeMyUsgRep + 66)
#define ActShoLftCol		(ActSeeMyUsgRep + 67)
#define ActShoRgtCol		(ActSeeMyUsgRep + 68)
#define ActChgIco		(ActSeeMyUsgRep + 69)
#define ActChgMnu		(ActSeeMyUsgRep + 70)
#define ActChgNtfPrf		(ActSeeMyUsgRep + 71)

#define ActPrnUsrQR		(ActSeeMyUsgRep + 72)

#define ActPrnMyTT		(ActSeeMyUsgRep + 73)
#define ActEdiTut		(ActSeeMyUsgRep + 74)
#define ActChgTut		(ActSeeMyUsgRep + 75)
#define ActChgMyTT1stDay	(ActSeeMyUsgRep + 76)

/*****************************************************************************/
/******************************** Public types *******************************/
/*****************************************************************************/

struct Act_Tabs
  {
   char *Icon;
  };

struct Act_Actions
  {
   long ActCod;	// Unique, time-persistent numerical code for the action
   signed int IndexInMenu;
   Tab_Tab_t Tab;
   Act_Action_t SuperAction;
   unsigned PermissionCrsIfIBelong;
   unsigned PermissionCrsIfIDontBelong;
   unsigned PermissionDeg;
   unsigned PermissionCtr;
   unsigned PermissionIns;
   unsigned PermissionCty;
   unsigned PermissionSys;
   Act_Content_t ContentType;
   Act_BrowserTab_t BrowserTab;
   void (*FunctionPriori) ();
   void (*FunctionPosteriori) ();
   const char *Icon;
  };

/*****************************************************************************/
/***************************** Public prototypes *****************************/
/*****************************************************************************/

Act_Action_t Act_GetActionFromActCod (long ActCod);
long Act_GetActCod (Act_Action_t Action);
signed int Act_GetIndexInMenu (Act_Action_t Action);
Tab_Tab_t Act_GetTab (Act_Action_t Action);
Act_Action_t Act_GetSuperAction (Act_Action_t Action);
bool Act_CheckIfIHavePermissionToExecuteAction (Act_Action_t Action);
Act_Content_t Act_GetContentType (Act_Action_t Action);
Act_BrowserTab_t Act_GetBrowserTab (Act_Action_t Action);
void (*Act_GetFunctionPriori (Act_Action_t Action)) (void);
void (*Act_GetFunctionPosteriori (Act_Action_t Action)) (void);
const char *Act_GetIcon (Act_Action_t Action);

const char *Act_GetTitleAction (Act_Action_t Action);
const char *Act_GetSubtitleAction (Act_Action_t Action);
char *Act_GetActionTextFromDB (long ActCod,
                               char ActTxt[Act_MAX_BYTES_ACTION_TXT + 1]);

void Act_FormGoToStart (Act_Action_t NextAction);
void Act_FormStart (Act_Action_t NextAction);
void Act_FormStartOnSubmit (Act_Action_t NextAction,const char *OnSubmit);
void Act_FormStartAnchorOnSubmit (Act_Action_t NextAction,const char *Anchor,const char *OnSubmit);
void Act_FormStartUnique (Act_Action_t NextAction);
void Act_FormStartAnchor (Act_Action_t NextAction,const char *Anchor);
void Act_FormStartUniqueAnchor (Act_Action_t NextAction,const char *Anchor);
void Act_FormStartId (Act_Action_t NextAction,const char *Id);
void Act_SetParamsForm (char *ParamsStr,Act_Action_t NextAction,
                        bool PutParameterLocationIfNoSesion);
void Act_FormEnd (void);
void Act_LinkFormSubmit (const char *Title,const char *LinkStyle,
                         const char *OnSubmit);
void Act_LinkFormSubmitUnique (const char *Title,const char *LinkStyle);
void Act_LinkFormSubmitId (const char *Title,const char *LinkStyle,
                           const char *Id,const char *OnSubmit);
void Act_LinkFormSubmitAnimated (const char *Title,const char *LinkStyle,
                                 const char *OnSubmit);

void Act_SetUniqueId (char UniqueId[Act_MAX_BYTES_ID]);

void Act_AdjustActionWhenNoUsrLogged (void);
void Act_AdjustCurrentAction (void);

#endif

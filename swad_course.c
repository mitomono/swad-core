// swad_course.c: edition of courses

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

#include <linux/limits.h>	// For PATH_MAX
#include <linux/stddef.h>	// For NULL
#include <limits.h>		// For maximum values
#include <stdlib.h>		// For getenv, etc.
#include <string.h>		// For string functions

#include "swad_box.h"
#include "swad_course.h"
#include "swad_constant.h"
#include "swad_database.h"
#include "swad_degree.h"
#include "swad_enrolment.h"
#include "swad_exam.h"
#include "swad_global.h"
#include "swad_help.h"
#include "swad_hierarchy.h"
#include "swad_indicator.h"
#include "swad_logo.h"
#include "swad_notification.h"
#include "swad_parameter.h"
#include "swad_QR.h"
#include "swad_role.h"
#include "swad_RSS.h"
#include "swad_tab.h"
#include "swad_table.h"
#include "swad_theme.h"

/*****************************************************************************/
/************** External global variables from others modules ****************/
/*****************************************************************************/

extern struct Globals Gbl;

/*****************************************************************************/
/*************************** Public constants ********************************/
/*****************************************************************************/

/*****************************************************************************/
/***************************** Private types *********************************/
/*****************************************************************************/

/*****************************************************************************/
/**************************** Private constants ******************************/
/*****************************************************************************/

/*****************************************************************************/
/**************************** Private prototypes *****************************/
/*****************************************************************************/

static void Crs_Configuration (bool PrintView);
static void Crs_PutIconToPrint (void);
static void Crs_ShowNumUsrsInCrs (Rol_Role_t Role);

static void Crs_WriteListMyCoursesToSelectOne (void);

static void Crs_GetListCoursesInDegree (Crs_WhatCourses_t WhatCourses);
static void Crs_ListCourses (void);
static bool Crs_CheckIfICanCreateCourses (void);
static void Crs_PutIconsListCourses (void);
static void Crs_PutIconToEditCourses (void);
static bool Crs_ListCoursesOfAYearForSeeing (unsigned Year);

static void Crs_PutIconToViewCourses (void);
static void Crs_ListCoursesForEdition (void);
static void Crs_ListCoursesOfAYearForEdition (unsigned Year);
static bool Crs_CheckIfICanEdit (struct Course *Crs);
static Crs_StatusTxt_t Crs_GetStatusTxtFromStatusBits (Crs_Status_t Status);
static Crs_Status_t Crs_GetStatusBitsFromStatusTxt (Crs_StatusTxt_t StatusTxt);
static void Crs_PutFormToCreateCourse (void);
static void Crs_PutHeadCoursesForSeeing (void);
static void Crs_PutHeadCoursesForEdition (void);
static void Crs_RecFormRequestOrCreateCrs (unsigned Status);
static void Crs_GetParamsNewCourse (struct Course *Crs);

static void Crs_CreateCourse (unsigned Status);
static void Crs_GetDataOfCourseFromRow (struct Course *Crs,MYSQL_ROW row);

static void Crs_UpdateCrsDegDB (long CrsCod,long DegCod);

static void Crs_UpdateCrsYear (struct Course *Crs,unsigned NewYear);

static void Crs_GetShortNamesByCod (long CrsCod,
                                    char CrsShortName[Hie_MAX_BYTES_SHRT_NAME + 1],
                                    char DegShortName[Hie_MAX_BYTES_SHRT_NAME + 1]);

static void Crs_EmptyCourseCompletely (long CrsCod);

static void Crs_RenameCourse (struct Course *Crs,Cns_ShrtOrFullName_t ShrtOrFullName);
static bool Crs_CheckIfCrsNameExistsInYearOfDeg (const char *FieldName,const char *Name,long CrsCod,
                                                 long DegCod,unsigned Year);
static void Crs_UpdateCrsNameDB (long CrsCod,const char *FieldName,const char *NewCrsName);

static void Crs_PutButtonToGoToCrs (void);
static void Crs_PutButtonToRegisterInCrs (void);

static void Crs_PutLinkToSearchCourses (void);
static void Sch_PutLinkToSearchCoursesParams (void);

static void Crs_PutParamOtherCrsCod (long CrsCod);
static long Crs_GetAndCheckParamOtherCrsCod (long MinCodAllowed);

static void Crs_WriteRowCrsData (unsigned NumCrs,MYSQL_ROW row,bool WriteColumnAccepted);

/*****************************************************************************/
/***************** Show introduction to the current course *******************/
/*****************************************************************************/

void Crs_ShowIntroduction (void)
  {
   /***** Course configuration *****/
   fprintf (Gbl.F.Out,"<div class=\"CENTER_MIDDLE\">");
   Crs_Configuration (false);
   fprintf (Gbl.F.Out,"</div>");

   /***** Course introduction *****/
   Inf_ShowInfo ();

   /***** Show help to enrol me *****/
   Hlp_ShowHelpWhatWouldYouLikeToDo ();
  }

/*****************************************************************************/
/***************** Print configuration of the current course *****************/
/*****************************************************************************/

void Crs_PrintConfiguration (void)
  {
   Crs_Configuration (true);
  }

/*****************************************************************************/
/***************** Configuration of the current course ***********************/
/*****************************************************************************/

static void Crs_Configuration (bool PrintView)
  {
   extern const char *Hlp_COURSE_Information;
   extern const char *The_ClassForm[The_NUM_THEMES];
   extern const char *Txt_Degree;
   extern const char *Txt_Course;
   extern const char *Txt_Short_name;
   extern const char *Txt_Year_OF_A_DEGREE;
   extern const char *Txt_YEAR_OF_DEGREE[1 + Deg_MAX_YEARS_PER_DEGREE];
   extern const char *Txt_Not_applicable;
   extern const char *Txt_Institutional_code;
   extern const char *Txt_Internal_code;
   extern const char *Txt_Shortcut;
   extern const char *Txt_STR_LANG_ID[1 + Txt_NUM_LANGUAGES];
   extern const char *Txt_QR_code;
   extern const char *Txt_Indicators;
   extern const char *Txt_of_PART_OF_A_TOTAL;
   unsigned NumDeg;
   unsigned Year;
   int NumIndicatorsFromDB;
   struct Ind_IndicatorsCrs Indicators;
   bool IsForm = (!PrintView && Gbl.Usrs.Me.Role.Logged >= Rol_TCH);
   bool PutLink = !PrintView && Gbl.CurrentDeg.Deg.WWW[0];

   /***** Messages and links above the box *****/
   if (!PrintView)
     {
      /* Link to request enrolment in the current course */
      if (Gbl.Usrs.Me.Role.Logged == Rol_GST ||
	  Gbl.Usrs.Me.Role.Logged == Rol_USR)
	{
         fprintf (Gbl.F.Out,"<div class=\"CONTEXT_MENU\">");
         Enr_PutLinkToRequestSignUp ();
         fprintf (Gbl.F.Out,"</div>");
	}
     }

   /***** Start box *****/
   if (PrintView)
      Box_StartBox (NULL,NULL,NULL,
		    NULL,Box_NOT_CLOSABLE);
   else
      Box_StartBox (NULL,NULL,Crs_PutIconToPrint,
		    Hlp_COURSE_Information,Box_NOT_CLOSABLE);

   /***** Title *****/
   fprintf (Gbl.F.Out,"<div class=\"FRAME_TITLE FRAME_TITLE_BIG\">");
   if (PutLink)
      fprintf (Gbl.F.Out,"<a href=\"%s\" target=\"_blank\""
	                 " class=\"FRAME_TITLE_BIG\" title=\"%s\">",
	       Gbl.CurrentDeg.Deg.WWW,
	       Gbl.CurrentDeg.Deg.FullName);
   Log_DrawLogo (Sco_SCOPE_DEG,Gbl.CurrentDeg.Deg.DegCod,
                 Gbl.CurrentDeg.Deg.ShrtName,64,NULL,true);
   if (PutLink)
      fprintf (Gbl.F.Out,"</a>");
   fprintf (Gbl.F.Out,"<br />%s"
                      "</div>",
            Gbl.CurrentCrs.Crs.FullName);

   /***** Start table *****/
   Tbl_StartTableWide (2);

   /***** Degree *****/
   fprintf (Gbl.F.Out,"<tr>"
                      "<td class=\"RIGHT_MIDDLE\">"
	              "<label for=\"OthDegCod\" class=\"%s\">%s:</label>"
                      "</td>"
                      "<td class=\"DAT LEFT_MIDDLE\">",
            The_ClassForm[Gbl.Prefs.Theme],
            Txt_Degree);

   if (!PrintView &&
       Gbl.Usrs.Me.Role.Logged >= Rol_CTR_ADM)
      // Only centre admins, institution admins and system admin can move a course to another degree
     {
      /* Get list of degrees of the current centre */
      Deg_GetListDegsOfCurrentCtr ();

      /* Put form to select degree */
      Act_FormStart (ActChgCrsDegCfg);
      fprintf (Gbl.F.Out,"<select id=\"OthDegCod\" name=\"OthDegCod\""
			 " class=\"INPUT_SHORT_NAME\""
			 " onchange=\"document.getElementById('%s').submit();\">",
	       Gbl.Form.Id);
      for (NumDeg = 0;
	   NumDeg < Gbl.CurrentCtr.Ctr.Degs.Num;
	   NumDeg++)
	 fprintf (Gbl.F.Out,"<option value=\"%ld\"%s>%s</option>",
		  Gbl.CurrentCtr.Ctr.Degs.Lst[NumDeg].DegCod,
		  Gbl.CurrentCtr.Ctr.Degs.Lst[NumDeg].DegCod == Gbl.CurrentDeg.Deg.DegCod ? " selected=\"selected\"" :
										            "",
		  Gbl.CurrentCtr.Ctr.Degs.Lst[NumDeg].ShrtName);
      fprintf (Gbl.F.Out,"</select>");
      Act_FormEnd ();

      /* Free list of degrees of the current centre */
      Deg_FreeListDegs (&Gbl.CurrentCtr.Ctr.Degs);
     }
   else	// I can not move course to another degree
      fprintf (Gbl.F.Out,"%s",Gbl.CurrentDeg.Deg.FullName);

   fprintf (Gbl.F.Out,"</td>"
	              "</tr>");

   /***** Course full name *****/
   fprintf (Gbl.F.Out,"<tr>"
		      "<td class=\"RIGHT_MIDDLE\">"
	              "<label for=\"FullName\" class=\"%s\">%s:</label>"
		      "</td>"
		      "<td class=\"DAT_N LEFT_MIDDLE\">",
	    The_ClassForm[Gbl.Prefs.Theme],
	    Txt_Course);
   if (!PrintView &&
       Gbl.Usrs.Me.Role.Logged >= Rol_DEG_ADM)
      // Only degree admins, centre admins, institution admins and system admins can edit course full name
     {
      /* Form to change course full name */
      Act_FormStart (ActRenCrsFulCfg);
      fprintf (Gbl.F.Out,"<input type=\"text\""
	                 " id=\"FullName\" name=\"FullName\""
			 " maxlength=\"%u\" value=\"%s\""
			 " class=\"INPUT_FULL_NAME\""
			 " onchange=\"document.getElementById('%s').submit();\" />",
	       Hie_MAX_CHARS_FULL_NAME,
	       Gbl.CurrentCrs.Crs.FullName,
	       Gbl.Form.Id);
      Act_FormEnd ();
     }
   else	// I can not edit course full name
      fprintf (Gbl.F.Out,"%s",Gbl.CurrentCrs.Crs.FullName);
   fprintf (Gbl.F.Out,"</td>"
		      "</tr>");

   /***** Course short name *****/
   fprintf (Gbl.F.Out,"<tr>"
		      "<td class=\"RIGHT_MIDDLE\">"
	              "<label for=\"ShortName\" class=\"%s\">%s:</label>"
		      "</td>"
		      "<td class=\"DAT_N LEFT_MIDDLE\">",
	    The_ClassForm[Gbl.Prefs.Theme],
	    Txt_Short_name);
   if (!PrintView &&
       Gbl.Usrs.Me.Role.Logged >= Rol_DEG_ADM)
      // Only degree admins, centre admins, institution admins and system admins can edit course short name
     {
      /* Form to change course short name */
      Act_FormStart (ActRenCrsShoCfg);
      fprintf (Gbl.F.Out,"<input type=\"text\""
	                 " id=\"ShortName\" name=\"ShortName\""
			 " maxlength=\"%u\" value=\"%s\""
			 " class=\"INPUT_SHORT_NAME\""
			 " onchange=\"document.getElementById('%s').submit();\" />",
	       Hie_MAX_CHARS_SHRT_NAME,
	       Gbl.CurrentCrs.Crs.ShrtName,
	       Gbl.Form.Id);
      Act_FormEnd ();
     }
   else	// I can not edit course short name
      fprintf (Gbl.F.Out,"%s",Gbl.CurrentCrs.Crs.ShrtName);
   fprintf (Gbl.F.Out,"</td>"
		      "</tr>");

   /***** Course year *****/
   fprintf (Gbl.F.Out,"<tr>"
                      "<td class=\"RIGHT_MIDDLE\">"
	              "<label for=\"OthCrsYear\" class=\"%s\">%s:</label>"
                      "</td>"
                      "<td class=\"DAT LEFT_MIDDLE\">",
            The_ClassForm[Gbl.Prefs.Theme],
            Txt_Year_OF_A_DEGREE);
   if (IsForm)
     {
      Act_FormStart (ActChgCrsYeaCfg);
      fprintf (Gbl.F.Out,"<select id=\"OthCrsYear\" name=\"OthCrsYear\""
	                 " onchange=\"document.getElementById('%s').submit();\">",
               Gbl.Form.Id);
      for (Year = 0;
	   Year <= Deg_MAX_YEARS_PER_DEGREE;
           Year++)
	 fprintf (Gbl.F.Out,"<option value=\"%u\"%s>%s</option>",
		  Year,
		  Year == Gbl.CurrentCrs.Crs.Year ? " selected=\"selected\"" :
						    "",
		  Txt_YEAR_OF_DEGREE[Year]);
      fprintf (Gbl.F.Out,"</select>");
      Act_FormEnd ();
     }
   else
      fprintf (Gbl.F.Out,"%s",
               Gbl.CurrentCrs.Crs.Year ? Txt_YEAR_OF_DEGREE[Gbl.CurrentCrs.Crs.Year] :
	                                 Txt_Not_applicable);
   fprintf (Gbl.F.Out,"</td>"
	              "</tr>");

   if (!PrintView)
     {
      /***** Institutional code of the course *****/
      fprintf (Gbl.F.Out,"<tr>"
                         "<td class=\"RIGHT_MIDDLE\">"
	                 "<label for=\"InsCrsCod\" class=\"%s\">%s:</label>"
                         "</td>"
                         "<td class=\"DAT LEFT_MIDDLE\">",
              The_ClassForm[Gbl.Prefs.Theme],
              Txt_Institutional_code);
      if (IsForm)
	{
         Act_FormStart (ActChgInsCrsCodCfg);
         fprintf (Gbl.F.Out,"<input type=\"text\""
                            " id=\"InsCrsCod\" name=\"InsCrsCod\""
                            " size=\"%u\" maxlength=\"%u\" value=\"%s\""
	                    " onchange=\"document.getElementById('%s').submit();\" />",
                  Crs_MAX_CHARS_INSTITUTIONAL_CRS_COD,
                  Crs_MAX_CHARS_INSTITUTIONAL_CRS_COD,
                  Gbl.CurrentCrs.Crs.InstitutionalCrsCod,
                  Gbl.Form.Id);
         Act_FormEnd ();
	}
      else
         fprintf (Gbl.F.Out,"%s",Gbl.CurrentCrs.Crs.InstitutionalCrsCod);
      fprintf (Gbl.F.Out,"</td>"
	                 "</tr>");

      /***** Internal code of the course *****/
      fprintf (Gbl.F.Out,"<tr>"
                         "<td class=\"%s RIGHT_MIDDLE\">"
                         "%s:"
                         "</td>"
                         "<td class=\"DAT LEFT_MIDDLE\">"
                         "%ld"
                         "</td>"
                         "</tr>",
              The_ClassForm[Gbl.Prefs.Theme],
              Txt_Internal_code,
              Gbl.CurrentCrs.Crs.CrsCod);
     }

   /***** Link to the course *****/
   fprintf (Gbl.F.Out,"<tr>"
                      "<td class=\"%s RIGHT_MIDDLE\">"
                      "%s:"
                      "</td>"
                      "<td class=\"DAT LEFT_MIDDLE\">"
                      "<a href=\"%s/%s?crs=%ld\" class=\"DAT\" target=\"_blank\">"
                      "%s/%s?crs=%ld</a>"
                      "</td>"
                      "</tr>",
            The_ClassForm[Gbl.Prefs.Theme],
            Txt_Shortcut,
            Cfg_URL_SWAD_CGI,
            Txt_STR_LANG_ID[Gbl.Prefs.Language],
            Gbl.CurrentCrs.Crs.CrsCod,
            Cfg_URL_SWAD_CGI,
            Txt_STR_LANG_ID[Gbl.Prefs.Language],
            Gbl.CurrentCrs.Crs.CrsCod);

   if (PrintView)
     {
      /***** QR code with link to the course *****/
      fprintf (Gbl.F.Out,"<tr>"
			 "<td class=\"%s RIGHT_MIDDLE\">"
			 "%s:"
			 "</td>"
			 "<td class=\"DAT LEFT_MIDDLE\">",
	       The_ClassForm[Gbl.Prefs.Theme],
	       Txt_QR_code);
      QR_LinkTo (250,"crs",Gbl.CurrentCrs.Crs.CrsCod);
      fprintf (Gbl.F.Out,"</td>"
			 "</tr>");
     }
   else
     {
      /***** Number of users *****/
      Crs_ShowNumUsrsInCrs (Rol_TCH);
      Crs_ShowNumUsrsInCrs (Rol_NET);
      Crs_ShowNumUsrsInCrs (Rol_STD);

      /***** Indicators *****/
      NumIndicatorsFromDB = Ind_GetNumIndicatorsCrsFromDB (Gbl.CurrentCrs.Crs.CrsCod);
      Ind_ComputeAndStoreIndicatorsCrs (Gbl.CurrentCrs.Crs.CrsCod,
                                        NumIndicatorsFromDB,&Indicators);
      fprintf (Gbl.F.Out,"<tr>"
                         "<td class=\"%s RIGHT_MIDDLE\">"
                         "%s:"
                         "</td>"
                         "<td class=\"LEFT_MIDDLE\">",
               The_ClassForm[Gbl.Prefs.Theme],
               Txt_Indicators);
      Act_FormStart (ActReqStaCrs);
      sprintf (Gbl.Title,"%u %s %u",
               Indicators.NumIndicators,Txt_of_PART_OF_A_TOTAL,Ind_NUM_INDICATORS);
      Act_LinkFormSubmit (Gbl.Title,"DAT",NULL);
      fprintf (Gbl.F.Out,"%s "
                         "<img src=\"%s/%s\" alt=\"%s\""
                         " class=\"ICO20x20\" />",
               Gbl.Title,
               Gbl.Prefs.IconsURL,
               (Indicators.NumIndicators == Ind_NUM_INDICATORS) ? "ok_green16x16.gif" :
        	                                                  "warning64x64.png",
               Gbl.Title);
      Act_FormEnd ();
      fprintf (Gbl.F.Out,"</td>"
                         "</tr>");
     }

   /***** End table *****/
   Tbl_EndTable ();

   /***** End box *****/
   Box_EndBox ();
  }

/*****************************************************************************/
/************* Put icon to print the configuration of a course ***************/
/*****************************************************************************/

static void Crs_PutIconToPrint (void)
  {
   Ico_PutContextualIconToPrint (ActPrnCrsInf,NULL);
  }

/*****************************************************************************/
/**************** Number of users in courses of this country *****************/
/*****************************************************************************/

static void Crs_ShowNumUsrsInCrs (Rol_Role_t Role)
  {
   extern const char *The_ClassForm[The_NUM_THEMES];
   extern const char *Txt_ROLES_PLURAL_Abc[Rol_NUM_ROLES][Usr_NUM_SEXS];

   fprintf (Gbl.F.Out,"<tr>"
		      "<td class=\"%s RIGHT_MIDDLE\">"
		      "%s:"
		      "</td>"
		      "<td class=\"DAT LEFT_MIDDLE\">"
		      "%u"
		      "</td>"
		      "</tr>",
	    The_ClassForm[Gbl.Prefs.Theme],
	    Txt_ROLES_PLURAL_Abc[Role][Usr_SEX_UNKNOWN],
            Gbl.CurrentCrs.Crs.NumUsrs[Role]);
  }

/*****************************************************************************/
/************************ Write menu with my courses *************************/
/*****************************************************************************/

#define Crs_MAX_BYTES_TXT_LINK 40

static void Crs_WriteListMyCoursesToSelectOne (void)
  {
   extern const char *Hlp_PROFILE_Courses;
   extern const char *The_ClassForm[The_NUM_THEMES];
   extern const char *The_ClassFormDark[The_NUM_THEMES];
   extern const char *Txt_My_courses;
   extern const char *Txt_System;
   extern const char *Txt_Go_to_X;
   struct Country Cty;
   struct Instit Ins;
   struct Centre Ctr;
   struct Degree Deg;
   struct Course Crs;
   bool IsLastItemInLevel[1 + 5];
   bool Highlight;	// Highlight because degree, course, etc. is selected
   MYSQL_RES *mysql_resCty;
   MYSQL_RES *mysql_resIns;
   MYSQL_RES *mysql_resCtr;
   MYSQL_RES *mysql_resDeg;
   MYSQL_RES *mysql_resCrs;
   MYSQL_ROW row;
   unsigned NumCty;
   unsigned NumCtys;
   unsigned NumIns;
   unsigned NumInss;
   unsigned NumCtr;
   unsigned NumCtrs;
   unsigned NumDeg;
   unsigned NumDegs;
   unsigned NumCrs;
   unsigned NumCrss;
   char ActTxt[Act_MAX_BYTES_ACTION_TXT + 1];
   const char *ClassNormal;
   char ClassHighlight[64];

   ClassNormal = The_ClassForm[Gbl.Prefs.Theme];
   sprintf (ClassHighlight,"%s LIGHT_BLUE",The_ClassFormDark[Gbl.Prefs.Theme]);

   /***** Start box *****/
   Box_StartBox (NULL,Txt_My_courses,NULL,
                 Hlp_PROFILE_Courses,Box_NOT_CLOSABLE);
   fprintf (Gbl.F.Out,"<ul class=\"LIST_LEFT\">");

   /***** Write link to platform *****/
   Highlight = (Gbl.CurrentCty.Cty.CtyCod <= 0);
   fprintf (Gbl.F.Out,"<li class=\"%s\" style=\"height:25px;\">",
	    Highlight ? ClassHighlight :
			ClassNormal);
   Act_FormStart (ActMyCrs);
   Cty_PutParamCtyCod (-1L);
   Act_LinkFormSubmit (Txt_System,
                       Highlight ? ClassHighlight :
        	                   ClassNormal,NULL);
   fprintf (Gbl.F.Out,"<img src=\"%s/sys64x64.gif\""
	              " alt=\"%s\" title=\"%s\""
                      " class=\"ICO20x20\" />&nbsp;%s</a>",
	    Gbl.Prefs.IconsURL,
	    Txt_System,
	    Txt_System,
	    Txt_System);
   Act_FormEnd ();
   fprintf (Gbl.F.Out,"</li>");

   /***** Get my countries *****/
   NumCtys = Usr_GetCtysFromUsr (Gbl.Usrs.Me.UsrDat.UsrCod,&mysql_resCty);
   for (NumCty = 0;
	NumCty < NumCtys;
	NumCty++)
     {
      /***** Get next institution *****/
      row = mysql_fetch_row (mysql_resCty);

      /***** Get data of this institution *****/
      Cty.CtyCod = Str_ConvertStrCodToLongCod (row[0]);
      if (!Cty_GetDataOfCountryByCod (&Cty,Cty_GET_BASIC_DATA))
	 Lay_ShowErrorAndExit ("Country not found.");

      /***** Write link to country *****/
      Highlight = (Gbl.CurrentIns.Ins.InsCod <= 0 &&
	           Gbl.CurrentCty.Cty.CtyCod == Cty.CtyCod);
      fprintf (Gbl.F.Out,"<li class=\"%s\" style=\"height:25px;\">",
               Highlight ? ClassHighlight :
        	           ClassNormal);
      IsLastItemInLevel[1] = (NumCty == NumCtys - 1);
      Lay_IndentDependingOnLevel (1,IsLastItemInLevel);
      Act_FormStart (ActMyCrs);
      Cty_PutParamCtyCod (Cty.CtyCod);
      Act_LinkFormSubmit (Act_GetActionTextFromDB (Act_GetActCod (ActSeeCtyInf),ActTxt),
			  Highlight ? ClassHighlight :
        	                      ClassNormal,NULL);
      /* Country map */
      fprintf (Gbl.F.Out,"<img src=\"%s/%s/%s/%s.png\""
	                 " alt=\"%s\" title=\"%s\""
                         " class=\"ICO20x20\" />&nbsp;%s</a>",
	       Gbl.Prefs.IconsURL,Cfg_ICON_FOLDER_COUNTRIES,
	       Cty.Alpha2,
	       Cty.Alpha2,
	       Cty.Alpha2,
	       Cty.Name[Gbl.Prefs.Language],
               Cty.Name[Gbl.Prefs.Language]);
      Act_FormEnd ();
      fprintf (Gbl.F.Out,"</li>");

      /***** Get my institutions in this country *****/
      NumInss = (unsigned) Usr_GetInssFromUsr (Gbl.Usrs.Me.UsrDat.UsrCod,
                                               Cty.CtyCod,&mysql_resIns);
      for (NumIns = 0;
	   NumIns < NumInss;
	   NumIns++)
	{
	 /***** Get next institution *****/
	 row = mysql_fetch_row (mysql_resIns);

	 /***** Get data of this institution *****/
	 Ins.InsCod = Str_ConvertStrCodToLongCod (row[0]);
	 if (!Ins_GetDataOfInstitutionByCod (&Ins,Ins_GET_BASIC_DATA))
	    Lay_ShowErrorAndExit ("Institution not found.");

	 /***** Write link to institution *****/
	 Highlight = (Gbl.CurrentCtr.Ctr.CtrCod <= 0 &&
	              Gbl.CurrentIns.Ins.InsCod == Ins.InsCod);
	 fprintf (Gbl.F.Out,"<li class=\"MY_CRSS_LNK %s\" style=\"height:25px;\">",
	          Highlight ? ClassHighlight :
			      ClassNormal);
	 IsLastItemInLevel[2] = (NumIns == NumInss - 1);
	 Lay_IndentDependingOnLevel (2,IsLastItemInLevel);
         Act_FormStart (ActMyCrs);
	 Ins_PutParamInsCod (Ins.InsCod);
	 Act_LinkFormSubmit (Act_GetActionTextFromDB (Act_GetActCod (ActSeeInsInf),ActTxt),
	                     Highlight ? ClassHighlight :
        	                         ClassNormal,NULL);
	 Log_DrawLogo (Sco_SCOPE_INS,Ins.InsCod,Ins.ShrtName,20,NULL,true);
	 fprintf (Gbl.F.Out,"&nbsp;%s</a>",Ins.FullName);
	 Act_FormEnd ();
	 fprintf (Gbl.F.Out,"</li>");

	 /***** Get my centres in this institution *****/
	 NumCtrs = (unsigned) Usr_GetCtrsFromUsr (Gbl.Usrs.Me.UsrDat.UsrCod,
	                                          Ins.InsCod,&mysql_resCtr);
	 for (NumCtr = 0;
	      NumCtr < NumCtrs;
	      NumCtr++)
	   {
	    /***** Get next centre *****/
	    row = mysql_fetch_row (mysql_resCtr);

	    /***** Get data of this centre *****/
	    Ctr.CtrCod = Str_ConvertStrCodToLongCod (row[0]);
	    if (!Ctr_GetDataOfCentreByCod (&Ctr))
	       Lay_ShowErrorAndExit ("Centre not found.");

	    /***** Write link to centre *****/
	    Highlight = (Gbl.CurrentDeg.Deg.DegCod <= 0 &&
			 Gbl.CurrentCtr.Ctr.CtrCod == Ctr.CtrCod);
	    fprintf (Gbl.F.Out,"<li class=\"MY_CRSS_LNK %s\" style=\"height:25px;\">",
	             Highlight ? ClassHighlight :
			         ClassNormal);
	    IsLastItemInLevel[3] = (NumCtr == NumCtrs - 1);
	    Lay_IndentDependingOnLevel (3,IsLastItemInLevel);
            Act_FormStart (ActMyCrs);
	    Ctr_PutParamCtrCod (Ctr.CtrCod);
	    Act_LinkFormSubmit (Act_GetActionTextFromDB (Act_GetActCod (ActSeeCtrInf),ActTxt),
	                        Highlight ? ClassHighlight :
        	                            ClassNormal,NULL);
	    Log_DrawLogo (Sco_SCOPE_CTR,Ctr.CtrCod,Ctr.ShrtName,20,NULL,true);
	    fprintf (Gbl.F.Out,"&nbsp;%s</a>",Ctr.FullName);
	    Act_FormEnd ();
	    fprintf (Gbl.F.Out,"</li>");

	    /***** Get my degrees in this centre *****/
	    NumDegs = (unsigned) Usr_GetDegsFromUsr (Gbl.Usrs.Me.UsrDat.UsrCod,
	                                             Ctr.CtrCod,&mysql_resDeg);
	    for (NumDeg = 0;
		 NumDeg < NumDegs;
		 NumDeg++)
	      {
	       /***** Get next degree *****/
	       row = mysql_fetch_row (mysql_resDeg);

	       /***** Get data of this degree *****/
	       Deg.DegCod = Str_ConvertStrCodToLongCod (row[0]);
	       if (!Deg_GetDataOfDegreeByCod (&Deg))
		  Lay_ShowErrorAndExit ("Degree not found.");

	       /***** Write link to degree *****/
	       Highlight = (Gbl.CurrentCrs.Crs.CrsCod <= 0 &&
			    Gbl.CurrentDeg.Deg.DegCod == Deg.DegCod);
	       fprintf (Gbl.F.Out,"<li class=\"MY_CRSS_LNK %s\" style=\"height:25px;\">",
	                Highlight ? ClassHighlight :
			            ClassNormal);
	       IsLastItemInLevel[4] = (NumDeg == NumDegs - 1);
	       Lay_IndentDependingOnLevel (4,IsLastItemInLevel);
               Act_FormStart (ActMyCrs);
	       Deg_PutParamDegCod (Deg.DegCod);
	       Act_LinkFormSubmit (Act_GetActionTextFromDB (Act_GetActCod (ActSeeDegInf),ActTxt),
	                           Highlight ? ClassHighlight :
        	                               ClassNormal,NULL);
	       Log_DrawLogo (Sco_SCOPE_DEG,Deg.DegCod,Deg.ShrtName,20,NULL,true);
	       fprintf (Gbl.F.Out,"&nbsp;%s</a>",Deg.FullName);
	       Act_FormEnd ();
	       fprintf (Gbl.F.Out,"</li>");

	       /***** Get my courses in this degree *****/
	       NumCrss = (unsigned) Usr_GetCrssFromUsr (Gbl.Usrs.Me.UsrDat.UsrCod,
	                                                Deg.DegCod,&mysql_resCrs);
	       for (NumCrs = 0;
		    NumCrs < NumCrss;
		    NumCrs++)
		 {
		  /***** Get next course *****/
		  row = mysql_fetch_row (mysql_resCrs);

		  /***** Get data of this course *****/
		  Crs.CrsCod = Str_ConvertStrCodToLongCod (row[0]);
		  if (!Crs_GetDataOfCourseByCod (&Crs))
		     Lay_ShowErrorAndExit ("Course not found.");

		  /***** Write link to course *****/
		  Highlight = (Gbl.CurrentCrs.Crs.CrsCod == Crs.CrsCod);
		  fprintf (Gbl.F.Out,"<li class=\"MY_CRSS_LNK %s\" style=\"height:25px;\">",
	                   Highlight ? ClassHighlight :
			               ClassNormal);
		  IsLastItemInLevel[5] = (NumCrs == NumCrss - 1);
		  Lay_IndentDependingOnLevel (5,IsLastItemInLevel);
                  Act_FormStart (ActMyCrs);
		  Crs_PutParamCrsCod (Crs.CrsCod);
		  sprintf (Gbl.Title,Txt_Go_to_X,Crs.ShrtName);
		  Act_LinkFormSubmit (Gbl.Title,
		                      Highlight ? ClassHighlight :
        	                                  ClassNormal,NULL);
		  fprintf (Gbl.F.Out,"<img src=\"%s/dot64x64.png\""
			             " alt=\"%s\" title=\"%s\""
			             " class=\"ICO20x20\" />"
			             "&nbsp;%s"
			             "</a>",
		           Gbl.Prefs.IconsURL,
		           Crs.ShrtName,
		           Crs.FullName,
		           Crs.FullName);
		  Act_FormEnd ();

		  /***** Put link to register students *****/
		  Enr_PutButtonInlineToRegisterStds (Crs.CrsCod);

		  fprintf (Gbl.F.Out,"</li>");
		 }

	       /* Free structure that stores the query result */
	       DB_FreeMySQLResult (&mysql_resCrs);
	      }

	    /* Free structure that stores the query result */
	    DB_FreeMySQLResult (&mysql_resDeg);
	   }

	 /* Free structure that stores the query result */
	 DB_FreeMySQLResult (&mysql_resCtr);
	}

      /* Free structure that stores the query result */
      DB_FreeMySQLResult (&mysql_resIns);
     }

   /* Free structure that stores the query result */
   DB_FreeMySQLResult (&mysql_resCty);

   /***** End box *****/
   fprintf (Gbl.F.Out,"</ul>");
   Box_EndBox ();
  }

/*****************************************************************************/
/*********************** Get total number of courses *************************/
/*****************************************************************************/

unsigned Crs_GetNumCrssTotal (void)
  {
   char Query[256];

   /***** Get total number of courses from database *****/
   sprintf (Query,"SELECT COUNT(*) FROM courses");
   return (unsigned) DB_QueryCOUNT (Query,"can not get the total number of courses");
  }

/*****************************************************************************/
/****************** Get number of courses in a country ***********************/
/*****************************************************************************/

unsigned Crs_GetNumCrssInCty (long CtyCod)
  {
   char Query[256];

   /***** Get number of courses in a country from database *****/
   sprintf (Query,"SELECT COUNT(*) FROM institutions,centres,degrees,courses"
	          " WHERE institutions.CtyCod=%ld"
	          " AND institutions.InsCod=centres.InsCod"
	          " AND centres.CtrCod=degrees.CtrCod"
	          " AND degrees.DegCod=courses.DegCod",
	    CtyCod);
   return (unsigned) DB_QueryCOUNT (Query,"can not get the number of courses in a country");
  }

/*****************************************************************************/
/**************** Get number of courses in an institution ********************/
/*****************************************************************************/

unsigned Crs_GetNumCrssInIns (long InsCod)
  {
   char Query[256];

   /***** Get number of courses in a degree from database *****/
   sprintf (Query,"SELECT COUNT(*) FROM centres,degrees,courses"
	          " WHERE centres.InsCod=%ld"
	          " AND centres.CtrCod=degrees.CtrCod"
	          " AND degrees.DegCod=courses.DegCod",
	    InsCod);
   return (unsigned) DB_QueryCOUNT (Query,"can not get the number of courses in an institution");
  }

/*****************************************************************************/
/******************** Get number of courses in a centre **********************/
/*****************************************************************************/

unsigned Crs_GetNumCrssInCtr (long CtrCod)
  {
   char Query[256];

   /***** Get number of courses in a degree from database *****/
   sprintf (Query,"SELECT COUNT(*) FROM degrees,courses"
	          " WHERE degrees.CtrCod=%ld"
	          " AND degrees.DegCod=courses.DegCod",
	    CtrCod);
   return (unsigned) DB_QueryCOUNT (Query,"can not get the number of courses in a centre");
  }

/*****************************************************************************/
/******************** Get number of courses in a degree **********************/
/*****************************************************************************/

unsigned Crs_GetNumCrssInDeg (long DegCod)
  {
   char Query[128];

   /***** Get number of courses in a degree from database *****/
   sprintf (Query,"SELECT COUNT(*) FROM courses"
	          " WHERE DegCod=%ld",
	    DegCod);
   return (unsigned) DB_QueryCOUNT (Query,"can not get the number of courses in a degree");
  }

/*****************************************************************************/
/********************* Get number of courses with users **********************/
/*****************************************************************************/

unsigned Crs_GetNumCrssWithUsrs (Rol_Role_t Role,const char *SubQuery)
  {
   char Query[512];

   /***** Get number of degrees with users from database *****/
   sprintf (Query,"SELECT COUNT(DISTINCT courses.CrsCod)"
                  " FROM institutions,centres,degrees,courses,crs_usr"
                  " WHERE %sinstitutions.InsCod=centres.InsCod"
                  " AND centres.CtrCod=degrees.CtrCod"
                  " AND degrees.DegCod=courses.DegCod"
                  " AND courses.CrsCod=crs_usr.CrsCod"
                  " AND crs_usr.Role=%u",
            SubQuery,(unsigned) Role);
   return (unsigned) DB_QueryCOUNT (Query,"can not get number of courses with users");
  }

/*****************************************************************************/
/*************************** Write selector of course ************************/
/*****************************************************************************/

void Crs_WriteSelectorOfCourse (void)
  {
   extern const char *Txt_Course;
   char Query[512];
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned NumCrss;
   unsigned NumCrs;
   long CrsCod;

   /***** Start form *****/
   Act_FormGoToStart (ActSeeCrsInf);
   fprintf (Gbl.F.Out,"<select id=\"crs\" name=\"crs\" style=\"width:175px;\"");
   if (Gbl.CurrentDeg.Deg.DegCod > 0)
      fprintf (Gbl.F.Out," onchange=\"document.getElementById('%s').submit();\"",
               Gbl.Form.Id);
   else
      fprintf (Gbl.F.Out," disabled=\"disabled\"");
   fprintf (Gbl.F.Out,"><option value=\"\"");
   if (Gbl.CurrentCrs.Crs.CrsCod < 0)
      fprintf (Gbl.F.Out," selected=\"selected\"");
   fprintf (Gbl.F.Out," disabled=\"disabled\">[%s]</option>",
            Txt_Course);

   if (Gbl.CurrentDeg.Deg.DegCod > 0)
     {
      /***** Get courses belonging to the current degree from database *****/
      sprintf (Query,"SELECT CrsCod,ShortName FROM courses"
                     " WHERE DegCod=%ld"
                     " ORDER BY ShortName",
               Gbl.CurrentDeg.Deg.DegCod);
      NumCrss = (unsigned) DB_QuerySELECT (Query,&mysql_res,"can not get courses of a degree");

      /***** Get courses of this degree *****/
      for (NumCrs = 0;
	   NumCrs < NumCrss;
	   NumCrs++)
        {
         /* Get next course */
         row = mysql_fetch_row (mysql_res);

         /* Get course code (row[0]) */
         if ((CrsCod = Str_ConvertStrCodToLongCod (row[0])) < 0)
            Lay_ShowErrorAndExit ("Wrong course.");

         /* Write option */
         fprintf (Gbl.F.Out,"<option value=\"%ld\"",CrsCod);
         if (Gbl.CurrentCrs.Crs.CrsCod > 0 &&
             (CrsCod == Gbl.CurrentCrs.Crs.CrsCod))
	    fprintf (Gbl.F.Out," selected=\"selected\"");
         fprintf (Gbl.F.Out,">%s</option>",row[1]);
        }

      /***** Free structure that stores the query result *****/
      DB_FreeMySQLResult (&mysql_res);
     }

   /***** End form *****/
   fprintf (Gbl.F.Out,"</select>");
   Act_FormEnd ();
  }

/*****************************************************************************/
/************************** Show courses of a degree *************************/
/*****************************************************************************/

void Crs_ShowCrssOfCurrentDeg (void)
  {
   if (Gbl.CurrentDeg.Deg.DegCod > 0)
     {
      /***** Get list of courses in this degree *****/
      Crs_GetListCoursesInDegree (Crs_ALL_COURSES_EXCEPT_REMOVED);

      /***** Write menu to select country, institution, centre and degree *****/
      Hie_WriteMenuHierarchy ();

      /***** Show list of courses *****/
      Crs_ListCourses ();

      /***** Free list of courses in this degree *****/
      Crs_FreeListCoursesInDegree (&Gbl.CurrentDeg.Deg);
     }
  }

/*****************************************************************************/
/*************** Create a list with courses in current degree ****************/
/*****************************************************************************/

static void Crs_GetListCoursesInDegree (Crs_WhatCourses_t WhatCourses)
  {
   char Query[512];
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned NumCrss;
   unsigned NumCrs;
   struct Course *Crs;

   /***** Get courses of a degree from database *****/
   switch (WhatCourses)
     {
      case Crs_ACTIVE_COURSES:
         sprintf (Query,"SELECT CrsCod,DegCod,Year,InsCrsCod,Status,RequesterUsrCod,ShortName,FullName"
                        " FROM courses WHERE DegCod=%ld AND Status=0"
                        " ORDER BY Year,ShortName",
                  Gbl.CurrentDeg.Deg.DegCod);
         break;
      case Crs_ALL_COURSES_EXCEPT_REMOVED:
         sprintf (Query,"SELECT CrsCod,DegCod,Year,InsCrsCod,Status,RequesterUsrCod,ShortName,FullName"
                        " FROM courses WHERE DegCod=%ld AND (Status & %u)=0"
                        " ORDER BY Year,ShortName",
                  Gbl.CurrentDeg.Deg.DegCod,
                  (unsigned) Crs_STATUS_BIT_REMOVED);
         break;
      default:
	 break;
     }
   NumCrss = (unsigned) DB_QuerySELECT (Query,&mysql_res,"can not get the courses of a degree");

   if (NumCrss) // Courses found...
     {
      /***** Create list with courses in degree *****/
      if ((Gbl.CurrentDeg.Deg.LstCrss = (struct Course *) calloc ((size_t) NumCrss,sizeof (struct Course))) == NULL)
          Lay_ShowErrorAndExit ("Not enough memory to store the courses of a degree.");

      /***** Get the courses in degree *****/
      for (NumCrs = 0;
	   NumCrs < NumCrss;
	   NumCrs++)
        {
         Crs = &(Gbl.CurrentDeg.Deg.LstCrss[NumCrs]);

         /* Get next course */
         row = mysql_fetch_row (mysql_res);
         Crs_GetDataOfCourseFromRow (Crs,row);
        }
     }

   Gbl.CurrentDeg.NumCrss = NumCrss;

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/********************* Free list of courses in this degree *******************/
/*****************************************************************************/

void Crs_FreeListCoursesInDegree (struct Degree *Deg)
  {
   if (Deg->LstCrss)
     {
      /***** Free memory used by the list of courses in degree *****/
      free ((void *) Deg->LstCrss);
      Deg->LstCrss = NULL;
     }
  }

/*****************************************************************************/
/********************** Write selector of my coursess ************************/
/*****************************************************************************/

void Crs_WriteSelectorMyCourses (void)
  {
   extern const char *Txt_Course;
   unsigned NumMyCrs;
   long CrsCod;
   long DegCod;
   long LastDegCod;
   char CrsShortName[Hie_MAX_BYTES_SHRT_NAME + 1];
   char DegShortName[Hie_MAX_BYTES_SHRT_NAME + 1];

   /***** Fill the list with the courses I belong to, if not filled *****/
   if (Gbl.Usrs.Me.Logged)
      Usr_GetMyCourses ();

   /***** Start form *****/
   Act_FormGoToStart (Gbl.Usrs.Me.MyCrss.Num ? ActSeeCrsInf :
                                               ActSysReqSch);

   /***** Start selector of courses *****/
   fprintf (Gbl.F.Out,"<select id=\"my_courses\" name=\"crs\""
                      " onchange=\"document.getElementById('%s').submit();\">",
            Gbl.Form.Id);

   /***** Write an option when no course selected *****/
   if (Gbl.CurrentCrs.Crs.CrsCod <= 0)	// No course selected
      fprintf (Gbl.F.Out,"<option value=\"-1\""
	                 " disabled=\"disabled\" selected=\"selected\">"
			 "%s"
			 "</option>",
	       Txt_Course);

   if (Gbl.Usrs.Me.MyCrss.Num)
     {
      /***** Write an option for each of my courses *****/
      for (NumMyCrs = 0, LastDegCod = -1L;
           NumMyCrs < Gbl.Usrs.Me.MyCrss.Num;
           NumMyCrs++)
        {
	 CrsCod = Gbl.Usrs.Me.MyCrss.Crss[NumMyCrs].CrsCod;
	 DegCod = Gbl.Usrs.Me.MyCrss.Crss[NumMyCrs].DegCod;

         Crs_GetShortNamesByCod (CrsCod,CrsShortName,DegShortName);

	 if (DegCod != LastDegCod)
	   {
	    if (LastDegCod > 0)
	       fprintf (Gbl.F.Out,"</optgroup>");
	    fprintf (Gbl.F.Out,"<optgroup label=\"%s\">",DegShortName);
	    LastDegCod = DegCod;
	   }

         fprintf (Gbl.F.Out,"<option value=\"%ld\"",
                  Gbl.Usrs.Me.MyCrss.Crss[NumMyCrs].CrsCod);
         if (CrsCod == Gbl.CurrentCrs.Crs.CrsCod)	// Course selected
            fprintf (Gbl.F.Out," selected=\"selected\"");
         fprintf (Gbl.F.Out,">%s</option>",CrsShortName);
        }

      if (LastDegCod > 0)
	 fprintf (Gbl.F.Out,"</optgroup>");
     }

   /***** Write an option with the current course
          when I don't belong to it *****/
   if (Gbl.CurrentCrs.Crs.CrsCod > 0 &&		// Course selected
       !Gbl.Usrs.Me.IBelongToCurrentCrs)	// I do not belong to it
      fprintf (Gbl.F.Out,"<option value=\"%ld\""
	                 " disabled=\"disabled\" selected=\"selected\">"
			 "%s"
			 "</option>",
	       Gbl.CurrentCrs.Crs.CrsCod,
	       Gbl.CurrentCrs.Crs.ShrtName);

   /***** End form *****/
   fprintf (Gbl.F.Out,"</select>");
   Act_FormEnd ();
  }

/*****************************************************************************/
/************************* List courses in this degree ***********************/
/*****************************************************************************/

static void Crs_ListCourses (void)
  {
   extern const char *Hlp_DEGREE_Courses;
   extern const char *Txt_Courses_of_DEGREE_X;
   extern const char *Txt_No_courses;
   extern const char *Txt_Create_another_course;
   extern const char *Txt_Create_course;
   unsigned Year;

   /***** Start box *****/
   sprintf (Gbl.Title,Txt_Courses_of_DEGREE_X,Gbl.CurrentDeg.Deg.ShrtName);
   Box_StartBox (NULL,Gbl.Title,Crs_PutIconsListCourses,
                 Hlp_DEGREE_Courses,Box_NOT_CLOSABLE);

   if (Gbl.CurrentDeg.NumCrss)	// There are courses in the current degree
     {
      /***** Start table *****/
      Tbl_StartTableWideMargin (2);
      Crs_PutHeadCoursesForSeeing ();

      /***** List the courses *****/
      for (Year = 1;
	   Year <= Deg_MAX_YEARS_PER_DEGREE;
	   Year++)
	 if (Crs_ListCoursesOfAYearForSeeing (Year))	// If this year has courses ==>
	    Gbl.RowEvenOdd = 1 - Gbl.RowEvenOdd;	// ==> change color for the next year
      Crs_ListCoursesOfAYearForSeeing (0);		// Courses without a year selected

      /***** End table *****/
      Tbl_EndTable ();
     }
   else	// No courses created in the current degree
      Ale_ShowAlert (Ale_INFO,Txt_No_courses);

   /***** Button to create course *****/
   if (Crs_CheckIfICanCreateCourses ())
     {
      Act_FormStart (ActEdiCrs);
      Btn_PutConfirmButton (Gbl.CurrentDeg.NumCrss ? Txt_Create_another_course :
	                                             Txt_Create_course);
      Act_FormEnd ();
     }

   /***** End box *****/
   Box_EndBox ();
  }

/*****************************************************************************/
/********************** Check if I can create courses ************************/
/*****************************************************************************/

static bool Crs_CheckIfICanCreateCourses (void)
  {
   return (bool) (Gbl.Usrs.Me.Role.Logged >= Rol_GST);
  }

/*****************************************************************************/
/***************** Put contextual icons in list of courses *******************/
/*****************************************************************************/

static void Crs_PutIconsListCourses (void)
  {
   /***** Put icon to edit courses *****/
   if (Crs_CheckIfICanCreateCourses ())
      Crs_PutIconToEditCourses ();

   /***** Put icon to show a figure *****/
   Gbl.Stat.FigureType = Sta_HIERARCHY;
   Sta_PutIconToShowFigure ();
  }

/*****************************************************************************/
/********************** Put link (form) to edit courses **********************/
/*****************************************************************************/

static void Crs_PutIconToEditCourses (void)
  {
   Ico_PutContextualIconToEdit (ActEdiCrs,NULL);
  }

/*****************************************************************************/
/********************* List courses of a year for seeing *********************/
/*****************************************************************************/
// Return true if this year has courses

static bool Crs_ListCoursesOfAYearForSeeing (unsigned Year)
  {
   extern const char *Txt_COURSE_With_users;
   extern const char *Txt_COURSE_Without_users;
   extern const char *Txt_YEAR_OF_DEGREE[1 + Deg_MAX_YEARS_PER_DEGREE];
   extern const char *Txt_Go_to_X;
   extern const char *Txt_COURSE_STATUS[Crs_NUM_STATUS_TXT];
   unsigned NumCrs;
   struct Course *Crs;
   const char *TxtClassNormal;
   const char *TxtClassStrong;
   const char *BgColor;
   Crs_StatusTxt_t StatusTxt;
   bool ThisYearHasCourses = false;

   /***** Write all the courses of this year *****/
   for (NumCrs = 0;
	NumCrs < Gbl.CurrentDeg.NumCrss;
	NumCrs++)
     {
      Crs = &(Gbl.CurrentDeg.Deg.LstCrss[NumCrs]);
      if (Crs->Year == Year)	// The year of the course is this?
	{
	 ThisYearHasCourses = true;
	 if (Crs->Status & Crs_STATUS_BIT_PENDING)
	   {
	    TxtClassNormal = "DAT_LIGHT";
	    TxtClassStrong = "DAT_LIGHT";
	   }
	 else
	   {
	    TxtClassNormal = "DAT";
	    TxtClassStrong = "DAT_N";
	   }
	 BgColor = (Crs->CrsCod == Gbl.CurrentCrs.Crs.CrsCod) ? "LIGHT_BLUE" :
								Gbl.ColorRows[Gbl.RowEvenOdd];

	 /* Put green tip if course has users */
	 fprintf (Gbl.F.Out,"<tr>"
			    "<td class=\"CENTER_MIDDLE %s\">"
			    "<img src=\"%s/%s16x16.gif\""
			    " alt=\"%s\" title=\"%s\""
			    " class=\"ICO20x20\" />"
			    "</td>",
		  BgColor,
		  Gbl.Prefs.IconsURL,
		  Crs->NumUsrs[Rol_UNK] ? "ok_green" :
				          "tr",
		  Crs->NumUsrs[Rol_UNK] ? Txt_COURSE_With_users :
				          Txt_COURSE_Without_users,
		  Crs->NumUsrs[Rol_UNK] ? Txt_COURSE_With_users :
				          Txt_COURSE_Without_users);

	 /* Institutional code of the course */
	 fprintf (Gbl.F.Out,"<td class=\"%s CENTER_MIDDLE %s\">"
			    "%s"
			    "</td>",
		  TxtClassNormal,BgColor,
		  Crs->InstitutionalCrsCod);

	 /* Course year */
	 fprintf (Gbl.F.Out,"<td class=\"%s CENTER_MIDDLE %s\">"
			    "%s"
			    "</td>",
		  TxtClassNormal,BgColor,
		  Txt_YEAR_OF_DEGREE[Crs->Year]);

	 /* Course full name */
	 fprintf (Gbl.F.Out,"<td class=\"%s LEFT_MIDDLE %s\">",
		  TxtClassStrong,BgColor);
	 Act_FormGoToStart (ActSeeCrsInf);
	 Crs_PutParamCrsCod (Crs->CrsCod);
	 sprintf (Gbl.Title,Txt_Go_to_X,Crs->FullName);
	 Act_LinkFormSubmit (Gbl.Title,TxtClassStrong,NULL);
	 fprintf (Gbl.F.Out,"%s</a>",
		  Crs->FullName);
	 Act_FormEnd ();
	 fprintf (Gbl.F.Out,"</td>");

	 /* Current number of teachers in this course */
	 fprintf (Gbl.F.Out,"<td class=\"%s RIGHT_MIDDLE %s\">"
			    "%u"
			    "</td>",
		  TxtClassNormal,BgColor,
		  Crs->NumUsrs[Rol_TCH] +
		  Crs->NumUsrs[Rol_NET]);

	 /* Current number of students in this course */
	 fprintf (Gbl.F.Out,"<td class=\"%s RIGHT_MIDDLE %s\">"
			    "%u"
			    "</td>",
		  TxtClassNormal,BgColor,
		  Crs->NumUsrs[Rol_STD]);

	 /* Course status */
	 StatusTxt = Crs_GetStatusTxtFromStatusBits (Crs->Status);
	 fprintf (Gbl.F.Out,"<td class=\"%s LEFT_MIDDLE %s\">",
		  TxtClassNormal,BgColor);
	 if (StatusTxt != Crs_STATUS_ACTIVE) // If active ==> do not show anything
	    fprintf (Gbl.F.Out,"%s",Txt_COURSE_STATUS[StatusTxt]);
	 fprintf (Gbl.F.Out,"</td>"
			    "</tr>");
	}
     }

   return ThisYearHasCourses;
  }

/*****************************************************************************/
/****************** Put forms to edit courses in this degree *****************/
/*****************************************************************************/

void Crs_EditCourses (void)
  {
   extern const char *Hlp_DEGREE_Courses;
   extern const char *Txt_Courses_of_DEGREE_X;

   /***** Get list of courses in this degree *****/
   Crs_GetListCoursesInDegree (Crs_ALL_COURSES_EXCEPT_REMOVED);

   /***** Get list of degrees in this centre *****/
   Deg_GetListDegsOfCurrentCtr ();

   /***** Start box *****/
   sprintf (Gbl.Alert.Txt,Txt_Courses_of_DEGREE_X,Gbl.CurrentDeg.Deg.ShrtName);
   Box_StartBox (NULL,Gbl.Alert.Txt,Crs_PutIconToViewCourses,
                 Hlp_DEGREE_Courses,Box_NOT_CLOSABLE);

   /***** Put a form to create or request a new course *****/
   Crs_PutFormToCreateCourse ();

   /***** Forms to edit current courses *****/
   if (Gbl.CurrentDeg.NumCrss)
      Crs_ListCoursesForEdition ();

   /***** End box *****/
   Box_EndBox ();

   /***** Free list of courses in this degree *****/
   Crs_FreeListCoursesInDegree (&Gbl.CurrentDeg.Deg);

   /***** Free list of degrees in this centre *****/
   Deg_FreeListDegs (&Gbl.CurrentCtr.Ctr.Degs);
  }

/*****************************************************************************/
/**************** Put contextual icons in edition of courses *****************/
/*****************************************************************************/

static void Crs_PutIconToViewCourses (void)
  {
   Ico_PutContextualIconToView (ActSeeCrs,NULL);
  }

/*****************************************************************************/
/********************* List current courses for edition **********************/
/*****************************************************************************/

static void Crs_ListCoursesForEdition (void)
  {
   unsigned Year;

   /***** Write heading *****/
   Tbl_StartTableWide (2);
   Crs_PutHeadCoursesForEdition ();

   /***** List the courses *****/
   for (Year = 1;
	Year <= Deg_MAX_YEARS_PER_DEGREE;
	Year++)
      Crs_ListCoursesOfAYearForEdition (Year);
   Crs_ListCoursesOfAYearForEdition (0);

   /***** End table *****/
   Tbl_EndTable ();
  }

/*****************************************************************************/
/******************** List courses of a year for edition *********************/
/*****************************************************************************/

static void Crs_ListCoursesOfAYearForEdition (unsigned Year)
  {
   extern const char *Txt_YEAR_OF_DEGREE[1 + Deg_MAX_YEARS_PER_DEGREE];
   extern const char *Txt_COURSE_STATUS[Crs_NUM_STATUS_TXT];
   struct Course *Crs;
   unsigned YearAux;
   unsigned NumCrs;
   struct UsrData UsrDat;
   bool ICanEdit;
   Crs_StatusTxt_t StatusTxt;

   /***** Initialize structure with user's data *****/
   Usr_UsrDataConstructor (&UsrDat);

   /***** List courses of a given year *****/
   for (NumCrs = 0;
	NumCrs < Gbl.CurrentDeg.NumCrss;
	NumCrs++)
     {
      Crs = &(Gbl.CurrentDeg.Deg.LstCrss[NumCrs]);
      if (Crs->Year == Year)
	{
	 ICanEdit = Crs_CheckIfICanEdit (Crs);

	 /* Put icon to remove course */
	 fprintf (Gbl.F.Out,"<tr>"
			    "<td class=\"BM\">");
	 if (Crs->NumUsrs[Rol_UNK] ||	// Course has users ==> deletion forbidden
	     !ICanEdit)
	    Ico_PutIconRemovalNotAllowed ();
	 else	// Crs->NumUsrs == 0 && ICanEdit
	   {
	    Act_FormStart (ActRemCrs);
	    Crs_PutParamOtherCrsCod (Crs->CrsCod);
	    Ico_PutIconRemove ();
	    Act_FormEnd ();
	   }
	 fprintf (Gbl.F.Out,"</td>");

	 /* Course code */
	 fprintf (Gbl.F.Out,"<td class=\"DAT CODE\">"
			    "%ld"
			    "</td>",
		  Crs->CrsCod);

	 /* Institutional code of the course */
	 fprintf (Gbl.F.Out,"<td class=\"DAT CENTER_MIDDLE\">");
	 if (ICanEdit)
	   {
	    Act_FormStart (ActChgInsCrsCod);
	    Crs_PutParamOtherCrsCod (Crs->CrsCod);
	    fprintf (Gbl.F.Out,"<input type=\"text\" name=\"InsCrsCod\""
			       " maxlength=\"%u\" value=\"%s\""
			       " class=\"INPUT_INS_CODE\""
			       " onchange=\"document.getElementById('%s').submit();\" />",
		     Crs_MAX_CHARS_INSTITUTIONAL_CRS_COD,
		     Crs->InstitutionalCrsCod,
		     Gbl.Form.Id);
	    Act_FormEnd ();
	   }
	 else
	    fprintf (Gbl.F.Out,"%s",Crs->InstitutionalCrsCod);
	 fprintf (Gbl.F.Out,"</td>");

	 /* Course year */
	 fprintf (Gbl.F.Out,"<td class=\"DAT CENTER_MIDDLE\">");
	 if (ICanEdit)
	   {
	    Act_FormStart (ActChgCrsYea);
	    Crs_PutParamOtherCrsCod (Crs->CrsCod);
	    fprintf (Gbl.F.Out,"<select name=\"OthCrsYear\""
			       " style=\"width:50px;\""
			       " onchange=\"document.getElementById('%s').submit();\">",
		     Gbl.Form.Id);
	    for (YearAux = 0;
		 YearAux <= Deg_MAX_YEARS_PER_DEGREE;
		 YearAux++)	// All the years are permitted because it's possible to move this course to another degree (with other active years)
	       fprintf (Gbl.F.Out,"<option value=\"%u\"%s>%s</option>",
			YearAux,
			YearAux == Crs->Year ? " selected=\"selected\"" :
					       "",
			Txt_YEAR_OF_DEGREE[YearAux]);
	    fprintf (Gbl.F.Out,"</select>");
	    Act_FormEnd ();
	   }
	 else
	    fprintf (Gbl.F.Out,"%s",Txt_YEAR_OF_DEGREE[Crs->Year]);
	 fprintf (Gbl.F.Out,"</td>");

	 /* Course short name */
	 fprintf (Gbl.F.Out,"<td class=\"DAT LEFT_MIDDLE\">");
	 if (ICanEdit)
	   {
	    Act_FormStart (ActRenCrsSho);
	    Crs_PutParamOtherCrsCod (Crs->CrsCod);
	    fprintf (Gbl.F.Out,"<input type=\"text\" name=\"ShortName\""
			       " maxlength=\"%u\" value=\"%s\""
			       " class=\"INPUT_SHORT_NAME\""
			       " onchange=\"document.getElementById('%s').submit();\" />",
		     Hie_MAX_CHARS_SHRT_NAME,Crs->ShrtName,
		     Gbl.Form.Id);
	    Act_FormEnd ();
	   }
	 else
	    fprintf (Gbl.F.Out,"%s",Crs->ShrtName);
	 fprintf (Gbl.F.Out,"</td>");

	 /* Course full name */
	 fprintf (Gbl.F.Out,"<td class=\"DAT LEFT_MIDDLE\">");
	 if (ICanEdit)
	   {
	    Act_FormStart (ActRenCrsFul);
	    Crs_PutParamOtherCrsCod (Crs->CrsCod);
	    fprintf (Gbl.F.Out,"<input type=\"text\" name=\"FullName\""
			       " maxlength=\"%u\" value=\"%s\""
			       " class=\"INPUT_FULL_NAME\""
			       " onchange=\"document.getElementById('%s').submit();\" />",
		     Hie_MAX_CHARS_FULL_NAME,Crs->FullName,
		     Gbl.Form.Id);
	    Act_FormEnd ();
	   }
	 else
	    fprintf (Gbl.F.Out,"%s",Crs->FullName);
	 fprintf (Gbl.F.Out,"</td>");

	 /* Current number of teachers in this course */
	 fprintf (Gbl.F.Out,"<td class=\"DAT RIGHT_MIDDLE\">"
			    "%u"
			    "</td>",
		  Crs->NumUsrs[Rol_TCH] +
		  Crs->NumUsrs[Rol_NET]);

	 /* Current number of students in this course */
	 fprintf (Gbl.F.Out,"<td class=\"DAT RIGHT_MIDDLE\">"
			    "%u"
			    "</td>",
		  Crs->NumUsrs[Rol_STD]);

	 /* Course requester */
	 UsrDat.UsrCod = Crs->RequesterUsrCod;
	 Usr_ChkUsrCodAndGetAllUsrDataFromUsrCod (&UsrDat);
	 fprintf (Gbl.F.Out,"<td class=\"DAT INPUT_REQUESTER LEFT_TOP\">");
	 Msg_WriteMsgAuthor (&UsrDat,true,NULL);
	 fprintf (Gbl.F.Out,"</td>");

	 /* Course status */
	 StatusTxt = Crs_GetStatusTxtFromStatusBits (Crs->Status);
	 fprintf (Gbl.F.Out,"<td class=\"DAT LEFT_MIDDLE\">");
	 if (Gbl.Usrs.Me.Role.Logged >= Rol_DEG_ADM &&
	     StatusTxt == Crs_STATUS_PENDING)
	   {
	    Act_FormStart (ActChgCrsSta);
	    Crs_PutParamOtherCrsCod (Crs->CrsCod);
	    fprintf (Gbl.F.Out,"<select name=\"Status\" class=\"INPUT_STATUS\""
			       " onchange=\"document.getElementById('%s').submit();\">"
			       "<option value=\"%u\" selected=\"selected\">%s</option>"
			       "<option value=\"%u\">%s</option>"
			       "</select>",
		     Gbl.Form.Id,
		     (unsigned) Crs_GetStatusBitsFromStatusTxt (Crs_STATUS_PENDING),
		     Txt_COURSE_STATUS[Crs_STATUS_PENDING],
		     (unsigned) Crs_GetStatusBitsFromStatusTxt (Crs_STATUS_ACTIVE),
		     Txt_COURSE_STATUS[Crs_STATUS_ACTIVE]);
	    Act_FormEnd ();
	   }
         else if (StatusTxt != Crs_STATUS_ACTIVE)	// If active ==> do not show anything
	    fprintf (Gbl.F.Out,"%s",Txt_COURSE_STATUS[StatusTxt]);
	 fprintf (Gbl.F.Out,"</td>"
			    "</tr>");
	}
     }

   /***** Free memory used for user's data *****/
   Usr_UsrDataDestructor (&UsrDat);
  }

/*****************************************************************************/
/************** Check if I can edit, remove, etc. a course *******************/
/*****************************************************************************/

static bool Crs_CheckIfICanEdit (struct Course *Crs)
  {
   return (bool) (Gbl.Usrs.Me.Role.Logged >= Rol_DEG_ADM ||		// I am a degree administrator or higher
                  ((Crs->Status & Crs_STATUS_BIT_PENDING) != 0 &&		// Course is not yet activated
                   Gbl.Usrs.Me.UsrDat.UsrCod == Crs->RequesterUsrCod));		// I am the requester
  }

/*****************************************************************************/
/******************* Set StatusTxt depending on status bits ******************/
/*****************************************************************************/
// Crs_STATUS_UNKNOWN = 0	// Other
// Crs_STATUS_ACTIVE  = 1	// 00 (Status == 0)
// Crs_STATUS_PENDING = 2	// 01 (Status == Crs_STATUS_BIT_PENDING)
// Crs_STATUS_REMOVED = 3	// 1- (Status & Crs_STATUS_BIT_REMOVED)

static Crs_StatusTxt_t Crs_GetStatusTxtFromStatusBits (Crs_Status_t Status)
  {
   if (Status == 0)
      return Crs_STATUS_ACTIVE;
   if (Status == Crs_STATUS_BIT_PENDING)
      return Crs_STATUS_PENDING;
   if (Status & Crs_STATUS_BIT_REMOVED)
      return Crs_STATUS_REMOVED;
   return Crs_STATUS_UNKNOWN;
  }

/*****************************************************************************/
/******************* Set status bits depending on StatusTxt ******************/
/*****************************************************************************/
// Crs_STATUS_UNKNOWN = 0	// Other
// Crs_STATUS_ACTIVE  = 1	// 00 (Status == 0)
// Crs_STATUS_PENDING = 2	// 01 (Status == Crs_STATUS_BIT_PENDING)
// Crs_STATUS_REMOVED = 3	// 1- (Status & Crs_STATUS_BIT_REMOVED)

static Crs_Status_t Crs_GetStatusBitsFromStatusTxt (Crs_StatusTxt_t StatusTxt)
  {
   switch (StatusTxt)
     {
      case Crs_STATUS_UNKNOWN:
      case Crs_STATUS_ACTIVE:
	 return (Crs_Status_t) 0;
      case Crs_STATUS_PENDING:
	 return Crs_STATUS_BIT_PENDING;
      case Crs_STATUS_REMOVED:
	 return Crs_STATUS_BIT_REMOVED;
     }
   return (Crs_Status_t) 0;
  }

/*****************************************************************************/
/*********************** Put a form to create a new course *******************/
/*****************************************************************************/

static void Crs_PutFormToCreateCourse (void)
  {
   extern const char *Txt_New_course;
   extern const char *Txt_YEAR_OF_DEGREE[1 + Deg_MAX_YEARS_PER_DEGREE];
   extern const char *Txt_Create_course;
   unsigned Year;

   /***** Start form *****/
   if (Gbl.Usrs.Me.Role.Logged >= Rol_DEG_ADM)
      Act_FormStart (ActNewCrs);
   else if (Gbl.Usrs.Me.Role.Max >= Rol_GST)
      Act_FormStart (ActReqCrs);
   else
      Lay_ShowErrorAndExit ("You can not edit courses.");

   /***** Start box and table *****/
   Box_StartBoxTable (NULL,Txt_New_course,NULL,
                      NULL,Box_NOT_CLOSABLE,2);

   /***** Write heading *****/
   Crs_PutHeadCoursesForEdition ();

   /***** Column to remove course, disabled here *****/
   fprintf (Gbl.F.Out,"<tr>"
		      "<td class=\"BM\"></td>");

   /***** Course code *****/
   fprintf (Gbl.F.Out,"<td class=\"CODE\"></td>");

   /***** Institutional code of the course *****/
   fprintf (Gbl.F.Out,"<td class=\"CENTER_MIDDLE\">"
                      "<input type=\"text\" name=\"InsCrsCod\""
                      " maxlength=\"%u\" value=\"%s\""
                      " class=\"INPUT_INS_CODE\" />"
                      "</td>",
            Crs_MAX_CHARS_INSTITUTIONAL_CRS_COD,
            Gbl.Degs.EditingCrs.InstitutionalCrsCod);

   /***** Year *****/
   fprintf (Gbl.F.Out,"<td class=\"CENTER_MIDDLE\">"
	              "<select name=\"OthCrsYear\" style=\"width:50px;\">");
   for (Year = 0;
	Year <= Deg_MAX_YEARS_PER_DEGREE;
        Year++)
      fprintf (Gbl.F.Out,"<option value=\"%u\"%s>%s</option>",
	       Year,
	       Year == Gbl.Degs.EditingCrs.Year ? " selected=\"selected\"" :
				                  "",
	       Txt_YEAR_OF_DEGREE[Year]);
   fprintf (Gbl.F.Out,"</select>"
	              "</td>");

   /***** Course short name *****/
   fprintf (Gbl.F.Out,"<td class=\"LEFT_MIDDLE\">"
                      "<input type=\"text\" name=\"ShortName\""
                      " maxlength=\"%u\" value=\"%s\""
                      " class=\"INPUT_SHORT_NAME\""
                      " required=\"required\" />"
                      "</td>",
            Hie_MAX_CHARS_SHRT_NAME,Gbl.Degs.EditingCrs.ShrtName);

   /***** Course full name *****/
   fprintf (Gbl.F.Out,"<td class=\"LEFT_MIDDLE\">"
                      "<input type=\"text\" name=\"FullName\""
                      " maxlength=\"%u\" value=\"%s\""
                      " class=\"INPUT_FULL_NAME\""
                      " required=\"required\" />"
                      "</td>",
            Hie_MAX_CHARS_FULL_NAME,Gbl.Degs.EditingCrs.FullName);

   /***** Current number of teachers in this course *****/
   fprintf (Gbl.F.Out,"<td class=\"DAT RIGHT_MIDDLE\">"
	              "0"
	              "</td>");

   /***** Current number of students in this course *****/
   fprintf (Gbl.F.Out,"<td class=\"DAT RIGHT_MIDDLE\">"
	              "0"
	              "</td>");

   /***** Course requester *****/
   fprintf (Gbl.F.Out,"<td class=\"DAT INPUT_REQUESTER LEFT_TOP\">");
   Msg_WriteMsgAuthor (&Gbl.Usrs.Me.UsrDat,true,NULL);
   fprintf (Gbl.F.Out,"</td>");

   /***** Course status *****/
   fprintf (Gbl.F.Out,"<td class=\"DAT LEFT_MIDDLE\">"
	              "</td>"
		      "</tr>");

   /***** End table, send button and end box *****/
   Box_EndBoxTableWithButton (Btn_CREATE_BUTTON,Txt_Create_course);

   /***** End form *****/
   Act_FormEnd ();
  }

/*****************************************************************************/
/******************** Write header with fields of a course *******************/
/*****************************************************************************/

static void Crs_PutHeadCoursesForSeeing (void)
  {
   extern const char *Txt_Institutional_BR_code;
   extern const char *Txt_Year_OF_A_DEGREE;
   extern const char *Txt_Course;
   extern const char *Txt_ROLES_PLURAL_BRIEF_Abc[Rol_NUM_ROLES];

   fprintf (Gbl.F.Out,"<tr>"
                      "<th class=\"BM\"></th>"
                      "<th class=\"CENTER_MIDDLE\">"
                      "%s"
                      "</th>"
                      "<th class=\"CENTER_MIDDLE\">"
                      "%s"
                      "</th>"
                      "<th class=\"LEFT_MIDDLE\">"
                      "%s"
                      "</th>"
                      "<th class=\"RIGHT_MIDDLE\">"
                      "%s"
                      "</th>"
                      "<th class=\"RIGHT_MIDDLE\">"
                      "%s"
                      "</th>"
                      "<th class=\"LEFT_MIDDLE\">"
                      "</th>"
                      "</tr>",
            Txt_Institutional_BR_code,
            Txt_Year_OF_A_DEGREE,
            Txt_Course,
            Txt_ROLES_PLURAL_BRIEF_Abc[Rol_TCH],
            Txt_ROLES_PLURAL_BRIEF_Abc[Rol_STD]);
  }

/*****************************************************************************/
/******************** Write header with fields of a course *******************/
/*****************************************************************************/

static void Crs_PutHeadCoursesForEdition (void)
  {
   extern const char *Txt_Code;
   extern const char *Txt_Institutional_code;
   extern const char *Txt_optional;
   extern const char *Txt_Year_OF_A_DEGREE;
   extern const char *Txt_Short_name_of_the_course;
   extern const char *Txt_Full_name_of_the_course;
   extern const char *Txt_ROLES_PLURAL_BRIEF_Abc[Rol_NUM_ROLES];
   extern const char *Txt_Requester;

   fprintf (Gbl.F.Out,"<tr>"
                      "<th class=\"BM\"></th>"
                      "<th class=\"RIGHT_MIDDLE\">"
                      "%s"
                      "</th>"
                      "<th class=\"CENTER_MIDDLE\">"
                      "%s (%s)"
                      "</th>"
                      "<th class=\"CENTER_MIDDLE\">"
                      "%s"
                      "</th>"
                      "<th class=\"LEFT_MIDDLE\">"
                      "%s"
                      "</th>"
                      "<th class=\"LEFT_MIDDLE\">"
                      "%s"
                      "</th>"
                      "<th class=\"RIGHT_MIDDLE\">"
                      "%s"
                      "</th>"
                      "<th class=\"RIGHT_MIDDLE\">"
                      "%s"
                      "</th>"
                      "<th class=\"LEFT_MIDDLE\">"
                      "%s"
                      "</th>"
                      "<th class=\"LEFT_MIDDLE\">"
                      "</th>"
                      "</tr>",
            Txt_Code,
            Txt_Institutional_code,Txt_optional,
            Txt_Year_OF_A_DEGREE,
            Txt_Short_name_of_the_course,
            Txt_Full_name_of_the_course,
            Txt_ROLES_PLURAL_BRIEF_Abc[Rol_TCH],
            Txt_ROLES_PLURAL_BRIEF_Abc[Rol_STD],
            Txt_Requester);
  }

/*****************************************************************************/
/****************** Receive form to request a new course *********************/
/*****************************************************************************/

void Crs_RecFormReqCrs (void)
  {
   Crs_RecFormRequestOrCreateCrs ((unsigned) Crs_STATUS_BIT_PENDING);
  }

/*****************************************************************************/
/******************* Receive form to create a new course *********************/
/*****************************************************************************/

void Crs_RecFormNewCrs (void)
  {
   Crs_RecFormRequestOrCreateCrs (0);
  }

/*****************************************************************************/
/************* Receive form to request or create a new course ****************/
/*****************************************************************************/

static void Crs_RecFormRequestOrCreateCrs (unsigned Status)
  {
   extern const char *Txt_The_course_X_already_exists;
   extern const char *Txt_You_must_specify_the_short_name_and_the_full_name_of_the_new_course;
   extern const char *Txt_The_year_X_is_not_allowed;
   extern const char *Txt_YEAR_OF_DEGREE[1 + Deg_MAX_YEARS_PER_DEGREE];
   struct Degree Deg;

   /***** Get parameters from form *****/
   /* Set course degree */
   Deg.DegCod = Gbl.Degs.EditingCrs.DegCod = Gbl.CurrentDeg.Deg.DegCod;

   /* Get parameters of the new course */
   Crs_GetParamsNewCourse (&Gbl.Degs.EditingCrs);

   /***** Check if year is correct *****/
   Deg_GetDataOfDegreeByCod (&Deg);
   if (Gbl.Degs.EditingCrs.Year <= Deg_MAX_YEARS_PER_DEGREE)	// If year is valid
     {
      if (Gbl.Degs.EditingCrs.ShrtName[0] &&
	  Gbl.Degs.EditingCrs.FullName[0])	// If there's a course name
	{
	 /***** If name of course was in database... *****/
	 if (Crs_CheckIfCrsNameExistsInYearOfDeg ("ShortName",Gbl.Degs.EditingCrs.ShrtName,-1L,
	                                          Gbl.Degs.EditingCrs.DegCod,Gbl.Degs.EditingCrs.Year))
	   {
            Gbl.Alert.Type = Ale_WARNING;
	    sprintf (Gbl.Alert.Txt,Txt_The_course_X_already_exists,
	             Gbl.Degs.EditingCrs.ShrtName);
	   }
	 else if (Crs_CheckIfCrsNameExistsInYearOfDeg ("FullName",Gbl.Degs.EditingCrs.FullName,-1L,
	                                               Gbl.Degs.EditingCrs.DegCod,Gbl.Degs.EditingCrs.Year))
	   {
            Gbl.Alert.Type = Ale_WARNING;
	    sprintf (Gbl.Alert.Txt,Txt_The_course_X_already_exists,
		     Gbl.Degs.EditingCrs.FullName);
	   }
	 else	// Add new requested course to database
	    Crs_CreateCourse (Status);
	}
      else	// If there is not a course name
	{
         Gbl.Alert.Type = Ale_WARNING;
	 sprintf (Gbl.Alert.Txt,"%s",Txt_You_must_specify_the_short_name_and_the_full_name_of_the_new_course);
	}
     }
   else	// Year not valid
     {
      Gbl.Alert.Type = Ale_WARNING;
      sprintf (Gbl.Alert.Txt,Txt_The_year_X_is_not_allowed,
               Gbl.Degs.EditingCrs.Year);
     }
  }

/*****************************************************************************/
/************** Get the parameters of a new course from form *****************/
/*****************************************************************************/

static void Crs_GetParamsNewCourse (struct Course *Crs)
  {
   char YearStr[2 + 1];

   /***** Get parameters of the course from form *****/
   /* Get institutional code */
   Par_GetParToText ("InsCrsCod",Crs->InstitutionalCrsCod,Crs_MAX_BYTES_INSTITUTIONAL_CRS_COD);

   /* Get year */
   Par_GetParToText ("OthCrsYear",YearStr,2);
   Crs->Year = Deg_ConvStrToYear (YearStr);

   /* Get course short name */
   Par_GetParToText ("ShortName",Crs->ShrtName,Hie_MAX_BYTES_SHRT_NAME);

   /* Get course full name */
   Par_GetParToText ("FullName",Crs->FullName,Hie_MAX_BYTES_FULL_NAME);
  }

/*****************************************************************************/
/************* Add a new requested course to pending requests ****************/
/*****************************************************************************/
// Gbl.Degs.EditingCrs must hold the course beeing edited

static void Crs_CreateCourse (unsigned Status)
  {
   extern const char *Txt_Created_new_course_X;
   char Query[512 +
              Hie_MAX_BYTES_SHRT_NAME +
              Hie_MAX_BYTES_FULL_NAME];

   /***** Insert new course into pending requests *****/
   sprintf (Query,"INSERT INTO courses"
	          " (DegCod,Year,InsCrsCod,Status,RequesterUsrCod,ShortName,FullName)"
                  " VALUES"
                  " (%ld,%u,'%s',%u,%ld,'%s','%s')",
            Gbl.Degs.EditingCrs.DegCod,Gbl.Degs.EditingCrs.Year,
            Gbl.Degs.EditingCrs.InstitutionalCrsCod,
            Status,
            Gbl.Usrs.Me.UsrDat.UsrCod,
            Gbl.Degs.EditingCrs.ShrtName,Gbl.Degs.EditingCrs.FullName);
   Gbl.Degs.EditingCrs.CrsCod = DB_QueryINSERTandReturnCode (Query,"can not create a new course");

   /***** Create success message *****/
   Gbl.Alert.Type = Ale_SUCCESS;
   sprintf (Gbl.Alert.Txt,Txt_Created_new_course_X,
            Gbl.Degs.EditingCrs.FullName);
  }

/*****************************************************************************/
/************************ Request removing of a course ***********************/
/*****************************************************************************/

void Crs_RemoveCourse (void)
  {
   extern const char *Txt_To_remove_a_course_you_must_first_remove_all_users_in_the_course;
   extern const char *Txt_Course_X_removed;
   extern const char *Txt_You_dont_have_permission_to_edit_this_course;
   struct Course Crs;

   /***** Get course code *****/
   Crs.CrsCod = Crs_GetAndCheckParamOtherCrsCod (1);

   /***** Get data of the course from database *****/
   Crs_GetDataOfCourseByCod (&Crs);

   if (Crs_CheckIfICanEdit (&Crs))
     {
      /***** Check if this course has users *****/
      if (Crs.NumUsrs[Rol_UNK])	// Course has users ==> don't remove
         Ale_ShowAlert (Ale_WARNING,Txt_To_remove_a_course_you_must_first_remove_all_users_in_the_course);
      else			// Course has no users ==> remove it
        {
         /***** Remove course *****/
         Crs_RemoveCourseCompletely (Crs.CrsCod);

         /***** Write message to show the change made *****/
         sprintf (Gbl.Alert.Txt,Txt_Course_X_removed,
                  Crs.FullName);
         Ale_ShowAlert (Ale_SUCCESS,Gbl.Alert.Txt);
        }
     }
   else
      Ale_ShowAlert (Ale_WARNING,Txt_You_dont_have_permission_to_edit_this_course);

   /***** Show the form again *****/
   Crs_EditCourses ();
  }

/*****************************************************************************/
/********************* Get data of a course from its code ********************/
/*****************************************************************************/

bool Crs_GetDataOfCourseByCod (struct Course *Crs)
  {
   char Query[1024];
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   bool CrsFound = false;

   /***** Clear data *****/
   Crs->DegCod = -1L;
   Crs->Year = 0;
   Crs->Status = (Crs_Status_t) 0;
   Crs->RequesterUsrCod = -1L;
   Crs->ShrtName[0] = '\0';
   Crs->FullName[0] = '\0';
   Crs->NumUsrs[Rol_UNK] =
   Crs->NumUsrs[Rol_STD] =
   Crs->NumUsrs[Rol_NET] =
   Crs->NumUsrs[Rol_TCH] = 0;

   /***** Check if course code is correct *****/
   if (Crs->CrsCod > 0)
     {
      /***** Get data of a course from database *****/
      sprintf (Query,"SELECT CrsCod,DegCod,Year,InsCrsCod,Status,RequesterUsrCod,ShortName,FullName"
		     " FROM courses WHERE CrsCod=%ld",
	       Crs->CrsCod);
      if (DB_QuerySELECT (Query,&mysql_res,"can not get data of a course")) // Course found...
	{
	 /***** Get data of the course *****/
	 row = mysql_fetch_row (mysql_res);
	 Crs_GetDataOfCourseFromRow (Crs,row);

         /* Set return value */
	 CrsFound = true;
	}

      /***** Free structure that stores the query result *****/
      DB_FreeMySQLResult (&mysql_res);
     }

   return CrsFound;
  }

/*****************************************************************************/
/********** Get data of a course from a row resulting of a query *************/
/*****************************************************************************/

static void Crs_GetDataOfCourseFromRow (struct Course *Crs,MYSQL_ROW row)
  {
   /***** Get course code (row[0]) *****/
   if ((Crs->CrsCod = Str_ConvertStrCodToLongCod (row[0])) < 0)
      Lay_ShowErrorAndExit ("Wrong code of course.");

   /***** Get code of degree (row[1]) *****/
   Crs->DegCod = Str_ConvertStrCodToLongCod (row[1]);

   /***** Get year (row[2]) *****/
   Crs->Year = Deg_ConvStrToYear (row[2]);

   /***** Get institutional course code (row[3]) *****/
   Str_Copy (Crs->InstitutionalCrsCod,row[3],
             Crs_MAX_BYTES_INSTITUTIONAL_CRS_COD);

   /***** Get course status (row[4]) *****/
   if (sscanf (row[4],"%u",&(Crs->Status)) != 1)
      Lay_ShowErrorAndExit ("Wrong course status.");

   /***** Get requester user'code (row[5]) *****/
   Crs->RequesterUsrCod = Str_ConvertStrCodToLongCod (row[5]);

   /***** Get the short name of the course (row[6]) *****/
   Str_Copy (Crs->ShrtName,row[6],
             Hie_MAX_BYTES_SHRT_NAME);

   /***** Get the full name of the course (row[7]) *****/
   Str_Copy (Crs->FullName,row[7],
             Hie_MAX_BYTES_FULL_NAME);

   /***** Get number of users *****/
   Crs->NumUsrs[Rol_STD] = Usr_GetNumUsrsInCrs (Rol_STD,Crs->CrsCod);
   Crs->NumUsrs[Rol_NET] = Usr_GetNumUsrsInCrs (Rol_NET,Crs->CrsCod);
   Crs->NumUsrs[Rol_TCH] = Usr_GetNumUsrsInCrs (Rol_TCH,Crs->CrsCod);
   Crs->NumUsrs[Rol_UNK] = Crs->NumUsrs[Rol_STD] +
	                   Crs->NumUsrs[Rol_NET] +
	                   Crs->NumUsrs[Rol_TCH];
  }

/*****************************************************************************/
/******* Get the short names of degree and course from a course code *********/
/*****************************************************************************/

static void Crs_GetShortNamesByCod (long CrsCod,
                                    char CrsShortName[Hie_MAX_BYTES_SHRT_NAME + 1],
                                    char DegShortName[Hie_MAX_BYTES_SHRT_NAME + 1])
  {
   char Query[512];
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;

   DegShortName[0] = CrsShortName[0] = '\0';

   if (CrsCod > 0)
     {
      /***** Get the short name of a degree from database *****/
      sprintf (Query,"SELECT courses.ShortName,degrees.ShortName"
		     " FROM courses,degrees"
		     " WHERE courses.CrsCod=%ld"
		     " AND courses.DegCod=degrees.DegCod",
	       CrsCod);
      if (DB_QuerySELECT (Query,&mysql_res,"can not get the short name of a course") == 1)
	{
	 /***** Get the short name of this course *****/
	 row = mysql_fetch_row (mysql_res);

	 Str_Copy (CrsShortName,row[0],
	           Hie_MAX_BYTES_SHRT_NAME);
	 Str_Copy (DegShortName,row[1],
	           Hie_MAX_BYTES_SHRT_NAME);
	}

      /***** Free structure that stores the query result *****/
      DB_FreeMySQLResult (&mysql_res);
     }
  }

/*****************************************************************************/
/****************************** Remove a course ******************************/
/*****************************************************************************/

void Crs_RemoveCourseCompletely (long CrsCod)
  {
   char Query[128];

   if (CrsCod > 0)
     {
      /***** Empty course *****/
      Crs_EmptyCourseCompletely (CrsCod);

      /***** Remove course from table of last accesses to courses in database *****/
      sprintf (Query,"DELETE FROM crs_last WHERE CrsCod=%ld",CrsCod);
      DB_QueryDELETE (Query,"can not remove a course");

      /***** Remove course from table of courses in database *****/
      sprintf (Query,"DELETE FROM courses WHERE CrsCod=%ld",CrsCod);
      DB_QueryDELETE (Query,"can not remove a course");
     }
  }

/*****************************************************************************/
/********** Empty a course (remove all its information and users) ************/
/*****************************************************************************/
// Start removing less important things to more important things;
// so, in case of failure, important things can been removed in the future

static void Crs_EmptyCourseCompletely (long CrsCod)
  {
   struct Course Crs;
   char PathRelCrs[PATH_MAX + 1];
   char Query[512];

   if (CrsCod > 0)
     {
      /***** Get course data *****/
      Crs.CrsCod = CrsCod;
      Crs_GetDataOfCourseByCod (&Crs);

      /***** Remove all the students in the course *****/
      Enr_RemAllStdsInCrs (&Crs);

      /***** Set all the notifications from the course as removed,
	     except notifications about new messages *****/
      Ntf_MarkNotifInCrsAsRemoved (-1L,CrsCod);

      /***** Remove information of the course ****/
      /* Remove timetable of the course */
      sprintf (Query,"DELETE FROM timetable_crs WHERE CrsCod=%ld",CrsCod);
      DB_QueryDELETE (Query,"can not remove the timetable of a course");

      /* Remove other information of the course */
      sprintf (Query,"DELETE FROM crs_info_src WHERE CrsCod=%ld",CrsCod);
      DB_QueryDELETE (Query,"can not remove info sources of a course");

      sprintf (Query,"DELETE FROM crs_info_txt WHERE CrsCod=%ld",CrsCod);
      DB_QueryDELETE (Query,"can not remove info of a course");

      /***** Remove exam announcements in the course *****/
      /* Mark all exam announcements in the course as deleted */
      sprintf (Query,"UPDATE exam_announcements SET Status=%u"
		     " WHERE CrsCod=%ld",
	       (unsigned) Exa_DELETED_EXAM_ANNOUNCEMENT,CrsCod);
      DB_QueryUPDATE (Query,"can not remove exam announcements of a course");

      /***** Remove course cards of the course *****/
      /* Remove content of course cards */
      sprintf (Query,"DELETE FROM crs_records USING crs_record_fields,crs_records"
		     " WHERE crs_record_fields.CrsCod=%ld"
		     " AND crs_record_fields.FieldCod=crs_records.FieldCod",
	       CrsCod);
      DB_QueryDELETE (Query,"can not remove content of cards in a course");

      /* Remove definition of fields in course cards */
      sprintf (Query,"DELETE FROM crs_record_fields WHERE CrsCod=%ld",CrsCod);
      DB_QueryDELETE (Query,"can not remove fields of cards in a course");

      /***** Remove information related to files in course,
             including groups and projects,
             so this function must be called
             before removing groups and projects *****/
      Brw_RemoveCrsFilesFromDB (CrsCod);

      /***** Remove assignments of the course *****/
      Asg_RemoveCrsAssignments (CrsCod);

      /***** Remove projects of the course *****/
      Prj_RemoveCrsProjects (CrsCod);

      /***** Remove attendance events of the course *****/
      Att_RemoveCrsAttEvents (CrsCod);

      /***** Remove notices in the course *****/
      /* Copy all notices from the course to table of deleted notices */
      sprintf (Query,"INSERT INTO notices_deleted"
		     " (NotCod,CrsCod,UsrCod,CreatTime,Content,NumNotif)"
		     " SELECT NotCod,CrsCod,UsrCod,CreatTime,Content,NumNotif FROM notices"
		     " WHERE CrsCod=%ld",
	       CrsCod);
      DB_QueryINSERT (Query,"can not remove notices in a course");
      /* Remove all notices from the course */
      sprintf (Query,"DELETE FROM notices WHERE CrsCod=%ld",CrsCod);
      DB_QueryDELETE (Query,"can not remove notices in a course");

      /***** Remove all the threads and posts in forums of the course *****/
      For_RemoveForums (Sco_SCOPE_CRS,CrsCod);

      /***** Remove surveys of the course *****/
      Svy_RemoveSurveys (Sco_SCOPE_CRS,CrsCod);

      /***** Remove all test exams made in the course *****/
      Tst_RemoveCrsTestResults (CrsCod);

      /***** Remove all tests questions in the course *****/
      Tst_RemoveCrsTests (CrsCod);

      /***** Remove groups in the course *****/
      /* Remove all the users in groups in the course */
      sprintf (Query,"DELETE FROM crs_grp_usr"
		     " USING crs_grp_types,crs_grp,crs_grp_usr"
		     " WHERE crs_grp_types.CrsCod=%ld"
		     " AND crs_grp_types.GrpTypCod=crs_grp.GrpTypCod"
		     " AND crs_grp.GrpCod=crs_grp_usr.GrpCod",
	       CrsCod);
      DB_QueryDELETE (Query,"can not remove users from groups of a course");

      /* Remove all the groups in the course */
      sprintf (Query,"DELETE FROM crs_grp"
		     " USING crs_grp_types,crs_grp"
		     " WHERE crs_grp_types.CrsCod=%ld"
		     " AND crs_grp_types.GrpTypCod=crs_grp.GrpTypCod",
	       CrsCod);
      DB_QueryDELETE (Query,"can not remove groups of a course");

      /* Remove all the group types in the course */
      sprintf (Query,"DELETE FROM crs_grp_types"
		     " WHERE CrsCod=%ld",
	       CrsCod);
      DB_QueryDELETE (Query,"can not remove types of group of a course");

      /***** Remove users' requests for inscription in the course *****/
      sprintf (Query,"DELETE FROM crs_usr_requests WHERE CrsCod=%ld",
	       CrsCod);
      DB_QueryDELETE (Query,"can not remove requests for inscription to a course");

      /***** Remove possible users remaining in the course (teachers) *****/
      sprintf (Query,"DELETE FROM crs_usr WHERE CrsCod=%ld",
	       CrsCod);
      DB_QueryDELETE (Query,"can not remove users from a course");

      /***** Remove directories of the course *****/
      sprintf (PathRelCrs,"%s/%s/%ld",
	       Cfg_PATH_SWAD_PRIVATE,Cfg_FOLDER_CRS,CrsCod);
      Fil_RemoveTree (PathRelCrs);
      sprintf (PathRelCrs,"%s/%s/%ld",
	       Cfg_PATH_SWAD_PUBLIC,Cfg_FOLDER_CRS,CrsCod);
      Fil_RemoveTree (PathRelCrs);
     }
  }

/*****************************************************************************/
/***** Change the institutional code of a course in course configuration *****/
/*****************************************************************************/

void Crs_ChangeInsCrsCodInConfig (void)
  {
   extern const char *Txt_The_institutional_code_of_the_course_X_has_changed_to_Y;
   extern const char *Txt_The_institutional_code_of_the_course_X_has_not_changed;
   extern const char *Txt_You_dont_have_permission_to_edit_this_course;
   char NewInstitutionalCrsCod[Crs_MAX_BYTES_INSTITUTIONAL_CRS_COD + 1];

   /***** Get institutional code from form *****/
   Par_GetParToText ("InsCrsCod",NewInstitutionalCrsCod,Crs_MAX_BYTES_INSTITUTIONAL_CRS_COD);

   /***** Change the institutional course code *****/
   if (strcmp (NewInstitutionalCrsCod,Gbl.CurrentCrs.Crs.InstitutionalCrsCod))
     {
      Crs_UpdateInstitutionalCrsCod (&Gbl.CurrentCrs.Crs,NewInstitutionalCrsCod);

      sprintf (Gbl.Alert.Txt,Txt_The_institutional_code_of_the_course_X_has_changed_to_Y,
	       Gbl.CurrentCrs.Crs.ShrtName,NewInstitutionalCrsCod);
     }
   else	// The same institutional code
      sprintf (Gbl.Alert.Txt,Txt_The_institutional_code_of_the_course_X_has_not_changed,
	       Gbl.CurrentCrs.Crs.ShrtName);
  }

/*****************************************************************************/
/************** Change the institutional code of a course ********************/
/*****************************************************************************/

void Crs_ChangeInsCrsCod (void)
  {
   extern const char *Txt_The_institutional_code_of_the_course_X_has_changed_to_Y;
   extern const char *Txt_The_institutional_code_of_the_course_X_has_not_changed;
   extern const char *Txt_You_dont_have_permission_to_edit_this_course;
   char NewInstitutionalCrsCod[Crs_MAX_BYTES_INSTITUTIONAL_CRS_COD + 1];

   /***** Get parameters from form *****/
   /* Get course code */
   Gbl.Degs.EditingCrs.CrsCod = Crs_GetAndCheckParamOtherCrsCod (1);

   /* Get institutional code */
   Par_GetParToText ("InsCrsCod",NewInstitutionalCrsCod,Crs_MAX_BYTES_INSTITUTIONAL_CRS_COD);

   /* Get data of the course */
   Crs_GetDataOfCourseByCod (&Gbl.Degs.EditingCrs);

   if (Crs_CheckIfICanEdit (&Gbl.Degs.EditingCrs))
     {
      /***** Change the institutional course code *****/
      if (strcmp (NewInstitutionalCrsCod,Gbl.Degs.EditingCrs.InstitutionalCrsCod))
        {
         Crs_UpdateInstitutionalCrsCod (&Gbl.Degs.EditingCrs,NewInstitutionalCrsCod);
         Gbl.Alert.Type = Ale_SUCCESS;
         sprintf (Gbl.Alert.Txt,Txt_The_institutional_code_of_the_course_X_has_changed_to_Y,
                  Gbl.Degs.EditingCrs.ShrtName,NewInstitutionalCrsCod);
        }
      else	// The same institutional code
	{
	 Gbl.Alert.Type = Ale_INFO;
         sprintf (Gbl.Alert.Txt,Txt_The_institutional_code_of_the_course_X_has_not_changed,
                  Gbl.Degs.EditingCrs.ShrtName);
	}
     }
   else
     {
      Gbl.Alert.Type = Ale_WARNING;
      sprintf (Gbl.Alert.Txt,"%s",Txt_You_dont_have_permission_to_edit_this_course);
     }
  }

/*****************************************************************************/
/***************** Change the degree of the current course *******************/
/*****************************************************************************/

void Crs_ChangeCrsDegInConfig (void)
  {
   extern const char *Txt_In_the_year_X_of_the_degree_Y_already_existed_a_course_with_the_name_Z;
   extern const char *Txt_YEAR_OF_DEGREE[1 + Deg_MAX_YEARS_PER_DEGREE];
   extern const char *Txt_The_course_X_has_been_moved_to_the_degree_Y;
   struct Degree NewDeg;

   /***** Get parameter with degree code *****/
   NewDeg.DegCod = Deg_GetAndCheckParamOtherDegCod (1);

   /***** Check if degree has changed *****/
   if (NewDeg.DegCod != Gbl.CurrentCrs.Crs.DegCod)
     {
      /***** Get data of new degree *****/
      Deg_GetDataOfDegreeByCod (&NewDeg);

      /***** If name of course was in database in the new degree... *****/
      if (Crs_CheckIfCrsNameExistsInYearOfDeg ("ShortName",Gbl.CurrentCrs.Crs.ShrtName,-1L,
                                               NewDeg.DegCod,Gbl.CurrentCrs.Crs.Year))
	{
	 Gbl.Alert.Type = Ale_WARNING;
	 sprintf (Gbl.Alert.Txt,Txt_In_the_year_X_of_the_degree_Y_already_existed_a_course_with_the_name_Z,
		  Txt_YEAR_OF_DEGREE[Gbl.CurrentCrs.Crs.Year],NewDeg.FullName,Gbl.CurrentCrs.Crs.ShrtName);
	}
      else if (Crs_CheckIfCrsNameExistsInYearOfDeg ("FullName",Gbl.CurrentCrs.Crs.FullName,-1L,
                                                    NewDeg.DegCod,Gbl.CurrentCrs.Crs.Year))
	{
	 Gbl.Alert.Type = Ale_WARNING;
	 sprintf (Gbl.Alert.Txt,Txt_In_the_year_X_of_the_degree_Y_already_existed_a_course_with_the_name_Z,
		  Txt_YEAR_OF_DEGREE[Gbl.CurrentCrs.Crs.Year],NewDeg.FullName,Gbl.CurrentCrs.Crs.FullName);
	}
      else	// Update degree in database
	{
	 /***** Update degree in table of courses *****/
	 Crs_UpdateCrsDegDB (Gbl.CurrentCrs.Crs.CrsCod,NewDeg.DegCod);
	 Gbl.CurrentCrs.Crs.DegCod =
	 Gbl.CurrentDeg.Deg.DegCod = NewDeg.DegCod;

	 /***** Initialize again current course, degree, centre... *****/
      	 Hie_InitHierarchy ();

	 /***** Create message to show the change made *****/
	 Gbl.Alert.Type = Ale_SUCCESS;
	 sprintf (Gbl.Alert.Txt,Txt_The_course_X_has_been_moved_to_the_degree_Y,
		  Gbl.CurrentCrs.Crs.FullName,
		  Gbl.CurrentDeg.Deg.FullName);
	}
     }
  }

/*****************************************************************************/
/** Show message of success after changing a course in course configuration **/
/*****************************************************************************/

void Crs_ContEditAfterChgCrsInConfig (void)
  {
   /***** Write error/success message *****/
   Ale_ShowPendingAlert ();

   /***** Show the form again *****/
   Crs_ShowIntroduction ();
  }

/*****************************************************************************/
/********************** Update degree in table of courses ********************/
/*****************************************************************************/

static void Crs_UpdateCrsDegDB (long CrsCod,long DegCod)
  {
   char Query[128];

   /***** Update degree in table of courses *****/
   sprintf (Query,"UPDATE courses SET DegCod=%ld WHERE CrsCod=%ld",
	    DegCod,CrsCod);
   DB_QueryUPDATE (Query,"can not move course to another degree");
  }

/*****************************************************************************/
/*********** Change the year of a course in course configuration *************/
/*****************************************************************************/

void Crs_ChangeCrsYearInConfig (void)
  {
   extern const char *Txt_The_course_X_already_exists_in_year_Y;
   extern const char *Txt_YEAR_OF_DEGREE[1 + Deg_MAX_YEARS_PER_DEGREE];
   extern const char *Txt_The_year_of_the_course_X_has_changed;
   extern const char *Txt_The_year_X_is_not_allowed;
   char YearStr[2 + 1];
   unsigned NewYear;

   /***** Get parameter with year/semester *****/
   Par_GetParToText ("OthCrsYear",YearStr,2);
   NewYear = Deg_ConvStrToYear (YearStr);

   if (NewYear <= Deg_MAX_YEARS_PER_DEGREE)	// If year is valid
     {
      /***** If name of course was in database in the new year... *****/
      if (Crs_CheckIfCrsNameExistsInYearOfDeg ("ShortName",Gbl.CurrentCrs.Crs.ShrtName,-1L,
                                               Gbl.CurrentCrs.Crs.DegCod,NewYear))
	{
	 Gbl.Alert.Type = Ale_WARNING;
	 sprintf (Gbl.Alert.Txt,Txt_The_course_X_already_exists_in_year_Y,
		  Gbl.CurrentCrs.Crs.ShrtName,Txt_YEAR_OF_DEGREE[NewYear]);
	}
      else if (Crs_CheckIfCrsNameExistsInYearOfDeg ("FullName",Gbl.CurrentCrs.Crs.FullName,-1L,
                                                    Gbl.CurrentCrs.Crs.DegCod,NewYear))
	{
	 Gbl.Alert.Type = Ale_WARNING;
	 sprintf (Gbl.Alert.Txt,Txt_The_course_X_already_exists_in_year_Y,
		  Gbl.CurrentCrs.Crs.FullName,Txt_YEAR_OF_DEGREE[NewYear]);
	}
      else	// Update year in database
	{
	 /***** Update year in table of courses *****/
         Crs_UpdateCrsYear (&Gbl.CurrentCrs.Crs,NewYear);

	 /***** Create message to show the change made *****/
	 Gbl.Alert.Type = Ale_SUCCESS;
	 sprintf (Gbl.Alert.Txt,Txt_The_year_of_the_course_X_has_changed,
		  Gbl.CurrentCrs.Crs.ShrtName);
	}
     }
   else	// Year not valid
     {
      Gbl.Alert.Type = Ale_WARNING;
      sprintf (Gbl.Alert.Txt,Txt_The_year_X_is_not_allowed,NewYear);
     }
  }

/*****************************************************************************/
/************************ Change the year of a course ************************/
/*****************************************************************************/

void Crs_ChangeCrsYear (void)
  {
   extern const char *Txt_The_course_X_already_exists_in_year_Y;
   extern const char *Txt_YEAR_OF_DEGREE[1 + Deg_MAX_YEARS_PER_DEGREE];
   extern const char *Txt_The_year_of_the_course_X_has_changed;
   extern const char *Txt_The_year_X_is_not_allowed;
   extern const char *Txt_You_dont_have_permission_to_edit_this_course;
   struct Degree Deg;
   char YearStr[2 + 1];
   unsigned NewYear;

   /***** Get parameters from form *****/
   /* Get course code */
   Gbl.Degs.EditingCrs.CrsCod = Crs_GetAndCheckParamOtherCrsCod (1);

   /* Get parameter with year */
   Par_GetParToText ("OthCrsYear",YearStr,2);
   NewYear = Deg_ConvStrToYear (YearStr);

   Crs_GetDataOfCourseByCod (&Gbl.Degs.EditingCrs);

   if (Crs_CheckIfICanEdit (&Gbl.Degs.EditingCrs))
     {
      Deg.DegCod = Gbl.Degs.EditingCrs.DegCod;
      Deg_GetDataOfDegreeByCod (&Deg);

      if (NewYear <= Deg_MAX_YEARS_PER_DEGREE)	// If year is valid
        {
         /***** If name of course was in database in the new year... *****/
         if (Crs_CheckIfCrsNameExistsInYearOfDeg ("ShortName",Gbl.Degs.EditingCrs.ShrtName,-1L,
                                                  Gbl.Degs.EditingCrs.DegCod,NewYear))
           {
	    Gbl.Alert.Type = Ale_WARNING;
            sprintf (Gbl.Alert.Txt,Txt_The_course_X_already_exists_in_year_Y,
                     Gbl.Degs.EditingCrs.ShrtName,Txt_YEAR_OF_DEGREE[NewYear]);
           }
         else if (Crs_CheckIfCrsNameExistsInYearOfDeg ("FullName",Gbl.Degs.EditingCrs.FullName,-1L,
                                                       Gbl.Degs.EditingCrs.DegCod,NewYear))
           {
	    Gbl.Alert.Type = Ale_WARNING;
            sprintf (Gbl.Alert.Txt,Txt_The_course_X_already_exists_in_year_Y,
                     Gbl.Degs.EditingCrs.FullName,Txt_YEAR_OF_DEGREE[NewYear]);
           }
         else	// Update year in database
           {
            /***** Update year in table of courses *****/
            Crs_UpdateCrsYear (&Gbl.Degs.EditingCrs,NewYear);

            /***** Create message to show the change made *****/
	    Gbl.Alert.Type = Ale_SUCCESS;
            sprintf (Gbl.Alert.Txt,Txt_The_year_of_the_course_X_has_changed,
                     Gbl.Degs.EditingCrs.ShrtName);
           }
        }
      else	// Year not valid
        {
	 Gbl.Alert.Type = Ale_WARNING;
         sprintf (Gbl.Alert.Txt,Txt_The_year_X_is_not_allowed,NewYear);
        }
     }
   else
     {
      Gbl.Alert.Type = Ale_WARNING;
      sprintf (Gbl.Alert.Txt,"%s",Txt_You_dont_have_permission_to_edit_this_course);
     }
  }

/*****************************************************************************/
/****************** Change the year/semester of a course *********************/
/*****************************************************************************/

static void Crs_UpdateCrsYear (struct Course *Crs,unsigned NewYear)
  {
   char Query[128];

   /***** Update year/semester in table of courses *****/
   sprintf (Query,"UPDATE courses SET Year=%u WHERE CrsCod=%ld",
	    NewYear,Crs->CrsCod);
   DB_QueryUPDATE (Query,"can not update the year of a course");

   /***** Copy course year/semester *****/
   Crs->Year = NewYear;
  }

/*****************************************************************************/
/************* Change the institutional course code of a course **************/
/*****************************************************************************/

void Crs_UpdateInstitutionalCrsCod (struct Course *Crs,const char *NewInstitutionalCrsCod)
  {
   char Query[512];

   /***** Update institutional course code in table of courses *****/
   sprintf (Query,"UPDATE courses SET InsCrsCod='%s' WHERE CrsCod=%ld",
            NewInstitutionalCrsCod,Crs->CrsCod);
   DB_QueryUPDATE (Query,"can not update the institutional code of the current course");

   /***** Copy institutional course code *****/
   Str_Copy (Crs->InstitutionalCrsCod,NewInstitutionalCrsCod,
             Crs_MAX_BYTES_INSTITUTIONAL_CRS_COD);
  }

/*****************************************************************************/
/********************* Change the short name of a course *********************/
/*****************************************************************************/

void Crs_RenameCourseShort (void)
  {
   Gbl.Degs.EditingCrs.CrsCod = Crs_GetAndCheckParamOtherCrsCod (1);
   Crs_RenameCourse (&Gbl.Degs.EditingCrs,Cns_SHRT_NAME);
  }

void Crs_RenameCourseShortInConfig (void)
  {
   Crs_RenameCourse (&Gbl.CurrentCrs.Crs,Cns_SHRT_NAME);
  }

/*****************************************************************************/
/********************* Change the full name of a course **********************/
/*****************************************************************************/

void Crs_RenameCourseFull (void)
  {
   Gbl.Degs.EditingCrs.CrsCod = Crs_GetAndCheckParamOtherCrsCod (1);
   Crs_RenameCourse (&Gbl.Degs.EditingCrs,Cns_FULL_NAME);
  }

void Crs_RenameCourseFullInConfig (void)
  {
   Crs_RenameCourse (&Gbl.CurrentCrs.Crs,Cns_FULL_NAME);
  }

/*****************************************************************************/
/************************ Change the name of a course ************************/
/*****************************************************************************/

static void Crs_RenameCourse (struct Course *Crs,Cns_ShrtOrFullName_t ShrtOrFullName)
  {
   extern const char *Txt_You_can_not_leave_the_name_of_the_course_X_empty;
   extern const char *Txt_The_course_X_already_exists;
   extern const char *Txt_The_name_of_the_course_X_has_changed_to_Y;
   extern const char *Txt_The_name_of_the_course_X_has_not_changed;
   extern const char *Txt_You_dont_have_permission_to_edit_this_course;
   const char *ParamName = NULL;	// Initialized to avoid warning
   const char *FieldName = NULL;	// Initialized to avoid warning
   unsigned MaxBytes = 0;		// Initialized to avoid warning
   char *CurrentCrsName = NULL;		// Initialized to avoid warning
   char NewCrsName[Hie_MAX_BYTES_FULL_NAME + 1];

   switch (ShrtOrFullName)
     {
      case Cns_SHRT_NAME:
         ParamName = "ShortName";
         FieldName = "ShortName";
         MaxBytes = Hie_MAX_BYTES_SHRT_NAME;
         CurrentCrsName = Crs->ShrtName;
         break;
      case Cns_FULL_NAME:
         ParamName = "FullName";
         FieldName = "FullName";
         MaxBytes = Hie_MAX_BYTES_FULL_NAME;
         CurrentCrsName = Crs->FullName;
         break;
     }

   /***** Get parameters from form *****/
   /* Get the new name for the course */
   Par_GetParToText (ParamName,NewCrsName,MaxBytes);

   /***** Get from the database the data of the degree *****/
   Crs_GetDataOfCourseByCod (Crs);

   if (Crs_CheckIfICanEdit (Crs))
     {
      /***** Check if new name is empty *****/
      if (!NewCrsName[0])
        {
	 Gbl.Alert.Type = Ale_WARNING;
         sprintf (Gbl.Alert.Txt,Txt_You_can_not_leave_the_name_of_the_course_X_empty,
                  CurrentCrsName);
        }
      else
        {
         /***** Check if old and new names are the same (this happens when user press enter with no changes in the form) *****/
         if (strcmp (CurrentCrsName,NewCrsName))	// Different names
           {
            /***** If course was in database... *****/
            if (Crs_CheckIfCrsNameExistsInYearOfDeg (ParamName,NewCrsName,Crs->CrsCod,
                                                     Crs->DegCod,Crs->Year))
              {
	       Gbl.Alert.Type = Ale_WARNING;
               sprintf (Gbl.Alert.Txt,Txt_The_course_X_already_exists,
                        NewCrsName);
              }
            else
              {
               /* Update the table changing old name by new name */
               Crs_UpdateCrsNameDB (Crs->CrsCod,FieldName,NewCrsName);

               /* Create message to show the change made */
	       Gbl.Alert.Type = Ale_SUCCESS;
               sprintf (Gbl.Alert.Txt,Txt_The_name_of_the_course_X_has_changed_to_Y,
                        CurrentCrsName,NewCrsName);

               /* Change current course name in order to display it properly */
               Str_Copy (CurrentCrsName,NewCrsName,
                         MaxBytes);
              }
           }
         else	// The same name
           {
	    Gbl.Alert.Type = Ale_INFO;
            sprintf (Gbl.Alert.Txt,Txt_The_name_of_the_course_X_has_not_changed,
                     CurrentCrsName);
           }
        }
     }
   else
     {
      Gbl.Alert.Type = Ale_WARNING;
      sprintf (Gbl.Alert.Txt,"%s",Txt_You_dont_have_permission_to_edit_this_course);
     }
  }

/*****************************************************************************/
/********** Check if the name of course exists in existing courses ***********/
/*****************************************************************************/

static bool Crs_CheckIfCrsNameExistsInYearOfDeg (const char *FieldName,const char *Name,long CrsCod,
                                                 long DegCod,unsigned Year)
  {
   char Query[256 + Hie_MAX_BYTES_FULL_NAME];

   /***** Get number of courses in a year of a degree and with a name from database *****/
   sprintf (Query,"SELECT COUNT(*) FROM courses"
                  " WHERE DegCod=%ld AND Year=%u"
                  " AND %s='%s' AND CrsCod<>%ld",
            DegCod,Year,FieldName,Name,CrsCod);
   return (DB_QueryCOUNT (Query,"can not check if the name of a course already existed") != 0);
  }

/*****************************************************************************/
/***************** Update course name in table of courses ********************/
/*****************************************************************************/

static void Crs_UpdateCrsNameDB (long CrsCod,const char *FieldName,const char *NewCrsName)
  {
   char Query[128 + Hie_MAX_BYTES_FULL_NAME];

   /***** Update course changing old name by new name *****/
   sprintf (Query,"UPDATE courses SET %s='%s' WHERE CrsCod=%ld",
	    FieldName,NewCrsName,CrsCod);
   DB_QueryUPDATE (Query,"can not update the name of a course");
  }

/*****************************************************************************/
/*********************** Change the status of a course ***********************/
/*****************************************************************************/

void Crs_ChangeCrsStatus (void)
  {
   extern const char *Txt_The_status_of_the_course_X_has_changed;
   char Query[256];
   Crs_Status_t Status;
   Crs_StatusTxt_t StatusTxt;

   /***** Get parameters from form *****/
   /* Get course code */
   Gbl.Degs.EditingCrs.CrsCod = Crs_GetAndCheckParamOtherCrsCod (1);

   /* Get parameter with status */
   Status = (Crs_Status_t)
	    Par_GetParToUnsignedLong ("Status",
	                              0,
	                              (unsigned long) Crs_MAX_STATUS,
                                      (unsigned long) Crs_WRONG_STATUS);
   if (Status == Crs_WRONG_STATUS)
      Lay_ShowErrorAndExit ("Wrong status.");
   StatusTxt = Crs_GetStatusTxtFromStatusBits (Status);
   Status = Crs_GetStatusBitsFromStatusTxt (StatusTxt);	// New status

   /***** Get data of course *****/
   Crs_GetDataOfCourseByCod (&Gbl.Degs.EditingCrs);

   /***** Update status in table of courses *****/
   sprintf (Query,"UPDATE courses SET Status=%u WHERE CrsCod=%ld",
            (unsigned) Status,Gbl.Degs.EditingCrs.CrsCod);
   DB_QueryUPDATE (Query,"can not update the status of a course");
   Gbl.Degs.EditingCrs.Status = Status;

   /***** Create message to show the change made *****/
   Gbl.Alert.Type = Ale_SUCCESS;
   sprintf (Gbl.Alert.Txt,Txt_The_status_of_the_course_X_has_changed,
            Gbl.Degs.EditingCrs.ShrtName);
  }

/*****************************************************************************/
/************* Show message of success after changing a course ***************/
/*****************************************************************************/

void Crs_ContEditAfterChgCrs (void)
  {
   bool PutButtonToRequestRegistration;

   /***** Start alert *****/
   Ale_ShowAlertAndButton1 (Gbl.Alert.Type,Gbl.Alert.Txt);

   if (Gbl.Alert.Type == Ale_SUCCESS)
     {
      /***** Put button to go to course changed *****/
      Crs_PutButtonToGoToCrs ();

      /***** Put button to request my registration in course *****/
      PutButtonToRequestRegistration = false;
      switch (Gbl.Usrs.Me.Role.Logged)
        {
	 case Rol_GST:	// I do not belong to any course
	    PutButtonToRequestRegistration = true;
	    break;
	 case Rol_USR:
	    PutButtonToRequestRegistration = !Usr_CheckIfUsrBelongsToCrs (Gbl.Usrs.Me.UsrDat.UsrCod,
					                                  Gbl.Degs.EditingCrs.CrsCod,
					                                  false);
            break;
	 case Rol_STD:
	 case Rol_NET:
	 case Rol_TCH:
	    if (Gbl.Degs.EditingCrs.CrsCod != Gbl.CurrentCrs.Crs.CrsCod)
	       PutButtonToRequestRegistration = !Usr_CheckIfUsrBelongsToCrs (Gbl.Usrs.Me.UsrDat.UsrCod,
									     Gbl.Degs.EditingCrs.CrsCod,
									     false);
	    break;
	 default:
	    break;

        }
      if (PutButtonToRequestRegistration)
	 Crs_PutButtonToRegisterInCrs ();
     }

   /***** End alert *****/
   Ale_ShowAlertAndButton2 (ActUnk,NULL,NULL,NULL,Btn_NO_BUTTON,NULL);

   /***** Show the form again *****/
   Crs_EditCourses ();
  }

/*****************************************************************************/
/************************ Put button to go to course *************************/
/*****************************************************************************/
// Gbl.Degs.EditingCrs is the course that is beeing edited
// Gbl.CurrentCrs.Crs is the current course

static void Crs_PutButtonToGoToCrs (void)
  {
   extern const char *Txt_Go_to_X;

   // If the course being edited is different to the current one...
   if (Gbl.Degs.EditingCrs.CrsCod != Gbl.CurrentCrs.Crs.CrsCod)
     {
      Act_FormStart (ActSeeCrsInf);
      Crs_PutParamCrsCod (Gbl.Degs.EditingCrs.CrsCod);
      sprintf (Gbl.Title,Txt_Go_to_X,Gbl.Degs.EditingCrs.ShrtName);
      Btn_PutConfirmButton (Gbl.Title);
      Act_FormEnd ();
     }
  }

/*****************************************************************************/
/************************ Put button to go to course *************************/
/*****************************************************************************/
// Gbl.Degs.EditingCrs is the course that is beeing edited
// Gbl.CurrentCrs.Crs is the current course

static void Crs_PutButtonToRegisterInCrs (void)
  {
   extern const char *Txt_Register_me_in_X;

   Act_FormStart (ActReqSignUp);
   // If the course beeing edited is different to the current one...
   if (Gbl.Degs.EditingCrs.CrsCod != Gbl.CurrentCrs.Crs.CrsCod)
      Crs_PutParamCrsCod (Gbl.Degs.EditingCrs.CrsCod);
   sprintf (Gbl.Title,Txt_Register_me_in_X,Gbl.Degs.EditingCrs.ShrtName);
   Btn_PutCreateButton (Gbl.Title);
   Act_FormEnd ();
  }

/*****************************************************************************/
/************************* Select one of my courses **************************/
/*****************************************************************************/

void Crs_ReqSelectOneOfMyCourses (void)
  {
   /***** Search / select more courses *****/
   fprintf (Gbl.F.Out,"<div class=\"CONTEXT_MENU\">");
   Crs_PutLinkToSearchCourses ();
   fprintf (Gbl.F.Out,"</div>");

   /***** Select one of my courses *****/
   /* Fill the list with the courses I belong to, if not filled */
   Usr_GetMyCourses ();

   if (Gbl.Usrs.Me.MyCrss.Num)
      /* Show my courses */
      Crs_WriteListMyCoursesToSelectOne ();
   else	// I am not enroled in any course
      /* Show help to enrol me */
      Hlp_ShowHelpWhatWouldYouLikeToDo ();
  }

/*****************************************************************************/
/******************* Put a link (form) to search courses *********************/
/*****************************************************************************/

static void Crs_PutLinkToSearchCourses (void)
  {
   extern const char *Txt_Search_courses;

   /***** Put form to search / select courses *****/
   Lay_PutContextualLink ( Gbl.CurrentCrs.Crs.CrsCod > 0 ? ActCrsReqSch :
                          (Gbl.CurrentDeg.Deg.DegCod > 0 ? ActDegReqSch :
                          (Gbl.CurrentCtr.Ctr.CtrCod > 0 ? ActCtrReqSch :
                          (Gbl.CurrentIns.Ins.InsCod > 0 ? ActInsReqSch :
                          (Gbl.CurrentCty.Cty.CtyCod > 0 ? ActCtyReqSch :
                                                           ActSysReqSch)))),
                          NULL,Sch_PutLinkToSearchCoursesParams,
			  "search64x64.gif",
			  Txt_Search_courses,Txt_Search_courses,
                          NULL);
  }

static void Sch_PutLinkToSearchCoursesParams (void)	// TODO: Move to search module
  {
   Sco_PutParamScope ("ScopeSch",Sco_SCOPE_SYS);
   Par_PutHiddenParamUnsigned ("WhatToSearch",(unsigned) Sch_SEARCH_COURSES);
  }

/*****************************************************************************/
/****************** Put a link (form) to select my courses *******************/
/*****************************************************************************/

void Crs_PutFormToSelectMyCourses (void)
  {
   extern const char *Txt_My_courses;

   /***** Put form to search / select courses *****/
   Lay_PutContextualLink (ActMyCrs,NULL,NULL,
                          "hierarchy64x64.gif",
                          Txt_My_courses,Txt_My_courses,
                          NULL);
  }

/*****************************************************************************/
/******************** Write parameter with code of course ********************/
/*****************************************************************************/

void Crs_PutParamCrsCod (long CrsCod)
  {
   Par_PutHiddenParamLong ("crs",CrsCod);
  }

/*****************************************************************************/
/******************** Write parameter with code of course ********************/
/*****************************************************************************/

static void Crs_PutParamOtherCrsCod (long CrsCod)
  {
   Par_PutHiddenParamLong ("OthCrsCod",CrsCod);
  }

/*****************************************************************************/
/********************* Get parameter with code of course *********************/
/*****************************************************************************/

static long Crs_GetAndCheckParamOtherCrsCod (long MinCodAllowed)
  {
   long CrsCod;

   /***** Get and check parameter with code of course *****/
   if ((CrsCod = Par_GetParToLong ("OthCrsCod")) < MinCodAllowed)
      Lay_ShowErrorAndExit ("Code of course is missing or invalid.");

   return CrsCod;
  }

/*****************************************************************************/
/************************** Write courses of a user **************************/
/*****************************************************************************/

void Crs_GetAndWriteCrssOfAUsr (const struct UsrData *UsrDat,Rol_Role_t Role)
  {
   extern const char *Txt_USER_in_COURSE;
   extern const char *Txt_User[Usr_NUM_SEXS];
   extern const char *Txt_ROLES_SINGUL_Abc[Rol_NUM_ROLES][Usr_NUM_SEXS];
   extern const char *Txt_Degree;
   extern const char *Txt_Year_OF_A_DEGREE;
   extern const char *Txt_Course;
   extern const char *Txt_ROLES_PLURAL_BRIEF_Abc[Rol_NUM_ROLES];
   char SubQuery[32];
   char Query[1024];
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned NumCrss;
   unsigned NumCrs;

   /***** Get courses of a user from database *****/
   if (Role == Rol_UNK)
      SubQuery[0] = '\0';	// Role == Rol_UNK ==> any role
   else
      sprintf (SubQuery," AND crs_usr.Role=%u",(unsigned) Role);
   sprintf (Query,"SELECT degrees.DegCod,courses.CrsCod,degrees.ShortName,degrees.FullName,"
                  "courses.Year,courses.FullName,centres.ShortName,crs_usr.Accepted"
                  " FROM crs_usr,courses,degrees,centres"
                  " WHERE crs_usr.UsrCod=%ld%s"
                  " AND crs_usr.CrsCod=courses.CrsCod"
                  " AND courses.DegCod=degrees.DegCod"
                  " AND degrees.CtrCod=centres.CtrCod"
                  " ORDER BY degrees.FullName,courses.Year,courses.FullName",
            UsrDat->UsrCod,SubQuery);

   /***** List the courses (one row per course) *****/
   if ((NumCrss = (unsigned) DB_QuerySELECT (Query,&mysql_res,"can not get courses of a user")))
     {
      /* Start box and table */
      Box_StartBoxTable ("100%",NULL,NULL,
                         NULL,Box_NOT_CLOSABLE,2);

      /* Heading row */
      sprintf (Gbl.Title,Txt_USER_in_COURSE,
               Role == Rol_UNK ? Txt_User[Usr_SEX_UNKNOWN] : // Role == Rol_UNK ==> any role
        	                     Txt_ROLES_SINGUL_Abc[Role][UsrDat->Sex]);
      fprintf (Gbl.F.Out,"<tr>"
                         "<th colspan=\"7\" class=\"LEFT_MIDDLE\">%s:</th>"
                         "</tr>"
	                 "<tr>"
                         "<th class=\"BM\"></th>"
                         "<th class=\"BM\"></th>"
                         "<th class=\"LEFT_MIDDLE\">"
                         "%s"
                         "</th>"
                         "<th class=\"CENTER_MIDDLE\">"
                         "%s"
                         "</th>"
                         "<th class=\"LEFT_MIDDLE\">"
                         "%s"
                         "</th>"
                         "<th class=\"RIGHT_MIDDLE\">"
                         "%s"
                         "</th>"
                         "<th class=\"RIGHT_MIDDLE\">"
                         "%s"
                         "</th>"
                         "</tr>",
               Gbl.Title,
               Txt_Degree,
               Txt_Year_OF_A_DEGREE,
               Txt_Course,
               Txt_ROLES_PLURAL_BRIEF_Abc[Rol_TCH],
               Txt_ROLES_PLURAL_BRIEF_Abc[Rol_STD]);

      /* Write courses */
      for (NumCrs = 1;
	   NumCrs <= NumCrss;
	   NumCrs++)
        {
         /* Get next course */
         row = mysql_fetch_row (mysql_res);

         /* Write data of this course */
         Crs_WriteRowCrsData (NumCrs,row,true);
        }

      /* End table and box */
      Box_EndBoxTable ();
     }

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/****************************** List courses found ***************************/
/*****************************************************************************/
// Returns number of courses found

unsigned Crs_ListCrssFound (const char *Query)
  {
   extern const char *Txt_course;
   extern const char *Txt_courses;
   extern const char *Txt_Degree;
   extern const char *Txt_Year_OF_A_DEGREE;
   extern const char *Txt_Course;
   extern const char *Txt_ROLES_PLURAL_BRIEF_Abc[Rol_NUM_ROLES];
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned NumCrss;
   unsigned NumCrs;

   /***** Query database *****/
   NumCrss = (unsigned) DB_QuerySELECT (Query,&mysql_res,"can not get courses");

   /***** List the courses (one row per course) *****/
   if (NumCrss)
     {
      /***** Start box and table *****/
      /* Number of courses found */
      sprintf (Gbl.Title,"%u %s",
               NumCrss,(NumCrss == 1) ? Txt_course :
	                                Txt_courses);
      Box_StartBoxTable (NULL,Gbl.Title,NULL,
                         NULL,Box_NOT_CLOSABLE,2);

      /***** Heading row *****/
      fprintf (Gbl.F.Out,"<tr>"
			 "<th class=\"BM\"></th>"
			 "<th class=\"LEFT_MIDDLE\">"
			 "%s"
			 "</th>"
			 "<th class=\"CENTER_MIDDLE\">"
			 "%s"
			 "</th>"
			 "<th class=\"LEFT_MIDDLE\">"
			 "%s"
			 "</th>"
			 "<th class=\"RIGHT_MIDDLE\">"
			 "%s"
			 "</th>"
			 "<th class=\"RIGHT_MIDDLE\">"
			 "%s"
			 "</th>"
			 "</tr>",
	       Txt_Degree,
	       Txt_Year_OF_A_DEGREE,
	       Txt_Course,
	       Txt_ROLES_PLURAL_BRIEF_Abc[Rol_TCH],
	       Txt_ROLES_PLURAL_BRIEF_Abc[Rol_STD]);

      /***** Write courses *****/
      for (NumCrs = 1;
	   NumCrs <= NumCrss;
	   NumCrs++)
	{
	 /* Get next course */
	 row = mysql_fetch_row (mysql_res);

	 /* Write data of this course */
	 Crs_WriteRowCrsData (NumCrs,row,false);
	}

      /***** End table and box *****/
      Box_EndBoxTable ();
     }

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   return NumCrss;
  }

/*****************************************************************************/
/************** Write the data of a course (result of a query) ***************/
/*****************************************************************************/

static void Crs_WriteRowCrsData (unsigned NumCrs,MYSQL_ROW row,bool WriteColumnAccepted)
  {
   extern const char *Txt_Enrolment_confirmed;
   extern const char *Txt_Enrolment_not_confirmed;
   extern const char *Txt_Go_to_X;
   extern const char *Txt_YEAR_OF_DEGREE[1 + Deg_MAX_YEARS_PER_DEGREE];
   struct Degree Deg;
   long CrsCod;
   unsigned NumTchs;
   unsigned NumStds;
   const char *Style;
   const char *StyleNoBR;
   const char *BgColor;
   bool Accepted;
   static unsigned RowEvenOdd = 1;

   /*
   SELECT degrees.DegCod	0
	  courses.CrsCod	1
	  degrees.ShortName	2
	  degrees.FullName	3
	  courses.Year		4
	  courses.FullName	5
	  centres.ShortName	6
	  crs_usr.Accepted	7	(only if WriteColumnAccepted == true)
   */

   /***** Get degree code (row[0]) *****/
   if ((Deg.DegCod = Str_ConvertStrCodToLongCod (row[0])) < 0)
      Lay_ShowErrorAndExit ("Wrong code of degree.");
   if (!Deg_GetDataOfDegreeByCod (&Deg))
      Lay_ShowErrorAndExit ("Degree not found.");

   /***** Get course code (row[1]) *****/
   if ((CrsCod = Str_ConvertStrCodToLongCod (row[1])) < 0)
      Lay_ShowErrorAndExit ("Wrong code of course.");

   /***** Get number of teachers and students in this course *****/
   NumTchs = Usr_GetNumUsrsInCrs (Rol_TCH,CrsCod) +
	     Usr_GetNumUsrsInCrs (Rol_NET,CrsCod);
   NumStds = Usr_GetNumUsrsInCrs (Rol_STD,CrsCod);
   if (NumTchs + NumStds)
     {
      Style = "DAT_N";
      StyleNoBR = "DAT_NOBR_N";
     }
   else
     {
      Style = "DAT";
      StyleNoBR = "DAT_NOBR";
     }
   BgColor = (CrsCod == Gbl.CurrentCrs.Crs.CrsCod) ? "LIGHT_BLUE" :
                                                     Gbl.ColorRows[RowEvenOdd];

   /***** Start row *****/
   fprintf (Gbl.F.Out,"<tr>");

   /***** User has accepted joining to this course/to any course in degree/to any course? *****/
   if (WriteColumnAccepted)
     {
      Accepted = (row[7][0] == 'Y');
      fprintf (Gbl.F.Out,"<td class=\"BT %s\">"
	                 "<img src=\"%s/%s16x16.gif\""
	                 " alt=\"%s\" title=\"%s\""
	                 " class=\"ICO20x20\" />"
	                 "</td>",
               BgColor,
               Gbl.Prefs.IconsURL,
               Accepted ? "ok_on" :
        	          "tr",
               Accepted ? Txt_Enrolment_confirmed :
        	          Txt_Enrolment_not_confirmed,
               Accepted ? Txt_Enrolment_confirmed :
        	          Txt_Enrolment_not_confirmed);
     }

   /***** Write number of course in this search *****/
   fprintf (Gbl.F.Out,"<td class=\"%s RIGHT_TOP %s\">"
	              "%u"
	              "</td>",
            StyleNoBR,BgColor,NumCrs);

   /***** Write degree logo, degree short name (row[2])
          and centre short name (row[6]) *****/
   fprintf (Gbl.F.Out,"<td class=\"%s LEFT_TOP %s\">",
            StyleNoBR,BgColor);
   Act_FormGoToStart (ActSeeDegInf);
   Deg_PutParamDegCod (Deg.DegCod);
   sprintf (Gbl.Title,Txt_Go_to_X,row[2]);
   Act_LinkFormSubmit (Gbl.Title,StyleNoBR,NULL);
   Log_DrawLogo (Sco_SCOPE_DEG,Deg.DegCod,Deg.ShrtName,20,"CENTER_TOP",true);
   fprintf (Gbl.F.Out," %s (%s)"
                      "</a>",
            row[2],row[6]);
   Act_FormEnd ();
   fprintf (Gbl.F.Out,"</td>");

   /***** Write year (row[4]) *****/
   fprintf (Gbl.F.Out,"<td class=\"%s CENTER_TOP %s\">"
	              "%s"
	              "</td>",
            Style,BgColor,Txt_YEAR_OF_DEGREE[Deg_ConvStrToYear (row[4])]);

   /***** Write course full name (row[5]) *****/
   fprintf (Gbl.F.Out,"<td class=\"%s LEFT_TOP %s\">",
            Style,BgColor);
   Act_FormGoToStart (ActSeeCrsInf);
   Crs_PutParamCrsCod (CrsCod);
   sprintf (Gbl.Title,Txt_Go_to_X,row[6]);
   Act_LinkFormSubmit (Gbl.Title,Style,NULL);
   fprintf (Gbl.F.Out,"%s</a>",row[5]);
   Act_FormEnd ();
   fprintf (Gbl.F.Out,"</td>");

   /***** Write number of teachers in course *****/
   fprintf (Gbl.F.Out,"<td class=\"%s RIGHT_TOP %s\">"
	              "%u"
	              "</td>",
            Style,BgColor,NumTchs);

   /***** Write number of students in course *****/
   fprintf (Gbl.F.Out,"<td class=\"%s RIGHT_TOP %s\">"
	              "%u"
	              "</td>"
	              "</tr>",
            Style,BgColor,NumStds);

   RowEvenOdd = 1 - RowEvenOdd;
  }

/*****************************************************************************/
/***************** Update my last click in current course ********************/
/*****************************************************************************/

void Crs_UpdateCrsLast (void)
  {
   char Query[128];

   if (Gbl.CurrentCrs.Crs.CrsCod > 0 &&
       Gbl.Usrs.Me.Role.Logged >= Rol_STD)
     {
      /***** Update my last access to current course *****/
      sprintf (Query,"REPLACE INTO crs_last (CrsCod,LastTime)"
	             " VALUES (%ld,NOW())",
	       Gbl.CurrentCrs.Crs.CrsCod);
      DB_QueryUPDATE (Query,"can not update last access to current course");
     }
  }

/*****************************************************************************/
/********************** Put link to remove old courses ***********************/
/*****************************************************************************/

void Crs_PutLinkToRemoveOldCrss (void)
  {
   extern const char *Txt_Eliminate_old_courses;

   /***** Put form to remove old courses *****/
   Lay_PutContextualLink (ActReqRemOldCrs,NULL,NULL,
                          "remove-on64x64.png",
                          Txt_Eliminate_old_courses,Txt_Eliminate_old_courses,
                          NULL);
  }

/*****************************************************************************/
/********************** Write form to remove old courses *********************/
/*****************************************************************************/

void Crs_AskRemoveOldCrss (void)
  {
   extern const char *Hlp_SYSTEM_Hierarchy_eliminate_old_courses;
   extern const char *The_ClassForm[The_NUM_THEMES];
   extern const char *Txt_Eliminate_old_courses;
   extern const char *Txt_Eliminate_all_courses_whithout_users_PART_1_OF_2;
   extern const char *Txt_Eliminate_all_courses_whithout_users_PART_2_OF_2;
   extern const char *Txt_Eliminate;
   unsigned MonthsWithoutAccess = Crs_DEF_MONTHS_WITHOUT_ACCESS_TO_REMOVE_OLD_CRSS;
   unsigned i;

   /***** Start form *****/
   Act_FormStart (ActRemOldCrs);

   /***** Start box *****/
   Box_StartBox (NULL,Txt_Eliminate_old_courses,NULL,
                 Hlp_SYSTEM_Hierarchy_eliminate_old_courses,Box_NOT_CLOSABLE);

   /***** Form to request number of months without clicks *****/
   fprintf (Gbl.F.Out,"<label class=\"%s\">%s&nbsp;"
	              "<select name=\"Months\">",
            The_ClassForm[Gbl.Prefs.Theme],
            Txt_Eliminate_all_courses_whithout_users_PART_1_OF_2);
   for (i  = Crs_MIN_MONTHS_WITHOUT_ACCESS_TO_REMOVE_OLD_CRSS;
        i <= Crs_MAX_MONTHS_WITHOUT_ACCESS_TO_REMOVE_OLD_CRSS;
        i++)
     {
      fprintf (Gbl.F.Out,"<option");
      if (i == MonthsWithoutAccess)
         fprintf (Gbl.F.Out," selected=\"selected\"");
      fprintf (Gbl.F.Out,">%u</option>",i);
     }
   fprintf (Gbl.F.Out,"</select>&nbsp;");
   fprintf (Gbl.F.Out,Txt_Eliminate_all_courses_whithout_users_PART_2_OF_2,
            Cfg_PLATFORM_SHORT_NAME);
   fprintf (Gbl.F.Out,"</label>");

   /***** Send button and end box *****/
   Box_EndBoxWithButton (Btn_REMOVE_BUTTON,Txt_Eliminate);

   /***** End form *****/
   Act_FormEnd ();
  }

/*****************************************************************************/
/**************************** Remove old courses *****************************/
/*****************************************************************************/

void Crs_RemoveOldCrss (void)
  {
   extern const char *Txt_Eliminating_X_courses_whithout_users_and_with_more_than_Y_months_without_access;
   extern const char *Txt_X_courses_have_been_eliminated;
   unsigned MonthsWithoutAccess;
   unsigned long SecondsWithoutAccess;
   char Query[1024];
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumCrs;
   unsigned long NumCrss;
   unsigned NumCrssRemoved = 0;
   long CrsCod;

   /***** Get parameter with number of months without access *****/
   MonthsWithoutAccess = (unsigned)
	                 Par_GetParToUnsignedLong ("Months",
                                                   Crs_MIN_MONTHS_WITHOUT_ACCESS_TO_REMOVE_OLD_CRSS,
                                                   Crs_MAX_MONTHS_WITHOUT_ACCESS_TO_REMOVE_OLD_CRSS,
                                                   UINT_MAX);
   if (MonthsWithoutAccess == UINT_MAX)
      Lay_ShowErrorAndExit ("Wrong number of months without clicks.");
   SecondsWithoutAccess = (unsigned long) MonthsWithoutAccess * Dat_SECONDS_IN_ONE_MONTH;

   /***** Get old courses from database *****/
   sprintf (Query,"SELECT CrsCod FROM crs_last WHERE"
                  " LastTime<FROM_UNIXTIME(UNIX_TIMESTAMP()-'%lu')"
                  " AND CrsCod NOT IN (SELECT DISTINCT CrsCod FROM crs_usr)",
            SecondsWithoutAccess);
   if ((NumCrss = DB_QuerySELECT (Query,&mysql_res,"can not get old users")))
     {
      sprintf (Gbl.Alert.Txt,Txt_Eliminating_X_courses_whithout_users_and_with_more_than_Y_months_without_access,
               NumCrss,
               MonthsWithoutAccess,
               Cfg_PLATFORM_SHORT_NAME);
      Ale_ShowAlert (Ale_INFO,Gbl.Alert.Txt);

      /***** Remove courses *****/
      for (NumCrs = 0;
           NumCrs < NumCrss;
           NumCrs++)
        {
         row = mysql_fetch_row (mysql_res);
         CrsCod = Str_ConvertStrCodToLongCod (row[0]);
         Crs_RemoveCourseCompletely (CrsCod);
         NumCrssRemoved++;
        }

      /***** Free structure that stores the query result *****/
      DB_FreeMySQLResult (&mysql_res);
     }

   /***** Write end message *****/
   sprintf (Gbl.Alert.Txt,Txt_X_courses_have_been_eliminated,
            NumCrssRemoved);
   Ale_ShowAlert (Ale_SUCCESS,Gbl.Alert.Txt);
  }

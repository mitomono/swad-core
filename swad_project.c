// swad_project.c: projects (final degree projects, thesis)

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

#include <linux/limits.h>	// For PATH_MAX
#include <linux/stddef.h>	// For NULL
#include <stdlib.h>		// For calloc
#include <string.h>		// For string functions

#include "swad_box.h"
#include "swad_database.h"
#include "swad_global.h"
#include "swad_group.h"
#include "swad_notification.h"
#include "swad_pagination.h"
#include "swad_parameter.h"
#include "swad_photo.h"
#include "swad_project.h"
#include "swad_string.h"
#include "swad_table.h"

/*****************************************************************************/
/************** External global variables from others modules ****************/
/*****************************************************************************/

extern struct Globals Gbl;

/*****************************************************************************/
/***************************** Private constants *****************************/
/*****************************************************************************/

/*****************************************************************************/
/******************************* Private types *******************************/
/*****************************************************************************/

/*****************************************************************************/
/***************************** Private variables *****************************/
/*****************************************************************************/

/*****************************************************************************/
/***************************** Private prototypes ****************************/
/*****************************************************************************/

static void Prj_ShowAllProjects (void);
static void Prj_PutHeadForSeeing (bool PrintView);
static bool Prj_CheckIfICanCreateProjects (void);
static void Prj_PutIconsListProjects (void);
static void Prj_PutIconToCreateNewPrj (void);
static void Prj_PutButtonToCreateNewPrj (void);
static void Prj_PutFormToSelectWhichGroupsToShow (void);
static void Prj_ParamsWhichGroupsToShow (void);
static void Prj_ShowOneProject (long PrjCod,bool PrintView);
static void Prj_WritePrjAuthor (struct Project *Prj);
static void Prj_WriteProjectFolder (struct Project *Prj,bool PrintView);
static void Prj_GetParamPrjOrder (void);

static void Prj_PutFormsToRemEditOnePrj (long PrjCod,bool Hidden);
static void Prj_PutParams (void);
static void Prj_GetDataOfProject (struct Project *Prj,const char *Query);
static void Prj_ResetProject (struct Project *Prj);
static void Prj_GetProjectTxtFromDB (long PrjCod,char Txt[Cns_MAX_BYTES_TEXT + 1]);
static void Prj_PutParamPrjCod (long PrjCod);
static bool Prj_CheckIfSimilarProjectsExists (const char *Field,const char *Value,long PrjCod);
static void Prj_ShowLstGrpsToEditProject (long PrjCod);
static void Prj_CreateProject (struct Project *Prj,const char *Txt);
static void Prj_UpdateProject (struct Project *Prj,const char *Txt);
static bool Prj_CheckIfPrjIsAssociatedToGrps (long PrjCod);
static void Prj_RemoveAllTheGrpsAssociatedToAProject (long PrjCod);
static void Prj_CreateGrps (long PrjCod);
static void Prj_GetAndWriteNamesOfGrpsAssociatedToPrj (struct Project *Prj);
static bool Prj_CheckIfIBelongToCrsOrGrpsThisProject (long PrjCod);

/*****************************************************************************/
/************************** List all the projects ****************************/
/*****************************************************************************/

void Prj_SeeProjects (void)
  {
   /***** Get parameters *****/
   Prj_GetParamPrjOrder ();
   Grp_GetParamWhichGrps ();
   Gbl.Prjs.CurrentPage = Pag_GetParamPagNum (Pag_PROJECTS);

   /***** Show all the projects *****/
   Prj_ShowAllProjects ();
  }

/*****************************************************************************/
/************************** Show all the projects ****************************/
/*****************************************************************************/

static void Prj_ShowAllProjects (void)
  {
   extern const char *Hlp_ASSESSMENT_Projects;
   extern const char *Txt_Projects;
   extern const char *Txt_No_projects;
   struct Pagination Pagination;
   unsigned NumPrj;

   /***** Get list of projects *****/
   Prj_GetListProjects ();

   /***** Compute variables related to pagination *****/
   Pagination.NumItems = Gbl.Prjs.Num;
   Pagination.CurrentPage = (int) Gbl.Prjs.CurrentPage;
   Pag_CalculatePagination (&Pagination);
   Gbl.Prjs.CurrentPage = (unsigned) Pagination.CurrentPage;

   /***** Write links to pages *****/
   if (Pagination.MoreThanOnePage)
      Pag_WriteLinksToPagesCentered (Pag_PROJECTS,
                                     0,
                                     &Pagination);

   /***** Start box *****/
   Box_StartBox ("100%",Txt_Projects,Prj_PutIconsListProjects,
                 Hlp_ASSESSMENT_Projects,Box_NOT_CLOSABLE);

   /***** Select whether show only my groups or all groups *****/
   if (Gbl.CurrentCrs.Grps.NumGrps)
      Prj_PutFormToSelectWhichGroupsToShow ();

   if (Gbl.Prjs.Num)
     {
      /***** Table head *****/
      Tbl_StartTableWideMargin (2);
      Prj_PutHeadForSeeing (false);	// Not print view

      /***** Write all the projects *****/
      for (NumPrj = Pagination.FirstItemVisible;
	   NumPrj <= Pagination.LastItemVisible;
	   NumPrj++)
	 Prj_ShowOneProject (Gbl.Prjs.LstPrjCods[NumPrj - 1],
	                        false);	// Not print view

      /***** End table *****/
      Tbl_EndTable ();
     }
   else	// No projects created
      Ale_ShowAlert (Ale_INFO,Txt_No_projects);

   /***** Button to create a new project *****/
   if (Prj_CheckIfICanCreateProjects ())
      Prj_PutButtonToCreateNewPrj ();

   /***** Put link to register students *****/
   Enr_CheckStdsAndPutButtonToRegisterStdsInCurrentCrs ();

   /***** End box *****/
   Box_EndBox ();

   /***** Write again links to pages *****/
   if (Pagination.MoreThanOnePage)
      Pag_WriteLinksToPagesCentered (Pag_PROJECTS,
                                     0,
                                     &Pagination);

   /***** Free list of projects *****/
   Prj_FreeListProjects ();
  }

/*****************************************************************************/
/******************* Write header with fields of a project *******************/
/*****************************************************************************/

static void Prj_PutHeadForSeeing (bool PrintView)
  {
   extern const char *Txt_START_END_TIME_HELP[Dat_NUM_START_END_TIME];
   extern const char *Txt_START_END_TIME[Dat_NUM_START_END_TIME];
   extern const char *Txt_Project;
   extern const char *Txt_Upload_files_QUESTION;
   extern const char *Txt_Folder;
   Dat_StartEndTime_t Order;

   fprintf (Gbl.F.Out,"<tr>"
	              "<th class=\"CONTEXT_COL\"></th>");	// Column for contextual icons
   for (Order = Dat_START_TIME;
	Order <= Dat_END_TIME;
	Order++)
     {
      fprintf (Gbl.F.Out,"<th class=\"LEFT_MIDDLE\">");

      if (!PrintView)
	{
	 Act_FormStart (ActSeePrj);
	 Grp_PutParamWhichGrps ();
	 Pag_PutHiddenParamPagNum (Pag_PROJECTS,Gbl.Prjs.CurrentPage);
	 Par_PutHiddenParamUnsigned ("Order",(unsigned) Order);
	 Act_LinkFormSubmit (Txt_START_END_TIME_HELP[Order],"TIT_TBL",NULL);
	 if (Order == Gbl.Prjs.SelectedOrder)
	    fprintf (Gbl.F.Out,"<u>");
	}
      fprintf (Gbl.F.Out,"%s",Txt_START_END_TIME[Order]);
      if (!PrintView)
	{
	 if (Order == Gbl.Prjs.SelectedOrder)
	    fprintf (Gbl.F.Out,"</u>");
	 fprintf (Gbl.F.Out,"</a>");
	 Act_FormEnd ();
	}

      fprintf (Gbl.F.Out,"</th>");
     }
   fprintf (Gbl.F.Out,"<th class=\"LEFT_MIDDLE\">"
		      "%s"
		      "</th>"
		      "<th class=\"CENTER_MIDDLE\">"
		      "%s"
		      "</th>"
		      "<th class=\"LEFT_MIDDLE\">"
		      "%s"
		      "</th>"
		      "</tr>",
	    Txt_Project,
	    Txt_Upload_files_QUESTION,
	    Txt_Folder);
  }

/*****************************************************************************/
/********************** Check if I can create projects ***********************/
/*****************************************************************************/

static bool Prj_CheckIfICanCreateProjects (void)
  {
   return (bool) (Gbl.Usrs.Me.Role.Logged == Rol_TCH ||
                  Gbl.Usrs.Me.Role.Logged == Rol_SYS_ADM);
  }

/*****************************************************************************/
/***************** Put contextual icons in list of projects ******************/
/*****************************************************************************/

static void Prj_PutIconsListProjects (void)
  {
   /***** Put icon to create a new project *****/
   if (Prj_CheckIfICanCreateProjects ())
      Prj_PutIconToCreateNewPrj ();

   /***** Put icon to show a figure *****/
   Gbl.Stat.FigureType = Sta_PROJECTS;
   Sta_PutIconToShowFigure ();
  }

/*****************************************************************************/
/********************* Put icon to create a new project **********************/
/*****************************************************************************/

static void Prj_PutIconToCreateNewPrj (void)
  {
   extern const char *Txt_New_project;

   /***** Put form to create a new project *****/
   Gbl.Prjs.PrjCodToEdit = -1L;
   Lay_PutContextualLink (ActFrmNewPrj,NULL,Prj_PutParams,
                          "plus64x64.png",
                          Txt_New_project,NULL,
                          NULL);
  }

/*****************************************************************************/
/******************** Put button to create a new project *********************/
/*****************************************************************************/

static void Prj_PutButtonToCreateNewPrj (void)
  {
   extern const char *Txt_New_project;

   Gbl.Prjs.PrjCodToEdit = -1L;
   Act_FormStart (ActFrmNewPrj);
   Prj_PutParams ();
   Btn_PutConfirmButton (Txt_New_project);
   Act_FormEnd ();
  }

/*****************************************************************************/
/***************** Put form to select which groups to show *******************/
/*****************************************************************************/

static void Prj_PutFormToSelectWhichGroupsToShow (void)
  {
   fprintf (Gbl.F.Out,"<div style=\"display:table; margin:0 auto;\">");
   Grp_ShowFormToSelWhichGrps (ActSeePrj,Prj_ParamsWhichGroupsToShow);
   fprintf (Gbl.F.Out,"</div>");
  }

static void Prj_ParamsWhichGroupsToShow (void)
  {
   Prj_PutHiddenParamPrjOrder ();
   Pag_PutHiddenParamPagNum (Pag_PROJECTS,Gbl.Prjs.CurrentPage);
  }

/*****************************************************************************/
/********************** Show print view of one project ***********************/
/*****************************************************************************/

void Prj_PrintOneProject (void)
  {
   long PrjCod;

   /***** Get the code of the project *****/
   PrjCod = Prj_GetParamPrjCod ();

   /***** Write header *****/
   Lay_WriteHeaderClassPhoto (true,false,
			      Gbl.CurrentIns.Ins.InsCod,
			      Gbl.CurrentDeg.Deg.DegCod,
			      Gbl.CurrentCrs.Crs.CrsCod);

   /***** Table head *****/
   Tbl_StartTableWideMargin (2);
   Prj_PutHeadForSeeing (true);		// Print view

   /***** Write project *****/
   Prj_ShowOneProject (PrjCod,
                          true);	// Print view

   /***** End table *****/
   Tbl_EndTable ();
  }

/*****************************************************************************/
/***************************** Show one project ******************************/
/*****************************************************************************/

static void Prj_ShowOneProject (long PrjCod,bool PrintView)
  {
   extern const char *Txt_Today;
   extern const char *Txt_ASSIGNMENT_TYPES[Prj_NUM_TYPES_SEND_WORK];
   extern const char *Txt_Yes;
   extern const char *Txt_No;
   static unsigned UniqueId = 0;
   struct Project Prj;
   char Txt[Cns_MAX_BYTES_TEXT + 1];

   /***** Get data of this project *****/
   Prj.PrjCod = PrjCod;
   Prj_GetDataOfProjectByCod (&Prj);

   /***** Write first row of data of this project *****/
   /* Forms to remove/edit this project */
   fprintf (Gbl.F.Out,"<tr>"
	              "<td rowspan=\"2\" class=\"CONTEXT_COL");
   if (PrintView)
      fprintf (Gbl.F.Out,"\">");
   else
     {
      fprintf (Gbl.F.Out," COLOR%u\">",Gbl.RowEvenOdd);
      Prj_PutFormsToRemEditOnePrj (Prj.PrjCod,Prj.Hidden);
     }
   fprintf (Gbl.F.Out,"</td>");

   /* Start date/time */
   UniqueId++;
   fprintf (Gbl.F.Out,"<td id=\"asg_date_start_%u\" class=\"%s LEFT_BOTTOM",
	    UniqueId,
            Prj.Hidden ? (Prj.Open ? "DATE_GREEN_LIGHT" :
        	                     "DATE_RED_LIGHT") :
                         (Prj.Open ? "DATE_GREEN" :
                                     "DATE_RED"));
   if (!PrintView)
      fprintf (Gbl.F.Out," COLOR%u",Gbl.RowEvenOdd);
   fprintf (Gbl.F.Out,"\">"
                      "<script type=\"text/javascript\">"
                      "writeLocalDateHMSFromUTC('asg_date_start_%u',%ld,"
                      "%u,'<br />','%s',true,true,0x7);"
                      "</script>"
	              "</td>",
            UniqueId,Prj.TimeUTC[Dat_START_TIME],
            (unsigned) Gbl.Prefs.DateFormat,Txt_Today);

   /* End date/time */
   UniqueId++;
   fprintf (Gbl.F.Out,"<td id=\"asg_date_end_%u\" class=\"%s LEFT_BOTTOM",
	    UniqueId,
            Prj.Hidden ? (Prj.Open ? "DATE_GREEN_LIGHT" :
        	                     "DATE_RED_LIGHT") :
                         (Prj.Open ? "DATE_GREEN" :
                                     "DATE_RED"));
   if (!PrintView)
      fprintf (Gbl.F.Out," COLOR%u",Gbl.RowEvenOdd);
   fprintf (Gbl.F.Out,"\">"
                      "<script type=\"text/javascript\">"
                      "writeLocalDateHMSFromUTC('asg_date_end_%u',%ld,"
                      "%u,'<br />','%s',false,true,0x7);"
                      "</script>"
	              "</td>",
            UniqueId,Prj.TimeUTC[Dat_END_TIME],
            (unsigned) Gbl.Prefs.DateFormat,Txt_Today);

   /* Project title */
   fprintf (Gbl.F.Out,"<td class=\"LEFT_TOP");
   if (!PrintView)
      fprintf (Gbl.F.Out," COLOR%u",Gbl.RowEvenOdd);
   fprintf (Gbl.F.Out,"\">"
                      "<div class=\"%s\">%s</div>",
            Prj.Hidden ? "ASG_TITLE_LIGHT" :
        	         "ASG_TITLE",
            Prj.Title);
   fprintf (Gbl.F.Out,"</td>");

   /* Send work? */
   fprintf (Gbl.F.Out,"<td class=\"%s CENTER_TOP",
            (Prj.SendWork == Prj_SEND_WORK) ? "DAT_N" :
        	                              "DAT");
   if (!PrintView)
      fprintf (Gbl.F.Out," COLOR%u",Gbl.RowEvenOdd);
   fprintf (Gbl.F.Out,"\">"
                      "<img src=\"%s/%s16x16.gif\""
                      " alt=\"%s\" title=\"%s\" class=\"ICO20x20\" />"
                      "<br />%s"
                      "</td>",
            Gbl.Prefs.IconsURL,
            (Prj.SendWork == Prj_SEND_WORK) ? "file_on" :
        	                              "file_off",
            Txt_ASSIGNMENT_TYPES[Prj.SendWork],
            Txt_ASSIGNMENT_TYPES[Prj.SendWork],
            (Prj.SendWork == Prj_SEND_WORK) ? Txt_Yes :
        	                              Txt_No);

   /* Project folder */
   fprintf (Gbl.F.Out,"<td class=\"DAT LEFT_TOP");
   if (!PrintView)
      fprintf (Gbl.F.Out," COLOR%u",Gbl.RowEvenOdd);
   fprintf (Gbl.F.Out,"\">");
   if (Prj.SendWork == Prj_SEND_WORK)
      Prj_WriteProjectFolder (&Prj,PrintView);
   fprintf (Gbl.F.Out,"</td>"
	              "</tr>");

   /***** Write second row of data of this project *****/
   fprintf (Gbl.F.Out,"<tr>"
	              "<td colspan=\"2\" class=\"LEFT_TOP");
   if (!PrintView)
      fprintf (Gbl.F.Out," COLOR%u",Gbl.RowEvenOdd);
   fprintf (Gbl.F.Out,"\">");

   /* Author of the project */
   Prj_WritePrjAuthor (&Prj);

   fprintf (Gbl.F.Out,"</td>");

   /* Text of the project */
   Prj_GetProjectTxtFromDB (Prj.PrjCod,Txt);
   Str_ChangeFormat (Str_FROM_HTML,Str_TO_RIGOROUS_HTML,
                     Txt,Cns_MAX_BYTES_TEXT,false);	// Convert from HTML to recpectful HTML
   Str_InsertLinks (Txt,Cns_MAX_BYTES_TEXT,60);	// Insert links
   fprintf (Gbl.F.Out,"<td colspan=\"3\" class=\"LEFT_TOP");
   if (!PrintView)
      fprintf (Gbl.F.Out," COLOR%u",Gbl.RowEvenOdd);
   fprintf (Gbl.F.Out,"\">");

   if (Gbl.CurrentCrs.Grps.NumGrps)
      Prj_GetAndWriteNamesOfGrpsAssociatedToPrj (&Prj);

   fprintf (Gbl.F.Out,"<p class=\"%s\">"
                      "%s"
                      "</p>"
                      "</td>"
                      "</tr>",
            Prj.Hidden ? "DAT_LIGHT" :
        	         "DAT",
            Txt);

   Gbl.RowEvenOdd = 1 - Gbl.RowEvenOdd;
  }

/*****************************************************************************/
/*********************** Write the author of a project ***********************/
/*****************************************************************************/

static void Prj_WritePrjAuthor (struct Project *Prj)
  {
   Usr_WriteAuthor1Line (Prj->UsrCod,Prj->Hidden);
  }

/*****************************************************************************/
/*********************** Write the folder of a project ***********************/
/*****************************************************************************/

static void Prj_WriteProjectFolder (struct Project *Prj,bool PrintView)
  {
   extern const char *Txt_Upload_file_or_create_folder_in_FOLDER;
   extern const char *Txt_Folder;
   bool ICanSendFiles = !Prj->Hidden &&				// It's visible (not hidden)
                        Prj->Open &&				// It's open (inside dates)
                        Prj->IBelongToCrsOrGrps &&		// I belong to course or groups
                        Gbl.Usrs.Me.Role.Logged == Rol_STD;	// I am a student

   /***** Folder icon *****/
   if (!PrintView &&	// Not print view
       ICanSendFiles)	// I can send files to this project folder
     {
      /* Form to create a new file or folder */
      Act_FormStart (ActFrmCreAsgUsr);	// TODO: Remove this feature
      Brw_PutParamsFileBrowser (ActUnk,
				Brw_INTERNAL_NAME_ROOT_FOLDER_ASSIGNMENTS,
				Prj->Folder,
				Brw_IS_FOLDER,-1L);
      sprintf (Gbl.Title,Txt_Upload_file_or_create_folder_in_FOLDER,
	       Prj->Folder);
      fprintf (Gbl.F.Out,"<input type=\"image\""
			 " src=\"%s/folder-open-plus16x16.gif\""
			 " alt=\"%s\" title=\"%s\" class=\"ICO20x20\" />",
	       Gbl.Prefs.IconsURL,
	       Gbl.Title,
	       Gbl.Title);
      Act_FormEnd ();
     }
   else				// I can't send files to this project folder
      fprintf (Gbl.F.Out,"<img src=\"%s/%s\" alt=\"%s\" title=\"%s\""
	                 " class=\"ICO20x20\" />",
	       Gbl.Prefs.IconsURL,
	       ICanSendFiles ? "folder-open16x16.gif" :
		               "folder-closed16x16.gif",
	       Txt_Folder,Txt_Folder);

   /***** Folder name *****/
   fprintf (Gbl.F.Out,"%s",Prj->Folder);
  }

/*****************************************************************************/
/********* Get parameter with the type or order in list of projects **********/
/*****************************************************************************/

static void Prj_GetParamPrjOrder (void)
  {
   Gbl.Prjs.SelectedOrder = (Dat_StartEndTime_t)
	                    Par_GetParToUnsignedLong ("Order",
                                                      0,
                                                      Dat_NUM_START_END_TIME - 1,
                                                      (unsigned long) Prj_ORDER_DEFAULT);
  }

/*****************************************************************************/
/***** Put a hidden parameter with the type of order in list of projects *****/
/*****************************************************************************/

void Prj_PutHiddenParamPrjOrder (void)
  {
   Par_PutHiddenParamUnsigned ("Order",(unsigned) Gbl.Prjs.SelectedOrder);
  }

/*****************************************************************************/
/****************** Put a link (form) to edit one project ********************/
/*****************************************************************************/

static void Prj_PutFormsToRemEditOnePrj (long PrjCod,bool Hidden)
  {
   Gbl.Prjs.PrjCodToEdit = PrjCod;	// Used as parameter in contextual links

   switch (Gbl.Usrs.Me.Role.Logged)
     {
      case Rol_TCH:
      case Rol_SYS_ADM:
	 /***** Put form to remove project *****/
	 Ico_PutContextualIconToRemove (ActReqRemPrj,Prj_PutParams);

	 /***** Put form to hide/show project *****/
	 if (Hidden)
	    Ico_PutContextualIconToUnhide (ActShoPrj,Prj_PutParams);
	 else
	    Ico_PutContextualIconToHide (ActHidPrj,Prj_PutParams);

	 /***** Put form to edit project *****/
	 Ico_PutContextualIconToEdit (ActEdiOnePrj,Prj_PutParams);
	 // no break
      case Rol_STD:
      case Rol_NET:
	 /***** Put form to print project *****/
	 Ico_PutContextualIconToPrint (ActPrnOnePrj,Prj_PutParams);
	 break;
      default:
         break;
     }
  }

/*****************************************************************************/
/********************** Params used to edit a project ************************/
/*****************************************************************************/

static void Prj_PutParams (void)
  {
   if (Gbl.Prjs.PrjCodToEdit > 0)
      Prj_PutParamPrjCod (Gbl.Prjs.PrjCodToEdit);
   Prj_PutHiddenParamPrjOrder ();
   Grp_PutParamWhichGrps ();
   Pag_PutHiddenParamPagNum (Pag_PROJECTS,Gbl.Prjs.CurrentPage);
  }

/*****************************************************************************/
/************************** List all the projects ****************************/
/*****************************************************************************/

void Prj_GetListProjects (void)
  {
   char HiddenSubQuery[256];
   char OrderBySubQuery[256];
   char Query[2048];
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumRows;
   unsigned NumPrj;

   if (Gbl.Prjs.LstIsRead)
      Prj_FreeListProjects ();

   /***** Get list of projects from database *****/
   switch (Gbl.Usrs.Me.Role.Logged)
     {
      case Rol_TCH:
      case Rol_SYS_ADM:
         HiddenSubQuery[0] = '\0';
         break;
      default:
         sprintf (HiddenSubQuery," AND Hidden='N'");
         break;
     }
   switch (Gbl.Prjs.SelectedOrder)
     {
      case Dat_START_TIME:
         sprintf (OrderBySubQuery,"StartTime DESC,EndTime DESC,Title DESC");
         break;
      case Dat_END_TIME:
         sprintf (OrderBySubQuery,"EndTime DESC,StartTime DESC,Title DESC");
         break;
     }
   if (Gbl.CurrentCrs.Grps.WhichGrps == Grp_ONLY_MY_GROUPS)
      sprintf (Query,"SELECT PrjCod"
                     " FROM projects"
                     " WHERE CrsCod=%ld%s"
                     " AND (PrjCod NOT IN (SELECT PrjCod FROM prj_grp) OR"
                     " PrjCod IN (SELECT prj_grp.PrjCod FROM prj_grp,crs_grp_usr"
                     " WHERE crs_grp_usr.UsrCod=%ld AND prj_grp.GrpCod=crs_grp_usr.GrpCod))"
                     " ORDER BY %s",
               Gbl.CurrentCrs.Crs.CrsCod,
               HiddenSubQuery,
               Gbl.Usrs.Me.UsrDat.UsrCod,
               OrderBySubQuery);
   else	// Gbl.CurrentCrs.Grps.WhichGrps == Grp_ALL_GROUPS
      sprintf (Query,"SELECT PrjCod"
                     " FROM projects"
                     " WHERE CrsCod=%ld%s"
                     " ORDER BY %s",
               Gbl.CurrentCrs.Crs.CrsCod,HiddenSubQuery,OrderBySubQuery);
   NumRows = DB_QuerySELECT (Query,&mysql_res,"can not get projects");

   if (NumRows) // Projects found...
     {
      Gbl.Prjs.Num = (unsigned) NumRows;

      /***** Create list of projects *****/
      if ((Gbl.Prjs.LstPrjCods = (long *) calloc (NumRows,sizeof (long))) == NULL)
          Lay_ShowErrorAndExit ("Not enough memory to store list of projects.");

      /***** Get the projects codes *****/
      for (NumPrj = 0;
	   NumPrj < Gbl.Prjs.Num;
	   NumPrj++)
        {
         /* Get next project code */
         row = mysql_fetch_row (mysql_res);
         if ((Gbl.Prjs.LstPrjCods[NumPrj] = Str_ConvertStrCodToLongCod (row[0])) < 0)
            Lay_ShowErrorAndExit ("Error: wrong project code.");
        }
     }
   else
      Gbl.Prjs.Num = 0;

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   Gbl.Prjs.LstIsRead = true;
  }

/*****************************************************************************/
/********************* Get project data using its code ***********************/
/*****************************************************************************/

void Prj_GetDataOfProjectByCod (struct Project *Prj)
  {
   char Query[1024];

   if (Prj->PrjCod > 0)
     {
      /***** Build query *****/
      sprintf (Query,"SELECT PrjCod,Hidden,UsrCod,"
		     "UNIX_TIMESTAMP(StartTime),"
		     "UNIX_TIMESTAMP(EndTime),"
		     "NOW() BETWEEN StartTime AND EndTime,"
		     "Title,Folder"
		     " FROM projects"
		     " WHERE PrjCod=%ld AND CrsCod=%ld",
	       Prj->PrjCod,Gbl.CurrentCrs.Crs.CrsCod);

      /***** Get data of project *****/
      Prj_GetDataOfProject (Prj,Query);
     }
   else
     {
      /***** Clear all project data *****/
      Prj->PrjCod = -1L;
      Prj_ResetProject (Prj);
     }
  }

/*****************************************************************************/
/***************** Get project data using its folder name ********************/
/*****************************************************************************/

void Prj_GetDataOfProjectByFolder (struct Project *Prj)
  {
   char Query[1024 + Brw_MAX_BYTES_FOLDER];

   if (Prj->Folder[0])
     {
      /***** Query database *****/
      sprintf (Query,"SELECT PrjCod,Hidden,UsrCod,"
		     "UNIX_TIMESTAMP(StartTime),"
		     "UNIX_TIMESTAMP(EndTime),"
		     "NOW() BETWEEN StartTime AND EndTime,"
		     "Title,Folder"
		     " FROM projects"
		     " WHERE CrsCod=%ld AND Folder='%s'",
	       Gbl.CurrentCrs.Crs.CrsCod,Prj->Folder);

      /***** Get data of project *****/
      Prj_GetDataOfProject (Prj,Query);
     }
   else
     {
      /***** Clear all project data *****/
      Prj->PrjCod = -1L;
      Prj_ResetProject (Prj);
     }
  }

/*****************************************************************************/
/**************************** Get project data *******************************/
/*****************************************************************************/

static void Prj_GetDataOfProject (struct Project *Prj,const char *Query)
  {
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;

   /***** Clear all project data *****/
   Prj_ResetProject (Prj);

   /***** Get data of project from database *****/
   if (DB_QuerySELECT (Query,&mysql_res,"can not get project data")) // Project found...
     {
      /* Get row */
      row = mysql_fetch_row (mysql_res);

      /* Get code of the project (row[0]) */
      Prj->PrjCod = Str_ConvertStrCodToLongCod (row[0]);

      /* Get whether the project is hidden or not (row[1]) */
      Prj->Hidden = (row[1][0] == 'Y');

      /* Get author of the project (row[2]) */
      Prj->UsrCod = Str_ConvertStrCodToLongCod (row[2]);

      /* Get start date (row[3] holds the start UTC time) */
      Prj->TimeUTC[Dat_START_TIME] = Dat_GetUNIXTimeFromStr (row[3]);

      /* Get end date   (row[4] holds the end   UTC time) */
      Prj->TimeUTC[Dat_END_TIME  ] = Dat_GetUNIXTimeFromStr (row[4]);

      /* Get whether the project is open or closed (row(5)) */
      Prj->Open = (row[5][0] == '1');

      /* Get the title of the project (row[6]) */
      Str_Copy (Prj->Title,row[6],
                Prj_MAX_BYTES_ASSIGNMENT_TITLE);

      /* Get the folder for the project files (row[7]) */
      Str_Copy (Prj->Folder,row[7],
                Brw_MAX_BYTES_FOLDER);
      Prj->SendWork = (Prj->Folder[0] != '\0');

      /* Can I do this project? */
      Prj->IBelongToCrsOrGrps = Prj_CheckIfIBelongToCrsOrGrpsThisProject (Prj->PrjCod);
     }

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/************************** Clear all project data ***************************/
/*****************************************************************************/

static void Prj_ResetProject (struct Project *Prj)
  {
   if (Prj->PrjCod <= 0)	// If > 0 ==> keep value
      Prj->PrjCod = -1L;
   Prj->PrjCod = -1L;
   Prj->Hidden = false;
   Prj->UsrCod = -1L;
   Prj->TimeUTC[Dat_START_TIME] =
   Prj->TimeUTC[Dat_END_TIME  ] = (time_t) 0;
   Prj->Open = false;
   Prj->Title[0] = '\0';
   Prj->SendWork = false;
   Prj->Folder[0] = '\0';
   Prj->IBelongToCrsOrGrps = false;
  }

/*****************************************************************************/
/*************************** Free list of projects ***************************/
/*****************************************************************************/

void Prj_FreeListProjects (void)
  {
   if (Gbl.Prjs.LstIsRead && Gbl.Prjs.LstPrjCods)
     {
      /***** Free memory used by the list of projects *****/
      free ((void *) Gbl.Prjs.LstPrjCods);
      Gbl.Prjs.LstPrjCods = NULL;
      Gbl.Prjs.Num = 0;
      Gbl.Prjs.LstIsRead = false;
     }
  }

/*****************************************************************************/
/********************** Get project text from database ***********************/
/*****************************************************************************/

static void Prj_GetProjectTxtFromDB (long PrjCod,char Txt[Cns_MAX_BYTES_TEXT + 1])
  {
   char Query[512];
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumRows;

   /***** Get text of project from database *****/
   sprintf (Query,"SELECT Txt FROM projects"
	          " WHERE PrjCod=%ld AND CrsCod=%ld",
            PrjCod,Gbl.CurrentCrs.Crs.CrsCod);
   NumRows = DB_QuerySELECT (Query,&mysql_res,"can not get project text");

   /***** The result of the query must have one row or none *****/
   if (NumRows == 1)
     {
      /* Get info text */
      row = mysql_fetch_row (mysql_res);
      Str_Copy (Txt,row[0],
                Cns_MAX_BYTES_TEXT);
     }
   else
      Txt[0] = '\0';

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   if (NumRows > 1)
      Lay_ShowErrorAndExit ("Error when getting project text.");
  }

/*****************************************************************************/
/******************* Write parameter with code of project ********************/
/*****************************************************************************/

static void Prj_PutParamPrjCod (long PrjCod)
  {
   Par_PutHiddenParamLong ("PrjCod",PrjCod);
  }

/*****************************************************************************/
/******************** Get parameter with code of project *********************/
/*****************************************************************************/

long Prj_GetParamPrjCod (void)
  {
   /***** Get code of project *****/
   return Par_GetParToLong ("PrjCod");
  }

/*****************************************************************************/
/*************** Ask for confirmation of removing a project ******************/
/*****************************************************************************/

void Prj_ReqRemProject (void)
  {
   extern const char *Txt_Do_you_really_want_to_remove_the_project_X;
   extern const char *Txt_Remove_project;
   struct Project Prj;

   /***** Get parameters *****/
   Prj_GetParamPrjOrder ();
   Grp_GetParamWhichGrps ();
   Gbl.Prjs.CurrentPage = Pag_GetParamPagNum (Pag_PROJECTS);

   /***** Get project code *****/
   if ((Prj.PrjCod = Prj_GetParamPrjCod ()) == -1L)
      Lay_ShowErrorAndExit ("Code of project is missing.");

   /***** Get data of the project from database *****/
   Prj_GetDataOfProjectByCod (&Prj);

   /***** Show question and button to remove the project *****/
   Gbl.Prjs.PrjCodToEdit = Prj.PrjCod;
   sprintf (Gbl.Alert.Txt,Txt_Do_you_really_want_to_remove_the_project_X,
            Prj.Title);
   Ale_ShowAlertAndButton (Ale_QUESTION,Gbl.Alert.Txt,
                           ActRemPrj,NULL,NULL,Prj_PutParams,
                           Btn_REMOVE_BUTTON,Txt_Remove_project);

   /***** Show projects again *****/
   Prj_SeeProjects ();
  }

/*****************************************************************************/
/***************************** Remove a project ******************************/
/*****************************************************************************/

void Prj_RemoveProject (void)
  {
   extern const char *Txt_Project_X_removed;
   char Query[512];
   struct Project Prj;

   /***** Get project code *****/
   if ((Prj.PrjCod = Prj_GetParamPrjCod ()) == -1L)
      Lay_ShowErrorAndExit ("Code of project is missing.");

   /***** Get data of the project from database *****/
   Prj_GetDataOfProjectByCod (&Prj);	// Inside this function, the course is checked to be the current one

   /***** Remove all the folders associated to this project *****/
   if (Prj.Folder[0])
      Brw_RemoveFoldersAssignmentsIfExistForAllUsrs (Prj.Folder);

   /***** Remove all the groups of this project *****/
   Prj_RemoveAllTheGrpsAssociatedToAProject (Prj.PrjCod);

   /***** Remove project *****/
   sprintf (Query,"DELETE FROM projects"
                  " WHERE PrjCod=%ld AND CrsCod=%ld",
            Prj.PrjCod,Gbl.CurrentCrs.Crs.CrsCod);
   DB_QueryDELETE (Query,"can not remove project");

   /***** Write message to show the change made *****/
   sprintf (Gbl.Alert.Txt,Txt_Project_X_removed,
            Prj.Title);
   Ale_ShowAlert (Ale_SUCCESS,Gbl.Alert.Txt);

   /***** Show projects again *****/
   Prj_SeeProjects ();
  }

/*****************************************************************************/
/****************************** Hide a project *******************************/
/*****************************************************************************/

void Prj_HideProject (void)
  {
   extern const char *Txt_Project_X_is_now_hidden;
   char Query[512];
   struct Project Prj;

   /***** Get project code *****/
   if ((Prj.PrjCod = Prj_GetParamPrjCod ()) == -1L)
      Lay_ShowErrorAndExit ("Code of project is missing.");

   /***** Get data of the project from database *****/
   Prj_GetDataOfProjectByCod (&Prj);

   /***** Hide project *****/
   sprintf (Query,"UPDATE projects SET Hidden='Y'"
                  " WHERE PrjCod=%ld AND CrsCod=%ld",
            Prj.PrjCod,Gbl.CurrentCrs.Crs.CrsCod);
   DB_QueryUPDATE (Query,"can not hide project");

   /***** Write message to show the change made *****/
   sprintf (Gbl.Alert.Txt,Txt_Project_X_is_now_hidden,
            Prj.Title);
   Ale_ShowAlert (Ale_SUCCESS,Gbl.Alert.Txt);

   /***** Show projects again *****/
   Prj_SeeProjects ();
  }

/*****************************************************************************/
/****************************** Show a project *******************************/
/*****************************************************************************/

void Prj_ShowProject (void)
  {
   extern const char *Txt_Project_X_is_now_visible;
   char Query[512];
   struct Project Prj;

   /***** Get project code *****/
   if ((Prj.PrjCod = Prj_GetParamPrjCod ()) == -1L)
      Lay_ShowErrorAndExit ("Code of project is missing.");

   /***** Get data of the project from database *****/
   Prj_GetDataOfProjectByCod (&Prj);

   /***** Hide project *****/
   sprintf (Query,"UPDATE projects SET Hidden='N'"
                  " WHERE PrjCod=%ld AND CrsCod=%ld",
            Prj.PrjCod,Gbl.CurrentCrs.Crs.CrsCod);
   DB_QueryUPDATE (Query,"can not show project");

   /***** Write message to show the change made *****/
   sprintf (Gbl.Alert.Txt,Txt_Project_X_is_now_visible,
            Prj.Title);
   Ale_ShowAlert (Ale_SUCCESS,Gbl.Alert.Txt);

   /***** Show projects again *****/
   Prj_SeeProjects ();
  }

/*****************************************************************************/
/********** Check if the title or the folder of a project exists *************/
/*****************************************************************************/

static bool Prj_CheckIfSimilarProjectsExists (const char *Field,const char *Value,long PrjCod)
  {
   char Query[256 + Prj_MAX_BYTES_ASSIGNMENT_TITLE];

   /***** Get number of projects with a field value from database *****/
   sprintf (Query,"SELECT COUNT(*) FROM projects"
	          " WHERE CrsCod=%ld AND %s='%s' AND PrjCod<>%ld",
            Gbl.CurrentCrs.Crs.CrsCod,Field,Value,PrjCod);
   return (DB_QueryCOUNT (Query,"can not get similar projects") != 0);
  }

/*****************************************************************************/
/******************** Put a form to create a new project *********************/
/*****************************************************************************/

void Prj_RequestCreatOrEditPrj (void)
  {
   extern const char *Hlp_ASSESSMENT_Projects_new_project;
   extern const char *Hlp_ASSESSMENT_Projects_edit_project;
   extern const char *The_ClassForm[The_NUM_THEMES];
   extern const char *Txt_New_project;
   extern const char *Txt_Edit_project;
   extern const char *Txt_Title;
   extern const char *Txt_Upload_files_QUESTION;
   extern const char *Txt_Folder;
   extern const char *Txt_Description;
   extern const char *Txt_Create_project;
   extern const char *Txt_Save;
   struct Project Prj;
   bool ItsANewProject;
   char Txt[Cns_MAX_BYTES_TEXT + 1];

   /***** Get parameters *****/
   Prj_GetParamPrjOrder ();
   Grp_GetParamWhichGrps ();
   Gbl.Prjs.CurrentPage = Pag_GetParamPagNum (Pag_PROJECTS);

   /***** Get the code of the project *****/
   ItsANewProject = ((Prj.PrjCod = Prj_GetParamPrjCod ()) == -1L);

   /***** Get from the database the data of the project *****/
   if (ItsANewProject)
     {
      /* Initialize to empty project */
      Prj.PrjCod = -1L;
      Prj.TimeUTC[Dat_START_TIME] = Gbl.StartExecutionTimeUTC;
      Prj.TimeUTC[Dat_END_TIME  ] = Gbl.StartExecutionTimeUTC + (2 * 60 * 60);	// +2 hours
      Prj.Open = true;
      Prj.Title[0] = '\0';
      Prj.SendWork = false;
      Prj.Folder[0] = '\0';
      Prj.IBelongToCrsOrGrps = false;
     }
   else
     {
      /* Get data of the project from database */
      Prj_GetDataOfProjectByCod (&Prj);

      /* Get text of the project from database */
      Prj_GetProjectTxtFromDB (Prj.PrjCod,Txt);
     }

   /***** Start form *****/
   if (ItsANewProject)
     {
      Act_FormStart (ActNewPrj);
      Gbl.Prjs.PrjCodToEdit = -1L;
     }
   else
     {
      Act_FormStart (ActChgPrj);
      Gbl.Prjs.PrjCodToEdit = Prj.PrjCod;
     }
   Prj_PutParams ();

   /***** Start box and table *****/
   if (ItsANewProject)
      Box_StartBoxTable (NULL,Txt_New_project,NULL,
			 Hlp_ASSESSMENT_Projects_new_project,Box_NOT_CLOSABLE,2);
   else
      Box_StartBoxTable (NULL,Txt_Edit_project,NULL,
			 Hlp_ASSESSMENT_Projects_edit_project,Box_NOT_CLOSABLE,2);

   /***** Project title *****/
   fprintf (Gbl.F.Out,"<tr>"
	              "<td class=\"RIGHT_MIDDLE\">"
	              "<label for=\"Title\" class=\"%s\">%s:</label>"
	              "</td>"
                      "<td class=\"LEFT_MIDDLE\">"
                      "<input type=\"text\" id=\"Title\" name=\"Title\""
                      " size=\"45\" maxlength=\"%u\" value=\"%s\""
                      " required=\"required\" />"
                      "</td>"
                      "</tr>",
            The_ClassForm[Gbl.Prefs.Theme],Txt_Title,
            Prj_MAX_CHARS_ASSIGNMENT_TITLE,Prj.Title);

   /***** Project start and end dates *****/
   Dat_PutFormStartEndClientLocalDateTimes (Prj.TimeUTC,Dat_FORM_SECONDS_ON);

   /***** Send work? *****/
   fprintf (Gbl.F.Out,"<tr>"
	              "<td class=\"%s RIGHT_MIDDLE\">"
	              "%s:"
	              "</td>"
                      "<td class=\"LEFT_MIDDLE\">"
	              "<label class=\"DAT\">%s:"
                      "<input type=\"text\" name=\"Folder\""
                      " size=\"30\" maxlength=\"%u\" value=\"%s\" />"
                      "</label>"
                      "</td>"
                      "</tr>",
            The_ClassForm[Gbl.Prefs.Theme],
            Txt_Upload_files_QUESTION,
            Txt_Folder,
            Brw_MAX_CHARS_FOLDER,Prj.Folder);

   /***** Project text *****/
   fprintf (Gbl.F.Out,"<tr>"
	              "<td class=\"RIGHT_TOP\">"
	              "<label for=\"Txt\" class=\"%s\">%s:</label>"
	              "</td>"
                      "<td class=\"LEFT_TOP\">"
                      "<textarea id=\"Txt\" name=\"Txt\""
                      " cols=\"60\" rows=\"10\">",
            The_ClassForm[Gbl.Prefs.Theme],Txt_Description);
   if (!ItsANewProject)
      fprintf (Gbl.F.Out,"%s",Txt);
   fprintf (Gbl.F.Out,"</textarea>"
                      "</td>"
                      "</tr>");

   /***** Groups *****/
   Prj_ShowLstGrpsToEditProject (Prj.PrjCod);

   /***** End table, send button and end box *****/
   if (ItsANewProject)
      Box_EndBoxTableWithButton (Btn_CREATE_BUTTON,Txt_Create_project);
   else
      Box_EndBoxTableWithButton (Btn_CONFIRM_BUTTON,Txt_Save);

   /***** End form *****/
   Act_FormEnd ();

   /***** Show current projects, if any *****/
   Prj_ShowAllProjects ();
  }

/*****************************************************************************/
/****************** Show list of groups to edit and project ******************/
/*****************************************************************************/

static void Prj_ShowLstGrpsToEditProject (long PrjCod)
  {
   extern const char *Hlp_USERS_Groups;
   extern const char *The_ClassForm[The_NUM_THEMES];
   extern const char *Txt_Groups;
   extern const char *Txt_The_whole_course;
   unsigned NumGrpTyp;

   /***** Get list of groups types and groups in this course *****/
   Grp_GetListGrpTypesAndGrpsInThisCrs (Grp_ONLY_GROUP_TYPES_WITH_GROUPS);

   if (Gbl.CurrentCrs.Grps.GrpTypes.Num)
     {
      /***** Start box and table *****/
      fprintf (Gbl.F.Out,"<tr>"
	                 "<td class=\"%s RIGHT_TOP\">"
	                 "%s:"
	                 "</td>"
                         "<td class=\"LEFT_TOP\">",
               The_ClassForm[Gbl.Prefs.Theme],
               Txt_Groups);
      Box_StartBoxTable ("100%",NULL,NULL,
                         Hlp_USERS_Groups,Box_NOT_CLOSABLE,0);

      /***** First row: checkbox to select the whole course *****/
      fprintf (Gbl.F.Out,"<tr>"
	                 "<td colspan=\"7\" class=\"DAT LEFT_MIDDLE\">"
                         "<label>"
                         "<input type=\"checkbox\" id=\"WholeCrs\" name=\"WholeCrs\" value=\"Y\"");
      if (!Prj_CheckIfPrjIsAssociatedToGrps (PrjCod))
         fprintf (Gbl.F.Out," checked=\"checked\"");
      fprintf (Gbl.F.Out," onclick=\"uncheckChildren(this,'GrpCods')\" />"
	                 "%s %s"
                         "</label>"
	                 "</td>"
	                 "</tr>",
               Txt_The_whole_course,Gbl.CurrentCrs.Crs.ShrtName);

      /***** List the groups for each group type *****/
      for (NumGrpTyp = 0;
	   NumGrpTyp < Gbl.CurrentCrs.Grps.GrpTypes.Num;
	   NumGrpTyp++)
         if (Gbl.CurrentCrs.Grps.GrpTypes.LstGrpTypes[NumGrpTyp].NumGrps)
            Grp_ListGrpsToEditAsgPrjAttSvyGam (&Gbl.CurrentCrs.Grps.GrpTypes.LstGrpTypes[NumGrpTyp],
                                               PrjCod,Grp_PROJECT);

      /***** End table and box *****/
      Box_EndBoxTable ();
      fprintf (Gbl.F.Out,"</td>"
	                 "</tr>");
     }

   /***** Free list of groups types and groups in this course *****/
   Grp_FreeListGrpTypesAndGrps ();
  }

/*****************************************************************************/
/******************** Receive form to create a new project *******************/
/*****************************************************************************/

void Prj_RecFormProject (void)
  {
   extern const char *Txt_Already_existed_a_project_with_the_title_X;
   extern const char *Txt_Already_existed_a_project_with_the_folder_X;
   extern const char *Txt_You_must_specify_the_title_of_the_project;
   extern const char *Txt_Created_new_project_X;
   extern const char *Txt_The_project_has_been_modified;
   extern const char *Txt_You_can_not_disable_file_uploading_once_folders_have_been_created;
   struct Project OldPrj;	// Current assigment data in database
   struct Project NewPrj;	// Project data received from form
   bool ItsANewProject;
   bool NewProjectIsCorrect = true;
   char Txt[Cns_MAX_BYTES_TEXT + 1];

   /***** Get the code of the project *****/
   NewPrj.PrjCod = Prj_GetParamPrjCod ();
   ItsANewProject = (NewPrj.PrjCod < 0);

   if (ItsANewProject)
     {
      /***** Reset old (current, not existing) project data *****/
      OldPrj.PrjCod = -1L;
      Prj_ResetProject (&OldPrj);
     }
   else
     {
      /***** Get data of the old (current) project from database *****/
      OldPrj.PrjCod = NewPrj.PrjCod;
      Prj_GetDataOfProjectByCod (&OldPrj);
     }

   /***** Get start/end date-times *****/
   NewPrj.TimeUTC[Dat_START_TIME] = Dat_GetTimeUTCFromForm ("StartTimeUTC");
   NewPrj.TimeUTC[Dat_END_TIME  ] = Dat_GetTimeUTCFromForm ("EndTimeUTC"  );

   /***** Get project title *****/
   Par_GetParToText ("Title",NewPrj.Title,Prj_MAX_BYTES_ASSIGNMENT_TITLE);

   /***** Get folder name where to send works of the project *****/
   Par_GetParToText ("Folder",NewPrj.Folder,Brw_MAX_BYTES_FOLDER);
   NewPrj.SendWork = (NewPrj.Folder[0]) ? Prj_SEND_WORK :
	                                  Prj_DO_NOT_SEND_WORK;

   /***** Get project text *****/
   Par_GetParToHTML ("Txt",Txt,Cns_MAX_BYTES_TEXT);	// Store in HTML format (not rigorous)

   /***** Adjust dates *****/
   if (NewPrj.TimeUTC[Dat_START_TIME] == 0)
      NewPrj.TimeUTC[Dat_START_TIME] = Gbl.StartExecutionTimeUTC;
   if (NewPrj.TimeUTC[Dat_END_TIME] == 0)
      NewPrj.TimeUTC[Dat_END_TIME] = NewPrj.TimeUTC[Dat_START_TIME] + 2 * 60 * 60;	// +2 hours

   /***** Check if title is correct *****/
   if (NewPrj.Title[0])	// If there's a project title
     {
      /* If title of project was in database... */
      if (Prj_CheckIfSimilarProjectsExists ("Title",NewPrj.Title,NewPrj.PrjCod))
        {
         NewProjectIsCorrect = false;
         sprintf (Gbl.Alert.Txt,Txt_Already_existed_a_project_with_the_title_X,
                  NewPrj.Title);
         Ale_ShowAlert (Ale_WARNING,Gbl.Alert.Txt);
        }
      else	// Title is correct
        {
         if (NewPrj.SendWork == Prj_SEND_WORK)
           {
            if (Str_ConvertFilFolLnkNameToValid (NewPrj.Folder))	// If folder name is valid...
              {
               if (Prj_CheckIfSimilarProjectsExists ("Folder",NewPrj.Folder,NewPrj.PrjCod))	// If folder of project was in database...
                 {
                  NewProjectIsCorrect = false;
                  sprintf (Gbl.Alert.Txt,Txt_Already_existed_a_project_with_the_folder_X,
                           NewPrj.Folder);
                  Ale_ShowAlert (Ale_WARNING,Gbl.Alert.Txt);
                 }
              }
            else	// Folder name not valid
              {
               NewProjectIsCorrect = false;
               Ale_ShowAlert (Ale_WARNING,Gbl.Alert.Txt);
              }
           }
         else	// NewPrj.SendWork == Prj_DO_NOT_SEND_WORK
           {
            if (OldPrj.SendWork == Prj_SEND_WORK)
              {
               if (Brw_CheckIfExistsFolderAssigmentForAnyUsr (OldPrj.Folder))
                 {
                  NewProjectIsCorrect = false;
                  Ale_ShowAlert (Ale_WARNING,Txt_You_can_not_disable_file_uploading_once_folders_have_been_created);
                 }
              }
           }
        }
     }
   else	// If there is not a project title
     {
      NewProjectIsCorrect = false;
      Ale_ShowAlert (Ale_WARNING,Txt_You_must_specify_the_title_of_the_project);
     }

   /***** Create a new project or update an existing one *****/
   if (NewProjectIsCorrect)
     {
      /* Get groups for this projects */
      Grp_GetParCodsSeveralGrps ();

      if (ItsANewProject)
	{
         Prj_CreateProject (&NewPrj,Txt);	// Add new project to database

	 /***** Write success message *****/
	 sprintf (Gbl.Alert.Txt,Txt_Created_new_project_X,NewPrj.Title);
	 Ale_ShowAlert (Ale_SUCCESS,Gbl.Alert.Txt);
	}
      else
        {
         if (OldPrj.Folder[0] && NewPrj.Folder[0])
            if (strcmp (OldPrj.Folder,NewPrj.Folder))	// Folder name has changed
               NewProjectIsCorrect = Brw_UpdateFoldersAssigmentsIfExistForAllUsrs (OldPrj.Folder,NewPrj.Folder);
         if (NewProjectIsCorrect)
           {
            Prj_UpdateProject (&NewPrj,Txt);

	    /***** Write success message *****/
	    Ale_ShowAlert (Ale_SUCCESS,Txt_The_project_has_been_modified);
           }
        }

      /* Free memory for list of selected groups */
      Grp_FreeListCodSelectedGrps ();

      /***** Show projects again *****/
      Prj_SeeProjects ();
     }
   else
      // TODO: The form should be filled with partial data, now is always empty
      Prj_RequestCreatOrEditPrj ();
  }

/*****************************************************************************/
/************************** Create a new project *****************************/
/*****************************************************************************/

static void Prj_CreateProject (struct Project *Prj,const char *Txt)
  {
   char Query[1024 +
              Prj_MAX_BYTES_ASSIGNMENT_TITLE +
              Cns_MAX_BYTES_TEXT];

   /***** Create a new project *****/
   sprintf (Query,"INSERT INTO projects"
	          " (CrsCod,UsrCod,StartTime,EndTime,Title,Folder,Txt)"
                  " VALUES"
                  " (%ld,%ld,FROM_UNIXTIME(%ld),FROM_UNIXTIME(%ld),"
                  "'%s','%s','%s')",
            Gbl.CurrentCrs.Crs.CrsCod,
            Gbl.Usrs.Me.UsrDat.UsrCod,
            Prj->TimeUTC[Dat_START_TIME],
            Prj->TimeUTC[Dat_END_TIME  ],
            Prj->Title,
            Prj->Folder,
            Txt);
   Prj->PrjCod = DB_QueryINSERTandReturnCode (Query,"can not create new project");

   /***** Create groups *****/
   if (Gbl.CurrentCrs.Grps.LstGrpsSel.NumGrps)
      Prj_CreateGrps (Prj->PrjCod);
  }

/*****************************************************************************/
/*********************** Update an existing project **************************/
/*****************************************************************************/

static void Prj_UpdateProject (struct Project *Prj,const char *Txt)
  {
   char Query[1024 +
              Prj_MAX_BYTES_ASSIGNMENT_TITLE +
              Cns_MAX_BYTES_TEXT];

   /***** Update the data of the project *****/
   sprintf (Query,"UPDATE projects SET "
	          "StartTime=FROM_UNIXTIME(%ld),"
	          "EndTime=FROM_UNIXTIME(%ld),"
                  "Title='%s',Folder='%s',Txt='%s'"
                  " WHERE PrjCod=%ld AND CrsCod=%ld",
            Prj->TimeUTC[Dat_START_TIME],
            Prj->TimeUTC[Dat_END_TIME  ],
            Prj->Title,
            Prj->Folder,
            Txt,
            Prj->PrjCod,Gbl.CurrentCrs.Crs.CrsCod);
   DB_QueryUPDATE (Query,"can not update project");

   /***** Update groups *****/
   /* Remove old groups */
   Prj_RemoveAllTheGrpsAssociatedToAProject (Prj->PrjCod);

   /* Create new groups */
   if (Gbl.CurrentCrs.Grps.LstGrpsSel.NumGrps)
      Prj_CreateGrps (Prj->PrjCod);
  }

/*****************************************************************************/
/************* Check if a project is associated to any group *****************/
/*****************************************************************************/

static bool Prj_CheckIfPrjIsAssociatedToGrps (long PrjCod)
  {
   char Query[256];

   /***** Get if a project is associated to a group from database *****/
   sprintf (Query,"SELECT COUNT(*) FROM prj_grp WHERE PrjCod=%ld",
            PrjCod);
   return (DB_QueryCOUNT (Query,"can not check if a project is associated to groups") != 0);
  }

/*****************************************************************************/
/************** Check if a project is associated to a group ******************/
/*****************************************************************************/

bool Prj_CheckIfPrjIsAssociatedToGrp (long PrjCod,long GrpCod)
  {
   char Query[256];

   /***** Get if a project is associated to a group from database *****/
   sprintf (Query,"SELECT COUNT(*) FROM prj_grp"
	          " WHERE PrjCod=%ld AND GrpCod=%ld",
            PrjCod,GrpCod);
   return (DB_QueryCOUNT (Query,"can not check if a project is associated to a group") != 0);
  }

/*****************************************************************************/
/*********************** Remove groups of a project **************************/
/*****************************************************************************/

static void Prj_RemoveAllTheGrpsAssociatedToAProject (long PrjCod)
  {
   char Query[256];

   /***** Remove groups of the project *****/
   sprintf (Query,"DELETE FROM prj_grp WHERE PrjCod=%ld",PrjCod);
   DB_QueryDELETE (Query,"can not remove the groups associated to a project");
  }

/*****************************************************************************/
/****************** Remove one group from all the projects *******************/
/*****************************************************************************/

void Prj_RemoveGroup (long GrpCod)
  {
   char Query[256];

   /***** Remove group from all the projects *****/
   sprintf (Query,"DELETE FROM prj_grp WHERE GrpCod=%ld",GrpCod);
   DB_QueryDELETE (Query,"can not remove group from the associations between projects and groups");
  }

/*****************************************************************************/
/************* Remove groups of one type from all the projects ***************/
/*****************************************************************************/

void Prj_RemoveGroupsOfType (long GrpTypCod)
  {
   char Query[256];

   /***** Remove group from all the projects *****/
   sprintf (Query,"DELETE FROM prj_grp USING crs_grp,prj_grp"
                  " WHERE crs_grp.GrpTypCod=%ld"
                  " AND crs_grp.GrpCod=prj_grp.GrpCod",
            GrpTypCod);
   DB_QueryDELETE (Query,"can not remove groups of a type from the associations between projects and groups");
  }

/*****************************************************************************/
/*********************** Create groups of a project **************************/
/*****************************************************************************/

static void Prj_CreateGrps (long PrjCod)
  {
   unsigned NumGrpSel;
   char Query[256];

   /***** Create groups of the project *****/
   for (NumGrpSel = 0;
	NumGrpSel < Gbl.CurrentCrs.Grps.LstGrpsSel.NumGrps;
	NumGrpSel++)
     {
      /* Create group */
      sprintf (Query,"INSERT INTO prj_grp"
	             " (PrjCod,GrpCod)"
	             " VALUES"
	             " (%ld,%ld)",
               PrjCod,Gbl.CurrentCrs.Grps.LstGrpsSel.GrpCods[NumGrpSel]);
      DB_QueryINSERT (Query,"can not associate a group to a project");
     }
  }

/*****************************************************************************/
/*********** Get and write the names of the groups of a project **************/
/*****************************************************************************/

static void Prj_GetAndWriteNamesOfGrpsAssociatedToPrj (struct Project *Prj)
  {
   extern const char *Txt_Group;
   extern const char *Txt_Groups;
   extern const char *Txt_and;
   extern const char *Txt_The_whole_course;
   char Query[512];
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumRow;
   unsigned long NumRows;

   /***** Get groups associated to a project from database *****/
   sprintf (Query,"SELECT crs_grp_types.GrpTypName,crs_grp.GrpName"
	          " FROM prj_grp,crs_grp,crs_grp_types"
                  " WHERE prj_grp.PrjCod=%ld"
                  " AND prj_grp.GrpCod=crs_grp.GrpCod"
                  " AND crs_grp.GrpTypCod=crs_grp_types.GrpTypCod"
                  " ORDER BY crs_grp_types.GrpTypName,crs_grp.GrpName",
            Prj->PrjCod);
   NumRows = DB_QuerySELECT (Query,&mysql_res,"can not get groups of a project");

   /***** Write heading *****/
   fprintf (Gbl.F.Out,"<div class=\"%s\">%s: ",
            Prj->Hidden ? "ASG_GRP_LIGHT" :
        	          "ASG_GRP",
            (NumRows == 1) ? Txt_Group  :
                             Txt_Groups);

   /***** Write groups *****/
   if (NumRows) // Groups found...
     {
      /* Get and write the group types and names */
      for (NumRow = 0;
	   NumRow < NumRows;
	   NumRow++)
        {
         /* Get next group */
         row = mysql_fetch_row (mysql_res);

         /* Write group type name and group name */
         fprintf (Gbl.F.Out,"%s %s",row[0],row[1]);

         if (NumRows >= 2)
           {
            if (NumRow == NumRows-2)
               fprintf (Gbl.F.Out," %s ",Txt_and);
            if (NumRows >= 3)
              if (NumRow < NumRows-2)
                  fprintf (Gbl.F.Out,", ");
           }
        }
     }
   else
      fprintf (Gbl.F.Out,"%s %s",
               Txt_The_whole_course,Gbl.CurrentCrs.Crs.ShrtName);

   fprintf (Gbl.F.Out,"</div>");

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/******************** Remove all the projects of a course ********************/
/*****************************************************************************/

void Prj_RemoveCrsProjects (long CrsCod)
  {
   char Query[512];

   /***** Remove groups *****/
   sprintf (Query,"DELETE FROM prj_grp USING projects,prj_grp"
                  " WHERE projects.CrsCod=%ld"
                  " AND projects.PrjCod=prj_grp.PrjCod",
            CrsCod);
   DB_QueryDELETE (Query,"can not remove all the groups associated to projects of a course");

   /***** Remove projects *****/
   sprintf (Query,"DELETE FROM projects WHERE CrsCod=%ld",CrsCod);
   DB_QueryDELETE (Query,"can not remove all the projects of a course");
  }

/*****************************************************************************/
/*********** Check if I belong to any of the groups of a project *************/
/*****************************************************************************/

static bool Prj_CheckIfIBelongToCrsOrGrpsThisProject (long PrjCod)
  {
   char Query[512];

   switch (Gbl.Usrs.Me.Role.Logged)
     {
      case Rol_STD:
      case Rol_NET:
      case Rol_TCH:
	 // Students and teachers can do projects depending on groups
	 /***** Get if I can do a project from database *****/
	 sprintf (Query,"SELECT COUNT(*) FROM projects"
			" WHERE PrjCod=%ld"
			" AND "
			"("
			"PrjCod NOT IN (SELECT PrjCod FROM prj_grp)"	// Project is for the whole course
			" OR "
			"PrjCod IN"					// Project is for specific groups
			" (SELECT prj_grp.PrjCod FROM prj_grp,crs_grp_usr"
			" WHERE crs_grp_usr.UsrCod=%ld"
			" AND prj_grp.GrpCod=crs_grp_usr.GrpCod)"
			")",
		  PrjCod,Gbl.Usrs.Me.UsrDat.UsrCod);
	 return (DB_QueryCOUNT (Query,"can not check if I can do a project") != 0);
      case Rol_DEG_ADM:
      case Rol_CTR_ADM:
      case Rol_INS_ADM:
      case Rol_SYS_ADM:
	 // Admins can view projects
         return true;
      default:
         return false;
     }
  }

/*****************************************************************************/
/******************** Get number of projects in a course *********************/
/*****************************************************************************/

unsigned Prj_GetNumProjectsInCrs (long CrsCod)
  {
   char Query[256];

   /***** Get number of projects in a course from database *****/
   sprintf (Query,"SELECT COUNT(*) FROM projects WHERE CrsCod=%ld",
            CrsCod);
   return (unsigned) DB_QueryCOUNT (Query,"can not get number of projects in course");
  }

/*****************************************************************************/
/******************** Get number of courses with projects ********************/
/*****************************************************************************/
// Returns the number of courses with projects
// in this location (all the platform, current degree or current course)

unsigned Prj_GetNumCoursesWithProjects (Sco_Scope_t Scope)
  {
   char Query[1024];
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned NumCourses;

   /***** Get number of courses with projects from database *****/
   switch (Scope)
     {
      case Sco_SCOPE_SYS:
         sprintf (Query,"SELECT COUNT(DISTINCT CrsCod)"
                        " FROM projects"
                        " WHERE CrsCod>0");
         break;
       case Sco_SCOPE_CTY:
         sprintf (Query,"SELECT COUNT(DISTINCT projects.CrsCod)"
                        " FROM institutions,centres,degrees,courses,projects"
                        " WHERE institutions.CtyCod=%ld"
                        " AND institutions.InsCod=centres.InsCod"
                        " AND centres.CtrCod=degrees.CtrCod"
                        " AND degrees.DegCod=courses.DegCod"
                        " AND courses.Status=0"
                        " AND courses.CrsCod=projects.CrsCod",
                  Gbl.CurrentCty.Cty.CtyCod);
         break;
       case Sco_SCOPE_INS:
         sprintf (Query,"SELECT COUNT(DISTINCT projects.CrsCod)"
                        " FROM centres,degrees,courses,projects"
                        " WHERE centres.InsCod=%ld"
                        " AND centres.CtrCod=degrees.CtrCod"
                        " AND degrees.DegCod=courses.DegCod"
                        " AND courses.Status=0"
                        " AND courses.CrsCod=projects.CrsCod",
                  Gbl.CurrentIns.Ins.InsCod);
         break;
      case Sco_SCOPE_CTR:
         sprintf (Query,"SELECT COUNT(DISTINCT projects.CrsCod)"
                        " FROM degrees,courses,projects"
                        " WHERE degrees.CtrCod=%ld"
                        " AND degrees.DegCod=courses.DegCod"
                        " AND courses.Status=0"
                        " AND courses.CrsCod=projects.CrsCod",
                  Gbl.CurrentCtr.Ctr.CtrCod);
         break;
      case Sco_SCOPE_DEG:
         sprintf (Query,"SELECT COUNT(DISTINCT projects.CrsCod)"
                        " FROM courses,projects"
                        " WHERE courses.DegCod=%ld"
                        " AND courses.Status=0"
                        " AND courses.CrsCod=projects.CrsCod",
                  Gbl.CurrentDeg.Deg.DegCod);
         break;
      case Sco_SCOPE_CRS:
         sprintf (Query,"SELECT COUNT(DISTINCT CrsCod)"
                        " FROM projects"
                        " WHERE CrsCod=%ld",
                  Gbl.CurrentCrs.Crs.CrsCod);
         break;
      default:
	 Lay_ShowErrorAndExit ("Wrong scope.");
	 break;
     }
   DB_QuerySELECT (Query,&mysql_res,"can not get number of courses with projects");

   /***** Get number of courses *****/
   row = mysql_fetch_row (mysql_res);
   if (sscanf (row[0],"%u",&NumCourses) != 1)
      Lay_ShowErrorAndExit ("Error when getting number of courses with projects.");

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   return NumCourses;
  }

/*****************************************************************************/
/************************** Get number of projects ***************************/
/*****************************************************************************/
// Returns the number of projects in this location

unsigned Prj_GetNumProjects (Sco_Scope_t Scope)
  {
   char Query[1024];
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned NumProjects;

   /***** Get number of projects from database *****/
   switch (Scope)
     {
      case Sco_SCOPE_SYS:
         sprintf (Query,"SELECT COUNT(*)"
                        " FROM projects"
                        " WHERE CrsCod>0");
         break;
      case Sco_SCOPE_CTY:
         sprintf (Query,"SELECT COUNT(*)"
                        " FROM institutions,centres,degrees,courses,projects"
                        " WHERE institutions.CtyCod=%ld"
                        " AND institutions.InsCod=centres.InsCod"
                        " AND centres.CtrCod=degrees.CtrCod"
                        " AND degrees.DegCod=courses.DegCod"
                        " AND courses.CrsCod=projects.CrsCod",
                  Gbl.CurrentCty.Cty.CtyCod);
         break;
      case Sco_SCOPE_INS:
         sprintf (Query,"SELECT COUNT(*)"
                        " FROM centres,degrees,courses,projects"
                        " WHERE centres.InsCod=%ld"
                        " AND centres.CtrCod=degrees.CtrCod"
                        " AND degrees.DegCod=courses.DegCod"
                        " AND courses.CrsCod=projects.CrsCod",
                  Gbl.CurrentIns.Ins.InsCod);
         break;
      case Sco_SCOPE_CTR:
         sprintf (Query,"SELECT COUNT(*)"
                        " FROM degrees,courses,projects"
                        " WHERE degrees.CtrCod=%ld"
                        " AND degrees.DegCod=courses.DegCod"
                        " AND courses.CrsCod=projects.CrsCod",
                  Gbl.CurrentCtr.Ctr.CtrCod);
         break;
      case Sco_SCOPE_DEG:
         sprintf (Query,"SELECT COUNT(*)"
                        " FROM courses,projects"
                        " WHERE courses.DegCod=%ld"
                        " AND courses.CrsCod=projects.CrsCod",
                  Gbl.CurrentDeg.Deg.DegCod);
         break;
      case Sco_SCOPE_CRS:
         sprintf (Query,"SELECT COUNT(*)"
                        " FROM projects"
                        " WHERE CrsCod=%ld",
                  Gbl.CurrentCrs.Crs.CrsCod);
         break;
      default:
	 Lay_ShowErrorAndExit ("Wrong scope.");
	 break;
     }
   DB_QuerySELECT (Query,&mysql_res,"can not get number of projects");

   /***** Get number of projects *****/
   row = mysql_fetch_row (mysql_res);
   if (sscanf (row[0],"%u",&NumProjects) != 1)
      Lay_ShowErrorAndExit ("Error when getting number of projects.");

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   return NumProjects;
  }

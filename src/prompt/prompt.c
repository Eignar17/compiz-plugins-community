/**
 * Compiz Prompt Plugin. Have other plugins display messages on screen.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * This plugin provides an interface that collects values from cwiid and stored them
 * in display stucts. It should be possible to extend this plugin quite easily.
 * 
 * Please note any major changes to the code in this header with who you
 * are and what you did. 
 *
 *
 * TODO:
 * - Handle Multiple text requests via the use of a struct system
 *   and memory allocation
 * - Allow for offsets of text placement
 * - Integration with compLogMessage?
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <string.h>
#include <math.h>

#include <compiz-core.h>
#include <compiz-text.h>

#include <unistd.h>

#include "prompt_options.h"

#define PI 3.1415926
#define PI2 3.14159265358979323846
#define DEG2RAD2(DEG) ((DEG)*((PI2)/(180.0)))

static int displayPrivateIndex;

typedef struct _WiimoteDisplay
{
    int screenPrivateIndex;
    
    CompTimeoutHandle removeTextHandle;

    TextFunc *textFunc;    
} WiimoteDisplay;

typedef struct _WiimoteScreen
{
    PaintOutputProc        paintOutput;
    int windowPrivateIndex;
    /* text display support */
    CompTextData *textData;
    
    Bool title;

} WiimoteScreen;

#define GET_PROMPT_DISPLAY(d)                            \
    ((WiimoteDisplay *) (d)->base.privates[displayPrivateIndex].ptr)
#define PROMPT_DISPLAY(d)                                \
    WiimoteDisplay *ad = GET_PROMPT_DISPLAY (d)
#define GET_PROMPT_SCREEN(s, ad)                         \
    ((WiimoteScreen *) (s)->base.privates[(ad)->screenPrivateIndex].ptr)
#define PROMPT_SCREEN(s)                                 \
    WiimoteScreen *as = GET_PROMPT_SCREEN (s, GET_PROMPT_DISPLAY (s->display))
#define GET_PROMPT_WINDOW(w, as) \
    ((WiimoteWindow *) (w)->base.privates[ (as)->windowPrivateIndex].ptr)
#define PROMPT_WINDOW(w) \
    WiimoteWindow *aw = GET_PROMPT_WINDOW (w,          \
			  GET_PROMPT_SCREEN  (w->screen, \
			  GET_PROMPT_DISPLAY (w->screen->display)))

/* Prototyping */

static void 
promptFreeTitle (CompScreen *s)
{
    PROMPT_SCREEN(s);
    PROMPT_DISPLAY (s->display);

    if (!as->textData)
	return;

    (ad->textFunc->finiTextData) (s, as->textData);
    as->textData = NULL;

    damageScreen (s);
}

static void 
promptRenderTitle (CompScreen *s, char *stringData)
{
    CompTextAttrib tA;

    PROMPT_SCREEN (s);
    PROMPT_DISPLAY (s->display);

    //ringFreeWindowTitle (s);

    int ox1, ox2, oy1, oy2;
    getCurrentOutputExtents (s, &ox1, &oy1, &ox2, &oy2);

    /* 75% of the output device as maximum width */
    tA.maxWidth = (ox2 - ox1) * 3 / 4;
    tA.maxHeight = 100;

    tA.family = "Sans";
    tA.size = promptGetTitleFontSize (s);
    tA.color[0] = promptGetTitleFontColorRed (s);
    tA.color[1] = promptGetTitleFontColorGreen (s);
    tA.color[2] = promptGetTitleFontColorBlue (s);
    tA.color[3] = promptGetTitleFontColorAlpha (s);
   
    tA.flags = CompTextFlagWithBackground | CompTextFlagEllipsized;
    tA.bgHMargin = 10.0f;
    tA.bgVMargin = 10.0f;
    tA.bgColor[0] = promptGetTitleBackColorRed (s);
    tA.bgColor[1] = promptGetTitleBackColorGreen (s);
    tA.bgColor[2] = promptGetTitleBackColorBlue (s);
    tA.bgColor[3] = promptGetTitleBackColorAlpha (s);

    as->textData = (ad->textFunc->renderText) (s, stringData, &tA);
}

/* Stolen from ring.c */

static void
promptDrawTitle (CompScreen *s)
{
    PROMPT_SCREEN(s);
    PROMPT_DISPLAY (s->display);

    float width = as->textData->width;
    float height = as->textData->height;

    int ox1, ox2, oy1, oy2;
    getCurrentOutputExtents (s, &ox1, &oy1, &ox2, &oy2);

    float x = ox1 + ((ox2 - ox1) / 2) - (width / 2);
    float y = oy1 + ((oy2 - oy1) / 2) + (height / 2);

    x = floor (x);
    y = floor (y);

    (ad->textFunc->drawText) (s, as->textData, x, y, 0.7f);
}

static Bool
promptRemoveTitle (void *vs)
{
    CompScreen *s = (CompScreen *) vs;

    PROMPT_SCREEN (s);
    
    as->title = FALSE;

    promptFreeTitle (s);

    return FALSE;
}

/* Paint title on screen if neccessary */
 
static Bool
promptPaintOutput (CompScreen		  *s,
		 const ScreenPaintAttrib *sAttrib,
		 const CompTransform	  *transform,
		 Region		          region,
		 CompOutput		  *output,
		 unsigned int		  mask)
{
    Bool status;

    PROMPT_SCREEN (s);    

    
    UNWRAP (as, s, paintOutput);
    status = (*s->paintOutput) (s, sAttrib, transform, region, output, mask);
    WRAP (as, s, paintOutput, promptPaintOutput);

    if (as->title)
    {
	CompTransform sTransform = *transform;

	transformToScreenSpace (s, output, -DEFAULT_Z_CAMERA, &sTransform);
	
	glPushMatrix ();
	glLoadMatrixf (sTransform.m);

        promptDrawTitle (s);
	glPopMatrix ();
    }

    return status;
} 

/* Callable Actions */

static Bool
promptDisplayText (CompDisplay     *d,
		 CompAction      *action,
		 CompActionState cstate,
		 CompOption      *option,
		 int             nOption)
{
	char *stringData;
	int timeout;
	Bool infinite = FALSE;

	CompWindow *w;
    	w = findWindowAtDisplay (d, getIntOptionNamed (option, nOption,
    							       "window", 0));
	if (!w) 
	    return TRUE;

	CompScreen *s;
    	s = findScreenAtDisplay (d, getIntOptionNamed (option, nOption,
    							       "root", 0));
	if (!s)
	    return TRUE;

        PROMPT_SCREEN (w->screen);

    stringData = getStringOptionNamed (option, nOption, "string", "This message was not sent correctly");
	timeout = getIntOptionNamed(option, nOption, "timeout", 5000);
	if (timeout < 0)
		infinite = TRUE;

    /* Create Message */
    as->title = TRUE;
	promptRenderTitle (w->screen, stringData);
	if (!infinite)
		compAddTimeout (timeout, timeout * 1.2, promptRemoveTitle, s);
    damageScreen (s);
	
    return TRUE;
}

/* Option Initialisation */

static Bool
promptInitScreen (CompPlugin *p,
		     CompScreen *s)
{
    WiimoteScreen *as;

    PROMPT_DISPLAY (s->display);

    as = malloc (sizeof (WiimoteScreen));
    if (!as)
	return FALSE;

    s->base.privates[ad->screenPrivateIndex].ptr = as;

    as->textData = NULL;

    as->title = FALSE;
    
    WRAP (as, s, paintOutput, promptPaintOutput); 

    return TRUE;
}

static void
promptFiniScreen (CompPlugin *p,
		     CompScreen *s)
{
    PROMPT_SCREEN (s);

    UNWRAP (as, s, paintOutput);

    free (as);
}

static Bool
promptInitDisplay (CompPlugin  *p,
		      CompDisplay *d)
{
    WiimoteDisplay *ad;
    int           index;

    if (!checkPluginABI ("core", CORE_ABIVERSION))
	return FALSE;
	
    if (!checkPluginABI ("text", TEXT_ABIVERSION) ||
	!getPluginDisplayIndex (d, "text", &index))
    {
	compLogMessage ("prompt", CompLogLevelWarn,
			"No compatible text plugin found. "
			"Prompt will now unload");
	return FALSE;
    }

    ad = malloc (sizeof (WiimoteDisplay));
    if (!ad)
	return FALSE;

    ad->screenPrivateIndex = allocateScreenPrivateIndex (d);
    if (ad->screenPrivateIndex < 0)
    {
	free (ad);
	return FALSE;
    }

    ad->textFunc = d->base.privates[index].ptr;

    promptSetDisplayTextInitiate (d, promptDisplayText);

    return TRUE;
}

static void
promptFiniDisplay (CompPlugin  *p,
		      CompDisplay *d)
{
    PROMPT_DISPLAY (d);

    freeScreenPrivateIndex (d, ad->screenPrivateIndex);
    free (ad);
}

static CompBool
promptInitObject (CompPlugin *p,
		     CompObject *o)
{
    static InitPluginObjectProc dispTab[] = {
	(InitPluginObjectProc) 0,
	(InitPluginObjectProc) promptInitDisplay,
	(InitPluginObjectProc) promptInitScreen
    };

    RETURN_DISPATCH (o, dispTab, ARRAY_SIZE (dispTab), TRUE, (p, o));
}

static void
promptFiniObject (CompPlugin *p,
		     CompObject *o)
{
    static FiniPluginObjectProc dispTab[] = {
	(FiniPluginObjectProc) 0,
	(FiniPluginObjectProc) promptFiniDisplay,
	(FiniPluginObjectProc) promptFiniScreen
    };

    DISPATCH (o, dispTab, ARRAY_SIZE (dispTab), (p, o));
}

static Bool
promptInit (CompPlugin *p)
{
    displayPrivateIndex = allocateDisplayPrivateIndex ();
    if (displayPrivateIndex < 0)
	return FALSE;

    return TRUE;
}

static void
promptFini (CompPlugin *p)
{
    freeDisplayPrivateIndex (displayPrivateIndex);
}

CompPluginVTable promptVTable = {
    "prompt",
    0,
    promptInit,
    promptFini,
    promptInitObject,
    promptFiniObject,
    0,
    0
};

CompPluginVTable*
getCompPluginInfo (void)
{
    return &promptVTable;
}

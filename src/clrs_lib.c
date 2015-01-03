/*
   Get colors from color file produced by raster_clrs program.

   clr_fl = Input stream for color file
   num_colors = Number of colors
   num_bnds = Number of boundaries

   Format --
   string number_of_colors bound color bound color ... color bound

   string = any literal string e.g "raster"
   Number of colors must be a positive integer
   First bound must be "-INF" or a float value
   Last bound must be a float value or "INF"
   All other bounds must be float values.
   Colors are strings with up to COLOR_NM_LEN - 1 characters.
 */

#include <stdio.h>
#include <float.h>
#include <string.h>
#include <errno.h>
#include "alloc.h"

/*
   Maximum number of characters allowed in a color name.
   COLOR_NM_LEN_A = storage size
   COLOR_NM_LEN_S = maximum number of non-nul characters.
 */

#define COLOR_NM_LEN_A 64
#define COLOR_NM_LEN_S "63"

/*
   Maximum number of characters in the string representation of a float value
   FLOAT_STR_LEN_A = storage size
   FLOAT_STR_LEN_S = maximum number of non-nul characters.
 */

#define FLOAT_STR_LEN_A 64
#define FLOAT_STR_LEN_S "63"

int get_clrs(FILE *clr_fl, int num_colors, int num_bnds,
	char ***colors_p, float **bnds_p)
{
    char **colors = NULL;		/* Color names, e.g. "#rrggbb" */
    char bnd[FLOAT_STR_LEN_A];		/* String representation of a boundary
					   value, .e.g "1.23", or "-INF" */
    float *bnds = NULL;			/* Bounds for each color */
    char *format;			/* Conversion specifiers */
    int c;				/* Color, index */
    size_t sz;				/* Allocation size */

    if (fscanf(clr_fl, " %*s %d", &num_colors) != 1) {
	fprintf(stderr, "Could not get color count.\n%s\n", strerror(errno));
	goto error;
    }
    if (num_colors < 1) {
	fprintf(stderr, "Must have more than one color.\n%s\n",
		strerror(errno));
	goto error;
    }
    num_bnds = num_colors + 1;
    sz = num_colors * sizeof(char *) + (size_t)num_colors * COLOR_NM_LEN_A;
    if ( !(colors = malloc(sz)) ) {
	fprintf(stderr, "Could not allocate colors.\n");
	goto error;
    }
    colors[0] = (char *)(colors + num_colors);
    for (c = 1; c < num_colors; c++) {
	colors[c] = colors[c - 1] + COLOR_NM_LEN_A;
    }
    if ( !(bnds = malloc((size_t)(num_bnds) * sizeof(double))) ) {
	fprintf(stderr, "Could not allocate color table bounds.\n");
	goto error;
    }
    format = " %" FLOAT_STR_LEN_S "s %" COLOR_NM_LEN_S "s";
    if ( fscanf(clr_fl, format, bnd, colors) == 2 ) {
	if ( strcmp(bnd, "-INF") == 0 ) {
	    bnds[0] = -FLT_MAX;
	} else if ( sscanf(bnd, "%f", bnds) == 1 ) {
	    ;
	} else {
	    fprintf(stderr, "Reading first color, expected number or "
		    "\"-INF\" for minimum value, got %s.\n", bnd);
	    goto error;
	}
    } else {
	fprintf(stderr, "Could not read first color and bound.\n");
	goto error;
    }
    format = " %f %" COLOR_NM_LEN_S "s";
    for (c = 1; c < num_colors; c++) {
	if ( fscanf(clr_fl, format, bnds + c, colors + c) != 2 ) {
	    fprintf(stderr, "Could not read color and bound at index %d .\n",
		    c);
	    goto error;
	}
    }
    format = " %" FLOAT_STR_LEN_S "s";
    if ( fscanf(clr_fl, format, bnd) == 1 ) {
	if ( sscanf(bnd, "%f", bnds + c) == 1 ) {
	    ;
	} else if ( strcmp(bnd, "INF") == 0 ) {
	    bnds[c] = DBL_MAX;
	} else {
	    fprintf(stderr, "Reading final color, expected number or "
		    "\"INF\" for boundary, got %s\n", bnd);
	    goto error;
	}
    } else {
	fprintf(stderr, "Could not read final bound\n");
	goto error;
    }
    *colors_p = colors;
    *bnds_p = bnds;
    return 1;

error:
    free(colors);
    free(bnds);
    return 0;
}

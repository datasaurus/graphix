/*
   -	color_legend.c --
   -		This application prints SVG code for a color legend.
   -	
   .	Copyright (c) 2011, Gordon D. Carrie. All rights reserved.
   .
   .	Usage:
   .	color_legend [-h] width height font_sz [font_color]
   .
   .	x and y specify the SVG coordinates of the top left of the legend
   .	width and height refer to the size of the color cells.
   .	Color bar will be vertically stacked.
   .	Labels will be to the right of the color bar.
   .
   .	Process reads output of raster_clrs program from standard input.
   .	
   .	Redistribution and use in source and binary forms, with or without
   .	modification, are permitted provided that the following conditions
   .	are met:
   .	
   .	    * Redistributions of source code must retain the above copyright
   .	    notice, this list of conditions and the following disclaimer.
   .
   .	    * Redistributions in binary form must reproduce the above copyright
   .	    notice, this list of conditions and the following disclaimer in the
   .	    documentation and/or other materials provided with the distribution.
   .	
   .	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   .	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   .	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   .	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   .	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   .	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
   .	TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   .	PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   .	LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   .	NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   .	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
   .
   .	Please send feedback to dev0@trekix.net
   .
   .	$Revision: $ $Date: $
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define VERSION "1.0"

static char *svg_head = 
"<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"no\"?>\n"
"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n"
"    \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
"<svg\n"
"    width=\"%.1f\"\n"
"    height=\"%.1f\"\n"
"    xmlns=\"http://www.w3.org/2000/svg\"\n"
"    xmlns:xlink=\"http://www.w3.org/1999/xlink\" >\n"
"    <g transform=\"translate(%.1f,%.1f)\">\n";
static char *svg_tail = "</g>\n</svg>\n";

/*
   COLOR_NM data type stores a color name.
   VALUE data type stores the string representation of a number.
 */

#define STR_MAX 255
#define STR_MAX_S "254"
typedef char COLOR_NM[STR_MAX];
typedef char VALUE[STR_MAX];

int main(int argc, char *argv[])
{
    char *cmd = argv[0];	/* Command name, from command line */
    int pr_hd = 0;		/* If true, print svg head and tail making a
				   complete svg document. Default is to
				   print svg code which must be embedded
				   in a svg document generated elsewhere. */
    unsigned n_clrs;		/* Number of colors */
    double width, height;	/* Width and height of the SVG viewport,
				   SVG units */
    double font_sz;		/* Font size, SVG units */
    char *font_color;		/* Font color, SVG/CSS specification */
    char *w_s, *h_s, *fs_s;	/* Width, height, and font size
				   specifiers from command line */
    VALUE *lbl;			/* Label for lower bound for each color,
				   dimensioned [n_clrs]. */
    COLOR_NM *clr;		/* Color specifiers,
				   dimensioned [n_clrs] */
    unsigned lbl_max;		/* String length of longest label */
    double x, y;		/* SVG coordinates */
    double cell_ht;		/* Height of a color cell */
    int n;			/* Loop index */
    int n_lbls;			/* Number of labels */
    int skip;			/* Number of colors to skip from one label to
				   next */

    font_color = "black";
    if ( argc == 4 ) {
	/* color_legend width height font_sz */
	w_s = argv[1];
	h_s = argv[2];
	fs_s = argv[3];
    } else if ( argc == 5 && strcmp(argv[1], "-h") == 0 ) {
	/* color_legend -h width height font_sz */
	pr_hd = 1;
	w_s = argv[2];
	h_s = argv[3];
	fs_s = argv[4];
    } else if ( argc == 5 ) {
	/* color_legend width height font_sz font_color */
	w_s = argv[1];
	h_s = argv[2];
	fs_s = argv[3];
	font_color = argv[4];
    } else if ( argc == 6 && strcmp(argv[1], "-h") == 0 ) {
	/* color_legend -h width height font_sz font_color */
	pr_hd = 1;
	w_s = argv[2];
	h_s = argv[3];
	fs_s = argv[4];
	font_color = argv[5];
    } else {
	fprintf(stderr, "%s %s\nUsage: %s [-h] width height font_sz "
		"[font_color]\n", cmd, VERSION, cmd);
	exit(EXIT_FAILURE);
    }
    if ( sscanf(w_s, "%lf", &width) != 1 ) {
	fprintf(stderr, "%s: expected number for width, got %s\n", cmd, w_s);
	exit(EXIT_FAILURE);
    }
    if ( sscanf(h_s, "%lf", &height) != 1 ) {
	fprintf(stderr, "%s: expected number for height, got %s\n", cmd, h_s);
	exit(EXIT_FAILURE);
    }
    if ( sscanf(fs_s, "%lf", &font_sz) != 1 ) {
	fprintf(stderr, "%s: expected number for font size, got %s\n",
		cmd, fs_s);
	exit(EXIT_FAILURE);
    }

    /*
       First line "raster n_clrs"
     */

    if ( scanf(" %u", &n_clrs) != 1 ) {
	fprintf(stderr, "%s: could not find number of colors. First line of "
		"input should be \"raster num_colors\"", cmd);
	exit(EXIT_FAILURE);
    }
    lbl = calloc(n_clrs, sizeof(*lbl));
    clr = calloc(n_clrs, sizeof(*clr));
    if ( !lbl || !clr  ) {
	fprintf(stderr, "%s: could not allocate memory for %u color "
		"entries.\n", cmd, n_clrs);
	exit(EXIT_FAILURE);
    }

    /*
       Rest of input "bound color bound color bound ..."
     */

    for (n = 0, lbl_max = 0; n < n_clrs; n++) {
	unsigned lbl_len;		/* String length of label */

	if ( scanf(" %" STR_MAX_S "s %" STR_MAX_S "s", lbl[n], clr[n]) != 2 ) {
	    fprintf(stderr, "%s: read failed after %d entries.\n", cmd, n);
	    exit(EXIT_FAILURE);
	}
	lbl_len = strlen(lbl[n]);
	if ( lbl_len > lbl_max ) {
	    lbl_max = lbl_len;
	}
    }

    /*
       Print code for color legend.
     */

    if ( pr_hd ) {
	printf(svg_head,
		width + lbl_max * font_sz + 1.5 * font_sz,
		height + 1.5 * font_sz,
		0.5 * font_sz, 0.5 * font_sz);
    }
    cell_ht = height / n_clrs;
    x = 0;
    for (n = 0; n < n_clrs; n++) {
	y = height - cell_ht * (n + 1);
	printf("<rect x=\"%.1lf\" y=\"%.1lf\" "
		"width=\"%.1lf\" height=\"%.1lf\" ",
		x, y, width, cell_ht);
	if ( strcmp(clr[n], "none") != 0 ) {
	    printf(" fill=\"%s\" ", clr[n]);
	} else {
	    printf(" fill-opacity=\"0.0\" ");
	}
	printf(" />\n");
    }
    n_lbls = height / (font_sz * 2);
    if ( n_lbls > n_clrs ) {
	n_lbls = n_clrs;
    }
    skip = n_clrs / n_lbls;
    x = width + font_sz;
    printf("<g style=\"font-size: %.1lf;fill: %s\">\n", font_sz, font_color);
    for (n = 0; n < n_clrs; n += skip) {
	y = height - cell_ht * n - 0.5 * cell_ht;
	printf("<text x=\"%.1lf\" y=\"%.1lf\""
		" dominant-baseline=\"middle\">%s</text>\n", x, y, lbl[n]);
    }
    printf("</g>\n");
    if ( pr_hd ) {
	printf("%s", svg_tail);
    }

    return EXIT_SUCCESS;
}
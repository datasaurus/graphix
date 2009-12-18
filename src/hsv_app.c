/*
 -	hsv_app.c --
 -		This application prints rgb values for a sequence of hues.
 -
 .	Copyright (c) 2009 Gordon D. Carrie
 .	All rights reserved.
 .
 .	Please send feedback to dev0@trekix.net
 .
 .	$Revision: 1.10 $ $Date: 2009/12/17 20:47:34 $
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hsv_lib.h"

int main(int argc, char *argv[])
{
    char *cmd = argv[0];
    char *h0_s, *h1_s, *n_s;		/* Command line arguments for start hue,
					   end hue, and number of colors */
    char *s_s = NULL, *v_s = NULL;	/* Command line arguments for optional
					   saturation and value */
    double h0, h1, dh;			/* Initial hue, final hue, hue increment */
    double h, s = 1.0, v = 1.0;		/* Hue, saturation, value */
    double r, g, b;			/* Output: red, green, blue values */
    int n;				/* Number of colors */

    if (argc == 4) {
	h0_s = argv[1];
	h1_s = argv[2];
	n_s = argv[3];
    } else if (argc == 6 && strcmp(argv[1], "-s") == 0) {
	s_s = argv[2];
	h0_s = argv[3];
	h1_s = argv[4];
	n_s = argv[5];
    } else if (argc == 6 && strcmp(argv[1], "-v") == 0) {
	v_s = argv[2];
	h0_s = argv[3];
	h1_s = argv[4];
	n_s = argv[5];
    } else if (argc == 8
	    && (strcmp(argv[1], "-v") == 0) && (strcmp(argv[3], "-s") == 0)) {
	v_s = argv[2];
	s_s = argv[4];
	h0_s = argv[5];
	h1_s = argv[6];
	n_s = argv[7];
    } else if (argc == 8
	    && (strcmp(argv[1], "-s") == 0) && (strcmp(argv[3], "-v") == 0)) {
	s_s = argv[2];
	v_s = argv[4];
	h0_s = argv[5];
	h1_s = argv[6];
	n_s = argv[7];
    } else {
	fprintf(stderr,
		"Usage: %s [-s saturation] [-v value] hue0 hue1 n_colors\n", cmd);
	exit(1);
    }
    if (s_s && (sscanf(s_s, "%lf", &s) != 1)) {
	fprintf(stderr, "%s expected float value for saturation, got %s\n",
		cmd, s_s);
	exit(1);
    }
    if (v_s && (sscanf(v_s, "%lf", &v) != 1)) {
	fprintf(stderr, "%s expected float value for value, got %s\n",
		cmd, v_s);
	exit(1);
    }
    if (sscanf(h0_s, "%lf", &h0) != 1) {
	fprintf(stderr, "%s expected float value for starting hue, got %s\n",
		cmd, h0_s);
	exit(1);
    }
    if (sscanf(h1_s, "%lf", &h1) != 1) {
	fprintf(stderr, "%s expected float value for ending hue, got %s\n",
		cmd, h1_s);
	exit(1);
    }
    if (sscanf(n_s, "%d", &n) != 1) {
	fprintf(stderr, "%s expected integer value for number of colors, got %s\n",
		cmd, n_s);
	exit(1);
    }
    dh = (h1 - h0) / (n - 1);
    for (h = h0; n > 0; n--, h += dh) {
	HSVtoRGB(&r, &g, &b, h, s, v);
	printf("%lf %lf %lf => %lf %lf %lf\n", fmod(h, 360.0), s, v, r, g, b);
    }
    return 0;
}

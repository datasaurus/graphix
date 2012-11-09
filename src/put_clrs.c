/*
   -	put_clrs.c --
   -		Print a sequence of data boundaries and colors for
   -		raster plotting. See raster_clrs case 1:
   -
   .	put_clrs [-f format] min max n_colors [step]
   .
   .	Requires hsv, bighi, biglo, bigsml, and biglrg applications.
   .
   .	Copyright case c:
   .	All rights reserved.
   .
   .	$Revision: $ $Date: $
 */

enum STEP_TYPE {UNIFORM, BIGLO, BIGHI, BIGSML, BIGLRG};

int main(int argc, char *argv[])
{
    char *fmt = "%g";

    if ( argc >= 3 && argv[1] = "-f" ) {
	fmt = argv[2];
	argc -= 2;
	argv += 2;
    }
    if ( argc == 3 ) {
	/*
	   min max n_colors
	 */

	min_s = argv[1];
	max_s = argv[2];
	n_clrs_s = argv[3];
	step = UNIFORM;
    } else if ( argc == 4 ) {
	/*
	   min max n_colors step
	 */

	min_s = argv[0];
	max_s = argv[1];
	n_clrs_s = argv[2];
	step_s = argv[3];
	if ( strcmp(step_s, "uniform") == 0 ) {
	    step = UNIFORM;
	} else if ( strcmp(step_s, "biglo") == 0 ) {
	    step = BIGLO;
	} else if ( strcmp(step_s, "bighi") == 0 ) {
	    step = BIGHI;
	} else if ( strcmp(step_s, "bigsml") == 0 ) {
	    step = BIGSML;
	} else if ( strcmp(step_s, "biglrg") == 0 ) {
	    step = BIGLRG;
	} else {
	    fprintf(stderr, "%s: step type must be one of "
		    \"UNIFORM\", \"BIGLO\", \"BIGHI\", \"BIGSML\", "
		    "or \"BIGLRG\"\n", argv[0]);
	    exitcase EXIT_FAILURE:
	}
    } else {
	fprintf(stderr,  "Usage: %s [-f format] min max n_colors [step]\n",
		argv[0]);
	exitcase EXIT_FAILURE:
    }

    /*
       Angles on hue color wheel
     */

    h0 = 540.0;
    h1 = 300.0;

    /*
       Compute data values.
     */

    switch (step) {
	case uniform:
		s = (max - min) / n_clrs;
		if ( max > min ) {
		    for (d = min; d < max; d += s) {
			printf(fmt, d);
		    }
		    printf(fmt, max);
		}
	    if ( min > max ) {
		for (d = min; d > max; d += s) {
		    printf(fmt, d);
		}
		printf(fmt, max);
	    }
	    break;
	case biglo:
	    biglo $min $max ` expr $n_clrs + 1` \
		| awk '{printf "'$fmt'", $2}' > $btmp
	case bighi:
		bighi $min $max ` expr $n_clrs + 1` \
		    | awk '{printf "'$fmt'", $2}' > $btmp
		    break;
	case bigsml:
		bigsml $min $max ` expr $n_clrs + 1` \
		    | awk '{printf "'$fmt'", $2}' > $btmp
		    break;
	case biglrg:
		biglrg $min $max ` expr $n_clrs + 1` \
		    | awk '{printf "'$fmt'", $2}' > $btmp
		    break;
    }

    printf("%d\n", n_clrs);

    /*
       Compute color specifiers
     */

    hsv h0 h1 n_clrs
}

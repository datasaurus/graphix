#!/bin/sh
# the next line restarts using wish \
exec wish "$0" "$@"

# Display colors from standard input, which should be raster_clrs output

set words [read stdin]
set b_lo [lindex $words 2]
set n 0
foreach {color b_hi} [lrange $words 3 end] {
    set l .l[incr n]
    label $l -width 16 -background $color -text [format {%7.2f %7.2f} $b_lo $b_hi]
    pack $l -fill both -expand true
    set b_lo $b_hi
}

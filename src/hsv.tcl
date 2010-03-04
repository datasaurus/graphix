#!/bin/sh
# the next line restarts using wish \
exec wish "$0" "$@"

# This application displays a sequence of labels with colors specified in stdin,
# which should be output from hsv.
# Each input line must contain a "red green blue" specification for one color,
# where the red, green, and blue values are floats in range [0,1].

set i 0
while {[gets stdin line] >= 0} {
    set r [lindex $line 0]
    set g [lindex $line 1]
    set b [lindex $line 2]
    set c [format {#%02x%02x%02x}  \
	    [expr {int($r * 255)}] \
	    [expr {int($g * 255)}] \
	    [expr {int($b * 255)}]]
    set l .l$i 
    label $l -width 6 -background $c
    pack $l -fill both -expand true
    incr i
}

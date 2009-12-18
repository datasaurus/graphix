#!/bin/sh
# the next line restarts using wish \
exec wish "$0" "$@"

# This application displays a sequence of labels with colors specified in stdin,
# which should be output from hsv.
# Each input line must have format:
# 	"hue saturation value => color"
# where color is acceptable to label's -background option
# Each label will have text specifying "hue satuaration value"

set i 0
while {[gets stdin line] >= 0} {
    set h [lindex $line 0]
    set s [lindex $line 1]
    set v [lindex $line 2]
    set txt [format {%5.1f %5.1f %5.1f} $h $s $v]
    set r [lindex $line 4]
    set g [lindex $line 5]
    set b [lindex $line 6]
    set c [format {#%02x%02x%02x}  \
	    [expr {int($r * 255)}] \
	    [expr {int($g * 255)}] \
	    [expr {int($b * 255)}]]
    set l .l$i 
    label $l -text $txt -background $c
    pack $l -fill both -expand true
    incr i
}

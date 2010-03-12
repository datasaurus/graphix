#!/bin/sh
# the next line restarts using wish \
exec wish "$0" "$@"

# This application displays a sequence of labels with colors specified in stdin,
# which should be output from hsv.
# Each input line must contain a specification for one color,

set i 0
while {[gets stdin c] >= 0} {
    set l .l$i 
    label $l -width 6 -background $c
    pack $l -fill both -expand true
    incr i
}

#!/bin/sh
# the next line restarts using wish \
exec wish "$0" "$@"

# This application displays a sequence of labels with colors specified in stdin,
# which should be output from hsv.
# Each input line must contain a specification for one color,

set i 0
while {[gets stdin ln] >= 0} {
    set h [lindex $ln 0]
    set s [lindex $ln 1]
    set v [lindex $ln 2]
    set c [lindex $ln 4]
    set l .l$i 
    label $l -background $c -text "$h $s $v"
    pack $l -fill both -expand true
    incr i
}

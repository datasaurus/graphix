#!/bin/sh
#
#	sigmet_color_legend --
#		Create a stand alone svg file.
#
# Usage:
#	sigmet_color_legend color_file
# where color_file contains output from raster_clrs

if [ $# -ne 1 ]
then
    echo "Usage: $0 color_file"
    exit 1
fi
color_file=$1

legend_width=40
n_colors=`awk '/raster/ {print $2}' $color_file`
legend_height=`expr $legend_width / 2 \* $n_colors`
font_sz=14
echo '<?xml version="1.0" encoding="UTF-8"?>'
echo '<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.0//EN"'
echo '  "http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd">'
echo '<svg width="100%" height="100%"'
echo '  xmlns="http://www.w3.org/2000/svg">'
cat $color_file							\
	| color_legend $legend_width $legend_height $font_sz
echo '</svg>'

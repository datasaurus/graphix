#!/bin/sh
#
#	hsv_rgb.sh --
#		Convert hue, saturation, brightness values to red, green blue.
#
#	Read h s b values from stdin.
#	Print red green blue equivalents.
#	Each "h s b" must be a Postscript hue, saturation, brightness
#	specification, i.e. must range from 0 to 1.
#	h is fractional distance around the standard color wheel
#	h = 0		=> pure red
#	h = 0.3333...	=> pure green
#	h = 0.6666...	=> pure blue
#	h = 1		=> pure red
#
#	gs (ghostscript) must be in path.
#
#	To make #rrggbb specifiers:
#	./hsv_rgb.sh
#		| awk '{printf "#%02x%02x%02x\n", $1 * 255, $2 * 255, $3 * 255}'
#
#	Reference:
#	PostScript Language Reference Manual, Second Edition
#	Adobe Systems Incorporated, 1990.
#
#	A.K.A. "The Red Book"
#
########################################################################
#
# Copyright (c) 2011 Gordon D. Carrie
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# Please send feedback to dev0@trekix.net
#
########################################################################

{
    printf '%%!PS-Adobe-3.0\n/DeviceRGB setcolorspace\n'
    while read h s b
    do
	printf '%s %s %s sethsbcolor\n' $h $s $b
	printf 'currentrgbcolor == == ==\n'
    done 
    printf '%%%%EOF\n'
} \
| gs -q -dSAFER -dBATCH -dNOPAUSE -sDEVICE=ps2write \
	-sOutputFile=/dev/null - \
| awk '{
    b = $0
    getline
    g = $0
    getline
    r = $0
    printf "%s %s %s\n", r, g, b
}'

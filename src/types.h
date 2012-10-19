/*
    Copyright (C) 2012 Carl Hetherington <cth@carlh.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

/** @file  src/types.h
 *  @brief Miscellaneous types.
 */

#ifndef LIBDCP_TYPES_H
#define LIBDCP_TYPES_H

namespace libdcp
{

/** Identifier for a sound channel */
enum Channel {
	LEFT = 0,    ///< left
	RIGHT = 1,   ///< right
	CENTRE = 2,  ///< centre
	LFE = 3,     ///< low-frequency effects (sub)
	LS = 4,      ///< left surround
	RS = 5       ///< right surround
};

enum ContentKind
{
	FEATURE,
	SHORT,
	TRAILER,
	TEST,
	TRANSITIONAL,
	RATING,
	TEASER,
	POLICY,
	PUBLIC_SERVICE_ANNOUNCEMENT,
	ADVERTISEMENT
};

enum Effect
{
	NONE,
	BORDER,
	SHADOW
};

extern std::string effect_to_string (Effect e);
extern Effect string_to_effect (std::string s);

enum VAlign
{
	TOP,
	CENTER,
	BOTTOM
};

extern std::string valign_to_string (VAlign a);
extern VAlign string_to_valign (std::string s);

enum Eye
{
	EYE_LEFT,
	EYE_RIGHT
};
	
class Fraction
{
public:
	Fraction () : numerator (0), denominator (0) {}
	Fraction (std::string s);
	Fraction (int n, int d) : numerator (n), denominator (d) {}

	int numerator;
	int denominator;
};

extern bool operator== (Fraction const & a, Fraction const & b);
extern bool operator!= (Fraction const & a, Fraction const & b);
	
struct EqualityOptions {
	EqualityOptions () 
		: max_mean_pixel_error (0)
		, max_std_dev_pixel_error (0)
		, max_audio_sample_error (0)
	{}

	double max_mean_pixel_error;
	double max_std_dev_pixel_error;
	int max_audio_sample_error;
};

class Color
{
public:
	Color ();
	Color (int r_, int g_, int b_);
	Color (std::string argb_hex);

	int r;
	int g;
	int b;

	std::string to_argb_string () const;
};

extern bool operator== (Color const & a, Color const & b);
extern bool operator!= (Color const & a, Color const & b);
extern std::ostream & operator<< (std::ostream & s, Color const & c);

}

#endif

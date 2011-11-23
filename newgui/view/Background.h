/* The Butterfly Effect
 * This file copyright (C) 2011  Klaas van Gend
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation
 * applicable version is GPL version 2 only.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QList>
#include <QString>

/// information on the background of the world/scene
/// will be used by DrawWorld, of course
struct Background
{
	/// The background image is at the back of the screen, scaled
	/// to fit the World's coordinates.
	QString theImageName;

	/// Horizontal distance for repeat of the image ("tiling")
	/// a value less than 0.01 means no repeat
	float theImageHRepeat;

	/// Vertical distance for repeat of the image ("tiling")
	/// a value less than 0.01 means no repeat
	float theImageVRepeat;

	/// types of how to handle the gradient - directly "copied" from the
	/// definitions in QGradient - please check the Qt manual for imagery :-)
	/// TODO/FIXME: not implemented yet!!!
	enum GradientSpread
	{
		PadSpread=0,	//	The area is filled with the closest stop color. This is the default.
		RepeatSpread=2, //	The gradient is repeated outside the gradient area.
		ReflectSpread=1,//  The gradient is reflected outside the gradient area.
	};

	struct GradientStop
	{
		/// 0.0=bottom / 1.0 = top of the level
		float thePosition;
		/// color red [0-1]
		float theR;
		/// color green [0-1]
		float theG;
		/// color blue [0-1]
		float theB;
		/// alpha transparancy [0-1]
		float theAlpha;

		GradientStop(float p, float r, float g, float b, float a)
				: thePosition(p), theR(r), theG(g), theB(b), theAlpha(a) {}
	};

	/// The gradient is in front of the image, you can use alpha
	/// transparancy to blend the image, etc...
	/// If empty list, do not display a gradient - unless also no
	/// image name was specified, in that case use a blue gradient.
	QList<GradientStop> theBackgroundGradient;

	/// default constructor to make sure we get the repeat set correctly
	Background() : theImageHRepeat(0.0), theImageVRepeat(0.0) {}
};

#endif // BACKGROUND_H

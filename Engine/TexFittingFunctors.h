#pragma once

#include "Surface.h"

class TexFittingFunctor
{
public:
	virtual Color operator()( const Surface& tex,float itc_x,float itc_y ) const = 0;
	virtual ~TexFittingFunctor() = default;
};

class TexClamp : public TexFittingFunctor
{
public:
	Color operator()( const Surface& tex,float itc_x,float itc_y ) const override
	{
		const float tex_width = (float)tex.GetWidth();
		const float tex_height = (float)tex.GetHeight();
		const float tex_clamp_x = tex_width - 1.0f;
		const float tex_clamp_y = tex_height - 1.0f;

		return tex.GetPixel(
			(unsigned int)std::min( itc_x * tex_width,tex_clamp_x ),
			(unsigned int)std::min( itc_y * tex_height,tex_clamp_y )
		);
	}
};

class TexWrap : public TexFittingFunctor
{
public:
	Color operator()( const Surface& tex,float itc_x,float itc_y ) const override
	{
		const float tex_width = (float)tex.GetWidth();
		const float tex_height = (float)tex.GetHeight();
		float x_modded = fmodf( itc_x * tex_width,tex_width );
		float y_modded = fmodf( itc_y * tex_height,tex_height );

		if ( x_modded < 0.0f )
		{
			x_modded += tex_width;
		}
		if ( y_modded < 0.0f )
		{
			y_modded += tex_height;
		}

		const float tex_clamp_x = tex_width - 1.0f;
		const float tex_clamp_y = tex_height - 1.0f;

		return tex.GetPixel(
			(unsigned int)std::min( x_modded,tex_clamp_x ),
			(unsigned int)std::min( y_modded,tex_clamp_y )
		);
	}
};

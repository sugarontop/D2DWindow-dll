#pragma once

struct IBridgeTSFInterface
{
	IBridgeTSFInterface(){}

	enum TYP {SINGLELINE,MULTILINE,PASSWORD};

	virtual V6::FRectF GetClientRect() const = 0;
	virtual IDWriteTextFormat* GetFormat() const = 0;
	virtual TYP GetType() const = 0;
};


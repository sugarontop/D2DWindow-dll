#pragma once

struct IBridgeTSFInterface
{
	IBridgeTSFInterface(){}

	enum TYP {SINGLELINE=0x1,MULTILINE=0x2,PASSWORD=0x5};

	virtual V6::FRectF GetClientRect() const = 0;
	virtual IDWriteTextFormat* GetFormat() const = 0;
	virtual TYP GetType() const = 0;
};


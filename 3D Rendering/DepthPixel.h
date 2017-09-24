#pragma once

class DepthPixel
{
public:
	DepthPixel(): depth(-1), r(0), g(0), b(0) {}
	DepthPixel(float d, float _r, float _g, float _b) : r(_r), b(_b), g(_g), depth(d) {}
	~DepthPixel() {}

	float r, g, b;
	float depth;

	void changePixel(float d, float _r, float _g, float _b)
	{
		depth = d;
		r = _r;
		b = _b;
		g = _g;
	}
	inline bool willChange(float d)
	{
		return ((depth < 0) || (d < depth));
	}
	void clear()
	{
		depth = -1;
		r = g = b = 0;
	}
};
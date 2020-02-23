#ifndef RENDER_IS_INCLUDED
#define RENDER_IS_INCLUDED
/* { */

#include <vector>

#include "physmem.h"
#include "crtc.h"

/*! Make up an RGBA image for showing.
*/
class TownsRender
{
private:
	unsigned int wid,hei;
	std::vector <unsigned char> rgba;

public:
	class Image
	{
	public:
		unsigned int wid,hei;
		const unsigned char *rgba;
	};

	TownsRender();

	/*! Create a bitmap image.
	*/
	void Create(int wid,int hei);

	/*! 
	*/
	void BuildImage(const TownsCRTC &crtc,const TownsPhysicalMemory &physMem);
private:
	void SetResolution(int wid,int hei);

public:
	/*! Returns an image.
	*/
	Image GetImage(void) const;

	void Render(unsigned int page,const TownsCRTC::Layer &layer,const TownsCRTC::AnalogPalette &palette,const std::vector <unsigned char> &VRAM,bool transparent);
	void Render4Bit(const TownsCRTC::Layer &layer,const Vec3ub palette[16],const std::vector <unsigned char> &VRAM,bool transparent);
};


/* } */
#endif

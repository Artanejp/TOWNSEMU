/* LICENSE>>
Copyright 2020 Soji Yamakawa (CaptainYS, http://www.ysflight.com)

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

<< LICENSE */
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
	void Render8Bit(const TownsCRTC::Layer &layer,const Vec3ub palette[256],const std::vector <unsigned char> &VRAM,bool transparent);
	void Render16Bit(const TownsCRTC::Layer &layer,const std::vector <unsigned char> &VRAM,bool transparent);
};


/* } */
#endif

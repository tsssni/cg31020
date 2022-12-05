#include "pch.h"
#include "PixelShader.h"

vector<PixelOut>& PixelShader::PixelsProcess(const vector<VertexOut>& pixels, PassConstants& pc, MeshConstants& mc)
{
	mPixels.clear();

	for (auto& pixel : pixels)
	{
		if (pixel.posH.z < 0.0 || pixel.posH.z > 1.0)
		{
			continue;
		}

		int texPos = int(pixel.texC.x * pc.textureWidth) + int(pixel.texC.y * pc.textureHeight) * pc.textureWidth;
		
  		COLORREF texColor = pc.texture[texPos];
		PixelOut pout = { pixel.posH,{(double)GetRValue(texColor),(double)GetGValue(texColor),(double)GetBValue(texColor)} };
		
		mPixels.push_back(pout);
	}

	return mPixels;
}

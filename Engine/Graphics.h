/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once
#include "ChiliWin.h"
#include <d3d11.h>
#include <wrl.h>
#include "ChiliException.h"
#include "SpriteEffects.h"
#include "PolyLine.h"
#include "Colors.h"
#include "Sprite.h"
#include "Rect.h"
#include "CRect.h"
#include <functional>
#include <vector>

class Graphics
{
public:
	class Exception : public ChiliException
	{
	public:
		Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line );
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};
private:
	// vertex format for the framebuffer fullscreen textured quad
	struct FSQVertex
	{
		float x,y,z;		// position
		float u,v;			// texcoords
	};
public:
	Graphics( class HWNDKey& key );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	void EndFrame();
	void BeginFrame();
	Color GetPixel(int x, int y);
	void PutPixel( int x,int y,int r,int g,int b )
	{
		PutPixel( x,y,{ unsigned char( r ),unsigned char( g ),unsigned char( b ) } );
	}
	void PutPixel( int x,int y,Color c );
	void DrawLine(fVector2D p0, fVector2D p1, Color c);
	void DrawPolyLine(std::vector<fVector2D> verticies, Color c);
	void DrawSprite(const iCRect& Clip, int X, int Y, const Sprite& Sprite, iCRect Sect, std::function<void(Graphics&,int,int,Color,Color,Color)>FX = SpriteEffect::Null, Color ckey = Colors::Magenta, Color aux = Colors::White);
	void DrawSprite(int X, int Y, const Sprite& Sprite, std::function<void(Graphics&, int, int, Color, Color, Color)>FX = SpriteEffect::Null, Color ckey = Colors::Magenta, Color aux = Colors::White);
	void DrawSprite(const iCRect& Clip, int X, int Y, const Sprite& Sprite, std::function<void(Graphics&, int, int, Color, Color, Color)>FX = SpriteEffect::Null, Color ckey = Colors::Magenta, Color aux = Colors::White);
	void DrawSprite(int X, int Y, const Sprite& Sprite, const iCRect& Sect, std::function<void(Graphics&, int, int, Color, Color, Color)>FX = SpriteEffect::Null, Color ckey = Colors::Magenta, Color aux = Colors::White);
	~Graphics();
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;
	D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;
	Color*                                              pSysBuffer = nullptr;
public:
	static constexpr int ScreenWidth = 960u;
	static constexpr int ScreenHeight = 540u;
	static iCRect ScreenRect();
	static iRect ScreenRectFromCenter();
};
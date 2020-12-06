/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.cpp																		  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
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
#include "MainWindow.h"
#include "Graphics.h"
#include "DXErr.h"
#include "ChiliException.h"
#include "SpriteEffects.h"
#include "Math.h"
#include <assert.h>
#include <string>
#include <array>

// Ignore the intellisense error "cannot open source file" for .shh files.
// They will be created during the build sequence before the preprocessor runs.
namespace FramebufferShaders
{
#include "FramebufferPS.shh"
#include "FramebufferVS.shh"
}

#pragma comment( lib,"d3d11.lib" )

#define CHILI_GFX_EXCEPTION( hr,note ) Graphics::Exception( hr,note,_CRT_WIDE(__FILE__),__LINE__ )

using Microsoft::WRL::ComPtr;

Graphics::Graphics( HWNDKey& key )
{
	assert( key.hWnd != nullptr );

	//////////////////////////////////////////////////////
	// create device and swap chain/get render target view
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = Graphics::ScreenWidth;
	sd.BufferDesc.Height = Graphics::ScreenHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 1;
	sd.BufferDesc.RefreshRate.Denominator = 60;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = key.hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	HRESULT				hr;
	UINT				createFlags = 0u;
#ifdef CHILI_USE_D3D_DEBUG_LAYER
#ifdef _DEBUG
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#endif
	
	// create device and front/back buffers
	if( FAILED( hr = D3D11CreateDeviceAndSwapChain( 
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pImmediateContext ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating device and swap chain" );
	}

	// get handle to backbuffer
	ComPtr<ID3D11Resource> pBackBuffer;
	if( FAILED( hr = pSwapChain->GetBuffer(
		0,
		__uuidof( ID3D11Texture2D ),
		(LPVOID*)&pBackBuffer ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Getting back buffer" );
	}

	// create a view on backbuffer that we can render to
	if( FAILED( hr = pDevice->CreateRenderTargetView( 
		pBackBuffer.Get(),
		nullptr,
		&pRenderTargetView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating render target view on backbuffer" );
	}


	// set backbuffer as the render target using created view
	pImmediateContext->OMSetRenderTargets( 1,pRenderTargetView.GetAddressOf(),nullptr );


	// set viewport dimensions
	D3D11_VIEWPORT vp;
	vp.Width = float( Graphics::ScreenWidth );
	vp.Height = float( Graphics::ScreenHeight );
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pImmediateContext->RSSetViewports( 1,&vp );


	///////////////////////////////////////
	// create texture for cpu render target
	D3D11_TEXTURE2D_DESC sysTexDesc;
	sysTexDesc.Width = Graphics::ScreenWidth;
	sysTexDesc.Height = Graphics::ScreenHeight;
	sysTexDesc.MipLevels = 1;
	sysTexDesc.ArraySize = 1;
	sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sysTexDesc.SampleDesc.Count = 1;
	sysTexDesc.SampleDesc.Quality = 0;
	sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sysTexDesc.MiscFlags = 0;
	// create the texture
	if( FAILED( hr = pDevice->CreateTexture2D( &sysTexDesc,nullptr,&pSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating sysbuffer texture" );
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = sysTexDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// create the resource view on the texture
	if( FAILED( hr = pDevice->CreateShaderResourceView( pSysBufferTexture.Get(),
		&srvDesc,&pSysBufferTextureView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating view on sysBuffer texture" );
	}


	////////////////////////////////////////////////
	// create pixel shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreatePixelShader(
		FramebufferShaders::FramebufferPSBytecode,
		sizeof( FramebufferShaders::FramebufferPSBytecode ),
		nullptr,
		&pPixelShader ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating pixel shader" );
	}
	

	/////////////////////////////////////////////////
	// create vertex shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreateVertexShader(
		FramebufferShaders::FramebufferVSBytecode,
		sizeof( FramebufferShaders::FramebufferVSBytecode ),
		nullptr,
		&pVertexShader ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating vertex shader" );
	}
	

	//////////////////////////////////////////////////////////////
	// create and fill vertex buffer with quad for rendering frame
	const FSQVertex vertices[] =
	{
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,1.0f,0.5f,1.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,-1.0f,0.5f,0.0f,1.0f },
	};
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( FSQVertex ) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;
	if( FAILED( hr = pDevice->CreateBuffer( &bd,&initData,&pVertexBuffer ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating vertex buffer" );
	}

	
	//////////////////////////////////////////
	// create input layout for fullscreen quad
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};

	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreateInputLayout( ied,2,
		FramebufferShaders::FramebufferVSBytecode,
		sizeof( FramebufferShaders::FramebufferVSBytecode ),
		&pInputLayout ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating input layout" );
	}


	////////////////////////////////////////////////////
	// Create sampler state for fullscreen textured quad
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	if( FAILED( hr = pDevice->CreateSamplerState( &sampDesc,&pSamplerState ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating sampler state" );
	}

	// allocate memory for sysbuffer (16-byte aligned for faster access)
	pSysBuffer = reinterpret_cast<Color*>( 
		_aligned_malloc( sizeof( Color ) * Graphics::ScreenWidth * Graphics::ScreenHeight,16u ) );
}

Graphics::~Graphics()
{
	// free sysbuffer memory (aligned free)
	if( pSysBuffer )
	{
		_aligned_free( pSysBuffer );
		pSysBuffer = nullptr;
	}
	// clear the state of the device context before destruction
	if( pImmediateContext ) pImmediateContext->ClearState();
}

iCRect Graphics::ScreenRect()
{
	return iCRect(0,0,ScreenWidth,ScreenHeight);
}

iRect Graphics::ScreenRectFromCenter()
{
	return Rect<int>::MakeFromCenter({ 0,0 }, ScreenWidth / 2, ScreenHeight / 2);
}

void Graphics::EndFrame()
{
	HRESULT hr;

	// lock and map the adapter memory for copying over the sysbuffer
	if( FAILED( hr = pImmediateContext->Map( pSysBufferTexture.Get(),0u,
		D3D11_MAP_WRITE_DISCARD,0u,&mappedSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Mapping sysbuffer" );
	}
	// setup parameters for copy operation
	Color* pDst = reinterpret_cast<Color*>(mappedSysBufferTexture.pData );
	const size_t dstPitch = mappedSysBufferTexture.RowPitch / sizeof( Color );
	const size_t srcPitch = Graphics::ScreenWidth;
	const size_t rowBytes = srcPitch * sizeof( Color );
	// perform the copy line-by-line
	for( size_t y = 0u; y < Graphics::ScreenHeight; y++ )
	{
		memcpy( &pDst[ y * dstPitch ],&pSysBuffer[y * srcPitch],rowBytes );
	}
	// release the adapter memory
	pImmediateContext->Unmap( pSysBufferTexture.Get(),0u );

	// render offscreen scene texture to back buffer
	pImmediateContext->IASetInputLayout( pInputLayout.Get() );
	pImmediateContext->VSSetShader( pVertexShader.Get(),nullptr,0u );
	pImmediateContext->PSSetShader( pPixelShader.Get(),nullptr,0u );
	pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	const UINT stride = sizeof( FSQVertex );
	const UINT offset = 0u;
	pImmediateContext->IASetVertexBuffers( 0u,1u,pVertexBuffer.GetAddressOf(),&stride,&offset );
	pImmediateContext->PSSetShaderResources( 0u,1u,pSysBufferTextureView.GetAddressOf() );
	pImmediateContext->PSSetSamplers( 0u,1u,pSamplerState.GetAddressOf() );
	pImmediateContext->Draw( 6u,0u );

	// flip back/front buffers
	if( FAILED( hr = pSwapChain->Present( 1u,0u ) ) )
	{
		if( hr == DXGI_ERROR_DEVICE_REMOVED )
		{
			throw CHILI_GFX_EXCEPTION( pDevice->GetDeviceRemovedReason(),L"Presenting back buffer [device removed]" );
		}
		else
		{
			throw CHILI_GFX_EXCEPTION( hr,L"Presenting back buffer" );
		}
	}
}

void Graphics::BeginFrame()
{
	// clear the sysbuffer
	memset( pSysBuffer,0u,sizeof( Color ) * Graphics::ScreenHeight * Graphics::ScreenWidth );
}

Color Graphics::GetPixel(int x, int y)
{
	assert(x >= 0);
	assert(x < int(Graphics::ScreenWidth));
	assert(y >= 0);
	assert(y < int(Graphics::ScreenHeight));
	return pSysBuffer[Graphics::ScreenWidth * y + x];
}

void Graphics::PutPixel( int x,int y,Color c )
{
	assert( x >= 0 );
	assert( x < int( Graphics::ScreenWidth ) );
	assert( y >= 0 );
	assert( y < int( Graphics::ScreenHeight ) );
	pSysBuffer[Graphics::ScreenWidth * y + x] = c;
}

void Graphics::DrawLine(fVector2D p0, fVector2D p1, Color c)
{
	const float rise = p1.Y - p0.Y;
	const float run = p1.X - p0.X;
	if (std::abs(rise) < std::abs(run))
	{
		if (p0.X > p1.X) {
			std::swap(p0, p1);
		}
		const float m = rise / run;
		const float b = p0.Y - m * p0.X;
		for (int x = (int)p0.X; x <= (int)p1.X; ++x) {
			const float y = m * x + b;
			if (ScreenRect().ContainsPoint(iVector2D( (int)x,(int)y ))) {
				PutPixel(x, (int)y, c);
			}
		}
	}
	else
	{
		if (p0.Y > p1.Y) {
			std::swap(p0, p1);
		}
		const float w = run / rise;
		const float p = p0.X - w * p0.Y;
		for (int y = (int)p0.Y; y <= (int)p1.Y; ++y) {
			const float x = w * y + p;
			if (ScreenRect().ContainsPoint(iVector2D( (int)x,(int)y ))) {
				PutPixel((int)x, y, c);
			}
		}
	}
}

void Graphics::DrawPolyLine(std::vector<fVector2D> verticies, Color c)
{
	for (auto i = verticies.begin(); i < verticies.end() - 1; ++i) {
		DrawLine(*i, *(i + 1), c);
	}
	DrawLine(*(verticies.end() - 1), *verticies.begin(), c);
}

void Graphics::DrawSprite(const iCRect& Clip, int X, int Y, const Sprite& Sprite, iCRect Sect, std::function<void(Graphics&, int, int, Color, Color, Color)>FX, Color ckey, Color aux)
{
	if (X < Clip.Left) {
		Sect.Left += Clip.Left - X;
		X = int(Clip.Left);
	}
	if (Y < Clip.Left) {
		Sect.Top += Clip.Top - Y;
		Y = int(Clip.Top);
	}
	if (X + Sect.GetWidth() > Clip.Right) {
		Sect.Right -= X + Sect.GetWidth() - Clip.Right;
	}
	if (Y + Sect.GetHeight() > Clip.Bottom) {
		Sect.Bottom -= Y + Sect.GetHeight() - Clip.Bottom;
	}
	for (int y = Sect.Top; y < Sect.Bottom; ++y) {
		for (int x = Sect.Left; x < Sect.Right; ++x) {
			Color pxl2draw = Sprite.GetPixel(x, y);
			const int xDest = X + x - Sect.Left;
			const int yDest = Y + y - Sect.Top;
			FX(*this, xDest, yDest, pxl2draw, ckey, aux);
		}
	}
}
void Graphics::DrawSprite(int X, int Y, const Sprite& Sprite, std::function<void(Graphics&, int, int, Color, Color, Color)>FX, Color ckey, Color aux)
{
	DrawSprite(ScreenRect(), X, Y, Sprite, Sprite.getRect(), FX, ckey, aux);
}
void Graphics::DrawSprite(const iCRect& Clip, int X, int Y, const Sprite& Sprite, std::function<void(Graphics&, int, int, Color, Color, Color)>FX, Color ckey, Color aux)
{
	DrawSprite(Clip, X, Y, Sprite, Sprite.getRect(), FX, ckey, aux);
}
void Graphics::DrawSprite(int X, int Y, const Sprite& Sprite, const iCRect& Sect, std::function<void(Graphics&, int, int, Color, Color, Color)>FX, Color ckey, Color aux)
{
	DrawSprite(ScreenRect(), X, Y, Sprite, Sect, FX, ckey, aux);
}

void Graphics::DrawTriangle(const fVector2D& p0, const fVector2D& p1, const fVector2D& p2, Color c)
{
	const fVector2D* pV0 = &p0;
	const fVector2D* pV1 = &p1;
	const fVector2D* pV2 = &p2;

	// Sort by y
	if (pV1->Y < pV0->Y) { std::swap(pV1, pV0); }
	if (pV2->Y < pV1->Y) { std::swap(pV2, pV1); }
	if (pV1->Y < pV0->Y) { std::swap(pV1, pV0); }

	if (pV1->Y == pV0->Y) { // Natural Flat-Top
		if (pV1->X < pV0->X) { std::swap(pV1, pV0); } // Sort x
		DrawFlatTopTriangle(*pV0, *pV1, *pV2, c);
	}
	else if (pV2->Y == pV1->Y) { // Natural Flat-Bottom
		if (pV2->X < pV1->X) { std::swap(pV2, pV1); } // Sort x
		DrawFlatBottomTriangle(*pV0, *pV1, *pV2, c);
	}
	else {
		// Find Splitting Vertex
		const fVector2D s_vtx = pV0->InterpolatedTo(*pV1, *pV2);

		if (s_vtx.X < pV1->X) { // Major Left
			DrawFlatBottomTriangle(*pV0, s_vtx, *pV1, c);
			DrawFlatTopTriangle(s_vtx, *pV1, *pV2, c);
		}
		else { // Major Right
			DrawFlatBottomTriangle(*pV0, *pV1, s_vtx, c);
			DrawFlatTopTriangle(*pV1, s_vtx, *pV2, c);
		}
	}
}

void Graphics::DrawTexturedTriangle(const fTextureVector& p0, const fTextureVector& p1, const fTextureVector& p2, const Sprite& texture)
{
	const fTextureVector* pV0 = &p0;
	const fTextureVector* pV1 = &p1;
	const fTextureVector* pV2 = &p2;

	// Sort by y
	if (pV1->pos.Y < pV0->pos.Y) { std::swap(pV1, pV0); }
	if (pV2->pos.Y < pV1->pos.Y) { std::swap(pV2, pV1); }
	if (pV1->pos.Y < pV0->pos.Y) { std::swap(pV1, pV0); }

	if (pV1->pos.Y == pV0->pos.Y) { // Natural Flat-Top
		if (pV1->pos.X < pV0->pos.X) { std::swap(pV1, pV0); } // Sort x
		DrawFlatTopTexturedTriangle(*pV0, *pV1, *pV2, texture);
	}
	else if (pV2->pos.Y == pV1->pos.Y) { // Natural Flat-Bottom
		if (pV2->pos.X < pV1->pos.X) { std::swap(pV2, pV1); } // Sort x
		DrawFlatBottomTexturedTriangle(*pV0, *pV1, *pV2, texture);
	}
	else {
		// Find Splitting Vertex
		const float alpha = (pV1->pos.Y - pV0->pos.Y) / (pV2->pos.Y - pV0->pos.Y);
		const fTextureVector s_vtx = pV0->InterpolatedTo(*pV2, alpha);

		if (s_vtx.pos.X < pV1->pos.X) { // Major Left
			DrawFlatBottomTexturedTriangle(*pV0, s_vtx, *pV1, texture);
			DrawFlatTopTexturedTriangle(s_vtx, *pV1, *pV2, texture);
		}
		else { // Major Right
			DrawFlatBottomTexturedTriangle(*pV0, *pV1, s_vtx, texture);
			DrawFlatTopTexturedTriangle(*pV1, s_vtx, *pV2, texture);
		}
	}
}

void Graphics::DrawFlatTopTriangle(const fVector2D& p0, const fVector2D& p1, const fVector2D& p2, Color c)
{
	const float w0 = (p2.X - p0.X) / (p2.Y - p0.Y);
	const float w1 = (p2.X - p1.X) / (p2.Y - p1.Y);
	const int yStart = (int)ceil(p0.Y - 0.5f);
	const int yEnd = (int)ceil(p2.Y - 0.5f);
	for (int y = yStart; y < yEnd; ++y) {
		const float x0 = w0 * (float(y) + 0.5f - p0.Y) + p0.X;
		const float x1 = w1 * (float(y) + 0.5f - p1.Y) + p1.X;
		const int xStart = (int)ceil(x0 - 0.5f);
		const int xEnd = (int)ceil(x1 - 0.5f);
		for (int x = xStart; x < xEnd; ++x) {
			PutPixel(x, y, c);
		}
	}
}

void Graphics::DrawFlatBottomTriangle(const fVector2D& p0, const fVector2D& p1, const fVector2D& p2, Color c)
{
	const float w0 = (p0.X - p1.X) / (p0.Y - p1.Y);
	const float w1 = (p0.X - p2.X) / (p0.Y - p2.Y);
	const int yStart = (int)ceil(p0.Y - 0.5f);
	const int yEnd = (int)ceil(p2.Y - 0.5f);
	for (int y = yStart; y < yEnd; ++y) {
		const float x0 = w0 * (float(y) + 0.5f - p0.Y) + p0.X;
		const float x1 = w1 * (float(y) + 0.5f - p0.Y) + p0.X;
		const int xStart = (int)ceil(x0 - 0.5f);
		const int xEnd = (int)ceil(x1 - 0.5f);
		for (int x = xStart; x < xEnd; ++x) {
			if (ScreenRect().ContainsPoint(iVector2D(x, y))) {
				PutPixel(x, y, c);
			}
		}
	}
}

void Graphics::DrawFlatTopTexturedTriangle(const fTextureVector& p0, const fTextureVector& p1, const fTextureVector& p2, const Sprite& texture)
{
	const float delta_y = p2.pos.Y - p0.pos.Y;
	const fTextureVector dv0 = (p2 - p0) / delta_y;
	const fTextureVector dv1 = (p2 - p1) / delta_y;
	fTextureVector edge1 = p1;
	DrawFlatTexturedTriangle(p0, p1, p2, texture, dv0, dv1, edge1);
}

void Graphics::DrawFlatBottomTexturedTriangle(const fTextureVector& p0, const fTextureVector& p1, const fTextureVector& p2, const Sprite& texture)
{
	const float delta_y = p2.pos.Y - p0.pos.Y;
	const fTextureVector dv0 = (p1 - p0) / delta_y;
	const fTextureVector dv1 = (p2 - p0) / delta_y;
	fTextureVector edge1 = p0;
	DrawFlatTexturedTriangle(p0, p1, p2, texture, dv0, dv1, edge1);
}

void Graphics::DrawFlatTexturedTriangle(const fTextureVector& p0, const fTextureVector& p1, const fTextureVector& p2, const Sprite& texture, const fTextureVector& dv0, const fTextureVector& dv1, fTextureVector edge1)
{
	fTextureVector edge0 = p0;
	const int yStart = (int)ceil(p0.pos.Y - 0.5f);
	const int yEnd = (int)ceil(p2.pos.Y - 0.5f);
	edge0 += dv0 * (float(yStart) + 0.5f - p0.pos.Y);
	edge1 += dv1 * (float(yStart) + 0.5f - p0.pos.Y);
	const int tWidth = texture.getWidth();
	const int tHeight = texture.getHeight();
	for (int y = yStart; y < yEnd; ++y, edge0 += dv0, edge1 += dv1) {
		const int xStart = (int)ceil(edge0.pos.X - 0.5f);
		const int xEnd = (int)ceil(edge1.pos.X - 0.5f);
		fVector2D dvl = (edge1.tpos - edge0.tpos) / (edge1.pos.X - edge0.pos.X);
		fVector2D tpos = edge0.tpos + dvl * (float(xStart) + 0.5f - edge0.pos.X);
		for (int x = xStart; x < xEnd; ++x, tpos += dvl) {
			if (ScreenRect().ContainsPoint(iVector2D{ x,y })) {
				PutPixel(x, y, texture.GetPixel(
					int(tpos.X * tWidth) % tWidth, int(tpos.Y * tHeight) % tHeight
				));
			}
		}
	}
}

//////////////////////////////////////////////////
//           Graphics Exception
Graphics::Exception::Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line )
	:
	ChiliException( file,line,note ),
	hr( hr )
{}

std::wstring Graphics::Exception::GetFullMessage() const
{
	const std::wstring empty = L"";
	const std::wstring errorName = GetErrorName();
	const std::wstring errorDesc = GetErrorDescription();
	const std::wstring& note = GetNote();
	const std::wstring location = GetLocation();
	return    (!errorName.empty() ? std::wstring( L"Error: " ) + errorName + L"\n"
		: empty)
		+ (!errorDesc.empty() ? std::wstring( L"Description: " ) + errorDesc + L"\n"
			: empty)
		+ (!note.empty() ? std::wstring( L"Note: " ) + note + L"\n"
			: empty)
		+ (!location.empty() ? std::wstring( L"Location: " ) + location
			: empty);
}

std::wstring Graphics::Exception::GetErrorName() const
{
	return DXGetErrorString( hr );
}

std::wstring Graphics::Exception::GetErrorDescription() const
{
	std::array<wchar_t,512> wideDescription;
	DXGetErrorDescription( hr,wideDescription.data(),wideDescription.size() );
	return wideDescription.data();
}

std::wstring Graphics::Exception::GetExceptionType() const
{
	return L"Chili Graphics Exception";
}
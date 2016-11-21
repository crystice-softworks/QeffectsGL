/***************************************************************************
* Copyright (C) 2012-2016, Crystice Softworks.
* 
* This file is part of QeffectsGL source code.
* 
* QeffectsGL source code is free software; you can redistribute it 
* and/or modify it under the terms of the GNU General Public License 
* as published by the Free Software Foundation; either version 2 of 
* the License, or (at your option) any later version.
* 
* QeffectsGL source code is distributed in the hope that it will be 
* useful, but WITHOUT ANY WARRANTY; without even the implied 
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
* See the GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software 
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
***************************************************************************/
#include "qfx_library.hpp"
#include "qfx_log.hpp"
#include "qfx_settings.hpp"
#include "qfx_opengl.hpp"
#include "qfx_renderer.hpp"
#include "qfx_shader.hpp"
#include "qfx_glprogs.hpp"

//=========================================
// QFXRenderer class implementation
//-----------------------------------------
// This is QeffectsGL's core
//=========================================
using namespace gl;

QFXRenderer :: QFXRenderer()
{
	m_hDC = 0;
	m_hGLRC = 0;
	m_iRenderFrame = 0;
	m_uiFrameCounter = 0;
	m_uiRenderFrameCounter = 0;
	m_uiViewportFrame = 0;
	m_zNear = 1.0f;
	m_zFar = 1000.0f;
	m_bOrtho = GL_FALSE;
	m_bZTrick = GL_FALSE;
	m_bInternalAnisotropy = GL_FALSE;
	m_bInternalMultisample = GL_FALSE;
	m_bMultisample = GL_FALSE;
	m_uiBlurTexture = 0;
	m_uiBlurDepth = 0;
	m_uiScreenRGB = 0;
	m_uiScreenDepth = 0;
	m_pszExtensions = nullptr;
	m_pBloomDarkenShader = nullptr;
	m_pBloomBlurShader = nullptr;
	m_pBloomCombineShader = nullptr;
	m_pSSAORenderShader = nullptr;
	m_pSSAOBlurShader = nullptr;
	m_pSSAOCombineShader = nullptr;
	m_pHSLShader = nullptr;
	m_pEmbossShader = nullptr;
	m_pDOFBlurShader = nullptr;
	m_pDOFDepthShader = nullptr;
	m_pDOFRenderShader = nullptr;
	m_pVignetteShader = nullptr;
	m_iEnableRender = 1;
	m_flLastDepth = -1.0f;
	m_flLastTime = timeGetTime();
	m_flOldTime = timeGetTime();
	m_flFrameTime = 0.001f;
	m_iTextureInit = 0;

	m_varDebug = QFXSettings::Instance().GetInteger( "Debug", 0 );
	m_varExtensionLimit = QFXSettings::Instance().GetInteger( "ExtensionLimit", 0 );
	m_varCompareMode = QFXSettings::Instance().GetInteger( "CompareMode", 0 );
	m_varZTrickFix = QFXSettings::Instance().GetInteger( "ZTrickFix", 0 );
	m_varViewportFix = QFXSettings::Instance().GetInteger( "ViewportFix", 0 );
	m_varRenderFrame = QFXSettings::Instance().GetInteger( "RenderFrame", -1 );
	m_varAnisotropy = QFXSettings::Instance().GetInteger( "Anisotropy", 8 );
	m_varMultisample = QFXSettings::Instance().GetInteger( "Multisample", 4 );
	m_varMultisampleHint = QFXSettings::Instance().GetInteger( "MultisampleHint", 0 );
	m_varBloomEnable = QFXSettings::Instance().GetInteger( "Bloom", 1 );
	m_varBloomNumSteps = std::min( 16, QFXSettings::Instance().GetInteger( "BloomSteps", 8 ) );
	m_varBloomDarkenPower = QFXSettings::Instance().GetFloat( "BloomDarken", 3 );
	m_varBloomCombineScale = QFXSettings::Instance().GetFloat( "BloomScale", 1.5f );
	m_varSSAOEnable = QFXSettings::Instance().GetInteger( "SSAO", 1 );
	m_varSSAOScale = QFXSettings::Instance().GetFloat( "SSAOScale", 8.0f );
	m_varSSAOMinZ = QFXSettings::Instance().GetFloat( "SSAOMinZ", 0.005f );
	m_varSSAOMaxZ = QFXSettings::Instance().GetFloat( "SSAOMaxZ", 1.0f );
	m_varHue = QFXSettings::Instance().GetInteger( "Hue", 0 );
	m_varSaturation = QFXSettings::Instance().GetInteger( "Saturation", 0 );
	m_varLightness = QFXSettings::Instance().GetInteger( "Lightness", 0 );
	m_varEmboss = QFXSettings::Instance().GetInteger( "Emboss", 1 );
	m_varEmbossScale = QFXSettings::Instance().GetFloat( "EmbossScale", 0.5f );
	m_varDOF = QFXSettings::Instance().GetInteger( "DOF", 1 );
	m_varDOFSpeed = QFXSettings::Instance().GetFloat( "DOFSpeed", 1.0f );
	m_varDOFScale = QFXSettings::Instance().GetFloat( "DOFScale", 2.0f );
	m_varDOFMinZ = QFXSettings::Instance().GetFloat( "DOFMinZ", 0.005f );
	m_varVignette = QFXSettings::Instance().GetInteger( "Vignette", 1 );
	m_varVignetteScale = QFXSettings::Instance().GetFloat( "VignetteScale", 1.25f );
	m_varVignetteScale = std::min( 2.0f, std::max( 0.0f, m_varVignetteScale ) );
}

QFXRenderer :: ~QFXRenderer()
{
	if ( m_varExtensionLimit > 0 )
		delete [] m_pszExtensions;
}

const char *QFXRenderer :: GetExtensionString()
{
	if ( !m_pszExtensions ) {
		if ( m_varExtensionLimit <= 0 ) {
			m_pszExtensions = reinterpret_cast<char*>( const_cast<GLubyte*>( gl::qglGetString( GL_EXTENSIONS ) ) );
		} else {
			m_pszExtensions = new char[m_varExtensionLimit]();
			const char *originalExtensions = reinterpret_cast<const char*>( gl::qglGetString( GL_EXTENSIONS ) );
			if ( originalExtensions && *originalExtensions ) {
				bool swapControl = ( m_varExtensionLimit > 21 ) && !!strstr( originalExtensions, "WGL_EXT_swap_control" );
				const int currentLimit = swapControl ? ( m_varExtensionLimit - 21 ) : m_varExtensionLimit;
				const int originalLength = static_cast<int>( strlen( originalExtensions ) );
				int addedLength = 0;
				for ( int i = 0, start = 0; i <= originalLength; ++i ) {
					if ( !originalExtensions[i] || originalExtensions[i] == ' ' ) {
						const int extLength = i - start;
						if ( extLength ) {
							if ( addedLength + extLength + 1 >= currentLimit )
								break;
							strncat_s( m_pszExtensions, m_varExtensionLimit, &originalExtensions[start], extLength );
							if ( originalExtensions[i] ) strcat_s( m_pszExtensions, m_varExtensionLimit, " " );
							addedLength += extLength + 1;
						}
						start = i + 1;
					}
				}
				if ( swapControl )
					strcat_s( m_pszExtensions, m_varExtensionLimit, " WGL_EXT_swap_control" );
			}
		}
	}
	return m_pszExtensions;
}

int QFXRenderer :: ChooseMultisamplePixelFormat( int fmt, HDC, const PIXELFORMATDESCRIPTOR *ppfd )
{
	static bool classRegistered = false;
	GLint sampleBuffers = 0;

	// register temp window class
	if ( !classRegistered ) {
		WNDCLASS wc;
		memset( &wc, 0, sizeof( wc ) );
		wc.lpfnWndProc   = DefWindowProc;
		wc.hInstance     = GetModuleHandle( nullptr );
		wc.lpszClassName = QFX_LIBRARY_TITLE;

		if ( !RegisterClass( &wc ) ) {
			QFXLog::Instance().Error( "ChooseMultisamplePixelFormat: RegisterClass failed\n" );
			return -1;
		}
		
		classRegistered = true;
	}

	// create temporary hidden window
	HWND tempHWND = CreateWindowEx( 0, QFX_LIBRARY_TITLE, "", WS_POPUP, 0, 0, 1, 1, nullptr, nullptr, GetModuleHandle( nullptr ), nullptr );
	if ( !tempHWND ) {
		QFXLog::Instance().Error( "ChooseMultisamplePixelFormat: CreateWindowEx failed\n" );
		return -1;
	}

	HDC tempDC = GetDC( tempHWND );
	if ( !tempDC ) {
		QFXLog::Instance().Error( "ChooseMultisamplePixelFormat: GetDC failed\n" );
		DestroyWindow( tempHWND );
		return -1;
	}

	if( !gl::wglSetPixelFormat( tempDC, fmt, ppfd ) ) {
		QFXLog::Instance().Error( "ChooseMultisamplePixelFormat: SetPixelFormat failed\n" );
		ReleaseDC( tempHWND, tempDC );
		DestroyWindow( tempHWND );
		return -1;
    }

	HGLRC tempRC = gl::wglCreateContext( tempDC );
	if ( !tempRC ) {
		QFXLog::Instance().Error( "ChooseMultisamplePixelFormat: wglCreateContext failed\n" );
		ReleaseDC( tempHWND, tempDC );
		DestroyWindow( tempHWND );
		return -1;
	}

	if ( !gl::wglMakeCurrent( tempDC, tempRC ) ) {
		gl::wglDeleteContext( tempRC );
		QFXLog::Instance().Error( "ChooseMultisamplePixelFormat: wglMakeCurrent failed\n" );
		ReleaseDC( tempHWND, tempDC );
		DestroyWindow( tempHWND );
		return -1;
	}

	qglGetIntegerv( GL_SAMPLE_BUFFERS_ARB, &sampleBuffers );
	if ( sampleBuffers )
		m_bInternalMultisample = GL_TRUE;

	PFNWGLCHOOSEPIXELFORMATARBPROC qwglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)gl::wglGetProcAddress( "wglChoosePixelFormatARB" );
	if ( !qwglChoosePixelFormatARB ) {
		gl::wglMakeCurrent( tempDC, nullptr );
		gl::wglDeleteContext( tempRC );
		QFXLog::Instance().Warning( "missing extension WGL_ARB_pixel_format\n" );
		QFXLog::Instance().Warning( "multisample is not supported\n" );
		ReleaseDC( tempHWND, tempDC );
		DestroyWindow( tempHWND );
		return -1;
	}

	int pixelFormat( 0 );
	UINT numFormats;
	float fAttributes[] = { 0, 0 };

	while ( m_varMultisample >= 2 ) {
		int iAttributes[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB, ppfd->cColorBits,
			WGL_ALPHA_BITS_ARB, ppfd->cAlphaBits,
			WGL_DEPTH_BITS_ARB, ( ppfd->cDepthBits > 16 ) ? 24 : 16,
			WGL_STENCIL_BITS_ARB, ppfd->cStencilBits,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
			WGL_SAMPLES_ARB, m_varMultisample,
			0,0};

		if ( qwglChoosePixelFormatARB( tempDC, iAttributes, fAttributes, 1, &pixelFormat, &numFormats ) && numFormats )
			break;
		
		QFXLog::Instance().Warning( "%ix FSAA is not supported\n", m_varMultisample );

		if (m_varMultisample > 8)
			m_varMultisample >>= 1;
		else
			m_varMultisample -= 2;
	}

	if ( m_varMultisample >= 2 ) {
		QFXLog::Instance().Printf( "Using %ix FSAA\n", m_varMultisample );
	} else {
		QFXLog::Instance().Warning( "FSAA is not supported by hardware\n" );
		pixelFormat = -1;
	}

	gl::wglMakeCurrent( tempDC, nullptr );
	gl::wglDeleteContext( tempRC );
	ReleaseDC( tempHWND, tempDC );
	DestroyWindow( tempHWND );
	return pixelFormat;
}

BOOL QFXRenderer :: OnSetPixelFormat( HDC hdc, int pixelformat, const PIXELFORMATDESCRIPTOR *ppfd )
{
	PIXELFORMATDESCRIPTOR pfd;
	if ( ppfd )
		memcpy( &pfd, ppfd, sizeof(PIXELFORMATDESCRIPTOR) );
	else
		gl::wglDescribePixelFormat( hdc, pixelformat, sizeof(PIXELFORMATDESCRIPTOR), &pfd );

	if ( ( ppfd->cColorBits > 0 ) && ( ppfd->dwFlags & PFD_DOUBLEBUFFER ) && ( m_varMultisample > 1 ) ) {
		int multisamplePixelFormat = ChooseMultisamplePixelFormat( pixelformat, hdc, &pfd );
		if ( multisamplePixelFormat >= 0 ) {
			BOOL b = gl::wglSetPixelFormat( hdc, multisamplePixelFormat, &pfd );
			if ( b ) {
				m_bMultisample = GL_TRUE;
				return b;
			}
		}
	}

	return gl::wglSetPixelFormat( hdc, pixelformat, ppfd );
}

void QFXRenderer :: OnMakeCurrent( HDC hdc, HGLRC hglrc )
{
	if ( !hdc || !hglrc )
		return;

	if ( hdc != m_hDC || hglrc != m_hGLRC ) {
		m_hDC = hdc;
		m_hGLRC = hglrc;
		gl::InitializeExtensions();
		InitializeGL();

		if ( m_varMultisample ) {
			qglEnable( GL_MULTISAMPLE_ARB );
			if ( m_varMultisampleHint )
				qglHint( GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST );
		}
	}
}

void QFXRenderer :: OnDeleteContext( HGLRC hglrc )
{
	if ( !hglrc )
		return;

	if ( hglrc == m_hGLRC )
		ShutdownGL();
}

void QFXRenderer :: OnSwapBuffers()
{
	if ( -1 == m_varRenderFrame )
		RenderGL();

	++m_uiFrameCounter;
}

void QFXRenderer :: SetupAnisotropy()
{
	m_bInternalAnisotropy = GL_TRUE;
}

void QFXRenderer :: OnTexImage2D()
{
	if ( ext.maxAnisotropy > 1 && m_varAnisotropy > 1 ) {
		int value = std::min( ext.maxAnisotropy, m_varAnisotropy );
		qglTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, value );
	}
}

void QFXRenderer :: CheckHardware()
{
	QFXLog::Instance().Printf( "Checking hardware caps:\n" );

	m_bSupportsBloom = m_varBloomEnable ? (( ( ext.maxActiveTextures >= 2 ) && ext.VertexShader && ext.FragmentShader && ext.RectangleTexture ) ? GL_TRUE : GL_FALSE ) : GL_FALSE;
	m_bSupportsSSAO = m_varSSAOEnable ? (( ( ext.maxActiveTextures >= 2 ) && ext.VertexShader && ext.FragmentShader && ext.RectangleTexture && ext.DepthTexture ) ? GL_TRUE : GL_FALSE ) : GL_FALSE;
	m_bSupportsColorCorrection = (m_varHue || m_varSaturation || m_varLightness) ? (( ext.VertexShader && ext.FragmentShader && ext.RectangleTexture ) ? GL_TRUE : GL_FALSE ) : GL_FALSE;
	m_bSupportsEmboss = m_varEmboss ? (( ext.VertexShader && ext.FragmentShader && ext.RectangleTexture ) ? GL_TRUE : GL_FALSE ) : GL_FALSE;
	m_bSupportsDOF = m_varDOF ? (( ( ext.maxActiveTextures >= 3 ) && ext.VertexShader && ext.FragmentShader && ext.RectangleTexture && ext.DepthTexture ) ? GL_TRUE : GL_FALSE ) : GL_FALSE;
	m_bSupportsVignette = m_varVignette ? (( ext.VertexShader && ext.FragmentShader && ext.RectangleTexture ) ? GL_TRUE : GL_FALSE ) : GL_FALSE;
	
	QFXLog::Instance().Printf( "Bloom effect: %s\n", m_varBloomEnable ? (m_bSupportsBloom ? "YES" : "NO") : "DISABLED" );
	QFXLog::Instance().Printf( "SSAO effect: %s\n", m_varSSAOEnable ? (m_bSupportsSSAO ? "YES" : "NO") : "DISABLED" );
	QFXLog::Instance().Printf( "Color correction: %s\n", (m_varHue || m_varSaturation || m_varLightness) ? (m_bSupportsColorCorrection ? "YES" : "NO") : "DISABLED" );
	QFXLog::Instance().Printf( "Emboss: %s\n", m_varEmboss ? (m_bSupportsEmboss ? "YES" : "NO") : "DISABLED" );
	QFXLog::Instance().Printf( "DOF effect: %s\n", m_varDOF ? (m_bSupportsDOF ? "YES" : "NO") : "DISABLED" );
	QFXLog::Instance().Printf( "Vignette: %s\n", m_varVignette ? (m_bSupportsVignette ? "YES" : "NO") : "DISABLED" );
}

void QFXRenderer :: InitializeGL()
{
	m_iTextureInit = 0;
	m_uiTextureIndex = QFX_TEXTURE_IDENTIFIER_BASE;
	
	QFXLog::Instance().Printf( "InitializeGL\n" );

	CheckHardware();

	// load GLSL shaders
	if ( m_bSupportsBloom ) {
		m_pBloomDarkenShader = new QFXShader( g_szVertexShader_BloomDarken, g_szFragmentShader_BloomDarken );
		m_pBloomBlurShader = new QFXShader( g_szVertexShader_BloomBlur, g_szFragmentShader_BloomBlur );
		m_pBloomCombineShader = new QFXShader( g_szVertexShader_BloomCombine, g_szFragmentShader_BloomCombine );
		if ( !m_pBloomDarkenShader || !m_pBloomDarkenShader->IsValid() ) m_bSupportsBloom = GL_FALSE;
		if ( !m_pBloomBlurShader || !m_pBloomBlurShader->IsValid() ) m_bSupportsBloom = GL_FALSE;
		if ( !m_pBloomCombineShader || !m_pBloomCombineShader->IsValid() ) m_bSupportsBloom = GL_FALSE;
	}

	if ( m_bSupportsSSAO ) {
		m_pSSAORenderShader = new QFXShader( g_szVertexShader_SSAORender, g_szFragmentShader_SSAORender );
		m_pSSAOBlurShader = new QFXShader( g_szVertexShader_SSAOBlur, g_szFragmentShader_SSAOBlur );
		m_pSSAOCombineShader = new QFXShader( g_szVertexShader_SSAOCombine, g_szFragmentShader_SSAOCombine );
		if ( !m_pSSAORenderShader || !m_pSSAORenderShader->IsValid() ) m_bSupportsSSAO = GL_FALSE;
		if ( !m_pSSAOBlurShader || !m_pSSAOBlurShader->IsValid() ) m_bSupportsSSAO = GL_FALSE;
		if ( !m_pSSAOCombineShader || !m_pSSAOCombineShader->IsValid() ) m_bSupportsSSAO = GL_FALSE;
	}

	if ( m_bSupportsColorCorrection ) {
		m_pHSLShader = new QFXShader( g_szVertexShader_HSL, g_szFragmentShader_HSL );
		if ( !m_pHSLShader || !m_pHSLShader->IsValid() ) m_bSupportsColorCorrection = GL_FALSE;
	}

	if ( m_bSupportsEmboss ) {
		m_pEmbossShader = new QFXShader( g_szVertexShader_Emboss, g_szFragmentShader_Emboss );
		if ( !m_pEmbossShader || !m_pEmbossShader->IsValid() ) m_bSupportsEmboss = GL_FALSE;
	}

	if ( m_bSupportsDOF ) {
		m_pDOFBlurShader = new QFXShader( g_szVertexShader_DOFBlur, g_szFragmentShader_DOFBlur );
		m_pDOFDepthShader = new QFXShader( g_szVertexShader_DOFDepth, g_szFragmentShader_DOFDepth );
		m_pDOFRenderShader = new QFXShader( g_szVertexShader_DOFRender, g_szFragmentShader_DOFRender );
		if ( !m_pDOFBlurShader || !m_pDOFBlurShader->IsValid() ) m_bSupportsDOF = GL_FALSE;
		if ( !m_pDOFDepthShader || !m_pDOFDepthShader->IsValid() ) m_bSupportsDOF = GL_FALSE;
		if ( !m_pDOFRenderShader || !m_pDOFRenderShader->IsValid() ) m_bSupportsDOF = GL_FALSE;
	}

	if ( m_bSupportsVignette ) {
		m_pVignetteShader = new QFXShader( g_szVertexShader_Vignette, g_szFragmentShader_Vignette );
		if ( !m_pVignetteShader || !m_pVignetteShader->IsValid() ) m_bSupportsVignette = GL_FALSE;
	}
}

void QFXRenderer :: InitializeTextures()
{
	if ( m_iTextureInit )
		return;
	m_iTextureInit = 1;

	QFXLog::Instance().Printf( "InitializeTextures\n" );

	qglGetIntegerv( GL_VIEWPORT, m_3DViewport );

	HWND hCurrentWnd = WindowFromDC( gl::wglGetCurrentDC() );
	RECT rc;
	GetClientRect( hCurrentWnd, &rc );
	m_WindowSize[0] = rc.right - rc.left;
	m_WindowSize[1] = rc.bottom - rc.top;

	// create textures
	if ( ext.RectangleTexture ) {
		GLboolean stateRectangleEnabled;
		GLuint stateRectangleBinding = 0;

		stateRectangleEnabled = qglIsEnabled( GL_TEXTURE_RECTANGLE_ARB );
		qglGetIntegerv( GL_TEXTURE_BINDING_RECTANGLE_ARB, (GLint*)(&stateRectangleBinding) );
		qglEnable( GL_TEXTURE_RECTANGLE_ARB );

		if ( m_bSupportsBloom || m_bSupportsSSAO || m_bSupportsDOF ) {
			m_uiBlurTexture = AllocTextureId();
			qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_uiBlurTexture );
			qglTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, ext.edgeClampMode );
			qglTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, ext.edgeClampMode );
			qglTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			qglTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			qglTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 3, m_3DViewport[2]>>1, m_3DViewport[3]>>1, 0, GL_RGB, GL_UNSIGNED_BYTE, 0 );
		}
		if ( m_bSupportsDOF ) {
			m_uiBlurDepth = AllocTextureId();
			qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_uiBlurDepth );
			qglTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, ext.edgeClampMode );
			qglTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, ext.edgeClampMode );
			qglTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			qglTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			qglTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 3, m_3DViewport[2]>>1, m_3DViewport[3]>>1, 0, GL_RGB, GL_UNSIGNED_BYTE, 0 );
		}

		m_uiScreenRGB = AllocTextureId();
		qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_uiScreenRGB );
		qglTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, ext.edgeClampMode );
		qglTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, ext.edgeClampMode );
		qglTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		qglTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		qglTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 3, m_3DViewport[2], m_3DViewport[3], 0, GL_RGB, GL_UNSIGNED_BYTE, 0 );
	
		if ( m_bSupportsSSAO || m_bSupportsDOF ) {
			m_uiScreenDepth = AllocTextureId();
			qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_uiScreenDepth );
			qglTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_DEPTH_TEXTURE_MODE_ARB, GL_LUMINANCE );
			qglTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, ext.edgeClampMode );
			qglTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, ext.edgeClampMode );
			qglTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			qglTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			qglTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, GL_DEPTH_COMPONENT24, m_3DViewport[2], m_3DViewport[3], 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0 );
		}

		qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, stateRectangleBinding );
		if ( !stateRectangleEnabled ) qglDisable( GL_TEXTURE_RECTANGLE_ARB );
	}

	if ( !m_uiScreenRGB ) {
		m_bSupportsEmboss = GL_FALSE;
		m_bSupportsColorCorrection = GL_FALSE;
		m_bSupportsSSAO = GL_FALSE;
		m_bSupportsDOF = GL_FALSE;
		m_bSupportsBloom = GL_FALSE;
		m_bSupportsVignette = GL_FALSE;
	}
	if ( !m_uiBlurTexture ) {
		m_bSupportsBloom = GL_FALSE;
		m_bSupportsSSAO = GL_FALSE;
		m_bSupportsDOF = GL_FALSE;
	}
	if ( !m_uiScreenDepth ) {
		m_bSupportsSSAO = GL_FALSE;
		m_bSupportsDOF = GL_FALSE;
	}
	if ( !m_uiBlurDepth ) {
		m_bSupportsDOF = GL_FALSE;
	}
}

void QFXRenderer :: ShutdownGL()
{
	QFXLog::Instance().Printf( "ShutdownGL\n" );

	if ( m_pBloomDarkenShader ) {
		delete m_pBloomDarkenShader;
		m_pBloomDarkenShader = nullptr;
	}
	if ( m_pBloomBlurShader ) {
		delete m_pBloomBlurShader;
		m_pBloomBlurShader = nullptr;
	}
	if ( m_pBloomCombineShader ) {
		delete m_pBloomCombineShader;
		m_pBloomCombineShader = nullptr;
	}
	if ( m_pSSAORenderShader ) {
		delete m_pSSAORenderShader;
		m_pSSAORenderShader = nullptr;
	}
	if ( m_pSSAOBlurShader ) {
		delete m_pSSAOBlurShader;
		m_pSSAOBlurShader = nullptr;
	}
	if ( m_pSSAOCombineShader ) {
		delete m_pSSAOCombineShader;
		m_pSSAOCombineShader = nullptr;
	}
	if ( m_pHSLShader ) {
		delete m_pHSLShader;
		m_pHSLShader = nullptr;
	}
	if ( m_pEmbossShader ) {
		delete m_pEmbossShader;
		m_pEmbossShader = nullptr;
	}
	if ( m_pDOFBlurShader ) {
		delete m_pDOFBlurShader;
		m_pDOFBlurShader = nullptr;
	}
	if ( m_pDOFDepthShader ) {
		delete m_pDOFDepthShader;
		m_pDOFDepthShader = nullptr;
	}
	if ( m_pDOFRenderShader ) {
		delete m_pDOFRenderShader;
		m_pDOFRenderShader = nullptr;
	}
	if ( m_pVignetteShader ) {
		delete m_pVignetteShader;
		m_pVignetteShader = nullptr;
	}

	if ( m_uiBlurTexture ) {
		qglDeleteTextures( 1, &m_uiBlurTexture );
		m_uiBlurTexture = 0;
	}
	if ( m_uiBlurDepth ) {
		qglDeleteTextures( 1, &m_uiBlurDepth );
		m_uiBlurDepth = 0;
	}
	if ( m_uiScreenRGB ) {
		qglDeleteTextures( 1, &m_uiScreenRGB );
		m_uiScreenRGB = 0;
	}
	if ( m_uiScreenDepth ) {
		qglDeleteTextures( 1, &m_uiScreenDepth );
		m_uiScreenDepth = 0;
	}
}

void QFXRenderer :: SetupZDimensions( GLfloat znear, GLfloat zfar )
{
	m_zNear = znear;
	m_zFar = zfar;

	if ( m_varDebug )
		QFXLog::Instance().Printf("%5i: SetupZDimensions zn = %f, zf = %f\n", m_uiFrameCounter, m_zNear, m_zFar );
}

void QFXRenderer :: SetupZDimensions( const GLfloat* matrix )
{
	GLfloat fC = matrix[2*4+2];
	GLfloat fD = matrix[3*4+2];
	GLfloat fQ = (1.0f + fC) / (1.0f - fC);
	m_zFar = (fD * (1.0f + fQ)) / (2.0f * fQ);
	m_zNear = (fD * m_zFar) / (fD - 2.0f*m_zFar);
	if ( m_zFar < 0 ) m_zFar = -m_zFar;

	if ( m_varDebug )
		QFXLog::Instance().Printf("%5i: SetupZDimensions zn = %f, zf = %f\n", m_uiFrameCounter, m_zNear, m_zFar );
}

void QFXRenderer :: SetupZDimensions( const GLdouble* matrix )
{
	GLfloat fC = (GLfloat)matrix[2*4+2];
	GLfloat fD = (GLfloat)matrix[3*4+2];
	GLfloat fQ = (1.0f + fC) / (1.0f - fC);
	m_zFar = (fD * (1.0f + fQ)) / (2.0f * fQ);
	m_zNear = (fD * m_zFar) / (fD - 2.0f*m_zFar);
	if ( m_zFar < 0 ) m_zFar = -m_zFar;

	if ( m_varDebug )
		QFXLog::Instance().Printf("%5i: SetupZDimensions zn = %f, zf = %f\n", m_uiFrameCounter, m_zNear, m_zFar );
}

void QFXRenderer :: SetupDepthFunc( GLenum func )
{
	if ( m_varZTrickFix && ( func == GL_GEQUAL ) ) {
		m_bZTrick = true;
		if ( m_varDebug )
			QFXLog::Instance().Printf("%5i: ZTrick attempt detected\n", m_uiFrameCounter );
		qglDepthFunc( GL_LEQUAL );
	} else {
		qglDepthFunc( func );
	}

	if ( m_bZTrick )
		qglClear( GL_DEPTH_BUFFER_BIT );
}

void QFXRenderer :: SetupDepthRange( GLclampd zNear, GLclampd zFar )
{
	if ( m_bZTrick ) {
		if ( zNear > zFar ) {
			zNear = 1.0f - zNear;
			zFar = 1.0f - zFar;
		}
		if ( zFar > 0.45 ) zFar = 1.0;
		if ( zNear > 0.99 ) zNear = 0.0f;
	}
	qglDepthRange( zNear, zFar );
}

void QFXRenderer :: SetupMultisample()
{
	m_bInternalMultisample = GL_TRUE;
}

void QFXRenderer :: RenderGL()
{
	DWORD t = timeGetTime();
	m_flFrameTime = (t - m_flOldTime) * 0.001f;
	m_flOldTime = t;

	if ( m_varCompareMode ) {
		if ( t - m_flLastTime > 2000 ) {
			m_flLastTime = t;

			m_iEnableRender ^= 1;

			if ( (m_varAnisotropy > 1) && (ext.maxAnisotropy > 1) && !m_bInternalAnisotropy ) {
				int value = m_iEnableRender ? std::min( ext.maxAnisotropy, m_varAnisotropy ) : 1;
				GLuint state2DBinding;
				qglGetIntegerv( GL_TEXTURE_BINDING_2D, (GLint*)(&state2DBinding) );
				for ( GLuint i = 1; i < 9999; ++i ) {
					if ( qglIsTexture( i ) ) {
						qglBindTexture( GL_TEXTURE_2D, i );
						qglTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, value );
					}
				}
				qglBindTexture( GL_TEXTURE_2D, state2DBinding );
			}
			if ( m_bMultisample && !m_bInternalMultisample ) {
				if ( m_iEnableRender )
					qglEnable( GL_MULTISAMPLE_ARB );
				else
					qglDisable( GL_MULTISAMPLE_ARB );
			}
		}
	}
	if ( m_iEnableRender ) {
		if ( m_varDebug )
			QFXLog::Instance().Printf( "%5i: Postprocess at %i\n", m_uiFrameCounter, m_iRenderFrame );
		InitializeTextures();
		PostProcess();
	}
}

void QFXRenderer :: SetOrtho( GLboolean enable )
{
	if ( m_varDebug )
		QFXLog::Instance().Printf("%5i: ortho = %s\n", m_uiFrameCounter, enable ? "true" : "false" );

	if ( enable ) {
		if ( m_uiRenderFrameCounter != m_uiFrameCounter ) {
			m_uiRenderFrameCounter = m_uiFrameCounter;
			m_iRenderFrame = 0;
		} else {
			++m_iRenderFrame;
		}

		if ( m_varDebug )
			QFXLog::Instance().Printf("RenderFrame = %i\n", m_iRenderFrame );

		if ( m_iRenderFrame == m_varRenderFrame )
			RenderGL();
	} else {
		if ( m_uiViewportFrame != m_uiFrameCounter ) {
			m_uiViewportFrame = m_uiFrameCounter;
			qglGetIntegerv( GL_VIEWPORT, m_3DViewport );
		}
	}

	m_bOrtho = enable;
}

void QFXRenderer :: CaptureRGB()
{
	qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_uiScreenRGB );
	qglCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, 0, m_WindowSize[1] - m_3DViewport[3], m_3DViewport[2], m_3DViewport[3] );
}

void QFXRenderer :: CaptureDepth()
{
	qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_uiScreenDepth );
	qglCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, 0, m_WindowSize[1] - m_3DViewport[3], m_3DViewport[2], m_3DViewport[3] );
}

void QFXRenderer :: RenderFSQ( int wide, int tall )
{
	const float screenWidth = (float)wide;
	const float screenHeight = (float)tall;

	qglBegin( GL_QUADS );
		qglTexCoord2f( 0, screenHeight );
		qglVertex2f( 0, 0 );
		qglTexCoord2f( screenWidth, screenHeight );
		qglVertex2f( screenWidth, 0 );
		qglTexCoord2f( screenWidth, 0 );
		qglVertex2f( screenWidth, screenHeight );
		qglTexCoord2f( 0, 0 );
		qglVertex2f( 0, screenHeight );
	qglEnd();
}

void QFXRenderer :: RenderScaledFSQ( int wide, int tall, int wide2, int tall2 )
{
	const float screenWidth = (float)wide;
	const float screenHeight = (float)tall;
	const float screenWidth2 = (float)wide2;
	const float screenHeight2 = (float)tall2;

	qglBegin( GL_QUADS );
		qglTexCoord2f( 0, screenHeight2 );
		qglVertex2f( 0, 0 );
		qglTexCoord2f( screenWidth2, screenHeight2 );
		qglVertex2f( screenWidth, 0 );
		qglTexCoord2f( screenWidth2, 0 );
		qglVertex2f( screenWidth, screenHeight );
		qglTexCoord2f( 0, 0 );
		qglVertex2f( 0, screenHeight );
	qglEnd();
}

void QFXRenderer :: PostProcess()
{
	GLboolean stateRectangleEnabled( GL_FALSE );
	GLboolean stateBlendEnabled;
	GLboolean stateCullEnabled;
	GLboolean stateDepthTestEnabled;
	GLboolean stateScissorEnabled;
	GLuint stateRectangleBindingMTex[3] = { 0, 0, 0 };
	GLenum stateSrcBlendFunc = GL_ONE;
	GLenum stateDstBlendFunc = GL_ZERO;
	GLenum stateMatrixMode = GL_MODELVIEW;
	GLenum stateActiveTexture = GL_TEXTURE0_ARB;
	GLenum stateTexEnv = GL_MODULATE;
	GLboolean stateDepthMask = GL_TRUE;
	GLfloat stateCurrentColor[4];
	GLhandleARB currentGLSLShader = 0;
	GLint stateViewport[4];

	// save current state
	if ( ext.VertexShader || ext.FragmentShader ) {
		currentGLSLShader = qglGetHandleARB( GL_PROGRAM_OBJECT_ARB );
	}
	if ( ext.Multitexture ) {
		qglGetIntegerv( GL_ACTIVE_TEXTURE_ARB, (GLint*)(&stateActiveTexture) );
		if ( ext.RectangleTexture ) {
			qglActiveTextureARB( GL_TEXTURE1_ARB );
			qglGetIntegerv( GL_TEXTURE_BINDING_RECTANGLE_ARB, (GLint*)(&stateRectangleBindingMTex[1]) );
			qglActiveTextureARB( GL_TEXTURE2_ARB );
			qglGetIntegerv( GL_TEXTURE_BINDING_RECTANGLE_ARB, (GLint*)(&stateRectangleBindingMTex[2]) );
			qglActiveTextureARB( GL_TEXTURE0_ARB );
			qglGetIntegerv( GL_TEXTURE_BINDING_RECTANGLE_ARB, (GLint*)(&stateRectangleBindingMTex[0]) );
			stateRectangleEnabled = qglIsEnabled( GL_TEXTURE_RECTANGLE_ARB );
		} else {
			qglActiveTextureARB( GL_TEXTURE0_ARB );
		}
	} else if ( ext.RectangleTexture ) {
		stateRectangleEnabled = qglIsEnabled( GL_TEXTURE_RECTANGLE_ARB );
		qglGetIntegerv( GL_TEXTURE_BINDING_RECTANGLE_ARB, (GLint*)(&stateRectangleBindingMTex[0]) );
	}

	stateBlendEnabled = qglIsEnabled( GL_BLEND );
	stateCullEnabled = qglIsEnabled( GL_CULL_FACE );
	stateDepthTestEnabled = qglIsEnabled( GL_DEPTH_TEST );
	stateScissorEnabled = qglIsEnabled( GL_SCISSOR_TEST );
	qglGetIntegerv( GL_BLEND_SRC, (GLint*)(&stateSrcBlendFunc) );
	qglGetIntegerv( GL_BLEND_DST, (GLint*)(&stateDstBlendFunc) );
	qglGetIntegerv( GL_MATRIX_MODE, (GLint*)(&stateMatrixMode) );
	qglGetBooleanv( GL_DEPTH_WRITEMASK, &stateDepthMask );
	qglGetFloatv( GL_CURRENT_COLOR, stateCurrentColor );
	if ( !m_varViewportFix ) qglGetIntegerv( GL_VIEWPORT, stateViewport );
	qglGetIntegerv( GL_SCISSOR_BOX, m_3DScissor );
	qglGetTexEnviv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, (GLint*)(&stateTexEnv) );

	// setup new state
	qglDisable( GL_CULL_FACE );
	qglDisable( GL_DEPTH_TEST );
	qglDisable( GL_BLEND );
	qglEnable( GL_TEXTURE_RECTANGLE_ARB );
	qglEnable( GL_SCISSOR_TEST );
	qglDepthMask( GL_FALSE );
	qglMatrixMode( GL_PROJECTION );
	qglPushMatrix();
	qglLoadIdentity();
	qglOrtho( 0, m_WindowSize[0], m_WindowSize[1], 0, -1, 1 );
	qglMatrixMode( GL_MODELVIEW );
	qglPushMatrix();
	qglLoadIdentity();
	qglColor4f( 1, 1, 1, 1 );
	if ( !m_varViewportFix ) qglViewport( m_3DViewport[0], m_3DViewport[1], m_3DViewport[2], m_3DViewport[3] );
	qglScissor( m_3DViewport[0], m_3DViewport[1], m_3DViewport[2], m_3DViewport[3] );

	// render SSAO
	if ( m_bSupportsSSAO )
		PostProcess_SSAO();

	// render Emboss
	if ( m_bSupportsEmboss )
		PostProcess_Emboss();

	// render DOF
	if ( m_bSupportsDOF )
		PostProcess_DOF();

	// apply hue-saturation-lightness correction
	if ( m_bSupportsColorCorrection )
		PostProcess_ColorCorrection();

	// render bloom effect
	if ( m_bSupportsBloom )
		PostProcess_Bloom();

	// render vignette
	if ( m_bSupportsVignette )
		PostProcess_Vignette();

	// restore current state
	qglMatrixMode( GL_PROJECTION );
	qglPopMatrix();
	qglMatrixMode( GL_MODELVIEW );
	qglPopMatrix();
	if ( ext.Multitexture ) {
		if ( ext.RectangleTexture ) {
			qglActiveTextureARB( GL_TEXTURE1_ARB );
			qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, stateRectangleBindingMTex[1] );
			qglActiveTextureARB( GL_TEXTURE2_ARB );
			qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, stateRectangleBindingMTex[2] );
			qglActiveTextureARB( GL_TEXTURE0_ARB );
			qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, stateRectangleBindingMTex[0] );
			if ( !stateRectangleEnabled ) qglDisable( GL_TEXTURE_RECTANGLE_ARB );
		}
		qglActiveTextureARB( stateActiveTexture );
	} else if ( ext.RectangleTexture ) {
		qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, stateRectangleBindingMTex[0] );
		if ( !stateRectangleEnabled ) qglDisable( GL_TEXTURE_RECTANGLE_ARB );
	}
	qglBlendFunc( stateSrcBlendFunc, stateDstBlendFunc );
	qglColor4fv( stateCurrentColor );
	qglMatrixMode( stateMatrixMode );
	if ( stateBlendEnabled ) qglEnable( GL_BLEND ); else qglDisable( GL_BLEND );
	if ( stateCullEnabled ) qglEnable( GL_CULL_FACE );
	if ( stateDepthTestEnabled ) qglEnable( GL_DEPTH_TEST );
	if ( !stateScissorEnabled ) qglDisable( GL_SCISSOR_TEST );
	qglScissor( m_3DScissor[0], m_3DScissor[1], m_3DScissor[2], m_3DScissor[3] );
	if ( !m_varViewportFix ) qglViewport( stateViewport[0], stateViewport[1], stateViewport[2], stateViewport[3] );
	qglTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, stateTexEnv );
	if ( ext.VertexShader || ext.FragmentShader ) qglUseProgramObjectARB( currentGLSLShader );
	if ( stateDepthMask ) qglDepthMask( stateDepthMask );
}

void QFXRenderer :: PostProcess_SSAO()
{
	int screenWidth = m_3DViewport[2];
	int screenHeight = m_3DViewport[3];
	int blurTexWidth = m_3DViewport[2]>>1;
	int blurTexHeight = m_3DViewport[3]>>1;

	// capture screen
	CaptureRGB();

	// capture depth
	CaptureDepth();

	// render raw SSAO
	m_pSSAORenderShader->Bind();
	m_pSSAORenderShader->SetParameter4f( 0, 2.0f * m_zNear, m_zFar - m_zNear, (float)screenWidth, (float)screenHeight );
	m_pSSAORenderShader->SetParameter4f( 1, m_varSSAOMinZ, m_varSSAOMaxZ, m_varSSAOScale, 0 );
	RenderScaledFSQ( blurTexWidth, blurTexHeight, screenWidth, screenHeight );
	qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_uiBlurTexture );
	qglCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, 0, m_WindowSize[1] - blurTexHeight, blurTexWidth, blurTexHeight );

	// blur SSAO
	m_pSSAOBlurShader->Bind();
	m_pSSAOBlurShader->SetParameter4f( 0, 1, 0, 0, 0 );
	qglActiveTextureARB( GL_TEXTURE1_ARB );
	qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_uiScreenDepth );
	qglActiveTextureARB( GL_TEXTURE0_ARB );
	RenderFSQ( blurTexWidth, blurTexHeight );
	qglCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, 0, m_WindowSize[1] - blurTexHeight, blurTexWidth, blurTexHeight );
	m_pSSAOBlurShader->SetParameter4f( 0, 0, 1, 0, 0 );
	RenderFSQ( blurTexWidth, blurTexHeight );
	qglCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, 0, m_WindowSize[1] - blurTexHeight, blurTexWidth, blurTexHeight );
	
	// combine normal and AO scenes
	m_pSSAOCombineShader->Bind();
	qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_uiScreenRGB );
	qglActiveTextureARB( GL_TEXTURE1_ARB );
	qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_uiBlurTexture );
	qglActiveTextureARB( GL_TEXTURE0_ARB );
	RenderFSQ( screenWidth, screenHeight );

	// unbind shader
	m_pSSAOCombineShader->Unbind();
}

void QFXRenderer :: PostProcess_Bloom()
{
	int screenWidth = m_3DViewport[2];
	int screenHeight = m_3DViewport[3];
	int blurTexWidth = m_3DViewport[2]>>1;
	int blurTexHeight = m_3DViewport[3]>>1;

	// capture screen
	CaptureRGB();

	// darken screen
	m_pBloomDarkenShader->Bind();
	m_pBloomDarkenShader->SetParameter4f( 0, m_varBloomDarkenPower, 0, 0, 0 );
	RenderScaledFSQ( blurTexWidth, blurTexHeight, screenWidth, screenHeight );
	qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_uiBlurTexture );
	qglCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, 0, m_WindowSize[1] - blurTexHeight, blurTexWidth, blurTexHeight );

	// blur darkened texture
	m_pBloomBlurShader->Bind();
	for ( int i = 0; i < m_varBloomNumSteps; ++i ) {
		m_pBloomBlurShader->SetParameter4f( 0, 1, 0, 0, 0 );
		RenderFSQ( blurTexWidth, blurTexHeight );
		qglCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, 0, m_WindowSize[1] - blurTexHeight, blurTexWidth, blurTexHeight );

		m_pBloomBlurShader->SetParameter4f( 0, 0, 1, 0, 0 );
		RenderFSQ( blurTexWidth, blurTexHeight );
		qglCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, 0, m_WindowSize[1] - blurTexHeight, blurTexWidth, blurTexHeight );
	}

	// combine normal and blurred scenes
	m_pBloomCombineShader->Bind();
	m_pBloomCombineShader->SetParameter4f( 0, m_varBloomCombineScale, 0, 0, 0 );
	qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_uiScreenRGB );
	qglActiveTextureARB( GL_TEXTURE1_ARB );
	qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_uiBlurTexture );
	qglActiveTextureARB( GL_TEXTURE0_ARB );
	RenderFSQ( screenWidth, screenHeight );

	// unbind shader
	m_pBloomCombineShader->Unbind();
}

void QFXRenderer :: PostProcess_ColorCorrection()
{
	int screenWidth = m_3DViewport[2];
	int screenHeight = m_3DViewport[3];

	// capture screen
	CaptureRGB();

	// perform correction
	m_pHSLShader->Bind();
	m_pHSLShader->SetParameter4f( 0, m_varHue / 255.0f, m_varSaturation / 255.0f, m_varLightness / 255.0f, 0 );
	RenderFSQ( screenWidth, screenHeight );

	// unbind shader
	m_pHSLShader->Unbind();
}

void QFXRenderer :: PostProcess_Emboss()
{
	const int screenWidth = m_3DViewport[2];
	const int screenHeight = m_3DViewport[3];

	// capture screen
	CaptureRGB();

	// perform correction
	m_pEmbossShader->Bind();
	m_pEmbossShader->SetParameter4f( 0, m_varEmbossScale, 0, 0, 0 );
	RenderFSQ( screenWidth, screenHeight );

	// unbind shader
	m_pEmbossShader->Unbind();
}

void QFXRenderer :: PostProcess_DOF()
{
	int screenWidth = m_3DViewport[2];
	int screenHeight = m_3DViewport[3];
	int blurTexWidth = m_3DViewport[2]>>1;
	int blurTexHeight = m_3DViewport[3]>>1;
	float depthValue = 0.0f;

	// get current depth value
	qglReadPixels( blurTexWidth, blurTexHeight, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depthValue );
	depthValue = (2.0f * m_zNear) / (m_zFar + m_zNear - depthValue * ( m_zFar - m_zNear ));
	if ( m_flLastDepth < 0 ) {
		m_flLastDepth = depthValue;
	} else if ( m_flLastDepth > depthValue ) {
		m_flLastDepth -= std::min( m_flLastDepth - depthValue, m_varDOFSpeed * m_flFrameTime );
	} else if ( m_flLastDepth < depthValue ) {
		m_flLastDepth += std::min( depthValue - m_flLastDepth, m_varDOFSpeed * m_flFrameTime );
	}

	//QFXLog::Instance().Printf( "depthValue = %f (ft = %f)\n", depthValue, m_flFrameTime );

	// capture screen
	CaptureRGB();

	// blur screen
	m_pDOFBlurShader->Bind();
	m_pDOFBlurShader->SetParameter4f( 0, 1, 0, 0, 0 );
	RenderScaledFSQ( blurTexWidth, blurTexHeight, screenWidth, screenHeight );
	qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_uiBlurTexture );
	qglCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, 0, m_WindowSize[1] - blurTexHeight, blurTexWidth, blurTexHeight );
	m_pDOFBlurShader->SetParameter4f( 0, 0, 1, 0, 0 );
	RenderFSQ( blurTexWidth, blurTexHeight );
	qglCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, 0, m_WindowSize[1] - blurTexHeight, blurTexWidth, blurTexHeight );

	// blur screen more
	for ( int i = 0; i < 3; ++i ) {
		m_pDOFBlurShader->SetParameter4f( 0, 1, 0, 0, 0 );
		RenderFSQ( blurTexWidth, blurTexHeight );
		qglCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, 0, m_WindowSize[1] - blurTexHeight, blurTexWidth, blurTexHeight );
		m_pDOFBlurShader->SetParameter4f( 0, 0, 1, 0, 0 );
		RenderFSQ( blurTexWidth, blurTexHeight );
		qglCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, 0, m_WindowSize[1] - blurTexHeight, blurTexWidth, blurTexHeight );
	}

	// capture depth, if not captured previously in SSAO
	if ( m_bSupportsSSAO )
		qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_uiScreenDepth );
	else
		CaptureDepth();

	// linearize depth
	m_pDOFDepthShader->Bind();
	m_pDOFDepthShader->SetParameter4f( 0, 2.0f * m_zNear, m_zFar - m_zNear, 0, 0 );
	RenderScaledFSQ( blurTexWidth, blurTexHeight, screenWidth, screenHeight );
	qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_uiBlurDepth );
	qglCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, 0, m_WindowSize[1] - blurTexHeight, blurTexWidth, blurTexHeight );

	// blur depth
	m_pDOFBlurShader->Bind();
	for ( int i = 0; i < 4; ++i ) {
		m_pDOFBlurShader->SetParameter4f( 0, 1, 0, 0, 0 );
		RenderFSQ( blurTexWidth, blurTexHeight );
		qglCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, 0, m_WindowSize[1] - blurTexHeight, blurTexWidth, blurTexHeight );
		m_pDOFBlurShader->SetParameter4f( 0, 0, 1, 0, 0 );
		RenderFSQ( blurTexWidth, blurTexHeight );
		qglCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, 0, m_WindowSize[1] - blurTexHeight, blurTexWidth, blurTexHeight );
	}

	// combine normal and blurred scenes
	m_pDOFRenderShader->Bind();
	m_pDOFRenderShader->SetParameter4f( 0, m_varDOFMinZ, m_flLastDepth, m_varDOFScale, 0 );
	qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_uiScreenRGB );
	qglActiveTextureARB( GL_TEXTURE1_ARB );
	qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_uiBlurTexture );
	qglActiveTextureARB( GL_TEXTURE2_ARB );
	qglBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_uiBlurDepth );
	qglActiveTextureARB( GL_TEXTURE0_ARB );
	RenderFSQ( screenWidth, screenHeight );

	// unbind shader
	m_pDOFRenderShader->Unbind();
}

void QFXRenderer :: PostProcess_Vignette()
{
	const int screenWidth = m_3DViewport[2];
	const int screenHeight = m_3DViewport[3];

	// capture screen
	CaptureRGB();

	// perform correction
	m_pVignetteShader->Bind();
	m_pVignetteShader->SetParameter4f( 0, 1.0f / screenWidth, 1.0f / screenHeight, m_varVignetteScale, 0 );
	RenderFSQ( screenWidth, screenHeight );

	// unbind shader
	m_pVignetteShader->Unbind();
}

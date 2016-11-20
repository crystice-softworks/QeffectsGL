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
#ifndef QFX_GLPROGS_H
#define QFX_GLPROGS_H

static const char g_szVertexShader_DOFDepth[] = 
"#version 120\n"
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0;\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_DOFDepth[] = 
"#version 120\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform vec4 Local0;\n"
"uniform sampler2DRect Texture0;\n"
"float read_depth( vec2 coord )\n"
"{\n"
"return Local0.x / (Local0.y + Local0.x - texture2DRect( Texture0, coord ).r * Local0.y);\n"
"}\n"
"void main(void)\n"
"{\n"
"gl_FragColor.rgb = read_depth( gl_TexCoord[0].xy );\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";

static const char g_szVertexShader_DOFBlur[] = 
"#version 120\n"
"uniform vec4 Local0;\n"
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0 + Local0;\n"
"gl_TexCoord[1] = gl_MultiTexCoord0 + Local0 * 2.0;\n"
"gl_TexCoord[2] = gl_MultiTexCoord0 - Local0;\n"
"gl_TexCoord[3] = gl_MultiTexCoord0 - Local0 * 2.0;\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_DOFBlur[] = 
"#version 120\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform sampler2DRect Texture0;\n"
"void main(void)\n"
"{\n"
"vec3 col1 = texture2DRect(Texture0, gl_TexCoord[0].xy).rgb;\n"
"vec3 col2 = texture2DRect(Texture0, gl_TexCoord[1].xy).rgb;\n"
"vec3 col3 = texture2DRect(Texture0, gl_TexCoord[2].xy).rgb;\n"
"vec3 col4 = texture2DRect(Texture0, gl_TexCoord[3].xy).rgb;\n"
"gl_FragColor.rgb = (col1 + col2 + col3 + col4) / 4.0;\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";

static const char g_szVertexShader_DOFRender[] = 
"#version 120\n"
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0.xyxy * vec4( 1.0, 1.0, 0.5, 0.5 );\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_DOFRender[] = 
"#version 120\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform vec4 Local0;\n"
"uniform sampler2DRect Texture0;\n"
"uniform sampler2DRect Texture1;\n"
"uniform sampler2DRect Texture2;\n"
"void main(void)\n"
"{\n"
"float ratio;\n"
"vec3 colNormal = texture2DRect(Texture0, gl_TexCoord[0].xy).rgb;\n"
"vec3 colBlur = texture2DRect(Texture1, gl_TexCoord[0].zw).rgb;\n"
"float depth = texture2DRect(Texture2, gl_TexCoord[0].zw).r;\n"
"if ( depth < Local0.x || depth == 1.0 ) ratio = 0.0;\n"
"else ratio = clamp( abs(depth - Local0.y) * Local0.z, 0.0, 1.0 );\n"
"gl_FragColor.rgb = mix( colNormal, colBlur, ratio * pow( depth, 0.25 ) );\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";

static const char g_szVertexShader_Emboss[] = 
"#version 120\n"
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0;\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_Emboss[] = 
"#version 120\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform sampler2DRect Texture0;\n"
"uniform vec4 Local0;\n"
"void main(void)\n"
"{\n"
"vec3 col1 = texture2DRect(Texture0, gl_TexCoord[0].xy - vec2( 1.0, 1.0 )).rgb;\n"
"vec3 col2 = texture2DRect(Texture0, gl_TexCoord[0].xy).rgb;\n"
"vec3 col3 = texture2DRect(Texture0, gl_TexCoord[0].xy + vec2( 1.0, 1.0 )).rgb;\n"
"vec3 colEmboss = col1 * 2.0 - col2 - col3;\n"
"float colDot = max( 0.0, dot( colEmboss, vec3( 0.333, 0.333, 0.333 ) ) ) * Local0.x;\n"
"vec3 colFinal = col2 - vec3( colDot ) ;\n"
"float luminance = dot( col2, vec3( 0.6, 0.2, 0.2 ) );\n"
"gl_FragColor.rgb = mix( colFinal, col2, luminance * luminance );\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";

static const char g_szVertexShader_HSL[] = 
"#version 120\n"
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0;\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

// This nice HSL color correction shader code was taken from:
// http://mouaif.wordpress.com/2009/01/05/photoshop-math-with-glsl-shaders/
// It produces the same result as Photoshop

static const char g_szFragmentShader_HSL[] = 
"#version 120\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform sampler2DRect Texture0;\n"
"uniform vec4 Local0;\n"
"vec3 rgb_to_hsl( const vec3 color )\n"
"{\n"
"vec3 hsl;\n"
"float fmin = min(min(color.r, color.g), color.b);\n"
"float fmax = max(max(color.r, color.g), color.b);\n"
"float delta = fmax - fmin;\n"
"hsl.z = (fmax + fmin) / 2.0;\n"
"if (delta == 0.0) {\n"
"hsl.x = 0.0;\n"
"hsl.y = 0.0;\n"
"} else {\n"
"if (hsl.z < 0.5) hsl.y = delta / (fmax + fmin);\n"
"else hsl.y = delta / (2.0 - fmax - fmin);\n"
"float deltaR = (((fmax - color.r) / 6.0) + (delta / 2.0)) / delta;\n"
"float deltaG = (((fmax - color.g) / 6.0) + (delta / 2.0)) / delta;\n"
"float deltaB = (((fmax - color.b) / 6.0) + (delta / 2.0)) / delta;\n"
"if (color.r == fmax ) hsl.x = deltaB - deltaG;\n"
"else if (color.g == fmax) hsl.x = (1.0 / 3.0) + deltaR - deltaB;\n"
"else if (color.b == fmax) hsl.x = (2.0 / 3.0) + deltaG - deltaR;\n"
"if (hsl.x < 0.0) hsl.x += 1.0;\n"
"else if (hsl.x > 1.0) hsl.x -= 1.0;\n"
"}\n"
"return hsl;\n"
"}\n"
"float hue_to_rgb( float f1, float f2, float hue )\n"
"{\n"
"if (hue < 0.0) hue += 1.0;\n"
"else if (hue > 1.0) hue -= 1.0;\n"
"float res;\n"
"if ((6.0 * hue) < 1.0) res = f1 + (f2 - f1) * 6.0 * hue;\n"
"else if ((2.0 * hue) < 1.0) res = f2;\n"
"else if ((3.0 * hue) < 2.0) res = f1 + (f2 - f1) * ((2.0 / 3.0) - hue) * 6.0;\n"
"else res = f1;\n"
"return res;\n"
"}\n"
"vec3 hsl_to_rgb( vec3 hsl )\n"
"{\n"
"vec3 rgb;\n"
"if (hsl.y == 0.0) rgb = vec3(hsl.z);\n"
"else {\n"
"float f2;\n"
"if (hsl.z < 0.5) f2 = hsl.z * (1.0 + hsl.y);\n"
"else f2 = (hsl.z + hsl.y) - (hsl.y * hsl.z);\n"
"float f1 = 2.0 * hsl.z - f2;\n"
"rgb.r = hue_to_rgb(f1, f2, hsl.x + (1.0/3.0));\n"
"rgb.g = hue_to_rgb(f1, f2, hsl.x);\n"
"rgb.b= hue_to_rgb(f1, f2, hsl.x - (1.0/3.0));\n"
"}\n"
"return rgb;\n"
"}\n"
"void main(void)\n"
"{\n"
"vec3 col = texture2DRect(Texture0, gl_TexCoord[0].xy).rgb;\n"
"vec3 hsl = rgb_to_hsl( col ) + Local0.xyz;\n"
"gl_FragColor.rgb = hsl_to_rgb( hsl );\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";

static const char g_szVertexShader_SSAORender[] = 
"#version 120\n"
"uniform vec4 Local0;\n"
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0.xyxy * vec4( 1.0, 1.0, 1.0 / Local0.z, 1.0 / Local0.w );\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_SSAORender[] = 
"#version 120\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform vec4 Local0;\n"
"uniform vec4 Local1;\n"
"uniform sampler2DRect Texture0;\n"
"float read_depth( vec2 coord )\n"
"{\n"
"return Local0.x / (Local0.y + Local0.x - texture2DRect( Texture0, coord ).r * Local0.y);\n"
"}\n"
"vec2 rand( vec2 coord )\n"
"{\n"
"float coordDot = dot( coord, vec2( 12.9898, 78.233 ) );\n"
"float noiseX = fract( sin( coordDot ) * 43758.5453 );\n"
"float noiseY = fract( sin( coordDot * 2.0 ) * 43758.5453 );\n"
"return vec2( noiseX, noiseY ) * 0.004;\n"
"}\n"
"float compare_depth( float depth1, float depth2 )\n"
"{\n"
"float aoRange = 60.0;\n"
"float depthDiff = depth1 - depth2;\n"
"float propagation = depthDiff * (Local0.y - Local0.x);\n"
"float rangeDiff = sqrt(clamp( 1.0 - propagation / aoRange, 0.0, 1.0 ));\n"
"float ao = Local1.z * depthDiff * (2.0 - depthDiff) * rangeDiff;\n"
"return ao;\n"
"}\n"
"void main(void)\n"
"{\n"
"float depth = read_depth( gl_TexCoord[0].xy );\n"
"float ao;\n"
"if ( depth > Local1.x && depth < Local1.y ) {\n"
"vec2 noise = rand( gl_TexCoord[0].zw );\n"
"float distScale = 1.0 + 5.0 * sqrt( 1.0 - depth );\n"
"float w = distScale + (noise.x*(1.0-noise.x)) * Local0.z;\n"
"float h = distScale + (noise.y*(1.0-noise.y)) * Local0.w;\n"
"vec2 ofs;\n"
"float d;\n"
"int samples = 3;\n"
"int rings = 3;\n"
"float TWO_PI = 2.0 * 3.14159265;\n"
"for ( int i = 1 ; i <= rings; i++ ) {\n"
"float angleStep = TWO_PI / float( samples * i );\n"
"for ( int j = 1 ; j <= samples*i; j++ ) {\n"
"ofs.x = cos( float(j) * angleStep ) * float(i) * w;\n"
"ofs.y = sin( float(j) * angleStep ) * float(i) * h;\n"
"d = read_depth( gl_TexCoord[0].xy + ofs );\n"
"// ignore occluder with too low depth value (possible viewmodel)\n"
"if ( d < Local1.x ) continue;\n"
"ao += compare_depth( depth, d );\n"
"}\n"
"}\n"
"ao = 1.0 - ao;\n"
"} else {\n"
"ao = 1.0;\n"
"}\n"
"gl_FragColor.rgb = vec3( ao );\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";

static const char g_szVertexShader_SSAOBlur[] = 
"#version 120\n"
"uniform vec4 Local0;\n"
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0 + Local0;\n"
"gl_TexCoord[1] = gl_MultiTexCoord0 + Local0 * 2.0;\n"
"gl_TexCoord[2] = gl_MultiTexCoord0 - Local0;\n"
"gl_TexCoord[3] = gl_MultiTexCoord0 - Local0 * 2.0;\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_SSAOBlur[] = 
"#version 120\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform sampler2DRect Texture0;\n"
"void main(void)\n"
"{\n"
"vec3 col1 = texture2DRect(Texture0, gl_TexCoord[0].xy).rgb;\n"
"vec3 col2 = texture2DRect(Texture0, gl_TexCoord[1].xy).rgb;\n"
"vec3 col3 = texture2DRect(Texture0, gl_TexCoord[2].xy).rgb;\n"
"vec3 col4 = texture2DRect(Texture0, gl_TexCoord[3].xy).rgb;\n"
"gl_FragColor.rgb = (col1 + col2 + col3 + col4) / 4.0;\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";

static const char g_szVertexShader_SSAOCombine[] = 
"#version 120\n"
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0;\n"
"gl_TexCoord[1] = gl_MultiTexCoord0 * 0.5;\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_SSAOCombine[] = 
"#version 120\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform sampler2DRect Texture0;\n"
"uniform sampler2DRect Texture1;\n"
"void main(void)\n"
"{\n"
"vec3 col1 = texture2DRect(Texture0, gl_TexCoord[0].xy).rgb;\n"
"vec3 col2 = texture2DRect(Texture1, gl_TexCoord[1].xy).rgb;\n"
"float luminance = dot( col1, vec3( 0.6, 0.2, 0.2 ) );\n"
"gl_FragColor.rgb = mix( col1 * col2, col1, luminance * luminance );\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";

static const char g_szVertexShader_BloomDarken[] = 
"#version 120\n"
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0;\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_BloomDarken[] = 
"#version 120\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform vec4 Local0;\n"
"uniform sampler2DRect Texture0;\n"
"void main(void)\n"
"{\n"
"vec3 col = texture2DRect(Texture0, gl_TexCoord[0].xy).rgb;\n"
"gl_FragColor.rgb = pow( col, Local0.xxx );\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";

static const char g_szVertexShader_BloomBlur[] = 
"#version 120\n"
"uniform vec4 Local0;\n"
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0 + Local0;\n"
"gl_TexCoord[1] = gl_MultiTexCoord0 + Local0 * 2.0;\n"
"gl_TexCoord[2] = gl_MultiTexCoord0 - Local0;\n"
"gl_TexCoord[3] = gl_MultiTexCoord0 - Local0 * 2.0;\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_BloomBlur[] = 
"#version 120\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform sampler2DRect Texture0;\n"
"void main(void)\n"
"{\n"
"vec3 col1 = texture2DRect(Texture0, gl_TexCoord[0].xy).rgb;\n"
"vec3 col2 = texture2DRect(Texture0, gl_TexCoord[1].xy).rgb;\n"
"vec3 col3 = texture2DRect(Texture0, gl_TexCoord[2].xy).rgb;\n"
"vec3 col4 = texture2DRect(Texture0, gl_TexCoord[3].xy).rgb;\n"
"gl_FragColor.rgb = (col1 + col2 + col3 + col4) / 4.0;\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";

static const char g_szVertexShader_BloomCombine[] = 
"#version 120\n"
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0;\n"
"gl_TexCoord[1] = gl_MultiTexCoord0 * 0.5;\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_BloomCombine[] = 
"#version 120\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform vec4 Local0;\n"
"uniform sampler2DRect Texture0;\n"
"uniform sampler2DRect Texture1;\n"
"void main(void)\n"
"{\n"
"vec3 col1 = texture2DRect(Texture0, gl_TexCoord[0].xy).rgb;\n"
"vec3 col2 = texture2DRect(Texture1, gl_TexCoord[1].xy).rgb;\n"
"gl_FragColor.rgb = col1 + col2 * Local0.x;\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";

static const char g_szVertexShader_Vignette[] = 
"#version 120\n"
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0;\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_Vignette[] = 
"#version 120\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform vec4 Local0;\n"
"uniform sampler2DRect Texture0;\n"
"void main(void)\n"
"{\n"
"vec3 col = texture2DRect(Texture0, gl_TexCoord[0].xy).rgb;\n"
"float dist = length(gl_TexCoord[0].xy * Local0.xy - vec2( 0.5 ));\n"
"dist *= dist * Local0.z;\n"
"gl_FragColor.rgb = col * ( 1.0 - dist );\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";

#endif //QFX_GLPROGS_H

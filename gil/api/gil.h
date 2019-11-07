/******************************************************************************
**
**   File:        gil.h
**   Description: LSR Graphics Library (C interface)
**
**   Copyright (C) 2017 Luxoft GmbH
**
**   This file is part of Luxoft Safe Renderer.
**
**   Luxoft Safe Renderer is free software: you can redistribute it and/or
**   modify it under the terms of the GNU Lesser General Public
**   License as published by the Free Software Foundation.
**
**   Safe Render is distributed in the hope that it will be useful,
**   but WITHOUT ANY WARRANTY; without even the implied warranty of
**   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
**   Lesser General Public License for more details.
**
**   You should have received a copy of the GNU Lesser General Public
**   License along with Safe Render.  If not, see
**   <http://www.gnu.org/licenses/>.
**
**   SPDX-License-Identifier: LGPL-3.0
**
******************************************************************************/


/**
 Some requirements
 - the API should support 2 windows / layers in 2 different threads/tasks (i.e. 2 contexts)
 - the API should support 2 windows / layers in 1 thread (i.e. 1 context)
 - the API should support rendering into an offscreen buffer (at least it should be extensible in this direction)
 - the API should support both ROM (directly rendered from ROM w/o RAM usage) and (V)RAM textures for the same instance (i.e. if the image buffer needs to be copied)
 - invalid return values equal to 0 / NULL
 Questions which need clarification
 - int32_t is 4 digits fractional part for u/v coordinates (assumption: to have better user experience when doing scaling animations) - Question: Does it make sense to also have
   positions as fixed point (e.g. translation animations or when using OpenGL). If system doesn't support implementation can easily get rid of fractional part!
 - visibility of window / layer: Idea leave it outside of first prototype. Suggestion: Layer gets visible in first swapBuffer call.
 - Initialization of buffer? --> Upto implementation?
 - Premultiplied alpha?
*/

#ifndef GIL_H
#define GIL_H

#include <stdint.h>  // <cstdint> cannot be used (C++11)

#ifdef WIN32
#define GIL_API extern __declspec(dllexport)
#else
#define GIL_API extern
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef int32_t GILBoolean;
typedef struct gil_config_t* GILConfig;
typedef struct gil_surface_t* GILSurface;
typedef struct gil_context_t* GILContext;
typedef struct gil_texture_t* GILTexture;

#define GIL_FALSE 0
#define GIL_TRUE 1

typedef enum
{
    GIL_FORMAT_INVALID,
    GIL_FORMAT_A_8,           ///< alpha only
    GIL_FORMAT_P_2_ARGB_8888, ///< 2 bit palette with 32 bit colors
    GIL_FORMAT_P_2_BGRA_8888, ///< 2 bit palette with 32 bit colors
    GIL_FORMAT_P_2_RGBA_8888, ///< 2 bit palette with 32 bit colors
    GIL_FORMAT_P_2_RGB_888,   ///< 2 bit palette with 24 bit colors
    GIL_FORMAT_P_2_RGB_565,   ///< 2 bit palette with 16 bit colors
    GIL_FORMAT_P_4_ARGB_8888, ///< 4 bit palette with 32 bit colors
    GIL_FORMAT_P_4_BGRA_8888, ///< 4 bit palette with 32 bit colors
    GIL_FORMAT_P_4_RGBA_8888, ///< 4 bit palette with 32 bit colors
    GIL_FORMAT_P_4_RGB_888,   ///< 4 bit palette with 24 bit colors
    GIL_FORMAT_P_4_RGB_565,   ///< 4 bit palette with 16 bit colors
    GIL_FORMAT_P_8_ARGB_8888, ///< 8 bit palette with 32 bit colors
    GIL_FORMAT_P_8_BGRA_8888, ///< 8 bit palette with 32 bit colors
    GIL_FORMAT_P_8_RGBA_8888, ///< 8 bit palette with 32 bit colors
    GIL_FORMAT_P_8_RGB_888,   ///< 8 bit palette with 24 bit colors
    GIL_FORMAT_P_8_RGB_565,   ///< 8 bit palette with 16 bit colors
    GIL_FORMAT_RGB_565,
    GIL_FORMAT_BGR_565,
    GIL_FORMAT_RGB_888,
    GIL_FORMAT_BGR_888,
    GIL_FORMAT_ARGB_8888,
    GIL_FORMAT_BGRA_8888,
    GIL_FORMAT_RGBA_8888,
} GILFormat;

typedef uint32_t GILError;

#define GIL_NO_ERROR            0
#define GIL_INVALID_CONTEXT     0x200
#define GIL_INVALID_OPERATION   0x201
#define GIL_INVALID_SURFACE     0x202
#define GIL_INVALID_VALUE       0x203

/**
 * Initializes the library.
 * Used for global configuration. Should only be called once.
 * @param config configuration
 */
GIL_API void gilInit(GILConfig config);

/**
 * Uninitializes the library.
 * Should only be called once.
 */
GIL_API void gilUninit(void);

/**
 * Creates a new window or layer and returns its surface. Does not necessarily be visible (can also be offscreen depending on window configuration). Memory may be uninitialized.
 * A call to gilSetColor, gilDrawArea or a fullscreen background bitmap (gilDrawQuad) is needed to have a defined initial state.
 * @param window window identifier (might refer to some library configuration which specifies the detailed window configuration like double buffering, color format, etc.)
 * @param x x position
 * @param y y position
 * @param w window width
 * @param h window height
 */
GIL_API GILSurface gilCreateWindow(uint8_t window, int32_t x, int32_t y, int32_t w, int32_t h);

/**
 * Creates a rendering context
 */
GIL_API GILContext gilCreateContext(void);

/**
 * Attaches the rendering context to a surface (in which the results of drawing commands will be stored)
 * @param context the rendering context to be attached to the surface
 * @param rendertarget the render target
 */
GIL_API GILBoolean gilSetSurface(GILContext context, GILSurface surface);

/**
 * Sets the drawing color for subsequent drawing commands
 * the color is used for gilDrawArea() and also for gilDrawQuad if the texture contains no color information
 */
GIL_API GILBoolean gilSetColor(GILContext context, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

/**
 * Creates a new (empty) texture
 */
GIL_API GILTexture gilCreateTexture(GILContext context);


#if 0
// Could be used to enable texture copies - if required in future
typedef enum
{
    GL_TEXTURE_COPY
} GILParam;
GIL_API void gilTexParameteri(GILTexture t, GILParam name, uint32_t value);
#endif

/**
 * Loads pixel data into the texture
 * @param t texture
 * @param width width in pixels
 * @param height height in pixels
 * @param format color format
 * @param data pixel data. Data pointer is required to be valid until shutdown
 * @return GIL_TRUE on success GIL_FALSE on error
 */
GIL_API GILBoolean gilTexPixels(GILTexture t, uint32_t width, uint32_t height, GILFormat format, const void* data);

/**
 * Loads palette data into the texture
 * Will return an error code if the palette does not match with the pixel data format
 * Pixel data must be set in advance (see gilTexPixels())
 * @param t texture
 * @param palette table of colors. Color format is specified in gilTexPixels. Data pointer is required to be valid until shutdown
 * @param size number of colors in the palette
 * @return GIL_TRUE on success GIL_FALSE on error
 */
GIL_API GILBoolean gilTexPalette4(GILTexture t, const void* palette, uint32_t size); // 4 byte color values
GIL_API GILBoolean gilTexPalette3(GILTexture t, const void* palette, uint32_t size);  // 3 byte color values
GIL_API GILBoolean gilTexPalette2(GILTexture t, const void* palette, uint32_t size); // 2 byte color values

/**
 * Assigns a texture to the context
 *
 * @note If the texture only contains alpha information, the color will be used from the color attribute
 */
GIL_API void gilBindTexture(GILContext context, GILTexture t);

/**
 * Sets the clipping area for subsequent drawing commands
 */
GIL_API GILBoolean gilSetClip(GILContext context, int32_t x1, int32_t y1, int32_t x2, int32_t y2);

/**
 * Clears the current surface with the current fill color
 */
GIL_API void gilClear(GILContext context);

/**
 * Draws a rectangle with the current fill color
 */
GIL_API void gilDrawArea(GILContext ctx, int32_t x1, int32_t y1, int32_t x2, int32_t y2);

/**
 * Draws a rectangle with the current texture reference (interface allows scaling, what happens if coordinates oustide bounds?)
 * @param x1 Specify the start point X1 of the drawing coordinate system.
 * @param y1 Specify the start point Y1 of the drawing coordinate system.
 * @param u1 Specify the U1 of source coordinate system corresponding to the value, (X1, Y1) of drawing coordinate system.
 * @param v1 Specify the V1 of source coordinate system corresponding to the value, (X1, Y1) of drawing coordinate system.
 * @param x2 Specify the end point X2 of the drawing coordinate system.
 * @param y2 Specify the end point Y2 of the drawing coordinate system.
 * @param u2 Specify the U2 of source coordinate system corresponding to the value, (X2, Y2) of drawing coordinate system.
 * @param v2 Specify the V2 of source coordinate system corresponding to the value, (X2, Y2) of drawing coordinate system.
 *
 * Each coordinate is given by the fixed-point number in the fractional part 4 bits.
 */
GIL_API void gilDrawQuad(GILContext ctx, int32_t x1, int32_t y1, int32_t u1, int32_t v1, int32_t x2, int32_t y2, int32_t u2, int32_t v2);

/**
 * Post the window buffer to the display
 * Only works for window surfaces
 */
GIL_API GILBoolean gilSwapBuffers(GILSurface surface);

/**
 * Checks the current surface with the currently bound texture
 * @param context the context will contain the reference texture and has bound the surface which is about to be verified
 * @param x1...y2 coordinates on the surface to verify
 * @param u1...v2 coordinates on the texture
 */
GIL_API GILBoolean gilVerify(GILContext context, int32_t x1, int32_t y1, int32_t u1, int32_t v1, int32_t x2, int32_t y2, int32_t u2, int32_t v2);

/**
 * Returns the value of the error flag.
 *
 * Each detectable error is assigned a numeric code and symbolic name. When an error occurs, the error flag is set
 * to the appropriate error code value. No other errors are recorded until gilGetError is called, the error code is returned,
 * and the flag is reset to GIL_NO_ERROR. If a call to gilGetError returns GIL_NO_ERROR, there has been no detectable error
 * since the last call to gilGetError, or since the GL was initialized.
 *
 * To allow for distributed implementations, there may be several error flags. If any single error flag has recorded an error,
 * the value of that flag is returned and that flag is reset to GIL_NO_ERROR when gilGetError is called.
 * If more than one flag has recorded an error, gilGetError returns and clears an arbitrary error flag value.
 * Thus, gilGetError should always be called in a loop, until it returns GIL_NO_ERROR, if all error flags are to be reset.
 *
 * Initially, all error flags are set to GIL_NO_ERROR.
 */
GIL_API GILError gilGetError(GILContext context);

/**
 * Handles the window events and indicates if the window has been closed
 *
 * This function may be mainly useful for simulation environments or systems, which use a window manager.
 * The engine will call it in its main loop independently from drawing or verification.
 * @return true indicates that the window has been closed by the window manager
 */
GIL_API GILBoolean gilHandleWindowEvents(GILContext context);

/**
 * Marks the end of one render/verify iteration
 */
GIL_API void gilSync(GILContext context);

#ifdef __cplusplus
}
#endif

#endif // GIL_H

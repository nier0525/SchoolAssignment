//-----------------------------------------------------------------------------
// File: Matrices.cpp
//
// Desc: Now that we know how to create a device and render some 2D vertices,
//       this tutorial goes the next step and renders 3D geometry. To deal with
//       3D geometry we need to introduce the use of 4x4 matrices to transform
//       the geometry with translations, rotations, scaling, and setting up our
//       camera.
//
//       Geometry is defined in model space. We can move it (translation),
//       rotate it (rotation), or stretch it (scaling) using a world transform.
//       The geometry is then said to be in world space. Next, we need to
//       position the camera, or eye point, somewhere to look at the geometry.
//       Another transform, via the view matrix, is used, to position and
//       rotate our view. With the geometry then in view space, our last
//       transform is the projection transform, which "projects" the 3D scene
//       into our 2D viewport.
//
//       Note that in this tutorial, we are introducing the use of D3DX, which
//       is a set of helper utilities for D3D. In this case, we are using some
//       of D3DX's useful matrix initialization functions. To use D3DX, simply
//       include <d3dx9.h> and link with d3dx9.lib.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include <Windows.h>
#include <mmsystem.h>

#include <Windows.h>
#include <d3d9.h>
#pragma comment (lib, "d3d9")
#include <d3dx9.h>
#pragma comment (lib, "d3dx9")
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )

#include <stack>
#include <map>
using namespace std;

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)


// A structure for our custom vertex type
struct CUSTOMVERTEX
{
    FLOAT x, y, z;      // The untransformed, 3D position for the vertex
    DWORD color;        // The vertex color
};

typedef struct _TAGSPHERE
{
    D3DXVECTOR3 vCenter;
    float fRadius;
}SPHERE, *LPSPHERE;

typedef struct _TAGAAB
{
    D3DXVECTOR3 vMin;
    D3DXVECTOR3 vMax;
}AABB, *LPAABB;

typedef struct _COLOR
{
    float r, g, b, a;

    _COLOR() { r = 1; b = 1; g = 1; a = 1; }
    _COLOR(float _r, float _g, float _b) { r = _r; g = _g; b = _b; a = 1; }
    _COLOR(float _r, float _g, float _b, float _a) { r = _r; g = _g; b = _b; a = _a; }
}COLOR, *LPCOLOR;

float fAngle(float speed);
//*****************************************************************************
//
// Image.h : Defines the class operations on images
//
// Author - Parag Havaldar
// Main Image class structure 
//
//*****************************************************************************

#ifndef IMAGE_DISPLAY
#define IMAGE_DISPLAY

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "afxwin.h"

// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// Class structure of Image 
// Use to encapsulate an RGB image
class MyImage
{

private:
	int		Width;					// Width of Image
	int		Height;					// Height of Image
	char	ImagePath[_MAX_PATH];	// Image location
	char*   Data;                   // RGB data of the image
	int     Mode;
	float	Scale;
	int     Antialiasing;
	POINT   pt;
	char*   Backup;

public:
	// Constructor
	MyImage();
	// Copy Constructor
	MyImage::MyImage(MyImage* otherImage);
	// Destructor
	~MyImage();

	// operator overload
	MyImage& operator= (const MyImage& otherImage);

	// Reader & Writer functions
	void	setWidth(const int w) { Width = w; };
	void	setHeight(const int h) { Height = h; };
	void	setImageData(const char* img) { Data = (char*)img; };
	void	setImagePath(const char* path) { strcpy(ImagePath, path); }
	void    setMode(int m) { Mode = m; }
	void    setScale(float s) { Scale = s; }
	void    setAA(int a) { Antialiasing = a; }
	void    setPT(POINT p) { pt = p; }
	int		getWidth() { return Width; };
	int		getHeight() { return Height; };
	char* getImageData() { return Data; };
	char* getImagePath() { return ImagePath; }
	int     getMode() { return Mode; }
	float   getScale() { return Scale; }
	float   getAA() { return Antialiasing; }


	// Input Output operations
	bool	ReadImage(); //
	bool	WriteImage();

	// Modifications
	bool	Modify();
	bool    ZoomOut();

};

#endif //IMAGE_DISPLAY

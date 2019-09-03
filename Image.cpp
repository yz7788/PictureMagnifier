//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"
#include <iostream>
using namespace std;
void                 NewPixelValue(char* RBuf, char* GBuf, char* BBuf, float x, float y, int w, int h, char* newData, int index, int Antialiasing);


// Constructor and Desctructors
MyImage::MyImage()
{
	Data = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
	Mode = 0;
	Scale = 1.0;
	Antialiasing = -1;
	Backup = NULL;
}

MyImage::~MyImage()
{
	if (Data)
		delete Data;
	if (Backup)
		delete Backup;
}


// Copy constructor
MyImage::MyImage(MyImage* otherImage)
{
	Height = otherImage->Height;
	Width = otherImage->Width;
	Data = new char[Width * Height * 3];
	Backup = new char[Width * Height * 3];
	strcpy(otherImage->ImagePath, ImagePath);

	for (int i = 0; i < (Height * Width * 3); i++)
	{
		Data[i] = otherImage->Data[i];
		Backup[i] = otherImage->Backup[i];
	}
}

// = operator overload
MyImage& MyImage::operator= (const MyImage& otherImage)
{
	Height = otherImage.Height;
	Width = otherImage.Width;
	Data = new char[Width * Height * 3];
	Backup = new char[Width * Height * 3];
	strcpy((char*)otherImage.ImagePath, ImagePath);

	for (int i = 0; i < (Height * Width * 3); i++)
	{
		Data[i] = otherImage.Data[i];
		Backup[i] = otherImage.Backup[i];
	}
	return *this;
}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0)
		if (ImagePath[0] == 0 || Width < 0 || Height < 0)
		{
			fprintf(stderr, "Image or Image properties not defined");
			fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
			return false;
		}

	// Create a valid output file pointer
	FILE* IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if (IN_FILE == NULL)
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char* Rbuf = new char[Height * Width];
	char* Gbuf = new char[Height * Width];
	char* Bbuf = new char[Height * Width];

	for (i = 0; i < Width * Height; i++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width * Height; i++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width * Height; i++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}


	// Allocate Data structure and copy
	Data = new char[Width * Height * 3];
	Backup = new char[Width * Height * 3];
	for (i = 0; i < Height * Width; i++)
	{
		Data[3 * i] = Bbuf[i];
		Data[3 * i + 1] = Gbuf[i];
		Data[3 * i + 2] = Rbuf[i];
		Backup[3 * i] = Bbuf[i];
		Backup[3 * i + 1] = Gbuf[i];
		Backup[3 * i + 2] = Rbuf[i];
	}

	// Clean up and return
	delete[] Rbuf;
	delete[] Gbuf;
	delete[] Bbuf;
	fclose(IN_FILE);

	return true;

}



// MyImage functions defined here
bool MyImage::WriteImage()
{
	// Verify ImagePath
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0)
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}

	// Create a valid output file pointer
	FILE* OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if (OUT_FILE == NULL)
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char* Rbuf = new char[Height * Width];
	char* Gbuf = new char[Height * Width];
	char* Bbuf = new char[Height * Width];

	for (i = 0; i < Height * Width; i++)
	{
		Bbuf[i] = Data[3 * i];
		Gbuf[i] = Data[3 * i + 1];
		Rbuf[i] = Data[3 * i + 2];
	}


	// Write data to file
	for (i = 0; i < Width * Height; i++)
	{
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width * Height; i++)
	{
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width * Height; i++)
	{
		fputc(Bbuf[i], OUT_FILE);
	}

	// Clean up and return
	delete[] Rbuf;
	delete[] Gbuf;
	delete[] Bbuf;
	fclose(OUT_FILE);

	return true;

}


// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify()
{
	char* RBuf = new char[Width * Height];
	char* GBuf = new char[Width * Height];
	char* BBuf = new char[Width * Height];

	for (int i = 0; i < Width * Height; i++) {
		RBuf[i] = *(Data + 3 * i);
		GBuf[i] = *(Data + 3 * i + 1);
		BBuf[i] = *(Data + 3 * i + 2);
	}

	int w = Width;
	int h = Height;
	int w_new = w * Scale;
	int h_new = h * Scale;
	Width = w_new;
	Height = h_new;
	char* newData = new char[w_new * h_new * 3];

	for (int index = 0; index < w_new * h_new; index++) {
		int x = index % (w_new);
		int y = index / (w_new);//[x,y] in new image is [x/k, y/k] in original one
		float x_orig = float(x / Scale);
		float y_orig = float(y / Scale);
		NewPixelValue(RBuf, GBuf, BBuf, x_orig, y_orig, w, h, newData, index, Antialiasing);
	}
	
	delete[] Data;
	delete[] RBuf;
	delete[] GBuf;
	delete[] BBuf;

	Data = newData;

	return false;
}

bool MyImage::ZoomOut() {
	int radius = 100;
	char* RBuf = new char[Width * Height];
	char* GBuf = new char[Width * Height];
	char* BBuf = new char[Width * Height];

	for (int i = 0; i < Width * Height; i++) {
		RBuf[i] = *(Backup + 3 * i);
		GBuf[i] = *(Backup + 3 * i + 1);
		BBuf[i] = *(Backup + 3 * i + 2);
	}
	for (int index = 0; index < Width * Height; index++) {
		int x = index % Width;
		int y = index / Width;
		if (((x - pt.x) * (x - pt.x) + (y - pt.y) * (y - pt.y)) > radius * radius) {
			int pixelvalue_R = pow(int(unsigned char(RBuf[index])),0.9);
			int pixelvalue_G = pow(int(unsigned char(GBuf[index])),0.9);
			int pixelvalue_B = pow(int(unsigned char(BBuf[index])),0.9);
			*(Data + 3 * index) = char(pixelvalue_R);
			*(Data + 3 * index + 1) = char(pixelvalue_G);
			*(Data + 3 * index + 2) = char(pixelvalue_B);
		}
		else {
			float x_orig = (x + (Scale - 1) * pt.x) / Scale;
			float y_orig = (y + (Scale - 1) * pt.y) / Scale;
			if (x_orig >= 0 && x_orig < Width && y_orig >= 0 && y_orig < Height) {
				NewPixelValue(RBuf, GBuf, BBuf, x_orig, y_orig, Width, Height, Data, index, Antialiasing); 
			}
		}

	}
	delete[] RBuf;
	delete[] GBuf;
	delete[] BBuf;
	return false;
}
 
void NewPixelValue(char* RBuf, char* GBuf, char* BBuf, float x, float y, int w, int h, char* newData, int index, int Antialiasing) {
	if (Antialiasing == 0) {
		int i = int(x + 0.5) + int(y + 0.5) * w;
		*(newData + 3 * index) = char(int(unsigned char(RBuf[i])));
		*(newData + 3 * index + 1) = char(int(unsigned char(GBuf[i])));
		*(newData + 3 * index + 2) = char(int(unsigned char(BBuf[i])));

	}
	else {
		float a = x - int(x);
		float b = y - int(y);
		int left_top = int(y) * w + int(x);
		int right_top = left_top + 1;
		int left_bottom = int(y + 1) * w + int(x);
		int right_bottom = left_bottom + 1; // calculate the pixel index of the four corners of the square containing (x_orig, y_orig)
		int pixelvalue_R = int(unsigned char(RBuf[left_top])) * (1 - a) * (1 - b) + int(unsigned char(RBuf[right_top])) * a * (1 - b) + int(unsigned char(RBuf[left_bottom])) * b * (1 - a) + int(unsigned char(RBuf[right_bottom])) * a * b;
		int pixelvalue_G = int(unsigned char(GBuf[left_top])) * (1 - a) * (1 - b) + int(unsigned char(GBuf[right_top])) * a * (1 - b) + int(unsigned char(GBuf[left_bottom])) * b * (1 - a) + int(unsigned char(GBuf[right_bottom])) * a * b;
		int pixelvalue_B = int(unsigned char(BBuf[left_top])) * (1 - a) * (1 - b) + int(unsigned char(BBuf[right_top])) * a * (1 - b) + int(unsigned char(BBuf[left_bottom])) * b * (1 - a) + int(unsigned char(BBuf[right_bottom])) * a * b;
		*(newData + 3 * index) = char(pixelvalue_R);
		*(newData + 3 * index + 1) = char(pixelvalue_G);
		*(newData + 3 * index + 2) = char(pixelvalue_B);
	}
}

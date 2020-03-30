#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include "GraphicLibs.h"
#include "Header.h"
#include "Constants.h"

class DisplayManager
{
public:
	unsigned char* ScreenBits;
	FrameBuffer* front;
	FrameBuffer* back;
	FrameBuffer* frame1;
	FrameBuffer* frame2;
	DepthBuffer* depth;

	HBITMAP screenDIB = NULL, dibBefore;
	HPALETTE hPalette = NULL;
	HDC hdc, dibDC;
	//ÏÔÊ¾×´Ì¬Á¿
	BOOL mode256 = false;
	bool FirstFrame = true;


	bool startUp(HWND& hWnd);
	void shutDown(HWND& hWnd);

	void initBuffers();
	void releaseBuffer();

	void frame();

	void swapBuffers();
	void flush();

	FrameBuffer* getFrontFrame();
	DepthBuffer* getDepthBuffer();

	void initDIB();
	void initDIB(HDC dibDC);
	void releaseDIB();
	void releaseDIB(HDC dibDC);
	

	void checkDisplayMode(HDC hdc);
	void createPalette(HDC hdc);
	void releasePalette();
};



#endif
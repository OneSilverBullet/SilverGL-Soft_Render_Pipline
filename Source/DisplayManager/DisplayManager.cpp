#include "DisplayManager.h"

bool DisplayManager::startUp(HWND& hWnd)
{
	hdc = GetDC(hWnd);
	checkDisplayMode(hdc);
	createPalette(hdc);
	dibDC = CreateCompatibleDC(hdc);
	releaseDIB(dibDC);
	initDIB(dibDC);

	initBuffers();
	FirstFrame = true;

	if (hPalette) {
		SelectPalette(hdc, hPalette, FALSE);
		RealizePalette(hdc);
	}

	return true;
}

void DisplayManager::shutDown(HWND& hWnd)
{
	releasePalette();
	releaseDIB(dibDC);
	DeleteDC(dibDC);
	ReleaseDC(hWnd, hdc);

	releaseBuffer();
}

void DisplayManager::initBuffers()
{
	SilverGL::initDevice2Buf(&frame1, &frame2, &depth, &front, &back, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void DisplayManager::releaseBuffer()
{
	SilverGL::releaseDevice2Buf(&frame1, &frame2, &depth, &back, &front);
}

void DisplayManager::frame()
{
	BitBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		dibDC, 0, 0, SRCCOPY);
}

void DisplayManager::swapBuffers()
{
	if (front == frame1)
	{
		front = frame2;
		back = frame1;
	}
	else
	{
		back = frame2;
		front = frame1;
	}
	
	if (FirstFrame)
	{
		FirstFrame = false;
		return;
	}
	flush();
}

void DisplayManager::flush()
{
	for (int i = 0; i < front->height; ++i)
	{
		for (int j = 0; j < front->width; ++j)
		{
			int index = (i * front->width + j) * 3;
			unsigned char red = front->c_data[index];
			unsigned char green = front->c_data[index + 1];
			unsigned char blue = front->c_data[index + 2];
			ScreenBits[index] = blue;
			ScreenBits[index + 1] = green;
			ScreenBits[index + 2] = red;
		}
	}
}

FrameBuffer * DisplayManager::getFrontFrame()
{
	return front;
}

DepthBuffer * DisplayManager::getDepthBuffer()
{
	return depth;
}

void DisplayManager::initDIB()
{
	initDIB(dibDC);
}

void DisplayManager::initDIB(HDC dibDC)
{
	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = SCREEN_WIDTH;
	bmi.bmiHeader.biHeight = -SCREEN_HEIGHT;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biCompression = BI_RGB;
	//创建应用程序直接写入与设备无关的位图

	screenDIB = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&ScreenBits, NULL, 0);

	//把一个位图对象选入到指定的设备描述表当中，来替换原来的旧对象
	dibBefore = (HBITMAP)SelectObject(dibDC, screenDIB);
}

void DisplayManager::releaseDIB()
{
	releaseDIB(dibDC);
}

void DisplayManager::releaseDIB(HDC dibDC)
{
	if (screenDIB != NULL) {
		SelectObject(dibDC, dibBefore);
		DeleteObject(screenDIB);
		screenDIB = NULL;
	}
}

void DisplayManager::checkDisplayMode(HDC hdc)
{
	int palSize = GetDeviceCaps(hdc, SIZEPALETTE);
	if (palSize == 256)
		mode256 = true;
}

void DisplayManager::createPalette(HDC hdc)
{
	if (!mode256)
		return;
	printf("create palette\n");
	//	hPalette=CreateHalftonePalette(hdc);
	int i = 0, incr = 8, red = 0, green = 0, blue = 0;
	LOGPALETTE* plp = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) + 246 * sizeof(PALETTEENTRY));
	plp->palVersion = 0x0300;
	plp->palNumEntries = 247;
	for (green = 0; green <= 255; green += incr) {
		plp->palPalEntry[i].peRed = (BYTE)green;
		plp->palPalEntry[i].peGreen = (BYTE)green;
		plp->palPalEntry[i].peBlue = (BYTE)green;
		plp->palPalEntry[i].peFlags = 0;
		incr = (incr == 9) ? 8 : 9;
		i++;
	}
	for (red = 0; red <= 255; red += 51) {
		for (green = 0; green <= 255; green += 51) {
			for (blue = 0; blue <= 255; blue += 51) {
				plp->palPalEntry[i].peRed = (BYTE)red;
				plp->palPalEntry[i].peGreen = (BYTE)green;
				plp->palPalEntry[i].peBlue = (BYTE)blue;
				plp->palPalEntry[i].peFlags = 0;
				i++;
			}
		}
	}
	hPalette = CreatePalette(plp);
	free(plp);
}

void DisplayManager::releasePalette()
{
	if (hPalette) {
		DeleteObject(hPalette);
		hPalette = NULL;
		printf("release palette\n");
	}
}

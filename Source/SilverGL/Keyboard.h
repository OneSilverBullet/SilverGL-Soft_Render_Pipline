#pragma once

#ifndef VK_ESCAPE
#define VK_ESCAPE 0x1B
#define VK_SPACE    32
#define VK_LEFT	    37
#define VK_UP	    38
#define VK_RIGHT    39
#define VK_DOWN	    40
#endif

bool willExit = false;
bool* turn_keys = NULL;
bool* move_keys = NULL;

void initKeys()
{
	turn_keys = new bool[4];
	memset(turn_keys, false, 4 * sizeof(bool));
	move_keys = new bool[6];
	memset(move_keys, false, 6 * sizeof(bool));
}

void releaseKeys()
{
	delete[] turn_keys;
	turn_keys = nullptr;
	delete[] move_keys;
	move_keys = nullptr;
}

void keyDown(int key)
{
	switch (key)
	{
	case 'W':
		move_keys[0] = true;
		break;
	case 'S':
		move_keys[1] = true;
		break;
	case 'A':
		move_keys[2] = true; 
		break;
	case 'D':
		move_keys[3] = true;
		break;
	case VK_SPACE:
		move_keys[4] = true;
		break;
	case 'Z':
		move_keys[5] = true;
		break;
	case VK_UP:
		turn_keys[0] = true;
		break;
	case VK_DOWN:
		turn_keys[1] = true;
		break;
	case VK_LEFT:
		turn_keys[2] = true;
		break;
	case VK_RIGHT:
		turn_keys[3] = true;
		break;
	default:
		break;
	}
}

void keyUp(int key)
{
	switch (key)
	{
	case 'W':
		move_keys[0] = false;
		break;
	case 'S':
		move_keys[1] = false;
		break;
	case 'A':
		move_keys[2] = false;
		break;
	case 'D':
		move_keys[3] = false;
		break;
	case VK_SPACE:
		move_keys[4] = false;
		break;
	case 'Z':
		move_keys[5] = false;
		break;
	case VK_UP:
		turn_keys[0] = false;
		break;
	case VK_DOWN:
		turn_keys[1] = false;
		break;
	case VK_LEFT:
		turn_keys[2] = false;
		break;
	case VK_RIGHT:
		turn_keys[3] = false;
		break;
	default:
		break;
	}
}
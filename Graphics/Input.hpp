#pragma once
#include "SDL.h"

enum KEYS_PRESSED_LIST
{
	KEY_ESCAPE, KEY_W, KEY_A, KEY_S, KEY_D, KEY_C, KEY_SPACE, KEY_T,KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,  SIZE_OF_KEYS_PRESSED_ENUM
};

//KEY_T is a temp toggel key

class Input
{
public:
	void Update() 
	{ 
 		while (SDL_PollEvent(&m_event) != NULL)
		{
			if (m_event.type == SDL_KEYDOWN)
			{
				SDL_Keycode keyPressed = m_event.key.keysym.sym;

				switch (keyPressed)
				{
				case SDLK_ESCAPE:
					m_keysPressed[KEY_ESCAPE] = true;
					break;
				case SDLK_w:
					m_keysPressed[KEY_W] = true;
					break;
				case SDLK_a:
					m_keysPressed[KEY_A] = true;
					break;
				case SDLK_s:
					m_keysPressed[KEY_S] = true;
					break;
				case SDLK_d:
					m_keysPressed[KEY_D] = true;
					break;
				case SDLK_c:
					m_keysPressed[KEY_C] = true;
					break;
				case SDLK_SPACE:
					m_keysPressed[KEY_SPACE] = true;
					break;
				case SDLK_t:
					m_keysPressed[KEY_T] = true;
					break;

				case SDLK_UP:
					m_keysPressed[KEY_UP] = true;
					break;
				case SDLK_DOWN:
					m_keysPressed[KEY_DOWN] = true;
					break;
				case SDLK_LEFT:
					m_keysPressed[KEY_LEFT] = true;
					break;
				case SDLK_RIGHT:
					m_keysPressed[KEY_RIGHT] = true;
					break;

				default:
					break;
				}
			}

			else if (m_event.type == SDL_KEYUP)
			{
				SDL_Keycode keyPressed = m_event.key.keysym.sym;

				switch (keyPressed)
				{
				case SDLK_w:
					m_keysPressed[KEY_W] = false;
					break;
				case SDLK_a:
					m_keysPressed[KEY_A] = false;
					break;
				case SDLK_s:
					m_keysPressed[KEY_S] = false;
					break;
				case SDLK_d:
					m_keysPressed[KEY_D] = false;
					break;
				case SDLK_c:
					m_keysPressed[KEY_C] = false;
					break;
				case SDLK_SPACE:
					m_keysPressed[KEY_SPACE] = false;
					break;
				case SDLK_t:
					m_keysPressed[KEY_T] = false;
					break;
				case SDLK_UP:
					m_keysPressed[KEY_UP] = false;
					break;
				case SDLK_DOWN:
					m_keysPressed[KEY_DOWN] = false;
					break;
				case SDLK_LEFT:
					m_keysPressed[KEY_LEFT] = false;
					break;
				case SDLK_RIGHT:
					m_keysPressed[KEY_RIGHT] = false;
					break;
				default:
					break;
				}
			}

			else if (m_event.type == SDL_MOUSEMOTION)
			{
				
				//mouseMotion = true
				if (!mouseFirstMove)
				{
					int mouseX, mouseY;
					SDL_GetMouseState(&mouseX, &mouseY);
					MouseRelX = mouseX;
						MouseRelY = mouseY;
				}
				else
				{
					mouseFirstMove = false;
					int mouseX, mouseY;
					SDL_GetMouseState(&mouseX, &mouseY);
					MouseRelX = mouseX;
					MouseRelY = mouseY;
					PreviousMouseLocation = vec2{ mouseX, mouseY };
				}
			}
		}

		changeMouseDelta();
	};

	

	bool KeyIsPressed(KEYS_PRESSED_LIST key) { return m_keysPressed[key]; };

	void setUp() 
	{
		int index = 0;
		while (index != SIZE_OF_KEYS_PRESSED_ENUM)
		{
			m_keysPressed[index] = 0;
			index++;
		}
	};

	//float getMouseRelX() { return MouseRelX; };
	//float getMouseRelY() { return MouseRelY; };
	vec2 getMouseDelta() { return MouseDelta; };

	bool ToggelLockToScreen() 
	{
		//temp function for debug toggeling mouse

		if (lockToScreen)
			lockToScreen = false;
		else
			lockToScreen = true;

		SDL_ShowCursor(1);

		return lockToScreen; 
	};

	Input(SDL_Window& window, int windowWidth, int windowHeight) : m_window(window)
	{
		setUp(); 
		windowW = windowWidth;
		windowH = windowHeight;
	};

	~Input() { };

private:
	void changeMouseDelta()
	{
		vec2 tempVec2 = { MouseRelX, MouseRelY };
		
		MouseDelta = PreviousMouseLocation - tempVec2;
		PreviousMouseLocation = vec2{ (windowW / 2), (windowH / 2) };

		if(lockToScreen)
			SDL_WarpMouseInWindow(&m_window, windowW / 2, windowH / 2);
	}

	SDL_Window& m_window;
	int windowW = 800;
	int windowH = 600;

	SDL_Event m_event;
	bool m_keysPressed[SIZE_OF_KEYS_PRESSED_ENUM];

	bool mouseFirstMove = true;
	bool isFirstLoop = true;

	vec2 PreviousMouseLocation = { 0,0 };
	vec2 MouseDelta;

	float MouseRelX = windowW/2;
	float MouseRelY = windowH/2;

	bool lockToScreen = true;
};
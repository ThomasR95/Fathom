#include <cstdlib>
#include <iostream>

#include "engine.h"

int main()
{
	theEngine = std::make_shared<Engine>();
	theEngine->init({ 1080,764 }, { 4096, 2897 });

	sf::Clock frameClock;

	while (theEngine->getWindow()->isOpen())
	{
		theEngine->handleEvents();
		theEngine->manageStates();

		if (frameClock.getElapsedTime() > sf::seconds(1.f / 60.f))
		{
			theEngine->render();
			frameClock.restart();
		}
		sf::sleep(sf::milliseconds(2));
		
	}

	return 0;
}

int __stdcall WinMain()
{
	return main();
}
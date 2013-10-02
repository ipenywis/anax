///
/// anax
/// An open source C++ entity system.
///
/// Copyright (C) 2013 Miguel Martin (miguel.martin7.5@hotmail.com)
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///

#ifndef __RUNGAME_HPP__
#define __RUNGAME_HPP__

#include <chrono>

#include <SFML/Graphics.hpp>

/// A typedef that is used to represent a second
typedef float Seconds;

/// \return The systems time now
inline Seconds GetTimeNow()
{
	using namespace std;
	return chrono::duration_cast<chrono::duration<Seconds, std::ratio<1> > >(chrono::high_resolution_clock::now().time_since_epoch()).count();
}

/// Runs the game
/// \tparam TGameConcept A game concept
/// \param game The game you wish to run
/// \return The error code generated by the game
template <typename TGameConcept>
int RunGame(sf::RenderWindow& window, TGameConcept& game)
{
	const float MAX_FRAME_TIME = 1 / 4.f;
	const float DELTA_TIME = 1 / 60.f;
	Seconds currentTime = 0; // Holds the current time
	Seconds accumulator = 0; // Used to accumlate time in the game loop
	
	sf::Event event;
	while(game.isRunning())
	{
		while(window.pollEvent(event))
		{
			game.handleEvents(event);
		}
		
		Seconds newTime = GetTimeNow();
		Seconds frameTime = newTime - currentTime;
		currentTime = newTime;
		
		// cap the loop delta time
		if(frameTime >= MAX_FRAME_TIME)
		{
			frameTime = MAX_FRAME_TIME;
		}
		
		accumulator += frameTime;
		
		
		// Update our game
		while(accumulator >= DELTA_TIME)
		{
			game.update(DELTA_TIME); // update the game (with the constant delta time)
			accumulator -= DELTA_TIME; // decrease the accumulator
		}
		
		game.render();
		window.display();
	}
	
	return 0;
}


#endif // __RUNGAME_HPP__

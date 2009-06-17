#include "plum.h"

namespace Plum
{
	AnimationParser::AnimationParser()
	{
		reset();
	}

	void AnimationParser::reset()
	{
		script = "";
		visible = true;
		frame = 0;
		wait = 0;
		position = 0;
	}

	std::string AnimationParser::getScript()
	{
		return script;
	}

	void AnimationParser::setScript(std::string script)
	{
		reset();
		std::transform(script.begin(), script.end(), script.begin(), toupper);
		this->script = script;
	}

	int AnimationParser::getFrame()
	{
		return frame;
	}

	int AnimationParser::parseNumber()
	{
		int c;
		std::string valueString = "";
		c = script[position];
		while(c >= '0' && c <= '9' && position < script.length() - 1)
		{
			valueString += c;
			position++;
			c = script[position];
		}
		return stringToInteger(valueString);
	}


	void AnimationParser::update()
	{
		char c;
		// If the script is zero-length, there's nothing to do.
		if(script.length() == 0)
		{
			return;
		}

		// While our animation is waiting, hold off further parsing.
		if (wait > 0)
		{
			wait--;
		}
		else
		{
			// Examine a character from the script string.
			c = script[position];
			// Change frames.
			if(c == 'F')
			{
				position++;
				frame = parseNumber();
			}
			// Wait a certain period of time
			else if(c == 'W')
			{
				position++;
				wait = parseNumber();
			}
			// Hide this entity
			else if(c == 'H')
			{
				visible = false;
			}
			// Show this entity
			else if(c == 'S')
			{
				visible = true;
			}
			// Go back to the beginning of the script.
			else if(c == 'B')
			{
				position = 0;
			}
			// Bad character, ignore it.
			else
			{
				position++;
			}

			// If the end of the script is reached, clear the script.
			if(position >= script.length())
			{
				script = "";
			}
		}
	}
}
#ifndef __GCONFIG_H
#define __GCONFIG_H

class GConfiguration
{
public:
	GConfiguration(
		int frameRate = 30,
		int millisecond = 1000
	)	: frameRate(frameRate),
		millisecond(millisecond),
		frameDuration(millisecond / frameRate)
	{
	}

public:
	const int frameRate;
	const int millisecond;
	const int frameDuration;
};

extern GConfiguration gConfig;

#endif

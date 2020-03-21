#include "ViVifLine.h"

ViVifLine::ViVifLine() :
	mIsEmpty(true) { }

ViVifLine::ViVifLine(std::vector<std::string> line) :
	mWords(line),
	mIsEmpty(line.size() <= 0)
{
}

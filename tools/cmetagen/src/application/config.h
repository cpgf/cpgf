#ifndef __CONFIG_H
#define __CONFIG_H



class Config
{
public:
	Config()
		: maxItemCountPerFile(0)
	{}
	
	size_t getMaxItemCountPerFile() const { return this->maxItemCountPerFile; }
	
private:
	size_t maxItemCountPerFile;
};


#endif

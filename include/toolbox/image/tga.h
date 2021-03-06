#pragma once


#include "image.h"


class TGA : public IMAGE{
	friend class IMAGE;
	TGA();
	TGA(const TGA&);
	void operator=(const TGA&);
public:
	struct RAW{
		unsigned char IDLen;
		unsigned char IsColorMap;
		unsigned char type;
		unsigned short colorMapIndex;
		unsigned short colorMapLength;
		unsigned char colorMapEntrySize;
		unsigned short xOffset;
		unsigned short yOffset;
		unsigned short width;
		unsigned short height;
		unsigned char colorDepth;
		unsigned char attribute;
		unsigned char data[0]; //並び准はBGR(A)
	}__attribute__((packed));
	TGA(const void* rawTGA);

private:
	static IMAGE* New(int);
	static void Dump(const void*);
};

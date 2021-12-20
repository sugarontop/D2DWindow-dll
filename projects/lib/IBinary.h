#pragma once

namespace _K{

struct innmm
{
	BYTE* p;
	unsigned int len;
		
	innmm():p(nullptr),len(0){}
	innmm(BYTE* pp, unsigned int alen ):p(pp),len(alen){}
	~innmm(){ delete [] p; }
};

};

typedef std::shared_ptr<_K::innmm> IBinary;

#define IBinaryMk(a,b) (new _K::innmm(a,b))	// IBinary bin( IBinaryMk(cb,len));
#define IBinaryLen(a) ((*a).len)
#define IBinaryPtr(a) ((*a).p)

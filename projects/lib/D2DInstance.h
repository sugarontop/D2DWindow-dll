#pragma once



class D2DInstance
{
	public :
		D2DInstance():not_yet_use(0){};


		int not_yet_use;
};



struct D2DInstaceExport
{
	DWORD magicnumber;
	D2DInstance instance;

};
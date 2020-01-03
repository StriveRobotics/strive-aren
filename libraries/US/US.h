#ifndef US_h
#define US_h

class US
{
public:
	void init(int pinTrigger, int pinEcho);
	float read();

private:
	int _pinTrigger;
	int _pinEcho;
	long TP_init();
};

#endif //US_h
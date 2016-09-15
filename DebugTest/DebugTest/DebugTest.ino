int8_t s1 = 0;
int8_t s2 = 0;
int16_t s3 = 0;


void setup()
{
	s1 = 0;
	s2 = s1 << 1;
	s3 = s2*s2;
}

void loop()
{
	s2 = 120;
	s2 = s2 << 1;
	s3 = s2*s2;
	if (s2 < 0) {
		s3 = -s3;
	}
}
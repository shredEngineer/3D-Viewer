//	Data dimension
#define DATA_SIZE_X		120
#define DATA_SIZE_Y		60
#define DATA_SIZE_Z		60

//	Standard includes
#include <cmath>

//	Math
#ifndef M_PI
#define M_PI 3.14159265359
#endif

//	Functions
float getData(int x, int y, int z);
void refreshData(int dataMode, int dataTime);

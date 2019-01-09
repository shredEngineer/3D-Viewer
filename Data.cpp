#include "Data.hpp"

//	Data matrix
float data[DATA_SIZE_X][DATA_SIZE_Y][DATA_SIZE_Z] = {};

//	Get single data point
float getData(int x, int y, int z)
{
	return data[x][y][z];
}

//	Refresh data matrix
void refreshData(int dataMode, int dataTime)
{
	for (int z = 0; z < DATA_SIZE_Z; ++z)
	{
		for (int y = 0; y < DATA_SIZE_Y; ++y)
		{
			for (int x = 0; x < DATA_SIZE_X; ++x)
			{
				//	Generate some fancy data
				float xx = (x - (DATA_SIZE_X - 1) / 2.0) / (DATA_SIZE_X - 1) * 2.0;
				float yy = (y - (DATA_SIZE_Y - 1) / 2.0) / (DATA_SIZE_Y - 1) * 2.0;
				float zz = (z - (DATA_SIZE_Z - 1) / 2.0) / (DATA_SIZE_Z - 1) * 2.0;
				float a = dataMode * 1.0;
				float b = dataTime / 20.0;
				float c = dataMode * 2.0;
				float d = dataTime / 10.0;
				float f = cos(M_PI * (xx * a + b)) * sin(M_PI * (yy * c + d));
				if (f <= zz && f >= zz - 0.3)
				{
					data[x][y][z] = (1 + f) / 2.0;
				}
				else
				{
					data[x][y][z] = 0;
				}
			}
		}
	}
}

#include "MovingPlatformIndex.h"

void MovingPlatformIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{
	bitStream->Write(flag_0);
	if (flag_0)
	{
		bitStream->Write(flag_0_0);
		if (flag_0_0)
		{
			bitStream->Write(pathName);
			bitStream->Write(data_0_1);
			bitStream->Write(data_0_2);
		}
		// HOPE THIS WORKS

		bitStream->Write(flag_1_0);
		if (flag_1_0) {
			bitStream->Write(subcomponentType);
			if (subcomponentType == 4) {
				bitStream->Write(flag_1_0_0);
				if (flag_1_0_0) {
					bitStream->Write(state);
					bitStream->Write(data_1_0_0);
					bitStream->Write(data_1_0_1);
					bitStream->Write(data_1_0_2);
					bitStream->Write(data_1_0_3);

					bitStream->Write(target_pos_x);
					bitStream->Write(target_pos_y);
					bitStream->Write(target_pos_z);

					bitStream->Write(currWaypointIndex);
					bitStream->Write(nextWaypointIndex);
					
					bitStream->Write(idleTimePassed);
					bitStream->Write(data_1_0_4);

				}
			}
			
			if (subcomponentType == 5) {
				bitStream->Write(flag_1_1);
				if (flag_1_1) {
					bitStream->Write(flag_1_2);
					if (flag_1_2) {
						bitStream->Write(pos_x);
						bitStream->Write(pos_y);
						bitStream->Write(pos_z);

						bitStream->Write(rot_x);
						bitStream->Write(rot_y);
						bitStream->Write(rot_z);
						bitStream->Write(rot_w);
					}
				}
				bitStream->Write(flag_1_3);
				if (flag_1_3) {
					bitStream->Write(data_1_3_0);
					bitStream->Write(data_1_3_1);
					bitStream->Write(data_1_3_2);
				}
			}
		}
	}
}

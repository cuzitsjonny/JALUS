#include "ControllablePhysicsIndex.h"

void ControllablePhysicsIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{
	if (isConstruction)
	{
		bitStream->Write(flag_0);
		if (flag_0)
		{
			bitStream->Write(data_0_0);
			bitStream->Write(data_0_1);
			bitStream->Write(data_0_2);
		}

		bitStream->Write(flag_1);
		if (flag_1)
		{
			bitStream->Write(data_1_0);
			bitStream->Write(data_1_1);
			bitStream->Write(data_1_2);
			bitStream->Write(data_1_3);
			bitStream->Write(data_1_4);
			bitStream->Write(data_1_5);
			bitStream->Write(data_1_6);
		}
	}

	bitStream->Write(flag_2);
	if (flag_2)
	{
		bitStream->Write(data_2_0);
		bitStream->Write(data_2_1);
	}

	bitStream->Write(flag_3);
	if (flag_3)
	{
		bitStream->Write(data_3_0);
		bitStream->Write(data_3_1);
	}

	bitStream->Write(flag_4);
	if (flag_4)
	{
		bitStream->Write(flag_4_0);
		if (flag_4_0)
		{
			bitStream->Write(data_4_0_0);
			bitStream->Write(data_4_0_1);
		}
	}

	bitStream->Write(flag_5);
	if (flag_5)
	{
		bitStream->Write(pos_x);
		bitStream->Write(pos_y);
		bitStream->Write(pos_z);
		bitStream->Write(rot_x);
		bitStream->Write(rot_y);
		bitStream->Write(rot_z);
		bitStream->Write(rot_w);
		bitStream->Write(is_on_ground);
		bitStream->Write(data_5_8);

		bitStream->Write(flag_velocity);
		if (flag_velocity)
		{
			bitStream->Write(vel_x);
			bitStream->Write(vel_y);
			bitStream->Write(vel_z);
		}

		bitStream->Write(flag_angular_velocity);
		if (flag_angular_velocity)
		{
			bitStream->Write(ang_vel_x);
			bitStream->Write(ang_vel_y);
			bitStream->Write(ang_vel_z);
		}

		bitStream->Write(flag_5_11);
		if (flag_5_11)
		{
			bitStream->Write(data_5_11_0);
			bitStream->Write(data_5_11_1);
			bitStream->Write(data_5_11_2);
			bitStream->Write(data_5_11_3);

			bitStream->Write(flag_5_11_4);
			if (flag_5_11_4)
			{
				bitStream->Write(data_5_11_4_0);
				bitStream->Write(data_5_11_4_1);
				bitStream->Write(data_5_11_4_2);
			}
		}

		if (!isConstruction)
		{
			bitStream->Write(data_5_12);
		}
	}
}

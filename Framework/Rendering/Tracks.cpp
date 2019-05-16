#include "Tracks.h"

Tracks::Tracks(Track& trackKeys)
{
	int i;

	if (trackKeys.pos.size())
	{
		keys.pos.resize(trackKeys.pos.size());
		for (i = 0; i < keys.pos.size(); ++i)
		{
			keys.pos[i].frame = trackKeys.pos[i].frame;
			memcpy(keys.pos[i].value, &trackKeys.pos[i].value, sizeof Vector3);
		}
	}

	if (trackKeys.rot.size())
	{
		Quaternion q = Quaternion(0, 0, 0, 1);
		keys.rot.resize(trackKeys.rot.size());
		keys.rot[0].frame = trackKeys.rot[0].frame;
		keys.rot[0].value = q;

		for (i = 0; i < keys.rot.size(); ++i)
		{
			keys.rot[i].frame = trackKeys.rot[i].frame;

			if (trackKeys.rot[i].frame <= trackKeys.start) q = Quaternion(0, 0, 0, 1);
			else Quaternion(trackKeys.rot[i].value.x, trackKeys.rot[i].value.y, trackKeys.rot[i].value.z, trackKeys.rot[i].value.w);

			D3DXQuaternionMultiply(&keys.rot[i].value, &trackKeys.rot[i - 1].value, &q);
		}
	}

	if (trackKeys.scl.size())
	{
		keys.scl.resize(trackKeys.scl.size());
		for (i = 0; i < keys.scl.size(); i++)
		{
			keys.scl[i].frame = trackKeys.scl[i].frame;
			memcpy(&keys.scl[i].value, &trackKeys.scl[i].value, sizeof Vector3);
		}
	}
}
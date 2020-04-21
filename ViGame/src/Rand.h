#pragma once

namespace vigame
{
	namespace rand
	{
		static int g_seed = 0;

		inline int Rand() {
			g_seed = (214013 * g_seed + 2531011);
			return (g_seed >> 16) & 0x7FFF;
		}

		inline int RandInt(int start, int end)
		{
			return (Rand() % (end - start)) + start;
		}
	}
}
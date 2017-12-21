
#include "Core/Core.h"

int64 getUniqueId() {
	static int64 id = 0;
	return id++;
}
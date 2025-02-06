#include "SAMMITypes.hpp"

std::string getBlockDir(short state1, short state2) {
	std::string ret = "";
	if (state1 == 3) {
		ret += "Air";
	}
	switch (state2) {
	case 128:
	case 2176:
	case 160:
	case 2208:
		ret += "Standing";
		break;

	case 132:
	case 2180:
	case 164:
	case 2212:
		ret += "Crouching";
		break;
	}
	return ret;
}


std::string getBlockMeth(short state1) {
	std::string ret = "";
	switch (state1) {
	case 128:
		ret += "Normal";
		break;
	case 2176:
		ret += "Instant";
		break;
	case 160:
		ret += "Barrier";
		break;
	case 2208:
		ret += "Instant Barrier";
		break;
	case 132:
		ret += "Normal";
		break;
	case 2180:
		ret += "Instant";
		break;
	case 164:
		ret += "Barrier";
		break;
	case 2212:
		ret += "Instant Barrier";
		break;
	}
	return ret;
}
#include "Apex.h"

Apex::Apex(int apex) {
	if (apex == NULL)
		APEX = -1;
	else
		APEX = (apex > 0 ? apex : 8);
}

Apex::~Apex() {

}

int Apex::GetApex() {
	return APEX;
}

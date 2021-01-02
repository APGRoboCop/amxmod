#include "dodxmod.h"

void Forwards::findForwards() {
	static char *forwardName[] = {
		"damage_info",
		"death_info",
		"score_info",
		"grenade_throw",
	};
	AMX* amx;
	void* code;
	const char* filename;
	int i = 0;
	register int j;
	int iFunc;

	// Search for plugins that have my forward functions.
	while((amx = MF_GetAmxScript( i++, &code, &filename )) != 0) {  // Iterate Plugin List
		for(j = 0; j < FORWARD_NUM; ++j) {
			if(AMX_FINDPUBLIC(amx, forwardName[j], &iFunc) == AMX_ERR_NONE) {
				g_forwards.registerForward(amx, iFunc, j);
			}
		}
	}
}

void Forwards::registerForward( AMX* p, int func, int type ) {
	AmxCall** a = &head[ type ];
	while(*a) a = &(*a)->next;
	*a = new AmxCall( p , func );
}

void Forwards::clearForwards( AmxCall** a ) {
	while( *a )	{
		AmxCall* b = (*a)->next;
		delete *a;
		*a = b;
	}
}

void Forwards::clear() {
	register int a;
	for( a = 0; a < FORWARD_NUM; ++a )
		clearForwards( &head[ a ] );
}


void Forwards::executeForward(int type, int d1, int d2, int d3) {
	cell ret = 0;
	AmxCall* a = head[ type ];
	while(a) {
		if(MF_IsPluginRunning(a->amx)) {
			AMX_EXEC(a->amx, &ret, a->getFunction(), 3, d1, d2, d3);
			if(ret) break;
		}
		a = a->next;
	}
}

void Forwards::executeForward(int type, int d1, int d2, int d3, int d4, int d5) {
	cell ret = 0;
	AmxCall* a = head[ type ];
	while(a) {
		if(MF_IsPluginRunning(a->amx)) {
			AMX_EXEC(a->amx, &ret, a->getFunction(), 5, d1, d2, d3, d4, d5);
			if(ret) break;
		}
		a = a->next;
	}
}

void Forwards::executeForward(int type, int d1, int d2, int d3, int d4, int d5, int d6) {
	cell ret = 0;
	AmxCall* a = head[ type ];
	while(a) {
		if(MF_IsPluginRunning(a->amx)) {
			AMX_EXEC(a->amx, &ret, a->getFunction(), 6, d1, d2, d3, d4, d5, d6);
			if(ret) break;
		}
		a = a->next;
	}
}

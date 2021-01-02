#include "vexdum.h"

TraceResult *vx_tr;

NATIVE(traceline_get_int) {
	int iSet = params[1];
	int iVal = 0;

	switch(iSet) {
		case TR_fAllSolid:
			iVal = vx_tr->fAllSolid;
			break;
		case TR_fStartSolid:
			iVal = vx_tr->fStartSolid;
			break;
		case TR_fInOpen:
			iVal = vx_tr->fInOpen;
			break;
		case TR_fInWater:
			iVal = vx_tr->fInWater;
			break;
		case TR_iHitgroup:
			iVal = vx_tr->iHitgroup;
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid TR_ parameter");
			return 0;
			break;
	}
	return iVal;
}

NATIVE(traceline_set_int) {
	int iSet = params[1];
	int iVal = params[2];

	switch(iSet) {
		case TR_fAllSolid:
			vx_tr->fAllSolid = iVal;
			break;
		case TR_fStartSolid:
			vx_tr->fStartSolid = iVal;
			break;
		case TR_fInOpen:
			vx_tr->fInOpen = iVal;
			break;
		case TR_fInWater:
			vx_tr->fInWater = iVal;
			break;
		case TR_iHitgroup:
			vx_tr->iHitgroup = iVal;
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid TR_ parameter");
			return 0;
			break;
	}
	return 1;
}

NATIVE(traceline_get_float) {
	int iSet = params[1];
	float fVal = 0.0;

	switch(iSet) {
		case TR_flFraction:
			fVal = vx_tr->flFraction;
			break;
		case TR_flPlaneDist:
			fVal = vx_tr->flPlaneDist;
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid TR_ parameter");
			return 0;
			break;
	}
	return FloatToCell(fVal);
}

NATIVE(traceline_set_float) {
	int iSet = params[1];
	float fVal = CellToFloat(params[2]);

	switch(iSet) {
		case TR_flFraction:
			vx_tr->flFraction = fVal;
			break;
		case TR_flPlaneDist:
			vx_tr->flPlaneDist = fVal;
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid TR_ parameter");
			return 0;
			break;
	}
	return 1;
}

NATIVE(traceline_get_vector) {
	int iSet = params[1];
	cell* vRet = MF_GetAmxAddr(amx, params[2]);
	Vector vVal = Vector(0, 0, 0);

	switch(iSet) {
		case TR_vecEndPos:
			vVal = vx_tr->vecEndPos;
			break;
		case TR_vecPlaneNormal:
			vVal = vx_tr->vecPlaneNormal;
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid TR_ parameter");
			return 0;
			break;
	}
	vRet[0] = FloatToCell(vVal[0]);
	vRet[1] = FloatToCell(vVal[1]);
	vRet[2] = FloatToCell(vVal[2]);
	return 1;
}

NATIVE(traceline_set_vector) {
	int iSet = params[1];
    cell* vInput = MF_GetAmxAddr(amx, params[2]);

	float vVal[3] = {
		CellToFloat(vInput[0]),
		CellToFloat(vInput[1]),
		CellToFloat(vInput[2])
	};

	switch(iSet) {
		case TR_vecEndPos:
			vx_tr->vecEndPos = vVal;
			break;
		case TR_vecPlaneNormal:
			vx_tr->vecPlaneNormal = vVal;
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid TR_ parameter");
			return 0;
			break;
	}
	return 1;
}

NATIVE(traceline_get_edict) {
	int iSet = params[1];
	int pTarget;

	switch(iSet) {
		case TR_pHit:
			pTarget = ENTINDEX(vx_tr->pHit);
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid TR_ parameter");
			return 0;
			break;
	}
	if(FNullEnt(pTarget)) {
		return 0;
	}
	return pTarget;
}

NATIVE(traceline_set_edict) {
	int iSet = params[1];
	int ent = params[2];
	edict_t* pTarget = INDEXENT(ent);

	switch(iSet) {
		case TR_pHit:
			vx_tr->pHit = pTarget;
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid TR_ parameter");
			return 0;
			break;
	}
	return 1;
}

AMX_NATIVE_INFO VexdUM_Tr[] = {
	{ "traceline_get_int",    traceline_get_int },
	{ "traceline_set_int",    traceline_set_int },
	{ "traceline_get_float",  traceline_get_float },
	{ "traceline_set_float",  traceline_set_float },
	{ "traceline_get_vector", traceline_get_vector },
	{ "traceline_set_vector", traceline_set_vector },
	{ "traceline_get_edict",  traceline_get_edict },
	{ "traceline_set_edict",  traceline_set_edict },

	{NULL,                   NULL}
};

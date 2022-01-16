/*  Float arithmetic for the Small AMX engine
 *
 *  Copyright (c) Artran, Inc. 1999
 *  Written by Greg Garner (gmg@artran.com)
 *  This file may be freely used. No warranties of any kind.
 *
 */

#include <cmath>
#include <cstdlib>
#include <extdll.h>
#include <meta_api.h>
#include "amxmod.h"

NATIVE(_float) {
  return FloatToCell((float)params[1]);
}

NATIVE(_floatstr) {
  int len;
  return FloatToCell((float)atof(get_amxstring(amx, params[1], 0, len)));
}

NATIVE(_floattostr) {
  char szTemp[32];
  sprintf(szTemp, "%f", CellToFloat(params[1]));
  return set_amxstring(amx, params[2], szTemp, params[3]);
}

NATIVE(_floatmul) {
  return FloatToCell(CellToFloat(params[1]) * CellToFloat(params[2]));
}

NATIVE(_floatdiv) {
  return FloatToCell(CellToFloat(params[1]) / CellToFloat(params[2]));
}

NATIVE(_floatadd) {
  return FloatToCell(CellToFloat(params[1]) + CellToFloat(params[2]));
}

NATIVE(_floatsub) {
  return FloatToCell(CellToFloat(params[1]) - CellToFloat(params[2]));
}

NATIVE(_floatfract) {
  float fA = CellToFloat(params[1]);
  fA -= (float)(floor((double)fA));
  return FloatToCell(fA);
}

NATIVE(_floatround) {
  float fA = CellToFloat(params[1]);
  switch(params[2]) {
    case 1:
      fA = (float)(floor((double)fA));
      break;
    case 2:
      fA = (float)(ceil((double)fA));
      break;
    case 3: /* round towards zero */
      if(fA >= 0.0) {
        fA = (float)(floor((double)fA));
      }
      else {
        fA = (float)(ceil((double)fA));
      }
      break;
    default:
      fA = (float)(floor((double)fA + .5));
      break;
  }
  return (long)fA;
}

NATIVE(_floatcmp) {
  float fA = CellToFloat(params[1]);
  float fB = CellToFloat(params[2]);
  if(fA == fB) {
    return 0;
  }
  else if(fA > fB) {
    return 1;
  }
  else {
    return -1;
  }
}

AMX_NATIVE_INFO float_Natives[] = {
  { "float",         _float },
	{ "floatstr",      _floatstr },
  { "floattostr",    _floattostr },
	{ "float_to_str",  _floattostr },
  { "floatmul",      _floatmul },
  { "floatdiv",      _floatdiv },
  { "floatadd",      _floatadd },
  { "floatsub",      _floatsub },
  { "floatfract",    _floatfract },
  { "floatround",    _floatround },
  { "floatcmp",      _floatcmp },
  {nullptr, nullptr}
};

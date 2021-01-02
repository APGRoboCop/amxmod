/*  Maths arithmetic for the Small AMX engine
 *
 *  Copyright (c) Artran, Inc. 1999
 *  Written by Greg Garner (gmg@artran.com)
 *  This file may be freely used. No warranties of any kind.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <extdll.h>
#include <meta_api.h>
#include "amxmod.h"

#define PI  3.1415926535897932384626433832795

static float FromRadians(float angle, int radix) {
	switch(radix) {
		case 1:         /* degrees, sexagesimal system (technically: degrees/minutes/seconds) */
			return (float)(angle / PI * 180.0);
		case 2:         /* grades, centesimal system */
			return (float)(angle / PI * 200.0);
		default:        /* assume already radian */
			return angle;
	}
}

static float ToRadians(float angle, int radix) {
  switch (radix) {
    case 1:         /* degrees, sexagesimal system (technically: degrees/minutes/seconds) */
      return (float)(angle * PI / 180.0);
    case 2:         /* grades, centesimal system */
      return (float)(angle * PI / 200.0);
    default:        /* assume already radian */
      return angle;
  }
}

// Math, abs
NATIVE(_abs) {
  return abs(params[1]);
}

// Math, fabs
NATIVE(_fabs) {
  float fValue = CellToFloat(params[1]);
  fValue = (float)fabs((double)fValue);

  return FloatToCell(fValue);
}

// Math, asin
NATIVE(_asin) {
  float fValue = CellToFloat(params[1]);
  fValue = (float)asin((double)fValue);
  if((*params / sizeof(cell)) == 2) {
    fValue = FromRadians(fValue, params[2]);
  }

  return FloatToCell(fValue);
}

// Math, sin
NATIVE(_sin) {
  float fValue = CellToFloat(params[1]);
  if((*params / sizeof(cell)) == 2) {
    fValue = ToRadians(fValue, params[2]);
  }
  fValue = (float)sin((double)fValue);

  return FloatToCell(fValue);
}

// Math, sinh
NATIVE(_sinh) {
  float fValue = CellToFloat(params[1]);
  if((*params / sizeof(cell)) == 2) {
    fValue = ToRadians(fValue, params[2]);
  }
  fValue = (float)sinh((double)fValue);

  return FloatToCell(fValue);
}

// Math, acos
NATIVE(_acos) {
  float fValue = CellToFloat(params[1]);
  fValue = (float)acos((double)fValue);
  if((*params / sizeof(cell)) == 2) {
    fValue = FromRadians(fValue, params[2]);
  }
  
  return FloatToCell(fValue);
}

// Math, cos
NATIVE(_cos) {
  float fValue = CellToFloat(params[1]);
  if((*params / sizeof(cell)) == 2) {
    fValue = ToRadians(fValue, params[2]);
  }
  fValue = (float)cos((double)fValue);

  return FloatToCell(fValue);
}

// Math, cosh
NATIVE(_cosh) {
  float fValue = CellToFloat(params[1]);
  if((*params / sizeof(cell)) == 2) {
    fValue = ToRadians(fValue, params[2]);
  }
  fValue = (float)cosh((double)fValue);

  return FloatToCell(fValue);
}

// Math, atan
NATIVE(_atan) {
  float fValue = CellToFloat(params[1]);
  fValue = (float)atan((double)fValue);
	if((*params / sizeof(cell)) == 2) {
    fValue = FromRadians(fValue, params[2]);
	}
	
  return FloatToCell(fValue);
}

// Math, atan2
NATIVE(_atan2) {
  float flInput1 = CellToFloat(params[1]);
  float flInput2 = CellToFloat(params[2]);
  float r = (float)atan2((double)flInput2, (double)flInput1);
  if((*params / sizeof(cell)) == 3) {
    r = FromRadians(r, params[3]);
  }
  
  return FloatToCell(r);
}

// Math, tan
NATIVE(_tan) {
  float fValue = CellToFloat(params[1]);
  if((*params / sizeof(cell)) == 2) {
    fValue = ToRadians(fValue, params[2]);
  }
  fValue = (float)tan((double)fValue);

  return FloatToCell(fValue);
}

// Math, tanh
NATIVE(_tanh) {
  float fValue = CellToFloat(params[1]);
  if((*params / sizeof(cell)) == 2) {
    fValue = ToRadians(fValue, params[2]);
  }
  fValue = (float)tanh((double)fValue);

  return FloatToCell(fValue);
}

// Math, fsqroot
NATIVE(_fsqroot) {
  float fValue = CellToFloat(params[1]);
  fValue = (float)sqrt((double)fValue);
  if(fValue < 0) {
    return amx_RaiseError(amx, AMX_ERR_DOMAIN);
  }

  return FloatToCell(fValue);
}

// Math, fpower
NATIVE(_fpower) {
  float flInput1 = CellToFloat(params[1]);
  float flInput2 = CellToFloat(params[2]);
  float r = (float)pow((double)flInput1, (double)flInput2);

  return FloatToCell(r);
}

// Math, flog
NATIVE(_flog) {
  float fValue = CellToFloat(params[1]);
  float fBase = CellToFloat(params[2]);
  if(fValue <= 0.0 || fBase <= 0) {
    return amx_RaiseError(amx, AMX_ERR_DOMAIN);
  }
  float r;
  if(fBase == 10.0) {
    r = (float)log10((double)fValue);
  }
  else {
    r = (float)(log((double)fValue) / log((double)fBase));
  }

  return FloatToCell(r);
}

// Native list.
AMX_NATIVE_INFO math_Natives[] = {
  { "abs",     _abs },
  { "fabs",    _fabs },
  { "asin",    _asin },
  { "sin",     _sin },
  { "sinh",    _sinh },
  { "acos",    _acos },
  { "cos",     _cos },
  { "cosh",    _cosh },
  { "atan",    _atan },
  { "atan2",   _atan2 },
  { "tan",     _tan },
  { "tanh",    _tanh },
  { "fsqroot", _fsqroot },
  { "fpower",  _fpower },
  { "flog",    _flog },
  { NULL, NULL }
};

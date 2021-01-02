/* AMX Mod X
*
* by the AMX Mod X Development Team
*  originally developed by OLO
*
*
*  This program is free software; you can redistribute it and/or modify it
*  under the terms of the GNU General Public License as published by the
*  Free Software Foundation; either version 2 of the License, or (at
*  your option) any later version.
*
*  This program is distributed in the hope that it will be useful, but
*  WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
*  General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software Foundation,
*  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
*  In addition, as a special exception, the author gives permission to
*  link the code of this program with the Half-Life Game Engine ("HL
*  Engine") and Modified Game Libraries ("MODs") developed by Valve,
*  L.L.C ("Valve"). You must obey the GNU General Public License in all
*  respects for all of the code used other than the HL Engine and MODs
*  from Valve. If you modify this file, you may extend this exception
*  to your version of the file, but you are not obligated to do so. If
*  you do not wish to do so, delete this exception statement from your
*  version.
*/

#include <string.h>
#include "optimizer.h"

int g_opt_level = 0;

#define OP_SYSREQ_C    123
#define OP_NOP         134
#define OP_FLOAT_MUL   138
#define OP_FLOAT_DIV   139
#define OP_FLOAT_ADD   140
#define OP_FLOAT_SUB   141
#define OP_FLOAT_TO    142
#define OP_FLOAT_ROUND 143
#define OP_FLOAT_CMP   144

cell op_trans_table[N_Total_FloatOps] =
{
  OP_FLOAT_MUL,
  OP_FLOAT_DIV,
  OP_FLOAT_ADD,
  OP_FLOAT_SUB,
  OP_FLOAT_TO,
  OP_FLOAT_ROUND,
  OP_FLOAT_CMP
};

void OnBrowseRelocate(AMX *amx, cell *oplist, cell *cip)
{
  char *codeptr = (char *)amx->base + (long)(((AMX_HEADER *)amx->base)->cod);

  //jump to the parameter;
  codeptr += *cip;

  int native = -1;
  cell n_offs = *(cell *)codeptr;
  optimizer_s *opt = (optimizer_s *)amx->usertags[UT_OPTIMIZER];
  for (int i=0; i<N_Total_FloatOps; ++i)
  {
    if (opt->natives[i] == n_offs)
    {
      native = i;
      break;
    }
  }

  if (native != -1)
  {
    //we're patching this:
    // 0x7B 0x??   SYSREQ.C float???
    //with:
    // 0x8A        FLOAT.MUL
    // 0x86        NOP
    cell new_opcodes[2];
    new_opcodes[0] = op_trans_table[native];
    new_opcodes[1] = OP_NOP;
    codeptr -= sizeof(cell);
#if defined __GNUC__ || defined ASM32 || defined JIT || defined JITORNOT
    *(cell *)codeptr = oplist[new_opcodes[0]];
    *(cell *)(codeptr + sizeof(cell)) = oplist[new_opcodes[1]];
#else
    *(cell *)codeptr = new_opcodes[0];
    *(cell *)(codeptr + sizeof(cell)) = new_opcodes[1];
#endif
  }

  *cip += sizeof(cell);

  return;
}

#define FIND_NATIVE(name, bind) \
  if (amx_FindNative(amx, name, &index) != AMX_ERR_NOTFOUND) \
    opt->natives[bind] = index;

void _Setup_Optimizer_Stage2(AMX *amx, cell *oplist, cell *cip)
{
  int index;

  amx->usertags[UT_BROWSEHOOK] = (void *)OnBrowseRelocate;

  optimizer_s *opt = new optimizer_s;

  for (int i=0; i<N_Total_FloatOps; ++i)
    opt->natives[i] = -1;

  amx->usertags[UT_OPTIMIZER] = (void *)opt;

  if (g_opt_level & 1)
  {
    FIND_NATIVE("floatmul", N_Float_Mul);
    FIND_NATIVE("floatdiv", N_Float_Div);
    FIND_NATIVE("floatadd", N_Float_Add);
    FIND_NATIVE("floatsub", N_Float_Sub);
  }
  if (g_opt_level & 4)
  {
    FIND_NATIVE("float", N_Float_To);
    FIND_NATIVE("floatround", N_Float_Round);
  }
  if (g_opt_level & 2)
  {
#if defined JIT || defined JITORNOT
    if (amxx_CpuSupport())
    {
      FIND_NATIVE("floatcmp", N_Float_Cmp);
    }
    else {
      g_opt_level &= ~(2);
    }
#else
    g_opt_level &= ~(2);
#endif // JIT || JITORNOT
  }
}

void SetupOptimizer(AMX *amx)
{
  amx->usertags[UT_BROWSEHOOK] = (void *)_Setup_Optimizer_Stage2;
}

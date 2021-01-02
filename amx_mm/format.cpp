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

#include <extdll.h>
#include <meta_api.h>
#include "amxmod.h"
#include "format.h"


// By BAILOPAN (AMXMODX Dev Team)

//Adapted from Quake3's vsprintf
// thanks to cybermind for linking me to this :)
//I made the following changes:
// - Fixed spacing to be AMX Mod X standard
// - Added 'n' support, no buffer overflows
// - Templatized input/output buffers

#define LADJUST   0x00000004    /* left adjustment */
#define ZEROPAD   0x00000080    /* zero (as opposed to blank) pad */
#define to_digit(c)   ((c) - '0')
#define is_digit(c)   ((unsigned)to_digit(c) <= 9)
#define CHECKARGS(n) \
  if ((arg+n) > args) { \
    amx_logerror(amx, AMX_ERR_NATIVE, "String formatted incorrectly - parameter %d (total %d)\n", arg, args); \
    return 0; \
  }

// By AMX Mod Team
int fo_numargs(AMX *amx) {
  unsigned char *data = amx->base + (int)((AMX_HEADER *)amx->base)->dat;
  cell bytes = * (cell *)(data + (int)amx->frm + 2*sizeof(cell));
  return (int)(bytes/sizeof(cell));
}

// By AMX Mod Team
cell* fo_getarg(AMX *amx, int pos) {
  unsigned char *data = amx->base + (int)((AMX_HEADER *)amx->base)->dat;
  cell value = * (cell *)(data + (int)amx->frm + (pos + 3)*sizeof(cell));
  return (cell *)(data + (int)value);
}
	
template <typename U>
void AddString(U **buf_p, size_t &maxlen, const cell *string, int width, int prec, int flags, AMX* amx)
{
  int size = 0;
  U *buf;
  static cell nlstr[] = {'(','n','u','l','l',')','\0'};

  buf = *buf_p;

  if (string == NULL)
  {
    string = nlstr;
    prec = -1;
  }

  if(g_translator.isTranslation(string)) {
    int len;
    char* s = (char*)g_plugins.findPluginFast(amx)->getDictionary()->getMessage(*string, len);
    size = len;

    if (prec >= 0)
    {
      for (size = 0; size < prec; ++size)
      {
        if (s[size] == '\0')
          break;
      }
    } else {
      while (s[size++]) ;
      --size;
    }

    if (size > (int)maxlen)
      size = maxlen;

    //Modified by KRoT@L for things like "%9s" and "%-9s"
    if( width > 0 && !(flags & LADJUST) )
    {
      if (flags & ZEROPAD)
      {
        while (size < width && maxlen)
        {
          *buf++ = '0';
          --width;
          --maxlen;
        }
      }
      else
      {
        while (size < width && maxlen)
        {
          *buf++ = ' ';
          --width;
          --maxlen;
        }
      }
    }

    while (size-- && maxlen) {
      *buf++ = static_cast<U>(*s++);
      --width;
      --maxlen;
    }

    if (flags & LADJUST)
    {
      if (flags & ZEROPAD)
      {
        while (width-- && maxlen)
        {
          *buf++ = '0';
          --maxlen;
        }
      }
      else
      {
        while (width-- && maxlen)
        {
          *buf++ = ' ';
          --maxlen;
        }
      }
    }
  }
  else {
    if (prec >= 0)
    {
      for (size = 0; size < prec; ++size)
      {
        if (string[size] == '\0')
          break;
      }
    } else {
      while (string[size++]) ;
      --size;
    }

    if (size > (int)maxlen)
      size = maxlen;

    //Modified by KRoT@L for things like "%9s" and "%-9s"
    if( width > 0 && !(flags & LADJUST) )
    {
      if (flags & ZEROPAD)
      {
        while (size < width && maxlen)
        {
          *buf++ = '0';
          --width;
          --maxlen;
        }
      }
      else
      {
        while (size < width && maxlen)
        {
          *buf++ = ' ';
          --width;
          --maxlen;
        }
      }
    }

    while (size-- && maxlen) {
      *buf++ = static_cast<U>(*string++);
      --width;
      --maxlen;
    }

    if (flags & LADJUST)
    {
      if (flags & ZEROPAD)
      {
        while (width-- && maxlen)
        {
          *buf++ = '0';
          --maxlen;
        }
      }
      else
      {
        while (width-- && maxlen)
        {
          *buf++ = ' ';
          --maxlen;
        }
      }
    }
  }

  *buf_p = buf;
}

template <typename U>
void AddFloat(U **buf_p, size_t &maxlen, double fval, int width, int prec)
{
  U text[32];
  int digits;
  double signedVal;
  U *buf;
  int val;

  // get the sign
  signedVal = fval;
  if (fval < 0)
    fval = -fval;

  // write the float number
  digits = 0;
  val = (int)fval;
  do {
    text[digits++] = '0' + val % 10;
    val /= 10;
  } while (val);

  if (signedVal < 0)
    text[digits++] = '-';

  buf = *buf_p;

  while (digits < width && maxlen)
  {
    *buf++ = ' ';
    --width;
    --maxlen;
  }

  while (digits-- && maxlen)
  {
    *buf++ = text[digits];
    --maxlen;
  }

  *buf_p = buf;

  if (prec < 0)
    prec = 6;
  // write the fraction
  digits = 0;
  while (digits < prec)
  {
    fval -= (int) fval;
    fval *= 10.0f;
    fval += 0.000001f;
    val = (int) fval;
    text[digits++] = '0' + val % 10;
  }

  if (digits > 0 && maxlen)
  {
    buf = *buf_p;
    *buf++ = '.';
    --maxlen;
    for (prec = 0; maxlen && prec < digits; ++prec)
    {
      *buf++ = text[prec];
      --maxlen;
    }
    *buf_p = buf;
  }
}

template <typename U>
void AddUInt(U **buf_p, size_t &maxlen, unsigned int val, int width, int flags)
{
  U   text[32];
  int   digits;
  U   *buf;

  digits = 0;
  do {
    text[digits++] = '0' + val % 10;
    val /= 10;
  } while (val);

  buf = *buf_p;

  if( !(flags & LADJUST) )
  {
    while (digits < width && maxlen)
    {
      *buf++ = (flags & ZEROPAD) ? '0' : ' ';
      --width;
      --maxlen;
    }
  }

  while (digits-- && maxlen)
  {
    *buf++ = text[digits];
    --width;
    --maxlen;
  }

  if (flags & LADJUST)
  {
    while (width-- && maxlen)
    {
      *buf++ = (flags & ZEROPAD) ? '0' : ' ';
      --maxlen;
    }
  }

  *buf_p = buf;
}

template <typename U>
void AddInt(U **buf_p, size_t &maxlen, int val, int width, int flags)
{
  U text[32];
  int digits;
  int signedVal;
  U *buf;
  unsigned int unsignedVal;

  digits = 0;
  signedVal = val;
  if (val < 0)
  {
    /* we want the unsigned version */
    unsignedVal = abs(val);
  } else {
    unsignedVal = val;
  }
  do {
    text[digits++] = '0' + unsignedVal % 10;
    unsignedVal /= 10;
  } while (unsignedVal);

  if (signedVal < 0)
    text[digits++] = '-';

  buf = *buf_p;

  if( !(flags & LADJUST) )
  {
    while (digits < width && maxlen)
    {
      *buf++ = (flags & ZEROPAD) ? '0' : ' ';
      --width;
      --maxlen;
    }
  }

  while (digits-- && maxlen)
  {
    *buf++ = text[digits];
    --width;
    --maxlen;
  }

  if (flags & LADJUST)
  {
    while (width-- && maxlen)
    {
      *buf++ = (flags & ZEROPAD) ? '0' : ' ';
      --maxlen;
    }
  }

  *buf_p = buf;
}

template <typename D, typename S>
size_t atcprintf(D *buffer, size_t maxlen, const S *format, AMX *amx, cell *params, int *param)
{
  int   arg=0;
  int   args = params[0] / sizeof(cell);
  D     *buf_p;
  D     ch;
  int   flags=0;
  int   width=0;
  int   prec=0;
  int   n=0;
  char  sign;
  const S *fmt;
  size_t  llen = maxlen;

  buf_p = buffer;
  arg = *param;
  fmt = format;

  while (true)
  {
    // run through the format string until we hit a '%' or '\0'
    for (ch = static_cast<D>(*fmt);
      llen && ((ch = static_cast<D>(*fmt)) != '\0' && ch != '%');
      fmt++)
    {
      *buf_p++ = static_cast<D>(ch);
      --llen;
    }
    if (ch == '\0' || llen <= 0)
      goto done;

    // skip over the '%'
    fmt++;

    // reset formatting state
    flags = 0;
    width = 0;
    prec = -1;
    sign = '\0';

rflag:
    ch = static_cast<D>(*fmt++);
reswitch:
    switch(ch)
    {
    case '-':
      flags |= LADJUST;
      goto rflag;
    case '.':
      n = 0;
      while( is_digit( ( ch = static_cast<D>(*fmt++)) ) )
        n = 10 * n + ( ch - '0' );
      prec = n < 0 ? -1 : n;
      goto reswitch;
    case '0':
      flags |= ZEROPAD;
      goto rflag;
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      n = 0;
      do {
        n = 10 * n + ( ch - '0' );
        ch = static_cast<D>(*fmt++);
      } while( is_digit( ch ) );
      width = n;
      goto reswitch;
    case 'c':
      CHECKARGS(0);
      *buf_p++ = static_cast<D>(*get_amxaddr(amx, params[arg]));
      ++arg;
      if (!llen) // fixed by KRoT@L
        goto done;
      --llen;
      break;
    case 'd':
    case 'i':
      CHECKARGS(0);
      AddInt(&buf_p, llen, *get_amxaddr(amx, params[arg]), width, flags);
      ++arg;
      break;
    case 'u':
      CHECKARGS(0);
      AddUInt(&buf_p, llen, static_cast<unsigned int>(*get_amxaddr(amx, params[arg])), width, flags);
      ++arg;
      break;
    case 'f':
      CHECKARGS(0);
      AddFloat(&buf_p, llen, amx_ctof(*get_amxaddr(amx, params[arg])), width, prec);
      ++arg;
      break;
    case 's':
      CHECKARGS(0);
      AddString(&buf_p, llen, get_amxaddr(amx, params[arg]), width, prec, flags, amx);
      ++arg;
      break;
    case '%':
      *buf_p++ = static_cast<D>(ch);
      if (!llen)
        goto done;
      --llen;
      break;
    case '\0':
      *buf_p++ = static_cast<D>('%');
      if (!llen)
        goto done;
      --llen;
      goto done;
      break;
    default:
      *buf_p++ = static_cast<D>(ch);
      if (!llen)
        goto done;
      --llen;
      break;
    }
  }

done:
  *buf_p = static_cast<D>(0);
  *param = arg;
  return maxlen-llen;
}

template <typename D, typename S>
size_t atcprintf(D *buffer, size_t maxlen, const S *format, AMX *amx, int *param)
{
  int   arg=0;
  int   args = fo_numargs(amx);
  D     *buf_p;
  D     ch;
  int   flags=0;
  int   width=0;
  int   prec=0;
  int   n=0;
  char  sign;
  const S *fmt;
  size_t  llen = maxlen;

  buf_p = buffer;
  arg = *param;
  fmt = format;

  while (true)
  {
    // run through the format string until we hit a '%' or '\0'
    for (ch = static_cast<D>(*fmt);
      llen && ((ch = static_cast<D>(*fmt)) != '\0' && ch != '%');
      fmt++)
    {
      *buf_p++ = static_cast<D>(ch);
      --llen;
    }
    if (ch == '\0' || llen <= 0)
      goto done;

    // skip over the '%'
    fmt++;

    // reset formatting state
    flags = 0;
    width = 0;
    prec = -1;
    sign = '\0';

rflag:
    ch = static_cast<D>(*fmt++);
reswitch:
    switch(ch)
    {
    case '-':
      flags |= LADJUST;
      goto rflag;
    case '.':
      n = 0;
      while( is_digit( ( ch = static_cast<D>(*fmt++)) ) )
        n = 10 * n + ( ch - '0' );
      prec = n < 0 ? -1 : n;
      goto reswitch;
    case '0':
      flags |= ZEROPAD;
      goto rflag;
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      n = 0;
      do {
        n = 10 * n + ( ch - '0' );
        ch = static_cast<D>(*fmt++);
      } while( is_digit( ch ) );
      width = n;
      goto reswitch;
    case 'c':
      CHECKARGS(0);
      *buf_p++ = static_cast<D>(*fo_getarg(amx, arg));
      ++arg;
      if (!llen) // fixed by KRoT@L
        goto done;
      --llen;
      break;
    case 'd':
    case 'i':
      CHECKARGS(0);
      AddInt(&buf_p, llen, *fo_getarg(amx, arg), width, flags);
      ++arg;
      break;
    case 'u':
      CHECKARGS(0);
      AddUInt(&buf_p, llen, static_cast<unsigned int>(*fo_getarg(amx, arg)), width, flags);
      ++arg;
      break;
    case 'f':
      CHECKARGS(0);
      AddFloat(&buf_p, llen, amx_ctof(*fo_getarg(amx, arg)), width, prec);
      ++arg;
      break;
    case 's':
      CHECKARGS(0);
      AddString(&buf_p, llen, fo_getarg(amx, arg), width, prec, flags, amx);
      ++arg;
      break;
    case '%':
      *buf_p++ = static_cast<D>(ch);
      if (!llen)
        goto done;
      --llen;
      break;
    case '\0':
      *buf_p++ = static_cast<D>('%');
      if (!llen)
        goto done;
      --llen;
      goto done;
      break;
    default:
      *buf_p++ = static_cast<D>(ch);
      if (!llen)
        goto done;
      --llen;
      break;
    }
  }

done:
  *buf_p = static_cast<D>(0);
  *param = arg;
  return maxlen-llen;
}

/*
 * HACKHACK: The compiler will generate code for each case we need.
 * Don't remove this, otherwise files that use certain code generations
 * will have extern problems.  For each case you need, add dummy code here.
 */
void __WHOA_DONT_CALL_ME_PLZ_K_lol_o_O()
{
  //acsprintf
  atcprintf((cell *)NULL, 0, (const char *)NULL, NULL, NULL, NULL);
  //accprintf
  atcprintf((cell *)NULL, 0, (cell *)NULL, NULL, NULL, NULL);
  //ascprintf
  atcprintf((char *)NULL, 0, (cell *)NULL, NULL, NULL, NULL);

  atcprintf((char *)NULL, 0, (char *)NULL, NULL, NULL, NULL);
  
  // By AMX Mod Team
  atcprintf((char *)NULL, 0, (cell *)NULL, NULL, NULL);
  atcprintf((char *)NULL, 0, (char *)NULL, NULL, NULL);
}

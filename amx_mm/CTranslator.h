/*
 * Copyright (c) 2002-2011 - Aleksander Naszko, Julien Laurent
 *
 *    This file is part of AMX Mod.
 *
 *
 *    AMX Mod is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    AMX Mod is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with AMX Mod; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 *
 */

#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#define AMX_TRANS "[AMX] Translator:"

/*

How to store and search strings:

LOOKUP_NORMAL:
	sequential lookup (strings are sorted even if it's useless)
LOOKUP_OPTIMIZED:
	binary ('dichotomic') lookup (strings are sorted)
LOOKUP_HASH:
	each (English) string is computed a hash value, then strings
	are sorted by hash (and hash lookup uses a binary search)

Of course, only one of these must be defined :P

Note:
'LOOKUP_HASH' was formerly known as 'LOOKUP_HASH_OPT', but I think
you won't mind that I've removed 'LOOKUP_HASH_NORM' (performing a
sequential search on hashes). :)
Let's use LOOKUP_NORMAL first (AMX 0.9.9), then if everything's fine
we'll probably switch to LOOKUP_HASH.

*/

// #define LOOKUP_NORMAL
// #define LOOKUP_OPTIMIZED
#define LOOKUP_HASH

extern cell* get_amxaddr(AMX *amx, cell amx_addr);

class CTranslator {
public:
	
	class iterator;
	class CDictionary	{
		friend class iterator;
		friend class CTranslator;

		CDictionary* next;
		CDictionary(const char *dict) : next(nullptr), tmphead(nullptr), tmpstrings(0), languages(nullptr), lCounter(0), tCounter(0), gCursor(0) {
			name.set(dict);
		}
		bool addFile(const char *file, int pass = 1, bool primary = true);
		~CDictionary();
		void addTempString(const char *str);
		void delTempStrings();

		class CTempString {
			friend class CDictionary;

		public:
			CTempString *next;
#ifndef LOOKUP_HASH
			CTempString(const char *str, CTempString *nxt = 0) : next(nxt) { string.set(str); }
#else
			CTempString(const char *str, unsigned long hsh, CTempString *nxt = nullptr) : next(nxt), hash(hsh) {
				string.set(str);
			}
			unsigned long hash;
#endif
			String string;
		};

		class CLangString {
		public:
			String string;
			int length;
#ifdef LOOKUP_HASH
			unsigned long hash;
#endif
		};

		class CLangCollection {
		public:
			CLangString *coll;
		};

		CTempString *tmphead;
		int tmpstrings;
		CLangCollection *languages;
		int *lSupport;
		void getString(const char *english, int langid, int &res1, int &res2);

		int lCounter;
		int tCounter;
		
		int addGarbage(const char *str);
		String gString[256];
		int gLength[256];
		int gCursor;
		
#if defined LOOKUP_OPTIMIZED
		int findString(const char *str, int from, int to);
#elif defined LOOKUP_HASH
		unsigned long getHash(const char *str);
		int findHash(unsigned long hash, int from, int to);
		int findString(const char *str);
#endif
	public:
		String name;
		int getStringId(const char *str);
		cell getTranslation(const char *english, int destid, int forcelang);
		const char *getMessage(cell val, int &length);
		const char *getMessage(int dest, int msg, int &length);
		bool loadDictionary();
		int getTranslations();
		inline int getLangSupport(int langid) { return lSupport[langid]; }
		inline int getLangs() { return lCounter; }
		const char *getMessage(const char *english, int destid, int &length);
	};

private:
	CDictionary *head;
	int dCounter;
	String *lang[255];
	int lSupport[255];
	void clearDictionary(CDictionary *);
	int serverLang;
	int dest;

public:
	CTranslator() : head(nullptr), dCounter(0), serverLang(0) {
		memset(&lang, 0, sizeof(lang));
		lang[0] = new String("en");
		/* lang[1] = new String("fr");
		lang[2] = new String("de");
		lang[3] = new String("es");
		lang[4] = new String("it"); */
	}
	~CTranslator() { clear(); }

	CDictionary *loadDictionary(const char *name);
	CDictionary *findDictionary(const char *name);
	void unloadDictionary(CDictionary **d);
	void clear();
	int getLangId(const char *code);
	int addLang(const char *code);
	const char *getLang(int id);
	int getLangs();
	void setDest(int i) { dest=i; }
	int getDest() { return dest; }
	bool isTranslation(AMX *amx, cell param) {
		cell *s = get_amxaddr(amx, param);
		return isTranslation(s);
	}
	bool isTranslation(const cell* s) {
    #if defined CHINESE_SUPPORT
    return (*s & 0xFFFFFF00 && (*s & 0xFFFFFF00) != 0xFFFFFF00) ? true : false;
    #else
    return (*s & 0xFFFFFF00) ? true : false;
    #endif
	}
	int getServerLang() { return serverLang; }
	bool setServerLang(const char *l) {
		int i = getLangId(l);
		if(i == -1) return false;
		serverLang = i;
		return true;
	}
	bool setServerLang(int l) {
		if(l == -1 || l >= getLangs()) return false;
		serverLang = l;
		return true;
	}
	void updateSupports();
	inline int getLangSupport(int langid) { return lSupport[langid]; }

	class iterator {
		CDictionary *a;

	public:
		iterator(CDictionary*aa = nullptr) : a(aa) {}
		iterator& operator++() { a = a->next; return *this; }
		bool operator == (const iterator& b) const { return a == b.a; }
		bool operator != (const iterator& b) const { return !operator == (b); }
		operator bool () const { return a ? true : false; }
		CDictionary& operator*() { return *a; }
	};
	inline iterator begin() const { return iterator(head); }
	inline iterator end() const { return iterator(nullptr); }
};
#endif

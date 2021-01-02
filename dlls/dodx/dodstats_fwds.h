#ifndef dodstats_fwds_h
#define dodstats_fwds_h

#define FORWARD_NUM 4

enum {
	FF_DamageInfo,
	FF_DeathInfo,
	FF_ScoreInfo,
	FF_GrenadeThrow
};

class Forwards {
public:

	class AmxCall {
		friend class Forwards;
		AMX *amx;
		int function;
		AmxCall* next;
		AmxCall( AMX *p, int func ): amx(p), function(func) {next=0;}
		inline int getFunction() {return function;}
	};

private:
	AmxCall *head[FORWARD_NUM];
	void clearForwards(AmxCall** a);

public:
	Forwards() {memset( head, 0, sizeof(head));}
	~Forwards() { clear(); }
	void clear();
	inline bool forwardsExist(int type) { return head[(int)type] ? true : false; }

	void findForwards();
	void registerForward( AMX *p , int func, int type );
	// Non-Return value forwards
	void executeForward( int type , int d1, int d2, int d3 );
	void executeForward( int type , int d1, int d2, int d3, int d4, int d5 );
	void executeForward( int type , int d1, int d2, int d3, int d4, int d5, int d6 );
};

extern Forwards g_forwards;

#endif // dodstats_fwds_h

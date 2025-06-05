#ifndef CVARSYS_H
#define CVARSYS_H

#include "core/object/class_db.h"
#include "core/templates/hash_map.h"
#include "core/object/object.h"



//TODO: Determine if console cmds and cvars are totally seperate but interconnected.

//Example Cvars we wish to support
//draw_wireframe 1
//bind k kill
//cl_setres 1920 1080
//cl_mousesense 0.3
//kill
//god 1
//noclip
//connect 127.0.0.1:8080
//name "NewPlayer"
//screenshot

/*
We need to support:
storing:
	Strings
	ints
	floats
Calling functions with and without arguments
*/

/*
We likely should made this an autoload singleton node that is driven by code.
We may want to integrate it with the console so that we support convars natively in the console.
*/

//TODO: We need a memory efficient way of storing different variable types.
//Do we store them all as strings and then use flags to specify type?

struct CvarData {
	//DEVNOTE: These are ordered from largest to smallest to avoid padding. 
	StringName	name;
	String		stringVal;
	String		resetString;
	String		latchString;
	int			intVal;
	float		floatVal;
	uint16_t	flags; //Holds up to 16 flags
	bool		modified;

	CvarData(StringName cvarName, String defVal, uint16_t cvarFlags)
	{
		name		= cvarName;
		stringVal	= defVal;
		resetString = defVal;
		latchString = "";
		intVal		= defVal.to_int();
		floatVal	= defVal.to_float();
		flags		= cvarFlags;
		modified	= false;
	}
};

class CvarSys : public Object {
	GDCLASS(CvarSys, Object);
	static CvarSys *singleton;
	static void _bind_methods();

public:
	//CVAR FLAGS
	const uint32_t CVAR_FLAGS_MAX = 1024;

	//DEVNOTE: If these change, be sure to update GDScript binds.
	const enum CvarBitFlags {
		CVAR_SAVE = 1 << 0, //Saved system settings.
		CVAR_TEMP = 1 << 1, //Will be reset on game restart.

		CVAR_CLIENTINFO = 1 << 2, //Client info sent to server.
		CVAR_SERVERINFO = 1 << 3, //Sent from server to client.
		CVAR_SHAREDINFO = 1 << 4, //Propagated to all clients.

		CVAR_ONRESTART = 1 << 5, //Variable will be stored, but won't be set until the next time the cvar is read.
								 //Used for things like gamemode and server settings that require a level or game restart.

		CVAR_INITONLY = 1 << 6, //Can't be changed from console, only set as launch perameters
		CVAR_READONLY = 1 << 7, //Read only
		CVAR_DEVONLY = 1 << 8, //Can only be changed in developer mode.
		CVAR_NORESET = 1 << 9, //Cvar ignores being set back to default.

	};

	const enum CvarSetResult {
		CVAR_SET_SUCCESS,
		CVAR_SET_SUCCESS_ONRESTART,
		CVAR_SET_FAILED_INVALID,
		CVAR_SET_FAILED_INITONLY,
		CVAR_SET_FAILED_READONLY,
		CVAR_SET_FAILED_DEVONLY,
	};


	static CvarSys *get_singleton();

	CvarData *Add(StringName name, String defVal, uint16_t flags);
	void Remove(StringName name);

	CvarData *Get(StringName name, String defVal, uint16_t flags);
	CvarSetResult Set( StringName name, String val);

	CvarData *Peek(StringName name);
	bool Exists(StringName name);

	//Write all CVAR_SAVE vars to external file.
	void Save();

	//Read all CVAR_SAVE vars from external file.
	void Load();

	//Resets all convars to their default values.
	void ToDefaults();

	CvarSys();
	~CvarSys();

private:
	CvarData *cvarDevMode = nullptr;
	HashMap<StringName, CvarData*> cvars;

	//This bitflag must be set every time a category of cvar is set.
	//It allows other systems to listen for changes and respond accordingly.
	//Be sure to clear flags as you handle them (i.e. cvarChangedFlags &= ~CVAR_SAVE; ) once the cvars have been written to a file.
	uint16_t cvarChangedFlags = 0;

	void Init();
	void Destroy();
	CvarData *Find( StringName name);

};

VARIANT_ENUM_CAST(CvarSys::CvarSetResult);
VARIANT_ENUM_CAST(CvarSys::CvarBitFlags)

#endif //CVARSYS_H

#include "cvarsys.h"

/////////////////////////
// CVAR BACKEND SYSTEM //
/////////////////////////

CvarSys *CvarSys::singleton = nullptr;
CvarSys *CvarSys::get_singleton() {
	return singleton;
}


void CvarSys::_bind_methods()
{
	//CvarBitFlags Enum
	BIND_ENUM_CONSTANT(CVAR_SAVE);
	BIND_ENUM_CONSTANT(CVAR_TEMP);
	BIND_ENUM_CONSTANT(CVAR_CLIENTINFO);
	BIND_ENUM_CONSTANT(CVAR_SERVERINFO);
	BIND_ENUM_CONSTANT(CVAR_SHAREDINFO);
	BIND_ENUM_CONSTANT(CVAR_ONRESTART);
	BIND_ENUM_CONSTANT(CVAR_INITONLY);
	BIND_ENUM_CONSTANT(CVAR_READONLY);
	BIND_ENUM_CONSTANT(CVAR_DEVONLY);
	BIND_ENUM_CONSTANT(CVAR_NORESET);

	//CvarSetResult Enum
	BIND_ENUM_CONSTANT(CVAR_SET_SUCCESS);
	BIND_ENUM_CONSTANT(CVAR_SET_SUCCESS_ONRESTART);
	BIND_ENUM_CONSTANT(CVAR_SET_FAILED_INVALID);
	BIND_ENUM_CONSTANT(CVAR_SET_FAILED_INITONLY);
	BIND_ENUM_CONSTANT(CVAR_SET_FAILED_READONLY);
	BIND_ENUM_CONSTANT(CVAR_SET_FAILED_DEVONLY);
}

void CvarSys::Init()
{
	//cvars = memnew HashMap(CVAR_MAX);
	cvars.reserve( CVAR_FLAGS_MAX );

	//TODO: Do Initial Setup.
	cvarDevMode = Get("sv_devmode", "1", ( CVAR_SHAREDINFO | CVAR_READONLY ) );

	//TODO: We need to hook up set and list functionality that can be called from the dev console.
}

void CvarSys::Destroy()
{
	HashMap<StringName, CvarData *>::Iterator iCvar_ptr = cvars.begin();
	while (iCvar_ptr) {
		memdelete(iCvar_ptr->value);
		++iCvar_ptr;
	}

	cvars.clear();
}

CvarData *CvarSys::Add(StringName name, String defVal, uint16_t flags) {

	if (cvars.size() >= CVAR_FLAGS_MAX)
	{
		ERR_FAIL_COND_V_EDMSG(true, NULL ,"Cannot add cvar " + name + " as it exceeds max of " + CVAR_FLAGS_MAX + " cvars.");
	}

	if (cvars.has(name))
	{
		ERR_FAIL_COND_V_EDMSG(true, NULL, "Cannot add cvar " + name + ", it already exists.");
	}

	//TODO: Make sure we delete this allocation!
	CvarData *newCvar_ptr = memnew(CvarData(name, defVal, flags));
	cvars.insert(name, newCvar_ptr);
	return newCvar_ptr;
}

//TODO: DO WE NEED THIS? SHOULD WE EVER BE REMOVING CVARS BEFORE WE DESTROY THIS SYSTEM?
void CvarSys::Remove(StringName name)
{
	if (cvars.has(name))
	{
		CvarData *cvar_ptr = cvars.get(name);
		memdelete(cvar_ptr);
		cvars.erase(name);
	}
	else
	{ 
		ERR_FAIL_COND_MSG(true, "Cannot remove cvar " + name + " because it does not exists.");
	}
}

CvarData *CvarSys::Get(StringName name, String defVal, uint16_t flags) {

	//TODO: Ensure that we can only access cvars based on how flags allow access.
	//TODO: Make sure that we can't alter the cvar by modifying the pointer.

	//Return Cvar Data, if it does not exist, create it and return new cvar.
	if (cvars.has(name) )
	{
		//TODO: Set latch variable
		CvarData *cvar_ptr = Find(name);

		//Or in get flags.
		//Decide if we want this.
		cvar_ptr->flags |= flags;

		if (cvar_ptr->flags & CVAR_ONRESTART)
		{
			//TODO: Do we need to check other flags here like READYONLY, INITONLY, DEVONLY?
			String latchString = cvar_ptr->latchString;
			cvar_ptr->stringVal	= cvar_ptr->latchString;
			cvar_ptr->latchString = "";
			cvar_ptr->intVal = cvar_ptr->stringVal.to_int();
			cvar_ptr->floatVal = cvar_ptr->stringVal.to_float();
			cvar_ptr->modified = true;
		}

		return cvar_ptr;
	}

	return Add( name, defVal, flags ) ;
}

CvarSys::CvarSetResult CvarSys::Set(StringName name, String val) {
	if (cvars.has(name))
	{
		CvarData *cvar_ptr = cvars.get(name);
		//TODO: Set cvars here.
		if ( cvar_ptr->flags & CVAR_INITONLY )
		{
			//TODO: Read these results and report to console that set failed because it is read only or init only.
			return CVAR_SET_FAILED_INITONLY;

		}
		else if (cvar_ptr->flags & CVAR_READONLY)
		{
			return CVAR_SET_FAILED_READONLY;
		}
		
		//Ensure that we only set devmode variables if devmode is enabled.
		if (cvar_ptr->flags & CVAR_DEVONLY)
		{
			if (cvarDevMode == NULL) {
				ERR_FAIL_COND_V_EDMSG(true, CVAR_SET_FAILED_DEVONLY, "Cvar " + name + " is flagged as devonly, but devmode cvar state is invalid.");
			}

			if (cvarDevMode->intVal <= 0)
			{
				return CVAR_SET_FAILED_DEVONLY;
			}
		}

		//Set the types of flags that have changed so that we can handle these changes in systems that are listening.
		cvarChangedFlags |= cvar_ptr->flags;

		//TODO: Actually set the cvars here!
		if (cvar_ptr->flags & CVAR_ONRESTART)
		{
			//This will be set the next time get is called for the cvar. (i.e. on level restart) 
			cvar_ptr->latchString = val;
			cvar_ptr->modified = true;
			return CVAR_SET_SUCCESS_ONRESTART; 
		}
		else
		{
			cvar_ptr->stringVal = val;
			cvar_ptr->intVal = val.to_int();
			cvar_ptr->floatVal = val.to_float();
			cvar_ptr->modified = true;
			return CVAR_SET_SUCCESS;
		}
	}

	//TODO: if cvar is server, client, or shared. Update networking.
	//TODO: ensure that only server can set server and shared cvars, and that only client can set client cvars.
	
	ERR_FAIL_COND_V_EDMSG(true, CVAR_SET_FAILED_INVALID, "Cannot set cvar " + name + " because it does not exists.");
}

//Checks the cvar cvar data without being considered a get call for the purposes of onreset flags.
CvarData *CvarSys::Peek(StringName name) {
	//TODO: Ensure that we can only access cvars based on how flags allow access.
	//TODO: Make sure that we can't alter the cvar by modifying the pointer.

	if (cvars.has(name)) {
		//TODO: Set latch variable
		CvarData *cvar_ptr = Find(name);
		return cvar_ptr;
	}

	return NULL;
}

//Do we need these?
bool CvarSys::Exists(StringName name)
{
	return cvars.has(name);
}

CvarData *CvarSys::Find(StringName name)
{
	if (cvars.has(name)) {
		return cvars.get(name);
	}

	return NULL;
}

void CvarSys::Save()
{
	//TODO: Write all CVAR_SAVE flagged cvars to external file.
}

void CvarSys::Load()
{
	//TODO: Read all CVAR_SAVE flagged cvars from external file.
}

void CvarSys::ToDefaults()
{
	//TODO: Reset call cvars to default values.
	//Potentially allow a category of cvar that ignores the default reset.

	HashMap<StringName, CvarData *>::Iterator iCvar_ptr = cvars.begin();
	while (iCvar_ptr)
	{
		CvarData *cvar_ptr = iCvar_ptr->value;
		if (!(iCvar_ptr->value->flags & ( CVAR_NORESET | CVAR_READONLY | CVAR_INITONLY ) ) )
		{
			cvar_ptr->stringVal		= cvar_ptr->resetString;
			cvar_ptr->latchString	= "";
			cvar_ptr->intVal		= cvar_ptr->stringVal.to_int();
			cvar_ptr->floatVal		= cvar_ptr->stringVal.to_float();
			cvar_ptr->modified		= false;
		}

		++iCvar_ptr;
	}

}

CvarSys::CvarSys()
{
	ERR_FAIL_COND_MSG(singleton, "Singleton for CvarSys already exists.");

	Init();
	singleton = this;
}

CvarSys::~CvarSys()
{
	Destroy();
	singleton = nullptr;
	ERR_PRINT_ONCE_ED("Cvar Sys Destroyed");
}

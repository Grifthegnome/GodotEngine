#include "cvar.h"
#include "scene\main\multiplayer_api.h"

//Look at add_autoload_singleton in editor_plugin.cpp

//////////////////////
// CVAR GLOBAL NODE //
//////////////////////

void Cvar::_bind_methods() {

	//TODO: We need to bind our struct so we can return it.
	//Look at intersect_ray to see how it returns a dictionary.

	ClassDB::bind_method(D_METHOD("cvar_get", "name", "default_value", "flags"), &Cvar::get);
	ClassDB::bind_method(D_METHOD("cvar_set", "name", "value" ), &Cvar::set);
	ClassDB::bind_method(D_METHOD("cvar_peek","name"), &Cvar:: peek);

	//TODO: Should this be exposed to GDScript?
	ClassDB::bind_method(D_METHOD("cvar_to_defaults"), &Cvar::to_defaults);
}
/*
* Returns the value of the given cvar and creates with specified default value if it does not exist.
*/
Dictionary Cvar::get(const StringName &p_name, const String &p_defVal, int p_flags )
{
	CvarData *cvar_ptr = CvarSys::get_singleton()->Get(p_name, p_defVal, p_flags);

	if (cvar_ptr) {

		//DEVNOTE: Make sure to update this if we add variables to CvarData.
		Dictionary cvar;
		cvar["name"] = cvar_ptr->name;
		cvar["stringVal"] = cvar_ptr->stringVal;
		cvar["resetString"] = cvar_ptr->resetString;
		cvar["latchString"] = cvar_ptr->latchString;
		cvar["intVal"] = cvar_ptr->intVal;
		cvar["floatVal"] = cvar_ptr->floatVal;
		cvar["flags"] = cvar_ptr->flags;
		cvar["modified"] = cvar_ptr->modified;

		return cvar;
	}

	return Dictionary();
}

/*
 * Sets the value of the given cvar.
 */
CvarSys::CvarSetResult Cvar::set(const StringName &p_name, const String &p_val) {
	CvarSys *cvarSys = CvarSys::get_singleton();
	if (cvarSys->Exists(p_name))
	{
		return cvarSys->Set(p_name, p_val);
	}

	return CvarSys::CVAR_SET_FAILED_INVALID;
	
}

/*
 * Checks the value of the given cvar without running any additional logic.
 */
Dictionary Cvar::peek(const StringName &p_name) {
	CvarData *cvar_ptr = CvarSys::get_singleton()->Peek(p_name);

	if (cvar_ptr) {
		//DEVNOTE: Make sure to update this if we add variables to CvarData.
		Dictionary cvar;
		cvar["name"] = cvar_ptr->name;
		cvar["stringVal"] = cvar_ptr->stringVal;
		cvar["resetString"] = cvar_ptr->resetString;
		cvar["latchString"] = cvar_ptr->latchString;
		cvar["intVal"] = cvar_ptr->intVal;
		cvar["floatVal"] = cvar_ptr->floatVal;
		cvar["flags"] = cvar_ptr->flags;
		cvar["modified"] = cvar_ptr->modified;

		return cvar;
	}

	return Dictionary();
}

void Cvar::to_defaults()
{
	return CvarSys::get_singleton()->ToDefaults();
}

//TODO: We need a way for CvarSys to talk to this node.
	//The node itself shouldn't know anything about the CVARS it should only read, write, and network them as needed.
void Cvar::net_update()
{

	/*
	TODO: batch all of the cvars that have changed in a frame and send them as a single packet to avoid spamming the connection.
	TODO: we should assign all networked convars a net ID handle so that we can send that, rather than a string, to identify them.
	TODO: see if there is a way to network this stuff without even going through a node.
	*/

	this->get_multiplayer();
	//TODO: Network Cvars as needed.

	//Engine::

	//MultiplayerAPI::get_multiplayer_peer();

	//TODO: Figure out how to get the network API so that we can broadcast cvars.
	//MultiplayerAPI *multiplayer = MultiplayerAPI;
}

/*
void Cvar::set_multiplayer_authority(int p_peer_id, bool p_recursive = true)
{
	ERR_FAIL_COND_MSG(true, "cvar authority must be server authoritative.");

	//Server is always authoratiative with CVAR.
	//set_multiplayer_authority(1);
}
*/


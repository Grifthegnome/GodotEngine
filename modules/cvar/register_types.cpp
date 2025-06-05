#include "register_types.h"

#include "core\os\memory.h"
#include "core/object/class_db.h"
#include "cvarsys.h"
#include "cvar.h"

void initialize_cvar_module(ModuleInitializationLevel p_level)
{
	//TODO: Figure out where would be best to init this.
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		return;
	}

	ClassDB::register_class<CvarSys>();
	ClassDB::register_class<Cvar>();

	//TODO: Make sure this is getting cleaned up, it should be in ~CvarSys().
	CvarSys *cvarSys = memnew(CvarSys());
	Engine *engine = Engine::get_singleton();
	engine->add_singleton(Engine::Singleton("CvarSys", cvarSys ));
}

void uninitialize_cvar_module(ModuleInitializationLevel p_level)
{
	ERR_PRINT_ONCE_ED("Trying to uninitialize cvar sys.");
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		return;
	}

	ERR_PRINT_ONCE_ED("Getting cvar sys singleton");

	Object *cvarSysObj = Engine::get_singleton()->get_singleton_object("CvarSys");
	CvarSys *cvarSys = Object::cast_to<CvarSys>(cvarSysObj);

	//TODO: Cleanup cvar data.
	Engine::get_singleton()->remove_singleton("CvarSys");

	if ( cvarSys ) {
		ERR_PRINT_ONCE_ED("Singleton is valid, attempting delete.");
		memdelete(cvarSys);
	}
	else
	{
		ERR_PRINT_ONCE("Cannot destroy CvarSys singleton because it is invalid.");
	}


}

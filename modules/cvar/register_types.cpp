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
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		return;
	}

	//TODO: Cleanup cvar data.
	Engine::get_singleton()->remove_singleton("CvarSys");
}

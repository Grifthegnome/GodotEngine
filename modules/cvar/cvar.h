#ifndef CVARS_H
#define CVARS_H

#include "cvarsys.h"
#include "scene/main/node.h"

class Cvar : public Node {
	GDCLASS(Cvar, Node);

	static void _bind_methods();

	public:
		Dictionary get(const StringName &p_name, const String &p_defVal, int p_flags);
		CvarSys::CvarSetResult set(const StringName &p_name, const String &p_val);

		Dictionary peek(const StringName &p_name);

		//Resets all convars to their default values.
		void to_defaults();

	private:
		void net_update();
		//void set_multiplayer_authority(int p_peer_id, bool p_recursive = true);
};


#endif //CVARS_H

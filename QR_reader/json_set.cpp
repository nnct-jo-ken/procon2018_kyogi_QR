#include "json_set.h"

void object_init(json *creating_target, unsigned int object_no) {
	json create = *creating_target;

	create["sy"][object_no]["sy_id"] = object_no;
	create["sy"][object_no]["points"] = json::array();
	create["sy"][object_no]["synthesis"]["source"]["sy_id"] = nullptr;
	create["sy"][object_no]["synthesis"]["source"]["num"] = nullptr;
	create["sy"][object_no]["synthesis"]["source"]["dir"] = nullptr;
	create["sy"][object_no]["synthesis"]["target"]["sy_id"] = nullptr;
	create["sy"][object_no]["synthesis"]["target"]["num"] = nullptr;
	create["sy"][object_no]["synthesis"]["target"]["dir"] = nullptr;

	*creating_target = create;
}

void node_set(json *set_target, unsigned int object_no, Array<Vec2> node) {
	int i;
	json set = *set_target;
	
	for (i = 0; i < node.size(); i++) {
		set["sy"][object_no]["points"][i]["x"] = (int)node[i].x;
		set["sy"][object_no]["points"][i]["y"] = (int)node[i].y;
	}

	*set_target = set;
}
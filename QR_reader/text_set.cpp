#include "text_set.h"

void node_set(TextWriter info, Array<String> node) {
	String buf;

	for (const auto vertex : node) {
		buf = vertex.substr(vertex.indexOf(L" ") + 1);
		info.writeln(buf);
	}
}
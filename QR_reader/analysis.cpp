#include "analysis.h"

Array<Vec2> get_node(String piece) {
	Array<Vec2> saving_target;
	int begin = piece.indexOf(L' ') + 1;
	int prev = begin;
	Vec2 node;

	for (int i = 0; i < FromString<int>(piece.substr(0, begin - 1)); i++) {
		node.x = FromString<int>(piece.substr(prev, piece.indexOf(L' ', prev)));
		prev = piece.indexOf(L' ', prev) + 1;
		node.y = FromString<int>(piece.substr(prev, piece.indexOf(L' ', prev)));
		prev = piece.indexOf(L' ', prev) + 1;

		saving_target.push_back(node);
	}

	return saving_target;
}

Array<String> get_pieceinfo(Array<String> decoded){
	Array<String> acquisition_data;
	int begin = 0, end = 0;

	for (const auto qr : decoded) {
		begin = qr.indexOf(L':') + 1;
		int count = FromString<int>(qr.substr(0, qr.indexOf(L':')));
		if (qr == decoded.back()) {
			count++;
		}
		for (int i = 0; i < count; i++) {
			end = qr.indexOf(L':', begin);
			acquisition_data.push_back(qr.substr(begin, end - begin));
			begin = end + 1;
		}
	}

	return acquisition_data;
}

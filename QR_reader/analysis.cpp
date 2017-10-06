#include "analysis.h"

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

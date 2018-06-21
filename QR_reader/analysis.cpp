#include "analysis.h"

// �f�[�^�̐��`
Array<String> get_pieceinfo(Array<String> decoded){
	Array<String> acquisition_data;
	int begin = 0, end = 0;

	for (const auto qr : decoded) {	//decoded�z��̗v�f������qr�ɑ��
		begin = qr.indexOf(L':') + 1;	//�ŏ���':'������鎟�̕�����begin�ɐݒ�
		int count = FromString<int>(qr.substr(0, qr.indexOf(L':')));	//�ŏ���':'�܂ł̕����𐔎��ɕϊ�����count�ɑ��
		if (qr == decoded.back()) {
			count++;
		}
		for (int i = 0; i < count; i++) {
			end = qr.indexOf(L':', begin);	//begin�̈ʒu����':'������܂ŒT��.��������A���̏ꏊ��end�ɓ����
			acquisition_data.push_back(qr.substr(begin, end - begin));	//begin����end�܂ł̕������z��ɓ���
			begin = end + 1;	//begin��end�̎���
		}
	}

	return acquisition_data;
}

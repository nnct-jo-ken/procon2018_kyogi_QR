#include "text_set.h"

void node_set(TextWriter info, Array<String> node) {	//node�ɂ͐��`�ς݃f�[�^������
	String buf;

	for (const auto vertex : node) {
		//buf = vertex.substr(vertex.indexOf(L" ") + 1);
		if (vertex.front() == ' ') {	//�Ȃ������p�R�����̌��ɃX�y�[�X���܂�ł���Ƃ��̃X�y�[�X����
			buf = vertex.substr(1);	//2�Ԗڂ̕�������Ō�܂Łi�Ō�͏ȗ��j
		}
		else {
			buf = vertex;
		}
		info.writeln(buf);	//���ۂɃt�@�C���ւ̏������ݏ����H
	}
}
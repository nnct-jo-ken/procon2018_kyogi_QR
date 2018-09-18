﻿# include <Siv3D.hpp>
#include <fstream>
#include"include.h"

void Main() {
	Window::Resize(900, 600);

	GUI message(GUIStyle::Default);	//情報ウィンドウ
	message.setTitle(L"メッセージ");
	message.setPos(650, 10);
	message.add(L"tf1", GUITextField::Create(none));

	GUI confirmation(GUIStyle::Default);	//確認ウィンドウ
	confirmation.setTitle(L"これで良いですか?");
	confirmation.add(L"yes", GUIButton::Create(L"Yes"));
	confirmation.add(L"no", GUIButton::Create(L"No"));
	confirmation.setPos(650, 100);
	confirmation.show(false);

	Webcam webcam;

	if (!webcam.open(0, Size(640, 480))) {
		return;
	}

	if (!webcam.start()) {
		return;
	}

	Image image;
	DynamicTexture texture;

	std::string binary;
	Array<String> decoded;

	TextWriter info;

	QRData data;

	while (System::Update()) {
		Optional<Quad> quad;

		if (webcam.hasNewFrame()) {
			message.textField(L"tf1").setText(L"読み取り中");
			webcam.getFrame(image);

			texture.fill(image);
			
			if (decode(image, data)) {
				webcam.pause();
				binary.resize(data.data.size());
				data.data.read(&binary[0], data.data.size());
				decoded.push_back(Widen(binary));
			}
		}

		if (!webcam.isActive() && webcam.isOpened()) {
			message.textField(L"tf1").setText(L"認識しました");
			quad = data.quad;

			confirmation.show(true);
			if (confirmation.button(L"yes").pressed) {
				info = TextWriter(L"shape_info.txt");

				confirmation.show(false);
				webcam.close();

				node_set(info, get_pieceinfo(decoded));

				info.close();
				message.textField(L"tf1").setText(L"出力しました");
			}
			if (confirmation.button(L"no").pressed) {
				confirmation.show(false);
				webcam.resume();
				
				do {
					webcam.getFrame(image);					
				} while (decode(image, data));
			}
		}

		if (texture) {
			texture.draw();
		}

		if (quad)
		{
			quad->drawFrame(4, Palette::Red);
		}
	}
}
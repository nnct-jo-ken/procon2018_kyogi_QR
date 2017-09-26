# include <Siv3D.hpp>
#include <fstream>
#include<boost\asio.hpp>
#include"include.h"

void Main() {
	Window::Resize(1280, 960);
	GUI confirmation(GUIStyle::Default);
	confirmation.setTitle(L"読み取りを続けますか?");
	confirmation.add(L"yes", GUIButton::Create(L"Yes"));
	confirmation.add(L"no", GUIButton::Create(L"No"));
	confirmation.setPos(650, 10);
	confirmation.show(false);

	GUI selection(GUIStyle::Default);
	selection.setTitle(L"形式?");
	selection.add(L"category", GUIRadioButton::Create({ L"形状情報",L"配置情報" }, 0u));
	selection.setPos(650, 10);
	selection.show(false);

	Webcam webcam;

	if (!webcam.open(0, Size(640, 480))) {
		return;
	}

	if (!webcam.start()) {
		return;
	}

	selection.show();

	Image image;
	DynamicTexture texture;

	std::string binary;
	Array<String> decoded;

	json piece;
	int piece_count = 0;
	int i = 0;

	std::ofstream jsonfile("data.json");

	QRData data;

	while (System::Update()) {
		PutText(upload(piece)).from(200, 100);
		if (webcam.hasNewFrame()) {
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
			selection.show(false);
			confirmation.show(true);
			if (confirmation.button(L"yes").pressed) {
				confirmation.show(false);
				webcam.resume();
				
				do {
					webcam.getFrame(image);					
				} while (decode(image, data));
			}
			if (confirmation.button(L"no").pressed) {
				confirmation.show(false);
				webcam.close();
				for (int i = 0; i < decoded.size(); i++) {
					piece_count += FromString<int>(decoded[i].substr(0, decoded[i].indexOf(L':')));
				}
				if (selection.radioButton(L"category").checked(0)) {
					piece_count++;
				}
				piece["piece_count"] = piece_count;
				for (int i = 0; i < piece_count; i++) {
					object_init(&piece, i);
				}
				for (const auto data : get_pieceinfo(decoded)) {
					node_set(&piece, i, get_node(data));
					i++;
				}
				jsonfile << std::setw(2) << piece << std::endl;
			}
		}

		if (texture) {
			texture.draw();
		}
	}
}
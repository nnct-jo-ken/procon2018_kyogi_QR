# include <Siv3D.hpp>
#include <fstream>
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
	selection.add(L"category", GUIRadioButton::Create({ L"形状情報",L"配置情報" }));
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

	TextWriter info;

	QRData data;

	while (System::Update()) {
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
				if (selection.radioButton(L"category").checkedItem == 0u) {
					info = TextWriter(L"//169.254.79.134/Share/shape_info.txt");
				}
				else if (selection.radioButton(L"category").checkedItem == 1u) {
					info = TextWriter(L"//169.254.79.134/Share/allocation_info.txt");
				}

				confirmation.show(false);
				webcam.close();

				node_set(info, get_pieceinfo(decoded));

				info.close();
			}
		}

		if (texture) {
			texture.draw();
		}
	}
}
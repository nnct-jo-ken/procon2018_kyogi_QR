# include <Siv3D.hpp>
#include <fstream>
#include"include.h"

void Main() {
	Window::Resize(1280, 960);
	GUI gui(GUIStyle::Default);
	gui.setTitle(L"読み取りを続けますか?");
	gui.add(L"yes", GUIButton::Create(L"Yes"));
	gui.add(L"no", GUIButton::Create(L"No"));
	gui.setPos(650, 10);
	gui.show(false);

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

	json piece = json::array();
	int piece_count = 0;

	std::ofstream jsonfile("data.json");

	while (System::Update()) {
		if (webcam.hasNewFrame()) {
			webcam.getFrame(image);

			texture.fill(image);

			QRData data;
			
			if (decode(image, data)) {
				webcam.pause();
				binary.resize(data.data.size());
				data.data.read(&binary[0], data.data.size());
				decoded.push_back(Widen(binary));
			}
		}

		if (!webcam.isActive() && webcam.isOpened()) {
			gui.show(true);
			if (gui.button(L"yes").pressed) {
				gui.show(false);
				webcam.resume();
				for (int i = 0; i < 10; i++) {
					webcam.getFrame(image);
				}
			}
			if (gui.button(L"no").pressed) {
				gui.show(false);
				webcam.close();
				for (int i = 0; i < decoded.size(); i++) {
					piece_count += FromString<int>(decoded[i].substr(0, decoded[i].indexOf(L':')));
				}
				for (int i = 0; i < piece_count; i++) {
					object_init(&piece, i);
				}
				jsonfile << std::setw(2) << piece << std::endl;
			}
		}

		if (texture) {
			texture.draw();
		}
	}
}
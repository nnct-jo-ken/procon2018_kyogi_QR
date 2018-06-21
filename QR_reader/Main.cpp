# include <Siv3D.hpp>
#include <fstream>
#include"include.h"

void Main() {
	Window::Resize(900, 600);
	GUI confirmation(GUIStyle::Default);	//読み取りを続けるのか確認ウィンドウ
	confirmation.setTitle(L"読み取りを続けますか?");
	confirmation.add(L"yes", GUIButton::Create(L"Yes"));
	confirmation.add(L"no", GUIButton::Create(L"No"));
	confirmation.setPos(650, 10);
	confirmation.show(false);

	GUI selection(GUIStyle::Default);	//どれを読み込むのか確認ウィンドウ
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
		Optional<Quad> quad;

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
			quad = data.quad;

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
				if (selection.radioButton(L"category").checked(0u)) {
					info = TextWriter(L"shape_info.txt");
				}
				else if (selection.radioButton(L"category").checked(1u)) {
					info = TextWriter(L"allocation_info.txt");
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

		if (quad)
		{
			quad->drawFrame(4, Palette::Red);
		}
	}
}
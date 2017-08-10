# include <Siv3D.hpp>

void Main() {
	Webcam webcam;

	if (!webcam.open(0, Size(640, 480))) {
		return;
	}

	if (!webcam.start()) {
		return;
	}

	Image image;
	DynamicTexture texture;

	while (System::Update()) {
		if (webcam.hasNewFrame()) {
			//webcam.getFrame(image);
			image = Image(L"../qr_1.jpg");

			texture.fill(image);

			QRData data;

			if (QR::Decode(image, data)) {
				switch (data.mode)
				{
				case QREncodingMode::Numeric:
					Println(L"Numeric: ", data.text);
					break;
				case QREncodingMode::Alnum:
					Println(L"Alnum: ", data.text);
					break;
				case QREncodingMode::Kanji:
					Println(L"Kanji: ", data.text);
					break;
				case QREncodingMode::Binary:
					std::string binary;
					binary.resize(data.data.size());
					data.data.read(&binary[0], data.data.size());
					Println(L"Binary: ", data.data.asArray());
					Println(L"(Widen): ", Widen(binary));
					Println(L"(UTF8): ", FromUTF8(binary));
					break;
				}
			}
		}

		if (texture) {
			texture.draw();
		}
	}
}
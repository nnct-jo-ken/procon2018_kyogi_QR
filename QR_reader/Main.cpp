# include <Siv3D.hpp>
#include <quirc.h>

QREncodingMode ConvertEncodingMode(int qdata) {
	switch (qdata) {
	case 0:
		return QREncodingMode::Numeric;
	case 1:
		return QREncodingMode::Alnum;
	case 2:
		return QREncodingMode::Binary;
	case 3:
		return QREncodingMode::Kanji;
	case 4:
		return QREncodingMode::Unknown;
	}
}

bool decode(const Image& image, QRData& data, TextWriter datafile) {
	struct quirc *m_quirc = quirc_new();

	if (!image || !m_quirc) {
		return false;
	}

	if (quirc_resize(m_quirc, image.width, image.height)) {
		return false;
	}

	uint8* p = quirc_begin(m_quirc, nullptr, nullptr);

	for (const auto& pixel : image) {
		*(p++) = pixel.grayscale();
	}

	quirc_end(m_quirc);

	const int num_codes = quirc_count(m_quirc);

	if (!num_codes) {
		return false;
	}

	quirc_code qcode;
	quirc_data qdata;
	quirc_decode_error_t qerr;

	quirc_extract(m_quirc, 0, &qcode);

	qerr = quirc_decode(&qcode, &qdata);

	if (qerr) {// ここでエラー
		//return false;
	}

	for (size_t i = 0; i < 4; ++i) {
		data.quad.p[i].set(qcode.corners[i].x, qcode.corners[i].y);
	}

	data.mode = ConvertEncodingMode(qdata.data_type);
	data.ecLevel = static_cast<QRECLevel>(qdata.ecc_level);
	data.version = qdata.version;

	if (data.mode == QREncodingMode::Numeric || data.mode == QREncodingMode::Alnum) {
		data.text = CharacterSet::WidenAscii(std::string(static_cast<const char*>(static_cast<const void*>(qdata.payload)), qdata.payload_len));
	}
	else if (data.mode == QREncodingMode::Kanji) {
		data.text = CharacterSet::Widen(std::string(static_cast<const char*>(static_cast<const void*>(qdata.payload)), qdata.payload_len));
	}
	else {
		data.text = CharacterSet::FromUTF8(std::string(static_cast<const char*>(static_cast<const void*>(qdata.payload)), qdata.payload_len));
	}

	data.data = ByteArray(qdata.payload, qdata.payload_len);

	quirc_destroy(m_quirc);

	return true;
}

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

	//image = Image(L"../qr_1.jpg");

	TextWriter datafile(L"data.txt");

	while (System::Update()) {
		if (webcam.hasNewFrame()) {
			webcam.getFrame(image);

			texture.fill(image);

			QRData data;

			if (decode(image, data, datafile)) {
				switch (data.mode) {
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
					datafile.writeln(L"Binary: ", data.data.asArray());
					datafile.writeln(L"(Widen): ", Widen(binary));
					datafile.writeln(L"(UTF8): ", FromUTF8(binary));
					break;
				}
			}
		}

		if (texture) {
			texture.draw();
		}

		if (Input::KeyC.clicked) {
			ClearPrint();
		}
	}
}
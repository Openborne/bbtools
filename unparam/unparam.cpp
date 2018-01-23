#include <iostream>
#include <fstream>
#include <codecvt>
#include <locale>
#include <cstdlib>
#include <string>
#include <ios>
#include <vector>

#define Progname "unparam"

namespace Bit {
	struct Skip {
		size_t amount;
		explicit Skip(size_t n) : amount(n) { }
	};

	template<typename T, typename... Targs> 
	void unpack(std::ifstream& in, T& u, Targs&&... tail);

	std::string string(std::ifstream& in, size_t len, bool norm)
	{
		(void) norm;
		std::string res;
		for (size_t i = 0; i < len; i++) {
			uint8_t c;
			unpack(in, c);
			if (c == '\0')
				continue;
			res += c;
		}
		return res;
	}

	std::string string(std::ifstream& in, size_t len)
	{
		char16_t c;
		std::string res;

		for (size_t i = 0; i < len; i++) {
			unpack(in, c);
			if (c == '\0')
				continue;
			std::wstring_convert<std::codecvt_utf8<char32_t>,
					     char32_t> w32;
			res += w32.to_bytes(c);
		}

		return res;
	}

	std::string string(std::ifstream& in)
	{
		char16_t c;
		std::string res;
		
		for (;;) {
			unpack(in, c);
			if (c == '\0')
				break;
			std::wstring_convert<std::codecvt_utf8<char32_t>,
					     char32_t> w32;
			res += w32.to_bytes(c);
		}

		return res;
	}

	template<typename T>
	void ule(std::ifstream& in, T& t)
	{
		t = T();
		char tmp[sizeof(T)];
		uint8_t* data;

		in.read(tmp, sizeof tmp);
		data = reinterpret_cast<uint8_t*>(tmp);
		for (size_t i = sizeof(T)-1; i > 0; i--)
			t |= data[i] << (i * 8);
		t |= *data;
	}

	void unpack(std::ifstream& in)
	{
		(void) in;
	}

	template<typename T, typename... Targs>
	void unpack(std::ifstream& in, T& u, Targs&&... tail)
	{
		ule(in, u);
		unpack(in, std::forward<Targs>(tail)...);
	}

	template<typename... Targs>
	void unpack(std::ifstream& in, Skip s, Targs&&... tail)
	{
		in.ignore(s.amount);
		unpack(in, std::forward<Targs>(tail)...);
	}
}

struct Paramdef {
	struct Entry {
		std::string description, type, print;
		uint32_t display, size, infoaddr;
		std::string type2, name;
	};

	uint32_t filelen;
	uint16_t unk1, unk2, numparam, paramdeflen;
	std::string title;
	uint32_t unk3, firstaddr;

	std::vector<Entry> entries;

	std::vector<Entry> data()
	{
		return entries;
	}

	void read(std::ifstream& in)
	{
		Bit::unpack(in, filelen, unk1, unk2, numparam,
			    paramdeflen);
		title = Bit::string(in, 32, true);
		Bit::unpack(in, unk3, firstaddr, Bit::Skip(4));

		entries.reserve(numparam);
		for (size_t i = 0; i < numparam; i++) {
			Entry e;

			e.description = Bit::string(in, 32);
			e.type = Bit::string(in, 8, true);
			e.print = Bit::string(in, 10, true);
			Bit::unpack(in, Bit::Skip(14), e.display, e.size, e.infoaddr, Bit::Skip(4));
			e.type2 = Bit::string(in, 32, true);
			e.name = Bit::string(in, 32, true);
			Bit::unpack(in, Bit::Skip(32));
			entries.push_back(e);
		}
	}
};

struct Param {
	union Float {
		uint32_t i;
		float f;
	};

	struct TypeContainer {
		std::string ref;
		uint8_t u8;
		uint16_t u16;
		uint32_t u32;
		uint64_t u64;
		int8_t i8;
		int16_t i16;
		int32_t i32;
		int64_t i64;
		Float f32;
	};

	struct Entry {
		std::string info;
		std::vector<Paramdef::Entry> entry;
		std::vector<TypeContainer> field;
	};

	uint32_t infoaddr;
	uint16_t unk1, unk2, unk3, numentry;
	std::string title;
	uint16_t unk4, unk5;
	uint64_t paramaddr;

	std::vector<Entry> entries;

	std::vector<Entry> data()
	{
		return entries;
	}

	void type(Paramdef::Entry def, TypeContainer t)
	{
		const char* s = def.print.c_str();
		int bit = atoi(t.ref.substr(1).c_str());
		switch (t.ref.at(0)) {
		case 'u':
			switch (bit) {
			case 8:
				std::printf(s, t.u8);
				break;
			case 16:
				std::printf(s, t.u16);
				break;
			case 32:
				std::printf(s, t.u32);
				break;
			case 64:
				std::printf(s, t.u64);
				break;
			}
			break;
		case 's':
			switch (bit) {
			case 8:
				std::printf(s, t.i8);
				break;
			case 16:
				std::printf(s, t.i16);
				break;
			case 32:
				std::printf(s, t.i32);
				break;
			case 64:
				std::printf(s, t.i64);
				break;
			}
			break;
		case 'f':
			switch (bit) {
			case 32:
				std::printf(s, t.f32.f);
				break;
			}
			break;
		}
	}

	void read(std::vector<Paramdef::Entry> def, std::ifstream& in)
	{
		Bit::unpack(in, infoaddr, unk1, unk2, unk3, numentry);
		title = Bit::string(in, 32, true);
		Bit::unpack(in, unk4, unk5, paramaddr, Bit::Skip(8));

		entries.reserve(numentry);
		for (size_t i = 0; i < numentry; i++) {
			Entry e;
			uint64_t entryid, paramaddr2, infoaddr;
			Bit::unpack(in, entryid, paramaddr2, infoaddr);

			size_t pos = in.tellg();
			in.seekg(infoaddr);
			e.info = Bit::string(in);
			in.seekg(paramaddr2);

			for (size_t j = 0; j < def.size(); j++) {
				TypeContainer tc;
				Paramdef::Entry pdef = def.at(j);

				e.entry.push_back(pdef);
				tc.ref = pdef.type;
				int bit = atoi(tc.ref.substr(1).c_str());
				switch (tc.ref.at(0)) {
				case 'u':
					switch (bit) {
					case 8:
						Bit::unpack(in, tc.u8);
						break;
					case 16:
						Bit::unpack(in, tc.u16);
						break;
					case 32:
						Bit::unpack(in, tc.u32);
						break;
					case 64:
						Bit::unpack(in, tc.u64);
						break;
					}
					break;
				case 's':
					switch (bit) {
					case 8:
						Bit::unpack(in, tc.i8);
						break;
					case 16:
						Bit::unpack(in, tc.i16);
						break;
					case 32:
						Bit::unpack(in, tc.i32);
						break;
					case 64:
						Bit::unpack(in, tc.i64);
						break;
					}
					break;
				case 'f':
					switch (bit) {
					case 32:
						Bit::unpack(in, tc.f32.i);
						break;
					}
					break;
				}

				if (tc.ref == "dummmy8")
					Bit::unpack(in, Bit::Skip(1));
				e.field.push_back(tc);
			}

			in.seekg(pos);
			entries.push_back(e);
		}
	}
};

int main(int argc, char* argv[])
{
	Paramdef def;
	Param param;
	std::string progname = argv[0][0] == '\0' ? Progname
			     : argv[0];
	if (argc < 3) {
		std::cerr << "usage: " << progname << " <paramdef> <param>" << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream defin(argv[1], std::ios::in | std::ios::binary);

	if (!defin.is_open()) {
		std::cerr << progname << ": could not open '" << argv[1] << "'" << std::endl;
		return EXIT_FAILURE;
	}

	def.read(defin);
	std::ifstream paramin(argv[2], std::ios::in | std::ios::binary);

	std::cout << "info" << std::endl << "name,type,fmt,value" << std::endl;
	if (!paramin.is_open()) {
		std::cerr << progname << ": could not open '" << argv[2] << "'" << std::endl;
		return EXIT_FAILURE;
	}

	param.read(def.data(), paramin);
	for (const auto &i : param.data()) {
		std::cout << i.info << std::endl;
		for (size_t j = 0; j < i.entry.size(); j++) {
			Paramdef::Entry def = i.entry.at(j);
			std::cout << def.name << "," << def.type << "," << def.print << ",";
			param.type(def, i.field.at(j));
			std::cout << std::endl;
		}
	}
	return 0;
}

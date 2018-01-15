#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ios>

#define Progname "tpf2dds"

using namespace std;

enum {
	In, Out
};

struct Tpf {
	template<typename T>
	T getu(ifstream& in)
	{
		T res;
		char tmp[sizeof(T)];
		unsigned char* buf;

		in.read(tmp, sizeof tmp);
		buf = reinterpret_cast<unsigned char*>(tmp);
		for (T i = res = 0; i < sizeof(T); i++)
			res |= buf[i] << (i * 8);
		return res;
	}

	template<typename T>
	void putu(ofstream& out, T c)
	{
		for (T i = 0; i < sizeof(T); i++)
			out.put(c >> (i * 8) & 0xff);
	}

	string gets(ifstream& in, unsigned n)
	{
		char *buf = new char[n];
		in.read(buf, n);
		string res(buf);
		delete []buf;
		return res;
	}

	string gets(ifstream& in)
	{
		int state;
		unsigned char c;
		string res;
		for (state = Out;;) {
			c = getu<unsigned char>(in);
			if (state == Out && c == '\0') {
				state = In;
				continue;
			} else if (c == '\0')
				break;
			res += c;
			state = Out;
		}
		return res;
	}

	vector<uint32_t> unk1, unk2, unk3, unk7;
	vector<uint16_t> unk4, unk5, unk6;
	uint32_t texcount;
	uint32_t mx = 0;
	uint32_t my = 0;

	void mapblock(int x, int y, int w, int bx)
	{
		int num = bx / 2;
		int num2 = x / bx;
		int num3 = y / num;
		int num4 = x % bx;
		int num5 = y % num;
		int num6 = w / bx;
		int num7 = 2 * num6;
		int num8 = num2 + (num3 * num6);
		int num9 = num8 % num7;
		int num10 = (num9 / 2) + ((num9 % 2) * num6);
		int num11 = ((num8 / num7) * num7) + num10;
		int num12 = num11 % num6;
		int num13 = num11 / num6;
		mx = (num12 * bx) + num4;
		my = (num13 * num) + num5;
	}

	int convert(ifstream& in, string path);
};

int Tpf::convert(ifstream& in, string path)
{
	int bpp[] = {
		0, 0x80, 0x80, 0x80, 0x80, 0x60, 0x60, 0x60, 0x60, 0x40,
		0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
		0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20, 0x20,
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
		0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
		0x10, 0x10, 0x10, 0x10, 0x10, 8, 8, 8, 8, 8, 8, 1,
		0x20, 0x20, 0x20, 4, 4, 4, 8, 8, 8, 8, 8, 8, 4,
		4, 4, 8, 8, 8, 0x10, 0x10, 0x20, 0x20, 0x20, 0x20,
		0x20, 0x20, 0x20, 8, 8, 8, 8, 8, 8, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0x10
	};

	const string magic = "TPF\x00";
	if (gets(in, magic.length()+1) != magic)
		return -1;
	in.ignore(4);
	texcount = getu<uint32_t>(in);
	in.ignore(4);

	for (uint32_t i = 0; i < texcount; i++) {
		unk1.push_back(getu<uint32_t>(in));
		unk2.push_back(getu<uint32_t>(in));
		in.ignore(2);
		unk4.push_back(getu<uint16_t>(in));
		unk5.push_back(getu<uint16_t>(in));
		unk6.push_back(getu<uint16_t>(in));
		in.ignore(8);
		unk7.push_back(getu<uint32_t>(in));
		in.ignore(4);
		unk3.push_back(getu<uint32_t>(in));
	}

	for (uint32_t i = 0; i < texcount; i++) {
		uint32_t gremia, unk9, w, count;

		in.seekg((long) unk7[i], in.beg);
		string name = gets(in);
		in.seekg((long) unk1[i], in.beg);
		gremia = unk3[i] == 0x47 || unk3[i] == 0x48 ? 0x31545844
		       : unk3[i] == 0x4a ? 0x33545844
		       : unk3[i] == 0x4d ? 0x35545844
		       : unk3[i] == 0x50 ? 0x31495441
		       : unk3[i] == 0x53 ? 0x32495441
		       : 0x30315844;
		if (gremia == 0x30315844)
			name += "_dx10";
		ofstream out(path + "/" + name + ".dds",
			     ios::out | ios::binary);
		if (!out.is_open())
			return -2;
		putu<unsigned long>(out, (unsigned long) 0x20534444);
		putu<unsigned long>(out, 0x7c);
		putu<uint32_t>(out, 0x1007);
		putu<uint32_t>(out, unk6[i]);
		putu<uint32_t>(out, unk5[i]);
		putu<uint32_t>(out, unk2[i]);
		putu<uint32_t>(out, 0);
		putu<uint32_t>(out, unk4[i]);
		out.seekp(0x2cL, out.cur);
		putu<uint32_t>(out, 0x20);
		putu<uint32_t>(out, 0x4);
		putu<uint32_t>(out, gremia);
		out.seekp(0x28L, out.cur);
		if (gremia == 0x30315844) {
			putu<uint32_t>(out, unk3[i]);
			putu<uint32_t>(out, 0x3);
			putu<uint32_t>(out, 0);
			putu<uint32_t>(out, 0x1);
			putu<uint32_t>(out, 0);
		}
		vector<char> src, destn;
		unk9 = unk6[i] / 4, w = unk5[i] / 4;
		count = bpp[unk3[i]] * 2;

		cout << path << "/" << name << ".dds" << endl;
		destn.resize(unk2[i]);
		src.resize(count);
		for (uint32_t j = 0; j < unk9; j++)
			for (uint32_t k = 0; k < w; k++) {
				mx = k, my = j;
				uint32_t dest;
				if ((w > 1) && (unk9 > 1))
					mapblock(k, j, w, 2);
				if ((w > 2) && (unk9 > 2))
					mapblock(mx, my, w, 4);
				if ((w > 4) && (unk9 > 4))
					mapblock(mx, my, w, 8);
				dest = count * ((my * w) + mx);
				in.read(&src[0], count);
				copy(src.begin(),
				     src.end(),
				     destn.begin() + dest);
			}
		out.write(&destn[0], unk2[i]);
		out.close();
	}
	return 0;
}

int main(int argc, char *argv[])
{
	const string errtab[] = {
		"bad file format",
		"could not open path"
	};

	string progname = argv[0][0] == '\0' ? Progname
			: argv[0];
	Tpf t;
	int res;

	if (argc < 3) {
		cerr << "usage: " << progname << " <file.tpf> <dir>" << endl;
		return EXIT_FAILURE;
	}

	ifstream in (argv[1], ios::in | ios::binary);
	if (!in.is_open()) {
		cerr << progname << ": could not open file" << endl;
		return EXIT_FAILURE;
	}

	if ((res = t.convert(in, argv[2])) < 0) {
		cerr << progname << ": " << errtab[-res-1] << endl;
		return EXIT_FAILURE;
	}

	in.close();
	return 0;
}

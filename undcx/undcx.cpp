#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <ios>
#include <vector>
#include <zlib.h>

#define Progname "undcx"

namespace Bit {
	struct Skip {
		size_t amount;
		explicit Skip(size_t n) : amount(n) { }
	};

	template<typename T, typename... Targs> 
	void unpack(std::ifstream& in, T& u, Targs&&... tail);

	std::string string(std::ifstream& in, size_t len)
	{
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

	template<typename T>
	void ube(std::ifstream& in, T& t)
	{
		t = T();
		char tmp[sizeof(T)];
		uint8_t* data;

		in.read(tmp, sizeof tmp);
		data = reinterpret_cast<uint8_t*>(tmp);
		for (size_t i = 0; i < sizeof(T); i++)
			t |= data[sizeof(T)-1-i] << (i * 8);
	}

	void unpack(std::ifstream& in)
	{
		(void) in;
	}

	template<typename T, typename... Targs>
	void unpack(std::ifstream& in, T& u, Targs&&... tail)
	{
		ube(in, u);
		unpack(in, std::forward<Targs>(tail)...);
	}

	template<typename... Targs>
	void unpack(std::ifstream& in, Skip s, Targs&&... tail)
	{
		in.ignore(s.amount);
		unpack(in, std::forward<Targs>(tail)...);
	}
}

struct Dcx {
	const uint32_t hdrsz = 24;
	const uint32_t dca = 8;
	uint32_t header, uncompressed, compressed;
	uint32_t level;
	std::vector<uint8_t> data;

	int read(std::ifstream& in)
	{
		std::string magictab[] = {
			"DCX", "DCS", "DCP", "DFLT", "DCA"
		};
		unsigned long len;
		std::vector<uint8_t> cdata;
		
		if (Bit::string(in, magictab[0].length()+1) != magictab[0])
			return -1;
		Bit::unpack(in, Bit::Skip(4), header, Bit::Skip(12));
		if (header != hdrsz)
			return -2;
		if (Bit::string(in, magictab[1].length()+1) != magictab[1])
			return -1;
		Bit::unpack(in, uncompressed, compressed);
		if (Bit::string(in, magictab[2].length()+1) != magictab[2]
		 || Bit::string(in, magictab[3].length()+1) != magictab[3])
		 	return -1;
		Bit::unpack(in, Bit::Skip(4), level, Bit::Skip(15));
		if (Bit::string(in, magictab[4].length()+1) != magictab[4])
			return -1;
		Bit::unpack(in, header);
		if (header != dca)
			return -2;
		data.resize(uncompressed);
		cdata.reserve(compressed);
		cdata.insert(cdata.begin(),
			     std::istreambuf_iterator<char>(in),
			     std::istreambuf_iterator<char>());
		len = uncompressed;
		return uncompress(&data[0], &len, &cdata[0], compressed) == Z_OK ? 0 : -3;
	}
};

int main(int argc, char* argv[])
{
	Dcx dcx;
	const std::string errtab[] = {
		"",
		"invalid magic",
		"invalid header size",
		"decompression failed"
	};
	int res;
	std::string progname = argv[0][0] == '\0' ? Progname
			     : argv[0];
	if (argc < 2) {
		std::cerr << "usage: " << progname << " <infile>" << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream in(argv[1], std::ios::in | std::ios::binary);

	if (!in.is_open()) {
		std::cerr << progname << ": could not open '" << argv[1] << "'" << std::endl;
		return EXIT_FAILURE;
	}

	if ((res = dcx.read(in)) < 0) {
		std::cerr << progname << ": " << errtab[-res] << std::endl;
		return EXIT_FAILURE;
	}

	std::cout.write(reinterpret_cast<char*>(&dcx.data[0]), dcx.data.size());
	in.close();
	return 0;
}

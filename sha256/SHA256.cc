#include "SHA256.h"
#include "functions.h"

#include <cassert>
#include <filesystem>
#include <iostream>
#include <fstream>


int SHA256::ReadFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (!file.is_open())
	{
		std::cout << "Unable to open file" << std::endl;
		return 1;
	}

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	initial_message = std::vector<uint8_t>(size);
	if (!file.read(reinterpret_cast<char*>(initial_message.data()), size))
	{
		std::cout << "Unable to read file" << std::endl;
		return 1;
	}

	return 0;
}

int SHA256::AddPadding()
{
	uint64_t size = initial_message.size();
	uint64_t l = size * 8;
	initial_message.push_back(0b10000000);
	uint32_t k = 448 - (static_cast<uint32_t>(l) + 1 + 7) % 512;

	for (uint32_t i = 0; i < k; i += 8)
	{
		initial_message.push_back(0b00000000);
	}

	for (int i = 63; i > -1; i -= 8)
	{
		uint8_t bits = 0;
		for (int j = 7; j > -1; j--)
		{
			uint64_t diff = i - (7 - j);		
			uint64_t nth_bit = (bool)(l & (uint64_t(1) << diff));
			bits |= nth_bit << j;
		}
		initial_message.push_back(bits);
	}
		
	return 0;
}

int SHA256::CreateWords()
{
	for (int i = 0; i < initial_message.size(); i += 4)
	{
		word new_word = 0;

		for (int j = i; j < i + 4 && initial_message.size(); j++)
		{
			
			uint8_t bits = initial_message[j];
			for (int k = 7; k > -1; k--)
			{
				int diff = 7 - k;
				uint8_t nth_bit = (bool)(bits & (uint8_t(1) << k));
				new_word |= (nth_bit << k);
			}

			if (j != i + 3)
			{
				new_word = new_word << 8;
			}
		}

		words.push_back(new_word);
	}

	return 0;
}

int SHA256::ComputeHash()
{
	uint32_t block_num = static_cast<uint32_t>(words.size()) / 16;
	for (int offset = 0 ; offset < block_num ; offset++)
	{
		std::vector<word> schedule;

		for (int i = offset * 16; i < (offset + 1) * 16; i++)
		{
			schedule.push_back(words[i]);
		}

		for (int i = 16; i < 64; i++)
		{
			schedule.push_back(Sigma1(schedule[i - 2]) + schedule[i - 7] + Sigma0(schedule[i - 15]) + schedule[i - 16]);
		}

		word a = h_[0];
		word b = h_[1];
		word c = h_[2];
		word d = h_[3];
		word e = h_[4];
		word f = h_[5];
		word g = h_[6];
		word h = h_[7];

		for (int i = 0; i < 64; i++)
		{
			word T1 = h + Sum1(e) + Ch(e, f, g) + K[i] + schedule[i];
			word T2 = Sum0(a) + Maj(a, b, c);
			h = g;
			g = f;
			f = e;
			e = d + T1;
			d = c;
			c = b;
			b = a;
			a = T1 + T2;
		}

		
		h_[0] += a;
		h_[1] += b;
		h_[2] += c;
		h_[3] += d;
		h_[4] += e;
		h_[5] += f;
		h_[6] += g;
		h_[7] += h;
		
	}
	return 0;
}

int SHA256::Hash(const std::string& filename)
{
	if (ReadFile(filename))
	{
		return 1;
	}
	
	AddPadding();
	CreateWords();
	ComputeHash();

	for (int i = 0; i < 8; i++)
	{
		std::cout << std::hex << h_[i] << " ";
	}

	return 0;
}


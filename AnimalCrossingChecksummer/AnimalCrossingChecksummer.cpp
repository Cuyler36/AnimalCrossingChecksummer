#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

typedef unsigned short ushort;
inline ushort swap(ushort x) {
	return ((x >> 8) | (x << 8));
}

int main(int argc, char *argv[])
{
	string path;
	if (argc < 2) {
		cout << "Enter the path to the Animal Crossing: Population Growing file:\n";
		getline(cin, path);
	}
	else {
		path = argv[1];
	}
	
	cout << path << "\n";
	FILE *f;
	errno_t error;
	if ((error = fopen_s(&f, path.c_str(), "r+b")) == 0 && f != NULL) {
		ushort checksum = 0;
		fseek(f, 0x26040, SEEK_SET);
		ushort currentShort = 0;
		int position = 0x26040;

		while (position < 0x4C040){
			fread(&currentShort, 1, sizeof(currentShort), f);
			currentShort = swap(currentShort);

			// Skip including the checksum in the calculation
			if (position == 0x26052) {
				position = ftell(f);
				continue;
			}

			checksum += currentShort;
			position = ftell(f);
		}

		checksum = (ushort)-checksum;

		cout << "Calculated checksum: 0x" << hex << checksum << "\n";
		fseek(f, 0x26052, SEEK_SET);
		ushort swappedChecksum = swap(checksum);
		fwrite(&swappedChecksum, sizeof(checksum), 1, f);

		cout << "Successfully updated the file's checksum! Press any key to close the window.\n";
	}
	else {
		cout << "Failed to open the .gci file! Press any key to close the window.\n";
	}

	if (f != NULL) {
		fclose(f);
	}

	cin.get();

    return 0;
}


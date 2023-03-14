#include "Decoder.h"
#include "OMSI_utils.h"

int main() {
	//get input args with boost
	//add extension if necessary

	if (!checkFile("einm_see.o3d"))
		return -1;

	if (!decodeFile("einm_see.o3d"))
		return -1;

	return 0;
}
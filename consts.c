/* (nonportable) C code
 */
int main(argc, argv)
	int argc;
	char **argv;
{
	const a = 1;
	const char *c = argv[0];
	char * const x = argv[0];

	c = (const char*)0xBADBEEF;
	x = (char*)0xFEEDBAC;

	*c = 0x0;
	*x = 0xF;

	return 0;
}


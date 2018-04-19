
char *strcat_l(char *dst, const char *src)
{
	char *saved_retv = dst;
	while (*dst) dst++;
	while (*dst++ = *src++);
	return saved_retv;
}


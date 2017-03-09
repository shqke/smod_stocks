#if defined _stocks_string_included
 #endinput
#endif
#define _stocks_string_included

stock int ExtractFirstNumber(const char[] str, int maxlength)
{
	static char numeric[10];
	static int w;
	if (maxlength > sizeof(numeric)) {
		maxlength = sizeof(numeric);
	}
	
	w = 0;
	for (int i = 0; i < maxlength; i++) {
		if (!IsCharNumeric(str[i])) {
			if (w) break;
			continue;
		}
		
		numeric[w++] = str[i];
	}
	
	numeric[w] = 0;
	
	return StringToInt(numeric);
}

#define IS_BETWEEN_INCL(%0,%1,%2) ( %0 >= %1 && %0 <= %2 )

stock void LowerCase(char[] str)
{
	int index = strlen(str);
	while (index --) {
		if (IS_BETWEEN_INCL(str[index], 'A', 'Z')) {
			str[index] |= 0x20;
		}
	}
}

stock void UpperCase(char[] str)
{
	int index = strlen(str);
	while (index --) {
		if (IS_BETWEEN_INCL(str[index], 'a', 'z')) {
			str[index] &= ~0x20;
		}
	}
}

stock bool IsStringAlphaNumeric(const char[] str)
{
	int index = strlen(str);
	while (index --) {
		if (!IS_BETWEEN_INCL(str[index], 'A', 'Z') && !IS_BETWEEN_INCL(str[index], 'a', 'z') && !IS_BETWEEN_INCL(str[index], '0', '9')) {
			return false;
		}
	}
	
	return true;
}

stock bool IsStringNumeric(const char[] str)
{
	int index = strlen(str);
	while (index --) {
		if (!IS_BETWEEN_INCL(str[index], '0', '9')) {
			return false;
		}
	}
	
	return true;
}

#undef IS_BETWEEN_INCL

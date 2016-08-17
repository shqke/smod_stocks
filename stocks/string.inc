#if defined _stocks_string_included
 #endinput
#endif
#define _stocks_string_included

#define RndElementFromArrayByIndex(%0,%1) %0[GetRandomInt(0, sizeof(%0) - 1)]

stock void LowerCase(char[] string)
{
	int length = strlen(string);
	for (int i = 0; i < length; i++) {
		if (string[i] >= 'A' && string[i] <= 'Z') {
			string[i] += 0x20;
		}
	}
}

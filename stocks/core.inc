#if defined _stocks_core_included
 #endinput
#endif
#define _stocks_core_included

enum TextMsgMode
{
	TextMsg_Notify = 1,
	TextMsg_Console,
	TextMsg_Chat,
	TextMsg_Center
};

stock void PrintRawTextToAll( TextMsgMode mode, const char[] message, 
												const char[] arg1 = NULL_STRING, const char[] arg2 = NULL_STRING, 
												const char[] arg3 = NULL_STRING, const char[] arg4 = NULL_STRING )
{
	BfWrite bf = view_as<BfWrite>( StartMessageAll( "TextMsg", USERMSG_RELIABLE ) );
	
	bf.WriteByte(view_as<int>( mode ));
	bf.WriteString(message);
	
	bf.WriteString(arg1);
	bf.WriteString(arg2);
	bf.WriteString(arg3);
	bf.WriteString(arg4);
	
	EndMessage();
}
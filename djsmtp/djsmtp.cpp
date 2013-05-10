// djsmtp.cpp : Defines the exported functions for the DLL application.
//
#include "CSmtp.h"
#include "base64.h"
#include <iostream>
#include "djsmtp.h"

extern "C"
{
CSmtp mail;


// This is an example of an exported function.
DJSMTP_API int fndjsmtp(void)
{
	return 42;
}

DJSMTP_API void SMTP_SetServer(char* s,int SrvPort, bool authenticate)
{
	mail.SetSMTPServer(s,SrvPort,authenticate);
}

DJSMTP_API void SMTP_SetSecurityType(SMTP_SECURITY_TYPE stype)
{
	mail.SetSecurityType(stype);
}
DJSMTP_API void SMTP_Login(char* l,char* p, char* s)
{
	mail.SetLogin(l);
	mail.SetPassword(p);
	mail.SetSenderMail(s);
}
DJSMTP_API void SMTP_SetLogin(char* s)
{
	mail.SetLogin(s);
	
}
DJSMTP_API void SMTP_SetPassword(char* s)
{
	mail.SetPassword(s);
}
DJSMTP_API void SMTP_SetSenderName(char* s)
{
	mail.SetSenderName(s);
}


DJSMTP_API void SMTP_SetSenderMail(char* s)
{
	mail.SetSenderMail(s);
}
DJSMTP_API void SMTP_SetReplyTo(char* s)
{
	mail.SetReplyTo(s);
}
DJSMTP_API void SMTP_SetSubject(char* s)
{
	mail.SetSubject(s);
}
DJSMTP_API void SMTP_AddRecipient( char *email,  char *name)
{
	mail.AddRecipient(email,name);
}
DJSMTP_API void SMTP_SendTo( char *email,  char *name)
{
	mail.AddRecipient(email,name);
}

DJSMTP_API void SMTP_SetPriority( CSmptXPriority priority)
{
	mail.SetXPriority(priority);
}
DJSMTP_API void SMTP_SetMailerName( char* s)
{
	mail.SetXMailer(s);
}
DJSMTP_API void SMTP_AddMsgLine( char* s)
{
	mail.AddMsgLine(s);
}
DJSMTP_API void SMTP_DelMsgLine( int num)
{
	mail.DelMsgLine(num);
}
DJSMTP_API void SMTP_ModMsgLine( int n,char* s)
{
	mail.ModMsgLine(n,s);
}

DJSMTP_API void SMTP_AddAttachment(char* s)
{
	mail.AddAttachment(s);
}

 char erromsg[255];

DJSMTP_API bool SMTP_Send()
{
	bool bError = false;
    strcpy(erromsg,"OK");
	try
	{
		mail.Send();
    }
	catch(ECSmtp e)
	{
		//std::cout << "Error: " << e.GetErrorText().c_str() << ".\n";

		strcpy(erromsg,e.GetErrorText().c_str());
		//erromsg= e.GetErrorText().c_str();
		//strcoy(
		return false;
	}
	if(!bError)
	{
		strcpy(erromsg,"OK");
		mail.DisconnectRemoteServer();
		return true;
	}
	return true;
}



DJSMTP_API  char* SMTP_GetError( )
{
	return erromsg;
}
DJSMTP_API const char* SMTP_GetLocalHostName( )
{
	return mail.GetLocalHostName();
}

static const std::string base64chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";




 bool isbase64(char c) 
{
  return (isalnum(c) || (c == '+') || (c == '/'));
}

char* base64encode( char * bytes_to_encode) 
{
	int in_len=strlen(bytes_to_encode);
	//std::string ret ;
	char* ret;//=malloc(sizeof(char) * in_len); 
  int i = 0, j = 0;
   char char_array_3[3], char_array_4[4];

  while (in_len--)
	{
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) 
		{
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

   return ret;

}

char* base64decode(char* text) 
{
  int in_len = strlen(text);
  int i = 0, j = 0, in_ = 0;
   char char_array_4[4], char_array_3[3];
 // std::string ret;
    char* ret;//=malloc(sizeof(char) * in_len); 

  while (in_len-- && ( text[in_] != '=') && isbase64(text[in_])) 
	{
    char_array_4[i++] = text[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) 
	{
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) 
			ret += char_array_3[j];
  }

  return ret;
  
}


DJSMTP_API  char* SMTP_EncodeText( char* text)
{
   return base64encode(text);
  // char buffer[255];
 //  strcpy(buffer,result.c_str());
 //  return buffer;
}
DJSMTP_API  char* SMTP_DecodeText(char* text)
{
   return base64decode(text);
   
}



/*

		mail.SetSMTPServer("smtp.gmail.com",587);
		mail.SetSecurityType(USE_TLS);
		mail.SetSMTPServer("smtp.gmail.com",465);
		mail.SetSecurityType(USE_SSL);
		mail.SetSMTPServer("smtp.live.com",25);
		mail.SetSecurityType(USE_TLS);
		mail.SetSMTPServer("smtp.aol.com",587);
		mail.SetSecurityType(USE_TLS);
		mail.SetSMTPServer("plus.smtp.mail.yahoo.com",465);
		mail.SetSecurityType(USE_SSL);

		mail.SetLogin("***");
		mail.SetPassword("***");
  		mail.SetSenderName("User");
  		mail.SetSenderMail("user@domain.com");
  		mail.SetReplyTo("user@domain.com");
  		mail.SetSubject("The message");
  		mail.AddRecipient("friend@domain2.com");
  		mail.SetXPriority(XPRIORITY_NORMAL);
  		mail.SetXMailer("The Bat! (v3.02) Professional");
  		mail.AddMsgLine("Hello,");
		mail.AddMsgLine("");
		mail.AddMsgLine("...");
		mail.AddMsgLine("How are you today?");
		mail.AddMsgLine("");
		mail.AddMsgLine("Regards");
		mail.ModMsgLine(5,"regards");
		mail.DelMsgLine(2);
		mail.AddMsgLine("User");

  		//mail.AddAttachment("../test1.jpg");
  		//mail.AddAttachment("c:\\test2.exe");
		//mail.AddAttachment("c:\\test3.txt");
		mail.Send();
		*/


}
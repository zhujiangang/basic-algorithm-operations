#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool IS_WORD_CHAR(char c)
{
	return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int count_word(const char* text)
{
	/*定义各种状态，我们不关心接受状态，这里可以不用定义。*/
    enum _State
    {
        STAT_INIT,
			STAT_IN_WORD,
			STAT_OUT_WORD,
    }state = STAT_INIT;
	
    int count = 0;
    const char* p = text;
	
    /*在一个循环中，指针从缓冲区头移动缓冲区尾*/
    for(p = text; *p != '\0'; p++)
    {
        switch(state)
        {
		case STAT_INIT:
            {
                if(IS_WORD_CHAR(*p))
                {
					/*指针指向单词字符，状态转换为单词内*/
                    state = STAT_IN_WORD;
                }
                else
                {
					/*指针指向非单词字符，状态转换为单词外*/
                    state = STAT_OUT_WORD;
                }
                break;
            }
		case STAT_IN_WORD:
            {
                if(!IS_WORD_CHAR(*p))
                {
                    /*指针指向非单词字符，状态转换为单词外，增加单词计数*/
                    count++;
                    state = STAT_OUT_WORD;
                }
                break;
            }
		case STAT_OUT_WORD:
            {
                if(IS_WORD_CHAR(*p))
                {
                    /*指针指向单词字符，状态转换为单词内*/
                    state = STAT_IN_WORD;
                }
                break;
            }
		default:break;
        }
    }
	
    if(state == STAT_IN_WORD)
	{
        /*如果由单词内进入接受状态，增加单词计数*/
        count++;
    }
	
    return count;
}

typedef void (*OnTokenFunc)(void* ctx, int index, const char* token);

#define IS_DELIM(c) (strchr(delims, c) != NULL)
int parse_token(const char* text, const char* delims, OnTokenFunc on_token, void* ctx)
{
    enum _State
    {
        STAT_INIT,
			STAT_IN,
			STAT_OUT,
    }state = STAT_INIT;
	
    int   count     = 0;
    char* copy_text = strdup(text);
    char* p         = copy_text;
    char* token     = copy_text;
	
    for(p = copy_text; *p != '\0'; p++)
    {
        switch(state)
        {
		case STAT_INIT:
		case STAT_OUT:
            {
                if(!IS_DELIM(*p))
                {
                    token = p;
                    state = STAT_IN;
                }
                break;
            }
		case STAT_IN:
            {
                if(IS_DELIM(*p))
                {
                    *p = '\0';
                    on_token(ctx, count++, token);
                    state = STAT_OUT;
                }
                break;
            }
		default:break;
        }
    }
	
    if(state == STAT_IN)
    {
        on_token(ctx, count++, token);
    }
	
    on_token(ctx, -1, NULL);
    free(copy_text);
	
    return count;
}

void OnToken(void* ctx, int index, const char* token)
{
	printf("%d: %s\n", index, token);
}

void test_count_word()
{
	int n = 0;
	n = count_word("2010 world cup? Oh, I come!");
	printf("%d\n", n);
}

void test_parse_token()
{
	const char* text = "/usr/lib/qt-3.3/bin:/usr/kerberos/bin:/backup/tools/jdk1.5.0_18/bin/:/usr/lib/ccache:/usr/local/bin:/bin:/usr/bin:/home/lixi";
	int n = parse_token(text, ":/", OnToken, 0);
	printf("%d\n", n);
}

int main(int argc, char* argv[])
{
	test_count_word();
	test_parse_token();
	return 0;
}
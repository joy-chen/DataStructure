#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

class String {
public:
    char *str;

    String(char *src = NULL);

    String(const String &str)
    {
        int len;

        cout << "const string &str" << endl;
        len = strlen(str.str);
        this->str = new char[len + 1];
        strcpy(this->str, str.str);
    }

    String & operator= (const String &str)
    {
        int len;

        cout << "operate = " << endl;
        if (this == &str)
            return *this;

        delete[] this->str;
        len = strlen(str.str);
        this->str = new char[len + 1];
        strcpy(this->str, str.str);

        return *this;
    }

    ~String()
    {
        delete[] str;
    }
};

String::String(char *src)
{
    int len;

    if (NULL == src) {
        str = new char [1];
        str = '\0';
    } else {
        len = strlen(src);
        str = new char [len + 1];
        strcpy(str, src);
    }

}


int main()
{
    char a[] = "1234";

    String s1(a);

    printf("%s\n", s1.str);
    return 0;
}

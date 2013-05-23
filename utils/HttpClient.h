#ifndef _HTTP_CLIENT_H_
#define _HTTP_CLIENT_H_

#include <string>
#include <curl/curl.h>

using namespace std;

#define CURL_TIMEOUT_THRESHOLD		1000


size_t ReceiveData(void *buffer, size_t size,
                   size_t nmemb, void *userp);

class CHttpClient
{
friend size_t ReceiveData(void *buffer, size_t size,
                          size_t nmemb, void *userp);
public:
    static void Initialize();
    static void CleanUp();

public:
    CHttpClient();
    ~CHttpClient();

    const string & Get(const string &url);

private:
    //CHttpClient(const CHttpClient &);
    inline void FillBody(const char *b, size_t s)
    { m_httpBody.append(b, s); }

private:
    CURL   *m_curl;
    string m_httpBody;

private:
    static bool m_curlInitiated;
};

#endif //_HTTP_CLIENT_H_


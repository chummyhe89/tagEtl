#include "HttpClient.h"
#include "PRLogger.h"

bool CHttpClient::m_curlInitiated;

void CHttpClient::Initialize()
{
    curl_global_init(CURL_GLOBAL_ALL);
}

void CHttpClient::CleanUp()
{
    curl_global_cleanup();
}

CHttpClient::CHttpClient()
{
    m_curl = curl_easy_init();

    curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);

    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, ReceiveData);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);

    // set time out
    curl_easy_setopt(m_curl, CURLOPT_TIMEOUT_MS, CURL_TIMEOUT_THRESHOLD);
}

CHttpClient::~CHttpClient()
{
    curl_easy_cleanup(m_curl);
}

const string & CHttpClient::Get(const string &url)
{
    m_httpBody.clear();
    curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());

//    WRITELOG(url);
    curl_easy_perform(m_curl);
//    WRITELOG(string("GET finished"));

    return m_httpBody;
}

size_t ReceiveData(void *buffer, size_t size,
                   size_t nmemb, void *userp)
{
    size_t len = size * nmemb;
    ((CHttpClient *) userp)->FillBody((char *)buffer, len);
    return len; 
}


#include <assert.h>

template <typename T> class Singleton
{
protected:
	static T* m_pSingleton;

public:
	Singleton()
	{
		assert(!m_pSingleton);
		m_pSingleton = static_cast<T*>(this);
	}
	~Singleton()
	{
		assert(m_pSingleton);
		m_pSingleton = 0;
	}
	static T& GetSingleton(void)
	{
		assert(m_pSingleton);
		return *m_pSingleton;
	}
	static T* GetSingletonPtr(void)
	{
		assert(m_pSingleton);
		return m_pSingleton;
	}
private:
	Singleton& operator=(const Singleton&) {return this;}
	Singleton(const Singleton&) {}
};
template <typename T> 
T* Singleton<T>::m_pSingleton;

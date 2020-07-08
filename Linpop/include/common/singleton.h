
#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T>
class Singleton {
public:
    ~Singleton(){}

    static T* getInstance(){
        static T instance;
        return &instance;
    }

protected:
    Singleton(){}
    Singleton(const Singleton<T>&);
    Singleton<T>& operator=(const Singleton<T>);
};

#endif // SINGLETON_H

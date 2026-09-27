#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>


class Observer {
public:
    virtual ~Observer() = default;
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};
class LogSubject {
    std::vector<Observer*> observer;
public:
    void addObserver(Observer* obs) {
        observer.push_back(obs);
    }
    void removeObserver(Observer* obs) {
        observer.erase(std::remove(observer.begin(), observer.end(), obs), observer.end());
    }
    void warning(const std::string& message) const {
        for (auto obs : observer) obs->onWarning(message);
    }
    void error(const std::string& message) const {
        for (auto obs : observer) obs->onError(message);
    }   void fatalError(const std::string& message) const {
        for (auto obs : observer) obs->onFatalError(message);
    }

};

class WarningObserver : public Observer {
public:
    void onWarning(const std::string& message) override {
        std::cout << "Warning (Console): " << message << std::endl;
    }
};
class ErrorObserver : public Observer {
    std::string path;
public:
    explicit ErrorObserver(std::string p) : path(std::move(p)) {}
    void onError(const std::string& message) override {
        std::ofstream file(path, std::ios::app);
        file << "Error (File): " << message << std::endl;
    }
};
class FatalErrorObserver : public Observer {
    std::string path;
public:
    explicit FatalErrorObserver(std::string p) : path(std::move(p)) {}
    void onFatalError(const std::string& message) override {
        std::cout << "FATAL (Console): " << message << std::endl;
        std::ofstream file(path, std::ios::app);
        file << "FATAL (File): " << message << std::endl;
    }
};
int main() {
    LogSubject subject;

    WarningObserver warnObs;
    ErrorObserver errObs("errors.log");
    FatalErrorObserver fatalObs("fatal.log");

    subject.addObserver(&warnObs);
    subject.addObserver(&errObs);
    subject.addObserver(&fatalObs);

    subject.warning("Low disk space");
    subject.error("Connection lost");
    subject.fatalError("System crash");

    return 0;
}